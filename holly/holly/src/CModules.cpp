
#include "Holly/Holly.hpp"
#include "CModules.hpp"
#include "Errors.hpp"

void CModules::SetLocation( const char* strName )
{
	m_strLocation = strName;
}

bool CModules::Load( const char* strName )
{
	void* pLibrary = LoadModule( (m_strLocation + Holly::Platform::LibraryPretension() + "holly_" + strName + Holly::Platform::LibraryExtension()).c_str() );
	
	if ( !pLibrary ) pLibrary = LoadModule( strName );
	if ( !pLibrary ) return false;

	void* pFunction = Holly::Platform::FindFunction( pLibrary, "Holly_Module_Dynamic" );
	if ( !pFunction )
	{
		Holly::Errors::Add( "Invalid Module" );
		Holly::Platform::FreeLibrary( pLibrary );
		return false;
	}

	Module mod;
		mod.pInstance = pLibrary;
		mod.pModuleQuery = (fnModuleQuery)pFunction;

	m_Modules[ strName ] = mod;

	return true;
}

Holly::IMovieElement* CModules::CreateElement( const char* strName )
{
	for ( Module::Map::iterator mod = m_Modules.begin(); mod != m_Modules.end(); mod++ )
	{
		Holly::IMovieElement* pProcessor = mod->second.pModuleQuery( strName );
		if ( pProcessor ) return pProcessor;
	}

	return NULL;
}

void CModules::UnloadAll()
{
	for ( Module::Map::iterator mod = m_Modules.begin(); mod != m_Modules.end(); mod++ )
	{
		if ( !mod->second.pInstance ) continue;

		Holly::Platform::FreeLibrary( mod->second.pInstance );
	}

	m_Modules.clear();
}

void* CModules::LoadModule( const char* strName )
{
	std::string Error;

	void* pModule = Holly::Platform::LoadModule( strName, Error );

	if ( !pModule )
	{
		Holly::Errors::Add( ("[" + std::string(strName) + "] " + Error).c_str() );
	}

	return pModule;
}

void CModules::AddProvider( const char* strName, fnModuleQuery fn )
{
	Module mod;
		mod.pInstance = NULL;
		mod.pModuleQuery = fn;

	m_Modules[ strName ] = mod;
}