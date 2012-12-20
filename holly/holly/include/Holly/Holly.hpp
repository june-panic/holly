#if !defined(HOLLY_HPP)
#define HOLLY_HPP

#include <stddef.h>
#include <string>

#if defined(_MSC_VER)
#pragma warning( disable : 4250 )
#endif

#include "Platform.hpp"
#include "Writer.hpp"
#include "ColourModes.hpp"
#include "Utility.hpp"
#include "MovieElements.hpp"
#include "IHolly.hpp"
#include "Movie.hpp"
#include "MovieEncoder.hpp"
#include "Modules.hpp"

namespace Holly 
{
	const unsigned int Version = 1;

	namespace Flags
	{
		const unsigned int Flip_Y = (1<<0);
	}

	namespace ContainerTypes
	{
		const unsigned char MKV = (1<<0);
		const unsigned char OGG = (1<<1);
	}

	#ifdef HOLLY_STATIC
	
	#endif
};

#ifdef HOLLY_STATIC

	//
	// This define adds a provider function from a statically linked module
	// The first param is a pointer to Holly, second is the name of the module.
	// Caps is important.
	//
	// usage: HOLLY_ADD_STATIC_MODULE( holly, OGG )
	//
	#define HOLLY_ADD_STATIC_MODULE( _hollyclass_, _NAME_ ) \
		Holly::IMovieElement* Holly_Module_##_NAME_ ( const char* name );\
		(_hollyclass_)->Modules()->AddProvider( #_NAME_, Holly_Module_##_NAME_ );

#endif 

#endif // HOLLY_HPP
