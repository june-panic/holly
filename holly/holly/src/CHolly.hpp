
class CUtility;
class CModules;

class CHolly : public Holly::IHolly
{
	public:

		CHolly();
		~CHolly();


		const char* LastError();

		Holly::IMovieEncoder*	CreateEncoder();
		Holly::IWriter*			CreateFileWriter( const char* strFileName );
		Holly::IMovieElement*	CreateElement( const char* strName );
		Holly::IModules*		Modules();

		virtual Holly::IUtility*		Utility();

	protected:

		CModules* m_Modules;
		CUtility* m_Utility;

};