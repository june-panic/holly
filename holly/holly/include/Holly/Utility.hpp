
namespace Holly 
{
	class IUtility 
	{
		public:

			virtual void Convert( void* pBuffer, unsigned int Width, unsigned int Height, Holly::ColorMode::ENUM mode, Holly::ColorMode::ENUM DestMode, void* pDestBuffer, unsigned int iFlags = 0 ) = 0;
			virtual void Convert( void* pBuffer, unsigned int Width, unsigned int Height, Holly::ColorMode::ENUM mode, Holly::ColorMode::ENUM DestMode, void* pDestA, void* pDestB, void* pDestC, unsigned int iFlags = 0 ) = 0;

	};
}
