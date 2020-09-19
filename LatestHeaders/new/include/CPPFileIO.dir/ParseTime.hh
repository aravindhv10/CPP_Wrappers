#ifndef _HEADER_GUARD_CPPFileIO_ParseTime_
#define _HEADER_GUARD_CPPFileIO_ParseTime_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

	inline time_t
	Parse_Time(
		char const * intime
	) {
		std::tm mytime ;
		strptime(intime, "%Y-%m-%d %H:%M:%S", &mytime);
		std::time_t ret = std::mktime(&mytime);
		return ret ;
	}

	inline time_t
	Parse_Time(
		std::string const intime
	) {
		return Parse_Time(intime.c_str()) ;
	}

#endif
