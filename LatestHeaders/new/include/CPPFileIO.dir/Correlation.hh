#ifndef _HEADER_GUARD_CPPFileIO_Correlation_
#define _HEADER_GUARD_CPPFileIO_Correlation_

#include "./D1.hh"

#define _MACRO_MAIN_TYPE_ Dynamic1DArray <T>

template <typename T>
inline void
NormalizeInPlace (_MACRO_MAIN_TYPE_ & in) {
	T sum[2] = {0,0} ;
	for(size_t i=0;i<in();i++){
		sum[0] += in(i) ;
		sum[1] += std::pow(in(i),2) ;
	}
	T const mu = sum[0] / static_cast <T> (in()) ;
	T const sigma =
		std::sqrt(
			( sum[1] / static_cast <T> (in()) )
			- std::pow(mu,2)
		)
	;
	for(size_t i=0;i<in();i++){
		in(i) =
			( in(i) - mu )
			/ sigma
		;
	}
}

template <typename T>
_MACRO_MAIN_TYPE_
GetNormalized (_MACRO_MAIN_TYPE_ const & in) {
	T sum[2] = {0,0} ;
	for(size_t i=0;i<in();i++){
		sum[0] += in(i) ;
		sum[1] += std::pow(in(i),2) ;
	}
	T const mu = sum[0] / static_cast <T> (in()) ;
	T const sigma =
		std::sqrt(
			( sum[1] / static_cast <T> (in()) )
			- std::pow(mu,2)
		)
	;
	_MACRO_MAIN_TYPE_ ret (in()) ;
	for(size_t i=0;i<in();i++){
		ret(i) =
			( in(i) - mu )
			/ sigma
		;
	}
	return ret ;
}

#undef _MACRO_MAIN_TYPE_

#endif
