#include <cstring>


void RGB_to_YUV( void* pSrc, unsigned int w, unsigned int h, void* pDestA, void* pDestB, void* pDestC, unsigned int iFlags )
{

	bool bFlipY = iFlags & Holly::Flags::Flip_Y;

	//
	// This isn't pretty, or fast. But right now it works.
	//

	unsigned char* pRGB = (unsigned char*)pSrc;
	unsigned char* pY = (unsigned char*)pDestA;
	unsigned char* pU = (unsigned char*)pDestB; //pY + (w*h);
	unsigned char* pV = (unsigned char*)pDestC; //pU + (((w*h)/4));

	for ( int y = 0; y<h; y++ )
		for ( int x = 0; x<w; x++ )
		{
			const unsigned int iRead = bFlipY ? (x + (h-y) * w)-w : (x + y * w);
			const unsigned int iPixel = x + y * w;

			const float r = pRGB[ iRead * 3 ];
			const float g = pRGB[ iRead * 3 + 1 ];
			const float b = pRGB[ iRead * 3 + 2 ];

			pY[ iPixel ] = (0.257f * r) + (0.504f * g) + (0.098f * b) + 16.0f;

			if ( y%2 == 0 && x%2 == 0 )
			{
				const unsigned int iPixelHalf = (x/2) + ((y/2) * (w/2));
				pV[ iPixelHalf ] = ((0.439f * r) - (0.368f * g) - (0.071f * b) + 128.0f);
				pU[ iPixelHalf ] = (-(0.148f * r) - (0.291f * g) + (0.439f * b) + 128.0f);
			}
		}

}
