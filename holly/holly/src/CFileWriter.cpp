
#include "Holly/Holly.hpp"
#include "CFileWriter.hpp"


CFileWriter::CFileWriter()
{
	m_File = NULL;
}

bool CFileWriter::Open( const char* strFilename )
{
	m_File = new std::ofstream( strFilename, std::ofstream::binary );
	if ( !m_File->is_open() )
	{
		delete m_File;
		return false;
	}

	return true;
}

void CFileWriter::Write( const void* pData, unsigned long long iSize )
{
	m_File->write( (const char*) pData, iSize );
}

unsigned long long CFileWriter::Tell()
{
	return m_File->tellp();
}

bool CFileWriter::Seek( unsigned long long iPos )
{
	m_File->seekp( iPos );
	return true;
}

void CFileWriter::Finish()
{
	if ( !m_File || !m_File->is_open() ) return;

	m_File->close();
	delete m_File;
	m_File = NULL;
}

void CFileWriter::Delete()
{
	Finish();
	delete this;
}