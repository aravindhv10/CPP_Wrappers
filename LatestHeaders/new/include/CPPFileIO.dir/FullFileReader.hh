#ifndef _HEADER_GUARD_CPPFileIO_FullFileReader_
#define _HEADER_GUARD_CPPFileIO_FullFileReader_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ FullFileReader

template <typename T>
class _MACRO_CLASS_NAME_ {

////////////////////////
// Definitions BEGIN: //
////////////////////////

public:

	using TYPE_SELF = _MACRO_CLASS_NAME_ ;
	using TYPE_ELEMENT = T ;

//////////////////////
// Definitions END. //
//////////////////////

/////////////////////////
// Data Element BEGIN: //
/////////////////////////

private:

	FileArray <TYPE_ELEMENT> MainReader ;
	size_t const limit ;
	TYPE_ELEMENT const * const ptr ;

///////////////////////
// Data Element END. //
///////////////////////

///////////////////
// Access BEGIN: //
///////////////////

public:

	inline size_t size () const {return limit;}
	inline T const & operator () (size_t i) const { return ptr[i] ; }
        inline size_t    operator () ()         const { return limit  ; }

/////////////////
// Access END. //
/////////////////

/////////////////////////////////////
// Constructor & Destructor BEGIN: //
/////////////////////////////////////

public:

	_MACRO_CLASS_NAME_
	(	std::string const filename
	) :	MainReader (filename)
	,	limit (MainReader.size())
	,	ptr (&(MainReader(0,limit)))
	{}

	~_MACRO_CLASS_NAME_ () {}

///////////////////////////////////
// Constructor & Destructor END. //
///////////////////////////////////

};

#undef _MACRO_CLASS_NAME_

#endif
