
#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"



#include "Holly_OGG_Container.h"

CHolly_OGG_Container::CHolly_OGG_Container()
{

}

void CHolly_OGG_Container::Delete()
{
	delete this;
}

bool CHolly_OGG_Container::Start()
{
	ogg_stream_init( &m_StreamVideo, rand() );
	ogg_stream_init( &m_StreamAudio, rand() );

	return true;
}

void CHolly_OGG_Container::PageOut( ogg_stream_state& stream )
{
	while ( ogg_stream_pageout( &stream, &m_Page ) )
	{
		Encoder()->Writer()->Write( m_Page.header, m_Page.header_len );
		Encoder()->Writer()->Write( m_Page.body, m_Page.body_len );
	}
}

void CHolly_OGG_Container::WriteHeader( IMovieElement* pElement, void* pBuffer, unsigned int iSize )
{
	if ( iSize != 0 ) return;

	ogg_packet* pPacket = static_cast<ogg_packet*>(pBuffer);
	ogg_stream_packetin( &GetStream( pElement ), pPacket );

	PageOut( GetStream( pElement ) );
}

void CHolly_OGG_Container::AddFrame( Holly::IVideo* pVideo, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame )
{
	if ( iSize != 0 ) return;

	ogg_packet* pPacket = static_cast<ogg_packet*>(pBuffer);
	ogg_stream_packetin( &GetStream( pVideo ), pPacket );

	PageOut( GetStream( pVideo ) );
}

void CHolly_OGG_Container::AddSample( Holly::IAudio* pAudio, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame )
{
	if ( iSize != 0 ) return;

	ogg_packet* pPacket = static_cast<ogg_packet*>(pBuffer);
	ogg_stream_packetin( &GetStream( pAudio ), pPacket );

	PageOut( GetStream( pAudio ) );
}

bool CHolly_OGG_Container::End()
{
	while ( ogg_stream_flush( &m_StreamAudio, &m_Page ) )
	{
		Encoder()->Writer()->Write( m_Page.header, m_Page.header_len );
		Encoder()->Writer()->Write( m_Page.body, m_Page.body_len );
	}

	while ( ogg_stream_flush( &m_StreamVideo, &m_Page ) )
	{
		Encoder()->Writer()->Write( m_Page.header, m_Page.header_len );
		Encoder()->Writer()->Write( m_Page.body, m_Page.body_len );
	}

	ogg_stream_clear( &m_StreamAudio );
	ogg_stream_clear( &m_StreamVideo );

	return true;
}



ogg_stream_state& CHolly_OGG_Container::GetStream( Holly::IMovieElement* pElement )
{
	if ( pElement->ToAudio() )
		return m_StreamAudio;

	return m_StreamVideo;
}