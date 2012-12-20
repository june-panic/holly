#pragma once

#include "theora/theoraenc.h"

class CHolly_Theora_Video : public virtual Holly::CBaseVideo
{
	public:

		CHolly_Theora_Video();
		~CHolly_Theora_Video();

		const char* GetName(){ return "Theora"; }
		const char* GetVersion(){ return "1.0"; }
		const char* GetExtension(){ return ".ogv"; }
		const char* GetCopyright(){ return ""; }
		const char* GetCaveats(){ return "Prefers YUV"; }

		bool Start();
		bool End();

		void AddFrame( float fDelta, Holly::ColorMode::ENUM colormode, void* pData, unsigned int iFlags );

		virtual void Delete();

	protected:

		void WriteHeader();

		void ProcessFrame( bool bLast );

		void SetupFrame();
		void FreeFrame();

		th_enc_ctx*			m_Encoder;
		th_ycbcr_buffer		m_Frame;
		bool				m_bFrameWaiting;

};
