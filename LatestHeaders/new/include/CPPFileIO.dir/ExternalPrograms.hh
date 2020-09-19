#ifndef _HEADER_GUARD_CPPFileIO_ExternalPrograms_
#define _HEADER_GUARD_CPPFileIO_ExternalPrograms_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Basic.hh"
//////////////////
// Headers END. //
//////////////////

//////////////////////////
// Forking Class BEGIN: //
//////////////////////////

#define _MACRO_CLASS_NAME_ ForkMe

class _MACRO_CLASS_NAME_ {

//////////////////////////
// Data Elements BEGIN: //
//////////////////////////

private:

	std::vector <pid_t> kids  ;
	bool                child ;
	size_t              kid_count ;

////////////////////////
// Data Elements END. //
////////////////////////

///////////////////////////////////
// Main Working Functions BEGIN: //
///////////////////////////////////

public:

	inline bool
	WaitOnKids () {
		if (child) {
			exit(0);
			return false;
		} else if (kids.size()>0) {
			waitonall(kids);
			kids.clear();
			return true;
		} else {
			return false;
		}
	}

	inline bool
	InKid () {
		if (!child){
			pid_t const tmp_pid = fork () ;
			if (tmp_pid==0) {
				child = true;
				return true;
			} else {
				kids.push_back (tmp_pid) ;
				kid_count++ ;
				return false ;
			}
		} else {
			exit (0) ;
			return false ;
		}
	}

	inline void
	operator () (
		size_t _count=1
	) {
		if(kid_count>_count)
		{ WaitOnKids () ; }
	}

/////////////////////////////////
// Main Working Functions END. //
/////////////////////////////////

/////////////////////////////////////
// Constructor & Destructor BEGIN: //
/////////////////////////////////////

public:

	_MACRO_CLASS_NAME_ () {
		kids.clear();
		child = false;
		kid_count = 1;
	}

	~_MACRO_CLASS_NAME_(){
		if (child) {
			exit (0) ;
		} else {
			WaitOnKids () ;
		}
	}

///////////////////////////////////
// Constructor & Destructor END. //
///////////////////////////////////

} ;

#undef _MACRO_CLASS_NAME_

////////////////////////
// Forking Class END. //
////////////////////////

////////////////////
// Starter BEGIN: //
////////////////////

#define _MACRO_CLASS_NAME_ ExternalStarter

template <bool DoFork=false>
class _MACRO_CLASS_NAME_ {

////////////////////////
// Definitions BEGIN: //
////////////////////////

public:
	using TYPE_SELF =
		_MACRO_CLASS_NAME_
			<DoFork>
	; //
	using TYPE_NAMES =
		std::vector <std::string>
	; //

//////////////////////
// Definitions END. //
//////////////////////

//////////////////////////
// Data Elements BEGIN: //
//////////////////////////

private:

	TYPE_NAMES prog ; //

////////////////////////
// Data Elements END. //
////////////////////////

/////////////////////////////////////
// Constructor & Destructor BEGIN: //
/////////////////////////////////////

public:

	ExternalStarter (
		TYPE_NAMES const &
			in
	) {
		prog.reserve(in.size());
		for(size_t i=0;i<in.size();i++){
			prog.push_back(in[i]);
		}
	}

	_MACRO_CLASS_NAME_  (
		std::string const
			in
	) {
		prog.push_back(in);
	}

        ~_MACRO_CLASS_NAME_ () {
		if(DoFork){
			ForkMe forker ;
			if(forker.InKid()){
				starter_self(prog);
			}
		} else {
			starter_self(prog);
		}
	}

///////////////////////////////////
// Constructor & Destructor END. //
///////////////////////////////////

////////////////////////////
// Main Interfaces BEGIN: //
////////////////////////////

public:

	inline TYPE_SELF &
	operator () (
		TYPE_NAMES const &
			in
	) {
		for(size_t i=0;i<in.size();i++){
			prog.push_back(in[i]);
		}
		return this[0] ;
	}

	inline TYPE_SELF &
	operator () (
		std::string const
			in
	) {
		prog.push_back(in);
		return this[0];
	}

	static inline TYPE_SELF
	GET (
		std::string const
			in
	) {
		TYPE_SELF ret(in);
		return ret ;
	}

//////////////////////////
// Main Interfaces END. //
//////////////////////////

};

//////////////////
// Starter END. //
//////////////////

#undef _MACRO_CLASS_NAME_

#endif
