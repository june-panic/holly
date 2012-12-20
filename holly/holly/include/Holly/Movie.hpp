
namespace Holly 
{
	class IMovieEncoder;
	class IHolly;
	class IUtility;
	
	class IMovie 
	{
		public:

			// This returns this class as an Encoder (if it is an encoder)
			virtual IMovieEncoder* Encoder() = 0;

			// Element access
			virtual IContainer*		Container() = 0;;
			virtual IVideo*			Video( unsigned int iTrackNum = 0 ) = 0;
			virtual IAudio*			Audio( unsigned int iTrackNum = 0 ) = 0;

			// Misc Accessors
			virtual IUtility*	Utility() = 0;
			virtual IHolly*		Holly() = 0;

			// MUST be called to delete this object from memory
			virtual void Delete() = 0;

		protected:

	};
}