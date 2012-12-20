
namespace Holly 
{
	class IContainer;
	class IVideo;
	class IAudio;
	class IMovie;

	class IMovieElement 
	{
	public:

		virtual void	Init( IMovie* pVideo ) = 0;

		// Information
		virtual const char* GetName() = 0;
		virtual const char* GetVersion() = 0;
		virtual const char* GetExtension() = 0;
		virtual const char* GetCopyright() = 0;
		virtual const char* GetCaveats() = 0;

		virtual bool	Start() = 0;
		virtual bool	End() = 0;

		virtual void	Delete() = 0;

		virtual IContainer*			ToContainer() = 0;
		virtual IVideo*				ToVideo() = 0;
		virtual IAudio*				ToAudio() = 0;

		virtual unsigned long long	GetTime() = 0;

		virtual const char*		LastError() = 0;

	};

	class IContainer : public virtual IMovieElement
	{
	public:

		virtual void AddFrame( IVideo* pVideo, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame ) = 0;
		virtual void AddSample( IAudio* pAudio, void* pBuffer, unsigned int iSize, unsigned long long iTime, bool bKeyFrame ) = 0;

		//
		// The container may typecast pBuffer for its unique purposes - in which case 
		// iSize should be 0. Otherwise iSize should be the size of the generic data being passed.
		//
		virtual void WriteHeader( IMovieElement* pElement, void* pBuffer, unsigned int iSize = 0 ) = 0;

		//
		// Returns the type of container this is. Codecs may check 
		// what the container is and make tailor themselves to that 
		// type of container. May return MKV, AVI, OGG etc
		//
		virtual unsigned char				GetType() = 0;
	};

	class IVideo : public virtual IMovieElement
	{
	public:

		//
		// Sets the width and height of the video. The container will look
		// at the size of the video and set itself to that size.
		//
		virtual void SetSize( unsigned int iWidth, unsigned int iHeight ) = 0;

		//
		// FPS may or may not be used. Most of the time the delta you're passing to
		// AddFrame will determine what FPS you get.
		//
		virtual void SetFPS( float fFPS ) = 0;

		//
		// Target bitrate in kilobits per second.
		//
		// Here's some approximations to help you fill this in.
		//
		//	16				= Video Phone
		//	256				= Video Conference
		//	1536			= VCD
		//	3584			= Standard Def TV
		//	10035			= DVD
		//	15360			= HDTV
		//	30105			= HD DVD (max)
		//	40960			= Blu-ray (max)
		//	
		virtual void SetBitRate( unsigned int iBitrateKiloBits ) = 0;

		//
		// Some codecs take a deadline. This is the number of milliseconds
		// to spend encoding the frame. If you're a `live` encoding application
		// you're going to want to send this to 1. The more time it takes
		// on a frame the more compressed it can become.
		//
		virtual void SetDeadline( unsigned int iDeadline ) = 0;

		//
		// Adds a single frame to the movie. pData should point to a buffer full of colormode
		//
		virtual void AddFrame( float fDelta, Holly::ColorMode::ENUM colormode, void* pData, unsigned int iFlags = 0 ) = 0;

		// Accessors
		virtual float FPS() = 0;
		virtual unsigned int Width() = 0;
		virtual unsigned int Height() = 0;
		virtual unsigned int BitRate() = 0;
		virtual unsigned int Deadline() = 0;
	};

	class IAudio : public virtual IMovieElement
	{
	public:

		virtual void Setup( unsigned int iChannels, unsigned int iSampleRate ) = 0;
		virtual void AddSamples( unsigned int iNumSamples, void* pData, unsigned int iBits = 8, unsigned int iChannels = 1 ) = 0;

		virtual unsigned int Channels() = 0;
		virtual unsigned int SampleRate() = 0;


	};

}