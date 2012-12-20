#pragma once

#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"


class CHolly_VP8_Video : public virtual Holly::CBaseVideo
{
	public:

		CHolly_VP8_Video();

		const char* GetName(){ return "VP8"; }
		const char* GetVersion(){ return "1.0"; }
		const char* GetExtension(){ return ".vp8"; }
		const char* GetCopyright(){ return "Google"; }
		const char* GetCaveats(){ return "Prefers being fed YUV"; }

		bool Start();
		bool End();

		void AddFrame( float fDelta, Holly::ColorMode::ENUM colormode, void* pData, unsigned int iFlags );

		virtual void Delete();

	protected:

		vpx_codec_enc_cfg_t		m_Config;
		vpx_codec_ctx_t			m_Codec;		
		vpx_image_t				m_Frame;
};
