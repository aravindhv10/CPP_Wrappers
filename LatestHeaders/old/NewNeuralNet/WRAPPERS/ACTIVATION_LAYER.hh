namespace THREADED_NN {

	template <
		size_t		S_T	,
		size_t		S_B	,
		size_t		S_Y	,
		typename	TF
	> class SoftLRU {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_Y;}

		_MACRO__DEFINE_0_

		using TYPE_NET =
			NN::SoftLRU <
				SIZE_B()	,
				SIZE_Y()	,
				TYPE_DATA
			>
		;

		_MACRO_DEFINE_1_

		_MACRO_DEFINE_2_A_

		SoftLRU(){}
		~SoftLRU(){}

		TYPE_NETS NETS ;

	} ;

}

namespace THREADED_NN_GRAD {

	template <
		size_t		S_T	,	size_t		S_B	,
		size_t		S_Z	,	size_t		S_Y	,
		typename	TF
	> class SoftLRU {
	public:

		inline static size_t constexpr
		SIZE_X () {return S_Y;}

		_MACRO__DEFINE_0_B_

		using TYPE_NET =
			NN_GRAD::SoftLRU <
				SIZE_B()	,
				SIZE_Z()	,
				SIZE_Y()	,
				TYPE_DATA
			>
		; //

		_MACRO_DEFINE_1_B_

		_MACRO_DEFINE_2_B_

		SoftLRU(){}
		~SoftLRU(){}

		TYPE_NETS NETS ;

	} ;
}
