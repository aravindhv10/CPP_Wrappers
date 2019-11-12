#include "./NewNeuralNet/main.hh"

namespace COMMON {

    size_t constexpr
        SIZES[4] =
            {1,70,70,1}
    ; //

    inline size_t constexpr
    SIZE_T () {return 4;}

    inline size_t constexpr
    SIZE_B () {return 200;}

    inline size_t constexpr
    SIZE_N () {return 20;}

    inline size_t constexpr
    SIZE_Z () {return 1;}

    using TYPE_DATA =
		double
	;

	TYPE_DATA constexpr RESCALE =
		1.0 /
		static_cast<TYPE_DATA>(
			SIZE_B() *
			SIZE_T()
		)
	; //

}

namespace BASIC {
	using namespace COMMON ;

	using TYPE_LN1 =
		Tensors::NN_GRAD::LinearLayer <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[1]	,
			SIZES[0]	,
			TYPE_DATA
		>
	; //

	using TYPE_W1 =
		typename TYPE_LN1::TYPE_I2
	; //

	using TYPE_B1 =
		typename TYPE_LN1::TYPE_I3
	; //

	using TYPE_TW1 =
		Tensors::NN::AdamTrainer
			<TYPE_W1::SIZE(),TYPE_DATA>
	; //

	using TYPE_TB1 =
		Tensors::NN::AdamTrainer
			<TYPE_B1::SIZE(),TYPE_DATA>
	; //

	using TYPE_I1 =
		typename TYPE_LN1::TYPE_I1
	; //

	using TYPE_I1s =
		Tensors::Array::ND_ARRAY
			<SIZE_N(),TYPE_I1>
	; //

	using TYPE_LA1 =
		Tensors::NN_GRAD::SoftLRU <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[1]	,
			TYPE_DATA
		>
	; //

	using TYPE_LN2 =
		Tensors::NN_GRAD::LinearLayer <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[2]	,
			SIZES[1]	,
			TYPE_DATA
		>
	; //

	using TYPE_W2 =
		typename TYPE_LN2::TYPE_I2
	; //

	using TYPE_B2 =
		typename TYPE_LN2::TYPE_I3
	; //

	using TYPE_TW2 =
		Tensors::NN::AdamTrainer
			<TYPE_W2::SIZE(),TYPE_DATA>
	; //

	using TYPE_TB2 =
		Tensors::NN::AdamTrainer
			<TYPE_B2::SIZE(),TYPE_DATA>
	; //

	using TYPE_LA2 =
		Tensors::NN_GRAD::SoftLRU <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[2]	,
			TYPE_DATA
		>
	; //

	using TYPE_LN3 =
		Tensors::NN_GRAD::LinearLayer <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[3]	,
			SIZES[2]	,
			TYPE_DATA
		>
	; //

	using TYPE_W3 =
		typename TYPE_LN3::TYPE_I2
	; //

	using TYPE_B3 =
		typename TYPE_LN3::TYPE_I3
	; //

	using TYPE_TW3 =
		Tensors::NN::AdamTrainer
			<TYPE_W3::SIZE(),TYPE_DATA>
	; //

	using TYPE_TB3 =
		Tensors::NN::AdamTrainer
			<TYPE_B3::SIZE(),TYPE_DATA>
	; //

	using TYPE_O1 =
		typename TYPE_LN3::TYPE_O1
	; //

	using TYPE_DO1 =
		typename TYPE_LN3::TYPE_DO1
	; //

	using TYPE_OD1 =
		typename TYPE_LN3::TYPE_OD1
	; //

	using TYPE_OD1s =
		Tensors::Array::ND_ARRAY
			<SIZE_N(),TYPE_OD1>
	; //

	using TYPE_DOD1 =
		typename TYPE_LN3::TYPE_DOD1
	; //

	class Net {
	public:

		TYPE_I1			I1_1	;
		TYPE_O1			O1_0	;
		TYPE_OD1		OD1_1	;

		TYPE_I1s		I1s		;
		TYPE_OD1s		OD1s	;

		TYPE_LN1		LN1		;
		TYPE_W1			W1		;
		TYPE_B1			B1		;
		TYPE_TW1		TW1		;
		TYPE_TB1		TB1		;
		TYPE_LA1		LA1		;

		TYPE_LN2		LN2		;
		TYPE_W2			W2		;
		TYPE_B2			B2		;
		TYPE_TW2		TW2		;
		TYPE_TB2		TB2		;
		TYPE_LA2		LA2		;

		TYPE_LN3		LN3		;
		TYPE_W3			W3		;
		TYPE_B3			B3		;
		TYPE_TW3		TW3		;
		TYPE_TB3		TB3		;

		TYPE_DO1		DO1		;
		TYPE_DOD1		DOD1	;

		TYPE_O1		&	NO1		;
		TYPE_OD1	&	NOD1	;

		inline void
		Eval_I1s () {

			CPPFileIO::myrandgen <pcg64_fast>
				gener (1,1.0,10.0)
			;

			I1_1	= 1.0 ;
			O1_0	= 0.0 ;
			OD1_1	= 1.0 ;

			for(size_t n=0;n<I1s.SIZE();n++){
				for(size_t i2=0;i2<I1s[n].SIZE_2();i2++){
					I1s[n][i2][0] =
						gener[0]
					; //
					OD1s[n][i2][0][0] =
						1.0 /
						I1s[n][i2][0]
					; //
				}
			}

			DO1 = 0 ;
		}

		inline void
		MakeNet () {

			/* LN1 FORWARD: */ {
				LN1.MAKE_FIRST_LAYER();
				LN1.I2		= & W1					;
				LN1.I3		= & B1					;

				TW1.P		= & W1.FLATTEN()		;
				TW1.DP		= & LN1.DI2.FLATTEN()	;
				TW1.DP_2	= & LN1.DI2_2.FLATTEN()	;

				TB1.P		= & B1.FLATTEN()		;
				TB1.DP		= & LN1.DI3.FLATTEN()	;
				TB1.DP_2	= & LN1.DI3_2.FLATTEN()	;
			}

			/* LA1 FORWARD: */ {
				LA1.I1	= & LN1.O1	;
				LA1.ID1	= & LN1.OD1	;
			}

			/* LN2 FORWARD: */ {
				LN2.I1	= & LA1.O1	;
				LN2.ID1	= & LA1.OD1	;
				LN2.I2	= & W2		;
				LN2.I3	= & B2		;

				TW2.P		= & W2.FLATTEN()		;
				TW2.DP		= & LN2.DI2.FLATTEN()	;
				TW2.DP_2	= & LN2.DI2_2.FLATTEN()	;

				TB2.P		= & B2.FLATTEN()		;
				TB2.DP		= & LN2.DI3.FLATTEN()	;
				TB2.DP_2	= & LN2.DI3_2.FLATTEN()	;
			}

			/* LA2 FORWARD: */ {
				LA2.I1	= & LN2.O1	;
				LA2.ID1	= & LN2.OD1	;
			}

			/* LN3 FORWARD: */ {
				LN3.I1	= & LA2.O1	;
				LN3.ID1	= & LA2.OD1	;
				LN3.I2	= & W3		;
				LN3.I3	= & B3		;

				TW3.P		= & W3.FLATTEN()		;
				TW3.DP		= & LN3.DI2.FLATTEN()	;
				TW3.DP_2	= & LN3.DI2_2.FLATTEN()	;

				TB3.P		= & B3.FLATTEN()		;
				TB3.DP		= & LN3.DI3.FLATTEN()	;
				TB3.DP_2	= & LN3.DI3_2.FLATTEN()	;
			}

			/* LN3 BACKWARD: */ {
				LN3.DO1		= & DO1		;
				LN3.DOD1	= & DOD1	;
			}

			/* LA2 BACKWARD: */ {
				LA2.DO1		= & LN3.DI1		;
				LA2.DOD1	= & LN3.DID1	;
			}

			/* LN2 BACKWARD: */ {
				LN2.DO1		= & LA2.DI1		;
				LN2.DOD1	= & LA2.DID1	;
			}

			/* LA1 BACKWARD: */ {
				LA1.DO1		= & LN2.DI1		;
				LA1.DOD1	= & LN2.DID1	;
			}

			/* LN1 BACKWARD: */ {
				LN1.DO1		= & LA1.DI1		;
				LN1.DOD1	= & LA1.DID1	;
			}

		}

		inline void
		Update (
			TYPE_DATA eta =
				0.0001
		) {
			eta =
				eta /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
			TW1.UPDATE	(eta)	;
			TB1.UPDATE	(eta)	;
			TW2.UPDATE	(eta)	;
			TB2.UPDATE	(eta)	;
			TW3.UPDATE	(eta)	;
			TB3.UPDATE	(eta)	;
		}

		inline void
		Randomize () {
			Tensors::FUNCTIONS::RANDOMIZE(W1);
			Tensors::FUNCTIONS::RANDOMIZE(W2);
			Tensors::FUNCTIONS::RANDOMIZE(W3);
			Tensors::FUNCTIONS::RANDOMIZE(B1);
			Tensors::FUNCTIONS::RANDOMIZE(B2);
			Tensors::FUNCTIONS::RANDOMIZE(B3);
		}

		inline void
		Forward_Zero () {
			LN1.I1 =
				& I1_1
			; //
			LN1.FORWARD();
			LA1.FORWARD();
			LN2.FORWARD();
			LA2.FORWARD();
			LN3.FORWARD();
		}

		inline void
		Forward (
			size_t const
				i
		) {
			LN1.I1 =
				& I1s[i]
			; //
			LN1.FORWARD();
			LA1.FORWARD();
			LN2.FORWARD();
			LA2.FORWARD();
			LN3.FORWARD();
		}

		inline TYPE_DATA
		Backward_Zero () {
			for (
				size_t b = 0		;
				b < DOD1.SIZE_3()	;
				b++
			) {
				DO1[b][0] =
					NO1[b][0] -
					O1_0[b][0]
				;
				DOD1[b][0][0] =
					NOD1[b][0][0] -
					OD1_1[b][0][0]
				; //
			}
			LN3.BACKWARD();
			LA2.BACKWARD();
			LN2.BACKWARD();
			LA1.BACKWARD();
			LN1.BACKWARD();
			return
				DOD1.L1_NORM()
			; //
		}

		inline TYPE_DATA
		Backward (
			size_t const
				i
		) {
			for (
				size_t b = 0		;
				b < DOD1.SIZE_3()	;
				b++
			) {
				DOD1[b][0][0] =
					NOD1[b][0][0] -
					OD1s[i][b][0][0]
				; //
			}
			LN3.BACKWARD();
			LA2.BACKWARD();
			LN2.BACKWARD();
			LA1.BACKWARD();
			LN1.BACKWARD();
			return
				DOD1.L1_NORM()
			; //
		}

		inline void
		Train_Zero (
			TYPE_DATA eta =
				0.001
		) {
			Forward_Zero();
			auto error = Backward_Zero();
			Update(eta) ;
			if(false){
				printf (
					"TOTAL ZERO ERROR = %e\n",
					error / static_cast<TYPE_DATA>(SIZE_B())
				) ; //
			}

		}

		inline TYPE_DATA
		Train (
			size_t		const	i	,
			TYPE_DATA			eta
		) {
			Forward(i);
			auto ret =
				Backward(i)
			;
			Update(eta) ;
			Train_Zero();
			Update(eta*eta) ;
			return ret ;
		}

		inline void
		Train (
			TYPE_DATA eta =
				0.001
		) {
			TYPE_DATA error = 0 ;

			for (
				size_t i = 0 ;
				i < SIZE_N() ;
				i++
			) {
				error +=
					Train (i,eta)
				; //
			}
			printf (
				"TOTAL ERROR = %e\n",
				error / static_cast<TYPE_DATA>(SIZE_N()*SIZE_B())
			) ; //
		}

		inline void
		Write () {
			printf("Writing... ");
			CPPFileIO::FileFD filefd("./pars/pars");
			filefd.writefile();
			filefd.multiwrite2file(W1);
			filefd.multiwrite2file(B1);
			filefd.multiwrite2file(W2);
			filefd.multiwrite2file(B2);
			filefd.multiwrite2file(W3);
			filefd.multiwrite2file(B3);
			printf("Wrote...\n");
		}

		inline void
		Read () {
			printf("Readine... ");
			CPPFileIO::FileFD filefd("./pars/pars");
			filefd.readfile();
			filefd.multiread2file(W1);
			filefd.multiread2file(B1);
			filefd.multiread2file(W2);
			filefd.multiread2file(B2);
			filefd.multiread2file(W3);
			filefd.multiread2file(B3);
			printf("Read...\n");
		}

		Net():
		NO1(LN3.O1),
		NOD1(LN3.OD1) {
			Eval_I1s	() ;
			MakeNet		() ;
			Randomize	() ;
		}

		~Net(){}

		static inline Net *
		NEW () {
			return
				new Net
			; //
		}

		static inline void
		CREATE () {
			auto * tmp =
				NEW()
			; //
			tmp->Write();
			delete
				tmp
			; //
		}

		static inline Net *
		GET () {
			auto * tmp =
				NEW ()
			; //
			tmp->Read();
			return
				tmp
			; //
		}

		static inline void
		TRAIN (size_t count = 1000) {

			auto * tmp =
				GET ()
			; //

			TYPE_DATA constexpr
				eta = 0.0001
			; //

			for (
				size_t i = 0	;
				i < count		;
				i++
			) {
				tmp->Train(eta);
				if((i%10)==0){
					tmp->Write();
				}
			}

			delete
				tmp
			; //
		}

		static inline void
		TRAIN_ZERO () {
			auto * tmp = GET () ;
			TYPE_DATA constexpr eta = 0.00001 ;
			TYPE_DATA constexpr eta2 = eta * eta ;
			tmp->Train_Zero(eta2);
			tmp->Write();
			delete tmp ;
		}


	} ;

}

namespace NN_GRAD_LOG {

	using namespace COMMON ;

	TYPE_DATA RATE = 0.00001;

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

		inline void
		Update (TYPE_DATA eta) {
			LN1.UPDATE(eta) ;
			LN2.UPDATE(eta) ;
			LN3.UPDATE(eta) ;
		}

		inline TYPE_DATA
		Train (
			size_t const IDX ,
			size_t const NTH
		) {

			auto		&	DO1		= DO1s[NTH].FLATTEN()			;
			auto		&	DOD1	= DOD1s[NTH].FLATTEN()			;
			auto const	&	X		= input[IDX][NTH].FLATTEN()		;
			auto const	&	Y		= One_By_X[IDX][NTH].FLATTEN()	;

			TYPE_DATA
				EPSILON = 0
			; /* Gradient backward step: */ {

				auto	tmp1	= LN1.FORWARD({NTH,input[IDX][NTH]})	;
				auto	tmp2	= LA1.FWD_FROM(tmp1)					;
				auto	tmp3	= LN2.FWD_FROM(tmp2)					;
				auto	tmp4	= LA2.FWD_FROM(tmp3)					;
				auto	tmp5	= LN3.FWD_FROM(tmp4)					;

				auto &	O1		= tmp5.O1.FLATTEN()						;
				auto &	OD1		= tmp5.OD1.FLATTEN()					;

				DO1 = 0 ;

				for (
					size_t i = 0	;
					i < DOD1.SIZE()	;
					i++
				) {
					DOD1[i]	=
						OD1[i] - Y[i]
					; //
				}

				EPSILON =
					DOD1.L1_NORM()
				; //

				auto bak5 =
					LN3.BACKWARD(
						{
							NTH			,
							DO1s[NTH]	,
							DOD1s[NTH]
						}
					)
				; //

				auto bak4 = LA2.BKW_FROM ( bak5 ) ;
				auto bak3 = LN2.BKW_FROM ( bak4 ) ;

				auto bak2 = LA1.BKW_FROM ( bak3 ) ;
				auto bak1 = LN1.BKW_FROM ( bak2 ) ;

			}

			ERRORS[NTH] =
				EPSILON
			; //

			return
				EPSILON
			; //

		}

		inline TYPE_DATA
		Train (
			size_t In_N
		) {

			#pragma omp parallel for
			for (
				size_t i = 0 ;
				i < SIZE_T() ;
				i++
			) {
				Train (In_N,i) ;
			}

			TYPE_DATA ERROR = 0 ;
			for (
				size_t i = 0 ;
				i < SIZE_T() ;
				i++
			) {
				ERROR += ERRORS[i] ;
			}

			Update (RATE) ;

			return ERROR ;

		}

		inline void
		Train () {
			TYPE_DATA error = 0 ;

			for (
				size_t i = 0 ;
				i < SIZE_N() ;
				i++
			) {
				error +=
					Train(i)
				; //
			}

			printf (
				"TOTAL ERROR = %e\n",
				error /
				static_cast<TYPE_DATA>(input.SIZE())
			) ; //
		}

		inline void
		Train_Zero (
			size_t	const	NTH
		) {

			auto &	DO1		= DO1s[NTH].FLATTEN()		;
			auto &	DOD1	= DOD1s[NTH].FLATTEN()		;

			auto	tmp1	= LN1.FORWARD({NTH,I1_1})	;
			auto	tmp2	= LA1.FWD_FROM(tmp1)		;
			auto	tmp3	= LN2.FWD_FROM(tmp2)		;
			auto	tmp4	= LA2.FWD_FROM(tmp3)		;
			auto	tmp5	= LN3.FWD_FROM(tmp4)		;

			auto &	O1		= tmp5.O1.FLATTEN()			;
			auto &	OD1		= tmp5.OD1.FLATTEN()		;

			for (size_t i=0;i<DO1.SIZE();i++) {

				DO1[i]	=
					2.0 * O1[i]
				; //

				DOD1[i]	=
					2.0 * (
						OD1[i] - 1.0
					)
				; //

			}

			auto bak5 =
				LN3.BACKWARD (
					{
						NTH			,
						DO1s[NTH]	,
						DOD1s[NTH]
					}
				)
			; //

			auto bak4 = LA2.BKW_FROM ( bak5 ) ;
			auto bak3 = LN2.BKW_FROM ( bak4 ) ;

			auto bak2 = LA1.BKW_FROM ( bak3 ) ;
			auto bak1 = LN1.BKW_FROM ( bak2 ) ;

		}

		inline void
		Train_Zero () {
			Train_Zero (0) ;
			Update (RATE) ;
		}

		inline void
		Eval (
			size_t	const	IDX	,
			size_t	const	NTH
		) {

			auto & X =
				input[IDX][NTH]
					.FLATTEN()
			; //

			auto & Y =
				output[IDX][NTH]
					.FLATTEN()
			; //

			auto tmp1 =
				LN1.FORWARD({
					NTH				,
					input[IDX][NTH]
				})
			; //

			auto	tmp2	= LA1.FWD_FROM(tmp1)	;

			auto	tmp3	= LN2.FWD_FROM(tmp2)	;
			auto	tmp4	= LA2.FWD_FROM(tmp3)	;

			auto	tmp5	= LN3.FWD_FROM(tmp4)	;

			auto &	O1		= tmp5.O1.FLATTEN()		;
			auto &	OD1		= tmp5.OD1.FLATTEN()	;

			for (
				size_t i = 0 ;
				i < Y.SIZE() ;
				i++
			) {
				Y[i] =
					O1[i]
				; //
			}

		}

		inline void
		Eval () {

			for(size_t j=0;j<SIZE_N();j++)
			for(size_t i=0;i<SIZE_T();i++)
			{ Eval(j,i); }

			auto & I1 = input.FLATTEN()		;
			auto & O1 = output.FLATTEN()	;

			FILE * F = fopen ("vals","w") ;

			for(size_t i=0;i<I1.SIZE();i++){
				fprintf (
					F			,
					"%e %e\n"	,
					I1[i]		,
					O1[i]-O1[0]
				) ; //
			}

			fclose(F);

		}

		inline void
		Write () {
			printf("Writing... ");
			LN1 >> "./pars/L1" ;
			LN2 >> "./pars/L2" ;
			LN3 >> "./pars/L3" ;
			printf("Written...\n");
		}

		inline void
		Read () {
			printf("Reading... ");
			LN1 << "./pars/L1" ;
			LN2 << "./pars/L2" ;
			LN3 << "./pars/L3" ;
			printf("Read...\n");
		}

		MainNet(){

			auto & X =
				input.FLATTEN()
			; //

			TYPE_DATA x1 = 1.0	;
			TYPE_DATA x2 = 10.0	;

			TYPE_DATA dx =
				( x2 - x1 ) /
				static_cast<TYPE_DATA>(X.SIZE())
			; //


			for(size_t n=0;n<input.SIZE_4();n++)
			for(size_t t=0;t<input.SIZE_3();t++)
			for(size_t b=0;b<input.SIZE_2();b++){
				size_t const N =
					(b*input.SIZE_3()*input.SIZE_4())	+
					(t*input.SIZE_4())					+
					n
				; //
				input[n][t][b][0] =
					x1 + (N*dx)
				; //
			}


			auto & Y =
				One_By_X.FLATTEN()
			; //

			for (
				size_t i = 0 ;
				i < X.SIZE() ;
				i++
			) {
				Y[i] =
					1.0 / X[i]
				; //
			}

			I1_1	= 1 ;
			O1_0	= 0 ;
			OD1_0	= 0 ;
		}

		~MainNet(){}

		TYPE_INPUT_DATA
			input		,
			One_By_X	,
			output
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

		inline static MainNet *
		NEW () {
			return
				new MainNet
			; //
		}

		inline static void
		CREATE () {
			auto * tmp = NEW () ;
			tmp->Write () ;
			delete tmp ;
		}

		inline static MainNet *
		GET () {
			auto * tmp = NEW () ;
			tmp->Read () ;
			return tmp ;
		}

		inline static void
		TRAIN (
			size_t const count =
				10
		) {
			auto * tmp = GET () ;

			for(size_t i=0;i<count;i++) {
				tmp->Eval();
				for(size_t j=0;j<50;j++) {
					RATE =
						RESCALE /
						100000.0
					; //
					tmp->Train();

				}
				tmp->Eval();
				if(false){
					RATE =
						RATE /
						10000.0
					; //
					tmp->Train_Zero();
				}
				tmp->Write();
				//tmp->Eval();
			}

			tmp->Write () ;

			delete tmp ;
		}


		inline static void
		TRAIN_ZERO () {

			RATE =
				RESCALE /
				1000.0
			; //

			auto * tmp = GET () ;

			for(size_t i=0;i<4;i++)
			{ tmp->Train_Zero(); }

			tmp->Write () ;

			delete tmp ;
		}

		inline static void
		EVAL () {
			auto * tmp = GET () ;
			tmp->Eval();
			delete tmp ;
		}

	} ;
}
