
#include <stdio.h>
#include "mkvmuxer/mkvmuxertypes.hpp"

class Holly_MKMuxer_Writer : public mkvmuxer::IMkvWriter 
{
	public:

		mkvmuxer::int32 Write(const void* buf, mkvmuxer::uint32 len)
		{
			holly_writer->Write( buf, len );
			return 0;
		}

		mkvmuxer::int64 Position() const
		{
			return holly_writer->Tell();
		}

		mkvmuxer::int32 Position( mkvmuxer::int64 position )
		{
			return holly_writer->Seek( position ) ? 0 : 1;
		}

		bool Seekable() const
		{
			return true;
		}

		Holly::IWriter*	holly_writer;
};
