
#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"



#include "Holly_WEBM_Container.h"

CHolly_WEBM_Container::CHolly_WEBM_Container()
{
	m_iVideo = 0;
	m_iAudio = 0;
	m_bDataWritten = false;
}


void CHolly_WEBM_Container::Delete()
{
	delete this;
}

bool CHolly_WEBM_Container::Start()
{
	m_MuxerOutput.holly_writer = Movie()->Encoder()->Writer();
	m_pSegment = new mkvmuxer::Segment( &m_MuxerOutput );
	m_pSegment->set_mode( mkvmuxer::Segment::kFile );
	m_pSegment->OutputCues( true );

	if ( Movie()->Video() )
	{
		m_iVideo = m_pSegment->AddVideoTrack( Movie()->Video()->Width(), Movie()->Video()->Height() );
	}
	
	if ( Movie()->Audio() )
	{
		m_iAudio = m_pSegment->AddAudioTrack( Movie()->Audio()->Channels(), Movie()->Audio()->SampleRate() );
		mkvmuxer::Track* pTrack = m_pSegment->GetTrackByNumber( m_iAudio );
		mkvmuxer::AudioTrack* pAudioTrack = static_cast<mkvmuxer::AudioTrack*>(pTrack);
		pAudioTrack->set_channels( Movie()->Audio()->Channels() );
		pAudioTrack->set_bit_depth( 16 );
		pAudioTrack->set_sample_rate( Movie()->Audio()->SampleRate() );
	}

	mkvmuxer::SegmentInfo* const info = m_pSegment->GetSegmentInfo();
	info->set_writing_app( "Holly WebM" );
	info->set_duration( 1.0 );

	mkvmuxer::Cues* const cues = m_pSegment->GetCues();
	cues->set_output_block_number( true );

	m_pSegment->CuesTrack( m_iVideo );
	m_pSegment->CuesTrack( m_iAudio );

	m_bDataWritten = false;

	return true;
}

void CHolly_WEBM_Container::WriteHeader( IMovieElement* pElement, void* pBuffer, unsigned int iSize )
{
	if ( pElement->ToAudio() )
	{
		mkvmuxer::Track* pTrack = m_pSegment->GetTrackByNumber( m_iAudio );
		pTrack->SetCodecPrivate( (const mkvmuxer::uint8*)pBuffer, iSize );
	}
}

void CHolly_WEBM_Container::AddFrame( Holly::IVideo* pVideo, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame )
{
	if ( Movie()->Audio() && Movie()->Audio()->GetTime() > 0 )
		iTime = Movie()->Audio()->GetTime();

	m_pSegment->AddFrame( (const unsigned char*) pBuffer, iSize, m_iVideo, iTime, bKeyFrame );
	m_bDataWritten = true;
}

void CHolly_WEBM_Container::AddSample( Holly::IAudio* pAudio, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame )
{
	m_pSegment->AddFrame( (const unsigned char*) pBuffer, iSize, m_iAudio, iTime, bKeyFrame );
	m_iAudioTime = iTime;
	m_bDataWritten = true;
}

bool CHolly_WEBM_Container::End()
{
	if ( m_pSegment )
	{
		// Calling Finalize when there's no data written causes a crash!
		if ( m_bDataWritten )
			m_pSegment->Finalize();

		delete m_pSegment;
		m_pSegment = NULL;
	}

	return true;
}

