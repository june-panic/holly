
namespace Holly 
{
	class IHolly;

	#ifdef HOLLY_STATIC
	Holly::IHolly* Get_Static_Holly_Interface( unsigned int iVersion );
	#endif

	class Loader 
	{
			typedef IHolly* (*fnLoader)( unsigned int iVersion );

		public:

			Loader()
			{
				m_LibraryHandle = NULL;
				m_pHolly = NULL;
				
				SetLibraryLocation( Holly::Platform::DefaultLibraryLocation() );
				SetLibraryName( (Holly::Platform::LibraryPretension() + std::string("holly") + Holly::Platform::LibraryExtension()).c_str() );
			}

			~Loader()
			{
				UnloadOurLibrary();
			}

			IHolly*	GetHolly()
			{
				if ( !LoadOurLibrary() )
					return NULL;

				return m_pHolly;
			}

			const char* LastError()
			{
				return m_LastError.c_str();
			}

			//
			// If you're adding a path this MUST end in a slash!
			//
			void SetLibraryLocation( const char* location )
			{
				m_LibraryLocation = location;
			}
			
			void SetLibraryName( const char* name )
			{
				m_LibraryName = name;
			}

		protected:

			inline bool LoadOurLibrary()
			{
				#ifdef HOLLY_STATIC
					m_pHolly = Get_Static_Holly_Interface( Version );
				#else
					if ( m_LibraryHandle ) return true;

					std::string strError;

					m_LibraryHandle = Holly::Platform::LoadModule( (m_LibraryLocation + m_LibraryName).c_str(), strError );

					if ( !m_LibraryHandle )
					{
						m_LastError =  "[" + (m_LibraryLocation + m_LibraryName) + "] " + strError;
						return false;
					}

					fnLoader pFnPointer = (fnLoader)Holly::Platform::FindFunction( m_LibraryHandle, "Get_Holly_Interface" );

					if ( !pFnPointer )
					{
						m_LastError = "[" + (m_LibraryLocation + m_LibraryName) + "] " + "Couldn't find 'Get_Holly_Interface' function";
						UnloadOurLibrary();
						return false;
					}

					m_pHolly = pFnPointer( Version );
					if ( !m_pHolly )
					{
						m_LastError = "[" + (m_LibraryLocation + m_LibraryName) + "] " + "Wrong Version";
						UnloadOurLibrary();
						return false;
					}
				#endif

				return m_pHolly != NULL;
			}

			inline void UnloadOurLibrary()
			{
				if ( !m_LibraryHandle ) return;

				Holly::Platform::FreeLibrary( m_LibraryHandle );
				m_LibraryHandle = NULL;
			}

			void*				m_LibraryHandle;
			IHolly*		m_pHolly;

			std::string			m_LastError;
			std::string			m_LibraryLocation;
			std::string			m_LibraryName;
	};

}
