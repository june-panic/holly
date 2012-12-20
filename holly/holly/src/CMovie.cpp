
#include "Holly/Holly.hpp"
#include "CHolly.hpp"
#include "CMovie.hpp"

CMovie::CMovie()
{
	m_Video = NULL;
	m_Audio = NULL;
}

CMovie::~CMovie()
{

}

Holly::IContainer* CMovie::Container()
{
	return m_Container;
}

Holly::IVideo* CMovie::Video( unsigned int iTrackNum )
{
	return m_Video;
}

Holly::IAudio* CMovie::Audio( unsigned int iTrackNum )
{
	return m_Audio;
}