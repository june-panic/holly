
#include <dlfcn.h>


namespace Holly 
{
	namespace Platform
	{
		inline const char* DefaultLibraryLocation(){ return "./"; }
		inline const char* LibraryPretension(){ return "lib"; }
		#if defined(__linux__)
			inline const char* LibraryExtension(){ return ".so"; }
		#else 
			inline const char* LibraryExtension(){ return ".dylib"; }
		#endif

		inline void* LoadModule( const char* ModuleName, std::string& Error )
		{
			void* inst = dlopen( ModuleName, RTLD_LAZY );
			if ( inst ) return inst;

			Error = dlerror();
			return NULL;
		}

		inline void* FindFunction( void* pModule, const char* Name )
		{
			return (void*)dlsym( pModule, Name );
		}

		inline void FreeLibrary( void* pModule )
		{
			dlclose( pModule );
		}
	}
}

#ifdef HOLLY_STATIC 
	#define HOLLY_EXPORT
#else 
	#define HOLLY_EXPORT extern "C" 
#endif 
