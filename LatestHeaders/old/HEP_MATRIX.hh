#include "./CPPFileIO/main.hh"
#include "./qcdloop/main.hh"
////////////////////////////////////////////////////////////////
class Expr {
public:
    std::string expression ;
	//
    inline void operator =
    (char a) {
        expression =
			a
        ; //
    }
    inline void operator =
    (char const * a) {
        expression =
                std::string(a)
        ; //
    }
    inline void operator =
    (std::string const & name) {
        expression = name ;
    }
    inline void operator =
    ( Expr const & other ) {
        expression = other.expression ;
    }
    inline void operator =
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator +=
    (char a) {
        expression =
                expression +
                std::string("+") +
                a
        ; //
    }
    inline void operator +=
    (char const * a) {
        expression =
                expression +
                std::string("+") +
                std::string(a)
        ; //
    }
    inline void operator +=
    (std::string const & name) {
        expression =
                expression +
                std::string("+") +
                name
        ; //
    }
    inline void operator +=
    ( Expr const & other ) {
        expression =
                expression +
                std::string("+") +
                other.expression
        ; //
    }
    inline void operator +=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator -=
    (char a) {
        expression =
                expression +
                std::string("-") +
                a
        ; //
    }
    inline void operator -=
    (char const * a) {
        expression =
                expression +
                std::string("-") +
                a
        ; //
    }
    inline void operator -=
    (std::string const & name) {
        expression =
                expression +
                std::string("-") +
                name
        ; //
    }
    inline void operator -=
    ( Expr const & other ) {
        expression =
                expression +
                std::string("-") +
                other.expression
        ; //
    }
    inline void operator -=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator *=
    (char a) {
        expression =
                expression +
                std::string(" \\times ")+
                a
        ; //
    }
    inline void operator *=
    (char const * a) {
        expression =
                expression +
                std::string(" \\times ")+
                std::string(a)
        ; //
    }
    inline void operator *=
    (std::string const & name) {
        expression =
                expression +
                std::string(" \\times ")+
                name
        ; //
    }
    inline void operator *=
    ( Expr const & other ) {
        expression =
                expression +
                std::string(" \\times ")+
                other.expression
        ; //
    }
    inline void operator *=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator /=
    (char a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                a+
                std::string("}")
        ; //
    }
    inline void operator /=
    (char const * a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(a)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (std::string const & a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                a +
                std::string("}")
        ; //
    }
    inline void operator /=
    ( Expr const & other ) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                other.expression +
                std::string("}")
        ; //
    }
    inline void operator /=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    template <typename T>
    inline Expr operator +
    (T const & a) {
        Expr ret(this[0]);
        ret+=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator -
    (T const & a) {
        Expr ret(this[0]);
        ret-=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator *
    (T const & a) {
        Expr ret(this[0]);
        ret*=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator /
    (T const & a) {
        Expr ret(this[0]);
        ret/=a;
        return ret ;
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline Expr operator - () {
        return Expr (
            std::string("-") +
            expression
        ) ; //
    }
////////////////////////////////////////////////////////////////
    inline Expr
    bracketed () {
        return Expr (
                    std::string ("\\left( ")+
                    expression +
                    std::string (" \\right)")
        ) ; //
    }
    inline void show () {
        std::cout << expression ;
    }
////////////////////////////////////////////////////////////////
    Expr(){}
    template <typename T>
    Expr (T const & a) {
        this[0] = a ;
    }
    ~Expr(){}
////////////////////////////////////////////////////////////////
} ;
////////////////////////////////////////////////////////////////

namespace NICE_TYPES {

	template <typename T>
	class LoopResultStorage {
	public:
		//
		using TYPE_DATA =
			T
		; //
		using TYPE_COMPLEX_DATA =
			std::complex <TYPE_DATA>
		; //
		using TYPE_RESULT_ELEMENT =
			Tensors::NN::ND_ARRAY
			<6,TYPE_COMPLEX_DATA>
		; //
		using TYPE_SELF =
			LoopResultStorage
			<TYPE_DATA>
		; //
		static inline size_t constexpr
		SIZE () {
			return
				TYPE_RESULT_ELEMENT::SIZE()
			; //
		} //
		static inline size_t const
		INTERPRET_EPSILON
		(long const i) {
			if((-2<=i)&&(i<=2)){
				return (i+2) ;
			} else {
				return 5 ;
			}
		}
	private:
		TYPE_RESULT_ELEMENT
			RESULT
		; //
		inline void
		READ_FROM (
			TYPE_COMPLEX_DATA const
				other
		) {
			RESULT =
				TYPE_COMPLEX_DATA(0,0)
			; //
			RESULT[INTERPRET_EPSILON(0)] =
				other
			; //
		} //
		inline void
		READ_FROM (
			TYPE_DATA const
				other = 0
		) {
			READ_FROM (
				TYPE_COMPLEX_DATA (
					other , 0
				)
			) ;
		} //
		inline void
		READ_FROM (
			TYPE_COMPLEX_DATA const other1 ,
			TYPE_COMPLEX_DATA const other2 ,
			TYPE_COMPLEX_DATA const other3 ,
			TYPE_COMPLEX_DATA const other4 ,
			TYPE_COMPLEX_DATA const other5
		) {
			READ_FROM (other3) ;
			RESULT[INTERPRET_EPSILON(-2)] = other1 ;
			RESULT[INTERPRET_EPSILON(-1)] = other2 ;
			RESULT[INTERPRET_EPSILON(1)]  = other4 ;
			RESULT[INTERPRET_EPSILON(2)]  = other5 ;
		} //
		inline void
		READ_FROM (
			TYPE_DATA const other1 ,
			TYPE_DATA const other2 ,
			TYPE_DATA const other3 ,
			TYPE_DATA const other4 ,
			TYPE_DATA const other5
		) {
			READ_FROM (
				TYPE_COMPLEX_DATA(other1,0),
				TYPE_COMPLEX_DATA(other2,0),
				TYPE_COMPLEX_DATA(other3,0),
				TYPE_COMPLEX_DATA(other4,0),
				TYPE_COMPLEX_DATA(other5,0)
			) ;
		} //
		inline TYPE_COMPLEX_DATA &
		GET_TERM (
			long const i
		) {
			return
				RESULT[
					INTERPRET_EPSILON(i)
				]
			; //
		} //
		inline TYPE_COMPLEX_DATA const
		GET_TERM (
			long const i
		) const {
			return
				RESULT[
					INTERPRET_EPSILON(i)
				]
			; //
		} //
	public:
		//
		inline TYPE_COMPLEX_DATA &
		operator [] (size_t const i)
		{ return RESULT[i] ; }
		//
		inline TYPE_COMPLEX_DATA const
		operator [] (size_t const i) const
		{ return RESULT[i] ; }
		//
		inline TYPE_COMPLEX_DATA &
		operator () (long const i) {
			return
				GET_TERM(i)
			; //
		} //
		inline TYPE_COMPLEX_DATA const
		operator () (long const i) const {
			return
				GET_TERM(i)
			; //
		} //
		inline void operator =
		(TYPE_SELF const & other) {
			RESULT =
				other.RESULT
			; //
		} //
		inline void operator =
		(TYPE_COMPLEX_DATA const other) {
			READ_FROM (other) ;
		} //
		inline void operator =
		(TYPE_DATA const other) {
			READ_FROM (other) ;
		} //
		//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	\
	inline TYPE_SELF operator SIGN				\
	(TYPE_COMPLEX_DATA const other) const {		\
		TYPE_SELF ret (RESULT) ;				\
		ret(0) =								\
			ret(0) SIGN							\
			other								\
		;										\
		return ret ;							\
	}											\
	inline TYPE_SELF operator SIGN				\
	(TYPE_DATA const other) const {				\
		TYPE_SELF ret (RESULT) ;				\
		ret(0) =								\
			ret(0) SIGN							\
			TYPE_COMPLEX_DATA(other,0)			\
		;										\
		return ret ;							\
	}											//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_OPERATOR_(SIGN)		\
	inline TYPE_SELF operator SIGN			\
	(TYPE_SELF const & other) const {		\
		TYPE_SELF							\
			ret (							\
				RESULT SIGN other.RESULT	\
			)								\
		;									\
		return ret ;						\
	}										\
	_MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	//
//////////////////////////////////////////////
_MACRO_DEFINE_OPERATOR_(+)					//
_MACRO_DEFINE_OPERATOR_(-)					//
#undef _MACRO_DEFINE_OPERATOR_				//
#undef _MACRO_DEFINE_SCALAR_OPERATOR_		//
//////////////////////////////////////////////
		//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	\
		inline TYPE_SELF						\
		operator SIGN (							\
			TYPE_COMPLEX_DATA const				\
				other							\
		) const {								\
			TYPE_SELF							\
				ret (							\
					RESULT SIGN					\
					other						\
				)								\
			; 									\
			return								\
				ret								\
			;									\
		}										\
		inline TYPE_SELF						\
		operator SIGN (							\
			TYPE_DATA const						\
				other							\
		) const {								\
			TYPE_SELF							\
				ret (							\
					RESULT SIGN					\
					TYPE_COMPLEX_DATA(other,0)	\
				)								\
			;									\
			return								\
				ret								\
			;									\
		}										//
//////////////////////////////////////////////////
_MACRO_DEFINE_SCALAR_OPERATOR_(*)				//
_MACRO_DEFINE_SCALAR_OPERATOR_(/)				//
#undef _MACRO_DEFINE_SCALAR_OPERATOR_			//
//////////////////////////////////////////////////
		//
		inline TYPE_SELF
		operator * (
			TYPE_SELF const &
				other
		) const {
			TYPE_SELF
				ret
			; /* Evaluate the return value */ {
				for(long x=-2;x<=2;x++)
				for(long y=-2;y<=2;y++){
					long const z = x + y ;
					if((-2<=z)&&(z<=2)){
						ret(z) +=
							GET_TERM(x) *
							other(y)
						; //
					}
				}
			}
			return
				ret
			; //
		} //
		//
		template <typename TB>
		inline void DimExp (
			TYPE_COMPLEX_DATA	const	A	,
			TB					const	B	,
			TYPE_COMPLEX_DATA	const	C
		) {
			TYPE_COMPLEX_DATA const
				BA =
					std::pow(A,B)
			; //
			TYPE_COMPLEX_DATA const
				CA =
					std::log(A)*C
			; //
			TYPE_COMPLEX_DATA const
				BACA =
					BA * CA
			;
			RESULT =
				TYPE_COMPLEX_DATA(0,0)
			; //
			GET_TERM(0) =
				BA
			; //
			GET_TERM(1) =
				BACA
			; //
			GET_TERM(2) =
				BACA * CA /
				2.0
			; //
		} //
		//
		LoopResultStorage
		( TYPE_SELF const & other )
		{ RESULT = other.RESULT ; }
		//
		LoopResultStorage
		( TYPE_RESULT_ELEMENT const & other )
		{ RESULT = other ; }
		//
		LoopResultStorage
		( TYPE_COMPLEX_DATA const other )
		{ READ_FROM(other) ; }
		//
		LoopResultStorage
		( TYPE_DATA const other )
		{ READ_FROM(other) ; }
		//
		LoopResultStorage (
			TYPE_COMPLEX_DATA const other1 ,
			TYPE_COMPLEX_DATA const other2 ,
			TYPE_COMPLEX_DATA const other3 ,
			TYPE_COMPLEX_DATA const other4 ,
			TYPE_COMPLEX_DATA const other5
		) {
			READ_FROM (
				other1,other2,
				other3,other4,
				other5
			) ;
		}
		//
		LoopResultStorage (
			TYPE_DATA const other1 ,
			TYPE_DATA const other2 ,
			TYPE_DATA const other3 ,
			TYPE_DATA const other4 ,
			TYPE_DATA const other5
		) {
			READ_FROM (
				other1,other2,
				other3,other4,
				other5
			) ;
		}
		//
		LoopResultStorage () {
			READ_FROM
				(0,0,0,0,0)
			; //
		}
		//
		~LoopResultStorage(){}
		//
		template <typename TB>
		inline static TYPE_SELF
		DIM_EXP (
			TYPE_COMPLEX_DATA	const	A	,
			TB					const	B	,
			TYPE_COMPLEX_DATA	const	C
		) {
			TYPE_SELF ret ;
			ret.DimExp(A,B,C);
			return ret ;
		}
		template <typename TB>
		inline static TYPE_SELF
		DIM_EXP (
			TYPE_DATA	const	A	,
			TB			const	B	,
			TYPE_DATA	const	C
		) {
			return
				DIM_EXP (
					TYPE_COMPLEX_DATA	(A,0)	,
					B							,
					TYPE_COMPLEX_DATA	(C,0)
				)
			; //
		}
		//
	} ;

	template <typename T>
	class STATIC_TYPES {
	public:
		////////////////////////////////
		// The Definitions /////////////
		////////////////////////////////
		using TYPE_DATA =
			T
		; //
		using TYPE_COMPLEX_DATA =
			std::complex <TYPE_DATA>
		; //
		using TYPE_MAIN_VECTOR =
			Tensors::NN::ND_ARRAY
				<4,TYPE_COMPLEX_DATA>
		; //
		using TYPE_MAIN_MATRIX =
			Tensors::NN::N2D_ARRAY
				<4,4,TYPE_COMPLEX_DATA>
		; //
		using TYPE_REAL_MATRIX =
			Tensors::NN::N2D_ARRAY
				<4,4,TYPE_DATA>
		; //
		using TYPE_FULL_GAMMA_MATRIX =
			Tensors::NN::ND_ARRAY
				<5,TYPE_MAIN_MATRIX>
		; //
		using TYPE_LOOP_RESULTS =
			LoopResultStorage
				<TYPE_DATA>
		; //
		////////////////////////////////
		// Important Constants: ////////
		////////////////////////////////
		static inline TYPE_DATA constexpr
		PI() {
			return
				3.1415926535897932384626433
			;
		} //
		static inline TYPE_DATA constexpr
		CHARGE_E() {
			return
				0.30282212
			;
		} //
		static inline TYPE_DATA constexpr
		EPS() {
			return
				0.000000001
			;
		} //
		static inline size_t constexpr
		FACTORIAL (size_t const i) {
			if (i<=1) {
				return
					1
				; //
			} else {
				return
					FACTORIAL(i-1)*i
				; //
			}
		} //
		static inline TYPE_DATA constexpr
		HARMONIC_SUM (size_t const i) {
			if (i<=1) {
				return
					1
				; //
			} else {
				return
					HARMONIC_SUM(i-1) +
					(
						1.0 /
						static_cast <TYPE_DATA> (i)
					)
				; //
			}
		} //
		static inline long constexpr
		MINUS_1 (long n) {
			if(n==0){return 1;}
			else if(n>0){return -MINUS_1(n-1);}
			else if(n<0){return -MINUS_1(n+1);}
		} //
		static inline TYPE_DATA constexpr
		EULER_GAMMA () {
			// https://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant
			return
				0.57721566490153286060651209008240243104215933593
			; //
		}
		////////////////////////////////
		// Complexified constants: /////
		////////////////////////////////
		static inline TYPE_COMPLEX_DATA
		TO_C (TYPE_DATA const in) {
			TYPE_COMPLEX_DATA
				ret (in,0)
			; //
			return
				ret
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA constexpr
		C_1 () {
			return
				TYPE_COMPLEX_DATA
					( 1.0 , 0.0 )
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA constexpr
		C_I () {
			return
				TYPE_COMPLEX_DATA
					( 0.0 , 1.0 )
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA constexpr
		IEPS () {
			return
				C_I () * EPS()
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA constexpr
		C_PI () {
			return
				C_I() * PI()
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA constexpr
		C_E () {
			return
				C_I() * CHARGE_E()
			; //
		}
		////////////////////////////////
		// Function to get metric //////
		////////////////////////////////
		static inline TYPE_REAL_MATRIX constexpr
		METRIC () {
			TYPE_REAL_MATRIX ret ;
			ret       =  0.0 ;
			ret[0][0] =  1.0 ;
			ret[1][1] = -1.0 ;
			ret[2][2] = -1.0 ;
			ret[3][3] = -1.0 ;
			return ret ;
		}
		////////////////////////////////
		// Simple Functions: ///////////
		////////////////////////////////
		static inline TYPE_COMPLEX_DATA
		DAGGER (
			TYPE_COMPLEX_DATA const
				in
		) {
			return
				std::conj (in)
			;
		} //
		static inline TYPE_MAIN_VECTOR
		GET_VECTOR (
			TYPE_DATA	const	En	,
			TYPE_DATA	const	X	,
			TYPE_DATA	const	Y	,
			TYPE_DATA	const	Z
		) {
			TYPE_MAIN_VECTOR
				ret
			; /* Prepare the vector */ {
				ret[0] =
					TYPE_COMPLEX_DATA
						( En , 0 )
				; //
				ret[1] =
					TYPE_COMPLEX_DATA
						( X  , 0 )
				; //
				ret[2] =
					TYPE_COMPLEX_DATA
						( Y  , 0 )
				; //
				ret[3] =
					TYPE_COMPLEX_DATA
						( Z  , 0 )
				; //
			} //
			return
				ret
			; //
		} //
		static inline TYPE_MAIN_VECTOR
		GET_VECTOR (
			TYPE_DATA const M ,
			TYPE_DATA const Z
		) {
			TYPE_DATA En =
				std::sqrt (
					(Z*Z) +
					(M*M)
				)
			;
			return
				GET_VECTOR(
					En	,	0 ,
					0	,	Z
				)
			;
		}
		//
		static inline TYPE_MAIN_VECTOR
		DAGGER (
			TYPE_MAIN_VECTOR const &
				in
		) {
			TYPE_MAIN_VECTOR
				ret
			; //
			for(size_t i=0;i<in.SIZE();i++){
				ret[i] =
					DAGGER (in[i])
				; //
			}
			return
				ret
			; //
		}
		//
		static inline TYPE_MAIN_MATRIX
		DAGGER (
			TYPE_MAIN_MATRIX const &
				in
		) {
			TYPE_MAIN_MATRIX
				ret
			; //
			for(size_t y=0;y<ret.SIZE_Y();y++){
				for(size_t x=0;x<ret.SIZE_X();x++){
					ret[y][x] =
						DAGGER (
							in[x][y]
						)
					; //
				}
			}
			return
				ret
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA
		DOT (
			TYPE_MAIN_VECTOR const & p1 ,
			TYPE_MAIN_VECTOR const & p2
		) {
			return
				(p1[0]*DAGGER(p2[0])) -
				(p1[1]*DAGGER(p2[1])) -
				(p1[2]*DAGGER(p2[2])) -
				(p1[3]*DAGGER(p2[3]))
			; //
		}
		//
		static inline TYPE_COMPLEX_DATA
		NORM2 (
			TYPE_MAIN_VECTOR const &
				p
		) {
			return
				DOT(p,p)
			; //
		}
		////////////////////////////////
		// Scalar Propagator: //////////
		////////////////////////////////
		static inline TYPE_COMPLEX_DATA
		PROPAGATOR (
			TYPE_DATA const & p2         ,
			TYPE_DATA const   m  = 0     ,
			TYPE_DATA const   w  = EPS()
		) {
			return C_I() / (
				p2 +
				TYPE_COMPLEX_DATA
					(-(m*m),w)
			) ;
			//
		}
		//
		static inline TYPE_COMPLEX_DATA
		PROPAGATOR (
			TYPE_MAIN_VECTOR const & p         ,
			TYPE_DATA        const   m = 0     ,
			TYPE_DATA        const   w = EPS()
		) {
			return
				PROPAGATOR (
					NORM2(p).real() ,
					m , w
				)
			;
		}
		////////////////////////////////
		// More convinence functions: //
		////////////////////////////////
		static inline TYPE_MAIN_MATRIX constexpr
		ZERO_MATRIX () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			; //
			return
				ret
			;
		}
		//
		static inline TYPE_MAIN_VECTOR constexpr
		ZERO_VECTOR () {
			TYPE_MAIN_VECTOR
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			return
				ret
			;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		IDENTITY () {
			auto ret = ZERO_MATRIX () ;
			ret[0][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[1][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[2][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[3][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			return ret ;
		}
		////////////////////////////////
		// The Gamma Matrix Part ///////
		////////////////////////////////
		static inline TYPE_MAIN_MATRIX constexpr
		C_METRIC() {
			TYPE_MAIN_MATRIX ret = ZERO_MATRIX () ;
			ret[0][0] = TYPE_COMPLEX_DATA (+1.0,0.0) ;
			ret[1][1] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
			ret[2][2] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
			ret[3][3] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		GAMMA_0 () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			ret[0][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[1][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[2][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			ret[3][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		GAMMA_1 () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			ret[0][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			ret[1][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			ret[2][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			ret[3][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		GAMMA_2 () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			ret[0][3] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
			ret[1][2] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
			ret[2][1] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
			ret[3][0] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		GAMMA_3 () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			ret[0][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			ret[1][3] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			ret[2][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			ret[3][1] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		GAMMA_5 () {
			TYPE_MAIN_MATRIX
				ret (
					TYPE_COMPLEX_DATA
						( 0 , 0 )
				)
			;
			ret[0][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			ret[1][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
			ret[2][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			ret[3][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
			return ret ;
		}
		////////////////////////////////
		// Projection Operators: ///////
		////////////////////////////////
		static inline TYPE_MAIN_MATRIX constexpr
		PR () {
			auto G5 = GAMMA_5 () ;
			TYPE_MAIN_MATRIX ret ;
			for(size_t y=0;y<ret.SIZE_Y();y++){
				for(size_t x=0;x<ret.SIZE_X();x++){
					if (y==x) {
						ret[y][x] = (
							+ TYPE_COMPLEX_DATA (1.0,0.0)
							+ G5[y][x]
						) / 2.0 ;
					} else {
						ret[y][x] = (
							+ G5[y][x]
						) / 2.0 ;
					}
				}
			}
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX constexpr
		PL () {
			auto G5 = GAMMA_5 () ;
			TYPE_MAIN_MATRIX ret ;
			for(size_t y=0;y<ret.SIZE_Y();y++){
				for(size_t x=0;x<ret.SIZE_X();x++){
					if (y==x) {
						ret[y][x] = (
							+ TYPE_COMPLEX_DATA (1.0,0.0)
							- G5[y][x]
						) / 2.0 ;
					} else {
						ret[y][x] = (
							- G5[y][x]
						) / 2.0 ;
					}
				}
			}
			return ret ;
		}
		//
		static inline TYPE_FULL_GAMMA_MATRIX constexpr
		ALL_GAMMA () {
			TYPE_FULL_GAMMA_MATRIX ret ;
			ret[0] = GAMMA_0 () ;
			ret[1] = GAMMA_1 () ;
			ret[2] = GAMMA_2 () ;
			ret[3] = GAMMA_3 () ;
			ret[4] = GAMMA_5 () ;
			return ret ;
		}
		////////////////////////////////
		// QED Vertex: /////////////////
		////////////////////////////////
		static inline TYPE_FULL_GAMMA_MATRIX constexpr
		QED_VERTEX () {
			auto tmp =
				TYPE_COMPLEX_DATA
					(0.0,-1.0*CHARGE_E())
			;
			//
			TYPE_FULL_GAMMA_MATRIX ret ;
			ret[0] = MULTIPLY(GAMMA_0(),tmp);
			ret[1] = MULTIPLY(GAMMA_1(),tmp);
			ret[2] = MULTIPLY(GAMMA_2(),tmp);
			ret[3] = MULTIPLY(GAMMA_3(),tmp);
			ret[4] = GAMMA_5();
			return ret ;
		}
		////////////////////////////////
		// Functions on Matrices ///////
		////////////////////////////////
		static inline TYPE_MAIN_MATRIX
		GET_SLASH_ACCURATE (
			TYPE_MAIN_VECTOR const &
				p
		) {
			TYPE_MAIN_MATRIX ret ;
			for(size_t y=0;y<ret.SIZE_Y();y++){
				for(size_t x=0;x<ret.SIZE_X();x++){
					ret[y][x] = 0 ;
					for(size_t i=0;i<4;i++){
						ret[y][x] +=
							ALL_GAMMA()[i][y][x] *
							p[i]
						; //
					}
				}
			}
			return ret ;
		}
		//
		static inline TYPE_MAIN_MATRIX
		GET_SLASH_FAST (
			TYPE_MAIN_VECTOR const &
				p
		) {
			TYPE_MAIN_MATRIX ret =
				ZERO_MATRIX()
			; //
			ret[0][2] =   p[3] + p[0] ;
			ret[0][3] =   p[1] - (p[2]*C_I()) ;
			//
			ret[1][2] =   p[1] + (p[2]*C_I()) ;
			ret[1][3] = - p[3] + p[0] ;
			//
			ret[2][0] = - p[3] + p[0] ;
			ret[2][1] = - p[1] + (p[2]*C_I()) ;
			//
			ret[3][0] = - p[1] - (p[2]*C_I()) ;
			ret[3][1] =   p[3] + p[0] ;
			//
			return ret ;
			//
		}
		//
		static inline TYPE_MAIN_MATRIX
		GET_SLASH (
			TYPE_MAIN_VECTOR const &
				p
		) {
			return
				GET_SLASH_FAST
					(p)
			;
		}
		////////////////////////////////
		static inline TYPE_MAIN_MATRIX
		FERMION_NUMERATOR_U (
			TYPE_MAIN_VECTOR const & p ,
			TYPE_DATA        const   m
		) {
			return
				ADD (
					GET_SLASH(p),
					MULTIPLY(IDENTITY(),m)
				)
			;
		}
		//
		static inline TYPE_MAIN_MATRIX
		FERMION_NUMERATOR_V (
			TYPE_MAIN_VECTOR const & p ,
			TYPE_DATA        const   m
		) {
			return
				SUBTRACT (
					GET_SLASH(p),
					MULTIPLY(IDENTITY(),m)
				)
			; //
		}
		//
		static inline TYPE_MAIN_MATRIX
		FERMION_PROPAGATOR_U (
			TYPE_MAIN_VECTOR const & p         ,
			TYPE_DATA        const   m = 0     ,
			TYPE_DATA        const   w = EPS()
		) {
			return
				MULTIPLY (
					FERMION_NUMERATOR_U(p,m),
					PROPAGATOR(p,m,w)
				)
			;
		}
		//
		static inline TYPE_MAIN_MATRIX
		FERMION_PROPAGATOR (
			TYPE_MAIN_VECTOR const & p         ,
			TYPE_DATA        const   m = 0     ,
			TYPE_DATA        const   w = EPS()
		) {
			return
				FERMION_PROPAGATOR_U
					(p,m,w)
			; //
		}
		////////////////////////////////////////////////////////////////
		// Vector propagator = -g_mu_nu * PROPAGATOR ///////////////////
		////////////////////////////////////////////////////////////////
		static inline TYPE_MAIN_MATRIX
		PHOTON_PROPAGATOR (
			TYPE_MAIN_VECTOR const &
				p
		) {
			auto k = PROPAGATOR (p) ;
			TYPE_MAIN_MATRIX ret =
				ZERO_MATRIX ()
			; /* Prepare the matrix: */ {
				ret[0][0] = -1 ;
				ret[1][1] =  1 ;
				ret[2][2] =  1 ;
				ret[3][3] =  1 ;
			}
			return
				MULTIPLY (ret,k)
			; //
		}
		////////////////////////////////////////////////////////////////
	} ;

}

////////////////////////////////////////////////////////////////
template <typename T>
class MainList {
public:

	using DEFINITIONS =
		NICE_TYPES::STATIC_TYPES
			<T>
	; //

//////////////////////////////////////////////////////
#define _MACRO_INHERIT_(TYPENAME)					\
	typename DEFINITIONS::TYPENAME					//
//////////////////////////////////////////////////////
#define _MACRO_DEFINE_(TYPENAME)					\
	using TYPENAME = _MACRO_INHERIT_(TYPENAME) ;	//
//////////////////////////////////////////////////////

	_MACRO_DEFINE_(TYPE_DATA)
	_MACRO_DEFINE_(TYPE_COMPLEX_DATA)
	_MACRO_DEFINE_(TYPE_MAIN_VECTOR)
	_MACRO_DEFINE_(TYPE_MAIN_MATRIX)
	_MACRO_DEFINE_(TYPE_REAL_MATRIX)
	_MACRO_DEFINE_(TYPE_FULL_GAMMA_MATRIX)
	_MACRO_DEFINE_(TYPE_LOOP_RESULTS)

//////////////////////////
#undef _MACRO_DEFINE_	//
#undef _MACRO_INHERIT_	//
//////////////////////////

	////////////////////////////////
    // Important Constants: ////////
    ////////////////////////////////
	static inline TYPE_DATA constexpr
	PI() {
		return
			3.1415926535897932384626433
		;
	} //

	static inline TYPE_DATA constexpr
	CHARGE_E() {
		return
			0.30282212
		;
	} //

	static inline TYPE_DATA constexpr
	EPS() {
		return
			0.000000001
		;
	} //

    static inline size_t const
    FACTORIAL (size_t const i) {
        if(i==1){return 1;}
        else if(i==0){return 1;}
        else{return i*FACTORIAL(i-1);}
    }

    static inline TYPE_DATA const
    HARMONIC_SUM (size_t const i) {
        if (i==1) {
            return 1 ;
        }
        else {
            return
                HARMONIC_SUM(i-1) +
                (
                    1.0 /
                    ( (TYPE_DATA) i )
                )
            ; //
        }

    }

    static inline long const
    MINUS_1 (long n) {
        if(n==0){return 1;}
        else if(n>0){return -MINUS_1(n-1);}
        else if(n<0){return -MINUS_1(n+1);}
    }

    static inline TYPE_DATA
    constexpr EULER_GAMMA () {
        // https://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant
        return
            0.57721566490153286060651209008240243104215933593
        ; //
    }
    ////////////////////////////////
    // Complexified constants: /////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    TO_C (TYPE_DATA const in) {
        TYPE_COMPLEX_DATA
            ret (in,0)
        ; //
        return
            ret
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_1 () {
        return
            TYPE_COMPLEX_DATA
                ( 1.0 , 0.0 )
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_I () {
        return
            TYPE_COMPLEX_DATA
                ( 0.0 , 1.0 )
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr IEPS () {
        return
            C_I () * EPS()
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_PI () {
        return
            C_I() * PI()
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_E () {
        return
            C_I() * CHARGE_E()
        ; //
    }
    ////////////////////////////////
    // Function to get metric //////
    ////////////////////////////////
    static inline TYPE_REAL_MATRIX
    constexpr METRIC () {
        TYPE_REAL_MATRIX ret ;
        ret       =  0.0 ;
        ret[0][0] =  1.0 ;
        ret[1][1] = -1.0 ;
        ret[2][2] = -1.0 ;
        ret[3][3] = -1.0 ;
        return ret ;
    }
    ////////////////////////////////
    // Simple Functions: ///////////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    DAGGER (
        TYPE_COMPLEX_DATA const
            in
    ) {
        return
            std::conj (in)
        ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    GET_VECTOR (
        TYPE_DATA const
            En ,
        TYPE_DATA const
            X  ,
        TYPE_DATA const
            Y  ,
        TYPE_DATA const
            Z
    ) {
        TYPE_MAIN_VECTOR
            ret
        ; /* Prepare the vector */ {
            ret[0] =
                TYPE_COMPLEX_DATA
                    ( En , 0 )
            ; //
            ret[1] =
                TYPE_COMPLEX_DATA
                    ( X  , 0 )
            ; //
            ret[2] =
                TYPE_COMPLEX_DATA
                    ( Y  , 0 )
            ; //
            ret[3] =
                TYPE_COMPLEX_DATA
                    ( Z  , 0 )
            ; //
        } //
        return
            ret
        ; //
    }
    //
    static inline TYPE_MAIN_VECTOR
    GET_VECTOR (
        TYPE_DATA const
            M ,
        TYPE_DATA const
            Z
    ) {
        TYPE_DATA En =
            std::sqrt (
                (Z*Z) +
                (M*M)
            )
        ;
        return
            GET_VECTOR(
                En,0,
                0,Z
            )
        ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    DAGGER (
        TYPE_MAIN_VECTOR const &
            in
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(size_t i=0;i<in.SIZE();i++){
            ret[i] =
                DAGGER (in[i])
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    DAGGER (
        TYPE_MAIN_MATRIX const &
            in
    ) {
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                ret[y][x] =
                    DAGGER (
                        in[x][y]
                    )
                ; //
            }
        }
        return ret ;
    }
    //
    static inline TYPE_COMPLEX_DATA
    DOT (
        TYPE_MAIN_VECTOR const &
            p1 ,
        TYPE_MAIN_VECTOR const &
            p2
    ) {
        return
            (p1[0]*DAGGER(p2[0])) -
            (p1[1]*DAGGER(p2[1])) -
            (p1[2]*DAGGER(p2[2])) -
            (p1[3]*DAGGER(p2[3]))
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    NORM2 (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        return
            DOT(p,p)
        ; //
    }
    ////////////////////////////////
    // Scalar Propagator: //////////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    PROPAGATOR (
        TYPE_DATA const & p2         ,
        TYPE_DATA const   m  = 0     ,
        TYPE_DATA const   w  = EPS()
    ) {
        return C_I() / (
            p2 +
            TYPE_COMPLEX_DATA
                (-(m*m),w)
        ) ;
        //
    }
    //
    static inline TYPE_COMPLEX_DATA
    PROPAGATOR (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            PROPAGATOR (
                NORM2(p).real() ,
                m , w
            )
        ;
    }
    ////////////////////////////////
    // More convinence functions: //
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr ZERO_MATRIX () {
        TYPE_MAIN_MATRIX ret ;
        ret =
            TYPE_COMPLEX_DATA
                ( 0 , 0 )
        ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    constexpr ZERO_VECTOR () {
        TYPE_MAIN_VECTOR ret ;
        ret =
            TYPE_COMPLEX_DATA
                ( 0 , 0 )
        ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr IDENTITY () {
        auto ret = ZERO_MATRIX () ;
        ret[0][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[1][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[2][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[3][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        return ret ;
    }
    ////////////////////////////////
    // The Gamma Matrix Part ///////
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr C_METRIC() {
        TYPE_MAIN_MATRIX ret = ZERO_MATRIX () ;
        ret[0][0] = TYPE_COMPLEX_DATA (+1.0,0.0) ;
        ret[1][1] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        ret[2][2] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        ret[3][3] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_0 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA ( 0 , 0 ) ;
        ret[0][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[1][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[2][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[3][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_1 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[1][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[2][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[3][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_2 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA ( 0 ,  0 ) ;
        ret[0][3] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
        ret[1][2] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
        ret[2][1] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
        ret[3][0] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_3 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[1][3] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[2][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[3][1] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_5 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[1][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[2][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[3][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        return ret ;
    }
    ////////////////////////////////
    // Projection Operators: ///////
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr PR () {
        auto G5 = GAMMA_5 () ;
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                if (y==x) {
                    ret[y][x] = (
                        + TYPE_COMPLEX_DATA (1.0,0.0)
                        + G5[y][x]
                    ) / 2.0 ;
                } else {
                    ret[y][x] = (
                        + G5[y][x]
                    ) / 2.0 ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr PL () {
        auto G5 = GAMMA_5 () ;
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                if (y==x) {
                    ret[y][x] = (
                        + TYPE_COMPLEX_DATA (1.0,0.0)
                        - G5[y][x]
                    ) / 2.0 ;
                } else {
                    ret[y][x] = (
                        - G5[y][x]
                    ) / 2.0 ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_FULL_GAMMA_MATRIX
    constexpr ALL_GAMMA () {
        TYPE_FULL_GAMMA_MATRIX ret ;
        ret[0] = GAMMA_0 () ;
        ret[1] = GAMMA_1 () ;
        ret[2] = GAMMA_2 () ;
        ret[3] = GAMMA_3 () ;
        ret[4] = GAMMA_5 () ;
        return ret ;
    }
    ////////////////////////////////
    // QED Vertex: /////////////////
    ////////////////////////////////
    static inline TYPE_FULL_GAMMA_MATRIX
    constexpr QED_VERTEX () {
        auto tmp =
            TYPE_COMPLEX_DATA
                (0.0,-1.0*CHARGE_E())
        ;
        //
        TYPE_FULL_GAMMA_MATRIX ret ;
        ret[0] = MULTIPLY(GAMMA_0(),tmp);
        ret[1] = MULTIPLY(GAMMA_1(),tmp);
        ret[2] = MULTIPLY(GAMMA_2(),tmp);
        ret[3] = MULTIPLY(GAMMA_3(),tmp);
        ret[4] = GAMMA_5();
        return ret ;
    }
    ////////////////////////////////
    // Functions on Matrices ///////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    TRACE (
        TYPE_MAIN_MATRIX const &
            a
    ) {
        return
            a[0][0] + a[1][1] +
            a[2][2] + a[3][3]
        ; //
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    MULTIPLY (
        TYPE_MAIN_MATRIX  const & a ,
        TYPE_COMPLEX_DATA const   b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp = ret.FLATTEN();
        auto & tmp_in = a.FLATTEN();
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ) {
            tmp[i] =
                tmp_in[i] * b
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    MULTIPLY (
        TYPE_MAIN_VECTOR  const & a ,
        TYPE_COMPLEX_DATA const   b
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(
            size_t i=0;
            i<ret.SIZE();
            i++
        ) {
            ret[i] =
                a[i] * b
            ;
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    MULTIPLY (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_MATRIX const & b
    ) {
        TYPE_MAIN_MATRIX ret ;
        ret = TYPE_COMPLEX_DATA(0,0) ;
        for(size_t y=0;y<TYPE_MAIN_MATRIX::SIZE_Y();y++){
            for(size_t x=0;x<TYPE_MAIN_MATRIX::SIZE_X();x++){
                for(size_t w=0;w<TYPE_MAIN_MATRIX::SIZE_X();w++){
                    ret[y][x] +=
                        a[y][w] * b[w][x]
                    ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    MULTIPLY (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_VECTOR const & b
    ) {
        TYPE_MAIN_VECTOR ret ;
        ret = TYPE_COMPLEX_DATA(0) ;
        for(size_t y=0;y<TYPE_MAIN_MATRIX::SIZE_Y();y++){
            for(size_t x=0;x<TYPE_MAIN_MATRIX::SIZE_X();x++){
                ret[y]+=
                    a[y][x] * b[x]
                ;
            }
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_VECTOR
    ADD (
        TYPE_MAIN_VECTOR const & a ,
        TYPE_MAIN_VECTOR const & b
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(size_t i=0;i<ret.SIZE();i++){
            ret[i]=
                a[i]+b[i]
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    ADD (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_MATRIX const & b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp =
            ret.FLATTEN() ;
        //
        tmp = TYPE_COMPLEX_DATA(0) ;
        auto tmp_a =
            a.FLATTEN();
        //
        auto tmp_b =
            b.FLATTEN();
        //
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ){
            tmp[i] =
                tmp_a[i] +
                tmp_b[i]
            ; //
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_VECTOR
    SUBTRACT (
        TYPE_MAIN_VECTOR const &
            a ,
        TYPE_MAIN_VECTOR const &
            b
    ) {
        TYPE_MAIN_VECTOR
            ret
        ;
        for (
            size_t i=0;
            i<TYPE_MAIN_VECTOR::SIZE();
            i++
        ) {
            ret = a[i] - b[i] ;
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    SUBTRACT (
        TYPE_MAIN_MATRIX const &
            a ,
        TYPE_MAIN_MATRIX const &
            b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp =
            ret.FLATTEN()
        ; //
        tmp =
            TYPE_COMPLEX_DATA
                (0)
        ;
        auto tmp_a =
            a.FLATTEN()
        ; //
        auto tmp_b =
            b.FLATTEN()
        ; //
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ) {
            tmp[i] =
                tmp_a[i] -
                tmp_b[i]
            ; //
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    GET_SLASH_ACCURATE (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                ret[y][x] = 0 ;
                for(size_t i=0;i<4;i++){
                    ret[y][x] +=
                        ALL_GAMMA()[i][y][x] *
                        p[i]
                    ; //
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    GET_SLASH_FAST (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        TYPE_MAIN_MATRIX ret =
            ZERO_MATRIX()
        ; //
        ret[0][2] =   p[3] + p[0] ;
        ret[0][3] =   p[1] - (p[2]*C_I()) ;
        //
        ret[1][2] =   p[1] + (p[2]*C_I()) ;
        ret[1][3] = - p[3] + p[0] ;
        //
        ret[2][0] = - p[3] + p[0] ;
        ret[2][1] = - p[1] + (p[2]*C_I()) ;
        //
        ret[3][0] = - p[1] - (p[2]*C_I()) ;
        ret[3][1] =   p[3] + p[0] ;
        //
        return ret ;
        //
    }
    //
    static inline TYPE_MAIN_MATRIX
    GET_SLASH (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        return
            GET_SLASH_FAST
                (p)
        ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    FERMION_NUMERATOR_U (
        TYPE_MAIN_VECTOR const & p ,
        TYPE_DATA        const   m
    ) {
        return
            ADD (
                GET_SLASH(p),
                MULTIPLY(IDENTITY(),m)
            )
        ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_NUMERATOR_V (
        TYPE_MAIN_VECTOR const & p ,
        TYPE_DATA        const   m
    ) {
        return
            SUBTRACT (
                GET_SLASH(p),
                MULTIPLY(IDENTITY(),m)
            )
        ; //
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_PROPAGATOR_U (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            MULTIPLY (
                FERMION_NUMERATOR_U(p,m),
                PROPAGATOR(p,m,w)
            )
        ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_PROPAGATOR (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            FERMION_PROPAGATOR_U
                (p,m,w)
        ; //
    }
////////////////////////////////////////////////////////////////
// Vector propagator = -g_mu_nu * PROPAGATOR ///////////////////
////////////////////////////////////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    PHOTON_PROPAGATOR (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        auto k = PROPAGATOR (p) ;
        TYPE_MAIN_MATRIX ret =
            ZERO_MATRIX ()
        ; /* Prepare the matrix: */ {
            ret[0][0] = -1 ;
            ret[1][1] =  1 ;
            ret[2][2] =  1 ;
            ret[3][3] =  1 ;
        }
        return
            MULTIPLY (ret,k)
        ; //
    }
////////////////////////////////////////////////////////////////
    //
    static inline TYPE_LOOP_RESULTS constexpr
    DIMENSION () {
        TYPE_LOOP_RESULTS
            ret
        ; /* Set up the values: */ {
            ret(0) =  4.0 ; //
            ret(1) = -2.0 ; //
        }
        return
            ret
        ; //
    }
    //
    using LoopType =
        ql::QCDLoop <
            TYPE_COMPLEX_DATA ,
            TYPE_DATA         ,
            TYPE_DATA
        >
    ; //
    //
    LoopType
        LoopEvaluator
    ; //
    //
    static inline TYPE_LOOP_RESULTS constexpr
    IDENTITY_LOOP () {
        TYPE_LOOP_RESULTS const
			ret(1.0)
		;
        return
            ret
        ; //
    }
    //
    static inline TYPE_LOOP_RESULTS constexpr
    EPSILON_LOOP () {
        TYPE_LOOP_RESULTS const
            ret(0,0,0,1,0)
        ;
        return
            ret
        ; //
    }
    //
    static inline TYPE_LOOP_RESULTS constexpr
    EPSILON_2_LOOP () {
        TYPE_LOOP_RESULTS const
            ret(0,0,0,0,1)
        ;
        return
            ret
        ; //
    }
    //
	static inline TYPE_LOOP_RESULTS constexpr
	INV_EPS_HAT () {
		TYPE_LOOP_RESULTS const
			ret (
				0.0 ,
				1.0 ,
				std::log(4.0*PI()) - EULER_GAMMA() ,
				0.0 ,
				0.0
			)
		; //
		return
			ret
		; //
	}
    //
	static inline TYPE_LOOP_RESULTS constexpr
	INV_EPS_HAT_SQR () {
		return
			INV_EPS_HAT() *
			INV_EPS_HAT()
		; //
	}
    //
	static inline TYPE_LOOP_RESULTS constexpr
	EPS_HAT () {
		TYPE_LOOP_RESULTS const
			ret (
				0.0 ,
				0.0 ,
				0.0 ,
				1.0 ,
				EULER_GAMMA() - std::log(4.0*PI())
			)
		; //
		return
			ret
		; //
	}
    //
	static inline TYPE_LOOP_RESULTS constexpr
	EPS_HAT_SQR () {
		return
			EPS_HAT() *
			EPS_HAT()
		; //
	}
    //
    static inline TYPE_LOOP_RESULTS const
    FROM_EPS_HAT (
        TYPE_LOOP_RESULTS const &
            in
    ) {
        auto const term0 =
                INV_EPS_HAT_SQR() *
                in(-2)
        ; //
        auto const term1 =
                INV_EPS_HAT() *
                in(-1)
        ; //
        auto const term2 =
                IDENTITY_LOOP() *
                in(0)
        ; //
        auto const term3 =
                EPS_HAT() *
                in(1)
        ; //
        auto const term4 =
                EPS_HAT_SQR() *
                in(2)
        ; //
		TYPE_LOOP_RESULTS const
			ret (
				term4 + term3 +
				term2 + term1 +
				term0
			)
		; //
		return
			ret
		; //
    }
    //
    static inline TYPE_LOOP_RESULTS constexpr
    QCDLoop_R_GAMMA () {
        TYPE_LOOP_RESULTS
            ret ;
        ; /* Evaluate the gamma factor: */ {
            if (false) {
                ret(0) =
                    1.0
                ; //
                ret(1) =
                    -EULER_GAMMA()
                ; //
                ret(2) =
                    ( std::pow ( EULER_GAMMA() , 2 ) /  2.0 ) -
                    ( std::pow (          PI() , 2 ) / 12.0 )
                ; //
            } else {
                ret(0) =
                    TYPE_COMPLEX_DATA(1,0)
                ; //
            }
        }
        return
            ret
        ; //
    }
    //
    // Evaluate: K^(p1 + \epsilon p2)
    static inline TYPE_LOOP_RESULTS constexpr
    DIM_EXP (
        TYPE_DATA const
            K ,
        long const
            p1 = 4 ,
        TYPE_DATA const
            p2 = -2
    ) {
		return
			TYPE_LOOP_RESULTS::DIM_EXP
				(K,p1,p2)
		; //
    }
    //
    static inline TYPE_LOOP_RESULTS constexpr
    GAMMA_FUNCTION (
        long const
            i ,
        TYPE_DATA const
            j = 1.0
    ) {
        TYPE_LOOP_RESULTS
            ret
        ; /* Evaluate the return value: */ {
            if ( i > 0 ) {
                ret(0) =
                    TYPE_COMPLEX_DATA(
                        FACTORIAL(i),
                        0
                    )
                ;
            } else if ( i == 0 ) {
                ret(0) =
                    -EULER_GAMMA()
                ; //
                ret(-1) =
                    1.0 / j
                ; //
            } else if ( i < 0 ) {
                long      const n     = -i              ;
                TYPE_DATA const fact  = FACTORIAL ( n ) ;
                TYPE_DATA const Minux = MINUS_1   ( n ) ;
                TYPE_DATA const term  = Minux  /  fact  ;
                ret(0) =
                    ( HARMONIC_SUM(n) - EULER_GAMMA() ) *
                    term
                ; //
                ret(-1) =
                    term / j
                ; //
            }
        }
        return
            ret
        ;
    }
    //
	static inline TYPE_LOOP_RESULTS constexpr
	CONVERSION_FACTOR_A0 () {
		if (true) {
			TYPE_LOOP_RESULTS const
				ret (1.0)
			; //
			return
				ret
			; //
		}
	}
    //
	static inline TYPE_LOOP_RESULTS constexpr
	CONVERSION_FACTOR_B0 () {
		if (true) {
			TYPE_LOOP_RESULTS
				ret (TYPE_COMPLEX_DATA (0,-1))
			;
			return
				ret
			; //
		}
	}
    //
	static inline TYPE_LOOP_RESULTS constexpr
	CONVERSION_FACTOR_C0 () {
		if (true) {
			TYPE_LOOP_RESULTS
				ret (
					static_cast <TYPE_DATA> (-1.0)
				)
			;
			return
				ret
			; //
		}
	}
    //
    inline TYPE_LOOP_RESULTS
    A0 (
        TYPE_DATA const
            m ,
        TYPE_DATA const
            mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
		TYPE_LOOP_RESULTS const
			ret (
				res[2] ,
				res[1] ,
				res[0] ,
				TYPE_COMPLEX_DATA (0,0) ,
				TYPE_COMPLEX_DATA (0,0)
			)
		;
		return
			ret
		; //
    }
    //
	inline TYPE_LOOP_RESULTS
	LoopIntegral (
		TYPE_DATA const
			m ,
		TYPE_DATA const
			mu2 = 1.0
	) {
		TYPE_LOOP_RESULTS const
			ret1 =
				A0 (
					m * m ,
					mu2
				)
		; //
		TYPE_LOOP_RESULTS const
			ret2 =
				FROM_EPS_HAT(ret1) *
				CONVERSION_FACTOR_A0() /
				(-std::pow(4.0*PI(),2))
		; //
		return
			ret2
		; //
	}
    //
    inline TYPE_LOOP_RESULTS
    Approx_A0 (
        TYPE_DATA const
            m ,
        TYPE_DATA const
            mu2 = 1.0
    ) {
		TYPE_LOOP_RESULTS const
			original =
				LoopIntegral
					(m,mu2)
		; //
		TYPE_DATA const
			DELTA =
				std::pow(m,2)
		; //
        TYPE_LOOP_RESULTS
            ret
        ; /* Final value: */ {
            TYPE_LOOP_RESULTS const
                term1 =
                    DIM_EXP (
                        1.0/(4.0*PI()) ,
                        2              ,
                        -1
                    ) *
                    TYPE_COMPLEX_DATA(0,-1)
            ; //
			TYPE_LOOP_RESULTS const
				term2 =
					term1 *
					GAMMA_FUNCTION
						(-1,1.0)
			; //
			TYPE_LOOP_RESULTS const
				term3 =
					term2 *
					DIM_EXP (
						DELTA ,
						1,-1.0
					)
			; //
			TYPE_LOOP_RESULTS const
				term4 =
					term3 *
					DIM_EXP (mu2,0,1.0)
			; //
            // Translating to my convention of 1 factor of i per propagator:
			ret =
				term4 * C_I()
			; //
        }
        TYPE_LOOP_RESULTS
            ret2
        ; /* Evaluate According to goto convention: */ {
            ret2(-1) =
                DELTA / (std::pow(4*PI(),2))
            ; //
            ret2(0)  =
                -DELTA * (
                    std::log (
                        4.0*PI()*mu2/DELTA
                    ) + 1.0 - EULER_GAMMA()
                ) / (std::pow(4*PI(),2))
            ; //
        }
        for(long i=-2;i<=2;i++){
            printf (
                "DEBUG(%ld) : PESKIN(%e,%e) ; QCDLoops(%e,%e) ; GOTO(%e,%e)\n" ,
                i                                        ,
                ret      (i) .real () ,
                ret      (i) .imag () ,
                original (i) .real () ,
                original (i) .imag () ,
                ret2     (i) .real () ,
                ret2     (i) .imag ()
            ) ; //
        }
        return
            ret
        ;
    }
    //
    inline TYPE_LOOP_RESULTS
    B0 (
        TYPE_DATA const m1 ,
        TYPE_DATA const m2 ,
        TYPE_DATA const p2 ,
        TYPE_DATA const mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m1)
                ; //
                ms.push_back
                    (m2)
                ; //
            } /* Momentum */ {
                ps.push_back
                    (p2)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
		TYPE_LOOP_RESULTS const
			ret (
				res[2] ,
				res[1] ,
				res[0] ,
				TYPE_COMPLEX_DATA (0,0) ,
				TYPE_COMPLEX_DATA (0,0)
			)
		;
		return
			ret
		; //
    }
    //
	inline TYPE_LOOP_RESULTS
	LoopIntegral (
		TYPE_DATA const m1 ,
		TYPE_DATA const m2 ,
		TYPE_DATA const p2 ,
		TYPE_DATA const mu2 = 1.0
	) {
		TYPE_LOOP_RESULTS const
			ret1 =
				B0 (
					m1*m1 , m2*m2 ,
					p2*p2 , mu2
				)
		; //
		TYPE_LOOP_RESULTS const
			ret2 =
				FROM_EPS_HAT(ret1) *
				CONVERSION_FACTOR_B0() /
				(-std::pow(4.0*PI(),2))
		; //
		return
			ret2
		; //
	}
    //
	inline TYPE_LOOP_RESULTS
	LoopIntegral (
		TYPE_DATA			const	m1 ,
		TYPE_DATA			const	m2 ,
		TYPE_MAIN_VECTOR	const	p2 ,
		TYPE_DATA			const	mu2 = 1.0
	) {
		TYPE_DATA p =
			NORM2(p2)
				.real()
		; //
		TYPE_LOOP_RESULTS const
			ret1 =
				B0 (
					m1*m1 , m2*m2 ,
					p     , mu2
				)
		; //
		TYPE_LOOP_RESULTS const
			ret2 =
				(
					FROM_EPS_HAT(ret1) *
					CONVERSION_FACTOR_B0()
				) /
				-std::pow(4.0*PI(),2)
		; //
		return
			ret2
		; //
	}
    //

    //
    inline TYPE_LOOP_RESULTS
    C0 (
        TYPE_DATA const m1 ,
        TYPE_DATA const m2 ,
        TYPE_DATA const m3 ,
        TYPE_DATA const p1 ,
        TYPE_DATA const p2 ,
        TYPE_DATA const p3 ,
        TYPE_DATA const mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m1)
                ; //
                ms.push_back
                    (m2)
                ; //
                ms.push_back
                    (m3)
                ; //
            } /* Momentum */ {
                ps.push_back
                    (p1)
                ; //
                ps.push_back
                    (p2)
                ; //
                ps.push_back
                    (p3)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
		TYPE_LOOP_RESULTS const
			ret (
				res[2] ,
				res[1] ,
				res[0] ,
				TYPE_COMPLEX_DATA (0,0) ,
				TYPE_COMPLEX_DATA (0,0)
			)
		;
		return
			ret
		; //
    }
    //
    inline TYPE_LOOP_RESULTS
    LoopIntegral (
        TYPE_DATA        const m1 ,
        TYPE_DATA        const m2 ,
        TYPE_DATA        const m3 ,
        TYPE_MAIN_VECTOR const q1 ,
        TYPE_MAIN_VECTOR const q2 ,
        TYPE_DATA        const mu2 = 1.0
    ) {
        auto const p1 =
            NORM2 (
                q1
            ).real()
        ; //
        auto const p2 =
            NORM2 (
                q2 - q1
            ).real()
        ; //
        auto const p3 =
            NORM2 (
                q2
            ).real()
        ; //
		auto const ret1 =
			C0 (
				m1*m1 , m2*m2 , m3*m3 ,
				p1    , p2    , p3    ,
				mu2
			)
		; //
		auto const ret =
			FROM_EPS_HAT(ret1) *
			CONVERSION_FACTOR_C0() /
			(-std::pow(4.0*PI(),2))
		; //
		return
			ret
		; //
    }
    //
    inline TYPE_LOOP_RESULTS
    APPROX_C0 (
        TYPE_DATA M ,
        TYPE_DATA m
    ) {
        TYPE_LOOP_RESULTS
            ret
        ; //
        auto p1 =
            GET_VECTOR (
                M/2.0,M/2.0,0,0
            )
        ; //
        auto p2 =
            GET_VECTOR (
                M/2.0,-M/2.0,0,0
            )
        ; //
        return
            ret
        ; //
    }
    //
} ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
class ee2mumu {
public:
    typedef float TYPE_DATA ;
    typedef MainList <TYPE_DATA>
        MatrixEval ;
    //
    typedef typename
        MatrixEval::TYPE_COMPLEX_DATA
            TYPE_COMPLEX_DATA ;
    //
    typedef typename
        MatrixEval::TYPE_MAIN_VECTOR
            TYPE_MAIN_VECTOR ;
    //
    typedef typename
        MatrixEval::TYPE_MAIN_MATRIX
            TYPE_MAIN_MATRIX ;
    //
    float            SHAT  ;
    MatrixEval       slave ;
    TYPE_MAIN_MATRIX p     ;
    //
    inline TYPE_COMPLEX_DATA term1 (
        size_t mu, size_t sigma
    ) {
        auto a1 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_V(p[1],0),
            slave.QED_VERTEX()[mu]
        );
        auto a2 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_U(p[0],0),
            slave.QED_VERTEX()[sigma]
        );
        auto a3 = slave.MULTIPLY(a1,a2);
        return slave.TRACE(a3);
    }
    //
    inline TYPE_COMPLEX_DATA
    term2 (
        size_t nu, size_t rho
    ) {
        auto a1 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_U(p[3],1.0),
            slave.QED_VERTEX()[nu]
        );
        auto a2 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_V(p[2],1.0),
            slave.QED_VERTEX()[rho]
        );
        auto a3 = slave.MULTIPLY(a1,a2);
        return slave.TRACE(a3);
    }
    //
    inline TYPE_COMPLEX_DATA
    eval () {
        //
        TYPE_COMPLEX_DATA P = slave.PROPAGATOR
            (slave.ADD(p[0],p[1]),0);
        //
        P = P * slave.DAGGER(P) ;
        //
        TYPE_COMPLEX_DATA ret = 0 ;
        //
        for(size_t mu=0;mu<4;mu++){
            for(size_t rho=0;rho<4;rho++){
                ret+=
                    slave.C_METRIC()[mu][mu]*
                    slave.C_METRIC()[rho][rho]*
                    term1(mu,rho)*
                    term2(mu,rho)*P;
            }
        }
        //
        return ret ;
        //
    }
    //
    inline TYPE_COMPLEX_DATA
    eval (
        TYPE_DATA const s     ,
        TYPE_DATA const theta ,
        TYPE_DATA const phi
    ) {
        TYPE_COMPLEX_DATA ret ;
        ret = std::sqrt(s/4.0);

        p[0] = slave.ZERO_VECTOR();
        p[1] = slave.ZERO_VECTOR();
        p[2] = slave.ZERO_VECTOR();
        p[3] = slave.ZERO_VECTOR();
        p[0][0] = ret ;
        p[1][0] = ret ;
        p[2][0] = ret ;
        p[3][0] = ret ;

        p[0][3] = ret ;
        p[1][3] = ret * TYPE_COMPLEX_DATA
            (-1.0,0) ;
        //
        p[2][3] = ret * TYPE_COMPLEX_DATA
            ( std::cos(theta) , 0 ) ;
        //
        p[2][2] = ret * TYPE_COMPLEX_DATA (
            std::sin(theta) * std::sin(phi) , 0
        ) ;
        //
        p[2][1] = ret * TYPE_COMPLEX_DATA (
            std::sin(theta) * std::cos(phi) , 0
        ) ;
        //
        p[3][1] = p[2][1] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        p[3][2] = p[2][2] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        p[3][3] = p[2][3] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        auto E4 = slave.CHARGE_E()*
            slave.CHARGE_E()*
            slave.CHARGE_E()*
            slave.CHARGE_E();
        //
        TYPE_COMPLEX_DATA Compare =
            TYPE_COMPLEX_DATA(32.0*E4/(s*s),0.0) * (
                (
                    slave.DOT(p[0],p[2]) *
                    slave.DOT(p[1],p[3])
                ) + (
                    slave.DOT(p[0],p[3]) *
                    slave.DOT(p[1],p[2])
                ) + (
                    slave.DOT(p[0],p[1])
                )
            ) ;
        //
        ret = eval () ;
        //printf("DEBUG: %e\n",Compare.real()/ret.real());
        return ret ;
    }
} ;
////////////////////////////////////////////////////////////////
