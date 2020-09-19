

#ifndef _HEADER_GUARD_Read_Show_Functions_Compare_
#define _HEADER_GUARD_Read_Show_Functions_Compare_

///////////////////
// Header BEGIN: //
///////////////////
#include "../StaticArray.hh"
/////////////////
// Header END. //
/////////////////

///////////////////////////////////////////////
#define COMPARE_INT(intype)                  \
	inline char Compare                  \
	(	intype const a               \
	,	intype const b               \
	) {	return                       \
			  ( -1 * (a  < b ) ) \
			+ (  1 * (a  > b ) ) \
	; }                                  //
///////////////////////////////////////////////
	COMPARE_INT(char)
	COMPARE_INT(unsigned char)
	COMPARE_INT(int)
	COMPARE_INT(unsigned int)
	COMPARE_INT(long)
	COMPARE_INT(unsigned long)
	COMPARE_INT(float)
	COMPARE_INT(double)
/////////////////////
#undef COMPARE_INT //
/////////////////////

	inline char
	Compare
	(	char const * S1
	,	char const * S2
	) {	int const val = strcmp(S1,S2);
		return Compare(val,0);
	}

	template
	<	size_t m
	,	size_t n
	> inline char
	Compare
	(	StaticArray::ND_ARRAY <m,char> const & S1
	,	StaticArray::ND_ARRAY <n,char> const & S2
	) {	return
			Compare
			(	S1.GET_DATA()
			,	S2.GET_DATA()
			)
		; //
	}

	template <size_t n>
	inline char
	Compare
	(	StaticArray::ND_ARRAY <n,char> const & S1
	,	std::string const & S2
	) {	return Compare(S1.GET_DATA(),S2.c_str());
	}
	
	template <size_t n>
	inline char
	Compare
	(	std::string const & S1
	,	StaticArray::ND_ARRAY <n,char> const & S2
	) {	return Compare(S2,S1) * -1 ;
	}

	inline char
	Compare
	(	std::string const & S1
	,	std::string const & S2
	) {	return Compare(S1.c_str(),S2.c_str()) ;
	}

#endif
