#ifndef _HEADER_GUARD_CPPFileIO_Shuffle_
#define _HEADER_GUARD_CPPFileIO_Shuffle_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../pcg-cpp.hh"
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

///////////////////////////////
// Shuffle On Vectors BEGIN: //
///////////////////////////////

	template
	<	typename T
	,	typename R = pcg64_fast
	>
	inline void
	Shuffle (
		std::vector <T> &
			in
	) {
		std::random_device randinit ;
		R engine(randinit()) ;
		/* Shuffel the array */ {
			for(size_t i=0;i<(in.size()-1);i++){
				std::uniform_int_distribution <size_t> dis(i,in.size()-1) ;
				size_t const j = dis(engine) ;
				if(i!=j){
					T & Src = in[i] ;
					T & Dst = in[j] ;
					T const tmp = Src ;
					Src = Dst ;
					Dst = tmp ;
				}
			}
		}
	}

/////////////////////////////
// Shuffle On Vectors END. //
/////////////////////////////

/////////////////////////////////////////
// Shuffle on whole mapped file BEGIN: //
/////////////////////////////////////////

	template
	<	typename T
	,	typename R = pcg64_fast
	>
	inline void
	Shuffle_Faster (
		std::string const
			filename
	) {
		FileArray <T> filereaderS (filename) ;
		size_t const limit = filereaderS.size() ;
		std::random_device randinit ;
		R engine(randinit()) ;
		filereaderS.writeable(true) ;
		T * buf = & ( filereaderS(0,limit) ) ;
		/* Shuffel the file */ {
			for(size_t i=0;i<(limit-1);i++){
				std::uniform_int_distribution <size_t> dis(i,limit-1) ;
				size_t const j = dis(engine) ;
				if(i!=j){
					T & Src = buf[i] ;
					T & Dst = buf[j] ;
					T const tmp = Src ;
					Src = Dst ;
					Dst = tmp ;
				}
			}
		}
		filereaderS.size(limit);
	}

///////////////////////////////////////
// Shuffle on whole mapped file END. //
///////////////////////////////////////

////////////////////////////////////////
// Shuffle on part mapped file BEGIN: //
////////////////////////////////////////

	template
	<	typename T
	,	typename R = pcg64_fast
	>
	inline void
	Shuffle (
		std::string const
			filename
	) {
		FileArray <T> filereaderS (filename) ;
		size_t const limit = filereaderS.size() ;
		std::random_device randinit ;
		R engine(randinit()) ;
		filereaderS.writeable(true) ;
		/* Shuffel the file */ {
			FileArray <T> filereaderD (filename) ;
			filereaderD.writeable(true);
			for(size_t i=0;i<(limit-1);i++){
				std::uniform_int_distribution <size_t> dis(i,limit-1) ;
				size_t const j = dis(engine) ;
				if(i!=j){
					T & Src = filereaderS(i) ;
					T & Dst = filereaderD(j) ;
					T const tmp = Src ;
					Src = Dst ;
					Dst = tmp ;
				}
			}
		}
		filereaderS.size(limit);
	}

//////////////////////////////////////
// Shuffle on part mapped file END. //
//////////////////////////////////////

#endif
