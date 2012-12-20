
namespace Holly 
{
	class IWriter 
	{
		public:

			virtual void Write( const void* pData, unsigned long long iSize ) = 0;
			virtual unsigned long long Tell() = 0;
			virtual bool Seek( unsigned long long iPos ) = 0;
			virtual void Finish() = 0;

			virtual void Delete() = 0;
	};
}
