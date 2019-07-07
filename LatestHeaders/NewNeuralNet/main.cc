//#include "/usr/local/include/cblas.h"
#include "./main.hh"

namespace Tensors {

	namespace Threaded_NN {

		using namespace NN ;

		template <
			size_t		S_3  ,
			size_t		S_2  ,
			size_t		S_1  ,
			typename	TF
		> class Sample_Pars {
		public:

			static inline size_t constexpr
			SIZE_3 () {return S_3;}

			static inline size_t constexpr
			SIZE_2 () {return S_2;}

			static inline size_t constexpr
			SIZE_1 () {return S_1;}

			static inline size_t constexpr
			SIZE () {
				return
					SIZE_1() *
					SIZE_2()
				; //
			}

			static inline size_t constexpr
			SIZE_X () {return SIZE_1();}

			static inline size_t constexpr
			SIZE_Y () {return SIZE_2();}

			static inline size_t constexpr
			SIZE_T () {return SIZE_3();}

			using TYPE_DATA =
				TF
			; //

			using TYPE_SELF =
				Sample_Pars <
					SIZE_3()	,
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			; //

			using TYPE_WEIGHT_TRAINER =
				AdamTrainer <
					SIZE()		,
					TYPE_DATA
				>
			; //

			using TYPE_BIAS_TRAINER =
				AdamTrainer <
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

			using TYPE_WEIGHT =
				N2D_ARRAY <
					SIZE_Y()	,
					SIZE_X()	,
					TYPE_DATA
				>
			; //

			using TYPE_BIAS =
				ND_ARRAY <
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

		private:

			inline void UPDATE (

				size_t const
					t			,

				TF const
					eta = 0.001

			) {
				L_W.DP		= &(DW[t][0]->FLATTEN()) ;
				L_W.DP_2	= &(DW[t][1]->FLATTEN()) ;
				L_B.DP		= &(DB[t][0]->FLATTEN()) ;
				L_B.DP_2	= &(DB[t][1]->FLATTEN()) ;
				L_W.UPDATE (eta) ;
				L_B.UPDATE (eta) ;
			}

		public:

			inline void
			UPDATE (
				TF const
					eta = 0.001
			) {
				for (
					size_t t=0;
					t<SIZE_T();
					t++
				) {
					UPDATE (t,eta) ;
				}
			}

			template <typename T>
			inline void
			TO_LAYER (

				size_t const
					t			,

				T & Layer

			) {
				Layer.I2 = & W				;
				Layer.I3 = & B				;
				DW[t][0] = & Layer.DI2		;
				DW[t][1] = & Layer.DI2_2	;
				DB[t][0] = & Layer.DI3		;
				DB[t][1] = & Layer.DI3_2	;
			}

			template <typename T>
			static inline TYPE_SELF
			GET_PARS (
				T & in
			) {

				TYPE_SELF
					ret
				; /* Initialize the layer: */ {
					for(
						size_t t = 0	;
						t < SIZE_T()	;
						t++
					) {
						ret.TO_LAYER
							(t,in[t])
						; //
					}
				}

				return
					ret
				;

			}

			Sample_Pars(){
				FUNCTIONS::RANDOMIZE (W) ;
				FUNCTIONS::RANDOMIZE (B) ;
				for(size_t t=0;t<SIZE_T();t++){
					L_W.P = & (W.FLATTEN()) ;
					L_B.P = & (B.FLATTEN()) ;
				}
			}

			~Sample_Pars(){}

			TYPE_WEIGHT_TRAINER	L_W	;
			TYPE_BIAS_TRAINER	L_B	;
			TYPE_WEIGHT			W	;
			TYPE_BIAS			B	;

			N2D_ARRAY <
				SIZE_T()			,
				2					,
				TYPE_WEIGHT const	*
			>	DW					;

			N2D_ARRAY <
				SIZE_T()		,
				2				,
				TYPE_BIAS const	*
			>	DB				;

		} ;
	}

	namespace SAMPLE_NN {

		using namespace Array		;
		using namespace FUNCTIONS	;
		using namespace NN			;

		template <
			size_t		Y  ,
			size_t		X  ,
			typename	TF
		> class Sample_Pars {
		public:

			inline void UPDATE (
				TF const
					eta = 0.001
			) {
				L_W.UPDATE(eta);
				L_B.UPDATE(eta);
			}

			template <typename T>
			inline void
			TO_LAYER (
				T & Layer
			) {
				Layer.I2 = & W ;
				Layer.I3 = & B ;

				L_W.DP = & Layer.DI2.FLATTEN() ;
				L_B.DP = & Layer.DI3 ;

				L_W.DP_2 = & Layer.DI2_2.FLATTEN() ;
				L_B.DP_2 = & Layer.DI3_2 ;
			}

			Sample_Pars(){
				L_W.P = &(W.FLATTEN()) ;
				L_B.P = &(B.FLATTEN()) ;
				RANDOMIZE (W) ;
				RANDOMIZE (B) ;
			}

			~Sample_Pars(){}

			AdamTrainer	<Y*X,TF>	L_W	;
			AdamTrainer	<Y,TF>		L_B	;
			N2D_ARRAY	<Y,X,TF>	W	;
			ND_ARRAY	<Y,TF>		B	;

		} ;
	}

}

namespace X2_2 {

	inline size_t constexpr
	SIZE_T () {return 4;}

	inline size_t constexpr
	SIZE_B () {return 5000;}

	inline size_t constexpr
	SIZE_N () {return 1;}

	using TYPE_DATA = float ;

	class Pars {
	public:

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),30,1,TYPE_DATA>
				P1
		; //

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),1,30,TYPE_DATA>
				P3
		; //

		inline void
		UPDATE (
			TYPE_DATA const
				eta = 0.001
		) {
			P1.UPDATE(eta);
			P3.UPDATE(eta);
		}

		Pars(){}
		~Pars(){}
	} ;

	class Net {
	public:

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY
				<SIZE_B(),1,TYPE_DATA>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		inline void FORWARD (
			TYPE_INPUT const &
				in
		) {
			L1.I1 = & in	;
			L1.FORWARD ()	;
			L2.FORWARD ()	;
			L3.FORWARD ()	;
		}

		inline TYPE_DATA
		BACKWARD (
			TYPE_OUTPUT const &
				out
		) {
			L4.I2 = & out	;
			L4.BACKWARD ()	;
			L3.BACKWARD ()	;
			L2.BACKWARD ()	;
			L1.BACKWARD ()	;
			return
				L4.O1.L2_NORM() /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
		}

		inline void
		TO_LAYER (
			size_t	const	t		,
			Pars	&		InPars
		) {
			InPars.P1
				.TO_LAYER
					(t,L1)
			; //

			InPars.P3
				.TO_LAYER
					(t,L3)
			; //
		}

		inline void ATTACH_ALL () {
			L1.ATTACH_FORWARD(L2);
			L2.ATTACH_FORWARD(L3);
			L3.ATTACH_TEACHER(L4);
		}

		Net () {ATTACH_ALL();}

		~Net(){}

		Tensors::NN::LinearLayer
			<SIZE_B(),30,1,TYPE_DATA>
				L1
		; //

//		Tensors::NN::LinearLayer
//			<SIZE_B(),30,1,TYPE_DATA>
//				L1
//		; //

		Tensors::NN::SoftLRU
			<SIZE_B(),30,TYPE_DATA>
				L2
		; //

//		Tensors::NN::SoftLRU
//			<SIZE_B(),30,TYPE_DATA>
//				L2
//		; //

		Tensors::NN::LinearLayer
			<SIZE_B(),1,30,TYPE_DATA>
				L3
		; //

		Tensors::NN::L2Teacher
			<SIZE_B(),1,TYPE_DATA>
				L4
		; //

	} ;

	class NetAll {
	public:

		using TYPE_INPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				1			,
				TYPE_DATA
			>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		Net		net[SIZE_T()]	;
		Pars	p				;

		inline TYPE_DATA
		TRAIN (
			TYPE_INPUT const &
				in ,

			TYPE_INPUT const &
				out ,

			TYPE_DATA eta =
				0.001
		) {
			TYPE_DATA error[SIZE_T()] ;

			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++){
				net[i].FORWARD(in[i]);
				error[i]=net[i].BACKWARD(out[i]);
			}

			TYPE_DATA sum = 0 ;
			for(size_t i=0;i<SIZE_T();i++)
			{ sum += error[i] ; }

			sum /=
				static_cast<TYPE_DATA>
					(SIZE_T())
			; //

			p.UPDATE(eta);

			return sum ;

		}

		NetAll(){
			for(size_t t=0;t<SIZE_T();t++)
			{ net[t].TO_LAYER(t,p); }
		}

		~NetAll(){}

	} ;

	class DataTrainer {
	public:
		using TYPE_MATRIX =
			Tensors::Array::N4D_ARRAY <
				SIZE_N ()	,
				SIZE_T ()	,
				SIZE_B ()	,
				1			,
				TYPE_DATA
			>
		; //

		TYPE_MATRIX DATA_IN		;
		TYPE_MATRIX DATA_OUT	;

		inline void
		SetUp () {

			TYPE_DATA constexpr x2 = 10.0 ;
			TYPE_DATA constexpr x1 = -5.0 ;

			TYPE_DATA constexpr dx =
				( x2 - x1 ) / (
					static_cast<TYPE_DATA>
						(TYPE_MATRIX::SIZE())
				)
			; //

			auto & tmp =
				DATA_IN.FLATTEN()
			; //

			for(size_t i=0;i<tmp.SIZE();i++) {
				tmp[i] =
					x1 + (
						static_cast<TYPE_DATA>(i) *
						dx
					)
				; //
			}

			DATA_OUT.FLATTEN().GET_SQUARED
				(DATA_IN.FLATTEN())
			; //

		}

		NetAll mainnet ;

		inline void TRAIN (
			TYPE_DATA eta =
				0.001
		) {
			eta /=
				static_cast<TYPE_DATA>
					(SIZE_B()*SIZE_T())
			; //
			TYPE_DATA sum = 0 ;
			for(size_t i=0;i<TYPE_MATRIX::SIZE_4();i++){
				auto res =
					mainnet.TRAIN (
						DATA_IN[i]	,
						DATA_OUT[i]	,
						eta
					)
				; //
				sum += res ;
			}
			printf(
				"avg error = %e\n",
				sum /
				static_cast<TYPE_DATA>
					(TYPE_MATRIX::SIZE_4())
			) ; //
			printf("DONE...\n");
		}

		DataTrainer(){SetUp();}

		~DataTrainer(){}

	} ;

}

//#ifdef asd
namespace Log_X {

	inline size_t constexpr
	SIZE_T () {return 1;}

	inline size_t constexpr
	SIZE_B () {return 50000;}

	inline size_t constexpr
	SIZE_N () {return 1;}

	using TYPE_DATA = float ;

	class Pars {
	public:

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),30,1,TYPE_DATA>
				P1
		; //

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),1,30,TYPE_DATA>
				P3
		; //

		inline void
		UPDATE (
			TYPE_DATA const
				eta = 0.001
		) {
			P1.UPDATE(eta);
			P3.UPDATE(eta);
		}

		Pars(){}
		~Pars(){}
	} ;

	class Net {
	public:

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY
				<SIZE_B(),1,TYPE_DATA>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		inline void FORWARD (
			TYPE_INPUT const &
				in
		) {
			L1.I1 = & in	;
			L1.FORWARD ()	;
			L2.FORWARD ()	;
			L3.FORWARD ()	;
		}

		inline TYPE_DATA
		BACKWARD (
			TYPE_OUTPUT const &
				out
		) {
			L4.I2 = & out	;
			L4.ID2 = L4.ID1 ;
			L4.BACKWARD ()	;
			L3.BACKWARD ()	;
			L2.BACKWARD ()	;
			L1.BACKWARD ()	;
			return
				L4.O1.L2_NORM() /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
		}

		inline void
		TO_LAYER (
			size_t	const	t		,
			Pars	&		InPars
		) {
			InPars.P1
				.TO_LAYER
					(t,L1)
			; //

			InPars.P3
				.TO_LAYER
					(t,L3)
			; //
		}

		inline void ATTACH_ALL () {
			L1.MAKE_FIRST_LAYER();
			L1.ATTACH_FORWARD(L2);
			L2.ATTACH_FORWARD(L3);
			L3.ATTACH_TEACHER(L4);
			L4.ID2 = L4.ID1 ;
		}

		Net () {ATTACH_ALL();}

		~Net(){}

		Tensors::NN_GRAD::LinearLayer
			<SIZE_B(),1,30,1,TYPE_DATA>
				L1
		; //

		Tensors::NN_GRAD::SoftLRU
			<SIZE_B(),1,30,TYPE_DATA>
				L2
		; //

		Tensors::NN_GRAD::LinearLayer
			<SIZE_B(),1,1,30,TYPE_DATA>
				L3
		; //

		Tensors::NN_GRAD::L2Teacher
			<SIZE_B(),1,1,TYPE_DATA>
				L4
		; //

	} ;

	class NetAll {
	public:

		using TYPE_INPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				1			,
				TYPE_DATA
			>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		Net		net[SIZE_T()]	;
		Pars	p				;

		inline TYPE_DATA
		TRAIN (
			TYPE_INPUT const &
				in ,

			TYPE_INPUT const &
				out ,

			TYPE_DATA eta =
				0.001
		) {

			TYPE_DATA error[SIZE_T()] ;

			//#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++){
				net[i].FORWARD(in[i]);
				error[i]=net[i].BACKWARD(out[i]);
			}

			//return 0 ;

			TYPE_DATA sum = 0 ;
			for(size_t i=0;i<SIZE_T();i++)
			{ sum += error[i] ; }

			sum /=
				static_cast<TYPE_DATA>
					(SIZE_T())
			; //

			p.UPDATE(eta);

			return sum ;

		}

		NetAll(){
			for(size_t t=0;t<SIZE_T();t++)
			{ net[t].TO_LAYER(t,p); }
		}

		~NetAll(){}

	} ;

	class DataTrainer {
	public:
		using TYPE_MATRIX =
			Tensors::Array::N4D_ARRAY <
				SIZE_N ()	,
				SIZE_T ()	,
				SIZE_B ()	,
				1			,
				TYPE_DATA
			>
		; //

		TYPE_MATRIX DATA_IN		;
		TYPE_MATRIX DATA_OUT	;

		inline void
		SetUp () {

			TYPE_DATA constexpr x2 = 10.0 ;
			TYPE_DATA constexpr x1 = -5.0 ;

			TYPE_DATA constexpr dx =
				( x2 - x1 ) / (
					static_cast<TYPE_DATA>
						(TYPE_MATRIX::SIZE())
				)
			; //

			auto & tmp =
				DATA_IN.FLATTEN()
			; //

			for(size_t i=0;i<tmp.SIZE();i++) {
				tmp[i] =
					x1 + (
						static_cast<TYPE_DATA>(i) *
						dx
					)
				; //
			}

			DATA_OUT.FLATTEN().GET_SQUARED
				(DATA_IN.FLATTEN())
			; //

		}

		NetAll mainnet ;

		inline void TRAIN (
			TYPE_DATA eta =
				0.001
		) {
			TYPE_DATA sum = 0 ;
			for(size_t i=0;i<TYPE_MATRIX::SIZE_4();i++){
				auto res =
					mainnet.TRAIN (
						DATA_IN[i]	,
						DATA_OUT[i]	,
						eta
					)
				; //
				sum += res ;
			}
			printf(
				"avg error = %e\n",
				sum /
				static_cast<TYPE_DATA>
					(TYPE_MATRIX::SIZE_4())
			) ; //
			printf("DONE...\n");
		}

		DataTrainer(){SetUp();}

		~DataTrainer(){}

	} ;

}
//#endif

int main () {

	if(true){

		auto * slave =
			new Log_X::DataTrainer
		; //

		for(size_t e=0;e<1000;e++)
		{slave->TRAIN(1.0);}

		delete slave ;

	}

	return 0 ;
}
