#ifndef _HEADER_GUARD_Read_Show_Functions_Read
#define _HEADER_GUARD_Read_Show_Functions_Read

///////////////////
// Header BEGIN: //
///////////////////
#include "../CPPFileIO.hh"
#include "../StaticArray.hh"
/////////////////
// Header END. //
/////////////////

	template <size_t n>
	inline void
	Read_All
	(	StaticArray::ND_ARRAY <n,char> & dest
	,	std::string const & src
	) {
		std::memset
		(	/* void *s = */ reinterpret_cast<void*>(dest.GET_DATA())
		,	/* int c = */ 0
		,	/* size_t n = */ dest.SIZE()
		) ; //
		size_t const limit =
			CPPFileIO::mymin
			(	static_cast<size_t>(src.size())
			,	static_cast<size_t>(dest.SIZE()-1)
			)
		; //
		memcpy
		(	/* void *dest = */ reinterpret_cast<void*>(dest.GET_DATA())
		,	/* const void *src = */ reinterpret_cast<void const*>(src.c_str())
		,	/* size_t n = */ limit
		) ; //
	}

	static inline void
	Read_All
	(	float & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=NAN;}
		else{dest=std::stof(src);}
	}

	static inline void
	Read_All
	(	double & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=NAN;}
		else{dest=std::stod(src);}
	}

	static inline void
	Read_All
	(	char & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=0;}
		else{dest=src[0];}
	}

	static inline void
	Read_All
	(	unsigned char & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=0;}
		else{ dest = static_cast <unsigned char> (std::stoi(src)) ; }
	}

	static inline void
	Read_All
	(	int & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=-999999;}
		else{dest = std::stoi(src);}
	}

	static inline void
	Read_All
	(	long & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=-999999;}
		else{dest = std::stol(src);}
	}

	static inline void
	Read_All
	(	size_t & dest
	,	std::string const & src
	) {	if(src.size()<1){dest=0;}
		else{ sscanf(src.c_str(),"%zu",&dest); }
	}

#endif
