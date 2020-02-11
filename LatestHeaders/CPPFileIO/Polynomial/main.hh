namespace POLYNOMIALS {


#define _MACRO_CLASS_NAME_ polynomial

	template <typename T>
	class _MACRO_CLASS_NAME_ {
	public:

		using TYPE_DATA =
				T
		; //

		using TYPE_SELF =
			_MACRO_CLASS_NAME_ <TYPE_DATA>
		; //

		using TYPE_ARRAY =
			DYNAMIC_ARRAYS::Dynamic1DArray
				<TYPE_DATA>
		; //

	public:

		inline size_t
		ORDER () const {
			return
				coefficients() -
				1
			; //
		}

		inline size_t
		operator () () const {
			return ORDER() ;
		}

	public:

		_MACRO_CLASS_NAME_(
			size_t const order = 1
		) : coefficients(order+1) {
			for (size_t i=0;i<coefficients();i++) {
				coefficients(i) = 0 ;
			}
			coefficients(1) = 1 ;
		}

		~_MACRO_CLASS_NAME_(){}

	private:

		TYPE_ARRAY coefficients ;

	} ;


#undef _MACRO_CLASS_NAME_

}
