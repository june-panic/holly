
namespace Holly 
{
	class CBaseElement : public virtual Holly::IMovieElement
	{
		public:

			CBaseElement()
			{
				m_pMovie = NULL;
				SetTime( 0 );
			}

			virtual void Init( Holly::IMovie* pVideo )
			{ 
				m_pMovie = pVideo; 
			}

			const char* GetName(){ return ""; }
			const char* GetVersion(){ return ""; }
			const char* GetExtension(){ return ""; }
			const char* GetCopyright(){ return ""; }
			const char* GetCaveats(){ return ""; }

			virtual bool Start(){ return true; };
			virtual bool End(){ return true; };

			virtual Holly::IContainer*		ToContainer(){ return NULL; }
			virtual Holly::IVideo*			ToVideo(){ return NULL; }
			virtual Holly::IAudio*			ToAudio(){ return NULL; }

			virtual Holly::IMovie*			Movie(){ return m_pMovie; }
			virtual Holly::IMovieEncoder*	Encoder(){ return Movie()->Encoder(); }

			unsigned long long	GetTime(){ return m_Time; }
			void	SetTime( unsigned long long t ){ m_Time = t; }

			const char* LastError(){ return m_LastError.c_str(); }
			void SetError( const char* str ){ m_LastError = str; }

		protected:

			Holly::IMovie* m_pMovie;
			unsigned long long m_Time;

			std::string		m_LastError;
	};

	class CBaseContainer : public Holly::IContainer, public CBaseElement
	{
		public:

			virtual Holly::IContainer*		ToContainer(){ return this; }

		protected:

	};

	class CBaseVideo : public Holly::IVideo, public CBaseElement
	{
		public:

			CBaseVideo()
			{
				SetSize( 1024, 768 );
				SetFPS( 30.0f );
				SetBitRate( 1024 );
				SetDeadline( 1000 );
			}

			virtual Holly::IVideo*			ToVideo(){ return this; }

			unsigned int Width(){ return m_iWidth; }
			unsigned int Height(){ return m_iHeight; }
			float FPS(){ return m_fFPS; }
			unsigned int BitRate(){ return m_iBitRate; }
			unsigned int Deadline(){ return m_iDeadline; }

			void SetSize( unsigned int iWidth, unsigned int iHeight )
			{
				m_iWidth = iWidth;
				m_iHeight = iHeight;
			}

			void SetFPS( float fFPS )
			{
				m_fFPS = fFPS;
			}

			void SetBitRate( unsigned int iBitrate )
			{
				m_iBitRate = iBitrate;
			}

			void SetDeadline( unsigned int iDeadline )
			{
				m_iDeadline = iDeadline;
			}

			unsigned long long	AddFrameMS( float fTimeInSeconds )
			{
				unsigned long long ms = fTimeInSeconds * 1000000.0f; // TODO: This value needs to come from the encoder!
				ms *= 1000ULL; 
				m_Time += ms;
				return ms;
			}


		protected:

			float			m_fFPS;
			unsigned int	m_iWidth;
			unsigned int	m_iHeight;
			unsigned int	m_iBitRate;
			unsigned int	m_iDeadline;

	};

	class CBaseAudio : public Holly::IAudio, public CBaseElement
	{
		public:

			CBaseAudio()
			{
			}

			virtual void Setup( unsigned int iChannels, unsigned int iSampleRate )
			{
				m_iChannels = iChannels;
				m_iSampleRate = iSampleRate;
			}

			virtual Holly::IAudio*			ToAudio(){ return this; }

			virtual unsigned int Channels(){ return m_iChannels; }
			virtual unsigned int SampleRate(){ return m_iSampleRate; }

		protected:

			unsigned int	m_iChannels;
			unsigned int	m_iSampleRate;

	};
}

#ifdef HOLLY_STATIC 
#define HOLLY_MODULE_QUERY( _NAME_, name ) Holly::IMovieElement* Holly_Module_##_NAME_ ( const char* name )
#else 
#define HOLLY_MODULE_QUERY( _NAME_, name ) HOLLY_EXPORT Holly::IMovieElement* Holly_Module_Dynamic( const char* name )
#endif 
