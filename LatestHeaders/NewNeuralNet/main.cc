//#include "/usr/local/include/cblas.h"
#include "./main.hh"

namespace COMMON {

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

}

namespace X2_2_SIMPLE {

	using namespace COMMON ;

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

		Tensors::NN::SoftLRU
			<SIZE_B(),30,TYPE_DATA>
				L2
		; //

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

namespace X2_2_GRAD {

	using namespace COMMON ;

	class Net {
	public:

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY
				<SIZE_B(),1,TYPE_DATA>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		using TYPE_GRAD_OUTPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_B()	,
				1			,
				1			,
				TYPE_DATA
			>
		; //

		inline void FORWARD (
			TYPE_INPUT const &
				in
		) {
			ATTACH_ALL () ;
			L1.I1 = & in	;
			L1.FORWARD ()	;
			L2.FORWARD ()	;
			L3.FORWARD ()	;
		}

		inline TYPE_DATA
		BACKWARD (

			TYPE_OUTPUT			const &
				out1						,

			TYPE_GRAD_OUTPUT	const &
				out2

		) {

			ATTACH_ALL	()		;
			L4.I2	= & out1	;
			L4.ID2	= & out2 	;
			L4.BACKWARD	()		;
			L3.BACKWARD	()		;
			L2.BACKWARD	()		;
			L1.BACKWARD	()		;
			return
				(L4.O1.L2_NORM() + L4.OD1.L2_NORM()) /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
		}

		inline TYPE_DATA
		BACKWARD (

			TYPE_OUTPUT			const &
				out1

		) {
			ATTACH_ALL	()		;
			L4.I2	= &	out1	;
			L4.ID2	=	L4.ID1	;
			L4.BACKWARD	()		;
			L3.BACKWARD	()		;
			L2.BACKWARD	()		;
			L1.BACKWARD	()		;
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

		using TYPE_GRAD_OUTPUT =
			Tensors::Array::N4D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				1			,
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

		inline TYPE_DATA
		TRAIN (
			TYPE_INPUT			const &
				in						,

			TYPE_OUTPUT			const &
				out						,

			TYPE_GRAD_OUTPUT	const &
				out_grad				,

			TYPE_DATA eta =
				0.001
		) {

			TYPE_DATA error[SIZE_T()] ;

			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++){
				net[i].FORWARD(in[i]);
				error[i]=
					net[i]
						.BACKWARD(
							out[i] ,
							out_grad[i]
						)
				;
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

		using TYPE_GRAD =
			Tensors::Array::N5D_ARRAY <
				SIZE_N ()	,
				SIZE_T ()	,
				SIZE_B ()	,
				1			,
				1			,
				TYPE_DATA
			>
		; //

		TYPE_MATRIX	DATA_IN		;
		TYPE_MATRIX	DATA_OUT	;
		TYPE_GRAD	GRAD		;

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

			DATA_OUT += 10.0 ;

			GRAD.FLATTEN() =
				DATA_IN.FLATTEN() *
				2
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
						GRAD[i]		,
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

namespace LOG_X_GRAD {

	using namespace COMMON ;

	inline size_t constexpr
	SIZE_Z ()
	{return 1;}

	inline size_t constexpr
	SIZE_T ()
	{return 4;}

	size_t constexpr
		SIZES[3] =
			{1,30,1}
	; //

	class Net {
	public:

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY <
				SIZE_B()	,
				SIZES[0]	,
				TYPE_DATA
			>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		using TYPE_GRAD_OUTPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_B()	,
				SIZE_Z()	,
				SIZES[2]	,
				TYPE_DATA
			>
		; //

		inline void
		FORWARD () {
			L1.FORWARD ()	;
			L2.FORWARD ()	;
			L3.FORWARD ()	;
		}

		inline void FORWARD (
			TYPE_INPUT const &
				in
		) {
			ATTACH_ALL () ;
			L1.I1 = & in	;
			FORWARD () ;
		}

		inline void
		BACKWARD () {
			L4.BACKWARD	()		;
			L3.BACKWARD	()		;
			L2.BACKWARD	()		;
			L1.BACKWARD	()		;
		}

		inline TYPE_DATA
		BACKWARD (
			TYPE_GRAD_OUTPUT	const &
				out2
		) {
			ATTACH_ALL	()		;
			L4.I2	=	L4.I1	;
			L4.ID2	= &	out2 	;
			BACKWARD ()			;
			return
				(L4.O1.L2_NORM() + L4.OD1.L2_NORM()) /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
		}


		inline TYPE_DATA
		BACKWARD (

			TYPE_OUTPUT			const &
				out1						,

			TYPE_GRAD_OUTPUT	const &
				out2

		) {
			ATTACH_ALL	()		;
			L4.I2	= & out1	;
			L4.ID2	= & out2 	;
			BACKWARD ()			;
			return
				(L4.O1.L2_NORM() + L4.OD1.L2_NORM()) /
				static_cast<TYPE_DATA>(SIZE_B())
			; //
		}

		inline TYPE_DATA
		BACKWARD (

			TYPE_OUTPUT			const &
				out1

		) {
			ATTACH_ALL	()		;
			L4.I2	= &	out1	;
			L4.ID2	=	L4.ID1	;
			BACKWARD ()			;
			return
				(L4.O1.L2_NORM() + L4.OD1.L2_NORM()) /
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

		inline void
		ATTACH_ALL () {
			L1.MAKE_FIRST_LAYER()	;
			L1.ATTACH_FORWARD(L2)	;
			L2.ATTACH_FORWARD(L3)	;
			L3.ATTACH_TEACHER(L4)	;
			L4.SHORT()				;
		}

		Net () {ATTACH_ALL();}

		~Net(){}

		Tensors::NN_GRAD::LinearLayer <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[1]	,
			SIZES[0]	,
			TYPE_DATA
		>	L1	; //

		Tensors::NN_GRAD::SoftLRU <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[1]	,
			TYPE_DATA
		>	L2	; //

		Tensors::NN_GRAD::LinearLayer <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[2]	,
			SIZES[1]	,
			TYPE_DATA
		>	L3	; //

		Tensors::NN_GRAD::L2Teacher <
			SIZE_B()	,
			SIZE_Z()	,
			SIZES[2]	,
			TYPE_DATA
		>	L4	; //

	} ;

	class NetAll {
	public:

		using TYPE_INPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				SIZES[0]	,
				TYPE_DATA
			>
		; //

		using TYPE_GRAD_OUTPUT =
			Tensors::Array::N4D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				SIZE_Z()	,
				SIZES[0]	,
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

			TYPE_OUTPUT const &
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

		inline TYPE_DATA
		TRAIN (

			TYPE_INPUT			const &
				in						,

			TYPE_OUTPUT			const &
				out						,

			TYPE_GRAD_OUTPUT	const &
				out_grad				,

			TYPE_DATA eta =
				0.001

		) {

			TYPE_DATA error[SIZE_T()] ;

			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++){
				net[i].FORWARD(in[i]);
				error[i]=
					net[i]
						.BACKWARD(
							out[i] ,
							out_grad[i]
						)
				;
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

		inline TYPE_DATA
		TRAIN (

			TYPE_INPUT			const &
				in						,

			TYPE_GRAD_OUTPUT	const &
				out_grad				,

			TYPE_DATA eta =
				0.001

		) {

			TYPE_DATA error[SIZE_T()] ;

			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++){
				net[i].FORWARD(in[i]);
				error[i]=
					net[i]
						.BACKWARD (
							out_grad[i]
						)
				;
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

		using TYPE_GRAD =
			Tensors::Array::N5D_ARRAY <
				SIZE_N ()	,
				SIZE_T ()	,
				SIZE_B ()	,
				1			,
				1			,
				TYPE_DATA
			>
		; //

		TYPE_MATRIX	DATA_IN		;
		TYPE_MATRIX	DATA_OUT	;
		TYPE_GRAD	GRAD		;

		inline void
		SetUp () {

			TYPE_DATA constexpr x2 = 10.0 ;
			TYPE_DATA constexpr x1 =  0.1 ;

			TYPE_DATA constexpr dx =
				( x2 - x1 ) / (
					static_cast<TYPE_DATA>
						(TYPE_MATRIX::SIZE())
				)
			; //

			auto & tmp =
				DATA_IN.FLATTEN()
			; //

			auto & tmp_grad =
				GRAD.FLATTEN()
			; //

			auto & tmp_out =
				DATA_OUT.FLATTEN()
			; //

			for(size_t i=0;i<tmp.SIZE();i++) {

				tmp[i] =
					x1 + (
						static_cast<TYPE_DATA>(i) *
						dx
					)
				; //

				tmp_grad[i] =
					1.0 / tmp[i]
				;

				tmp_out[i] =
					std::log(tmp[i])
				; //

			}

			printf("DONE ALLOCATING...\n");

			if(false){

				DATA_OUT.FLATTEN().GET_SQUARED
					(DATA_IN.FLATTEN())
				; //

				DATA_OUT += 10.0 ;

				GRAD.FLATTEN() =
					DATA_IN.FLATTEN() *
					2
				; //

			}


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
				if (false) {
					auto res =
						mainnet.TRAIN (
							DATA_IN[i]	,
							DATA_OUT[i]	,
							GRAD[i]		,
							eta
						)
					; //
					sum += res ;
				} else if (true) {
					auto res =
						mainnet.TRAIN (
							DATA_IN[i]	,
							GRAD[i]		,
							eta
						)
					; //
					sum += res ;
				}
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

int main () {

	if(true){

		auto * slave =
			new LOG_X_GRAD::DataTrainer
		; //

		for(size_t e=0;e<1000;e++)
		{slave->TRAIN(1.0);}

		delete slave ;

	}

	return 0 ;
}
