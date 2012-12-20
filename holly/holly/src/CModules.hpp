
#include <string>
#include <map>

class CModules : public Holly::IModules
{
	public:

		bool Load( const char* strName );
		void AddProvider( const char* strName, fnModuleQuery fn );

		void SetLocation( const char* strName );
		

		Holly::IMovieElement* CreateElement( const char* strName );

		void UnloadAll();

	protected:

		typedef Holly::IMovieElement* (*fnModuleQuery)( const char* strName );

		void* LoadModule( const char* strName );

		std::string	m_strLocation;

		struct Module 
		{
			typedef std::map<std::string, Module> Map;

			void*			pInstance;
			fnModuleQuery	pModuleQuery;
		};

		Module::Map	m_Modules;

};