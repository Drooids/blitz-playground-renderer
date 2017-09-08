#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <limits.h>
#include <unistd.h>
#endif

#define MAX_PATH 512

namespace FS {
	namespace Path {

		static const std::string getCurrent()
		{
		  #ifdef _WIN32
			  char result[ MAX_PATH ];
			  return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
		  #endif

		  #ifdef linux
			  char result[ MAX_PATH ];
			  ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
			  return std::string( result, (count > 0) ? count : 0 );
		  #endif
		}

	}
}
