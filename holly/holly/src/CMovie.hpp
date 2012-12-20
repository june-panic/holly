

class CMovie : public virtual Holly::IMovie
{
	public:

		CMovie();
		~CMovie();

	public:

		// This is called automatically by Holly when creating
		void Initialize( CHolly* pHolly ){ m_pHolly = pHolly; }

		Holly::IUtility* Utility(){ return m_pHolly->Utility(); }
		Holly::IHolly* Holly(){ return m_pHolly; }
		Holly::IMovieEncoder* Encoder(){ return NULL; }
		Holly::IContainer* Container();
		Holly::IVideo* Video( unsigned int iTrackNum = 0 );
		Holly::IAudio* Audio( unsigned int iTrackNum = 0 );

		// MUST be called to delete this object from memory
		void Delete() = 0;

	protected:

		CHolly*	m_pHolly;

		Holly::IContainer*	m_Container;
		Holly::IVideo*		m_Video;
		Holly::IAudio*		m_Audio;



};