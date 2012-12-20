#include "Holly/Holly.hpp"
#include "Holly/Loader.hpp"
#include "EmbeddedSoundFile.hpp"

#include <math.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>


int EncodeExampleVideo( Holly::IHolly* pHolly, const char* strContainer, const char* strVideo, const char* strAudio, const char* strOutputName )
{
	printf( "\n" );

	//
	// Create the encoder
	//
	Holly::IMovieEncoder* encoder = pHolly->CreateEncoder();

	printf( "Container:	%s\n", strContainer );
	// Set the container format (ie MKV, AVI, MP4, WEBM)
	if ( !encoder->SetContainer( strContainer ) )
	{
		printf( "Couldn't create the container format (%s)!\n", strContainer );
		getchar();
		return 0;
	}

	encoder->SetOutputFile( strOutputName );

	printf( "Video:		%s\n", strVideo );
	// Set the video codec
	if ( !encoder->SetVideo( strVideo ) )
	{
		printf( "Couldn't set the video codec!\n" );
		return 0;
	}

	printf( "Audio:		%s\n", strAudio );
	// Set the audio codec
	if ( !encoder->SetAudio( strAudio ) )
	{
		printf( "Couldn't set the audio codec!\n" );
		return 0;
	}

	// Setup our video codec
	if ( encoder->Video() )
	{
		encoder->Video()->SetSize( 640, 480 );
		encoder->Video()->SetFPS( 30 );
		encoder->Video()->SetDeadline( 1000 );
		encoder->Video()->SetBitRate( 512 );
	}

	// Setup our audio codec
	if ( encoder->Audio() )
	{
		encoder->Audio()->Setup( 2, 44100 );
	}

	// Start the video
	if ( !encoder->Start() )
	{
		printf( "There was an error calling Start! (%s)\n", encoder->LastError() );
		return 0;
	}

	// Create a fake RGB buffer to copy from
	unsigned char* pFrame = (unsigned char*)malloc( encoder->Video()->Width() * encoder->Video()->Height() * 3 );
	unsigned const char* pSoundData = TestSound::Data;

	printf( "[150 frames] " );

	for ( int i=0; i< 150; i++ )
	{
		float movie_delta = i / 150.0f;
		//
		// Draw something pretty in the frame
		//
		// I know this isn't as optimized as it could be, but it's just to fill the frame a bit!
		//
		{
			int w = encoder->Video()->Width();
			int h = encoder->Video()->Height();
			float longest = sqrt( (float) w * w + h * h ) * 0.5f;
			float pi_delta = movie_delta * 3.141592f * 2.0f;


			for (int x=0; x<w; x++)
				for ( int y=0; y<h; y++ )
				{
					int z = (3 * y * encoder->Video()->Width()) + x*3;

					// Draw RGB up above 
					if ( y < h * 0.1 )
					{
						pFrame[z] = 0;
						pFrame[z+1] = 0;
						pFrame[z+2] = 0;

						if ( x < w * 0.33 ) pFrame[z] = 255;
						else if ( x < w * 0.66 ) pFrame[z+1] = 255;
						else pFrame[z+2] = 255;
						
						continue;
					}

					//
					// Draw a box of static up the top left.. just so we know it's the top left.
					//
					if ( x > w * 0.1 && x < w * 0.15 && y > w * 0.1 && y < w * 0.15 )
					{
						pFrame[z] = rand()%255;
						pFrame[z+1] = rand()%255;
						pFrame[z+2] = rand()%255;
						continue;
					}

					float delta[2] = { x - w/2, y - h/2 };
					float length = sqrt( delta[0] * delta[0] + delta[1] * delta[1] );
					float sizedelta = length / longest;
					float ang = atan2( delta[0], delta[1] );
					ang += pow( length, 0.5f ) * 2.0f;

					pFrame[z] = (128.0f - sin( ang + pi_delta ) * 127.0f) * (1.0f-sizedelta);
					pFrame[z+1] = (128.0f - cos( ang + pi_delta ) * 127.0f) * (1.0f-sizedelta);
					pFrame[z+2] = (128.0f + sin( ang + pi_delta ) * 127.0f) * (1.0f-sizedelta);

				}
		}

		unsigned int iFlags = 0;

		if ( i > 75 ) iFlags = Holly::Flags::Flip_Y;

		if ( encoder->Video() )
			encoder->Video()->AddFrame( 1.0f / 30.0f, Holly::ColorMode::RGB, pFrame, iFlags );

		int iSampleCount = TestSound::Rate * (1.0f / 30.0f);
		int iSampleSize = iSampleCount * (TestSound::Bits / 8) * TestSound::Channels;

		// Loop sound data if we go past the end
		if ( pSoundData + iSampleSize > TestSound::Data + TestSound::Size )
			pSoundData = TestSound::Data;

		if ( encoder->Audio() )
			encoder->Audio()->AddSamples( iSampleCount, (void*)pSoundData, TestSound::Bits, TestSound::Channels );

		// Advance the sound data
		pSoundData += iSampleSize;
		printf( "." );
		fflush( stdout );
	}

	free( pFrame );

	//
	// End the video, finish writing, release the file
	//
	if ( !encoder->End() )
	{
		printf( "There was an error calling End!\n" );
		getchar();
	}

	// Delete the encoder
	encoder->Delete();

	return 0;
}

int main()
{
	//
	// This loads the holly library on demand.
	//
	// You must keep this around until we're done with it or it
	//  will unload the dll and your IMovieManager will be invalid!
	//
	// If you want to be a big man you can handle loading and unloading
	//  completely manually. See this class for details.
	//
	Holly::Loader	HollyLoader;

	//
	// At this point, on the first call of GetMovieManager the library
	//  is loaded. If we can't load the library it will return NULL.
	//
	Holly::IHolly*	holly = HollyLoader.GetHolly();

	//
	// If we do fail to load then LastError tells us why
	//
	if ( !holly )
	{
		std::string err = HollyLoader.LastError();
		printf( "Couldn't load Holly: %s\n", err.c_str() );
		getchar();
		return 0;
	}

	printf( "Holly loaded!\n" );

	//
	// Load the modules we need to use..
	//
	// These functions dynamically load the libraries
	// This allows you to possibly load all libraries in a folder etc
	// So that users can dynamically add new codecs to your program.
	//
	// If you're linking statically you don't need to do this.
	// Although you can if you want and it will work the same.
	//

#ifndef HOLLY_STATIC
	{
		if ( !holly->Modules()->Load( "webm" ) )
		{
			std::string err = holly->LastError();
			printf( "Error loading webm: %s\n", err.c_str() );
			getchar();
			return 0;
		}

		printf( "webm module loaded!\n" );

		if ( !holly->Modules()->Load( "ogg" ) )
		{
			std::string err = holly->LastError();
			printf( "Error loading ogg: %s\n", err.c_str() );
			getchar();
			return 0;
		}

		printf( "ogg module loaded!\n" );
	}
	//
	// If we're static we need to add add the static modules
	// for each lib we're including.
	//
#else

	HOLLY_ADD_STATIC_MODULE( holly, WEBM );
	HOLLY_ADD_STATIC_MODULE( holly, OGG );

#endif

	//
	// Here we output a file using each encoder
	//
	
	printf( "\nEverything is ok! Encoding videos..\n");

	EncodeExampleVideo( holly, "ogg", "theora", "vorbis", "test.ogg" );
	EncodeExampleVideo( holly, "webm", "vp8", "vorbis", "test.webm" );

	printf( "\n\nFinished everything.\n");

	// You don't HAVE to call this. When HollyLoader gets destroyed
	// it'll unload holly.dll - which will free all the modules anyway
	// It's just here to show that you can do it manually if you need to.
	printf( "Unloading Modules!\n");
	holly->Modules()->UnloadAll();

	printf( "All done!\n");

	getchar();
	return 0;
}
