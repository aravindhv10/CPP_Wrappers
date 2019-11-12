#include "./NewNeuralNet/main.hh"

namespace COMMON {

    size_t constexpr
        SIZES[4] =
            {1,30,30,1}
    ; //

    inline size_t constexpr
    SIZE_T () {return 4;}

    inline size_t constexpr
    SIZE_B () {return 200;}

    inline size_t constexpr
    SIZE_N () {return 100;}

    inline size_t constexpr
    SIZE_Z () {return 1;}

    using TYPE_DATA =
		double
	;

}

namespace NN_GRAD_LOG {

	using namespace COMMON ;

	TYPE_DATA RATE = 0.001;

//////////////////////////////////////////////////
#define _MACRO_LL_(SIZE_Y,SIZE_X)				\
												\
	Tensors::THREADED_NN_GRAD::LinearLayer <	\
		SIZE_T()	,	SIZE_B()	,			\
		SIZE_Z()	,	SIZE_Y		,			\
		SIZE_X		,	TYPE_DATA				\
	>											\
												//
//////////////////////////////////////////////////
#define _MACRO_LA_(SIZE_Y)					\
											\
	Tensors::THREADED_NN_GRAD::SoftLRU <	\
		SIZE_T()	,	SIZE_B()	,		\
		SIZE_Z()	,	SIZE_Y		,		\
		TYPE_DATA							\
	>										\
											//
//////////////////////////////////////////////
#define _MACRO_INHERIT_(Name)		\
									\
	using Name =					\
		typename FINAL_NET::Name	\
	;								\
									//
//////////////////////////////////////

	using TYPE_LN1 =
		_MACRO_LL_(SIZES[1],SIZES[0])
	; //

	using TYPE_LA1 =
		_MACRO_LA_(SIZES[1])
	; //

	using TYPE_LN2 =
		_MACRO_LL_(SIZES[2],SIZES[1])
	; //

	using TYPE_LA2 =
		_MACRO_LA_(SIZES[2])
	; //

	using TYPE_LN3 =
		_MACRO_LL_(SIZES[3],SIZES[2])
	; //

	using FINAL_NET =
		TYPE_LN3
	; //

	_MACRO_INHERIT_(TYPE_O1)
	_MACRO_INHERIT_(TYPE_OD1)
	_MACRO_INHERIT_(TYPE_DO1)
	_MACRO_INHERIT_(TYPE_DOD1)

#undef _MACRO_INHERIT_
#undef _MACRO_LA_
#undef _MACRO_LL_

	using TYPE_DO1s		=
		Tensors::Array::ND_ARRAY
			<SIZE_T(),TYPE_DO1>
	; //

	using TYPE_DOD1s	=
		Tensors::Array::ND_ARRAY
			<SIZE_T(),TYPE_DOD1>
	; //

	using TYPE_I1 =
		typename TYPE_LN1::TYPE_I1
	; //

	using TYPE_INPUT_DATA =
		Tensors::Array::N4D_ARRAY <
			SIZE_N()	,
			SIZE_T()	,
			SIZE_B()	,
			SIZES[0]	,
			TYPE_DATA
		>
	; //

	class MainNet {
	public:

		inline TYPE_DATA
		TRAIN (
			size_t	const	NTH		,
			TYPE_I1	const &	T_I1
		) {

			auto &	tmp_DO1		= DO1s[NTH].FLATTEN()		;
			auto &	tmp_DOD1	= DOD1s[NTH].FLATTEN()		;
			auto &	X			= T_I1.FLATTEN()			;

			TYPE_DATA
				EPSILON = 0
			; /* Gradient backward step: */ {

				auto	tmp1		= LN1.FORWARD({NTH,I1_1})	;
				auto	tmp2		= LA1.FWD_FROM(tmp1)		;
				auto	tmp3		= LN2.FWD_FROM(tmp2)		;
				auto	tmp4		= LA2.FWD_FROM(tmp3)		;
				auto	tmp5		= LN3.FWD_FROM(tmp4)		;

				auto &	O1			= tmp5.O1.FLATTEN()			;
				auto &	OD1			= tmp5.OD1.FLATTEN()		;

				for(size_t i=0;i<tmp_DO1.SIZE();i++){

					tmp_DO1[i]	=
						0
					; //

					tmp_DOD1[i]	=
						OD1[i] -  ( 1.0 / X[i] )
					; //

				}

				auto bak5 =
					LN3.BACKWARD(
						{NTH,DO1s[NTH],DOD1s[NTH]}
					)
				; //

				auto bak4 = LA2.BKW_FROM ( bak5 ) ;
				auto bak3 = LN2.BKW_FROM ( bak4 ) ;

				auto bak2 = LA1.BKW_FROM ( bak3 ) ;
				auto bak1 = LN1.BKW_FROM ( bak2 ) ;

				/* Evaluate the error: */ {
					for (size_t i=0;i<OD1.SIZE();i++) {
						TYPE_DATA tmp =
							( ( X[i] - 1.0 ) * OD1[i] ) +
							O1[i] - ( 1.0 / X[i] )
						; //
						EPSILON +=
							std::sqrt (
								std::pow (tmp,2)
							)
						; //
					}
				}
			}

			ERRORS[NTH] =
				EPSILON
			; //

			return EPSILON ;

		}

		inline void
		TRAIN_ZERO (
			size_t	const	NTH
		) {

			auto &	tmp_DO1		= DO1s[NTH].FLATTEN()		;
			auto &	tmp_DOD1	= DOD1s[NTH].FLATTEN()		;

			auto	tmp1		= LN1.FORWARD({NTH,I1_1})	;
			auto	tmp2		= LA1.FWD_FROM(tmp1)		;
			auto	tmp3		= LN2.FWD_FROM(tmp2)		;
			auto	tmp4		= LA2.FWD_FROM(tmp3)		;
			auto	tmp5		= LN3.FWD_FROM(tmp4)		;

			auto &	O1			= tmp5.O1.FLATTEN()			;
			auto &	OD1			= tmp5.OD1.FLATTEN()		;

			for(size_t i=0;i<tmp_DO1.SIZE();i++){

				tmp_DO1[i]	=
					2.0*O1[i]
				; //

				tmp_DOD1[i]	=
					2.0*(OD1[i] - 1.0)
				; //

			}

			auto bak5 =
				LN3.BACKWARD(
					{NTH,DO1s[NTH],DOD1s[NTH]}
				)
			; //

			auto bak4 = LA2.BKW_FROM ( bak5 ) ;
			auto bak3 = LN2.BKW_FROM ( bak4 ) ;

			auto bak2 = LA1.BKW_FROM ( bak3 ) ;
			auto bak1 = LN1.BKW_FROM ( bak2 ) ;

		}

		inline void
		TRAIN_ZERO () {
			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++)
			{ TRAIN_ZERO (i) ; }
			UPDATE (RATE) ;
		}

		inline void
		UPDATE (TYPE_DATA eta) {
			LN1.UPDATE(eta) ;
			LN2.UPDATE(eta) ;
			LN3.UPDATE(eta) ;
		}

		inline TYPE_DATA
		TRAIN (
			size_t In_N
		) {
			/* The gradient training: */ {
				#pragma omp parallel for
				for(size_t i=0;i<SIZE_T();i++)
				{ TRAIN ( i , input[In_N][i] ) ; }
				UPDATE (RATE) ;
			}

			TYPE_DATA ERROR = 0 ;
			for(size_t i=0;i<SIZE_T();i++)
			{ ERROR+=ERRORS[i]; }

			return ERROR ;
		}

		inline void
		TRAIN () {
			TYPE_DATA error = 0 ;
			for(size_t i=0;i<SIZE_N();i++)
			{ error+=TRAIN(i); }
			printf (
				"TOTAL ERROR = %e\n",
				error /
				(static_cast<TYPE_DATA>(input.SIZE()))
			) ; //
			//TRAIN_ZERO ();
		}

		inline void
		EVAL (
			size_t	const		NTH		,
			TYPE_I1	const 	&	T_I1	,
			TYPE_O1			&	T_O1
		) {

			auto tmp1 = LN1.FORWARD({NTH,T_I1})	;
			auto tmp2 = LA1.FWD_FROM(tmp1)		;
			auto tmp3 = LN2.FWD_FROM(tmp2)		;
			auto tmp4 = LA2.FWD_FROM(tmp3)		;
			auto tmp5 = LN3.FWD_FROM(tmp4)		;

			auto & X	= T_I1.FLATTEN()	;
			auto & Y	= T_O1.FLATTEN()	;
			auto & O1	= tmp5.O1.FLATTEN()	;

			for(size_t i=0;i<Y.SIZE();i++)
			{ Y[i] = O1[i] ; }

		}

		inline void
		EVAL (
			size_t	const	NTH	,
			size_t	const	IDX
		) {
			EVAL (
				NTH					,
				input[IDX][NTH]		,
				output[IDX][NTH]
			) ; //
		}

		inline void
		EVAL () {
			for(size_t j=0;j<SIZE_N();j++)
			for(size_t i=0;i<SIZE_T();i++){
				EVAL(i,j);
			}
			auto & I1 = input.FLATTEN()		;
			auto & O1 = output.FLATTEN()	;
			FILE * F = fopen ("vals","w") ;
			for(size_t i=0;i<I1.SIZE();i++){
				fprintf(F,"%e %e\n",I1[i],O1[i]);
			}
			fclose(F);
		}

		MainNet(){

			auto & tmp_in =
				input.FLATTEN()
			; //

			TYPE_DATA x1 = 0.2	;
			TYPE_DATA x2 = 20.0	;

			TYPE_DATA dx =
				( x2 - x1 ) /
				static_cast<TYPE_DATA>(tmp_in.SIZE())
			; //

			for(size_t i=0;i<tmp_in.SIZE();i++)
			{ tmp_in[i] = x1 + (i*dx) ; }

			I1_1	= 1 ;
			O1_0	= 0 ;
			OD1_0	= 0 ;
		}

		~MainNet(){}

		TYPE_INPUT_DATA
			input , output
		; //


		Tensors::Array::ND_ARRAY
			<SIZE_T(),TYPE_DATA>
				ERRORS
		; //


		TYPE_I1		I1_1	;
		TYPE_O1		O1_0	;
		TYPE_OD1	OD1_0	;

		TYPE_DO1s	DO1s	;
		TYPE_DOD1s	DOD1s	;

		TYPE_LN1	LN1		;
		TYPE_LA1	LA1		;
		TYPE_LN2	LN2		;
		TYPE_LA2	LA2		;
		TYPE_LN3	LN3		;

	} ;
}

int main () {

	auto * tmp =
		new NN_GRAD_LOG::MainNet
	; //

	for(size_t j=0;j<100;j++){

		NN_GRAD_LOG::RATE = 0.0001 ;
		if(j<5) {
			printf("0 train: ");
			for(size_t i=0;i<5;i++)
			{tmp->TRAIN_ZERO();}
			printf("DONE...\n");
		}

		NN_GRAD_LOG::RATE = 0.0000001 ;
		for(size_t i=0;i<20;i++)
		{tmp->TRAIN();tmp->EVAL();}
	}



	delete tmp ;
/*
	if(false){

		using namespace Tensors::Array ;

		ND_ARRAY <10,float> ars ;
		float factor = 1.2 ;

		auto mul =
			D1_OPERATORS::MULTIPLY_A_S<>::GET
				(ars,factor)
		; //

		auto mul2 =
			D1_OPERATORS::MULTIPLY_A_A<>::GET
				(mul,ars)
		; //

		for(size_t i=0;i<ars.SIZE();i++){
			ars[i] = i+1;
		}

		for(size_t i=0;i<ars.SIZE();i++){
			printf("%e ",mul2[i]);
		}

		printf("\nCompiled...\n");

	}
*/

    return 0 ;
}
