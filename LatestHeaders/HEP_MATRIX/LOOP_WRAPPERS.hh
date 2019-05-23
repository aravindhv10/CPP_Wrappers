#include "qcdloop/main.hh"

template <typename T>
class MainList {
public:

	#include "defns.hh"

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

template <typename T>
class VectorLoop {
public:

	using TYPE_SLAVE =
		MainList <T>
	;

	using TYPE_DATA =
		typename TYPE_SLAVE::TYPE_DATA
	;

	using TYPE_COMPLEX_DATA =
		typename
			TYPE_SLAVE::TYPE_COMPLEX_DATA
	;

	using TYPE_LOOP_RESULTS =
		typename
			TYPE_SLAVE::TYPE_LOOP_RESULTS
	;

	using TYPE_MAIN_VECTOR =
		typename
			TYPE_SLAVE::TYPE_MAIN_VECTOR
	;

	using TYPE_GRAM_MATRIX =
		Tensors::NN::N2D_ARRAY
			<2,2,TYPE_COMPLEX_DATA>
	;

	using TYPE_LOOP_VECTOR =
		Tensors::NN::ND_ARRAY
			<4,TYPE_LOOP_RESULTS>
	;

	TYPE_LOOP_VECTOR
		C_mu
	;

	TYPE_LOOP_RESULTS
		B121	,	B132	,	B2312	,
		C1		,	C2		,	C0		,
		C1_P_C2	,	C1_M_C2	,	CK2		,
		T1		,	T2
	;

	TYPE_COMPLEX_DATA
		F1	,	F2
	;

	TYPE_GRAM_MATRIX
		GRAM_MATRIX			,
		INVERSE_GRAM_MATRIX
	;

	TYPE_SLAVE
		slave
	;

	inline void EVAL (
		TYPE_DATA	M1	,	TYPE_DATA	M2	,	TYPE_DATA	M3	,
		TYPE_MAIN_VECTOR	Q1	,	TYPE_MAIN_VECTOR	Q2
	) {
		TYPE_COMPLEX_DATA constexpr IB2 (0.0,0.5) ;
		TYPE_COMPLEX_DATA constexpr OB2 (0.5,0.0) ;
		F1 =
			+std::pow(M1,2)
			-std::pow(M2,2)
			+slave.NORM2(Q1)
		;
		F2 =
			+std::pow(M1,2)
			-std::pow(M3,2)
			+slave.NORM2(Q2)
		;
		B132	=	slave.LoopIntegral	(	M1	,	M3	,	Q2					)	;
		B121	=	slave.LoopIntegral	(	M1	,	M2	,	Q1					)	;
		B2312	=	slave.LoopIntegral	(	M2	,	M3	,	Q2-Q1				)	;
		C0		=	slave.LoopIntegral	(	M1	,	M2	,	M3	,	Q1	,	Q2	)	;
		T1		=	((B132-B2312)*IB2)	-	(C0*F1*OB2)	;
		T2		=	((B121-B2312)*IB2)	-	(C0*F2*OB2)	;

		GRAM_MATRIX[0][0] = slave.NORM2	(Q1)	;
		GRAM_MATRIX[0][1] = slave.DOT	(Q1,Q2)	;
		GRAM_MATRIX[1][1] = slave.NORM2	(Q2)	;
		GRAM_MATRIX[1][0] = GRAM_MATRIX[0][1]	;

		INVERSE_GRAM_MATRIX =
			Tensors::FUNCTIONS::INVERT_MATRIX
				(GRAM_MATRIX)
		;

		C1	=	(T1*INVERSE_GRAM_MATRIX[0][0])	+	(T2*INVERSE_GRAM_MATRIX[0][1])	;
		C2	=	(T1*INVERSE_GRAM_MATRIX[1][0])	+	(T2*INVERSE_GRAM_MATRIX[1][1])	;

		for (size_t i=0;i<4;i++) {
			C_mu[i] =
				(C1*Q1[i])	+
				(C2*Q2[i])
			;
		}

		C1_P_C2	=	C1	+	C2	;
		C1_M_C2	=	C1	-	C2	;

		CK2 =
			(B2312*TYPE_COMPLEX_DATA(0.0,1.0)) +
			(C0*std::pow(M1,2))
		;

	}

} ;
