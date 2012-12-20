
namespace Holly 
{
	class IMovieEncoder : public virtual IMovie 
	{
		public:

			// Note: Writer will be deleted with the encoder!
			virtual bool SetWriter( IWriter* pWriter ) = 0; 
			// Internally creates a IWriter that writes to the file and calls SetWriter
			virtual bool SetOutputFile( const char* strFile ) = 0;
			// Returns the file writer
			virtual IWriter* Writer() = 0;

			// The container is what contains the audio and video. MVK, AVI etc
			virtual bool SetContainer( const char* strContainer ) = 0;

			virtual bool SetVideo( const char* strCodec, unsigned int iTrackNum = 0 ) = 0;
			virtual bool SetAudio( const char* strCodec, unsigned int iTrackNum = 0 ) = 0;
			
			virtual bool Start() = 0;
			virtual bool End() = 0;	

			virtual const char* LastError() = 0;
			
	};
}