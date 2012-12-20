
#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"
#include "Holly_Module/SampleQuery.hpp"
#include "Holly_Vorbis_Audio.h"
#include <fstream>

#include <cstdlib>
#include <cstring>

CHolly_Vorbis_Audio::CHolly_Vorbis_Audio()
{

}

void CHolly_Vorbis_Audio::Delete()
{
	delete this;
}

bool CHolly_Vorbis_Audio::Start()
{
	vorbis_info_init( &m_Info );

	float fQuality = 1.0f; // TODO! CONFIGURABLE

	if ( vorbis_encode_init_vbr( &m_Info, m_iChannels, m_iSampleRate, fQuality ) != 0 )
	{
		SetError( "Audio couldn't init vorbis codec" );
		return false;
	}

	if ( vorbis_analysis_init( &m_State, &m_Info ) != 0 )
	{
		SetError( "Audio couldn't init vorbis analysis" );
		return false;
	}

	if ( vorbis_block_init( &m_State, &m_Block) != 0 )
	{
		SetError( "Audio couldn't init vorbis block" );
		return false;
	}	

	if  ( !WriteHeader() )
		return false;

	return true;
}

bool CHolly_Vorbis_Audio::WriteHeader()
{
	vorbis_comment vc;
	vorbis_comment_init(&vc);

	ogg_packet header;
	ogg_packet header_comm;
	ogg_packet header_code;

	if ( vorbis_analysis_headerout( &m_State, &vc, &header, &header_comm, &header_code ) != 0 )
	{
		SetError( "Audio couldn't write vorbis header" );
		return false;
	}

	//
	// MKV wants the audio header in a specific format
	//
	// See From http://matroska.org/technical/specs/codecid/index.html
	//
	if ( Movie()->Container()->GetType() == Holly::ContainerTypes::MKV )
	{
		char* pData = (char*) malloc( 3 + header.bytes + header_comm.bytes + header_code.bytes );

		if ( header.bytes > 255 )
		{
			SetError( "Audio couldn't write vorbis header (too big)" );
			return false;
		}

		if ( header_comm.bytes > 255 )
		{
			SetError( "Audio couldn't write vorbis header_comm (too big)" );
			return false;
		}

		pData[0] = 2;					// Byte 1 (see above)
		pData[1] = header.bytes;		// bytes 2..
		pData[2] = header_comm.bytes;	// ..n

		unsigned int pos = 3;
		memcpy( pData + pos, header.packet, header.bytes );
		pos += header.bytes;
		memcpy( pData + pos, header_comm.packet, header_comm.bytes );
		pos += header_comm.bytes;
		memcpy( pData + pos, header_code.packet, header_code.bytes );
		pos += header_code.bytes;

		Encoder()->Container()->WriteHeader( this, pData, pos );

		free( pData );
	}

	if ( Movie()->Container()->GetType() == Holly::ContainerTypes::OGG )
	{
		Encoder()->Container()->WriteHeader( this, &header );
		Encoder()->Container()->WriteHeader( this, &header_comm );
		Encoder()->Container()->WriteHeader( this, &header_code );
	}

	vorbis_comment_clear(&vc);
	return true;
}



void CHolly_Vorbis_Audio::AddSamples( unsigned int iNumSamples, void* pData, unsigned int iBits, unsigned int iChannels )
{
	unsigned char ContainerType = Movie()->Container()->GetType();

	float **buffer = vorbis_analysis_buffer( &m_State, iNumSamples );

	Holly::SampleQuery sampler( pData, iBits, iChannels );

	unsigned char* pInput = (unsigned char*) pData;

	unsigned int iPos = 0;
	for ( unsigned int i=0; i < iNumSamples; i++ )
	{
		for ( unsigned int j=0; j < Channels(); j++ )
		{
			buffer[j][i] = sampler.GetFloatSample( i, j );
		}
	}

	vorbis_analysis_wrote( &m_State, iNumSamples );

	static unsigned long long i = 0;
	unsigned long long GranuleScale = SampleRate() / Channels();

	ogg_packet ogg_pkg;
	while ( vorbis_analysis_blockout( &m_State, &m_Block ) > 0 )
	{
		if ( vorbis_analysis( &m_Block, NULL ) != 0 )
			return;

		if ( vorbis_bitrate_addblock( &m_Block ) != 0 )
			return;

		while ( vorbis_bitrate_flushpacket( &m_State, &ogg_pkg) > 0 )
		{
			SetTime( ogg_pkg.granulepos * GranuleScale );

			if ( ContainerType == Holly::ContainerTypes::MKV )
			{
				Encoder()->Container()->AddSample( this, ogg_pkg.packet, ogg_pkg.bytes, GetTime(), false );
			}

			if ( ContainerType == Holly::ContainerTypes::OGG )
			{
				Encoder()->Container()->AddSample( this, (void*)&ogg_pkg, 0, 0, false );
			}
		}
	}

}

bool CHolly_Vorbis_Audio::End()
{
	vorbis_block_clear( &m_Block );
	vorbis_dsp_clear( &m_State );
	vorbis_info_clear( &m_Info );
	return true;
}

