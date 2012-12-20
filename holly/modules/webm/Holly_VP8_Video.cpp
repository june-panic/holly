
#include <cstring>

#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"
#include "Holly_VP8_Video.h"

CHolly_VP8_Video::CHolly_VP8_Video()
{
}


void CHolly_VP8_Video::Delete()
{
	delete this;
}

bool CHolly_VP8_Video::Start()
{
	if ( vpx_codec_enc_config_default( vpx_codec_vp8_cx(), &m_Config, 0 ) > 0 )
	{
		SetError( "Video couldn't load vp8 defaults" );
		return false;
	}

	// Add our settings
	m_Config.g_threads				= 4;
	m_Config.rc_target_bitrate		= BitRate(); // TODO!!!
	m_Config.g_w					= Width();       
	m_Config.g_h					= Height();    
	m_Config.rc_dropframe_thresh	= 0;
	m_Config.rc_min_quantizer		= 1;	// ALLOW ACCESS?
	m_Config.rc_max_quantizer		= 63;	// ALLOW ACCESS?
	m_Config.g_timebase.num			= 1;
	m_Config.g_timebase.den			= FPS();
	m_Config.g_pass					= VPX_RC_ONE_PASS;	// ALLOW ACCESS???
	m_Config.rc_end_usage			= VPX_VBR;			// ALLOW ACCESS????

	if ( m_Config.g_w < 16 || m_Config.g_w%2 || m_Config.g_h < 16 || m_Config.g_h%2 ) 
	{
		SetError( "Video width or height aren't appropriate" );
		return false;
	}

	if( !vpx_img_alloc( &m_Frame, VPX_IMG_FMT_YV12, Width(), Height(), 1 ) )
	{
		SetError( "Video couldn't allocate image buffer" );
		return false;
	}

	if ( vpx_codec_enc_init( &m_Codec, vpx_codec_vp8_cx(), &m_Config, 0 ) )
	{
		SetError( "Video couldn't initialize the vp8 codec" );
		return false;
	}

	SetTime( 0 );

	return true;
}

bool CHolly_VP8_Video::End()
{
	vpx_img_free( &m_Frame );
	vpx_codec_destroy( &m_Codec );

	return true;
}

void CHolly_VP8_Video::AddFrame( float fDelta, Holly::ColorMode::ENUM colormode, void* pData, unsigned int iFlags )
{
	unsigned long long iStartTime = GetTime();
	unsigned long long iFrameTime = AddFrameMS( fDelta );

	//
	// If we're not in YUV mode then we need to convert to YUV.
	// Which is slower, so ideally YUV will be passed.
	//
	if ( colormode != Holly::ColorMode::YUV )
	{
		Movie()->Utility()->Convert( pData, Width(), Height(), colormode, Holly::ColorMode::YUV, m_Frame.planes[0], m_Frame.planes[1], m_Frame.planes[2], iFlags );
	}
	else
	{
		if ( iFlags & Holly::Flags::Flip_Y )
		{
			// TODO: We need to flip the data here instead of just copying it!	
		}

		memcpy( (void*)m_Frame.planes[0], pData, (Width() * Height()) * 3 / 2 );
	}

	vpx_codec_encode( &m_Codec, &m_Frame, iStartTime, (unsigned long)iFrameTime, 0, Deadline() );

	vpx_codec_iter_t iter = NULL;
	const vpx_codec_cx_pkt_t *pkt;

	Holly::IContainer* pContainer = Encoder()->Container();
	while( (pkt = vpx_codec_get_cx_data( &m_Codec, &iter ) ) ) 
	{
		if ( pkt->kind == VPX_CODEC_CX_FRAME_PKT )
		{
			pContainer->AddFrame( this, pkt->data.frame.buf, pkt->data.frame.sz, pkt->data.frame.pts, pkt->data.frame.flags & VPX_FRAME_IS_KEY );
		}
	}
}
