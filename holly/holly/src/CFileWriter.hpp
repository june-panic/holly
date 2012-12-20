
#include <fstream>

class CFileWriter : public Holly::IWriter
{
	public:

		CFileWriter();
		bool Open( const char* strFilename );

		void Write( const void* pData, unsigned long long iSize );
		unsigned long long Tell();
		bool Seek( unsigned long long iPos );
		void Finish();
		void Delete();

	protected:

		std::ofstream*	m_File;

};