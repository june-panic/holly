

class CVideoEncoder : public CMovie, public Holly::IMovieEncoder
{
	public:

		CVideoEncoder();
		~CVideoEncoder();
	
		bool SetOutputFile( const char* strFile )
		{
			if ( m_pWriter ) m_pWriter->Delete();
			m_pWriter = m_pHolly->CreateFileWriter( strFile ); 
			return m_pWriter != NULL;
		}

		bool SetWriter( Holly::IWriter* pWriter )
		{
			if ( m_pWriter ) m_pWriter->Delete();
			m_pWriter = pWriter;
			return m_pWriter != NULL;
		}

		Holly::IWriter* Writer(){ return m_pWriter; }

		Holly::IMovieEncoder* Encoder(){ return this; }
		bool SetContainer( const char* strContainer );
		bool SetVideo( const char* strCodec, unsigned int iTrackNum = 0 );
		bool SetAudio( const char* strCodec, unsigned int iTrackNum = 0 );

		bool Start();
		bool End();	

		void Delete();

		const char* LastError(){ return m_Error.c_str(); }
		void SetError( const char* pError ){ m_Error = pError; }

	protected:

		Holly::IWriter*				m_pWriter;
		std::string					m_Error;


};