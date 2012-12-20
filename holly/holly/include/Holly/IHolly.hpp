

namespace Holly
{
	class IMovieEncoder;
	class IModules;

	class IHolly
	{
		public:

			virtual IMovieEncoder*	CreateEncoder() = 0;

			virtual IUtility*		Utility() = 0;
			virtual IModules*		Modules() = 0;

			virtual const char*		LastError() = 0;
			
	};
}