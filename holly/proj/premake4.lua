solution "Holly"

		language "C++"
		location ( os.get() .. "/" .. _ACTION )
		flags { "Unicode", "Symbols", "NoPCH", "StaticRuntime" }
		includedirs { "../holly/include/" }

		configurations
		{ 
			"ReleaseDLL",
			"ReleaseLIB",
			"DebugDLL",
			"DebugLIB"
		}
		
		if ( _ACTION == "vs2010" or _ACTION=="vs2008" ) then
			buildoptions { "/MP"  }
		end 

	configuration "ReleaseDLL"
		defines { "NDEBUG" }
		flags{ "OptimizeSpeed", "FloatFast" }
		targetdir ( "../out/" .. os.get() .. "/" .. _ACTION .. "/release/" )
		
	configuration "ReleaseLIB"
		defines { "NDEBUG", "HOLLY_STATIC" }
		flags{ "OptimizeSpeed", "FloatFast" }
		targetdir ( "../out/" .. os.get() .. "/" .. _ACTION .. "/release_static/" )
		
	configuration "DebugDLL"
		defines { "_DEBUG" }
		targetdir ( "../out/" .. os.get() .. "/" .. _ACTION .. "/debug/" )
		
	configuration "DebugLIB"
		defines { "_DEBUG", "HOLLY_STATIC" }
		targetdir ( "../out/" .. os.get() .. "/" .. _ACTION .. "/debug_static/" )
		
	function SetLibraryTypeBasedOnConfig()

		configuration "ReleaseDLL"
			kind "SharedLib"
		configuration "ReleaseLIB"
			kind "StaticLib"
			buildoptions { "-fPIC"  }
		configuration "DebugDLL"
			kind "SharedLib"
		configuration "DebugLIB"
			kind "StaticLib"
			buildoptions { "-fPIC"  }

	end

	project "Holly"	
	
		targetname( "holly" )
		files { "../holly/src/**.*" }
		files { "../holly/include/Holly/**.*" }
		defines { "HOLLY_BUILD" }
		
		if os.get() == "linux" then links { "dl" } end
		
		SetLibraryTypeBasedOnConfig()
		
	project "Module_webm"	
	
		targetname( "holly_webm" )
		files { "../modules/webm/**.*" }
		includedirs { "../modules/webm/libvpx/include/" }
		libdirs { "../modules/webm/libvpx/lib/" .. os.get() .. "/" .. _ACTION .. "/", "../modules/webm/libvpx/lib/" .. os.get() .. "/" }
				
		if ( os.get() == "windows" ) then links { "vpxmt" } else links { "vpx" } end
		
		SetLibraryTypeBasedOnConfig()


	project "Module_ogg"	
	
		targetname( "holly_ogg" )
		files { "../modules/ogg/**.*" }
		includedirs { "../modules/ogg/libogg/include/", "../modules/ogg/libvorbis/include/", "../modules/ogg/libtheora/include/" }
		libdirs { "../modules/ogg/libvorbis/lib/" .. os.get() .. "/" .. _ACTION .. "/", "../modules/ogg/libtheora/lib/" .. os.get() .. "/" .. _ACTION .. "/" }
		libdirs { "../modules/ogg/libvorbis/lib/" .. os.get() .. "/", "../modules/ogg/libtheora/lib/" .. os.get() .. "/" }
		
		
		if os.get() == "windows" then links { "libvorbis_static" } else links { "vorbis", "vorbisenc" } end
		if os.get() == "windows" then links { "libtheora_static" } else links { "theora" } end
		
		SetLibraryTypeBasedOnConfig()
		
	project "Example_Encoding"	
	
		targetname( "EncodingExample" )
		files { "../examples/Encoding.cpp" }

		libdirs { "../modules/webm/libvpx/lib/" .. os.get() .. "/" .. _ACTION .. "/", "../modules/webm/libvpx/lib/" .. os.get() .. "/" }
		libdirs { "../modules/ogg/libvorbis/lib/" .. os.get() .. "/" .. _ACTION .. "/", "../modules/ogg/libtheora/lib/" .. os.get() .. "/" .. _ACTION .. "/" }
		libdirs { "../modules/ogg/libvorbis/lib/" .. os.get() .. "/", "../modules/ogg/libtheora/lib/" .. os.get() .. "/" }

		if ( os.get() == "windows" ) then links { "vpxmt" } else links { "vpx" } end
		if ( os.get() == "windows" ) then links { "libvorbis_static" } else links { "vorbis", "vorbisenc" } end
		if ( os.get() == "windows" ) then links { "libtheora_static" } else links { "theora" } end

		links { "Holly", "Module_ogg", "Module_webm" }
		
		kind "ConsoleApp"
		
		if ( os.get() == "linux" or os.get() == "macosx" ) then 
			links { "dl" } 
		end


