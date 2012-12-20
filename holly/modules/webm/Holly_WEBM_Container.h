#pragma once

#include "mkvmuxer/mkvmuxer.hpp"
#include "Holly_MKMuxer_Writer.hpp"

class CHolly_WEBM_Container : public virtual Holly::CBaseContainer
{
	public:

		CHolly_WEBM_Container();

		const char* GetName(){ return "WEBM"; }
		const char* GetVersion(){ return "1.0"; }
		const char* GetExtension(){ return ".webm"; }
		const char* GetCopyright(){ return "Google"; }
		const char* GetCaveats(){ return "The same as MVK. Can only hold VP8 video and Vorbis Audio. Supports variable frame rate"; }
		unsigned char GetType(){ return Holly::ContainerTypes::MKV; }

		bool Start();
		bool End();

		void WriteHeader( IMovieElement* pElement, void* pBuffer, unsigned int iSize );
		void AddFrame( Holly::IVideo* pVideo, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame );
		void AddSample( Holly::IAudio* pAudio, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame );

		void Delete();

	protected:

		unsigned long long		m_iAudioTime;

		mkvmuxer::uint64		m_iVideo;
		mkvmuxer::uint64		m_iAudio;

		mkvmuxer::Segment*		m_pSegment;
		Holly_MKMuxer_Writer	m_MuxerOutput;

		bool					m_bDataWritten;
};
