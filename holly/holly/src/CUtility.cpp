
#include "Holly/Holly.hpp"
#include "CUtility.hpp"
#include "ColorConversion.hpp"


void CUtility::Convert( void* pSrc, unsigned int w, unsigned int h, Holly::ColorMode::ENUM m, Holly::ColorMode::ENUM mdest, void* pDest, unsigned int iFlags )
{
	unsigned char* pU = ((unsigned char*)pDest) + (w*h);
	unsigned char* pV = pU + (((w*h)/4));

	return Convert( pSrc, w, h, m, mdest, pDest, pU, pV, iFlags );
}

void CUtility::Convert( void* pSrc, unsigned int w, unsigned int h, Holly::ColorMode::ENUM m, Holly::ColorMode::ENUM mdest, void* pDestA, void* pDestB, void* pDestC, unsigned int iFlags )
{
	if ( m == Holly::ColorMode::RGB )
	{
		if ( mdest == Holly::ColorMode::YUV )
			return RGB_to_YUV( pSrc, w, h, pDestA, pDestB, pDestC, iFlags );
	}

}