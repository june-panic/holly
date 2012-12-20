
#include "Errors.hpp"

namespace Holly 
{
	namespace Errors 
	{
		std::string	m_LastError;

		void Add( const char* strError )
		{
			m_LastError = strError;
		}

		 const char* GetLast()
		{
			return m_LastError.c_str();
		}		
	}
}