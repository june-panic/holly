
#include <cstring>

#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"
#include "Holly_Theora_Video.h"

CHolly_Theora_Video::CHolly_Theora_Video()
{
	for ( int i=0; i<3; i++ )
	{
		m_Frame[i].data = NULL;
	}
}

CHolly_Theora_Video::~CHolly_Theora_Video()
{
	FreeFrame();
}


void CHolly_Theora_Video::Delete()
{
	delete this;
}

bool CHolly_Theora_Video::Start()
{
	m_bFrameWaiting = false;

	th_info thInfo;
	th_info_init( &thInfo );

	thInfo.frame_width				= Width();
	thInfo.frame_height				= Height();
	thInfo.pic_width				= Width();
	thInfo.pic_height				= Height();
	thInfo.pic_x					= 0;
	thInfo.pic_y					= 0;
	thInfo.colorspace				= TH_CS_ITU_REC_470BG;
	thInfo.pixel_fmt				= TH_PF_420; // TH_PF_444
	thInfo.target_bitrate			= BitRate() * 1024;
	thInfo.quality					= 42;
	thInfo.keyframe_granule_shift	= 6; // default value
	thInfo.fps_numerator			= FPS();
	thInfo.fps_denominator			= 1;
	thInfo.aspect_numerator			= 1;
	thInfo.aspect_denominator		= 1;

	m_Encoder = th_encode_alloc( &thInfo );
	if ( !m_Encoder )
	{
		SetError( "Video couldn't allocate theora encoder" );
		return false;
	}

	th_info_clear( &thInfo );

	SetupFrame();
	WriteHeader();

	return true;
}

void CHolly_Theora_Video::SetupFrame()
{
	FreeFrame();

	unsigned int iPixelSize = sizeof(unsigned char);

	for ( int i=0; i<3; i++ )
	{
		m_Frame[i].width	= Width();
		m_Frame[i].height	= Height();

		if ( i > 0 )
		{
			m_Frame[i].width /= 2;
			m_Frame[i].height /= 2;
		}

		m_Frame[i].stride	= m_Frame[i].width * iPixelSize;
		m_Frame[i].data		= (unsigned char*)malloc( m_Frame[i].width * m_Frame[i].height * iPixelSize );
	}
}

void CHolly_Theora_Video::FreeFrame()
{
	for ( int i=0; i<3; i++ )
	{
		if ( !m_Frame[i].data ) continue;

		free( m_Frame[i].data );
		m_Frame[i].data = NULL;
	}
}

void CHolly_Theora_Video::WriteHeader()
{
	th_comment       tc;
	th_comment_init( &tc );
	tc.vendor = (char *)"Holly";

	ogg_packet header;
	ogg_packet header_comm;
	ogg_packet header_code;

	while ( th_encode_flushheader( m_Encoder, &tc, &header ) > 0 )
	{
		Encoder()->Container()->WriteHeader( this, &header );
	}
}

bool CHolly_Theora_Video::End()
{
	ProcessFrame( true );

	th_encode_free( m_Encoder );
	m_Encoder = NULL;

	FreeFrame();

	return true;
}

void CHolly_Theora_Video::AddFrame( float fDelta, Holly::ColorMode::ENUM colormode, void* pData, unsigned int iFlags )
{
	ProcessFrame( false );

	Movie()->Utility()->Convert( pData, Width(), Height(), colormode, Holly::ColorMode::YUV, m_Frame[0].data, m_Frame[1].data, m_Frame[2].data, iFlags );
	m_bFrameWaiting = true;
}

void CHolly_Theora_Video::ProcessFrame( bool bLast )
{
	if ( !m_bFrameWaiting ) return;

	m_bFrameWaiting = false;
	Holly::IContainer* pContainer = Encoder()->Container();

	if ( th_encode_ycbcr_in( m_Encoder, m_Frame ) != 0 )
	{
		return;
	}

	ogg_packet Packet;

	while ( th_encode_packetout( m_Encoder, bLast ? 1 : 0, &Packet ) > 0 )
	{
		pContainer->AddFrame( this, &Packet, 0, 0, 0 );
	}

	
}
