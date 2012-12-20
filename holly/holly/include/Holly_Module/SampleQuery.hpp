#include <climits>
#include <cstdlib>

namespace Holly 
{
	class SampleQuery
	{
		public:

			SampleQuery( void* pData, unsigned int iBits, unsigned int iChannels )
			{
				m_pData = pData;
				m_iBits = iBits;
				m_iChannels = iChannels;
			}

			float GetFloatSample( int iSample, int iChannel )
			{
				iChannel = iChannel%m_iChannels;

				if ( m_iBits == 8 )
				{
					unsigned char* pData = (unsigned char*)m_pData;
					unsigned char pValue = pData[ iSample * m_iChannels + iChannel ];
					return ((float)pValue) / 256.0f;
				}

				if ( m_iBits == 16 )
				{
					short* pData = (short*)m_pData;
					short pValue = pData[ iSample * m_iChannels + iChannel ];
					return ((float)pValue) / ((float)SHRT_MAX);
				}

				// STATIC = UNHANDLED BITMODE!

				return (rand()%8) / 8.0f;
			}


		protected:

			void*				m_pData;
			unsigned int		m_iBits;
			unsigned int		m_iChannels;

	};
}
