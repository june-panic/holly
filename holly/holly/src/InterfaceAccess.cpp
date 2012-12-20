
#include "Holly/Holly.hpp"
#include "CHolly.hpp"

static CHolly	g_Holly;

HOLLY_EXPORT Holly::IHolly* Get_Holly_Interface( unsigned int iVersion )
{
	if ( iVersion != Holly::Version ) return NULL;

	return &g_Holly;
}


#ifdef HOLLY_STATIC
namespace Holly
{
	Holly::IHolly* Get_Static_Holly_Interface( unsigned int iVersion )
	{
		return ::Get_Holly_Interface( iVersion );
	}
};
#endif