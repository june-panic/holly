
namespace Holly 
{
	class IModules
	{
		public:

			typedef Holly::IMovieElement* (*fnModuleQuery)( const char* strName );

			// This MUST end in a slash
			virtual void SetLocation( const char* strName ) = 0;

			// 
			virtual bool Load( const char* strName ) = 0;
			virtual void AddProvider( const char* strName, fnModuleQuery fn ) = 0;

			virtual void UnloadAll() = 0;
			
	};
}