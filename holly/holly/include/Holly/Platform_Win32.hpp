
#ifdef _WIN32 
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <windows.h>
#endif 


namespace Holly 
{
	namespace Platform
	{
		inline const char* DefaultLibraryLocation(){ return ""; }
		inline const char* LibraryPretension(){ return ""; }
		inline const char* LibraryExtension(){ return ".dll"; }

		inline void* LoadModule( const char* ModuleName, std::string& Error )
		{
			HINSTANCE inst = LoadLibraryA( ModuleName );
			if ( inst ) return (void*)inst;

			char* strError = NULL;
			FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (char*)&strError, 0, NULL);
			Error = strError;
			LocalFree( strError );

			return NULL;
		}

		inline void* FindFunction( void* pModule, const char* Name )
		{
			return (void*)GetProcAddress( (HINSTANCE)pModule, Name );
		}

		inline void FreeLibrary( void* pModule )
		{
			FreeLibrary( (HINSTANCE)pModule );
		}
	}
}
#ifdef HOLLY_STATIC 
	#define HOLLY_EXPORT
#else 
	#define HOLLY_EXPORT extern "C" __declspec(dllexport)
#endif 
