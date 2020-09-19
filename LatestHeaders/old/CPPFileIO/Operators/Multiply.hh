template <
	typename T1 = float ,
	typename T2 = float
> class MULTIPLY {
public:

	using TYPE_SELF =
		MULTIPLY
			<T1,T2>
	; //

	T1 const & I1 ; //
	T2 const & I2 ; //

	MULTIPLY (
		T1 const & _I1 ,
		T2 const & _I2
	) : I1(_I1) , I2(_I2) {}

	template <
		typename S1 ,
		typename S2
	> static inline
	MULTIPLY <S1,S2>
	GET (
		S1 const & s1 ,
		S2 const & s2
	) {

		MULTIPLY
			< S1, S2 >
				ret (
					s1 , s2
				)
		; //

		return
			ret
		; //

	}

	template <typename T3>
	inline MULTIPLY_ADD
		<T1,T2,T3>
	operator + (
		T3 const &
			I3
	) const {

		MULTIPLY_ADD
			<T1,T2,T3>
				ret (
					I1,I2,I3
				)
		; //

		return
			ret
		; //

	}

} ;
