#!/bin/bash

UNAME=`uname`

if [ "$UNAME" == "Darwin" ]; then
 
	./premake4OSX --platform=universal32 gmake
	cd macosx/gmake
	make config=releasedlluniv32
	make config=releaselibuniv32
	make config=debugdlluniv32
	make config=debuglibuniv32
	
fi
	
	
if [ "$UNAME" == "Linux" ]; then

	./premake4Linux gmake
	cd linux/gmake
	make config=releasedll
	make config=debuglib
fi

