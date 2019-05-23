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

    namespace NN /* 1 D Array */ {

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
                return X;
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
        public:
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


    namespace NN /* 2 D Array */ {

        template <
            size_t Y , size_t X ,
            typename T = float ,
            bool Transposed = false
        > class N2D_ARRAY : public
        ND_ARRAY <Y,ND_ARRAY<X,T>> {
        public:
            //
            using TYPE_DATA = T ;
            //
            inline static size_t constexpr
            SIZE_X ()
			{ return X ; }
            //
            inline static size_t constexpr
            SIZE_Y ()
			{ return Y ; }
            //
            inline static size_t constexpr
            SIZE () {
                return
                    SIZE_X() *
                    SIZE_Y()
                ; //
            }
            //
            using TYPE_FLATTEN =
                ND_ARRAY
                    <SIZE(),TYPE_DATA>
            ; //
            using TYPE_VECTOR_DATA =
                ND_ARRAY
                    <SIZE_X(),TYPE_DATA>
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
						SIZE_X(),
						SIZE_Y()
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
                    SIZE_Y()   ,
                    SIZE_X()   ,
                    TYPE_DATA  ,
                    Transposed
                >
            ; //
            using TYPE_TRANSPOSED =
                N2D_ARRAY <
                    SIZE_Y()   ,
                    SIZE_X()   ,
                    TYPE_DATA  ,
                    !IS_TRANSPOSED()
                >
            ; //
            using TYPE_PARENT =
                ND_ARRAY <
                    SIZE_Y() ,
                    ND_ARRAY <
                        SIZE_X() ,
                        TYPE_DATA
                    >
                >
            ; //

//////////////////////////////
#define _MACRO_MATRIX_P_Y_	\
	N2D_ARRAY <				\
		P, SIZE_Y(),		\
		TYPE_DATA,			\
		true				\
	>						//
//////////////////////////////
#define _MACRO_MATRIX_Y_P_	\
	N2D_ARRAY <				\
		SIZE_Y(), P,		\
		TYPE_DATA,			\
		false				\
	>						//
//////////////////////////////
#define _MACRO_MATRIX_P_X_	\
	N2D_ARRAY <				\
		P, SIZE_X(),		\
		TYPE_DATA,			\
		false				\
	>						//
//////////////////////////////
#define _MACRO_MATRIX_X_P_	\
	N2D_ARRAY <				\
		SIZE_X(), P,		\
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
					y<SIZE_Y();
					y++
				) for (
					size_t x=0;
					x<SIZE_X();
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
                SIZE_Y() ,
                TYPE_DATA
            > operator * (
                ND_ARRAY <SIZE_X(),TYPE_DATA> const &
                    In
            ) {
                ND_ARRAY <
                    SIZE_Y(),
                    TYPE_DATA
                > ret ; //
                ret = 0 ;
                for(size_t y=0;y<SIZE_Y();y++){
                    for(size_t x=0;x<SIZE_X();x++){
                        ret[y]+=
                            this[0][y][x]*
                            In[x]
                        ; //
                    }
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
				_MACRO_MATRIX_P_Y_ const & M ,
				_MACRO_MATRIX_P_X_ const & N
			) {
				for ( size_t y  = 0 ; y < SIZE_Y()	; y = y + SKIP_SIZE )
				for ( size_t x  = 0 ; x < SIZE_X()	; x = x + SKIP_SIZE )
				for ( size_t p  = 0 ; p < P			; p = p + 1			)
				for ( size_t y2 = y ; ((y2<y+SKIP_SIZE)&&(y2<SIZE_Y())) ; y2++ )
				for ( size_t x2 = x ; ((x2<x+SKIP_SIZE)&&(x2<SIZE_X())) ; x2++ )
				{ this[0][y2][x2] += M[p][y2] * N[p][x2] ; }
			} //
			template <size_t P>
			inline void
			MULTIPLY (
				_MACRO_MATRIX_Y_P_ const & M ,
				_MACRO_MATRIX_P_X_ const & N
			) {
				for ( size_t p  = 0 ; p < P										; p  = p + SKIP_SIZE	)
				for ( size_t x  = 0 ; x < SIZE_X() 								; x  = x + SKIP_SIZE	)
				for ( size_t y  = 0 ; y < SIZE_Y() 								; y  = y  + 1			)
				for ( size_t p2 = p ; ( (p2<(p+SKIP_SIZE)) && (p2<P) )			; p2 = p2 + 1 			)
				for ( size_t x2 = x ; ( (x2<(x+SKIP_SIZE)) && (x2<SIZE_X()) )	; x2 = x2 + 1			) {
					this[0][y][x2] +=
						M[y][p2]*N[p2][x2]
					; //
				}
			}

			template <size_t P>
			inline _MACRO_MATRIX_Y_P_
			operator * (
				N2D_ARRAY <
					SIZE_X()  ,
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
						+ A.SIZE_Y() + A.SIZE_X()		//
						+ B.SIZE_Y() + B.SIZE_X()		//
						- SIZE_Y()   - SIZE_X()			//
					) / 2								//
				;										//
				FUNCTIONS::MyGEMM (						//
					FUNCTIONS::SET_TRANSPOSE (			//
						A.IS_TRANSPOSED()				//
					) ,									//
					FUNCTIONS::SET_TRANSPOSE (			//
						B.IS_TRANSPOSED()				//
					) ,									//
					SIZE_Y() , SIZE_X() , tmpz ,		//
					Alpha ,								//
					A.GET_DATA() , A.SIZE_X() ,			//
					B.GET_DATA() , B.SIZE_X() ,			//
					Beta , GET_DATA()					//
				) ;										//
			}											//
#endif													//
//////////////////////////////////////////////////////////

        } ;
    }

    namespace NN /* 3 D Array */ {
        template <
            size_t Z ,
            size_t Y ,
            size_t X ,
            typename T = float
        > class N3D_ARRAY : public
        ND_ARRAY <Z,N2D_ARRAY<Y,X,T>> {
        public:
            //
            typedef T TYPE_DATA   ;
            inline static constexpr
            size_t SIZE_X ()
            { return X;}
            //
            inline static constexpr
            size_t SIZE_Y ()
            { return Y;}
            //
            inline static constexpr
            size_t SIZE_Z ()
            { return Z;}
            //
            inline static constexpr
            size_t SIZE () {
                return
                    SIZE_X() *
                    SIZE_Y() *
                    SIZE_Z()
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
                    SIZE_Z()  ,
                    SIZE_Y()  ,
                    SIZE_X()  ,
                    TYPE_DATA
                >
            ; //
            //
            using TYPE_PARENT =
                ND_ARRAY <
                    SIZE_Z(),
                    N2D_ARRAY <
                        SIZE_Y(),
                        SIZE_X(),
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

//////////////////////////////////////////////////
#undef _MACRO_REGISTER_ALL_OPERATORS_           //
#undef _MACRO_REGISTER_OPERATOR_ON_TYPE_DATA_   //
//////////////////////////////////////////////////

	namespace FUNCTIONS {

#define _MACRO_MATRIX_ NN::N2D_ARRAY <2,2,TYPE_DATA,false>

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
