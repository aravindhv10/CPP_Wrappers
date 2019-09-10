#include "./NewNeuralNet/main.hh"

namespace COMMON {

    size_t constexpr
        SIZES[3] =
            {1,30,1}
    ; //

    inline size_t constexpr
    SIZE_T () {return 4;}

    inline size_t constexpr
    SIZE_B () {return 200;}

    inline size_t constexpr
    SIZE_N () {return 1000;}

    inline size_t constexpr
    SIZE_Z () {return 1;}

    using TYPE_DATA =
		float
	;

}

namespace NN_GRAD_LOG {

	using namespace COMMON ;

	using TYPE_INPUT_DATA =
		Tensors::Array::N4D_ARRAY <
			SIZE_N() ,
			SIZE_T() ,
			SIZE_B() ,
			SIZES[0] ,
			TYPE_DATA
		>
	; //

	using TYPE_DO1 =
		Tensors::Array::N3D_ARRAY <
			SIZE_T() ,
			SIZE_B() ,
			SIZES[0] ,
			TYPE_DATA
		>
	; //

	using TYPE_DOD1 =
		Tensors::Array::N4D_ARRAY <
			SIZE_T() ,
			SIZE_B() ,
			SIZES[0] ,
			SIZES[0] ,
			TYPE_DATA
		>
	; //

	class MainNet {
	public:

		MainNet(){

			auto & tmp_in = input.FLATTEN();
			auto & tmp2 = one_by_x.FLATTEN();

			TYPE_DATA x1 = 2.0 ;
			TYPE_DATA x2 = 20.0;

			TYPE_DATA dx =
				( x2 - x1 ) /
				static_cast<TYPE_DATA>(tmp_in.SIZE())
			; //

			for(size_t i=0;i<tmp_in.SIZE();i++){
				tmp_in[i] = x1 + (i*dx) ;
				tmp2[i] = 1.0 / tmp_in[i] ;
			}

		}

		~MainNet(){}

		inline void
		FORWARD (
			size_t const n_batch ,
			size_t const idx_th
		) {

			auto	tmp_input	= input[n_batch][idx_th]			;
			auto	tmp1		= LN1.FORWARD({n_batch,tmp_input})	;
			auto	tmp2		= LA1.FWD_FROM(tmp1)				;
			auto	tmp3		= LN2.FWD_FROM(tmp2)				;
			auto &	tmp_DO1		= DO1[idx_th].FLATTEN()				;
			auto &	tmp_DOD1	= DOD1[idx_th].FLATTEN()			;
			auto X				= tmp_input.FLATTEN()				;

			for(size_t i=0;i<tmp_DO1.SIZE();i++){

				tmp_DO1[i]	=
					2.0 * (
						tmp3.O1.FLATTEN()[i]				+
						((X[i]-1.0)*tmp3.OD1.FLATTEN()[i])	-
						(1.0/X[i])
					)
				; //

				tmp_DOD1[i]	=
					2.0 * ( X[i] - 1.0 ) * (
						tmp3.O1.FLATTEN()[i]						+
						( ( X[i] - 1.0 ) * tmp3.OD1.FLATTEN()[i] )	-
						( 1.0 / X[i] )
					)
				; //
			}

		}

		TYPE_INPUT_DATA
			input ,
			one_by_x
		; //

		TYPE_DO1	DO1		;
		TYPE_DOD1	DOD1	;

		Tensors::THREADED_NN_GRAD::LinearLayer <
			SIZE_T()	,	SIZE_B()	,
			SIZE_Z()	,	SIZES[1]	,
			SIZES[0]	,	TYPE_DATA
		> LN1 ;

		Tensors::THREADED_NN_GRAD::SoftSign <
			SIZE_T()	,	SIZE_B()	,
			SIZE_Z()	,	SIZES[1]	,
			TYPE_DATA
		> LA1 ;

		Tensors::THREADED_NN_GRAD::LinearLayer <
			SIZE_T()	,	SIZE_B()	,
			SIZE_Z()	,	SIZES[2]	,
			SIZES[1]	,	TYPE_DATA
		> LN2 ;

	} ;
}

int main () {

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

    return 0 ;
}
