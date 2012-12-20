
#include "Holly/Holly.hpp"
#include "CHolly.hpp"
#include "CMovie.hpp"
#include "CMovieEncoder.hpp"


CVideoEncoder::CVideoEncoder()
{
	m_pWriter = NULL;
}

CVideoEncoder::~CVideoEncoder()
{
	
}

void CVideoEncoder::Delete()
{
	if ( m_pWriter )
	{
		m_pWriter->Delete();
	}

	delete this;
}

bool CVideoEncoder::Start()
{
	if ( m_Container && !m_Container->Start() )
	{
		SetError( m_Container->LastError() );
		return false;
	}

	if ( m_Video && !m_Video->Start() )
	{
		SetError( m_Video->LastError() );
		return false;
	}

	if ( m_Audio && !m_Audio->Start() )
	{
		SetError( m_Audio->LastError() );
		return false;
	}

	return true;
}

bool CVideoEncoder::End()
{
	if ( m_Audio && !m_Audio->End() )
		return false;

	if ( m_Video && !m_Video->End() )
		return false;

	if ( m_Container && !m_Container->End() )
		return false;

	if ( m_pWriter )
	{
		m_pWriter->Finish();
		m_pWriter->Delete();
		m_pWriter = NULL;
	}
	
	return true;
}

bool CVideoEncoder::SetContainer( const char* strContainer )
{
	Holly::IMovieElement* pElement = m_pHolly->CreateElement( strContainer );
	if ( !pElement ) return false;

	if ( !pElement->ToContainer() )
	{
		pElement->Delete();
		return true;
	}

	m_Container = pElement->ToContainer();
	m_Container->Init( this );
	return true;
}

bool CVideoEncoder::SetVideo( const char* strCodec, unsigned int iTrackNum )
{
	Holly::IMovieElement* pElement = m_pHolly->CreateElement( strCodec );
	if ( !pElement ) return false;

	if ( !pElement->ToVideo() )
	{
		pElement->Delete();
		return true;
	}

	m_Video = pElement->ToVideo();
	m_Video->Init( this );
	return true;
}

bool CVideoEncoder::SetAudio( const char* strCodec, unsigned int iTrackNum )
{
	Holly::IMovieElement* pElement = m_pHolly->CreateElement( strCodec );
	if ( !pElement ) return false;

	if ( !pElement->ToAudio() )
	{
		pElement->Delete();
		return true;
	}

	m_Audio = pElement->ToAudio();
	m_Audio->Init( this );
	return true;
}