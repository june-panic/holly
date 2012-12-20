#pragma once

#include "vorbis/vorbisenc.h"

class CHolly_Vorbis_Audio : public virtual Holly::CBaseAudio
{
	public:

		CHolly_Vorbis_Audio();

		const char* GetName(){ return "Vorbis"; }
		const char* GetVersion(){ return "1.0"; }
		const char* GetExtension(){ return ".ogg"; }
		const char* GetCopyright(){ return "Xiph.Org Foundation"; }
		const char* GetCaveats(){ return ""; }

		bool Start();
		bool End();

		void AddSamples( unsigned int iNumSamples, void* pData, unsigned int iBits, unsigned int iChannels );

		void Delete();
		

	protected:

		bool WriteHeader();

		vorbis_info			m_Info;
		vorbis_dsp_state	m_State;
		vorbis_block		m_Block;

};
