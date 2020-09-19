#ifndef _HEADER_GUARD_CPPFileIO_FileWriter_
#define _HEADER_GUARD_CPPFileIO_FileWriter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ FileWriter

template <typename T>
class _MACRO_CLASS_NAME_ {

////////////////////////
// Definitions BEGIN: //
////////////////////////

public:

	using TYPE_ELEMENT = T ;
	
	using TYPE_SELF =
		_MACRO_CLASS_NAME_ <TYPE_ELEMENT>
	; //

//////////////////////
// Definitions END. //
//////////////////////

/////////////////
// Data BEGIN: //
/////////////////

private:

	FileArray <T> infile ;
	size_t count ;
	size_t const bufsize ;
	size_t const mask ;
	TYPE_ELEMENT * buf ;

///////////////
// Data END. //
///////////////

///////////////////////////////////
// Main Working Functions BEGIN: //
///////////////////////////////////

private:

	inline size_t
	size() const
	{return count;}

	inline void
	push_back (
		TYPE_ELEMENT const &
			indata
	) {	size_t const mod 
			= count & mask
		; //
		if(mod==0){
			buf = & (infile(count,bufsize)) ;
		}
		buf[mod] = indata ;
		count++ ;
	}

/////////////////////////////////
// Main Working Functions END. //
/////////////////////////////////

///////////////////////////////
// Exposed Interfaces BEGIN: //
///////////////////////////////

public:

	inline size_t
	operator () () const
	{return size();}

	inline void
	operator ()
	(TYPE_ELEMENT const & indata)
	{push_back(indata);}

/////////////////////////////
// Exposed Interfaces END. //
/////////////////////////////

/////////////////////////////////////
// Constructor & Destructor BEGIN: //
/////////////////////////////////////

public:

	_MACRO_CLASS_NAME_
	(	std::string const name
	,	size_t const _bufsize = 10
	) :	infile(name)
	,	bufsize(shifter(_bufsize))
	,	mask(bufsize-1)
	{	infile.writeable();
		count=0;
		infile.size(count);
	}

	~_MACRO_CLASS_NAME_ ()
	{infile.size(count);}

///////////////////////////////////
// Constructor & Destructor END. //
///////////////////////////////////

} ;

#undef _MACRO_CLASS_NAME_

#endif
