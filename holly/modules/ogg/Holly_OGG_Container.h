#pragma once

#include "ogg/ogg.h"

class CHolly_OGG_Container : public virtual Holly::CBaseContainer
{
	public:

		CHolly_OGG_Container();

		const char* GetName(){ return "OGG"; }
		const char* GetVersion(){ return "1.0"; }
		const char* GetExtension(){ return ".ogv"; }
		const char* GetCopyright(){ return "Xiph.Org Foundation"; }
		const char* GetCaveats(){ return "Doesn't support variable frame rate"; }
		unsigned char GetType(){ return Holly::ContainerTypes::OGG; }

		bool Start();
		bool End();

		void WriteHeader( Holly::IMovieElement* pElement, void* pBuffer, unsigned int iSize );
		void AddFrame( Holly::IVideo* pVideo, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame );
		void AddSample( Holly::IAudio* pAudio, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame );

		void Delete();

		ogg_stream_state& GetStream( Holly::IMovieElement* pElement );


	protected:

		void PageOut( ogg_stream_state& stream );

		ogg_stream_state	m_StreamVideo;
		ogg_stream_state	m_StreamAudio;

		ogg_page			m_Page;

};
