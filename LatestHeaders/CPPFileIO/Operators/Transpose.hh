template <typename T=float>
class TRANSPOSE {
public:

	using TYPE_SELF =
		TRANSPOSE <T>
	; //

	template <typename T2>
	inline MULTIPLY <
		TYPE_SELF	,
		T2
	> const
	operator * (
		T2 const &
			other
	) const {

		MULTIPLY <
			TYPE_SELF	,
			T2
		> const
			ret (
				this[0] ,
				other
			)
		; //

		return
			ret
		; //

	}

	TRANSPOSE (
		T const &
			_I1
	) : I1(_I1) {}

	T const &
		I1
	; //

} ;
