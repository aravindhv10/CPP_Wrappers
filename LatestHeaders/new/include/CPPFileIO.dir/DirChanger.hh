#ifndef _HEADER_GUARD_CPPFileIO_DirChanger_
#define _HEADER_GUARD_CPPFileIO_DirChanger_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ ToDir

class _MACRO_CLASS_NAME_ {

//////////////////////////
// Data Elements BEGIN: //
//////////////////////////

private:

	std::string originaldir ;
	bool goback ;

////////////////////////
// Data Elements END. //
////////////////////////

///////////////////////////////////
// Main Working Functions BEGIN: //
///////////////////////////////////

private:

	inline void
	godir (std::string dirname) {
		mkdir((const char*)&(dirname[0]),(mode_t)0755);
		chdir((const char*)&(dirname[0]));
	}

/////////////////////////////////
// Main Working Functions END. //
/////////////////////////////////

////////////////////////////////
// Debugging Functions BEGIN: //
////////////////////////////////

public:

	inline void
	showdir() {
		char tmp[1024] ;
		getcwd((char*)tmp,(size_t)1024);
		printf("%s\n",tmp);
       	}

	inline std::string
	getoriginal ()
	{return originaldir;}

//////////////////////////////
// Debugging Functions END. //
//////////////////////////////

////////////////////////////
// Main Interfaces BEGIN: //
////////////////////////////

public:

	inline void
	operator () (std::string dirname)
	{godir(dirname);}

	inline void
	operator () (const char *dirname)
	{godir(std::string(dirname));}

	inline void
	operator ()
	(long innum) {
		char tmp[128] ;
		sprintf(tmp,"%ld",innum);
		godir(std::string(tmp));
	}

       	inline void
	operator () ()
	{godir(originaldir);}

//////////////////////////
// Main Interfaces END. //
//////////////////////////

/////////////////////////////////////
// Constructor & Destructor BEGIN: //
/////////////////////////////////////

public:

	_MACRO_CLASS_NAME_(
		bool _goback=false
	) {
		goback = _goback ;
		char tmp[1024] ;
		getcwd((char*)tmp,(size_t)1024);
		originaldir = std::string(tmp);
		std::cout << originaldir << "\n" ;
	}

       	~_MACRO_CLASS_NAME_(){
		if(goback){
			godir(originaldir);
		}
	}

///////////////////////////////////
// Constructor & Destructor END. //
///////////////////////////////////

};

#undef _MACRO_CLASS_NAME_
#endif
