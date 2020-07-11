namespace Tensors {

    size_t constexpr SKIP_SIZE = 16 ;

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

    namespace Array /* 1 D Array */ {

////////////////////////////////////////////////////////////////
#define _MACRO_ND_LOOP_(I) for(size_t I=0;I<SIZE();I++) ////////
////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name,Sign)   \
        inline void Name ( const TYPE_DATA & other ) {         \
            _MACRO_ND_LOOP_(x)                                 \
            { DATA[x] Sign other ; }                           \
        }                                                      \
        inline void operator Sign ( const TYPE_DATA other )    \
        { Name (other) ; } /////////////////////////////////////
////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Name,Sign)   \
        inline void Name ( const TYPE_SELF & other ) {         \
            _MACRO_ND_LOOP_(x)                                 \
            { DATA[x] Sign other[x] ; }                        \
        }                                                      \
        inline void operator Sign ( const TYPE_SELF & other )  \
        { Name (other) ; }                                     \
        _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name,Sign) ///
////////////////////////////////////////////////////////////////

        template <size_t X, typename T=float>
        class ND_ARRAY {
        public:
            using TYPE_DATA = T ;
            //
            inline static
            size_t constexpr
            SIZE() {
                return
                    X
                ; //
            }
            //
            inline static
            size_t constexpr
            SIZE_1() {
                return
                    SIZE()
                ; //
            }
            //
            using TYPE_SELF =
                ND_ARRAY <
                    SIZE(),
                    TYPE_DATA
                >
            ; //
        private:
            TYPE_DATA
                DATA[SIZE()]
            ; //
        public:
            //
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Add,+=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Sub,-=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Mul,*=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Div,/=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Eqt,=)
            //
            inline TYPE_SELF const
            operator * (
                TYPE_DATA const
                    other
            ) const {
                TYPE_SELF
                    ret
                ; /* Prepare the return value : */ {
                    _MACRO_ND_LOOP_(x) {
                        ret[x] =
                            DATA[x] *
                            other
                        ; //
                    }
                }
                return
                    ret
                ;
            } //
            inline TYPE_SELF const
            operator / (
                TYPE_DATA const
                    other
            ) const {
                TYPE_SELF
                    ret
                ; /* Prepare the return value : */ {
                    _MACRO_ND_LOOP_(x) {
                        ret[x] =
                            DATA[x] /
                            other
                        ; //
                    }
                }
                return
                    ret
                ;
            } //
            inline TYPE_DATA
            Dot (
                const TYPE_SELF &
                    other
            ) const {
                TYPE_DATA ret =
                    0
                ; /* Prepare the matrix: */ {
                    _MACRO_ND_LOOP_(i) {
                        ret +=
                            DATA[i] *
                            other[i]
                        ;
                    }
                }
                return
                    ret
                ; //
            } //
            inline void * CopyFrom
            ( const void * __restrict__ other ) {
                return (void *) memcpy (
                    (       void * ) /* dest = */ DATA  ,
                    ( const void * ) /* src  = */ other ,
                    (     size_t   ) /* n    = */ sizeof (TYPE_SELF)
                ) ;
            } //
            inline void *
            CopyFrom (
                const TYPE_DATA *
                    __restrict__ other
            ) {
                return CopyFrom (
                    ( const void * )
                        /* other = */ other
                ) ;
            } //
            inline void * CopyFrom
            ( const TYPE_SELF other ) {
                return CopyFrom (
                    ( const TYPE_DATA * )
                        /* other = */ other.DATA
                ) ;
            } //
            //
            inline TYPE_SELF &
            FLATTEN () {
                return
                    this[0]
                ; //
            } //
            inline TYPE_SELF const &
            FLATTEN () const {
                return
                    this[0]
                ; //
            } //
            inline void
            GET_SQUARED (
                const TYPE_SELF &
                    other
            ) {
                _MACRO_ND_LOOP_(x) {
                    DATA[x] =
                        other[x] *
                        other[x]
                    ; //
                }
            } //
            inline void
            ADD_SQUARED (
                const TYPE_SELF &
                    other
            ) {
                _MACRO_ND_LOOP_(x) {
                    DATA[x] +=
                        other[x] *
                        other[x]
                    ; //
                }
            } //
            inline TYPE_DATA
            operator * (
                const TYPE_SELF &
                    other
            ) const {
                return
                    Dot (other)
                ; //
            }
            //
            ND_ARRAY(){}
            //
            inline TYPE_DATA &
            operator [] (size_t x)
            { return DATA[x] ; }
            //
            inline TYPE_DATA const
            operator [] (size_t x) const
            { return DATA[x] ; }
            //
            inline TYPE_DATA *
            GET_DATA () {
                return
                    reinterpret_cast
                        < TYPE_DATA * >
                        ( DATA )
                ; //
            } //
            inline TYPE_DATA const *
            GET_DATA () const {
                return
                    reinterpret_cast
                        < TYPE_DATA const * >
                        ( DATA )
                ; //
            } //
            inline TYPE_DATA
            L1_NORM () const {
                TYPE_DATA Ret =
                    0
                ; /* Prepare the output: */ {
                    _MACRO_ND_LOOP_(i) {
                        Ret +=
                            CPPFileIO::mymod
                                (DATA[i])
                        ; //
                    }
                }
                return
                    Ret
                ; //
            } //
            inline TYPE_DATA
            L2_NORM () const {
                TYPE_DATA Ret = 0 ;
                _MACRO_ND_LOOP_(i) {
                    Ret +=
                        DATA[i] *
                        DATA[i]
                    ; //
                }
                return Ret ;
            } //
            inline TYPE_SELF
            operator + (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                _MACRO_ND_LOOP_(i) {
                    ret[i] =
                        DATA[i] +
                        other[i]
                    ;
                }
                return ret ;
            } //
            inline TYPE_SELF
            operator - (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                _MACRO_ND_LOOP_(i) {
                    ret[i] =
                        DATA[i] -
                        other[i]
                    ;
                }
                return ret ;
            } //
        } ;

//////////////////////////////////////////////////////
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_	//
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_	//
#undef _MACRO_ND_LOOP_								//
//////////////////////////////////////////////////////

    }

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


    namespace Array /* 2 D Array */ {

		template <
			size_t		S_2 ,
			size_t		S_1 ,
			typename	TF
		> class TRANSPOSE_2D ;

        template <
            size_t S_2 , size_t S_1 ,
            typename T = float ,
            bool Transposed = false
        > class N2D_ARRAY : public
        ND_ARRAY <S_2,ND_ARRAY<S_1,T>> {
        public:
            //
            using TYPE_DATA = T ;
            //
            inline static size_t constexpr
            SIZE_X ()
            { return S_1 ; }
            //
            inline static size_t constexpr
            SIZE_1 ()
            { return S_1 ; }
            //
            inline static size_t constexpr
            SIZE_Y ()
            { return S_2 ; }
            //
            inline static size_t constexpr
            SIZE_2 ()
            { return S_2 ; }
            //
            inline static size_t constexpr
            SIZE () {
                return
                    SIZE_1() *
                    SIZE_2()
                ; //
            }
            //
			inline void
			operator = (
				TRANSPOSE_2D <
					SIZE_1()	,
					SIZE_2()	,
					TYPE_DATA
				> & in
			) {
				in.O1 = this ;
				in () ;
			}
			//
			N2D_ARRAY (
				TRANSPOSE_2D <
					SIZE_1()	,
					SIZE_2()	,
					TYPE_DATA
				> & in
			) {
				in.O1 = this ;
				in () ;
			}
            //
            using TYPE_FLATTEN =
                ND_ARRAY
                    <SIZE(),TYPE_DATA>
            ; //
            using TYPE_VECTOR_DATA =
                ND_ARRAY
                    <SIZE_1(),TYPE_DATA>
            ; //
            inline TYPE_FLATTEN const &
            FLATTEN () const {
                return
                    reinterpret_cast
                        < TYPE_FLATTEN const & >
                        ( this[0] )
                ; //
            } //
            inline TYPE_FLATTEN &
            FLATTEN () {
                return
                    reinterpret_cast
                        < TYPE_FLATTEN & >
                        ( this[0] )
                ; //
            } //
            inline TYPE_DATA const
            TRACE () const {
                TYPE_DATA ret = 0 ;
                size_t Min =
                    CPPFileIO::mymin(
                        SIZE_1(),
                        SIZE_2()
                    )
                ;
                for(size_t i=0;i<Min;i++){
                    ret+=this[0][i][i];
                }
                return ret ;
            } //
            static inline bool constexpr
            IS_TRANSPOSED ()
            {return Transposed;}
            //
            using TYPE_SELF =
                N2D_ARRAY <
                    SIZE_2()   ,
                    SIZE_1()   ,
                    TYPE_DATA  ,
                    IS_TRANSPOSED()
                >
            ; //
            using TYPE_TRANSPOSED =
                N2D_ARRAY <
                    SIZE_2()   ,
                    SIZE_1()   ,
                    TYPE_DATA  ,
                    !IS_TRANSPOSED()
                >
            ; //
            using TYPE_PARENT =
                ND_ARRAY <
                    SIZE_2() ,
                    TYPE_VECTOR_DATA
                >
            ; //

//////////////////////////////
#define _MACRO_MATRIX_P_Y_	\
    N2D_ARRAY <				\
        P, SIZE_2(),		\
        TYPE_DATA,			\
        true				\
    >						//
//////////////////////////////
#define _MACRO_MATRIX_Y_P_	\
    N2D_ARRAY <				\
        SIZE_2(), P,		\
        TYPE_DATA,			\
        false				\
    >						//
//////////////////////////////
#define _MACRO_MATRIX_P_X_	\
    N2D_ARRAY <				\
        P, SIZE_1(),		\
        TYPE_DATA,			\
        false				\
    >						//
//////////////////////////////
#define _MACRO_MATRIX_X_P_	\
    N2D_ARRAY <				\
        SIZE_1(), P,		\
        TYPE_DATA,			\
        true				\
    >						//
//////////////////////////////

            inline TYPE_PARENT &
            AS_PARENT () {
                return
                    reinterpret_cast
                        <TYPE_PARENT &>
                        (this[0])
                ; //
            } //
            inline TYPE_PARENT const &
            AS_PARENT () const {
                return
                    reinterpret_cast
                        <TYPE_PARENT const &>
                        (this[0])
                ; //
            } //

            inline void
            operator = (
                TYPE_VECTOR_DATA const &
                    other
            ) {
                AS_PARENT()=other;
            } //

            inline TYPE_TRANSPOSED &
            MARK_TRANSPOSED () {
                return
                    reinterpret_cast
                        <TYPE_TRANSPOSED &>
                        (this[0])
                ; //
            }
            inline TYPE_TRANSPOSED const &
            MARK_TRANSPOSED () const {
                return
                    reinterpret_cast
                        <TYPE_TRANSPOSED const &>
                        (this[0])
                ; //
            } //
            //
            inline void
            GET_SQUARED (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()
                    .GET_SQUARED (
                        other.FLATTEN()
                    )
                ; //
            }
            inline void
            ADD_SQUARED (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()
                    .ADD_SQUARED (
                        other.FLATTEN()
                    )
                ; //
            }
            //
            _MACRO_REGISTER_ALL_OPERATORS_
            //
            inline TYPE_DATA const
            L1_NORM () const {
                return
                    FLATTEN()
                    .L1_NORM()
                ; //
            }
            //
            inline TYPE_DATA const
            L2_NORM () const {
                return
                    FLATTEN()
                    .L2_NORM()
                ; //
            }
            //
            inline TYPE_DATA *
            GET_DATA () {
                return
                    FLATTEN()
                    .GET_DATA()
                ; //
            }
            //
            inline TYPE_DATA const *
            GET_DATA () const {
                return
                    FLATTEN()
                    .GET_DATA()
                ; //
            }
            //
            inline void
            SET_DIAGONAL (
                TYPE_DATA const in
            ) {
                for (
                    size_t y=0;
                    y<SIZE_2();
                    y++
                ) for (
                    size_t x=0;
                    x<SIZE_1();
                    x++
                ) {
                    if (y==x) {
                        this[0][y][x] =
                            in
                        ;
                    } else {
                        this[0][y][x] =
                            0
                        ;
                    }

                }
            }
            //
            N2D_ARRAY () {}
            //
            inline ND_ARRAY <
                SIZE_2() ,
                TYPE_DATA
            > operator * (
                TYPE_VECTOR_DATA const &
                    In
            ) const {
                ND_ARRAY <
                    SIZE_2(),
                    TYPE_DATA
                > ret ; //
                for(size_t y=0;y<SIZE_2();y++){
                    ret[y] =
                        this[0][y] * In
                    ; //
                }
                return ret ;
            }
            //
            inline void
            operator += (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()	+=
                    other.FLATTEN()
                ;
            }
            inline void
            operator -= (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()	-=
                    other.FLATTEN()
                ;
            }
            //
            inline TYPE_SELF
            operator + (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                for (
                    size_t i=0;
                    i<SIZE();
                    i++
                ) {
                    ret.FLATTEN()[i]=
                        FLATTEN()[i]+
                        other.FLATTEN()[i]
                    ; //
                }
                return ret ;
            }
            //
            inline TYPE_SELF
            operator - (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                for (
                    size_t i=0;
                    i<SIZE();
                    i++
                ) {
                    ret.FLATTEN()[i]=
                        FLATTEN()[i]-
                        other.FLATTEN()[i]
                    ; //
                }
                return ret ;
            }
            //
            inline TYPE_SELF
            operator / (
                TYPE_DATA const &
                    other
            ) const {
                TYPE_SELF
                    ret
                ; //
                for(size_t i=0;i<SIZE();i++){
                    ret.FLATTEN()[i] =
                        FLATTEN()[i] /
                        other
                    ; //
                }
                return
                    ret
                ; //
            }
            //
            inline TYPE_SELF
            operator * (
                TYPE_DATA const &
                    other
            ) const {
                TYPE_SELF
                    ret
                ; //
                for(size_t i=0;i<SIZE();i++){
                    ret.FLATTEN()[i] =
                        FLATTEN()[i] *
                        other
                    ; //
                }
                return
                    ret
                ; //
            }
            //
            inline TYPE_DATA const
            operator () (
                size_t y,
                size_t x
            ) const {
                return
                    this[0][y][x]
                ; //
            }
            //
            inline TYPE_DATA &
            operator () (
                size_t y,
                size_t x
            ) {
                return
                    this[0][y][x]
                ; //
            }

            template <size_t P>
            inline void
            MULTIPLY (
                _MACRO_MATRIX_Y_P_ const & M ,
                _MACRO_MATRIX_X_P_ const & N
            ) {
                for(size_t y=0;y<SIZE_2();y++)
                for(size_t x=0;x<SIZE_1();x++)
                { this[0][y][x] = M[y] * N[x] ; }
            } //

            template <size_t P>
            inline void
            MULTIPLY (
                _MACRO_MATRIX_P_Y_ const & M ,
                _MACRO_MATRIX_P_X_ const & N
            ) {
                for ( size_t y  = 0 ; y < SIZE_2()	; y = y + SKIP_SIZE )
                for ( size_t x  = 0 ; x < SIZE_1()	; x = x + SKIP_SIZE )
                for ( size_t p  = 0 ; p < P			; p = p + 1			)
                for ( size_t y2 = y ; ((y2<y+SKIP_SIZE)&&(y2<SIZE_2())) ; y2++ )
                for ( size_t x2 = x ; ((x2<x+SKIP_SIZE)&&(x2<SIZE_1())) ; x2++ )
                { this[0][y2][x2] += M[p][y2] * N[p][x2] ; }
            } //

            template <size_t P>
            inline void
            MULTIPLY (
                _MACRO_MATRIX_Y_P_ const & M ,
                _MACRO_MATRIX_P_X_ const & N
            ) {
                for ( size_t p  = 0 ; p < P										; p  = p + SKIP_SIZE	)
                for ( size_t x  = 0 ; x < SIZE_1() 								; x  = x + SKIP_SIZE	)
                for ( size_t y  = 0 ; y < SIZE_2() 								; y  = y  + 1			)
                for ( size_t p2 = p ; ( (p2<(p+SKIP_SIZE)) && (p2<P) )			; p2 = p2 + 1 			)
                for ( size_t x2 = x ; ( (x2<(x+SKIP_SIZE)) && (x2<SIZE_1()) )	; x2 = x2 + 1			)
                { this[0][y][x2] += M[y][p2] * N[p2][x2] ; }
            }

            template <size_t P>
            inline _MACRO_MATRIX_Y_P_
            operator * (
                N2D_ARRAY <
                    SIZE_1()  ,
                    P         ,
                    TYPE_DATA
                > const & In
            ) const {
                _MACRO_MATRIX_Y_P_ ret ;
                ret = 0 ;
                ret.MULTIPLY(this[0],In);
                return ret ;
            }

//////////////////////////////
#undef _MACRO_MATRIX_X_P_	//
#undef _MACRO_MATRIX_P_X_	//
#undef _MACRO_MATRIX_Y_P_	//
#undef _MACRO_MATRIX_P_Y_	//
//////////////////////////////

//////////////////////////////////////////////////////////
#ifdef CBLAS_H											//
            template <									//
                size_t Ax, size_t Ay,					//
                size_t Bx, size_t By,					//
                bool TransA, bool TransB				//
            > inline void								//
            MyGEMM (									//
                TYPE_DATA const Beta ,					//
                N2D_ARRAY <Ay,Ax,TYPE_DATA,TransA> A ,	//
                N2D_ARRAY <By,Bx,TYPE_DATA,TransB> B ,	//
                TYPE_DATA const Alpha					//
            ) {											//
                using namespace FUNCTIONS ;				//
                size_t const							//
                    tmpz = (							//
                        + A.SIZE_2() + A.SIZE_1()		//
                        + B.SIZE_2() + B.SIZE_1()		//
                        - SIZE_2()   - SIZE_1()			//
                    ) / 2								//
                ;										//
                FUNCTIONS::MyGEMM (						//
                    FUNCTIONS::SET_TRANSPOSE (			//
                        A.IS_TRANSPOSED()				//
                    ) ,									//
                    FUNCTIONS::SET_TRANSPOSE (			//
                        B.IS_TRANSPOSED()				//
                    ) ,									//
                    SIZE_2() , SIZE_1() , tmpz ,		//
                    Alpha ,								//
                    A.GET_DATA() , A.SIZE_1() ,			//
                    B.GET_DATA() , B.SIZE_1() ,			//
                    Beta , GET_DATA()					//
                ) ;										//
            }											//
#endif													//
//////////////////////////////////////////////////////////

        } ;
    }

	namespace Array /* Transpose a 2D Array */ {

		template <
			size_t		S_2	=	1		,
			size_t		S_1	=	1		,
			typename	TF	=	float
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

				for(size_t s2=0;s2<SIZE_2();s2+=SKIP_SIZE){
					for(size_t s1=0;s1<SIZE_1();s1+=SKIP_SIZE){

						size_t const limit_1 =
							CPPFileIO::mymin(
								s1+SKIP_SIZE,
								SIZE_1()
							)
						; //

						size_t const limit_2 =
							CPPFileIO::mymin(
								s2+SKIP_SIZE,
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


			}

			inline void
			operator () (
				TYPE_O1 & in
			) {
				O1 = & in ;
				EVAL();
			}

			inline void
			operator () ()
			{ EVAL(); }

			TRANSPOSE_2D(){}

			TRANSPOSE_2D
			(TYPE_I1 & in)
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

    namespace Array /* 3 D Array */ {

        template <
            size_t S_3 ,
            size_t S_2 ,
            size_t S_1 ,
            typename T = float
        > class N3D_ARRAY : public
        ND_ARRAY <S_3,N2D_ARRAY<S_2,S_1,T>> {
        public:
            //
            using TYPE_DATA = T ;
            //
            inline static constexpr
            size_t SIZE_1 ()
            { return S_1;}
            //
            inline static constexpr
            size_t SIZE_X ()
            { return SIZE_1();}
            //
            inline static constexpr
            size_t SIZE_2 ()
            { return S_2;}
            //
            inline static constexpr
            size_t SIZE_Y ()
            { return SIZE_2();}
            //
            inline static constexpr
            size_t SIZE_3 ()
            { return S_3;}
            //
            inline static constexpr
            size_t SIZE_Z ()
            { return SIZE_3();}
            //
            inline static constexpr
            size_t SIZE () {
                return
                    SIZE_1() *
                    SIZE_2() *
                    SIZE_3()
                ; //
            }
            //
            using TYPE_FLATTEN =
                ND_ARRAY <
                    SIZE()   ,
                    TYPE_DATA
                >
            ; //
            //
            inline TYPE_FLATTEN const &
            FLATTEN () const {
                return
                    reinterpret_cast
                        < TYPE_FLATTEN const & >
                        ( this[0] )
                ; //
            }
            //
            inline TYPE_FLATTEN &
            FLATTEN () {
                return
                    reinterpret_cast
                        < TYPE_FLATTEN & >
                        ( this[0] )
                ; //
            }
            //
            using TYPE_SELF =
                N3D_ARRAY <
                    SIZE_3()  ,
                    SIZE_2()  ,
                    SIZE_1()  ,
                    TYPE_DATA
                >
            ; //
            //
            using TYPE_PARENT =
                ND_ARRAY <
                    SIZE_3(),
                    N2D_ARRAY <
                        SIZE_2(),
                        SIZE_1(),
                        TYPE_DATA
                    >
                >
            ; //
            //
            inline void GET_SQUARED
            ( TYPE_SELF const & other ) {
                FLATTEN().GET_SQUARED
                    (other.FLATTEN())
                ; //
            }
            //
            _MACRO_REGISTER_ALL_OPERATORS_
            //
            inline TYPE_DATA const
            L1_NORM () const {
                return
                    FLATTEN()
                    .L1_NORM()
                ; //
            }
            //
            inline TYPE_DATA const
            L2_NORM () const {
                return
                    FLATTEN()
                    .L2_NORM()
                ; //
            }
            //
            inline TYPE_DATA *
            GET_DATA () {
                return
                    FLATTEN()
                    .GET_DATA()
                ; //
            }
            //
            inline TYPE_DATA const *
            GET_DATA () const {
                return
                    FLATTEN()
                    .GET_DATA()
                ; //
            }
            //
        } ;
    }

    namespace Array /* 4 D Array */ {
        template <
            size_t	S_4	,	size_t S_3	,
            size_t	S_2	,	size_t S_1	,
            typename	TF
        > class N4D_ARRAY : public
        ND_ARRAY <
            S_4	,
            N3D_ARRAY <
                S_3,S_2,S_1,TF
            >
        > {
        public:

            using TYPE_DATA = TF ;

            static inline size_t constexpr
            SIZE_4 () { return S_4 ; }

            static inline size_t constexpr
            SIZE_3 () { return S_3 ; }

            static inline size_t constexpr
            SIZE_2 () { return S_2 ; }

            static inline size_t constexpr
            SIZE_1 () { return S_1 ; }

            static inline size_t constexpr
            SIZE () {
                return
                    SIZE_4()	*
                    SIZE_3()	*
                    SIZE_2()	*
                    SIZE_1()
                ; //
            }

            using TYPE_FLATTEN =
                ND_ARRAY <
                    SIZE(),
                    TYPE_DATA
                >
            ;

            inline TYPE_FLATTEN &
            FLATTEN() {
                return
                    reinterpret_cast
                        <TYPE_FLATTEN &>
                        (this[0])
                ; //
            }

            inline TYPE_FLATTEN const &
            FLATTEN() const {
                return
                    reinterpret_cast
                        <TYPE_FLATTEN const &>
                        (this[0])
                ; //
            }

            _MACRO_REGISTER_ALL_OPERATORS_

        } ;
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
