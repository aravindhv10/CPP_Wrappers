#ifndef _HEADER_GUARD_StaticArray_ImportantHeads_
#define _HEADER_GUARD_StaticArray_ImportantHeads_

///////////////////////////////////
//// Pre-requisit Headers BEGIN: //
///////////////////////////////////
#include "../Headers.hh"
/////////////////////////////////
//// Pre-requisit Headers END. //
/////////////////////////////////

	size_t constexpr
		SKIP_SIZE =
			4
	; //

	template
	<	size_t S_2 = 1
	,	size_t S_1 = 1
	,	typename TF = float
	> class TRANSPOSE_2D ;

	template
	<	size_t S2 = 1
	,	size_t SP = 1
	,	size_t S1 = 1
	,	typename Tf = float
	> class MULTIPLY_2D_P21P ;

	template
	<	size_t S2 = 1
	,	size_t SP = 1
	,	size_t S1 = 1
	,	typename Tf = float
	> class MULTIPLY_2D_P2P1 ;

	template
	<	size_t S2 = 1
	,	size_t SP = 1
	,	size_t S1 = 1
	,	typename Tf = float
	> class MULTIPLY_2D_2P1P ;

	template
	<	size_t S2 = 1
	,	size_t SP = 1
	,	size_t S1 = 1
	,	typename Tf = float
	> class MULTIPLY_2D_2PP1 ;

//////////////////////////////////////////////////////////////
#ifdef CBLAS_H                                              //
//////////////////////////////////////////////////////////////
	namespace FUNCTIONS /* Wrappers for cblas: */ {     //
#define _MACRO_MyGEMM_(FloatType,BlasFunc)                  \
        inline void MyGEMM (                                \
            CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, \
            int const M, int const N, int const K,          \
            FloatType const alpha,                          \
            FloatType const *A, int lda,                    \
            FloatType const *B, int ldb,                    \
            FloatType beta, FloatType *C                    \
        ) {                                                 \
            BlasFunc (                                      \
                CblasRowMajor, TransA, TransB, M, N, K,     \
                alpha, A, lda, B, ldb, beta, C, N           \
            ) ;                                             \
        }                                                   //
_MACRO_MyGEMM_(float,cblas_sgemm)                           //
_MACRO_MyGEMM_(double,cblas_dgemm)                          //
#undef _MACRO_MyGEMM_                                       //
//////////////////////////////////////////////////////////////
inline CBLAS_TRANSPOSE constexpr                            //
SET_TRANSPOSE (bool const val) {                            //
	if(val){                                            //
		return CblasTrans ;                         //
	} else {                                            //
		return CblasNoTrans ;                       //
	}                                                   //
}                                                           //
//////////////////////////////////////////////////////////////
	}                                                   //
#endif                                                      //
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
#define _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(Sign)        \
        inline void operator Sign (TYPE_DATA const & a)     \
        { FLATTEN() Sign a ; }                              //
//////////////////////////////////////////////////////////////
#define _MACRO_REGISTER_ALL_OPERATORS_                      \
        _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(=)           \
        _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(+=)          \
        _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(-=)          \
        _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(*=)          \
        _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_(/=)          //
//////////////////////////////////////////////////////////////

#endif
