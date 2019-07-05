    namespace Array /* 2 D Array */ {

        template <
            size_t S_2 , size_t S_1 ,
            typename T = float ,
            bool Transposed = false
        > class N2D_ARRAY : public
        ND_ARRAY <S_2,ND_ARRAY<S_1,T>> {
        public:

			////////////////////////
			// DEFINITIONS BEGIN: //
			////////////////////////
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
            static inline size_t constexpr
			SkipSize()
			{ return SKIP_SIZE ; }
			//
            using TYPE_FLATTEN =
                ND_ARRAY
                    <SIZE(),TYPE_DATA>
            ; //
            //
            using TYPE_VECTOR_DATA =
                ND_ARRAY
                    <SIZE_1(),TYPE_DATA>
            ; //
            //
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
            //
            using TYPE_TRANSPOSED =
                N2D_ARRAY <
                    SIZE_2()   ,
                    SIZE_1()   ,
                    TYPE_DATA  ,
                    !IS_TRANSPOSED()
                >
            ; //
            //
            using TYPE_PARENT =
                ND_ARRAY <
                    SIZE_2() ,
                    TYPE_VECTOR_DATA
                >
            ; //
            //
			inline TYPE_PARENT &
            AS_PARENT () {
                return
                    reinterpret_cast
                        <TYPE_PARENT &>
                        (this[0])
                ; //
            } //
            //
            inline TYPE_PARENT const &
            AS_PARENT () const {
                return
                    reinterpret_cast
                        <TYPE_PARENT const &>
                        (this[0])
                ; //
            } //
			//
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
            //
            inline TYPE_TRANSPOSED &
            MARK_TRANSPOSED () {
                return
                    reinterpret_cast
                        <TYPE_TRANSPOSED &>
                        (this[0])
                ; //
            } //
            //
            inline TYPE_TRANSPOSED const &
            MARK_TRANSPOSED () const {
                return
                    reinterpret_cast
                        <TYPE_TRANSPOSED const &>
                        (this[0])
                ; //
            } //
            //
            N2D_ARRAY(){}
            ~N2D_ARRAY(){}
            //////////////////////
			// DEFINITIONS END. //
			//////////////////////

			////////////////////////////
			// BASIC OPERATORS BEGIN: //
			////////////////////////////
			//
            _MACRO_REGISTER_ALL_OPERATORS_
            //
			inline TYPE_DATA const
			TRACE () const {

				TYPE_DATA ret =
					0
				; //

				size_t Min =
					CPPFileIO::mymin(
						SIZE_1(),
						SIZE_2()
					)
				; //

				for(size_t i=0;i<Min;i++){
					ret+=this[0][i][i];
				}

				return
					ret
				; //

			} //
			//
            inline TYPE_SELF &
            operator = (
                TYPE_VECTOR_DATA const &
                    other
            ) {

				AS_PARENT() =
					other
				; //

				return
					this[0]
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
                        ; //
                    } else {
                        this[0][y][x] =
                            0
                        ; //
                    }

                }
            }
            //
			inline void
            operator += (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()	+=
                    other.FLATTEN()
                ; //
            }
            //
            inline void
            operator -= (
                TYPE_SELF const &
                    other
            ) {
                FLATTEN()	-=
                    other.FLATTEN()
                ; //
            }
            //
            //////////////////////////
			// BASIC OPERATORS END. //
			//////////////////////////


			////////////////////////////////
			// BAD HEAVY OPERATORS BEGIN: //
			////////////////////////////////
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
				for(
					size_t y=0;
					y<SIZE_2();
					y++
				) {
                    ret[y] =
                        this[0][y] *
                        In
                    ; //
                }
                return ret ;
            }
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
            //////////////////////////////
			// BAD HEAVY OPERATORS END. //
			//////////////////////////////


			/////////////////
			// MISC BEGIN: //
			/////////////////
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
            ///////////////
			// MISC END. //
			///////////////


////////////////////////////////////////////////////////////////
// MULTIPLICATION AND TRANSPOSE FUNCTIONS BEGIN: ///////////////
////////////////////////////////////////////////////////////////

//////////////////////////////////////
#define _MACRO_LIMIT_1_				\
	size_t const l1 =				\
		CPPFileIO::mymin (			\
			s1 + SkipSize() ,		\
			SIZE_1()				\
		)							\
	;								//
//////////////////////////////////////
#define _MACRO_LIMIT_2_				\
	size_t const l2 =				\
		CPPFileIO::mymin (			\
			s2 + SkipSize() ,		\
			SIZE_2()				\
		)							\
	;								//
//////////////////////////////////////
#define _MACRO_LIMIT_3_				\
	size_t const l3 =				\
		CPPFileIO::mymin (			\
			s3 + SkipSize() ,		\
			SIZE_3					\
		)							\
	;								//
//////////////////////////////////////
#define _MACRO_DEFINE_LIMITS_	\
	_MACRO_LIMIT_1_				\
	_MACRO_LIMIT_2_				\
	_MACRO_LIMIT_3_				//
//////////////////////////////////////
#define _MACRO_ARR_(Size2,Size1)	\
	N2D_ARRAY <						\
		Size2		,				\
		Size1		,				\
		TYPE_DATA					\
	>								//
//////////////////////////////////////////
#define _MACRO_MAT_(Size2,Size1)		\
	_MACRO_ARR_(Size2,Size1) const &	//
//////////////////////////////////////////////////
#define _MACRO_OUT_(Var,Lim)					\
	for(size_t Var=0;Var<Lim;Var+=SkipSize())	//
//////////////////////////////////////////////////
#define _MACRO_S1_ _MACRO_OUT_(s1,SIZE_1())	//
#define _MACRO_S2_ _MACRO_OUT_(s2,SIZE_2())	//
#define _MACRO_S3_ _MACRO_OUT_(s3,SIZE_3)	//
//////////////////////////////////////////////////
#define _MACRO_T1_ for(size_t t1=s1;t1<l1;t1++)	//
#define _MACRO_T2_ for(size_t t2=s2;t2<l2;t2++)	//
#define _MACRO_T3_ for(size_t t3=s3;t3<l3;t3++)	//
//////////////////////////////////////////////////////////////////
#define _MACRO_MULTIPLY_ template <size_t SIZE_3> inline void	//
//////////////////////////////////////////////////////////////////////////////
#define _MACRO_M21_ Operator::TRANSPOSE <_MACRO_ARR_(SIZE_2(),SIZE_1())>	//
#define _MACRO_M12_ Operator::TRANSPOSE <_MACRO_ARR_(SIZE_1(),SIZE_2())>	//
//////////////////////////////////////////////////////////////////////////////
#define _MACRO_M2P_ _MACRO_ARR_(SIZE_2(),SIZE_3)						//
#define _MACRO_MP2_ Operator::TRANSPOSE <_MACRO_ARR_(SIZE_3,SIZE_2())>	//
//////////////////////////////////////////////////////////////////////////
#define _MACRO_MP1_ _MACRO_ARR_(SIZE_3,SIZE_1())						//
#define _MACRO_M1P_ Operator::TRANSPOSE <_MACRO_ARR_(SIZE_1(),SIZE_3)>	//
//////////////////////////////////////////////////////////////////////////
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

			/////////////////
			// P21P BEGIN: //
			/////////////////
			_MACRO_MULTIPLY_
			MULTIPLY_P21P (
				_MACRO_MAT_(SIZE_3,SIZE_2()) I1 ,
				_MACRO_MAT_(SIZE_1(),SIZE_3) I2
			) {
				printf("called MULTIPLY_P21P\n");
				_MACRO_S1_ _MACRO_S2_ _MACRO_S3_ {
					_MACRO_DEFINE_LIMITS_
					_MACRO_T1_ _MACRO_T2_ _MACRO_T3_ {
						this[0][t2][t1]	+=
							I1[t3][t2]	*
							I2[t1][t3]
						; //
					}
				}
			} //
			_MACRO_MULTIPLY_
			MULTIPLY (
				_MACRO_MP2_ const & I1 ,
				_MACRO_M1P_ const & I2
			) {
				MULTIPLY_P21P (
					I1.I1 ,
					I2.I1
				) ; //
			} //
			template <size_t P>
			inline void
			MULTIPLY (
				_MACRO_MATRIX_P_Y_ const & M ,
				_MACRO_MATRIX_X_P_ const & N
			) {
				MULTIPLY_P21P
					(M,N)
				; //
			} //
			///////////////
			// P21P END. //
			///////////////

			/////////////////
			// 2PP1 BEGIN: //
			/////////////////
			_MACRO_MULTIPLY_  MULTIPLY_2PP1 (
				_MACRO_MAT_(SIZE_2(),SIZE_3) I1 ,
				_MACRO_MAT_(SIZE_3,SIZE_1()) I2
			) {
				_MACRO_S2_ _MACRO_S3_ _MACRO_S1_ {
					_MACRO_DEFINE_LIMITS_
					_MACRO_T2_ _MACRO_T3_ _MACRO_T1_ {
						this[0][t2][t1]	+=
							I1[t2][t3]	*
							I2[t3][t1]
						; //
					}
				}
			} //
			_MACRO_MULTIPLY_
			MULTIPLY (
				_MACRO_M2P_ const & I1 ,
				_MACRO_MP1_ const & I2
			) {
				MULTIPLY_2PP1
					(I1,I2)
				; //
			} //
			///////////////
			// 2PP1 END. //
			///////////////

			/////////////////
			// 2P1P BEGIN: //
			/////////////////
			_MACRO_MULTIPLY_ MULTIPLY_2P1P (
				_MACRO_MAT_(SIZE_2(),SIZE_3) I1 ,
				_MACRO_MAT_(SIZE_1(),SIZE_3) I2
			) {
				_MACRO_S2_ _MACRO_S1_ _MACRO_S3_ {
					_MACRO_DEFINE_LIMITS_
					_MACRO_T2_ _MACRO_T1_ _MACRO_T3_ {
						this[0][t2][t1]	+=
							I1[t2][t3]	*
							I2[t1][t3]
						; //
					}
				}
			} //
			_MACRO_MULTIPLY_
			MULTIPLY (
				_MACRO_M2P_ const & I1 ,
				_MACRO_M1P_ const & I2
			) {
				MULTIPLY_2P1P (
					I1 ,
					I2.I1
				) ;
			} //
			template <size_t P>
			inline void
			MULTIPLY (
				_MACRO_MATRIX_Y_P_ const & M ,
				_MACRO_MATRIX_X_P_ const & N
			) {
				MULTIPLY_2P1P
					(M,N)
				; //
			} //
			///////////////
			// 2P1P END. //
			///////////////

			/////////////////
			// P2P1 BEGIN: //
			/////////////////
			_MACRO_MULTIPLY_ MULTIPLY_P2P1 (
				_MACRO_MAT_(SIZE_3,SIZE_2()) I1 ,
				_MACRO_MAT_(SIZE_3,SIZE_1()) I2
			) {
				_MACRO_S3_ _MACRO_S2_ _MACRO_S1_ {
					_MACRO_DEFINE_LIMITS_
					_MACRO_T3_ _MACRO_T2_ _MACRO_T1_ {
						this[0][t2][t1]	+=
							I1[t3][t2]	*
							I2[t3][t1]
						; //
					}
				}
			}
			_MACRO_MULTIPLY_
			MULTIPLY (
				_MACRO_MP2_ const & I1 ,
				_MACRO_MP1_ const & I2
			) {
				MULTIPLY_P2P1 (
					I1.I1 ,
					I2
				) ;
			}
			template <size_t P>
			inline void
			MULTIPLY (
				_MACRO_MATRIX_P_Y_ const & M ,
				_MACRO_MATRIX_P_X_ const & N
			) {
				MULTIPLY_P2P1
					(M,N)
				; //
			} //
			///////////////
			// P2P1 END. //
			///////////////


			//////////////////////
			// TRANSPOSE BEGIN: //
			//////////////////////
			inline void
			GET_TRANSPOSE (
				_MACRO_MAT_(SIZE_1(),SIZE_2())
					in
			) {
				_MACRO_S2_ _MACRO_S1_ {
					_MACRO_LIMIT_1_
					_MACRO_LIMIT_2_
					_MACRO_T1_ _MACRO_T2_ {
						this[0][t2][t1] =
							in[t1][t2]
						; //
					}
				}
			}
			inline void
			PUT_TRANSPOSE (
				_MACRO_MAT_(SIZE_1(),SIZE_2())
					in
			) const {

				in.GET_TRANSPOSE
					(this[0])
				; //

			}
			inline void
			operator = (
				_MACRO_M12_ const &
					in
			) {
				GET_TRANSPOSE
					(in.I1)
				; //
			}
			N2D_ARRAY (
				_MACRO_M12_ const &
					in
			) {
				this[0] = in ;
			}
			inline _MACRO_M21_ const
			TRANSPOSE () const {
				_MACRO_M21_ const
					ret (
						this[0]
					)
				; //
				return
					ret
				; //
			}
			////////////////////
			// TRANSPOSE END. //
			////////////////////


//////////////////////////////
#define _MACRO_RET_			\
	Operator::MULTIPLY <	\
		_MACRO_A_ ,			\
		_MACRO_B_			\
	> const					//
//////////////////////////////

//////////////////////////////
#define _MACRO_A_ TYPE_SELF //
//////////////////////////////

/////////////////////////////////////////////////
#define _MACRO_B_ _MACRO_ARR_(SIZE_1(),SIZE_3) //
/////////////////////////////////////////////////

	template <size_t SIZE_3>
	inline _MACRO_RET_
	operator * (
		_MACRO_B_ const &
			other
	) const {
		_MACRO_RET_
			ret (
				this[0] ,
				other
			)
		; //
		return
			ret
		; //
	}

///////////////////
#undef _MACRO_B_ //
///////////////////

///////////////////////////////////////////////////////////////////////
#define _MACRO_B_ Operator::TRANSPOSE <_MACRO_ARR_(SIZE_3,SIZE_1())> //
///////////////////////////////////////////////////////////////////////

		template <size_t SIZE_3>
		inline
		Operator::MULTIPLY <
			_MACRO_A_ ,
			_MACRO_B_
		> const
		operator * (
			_MACRO_B_ const &
					other
		) const {
			_MACRO_RET_
				ret (
					this[0] ,
					other
				)
			; //
			return
				ret
			; //
		}

///////////////////
#undef _MACRO_B_ //
///////////////////

///////////////////
#undef _MACRO_A_ //
///////////////////

/////////////////////
#undef _MACRO_RET_ //
/////////////////////

//////////////////////////
#undef _MACRO_LIMIT_3_	//
#undef _MACRO_LIMIT_2_	//
#undef _MACRO_LIMIT_1_	//
//////////////////////////

//////////////////////
#undef _MACRO_M21_	//
#undef _MACRO_M12_	//
#undef _MACRO_M2P_	//
#undef _MACRO_MP2_	//
#undef _MACRO_MP1_	//
#undef _MACRO_M1P_	//
//////////////////////

//////////////////////////////////
#undef _MACRO_ARR_				//
#undef _MACRO_S1_				//
#undef _MACRO_S2_				//
#undef _MACRO_S3_				//
#undef _MACRO_T1_				//
#undef _MACRO_T2_				//
#undef _MACRO_T3_				//
#undef _MACRO_OUT_				//
#undef _MACRO_MAT_				//
#undef _MACRO_DEFINE_LIMITS_	//
#undef _MACRO_MULTIPLY_			//
//////////////////////////////////

//////////////////////////////
#undef _MACRO_MATRIX_X_P_	//
#undef _MACRO_MATRIX_P_X_	//
#undef _MACRO_MATRIX_Y_P_	//
#undef _MACRO_MATRIX_P_Y_	//
//////////////////////////////

	inline void
	MULTIPLY (
		TYPE_SELF const & I1 ,
		TYPE_DATA const I2
	) {

		auto & out =
			FLATTEN ()
		; //

		auto const & in =
			I1.FLATTEN ()
		; //

		for(size_t i=0;i<SIZE();i++)
		{ out[i] = in[i] * I2 ; }

	}


	template <
		typename	T1	,
		typename	T2
	> inline TYPE_SELF &
	operator += (
		Operator::MULTIPLY
			<T1,T2> const &
				in
	) {
		MULTIPLY (
			in.I1 ,
			in.I2
		) ; //
		return
			this[0]
		; //
	}

	template <
		typename	T1	,
		typename	T2
	> inline TYPE_SELF &
	operator = (
		Operator::MULTIPLY
			<T1,T2> const &
				in
	) {
		this[0] =
			0
		; //
		return (
			this[0] +=
				in
		) ; //
	}

	template <
		typename	T1	,
		typename	T2
	> N2D_ARRAY (
		Operator::MULTIPLY
			<T1,T2> const &
				in
	) {
		this[0]	=
			in
		; //
	}

////////////////////////////////////////////////////////////////
// MULTIPLICATION AND TRANSPOSE FUNCTIONS END. /////////////////
////////////////////////////////////////////////////////////////

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

//////////////////////////////
#undef _MACRO_INTERFACE_	//
//////////////////////////////

	}
