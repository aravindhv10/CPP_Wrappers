namespace Tensors {

    size_t constexpr
		SKIP_SIZE =
			4
	; //

    class Nothing {
    public:
        Nothing(){}
        ~Nothing(){}
    } ;

    namespace UNROLL {

        template <typename WORK_TYPE,size_t I>
        class UNROLLER {
        public:
            using TYPE_DATA =
                typename WORK_TYPE::TYPE_DATA
            ; //
            template <typename T>
            inline static TYPE_DATA
            ENSLAVE (
                T const &
                    in
            ) {
                return
                    WORK_TYPE::OPERATE (
                        WORK_TYPE::EVAL(in[I]) ,
                        UNROLLER<WORK_TYPE,I-1>::ENSLAVE(in)
                    )
                ; //
            } //
            template <typename T>
            inline static TYPE_DATA
            ENSLAVE (
                T const & in1,
                T const & in2
            ) {
                return
                    WORK_TYPE::OPERATE (
                        WORK_TYPE::EVAL(in1[I],in2[I]) ,
                        UNROLLER<WORK_TYPE,I-1>::ENSLAVE(in1,in2)
                    )
                ; //
            }
        } ;
        template <typename WORK_TYPE>
        class UNROLLER <WORK_TYPE,0> {
        public:
            using TYPE_DATA =
                typename WORK_TYPE::TYPE_DATA
            ;
            template <typename T>
            inline static TYPE_DATA
            ENSLAVE (
                T const &
                    in
            ) {
                return
                    WORK_TYPE::EVAL(in[0])
                ; //
            }
            template <typename T>
            inline static TYPE_DATA
            ENSLAVE (
                T const & in1,
                T const & in2
            ) {
                return
                    WORK_TYPE::EVAL(in1[0],in2[0])
                ; //
            }
        } ;

    }

    namespace SLAVES {
        //
        template <typename T>
        class SLAVE_L1_NORM {
        public:
            using TYPE_DATA =
                T
            ; //
            inline static TYPE_DATA
            EVAL (
                TYPE_DATA const &
                    in
            ) {
                return
                    CPPFileIO::mymod
                        (in)
                ; //
            }
            //
            inline static TYPE_DATA
            OPERATE (
                TYPE_DATA const in1,
                TYPE_DATA const in2
            ) {
                return
                    in1 + in2
                ; //
            }
            //
        } ;
        //
        template <typename T>
        class SLAVE_L2_NORM {
        public:
            using TYPE_DATA =
                T
            ; //
            inline static TYPE_DATA
            EVAL (
                TYPE_DATA const &
                    in
            ) {
                return
                    in * in
                ; //
            }
            inline static TYPE_DATA
            EVAL (
                TYPE_DATA const & in1 ,
                TYPE_DATA const & in2
            ) {
                return
                    in1 * in2
                ; //
            }
            //
            inline static TYPE_DATA
            OPERATE (
                TYPE_DATA const in1,
                TYPE_DATA const in2
            ) {
                return
                    in1 + in2
                ; //
            }
            //
        } ;
        //
    }

#include "./Operators/main.hh"

#include "./Array/D1.hh"

	namespace Array /* Pre-Declare important classes: */ {

		template <
			size_t		S_2	= 1		,
			size_t		S_1 = 1		,
			typename	TF  = float
		> class TRANSPOSE_2D		;

		template <
			size_t		S2 = 1		,
			size_t		SP = 1		,
			size_t		S1 = 1		,
			typename	Tf = float
		> class MULTIPLY_2D_P21P	;


		template <
			size_t		S2 = 1		,
			size_t		SP = 1		,
			size_t		S1 = 1		,
			typename	Tf = float
		> class MULTIPLY_2D_P2P1	;


		template <
			size_t		S2 = 1		,
			size_t		SP = 1		,
			size_t		S1 = 1		,
			typename	Tf = float
		> class MULTIPLY_2D_2P1P	;

		template <
			size_t		S2 = 1		,
			size_t		SP = 1		,
			size_t		S1 = 1		,
			typename	Tf = float
		> class MULTIPLY_2D_2PP1	;

	}

//////////////////////////////////////////////////////////////
#ifdef CBLAS_H 												//
	namespace FUNCTIONS /* Wrappers for cblas: */ { 		//
//////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////
_MACRO_MyGEMM_(float,cblas_sgemm)                           //
_MACRO_MyGEMM_(double,cblas_dgemm)                          //
#undef _MACRO_MyGEMM_                                       //
//////////////////////////////////////////////////////////////
inline CBLAS_TRANSPOSE constexpr    						//
SET_TRANSPOSE (bool const val) {    						//
	if(val){                        						//
		return CblasTrans ;         						//
	} else {                        						//
		return CblasNoTrans ;       						//
	}                               						//
}                                   						//
//////////////////////////////////////////////////////////////
	}														//
#endif														//
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

#include "./Array/D2.hh"
#include "./Array/D3.hh"
#include "./Array/D4.hh"
#include "./Array/D5.hh"

	namespace Array /* Transpose a 2D Array */ {

		template <
			size_t		S_2 ,
			size_t		S_1 ,
			typename	TF
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
				N2D_ARRAY <
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			; //

			using TYPE_O1 =
				N2D_ARRAY <
					SIZE_1()	,
					SIZE_2()	,
					TYPE_DATA
				>
			; //

			inline void
			EVAL () {

				for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE)
				for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE) {

					size_t const limit_1 =
						CPPFileIO::mymin (
							s1 + SKIP_SIZE ,
							SIZE_1()
						)
					; //

					size_t const limit_2 =
						CPPFileIO::mymin (
							s2 + SKIP_SIZE ,
							SIZE_2()
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
			operator () (
				TYPE_O1 & in
			) {
				O1 = & in ;
				EVAL();
			}

			template <size_t SIZE_3>
			inline MULTIPLY_2D_P2P1 <
				SIZE_1()	,
				SIZE_2()	,
				SIZE_3		,
				TYPE_DATA
			> operator * (
				N2D_ARRAY <
					SIZE_2(),
					SIZE_3,
					TYPE_DATA
				> const &
					other
			) const {

				MULTIPLY_2D_P2P1 <
					SIZE_1()	,
					SIZE_2()	,
					SIZE_3		,
					TYPE_DATA
				> ret (
					I1 ,
					& other
				) ;

				return
					ret
				;

			}

			template <size_t SIZE_3>
			inline MULTIPLY_2D_P21P <
				SIZE_1()	,
				SIZE_2()	,
				SIZE_3		,
				TYPE_DATA
			> operator * (
				TRANSPOSE_2D <
					SIZE_3		,
					SIZE_2()	,
					TYPE_DATA
				> const
					other
			) const {

				MULTIPLY_2D_P21P <
					SIZE_1()	,
					SIZE_2()	,
					SIZE_3		,
					TYPE_DATA
				> ret (
					I1			,
					other.I1
				) ;

				return
					ret
				; //

			}

			inline void
			operator () ()
			{ EVAL(); }

			TRANSPOSE_2D(){}

			TRANSPOSE_2D
			(TYPE_I1 const & in)
			{ I1 = & in ; }

			~TRANSPOSE_2D(){}

			TYPE_I1	const	*	I1 ;
			TYPE_O1			*	O1 ;

			template <
				size_t		S2	,
				size_t		S1	,
				typename	Tf
			> static inline
			TRANSPOSE_2D <S2,S1,Tf>
			GET (
				N2D_ARRAY <S2,S1,Tf> & in
			) {
				TRANSPOSE_2D <S2,S1,Tf> ret (in) ;
				return ret ;
			}

		} ;

	}

	namespace Array /* Multiplication */ {

//////////////////////////////////
#define _MACRO_MATRIX_(M,N2,N1)	\
using M =						\
	N2D_ARRAY <					\
		N2()		,			\
		N1()		,			\
		TYPE_DATA				\
	>							\
;								//
//////////////////////////////////////////////
#define _MACRO_DEFINE_HEAD_(CLASSNAME)		\
	template <								\
		size_t		S2 ,					\
		size_t		SP ,					\
		size_t		S1 ,					\
		typename	Tf						\
	> class CLASSNAME {						\
	public:									\
	static inline size_t constexpr			\
	SIZE_2 () { return S2 ; }				\
	static inline size_t constexpr			\
	SIZE_1 () { return S1 ; }				\
	static inline size_t constexpr			\
	SIZE_P () { return SP ; }				\
	using TYPE_DATA = Tf ;					\
	_MACRO_MATRIX_(TYPE_O1,SIZE_2,SIZE_1)	//
//////////////////////////////////////////////
#define _MACRO_DEFINE_TAIL_(CLASSNAME)	\
			inline void					\
			operator () (				\
				TYPE_O1 &				\
					in					\
			) {							\
				O1 = & in ;				\
				EVAL();					\
			}							\
			inline void					\
			operator () (				\
				TYPE_O1 *				\
					in					\
			) {							\
				O1 = in ;				\
				EVAL();					\
			}							\
			inline void					\
			operator () ()				\
			{EVAL();}					\
			CLASSNAME (					\
				TYPE_I1 const & _I1 ,	\
				TYPE_I2 const & _I2		\
			) {							\
				I1 = & _I1 ;			\
				I2 = & _I2 ;			\
			}							\
			CLASSNAME (					\
				TYPE_I1 const * _I1 ,	\
				TYPE_I2 const * _I2		\
			) {							\
				I1 = _I1 ;				\
				I2 = _I2 ;				\
			}							\
			CLASSNAME(){}				\
			~CLASSNAME(){}				\
			TYPE_I1	const	* I1 ;		\
			TYPE_I2	const	* I2 ;		\
			TYPE_O1			* O1 ;		\
		} ;								//
//////////////////////////////////////////
#define _MACRO_DEFINE_LIMITS_	\
size_t const limit_s2 =			\
	CPPFileIO::mymin (			\
		s2 + SKIP_SIZE ,		\
		SIZE_2()				\
	)							\
;								\
size_t const limit_sp =			\
	CPPFileIO::mymin (			\
		sp + SKIP_SIZE ,		\
		SIZE_P()				\
	)							\
;								\
size_t const limit_s1 =			\
	CPPFileIO::mymin (			\
		s1 + SKIP_SIZE ,		\
		SIZE_1()				\
	)							\
;								//
//////////////////////////////////

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

//////////////////////////////////
#undef _MACRO_DEFINE_LIMITS_	//
#undef _MACRO_DEFINE_TAIL_		//
#undef _MACRO_DEFINE_HEAD_		//
#undef _MACRO_MATRIX_			//
//////////////////////////////////

	}


//////////////////////////////////////////////////
#undef _MACRO_REGISTER_ALL_OPERATORS_           //
#undef _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_   //
//////////////////////////////////////////////////

    namespace FUNCTIONS {

#define _MACRO_MATRIX_ Array::N2D_ARRAY <2,2,TYPE_DATA,false>

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
