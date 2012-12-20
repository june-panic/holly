
#include "Holly/Holly.hpp"
#include "CHolly.hpp"
#include "CFileWriter.hpp"
#include "CUtility.hpp"
#include "CMovie.hpp"
#include "CMovieEncoder.hpp"
#include "CModules.hpp"
#include "Errors.hpp"

#include <algorithm>

CHolly::CHolly()
{
	m_Utility = new CUtility();
	m_Modules = new CModules();
}

CHolly::~CHolly()
{
	delete m_Utility;

	m_Modules->UnloadAll();
	delete m_Modules;
}

Holly::IMovieEncoder*	CHolly::CreateEncoder()
{
	CVideoEncoder* pEncoder = new CVideoEncoder();
	pEncoder->Initialize( this );
	return pEncoder;
}

Holly::IModules* CHolly::Modules()
{
	return m_Modules;
}

Holly::IWriter* CHolly::CreateFileWriter( const char* strFileName )
{
	CFileWriter* pWriter = new CFileWriter();
	if ( !pWriter->Open( strFileName ) )
	{
		pWriter->Delete();
		return NULL;
	}

	return pWriter;
}

Holly::IUtility* CHolly::Utility()
{ 
	return m_Utility; 
}

Holly::IMovieElement* CHolly::CreateElement( const char* strName )
{
	std::string strName_Lower = strName;
	std::transform( strName_Lower.begin(), strName_Lower.end(), strName_Lower.begin(), ::tolower );
	Holly::IMovieElement* pElement = m_Modules->CreateElement( strName_Lower.c_str() );
	return pElement;
}

const char* CHolly::LastError()
{
	return Holly::Errors::GetLast();
}