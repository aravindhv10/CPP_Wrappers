#ifndef _HEADER_GUARD_StaticArray_
#define _HEADER_GUARD_StaticArray_

/////////////////////////////////
// Pre-requisit Headers BEGIN: //
/////////////////////////////////
#include "./Headers.hh"
#include "./CPPFileIO.hh"
///////////////////////////////
// Pre-requisit Headers END. //
///////////////////////////////

namespace StaticArray {

	/////////////////////////////
	// Main Headers - 1 BEGIN: //
	/////////////////////////////
	#include "./StaticArray.dir/Nothing.hh"
	#include "./StaticArray.dir/UNROLL.hh"
	#include "./StaticArray.dir/SLAVES.hh"
	#include "./StaticArray.dir/ImportantHeads.hh"
	#include "./StaticArray.dir/Operator.hh"
	#include "./StaticArray.dir/D1.hh"
	#include "./StaticArray.dir/RadixSort.hh"
	#include "./StaticArray.dir/D2.hh"
	#include "./StaticArray.dir/D3.hh"
	#include "./StaticArray.dir/D4.hh"
	#include "./StaticArray.dir/D5.hh"
	#include "./StaticArray.dir/type_u128.hh"
	///////////////////////////
	// Main Headers - 1 END. //
	///////////////////////////

	template
	<	size_t S_2
	,	size_t S_1
	,	typename TF
	> class TRANSPOSE_2D {

	public:

		using TYPE_DATA =
			TF
		; //

		static inline size_t constexpr
		SIZE_1 () {

			return
				S_1
			; //

		}

		static inline size_t constexpr
		SIZE_2 () {

			return
				S_2
			; //

		}

		static inline size_t constexpr
		SIZE_Y () {

			return
				SIZE_2()
			; //

		}

		static inline size_t constexpr
		SIZE_X () {

			return
				SIZE_1()
			; //

		}

		using TYPE_I1 =
			N2D_ARRAY
			<	SIZE_2()
			,	SIZE_1()
			,	TYPE_DATA
			>
		; //

		using TYPE_O1 =
			N2D_ARRAY
			<	SIZE_1()
			,	SIZE_2()
			,	TYPE_DATA
			>
		; //

		inline void
		EVAL () {

			for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
			for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE) {

				size_t const limit_1 =
					CPPFileIO::mymin
					(	s1 + SKIP_SIZE
					,	SIZE_1()
					)
				; //

				size_t const limit_2 =
					CPPFileIO::mymin
					(	s2 + SKIP_SIZE
					,	SIZE_2()
					)
				; //

				for(size_t ts2=s2;ts2<limit_2;ts2++)
				for(size_t ts1=s1;ts1<limit_1;ts1++) {

					O1[0][ts1][ts2] =
						I1[0][ts2][ts1]
					; //

				}

			}

		}

		inline TYPE_I1 const &
		GET_TRANSPOSED () const {

			return
				I1[0]
			; //

		}

		inline void
		operator ()
		(	TYPE_O1 & in
		) {	O1 = & in ;
			EVAL();
		}

		template <size_t SIZE_3>
		inline MULTIPLY_2D_P2P1
		<	SIZE_1()
		,	SIZE_2()
		,	SIZE_3
		,	TYPE_DATA
		>
		operator * (
			N2D_ARRAY
			<	SIZE_2()
			,	SIZE_3
			,	TYPE_DATA
			> const &
				other
		) const {

			MULTIPLY_2D_P2P1
			<	SIZE_1()
			,	SIZE_2()
			,	SIZE_3
			,	TYPE_DATA
			>
				ret
				(	I1
				,	& other
				)
			; //

			return
				ret
			;

		}

		template <size_t SIZE_3>
		inline MULTIPLY_2D_P21P
		<	SIZE_1()
		,	SIZE_2()
		,	SIZE_3
		,	TYPE_DATA
		>
		operator * (
			TRANSPOSE_2D
			<	SIZE_3
			,	SIZE_2()
			,	TYPE_DATA
			> const
				other
		) const {

			MULTIPLY_2D_P21P
			<	SIZE_1()
			,	SIZE_2()
			,	SIZE_3
			,	TYPE_DATA
			>
				ret
				(	I1
				,	other.I1
				)
			; //

			return
				ret
			; //

		}

		inline void
		operator () ()
		{EVAL();}

		TRANSPOSE_2D(){}

		TRANSPOSE_2D
		(TYPE_I1 const & in)
		{I1=&in;}

		~TRANSPOSE_2D(){}

		TYPE_I1	const * I1 ;
		TYPE_O1 * O1 ;

		template
		<	size_t S2
		,	size_t S1
		,	typename Tf
		> static inline
		TRANSPOSE_2D <S2,S1,Tf>
		GET (
			N2D_ARRAY <S2,S1,Tf> & in
		) {
			TRANSPOSE_2D <S2,S1,Tf> ret (in) ;
			return ret ;
		}

	} ;

//////////////////////////////////
#define _MACRO_MATRIX_(M,N2,N1) \
using M =                       \
	N2D_ARRAY               \
	<	N2()            \
	,	N1()            \
	,	TYPE_DATA       \
	>                       \
;                               //
//////////////////////////////////

////////////////////////////////////////////////
#define _MACRO_DEFINE_HEAD_(CLASSNAME)        \
	template                              \
	<	size_t S2                     \
	,	size_t SP                     \
	,	size_t S1                     \
       	,	typename Tf                   \
	> class CLASSNAME {                   \
	public:                               \
	static inline size_t constexpr        \
	SIZE_2 () { return S2 ; }             \
	static inline size_t constexpr        \
	SIZE_1 () { return S1 ; }             \
	static inline size_t constexpr        \
	SIZE_P () { return SP ; }             \
	using TYPE_DATA = Tf ;                \
	_MACRO_MATRIX_(TYPE_O1,SIZE_2,SIZE_1) //
////////////////////////////////////////////////

/////////////////////////////////////////
#define _MACRO_DEFINE_TAIL_(CLASSNAME) \
                                       \
	inline void                    \
	operator ()                    \
	(	TYPE_O1 & in           \
	) {	O1 = & in ;            \
		EVAL();                \
	}                              \
                                       \
	inline void                    \
	operator ()                    \
	(	TYPE_O1 * in           \
	) {                            \
		O1 = in ;              \
		EVAL();                \
	}                              \
                                       \
	inline void                    \
	operator () ()                 \
	{EVAL();}                      \
                                       \
	CLASSNAME                      \
	(	TYPE_I1 const & _I1    \
	,	TYPE_I2 const & _I2    \
	) {	I1 = & _I1 ;           \
		I2 = & _I2 ;           \
	}                              \
                                       \
	CLASSNAME                      \
	(	TYPE_I1 const * _I1    \
	,	TYPE_I2 const * _I2    \
	) {	I1 = _I1 ;             \
		I2 = _I2 ;             \
	}                              \
                                       \
	CLASSNAME(){}                  \
                                       \
	~CLASSNAME(){}                 \
                                       \
	TYPE_I1	const * I1 ;           \
	TYPE_I2	const * I2 ;           \
	TYPE_O1	* O1 ;                 \
                                       \
	} ;                            //
/////////////////////////////////////////

/////////////////////////////////
#define _MACRO_DEFINE_LIMITS_  \
                               \
size_t const limit_s2 =        \
	CPPFileIO::mymin       \
	(	s2 + SKIP_SIZE \
	,	SIZE_2()       \
	)                      \
;                              \
                               \
size_t const limit_sp =        \
	CPPFileIO::mymin       \
	(	sp + SKIP_SIZE \
	,	SIZE_P()       \
	)                      \
;                              \
                               \
size_t const limit_s1 =        \
	CPPFileIO::mymin       \
	(	s1 + SKIP_SIZE \
	,	SIZE_1()       \
	)                      \
;                              \
                               //
/////////////////////////////////

_MACRO_DEFINE_HEAD_(MULTIPLY_2D_P21P)

	_MACRO_MATRIX_(TYPE_I1,SIZE_P,SIZE_2)
	_MACRO_MATRIX_(TYPE_I2,SIZE_1,SIZE_P)

	inline void
	EVAL () {

		O1->MULTIPLY_P21P(I1[0],I2[0]);
		return;

		for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
		for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE)
		for(size_t sp=0;sp<SIZE_P();sp+=SKIP_SIZE) {

			_MACRO_DEFINE_LIMITS_

			for(size_t s2_2=s2;s2_2<limit_s2;s2_2+=1)
			for(size_t s1_2=s1;s1_2<limit_s1;s1_2+=1)
			for(size_t sp_2=sp;sp_2<limit_sp;sp_2+=1) {

				O1[0][s2_2][s1_2] +=
					I1[0][sp_2][s2_2] *
					I2[0][s1_2][sp_2]
				; //

			}

		}

	}

_MACRO_DEFINE_TAIL_(MULTIPLY_2D_P21P)

_MACRO_DEFINE_HEAD_(MULTIPLY_2D_P2P1)

	_MACRO_MATRIX_(TYPE_I1,SIZE_P,SIZE_2)
	_MACRO_MATRIX_(TYPE_I2,SIZE_P,SIZE_1)

	inline void
	EVAL () {

		O1->MULTIPLY_P2P1(I1[0],I2[0]);
		return;

		for(size_t sp=0;sp<SIZE_P();sp+=SKIP_SIZE)
		for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
		for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE) {

			_MACRO_DEFINE_LIMITS_

			for(size_t sp_2=sp;sp_2<limit_sp;sp_2+=1)
			for(size_t s2_2=s2;s2_2<limit_s2;s2_2+=1)
			for(size_t s1_2=s1;s1_2<limit_s1;s1_2+=1) {

				O1[0][s2_2][s1_2] +=
					I1[0][sp_2][s2_2] *
					I2[0][sp_2][s1_2]
				; //

			}

		}



	}

_MACRO_DEFINE_TAIL_(MULTIPLY_2D_P2P1)

_MACRO_DEFINE_HEAD_(MULTIPLY_2D_2P1P)

	_MACRO_MATRIX_(TYPE_I1,SIZE_2,SIZE_P)
	_MACRO_MATRIX_(TYPE_I2,SIZE_1,SIZE_P)

	inline void
	EVAL () {

		O1->MULTIPLY_2P1P(I1[0],I2[0]);
		return;

		for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
		for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE)
		for(size_t sp=0;sp<SIZE_P();sp+=SKIP_SIZE) {

			_MACRO_DEFINE_LIMITS_

			for(size_t s2_2=s2;s2_2<limit_s2;s2_2+=1)
			for(size_t s1_2=s1;s1_2<limit_s1;s1_2+=1)
			for(size_t sp_2=sp;sp_2<limit_sp;sp_2+=1) {

				O1[0][s2_2][s1_2] +=
					I1[0][s2_2][sp_2] *
					I2[0][s1_2][sp_2]
				; //

			}

		}

	}

_MACRO_DEFINE_TAIL_(MULTIPLY_2D_2P1P)

_MACRO_DEFINE_HEAD_(MULTIPLY_2D_2PP1)

_MACRO_MATRIX_(TYPE_I1,SIZE_2,SIZE_P)
_MACRO_MATRIX_(TYPE_I2,SIZE_P,SIZE_1)

	inline void
	EVAL () {

		O1->MULTIPLY_2PP1(I1[0],I2[0]);
		return ;

		for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
		for(size_t sp=0;sp<SIZE_P();sp+=SKIP_SIZE)
		for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE) {

			_MACRO_DEFINE_LIMITS_

			for(size_t s2_2=s2;s2_2<limit_s2;s2_2+=1)
			for(size_t sp_2=sp;sp_2<limit_sp;sp_2+=1)
			for(size_t s1_2=s1;s1_2<limit_s1;s1_2+=1) {

				O1[0][s2_2][s1_2] +=
					I1[0][s2_2][sp_2] *
					I2[0][sp_2][s1_2]
				; //

			}

		}

	}

_MACRO_DEFINE_TAIL_(MULTIPLY_2D_2PP1)

///////////////////////////////
#undef _MACRO_DEFINE_LIMITS_ //
#undef _MACRO_DEFINE_TAIL_   //
#undef _MACRO_DEFINE_HEAD_   //
#undef _MACRO_MATRIX_        //
///////////////////////////////

//////////////////////////////////////////////////
#undef _MACRO_REGISTER_ALL_OPERATORS_           //
#undef _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_   //
//////////////////////////////////////////////////

    namespace FUNCTIONS {

#define _MACRO_MATRIX_ N2D_ARRAY <2,2,TYPE_DATA,false>

        template <typename TYPE_DATA>
        inline _MACRO_MATRIX_
        INVERT_MATRIX (
            _MACRO_MATRIX_ const &
                in
        ) {
            _MACRO_MATRIX_ ret ;
            /* Prepare the inverse: */ {
                ret[0][0] = + in[1][1] ;
                ret[0][1] = - in[0][1] ;
                ret[1][0] = - in[1][0] ;
                ret[1][1] = + in[0][0] ;
                TYPE_DATA const det =
                    + (in[0][0]*in[1][1])
                    - (in[0][1]*in[1][0])
                ;
                ret /= det ;
            }
            return ret ;
        }

#undef _MACRO_MATRIX_

    }

}
#endif
