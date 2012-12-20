
#include <cstring>

#include "Holly/Holly.hpp"
#include "Holly_Module/ModuleBase.hpp"

#include "Holly_OGG_Container.h"
#include "Holly_Vorbis_Audio.h"
#include "Holly_Theora_Video.h"


HOLLY_MODULE_QUERY( OGG, name )
{
	std::string strName = name;

	//
	// This is what Holly calls in our DLL to 
	// retrieve the video/container/audio. 
	//
	// Names are always passed as lower case.
	//

	if ( strName == "vorbis" )
		return new CHolly_Vorbis_Audio();

	if ( strName == "ogg" )
		return new CHolly_OGG_Container();

	if ( strName == "theora" )
		return new CHolly_Theora_Video();

	return NULL;
};
