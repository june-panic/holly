
#include <cstring>

#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"

#include "Holly_VP8_Video.h"
#include "Holly_WEBM_Container.h"


HOLLY_MODULE_QUERY( WEBM, name )
{
	std::string strName = name;
	//
	// This is what Holly calls in our DLL to 
	// retrieve the video/container/audio. 
	//
	// Names are always passed as lower case.
	//

	if ( strName == "vp8" )
		return new CHolly_VP8_Video();

	if ( strName == "webm" )
		return new CHolly_WEBM_Container();

	return NULL;
};
