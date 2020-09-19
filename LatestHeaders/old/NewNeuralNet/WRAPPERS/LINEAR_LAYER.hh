namespace THREADED_NN {

	template <
		size_t		S_T	, size_t S_B ,
		size_t		S_Y	, size_t S_X ,
		typename	TF	= float
	> class LinearLayer {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_X;}

		_MACRO__DEFINE_0_

		using TYPE_NET =
			NN::LinearLayer <
				SIZE_B()	,
				SIZE_Y()	,
				SIZE_X()	,
				TYPE_DATA
			>
		; //

		_MACRO_DEFINE_1_

		using TYPE_PARS =
			THREADED_NN::Sample_Pars <
				SIZE_T()	,
				SIZE_Y()	,
				SIZE_X()	,
				TYPE_DATA
			>
		; //

		_MACRO_DEFINE_2_A_

		inline void
		UPDATE (
			TYPE_DATA const
				eta =
					0.001
		) {

			PARS.UPDATE
				(eta)
			; //

		}

		_MACRO_IO_

		LinearLayer(){

			for(size_t i=0;i<SIZE_T();i++){
				PARS.TO_LAYER
					(i,NETS[i])
				; //
			}

		}

		~LinearLayer(){}

		TYPE_NETS NETS ;
		TYPE_PARS PARS ;

	} ;

}

namespace THREADED_NN_GRAD {

	template <
		size_t	S_T = 1	,	size_t		S_B	= 1	,
		size_t	S_Z = 1	,	size_t		S_Y	= 1	,
		size_t	S_X = 1	,	typename	TF	= float
	> class LinearLayer {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_X;}

		_MACRO__DEFINE_0_B_

		using TYPE_NET =
			NN_GRAD::LinearLayer <
				SIZE_B()	,
				SIZE_Z()	,
				SIZE_Y()	,
				SIZE_X()	,
				TYPE_DATA
			>
		; //

		_MACRO_DEFINE_1_B_

		using TYPE_PARS =
			THREADED_NN::Sample_Pars <
				SIZE_T()	,
				SIZE_Y()	,
				SIZE_X()	,
				TYPE_DATA
			>
		; //

		_MACRO_DEFINE_2_B_

		inline void
		UPDATE (
			TYPE_DATA const
				eta =
					0.001
		) {

			PARS.UPDATE
				(eta)
			; //

		}

		_MACRO_IO_

		LinearLayer(){

			for(size_t i=0;i<SIZE_T();i++){
				PARS.TO_LAYER
					(i,NETS[i])
				; //
			}

		}

		~LinearLayer(){}

		TYPE_NETS NETS ;
		TYPE_PARS PARS ;

	} ;

}
