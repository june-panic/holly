
#if defined(_WIN32)
	#include "Platform_Win32.hpp"
#endif 

#if defined(__linux__) || defined(__APPLE__)
	#include "Platform_Posix.hpp"
#endif 