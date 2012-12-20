


class CUtility : public Holly::IUtility
{
	public:

		void Convert( void* pBuffer, unsigned int Width, unsigned int Height, Holly::ColorMode::ENUM mode, Holly::ColorMode::ENUM DestMode, void* pDestBuffer, unsigned int iFlags = 0 );
		void Convert( void* pSrc, unsigned int w, unsigned int h, Holly::ColorMode::ENUM m, Holly::ColorMode::ENUM mdest, void* pDestA, void* pDestB, void* pDestC, unsigned int iFlags = 0 );

	protected:
		

};