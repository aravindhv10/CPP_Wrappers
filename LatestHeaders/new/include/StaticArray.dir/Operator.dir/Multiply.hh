#ifndef _HEADER_GUARD_StaticArray_Operators_Multiply_
#define _HEADER_GUARD_StaticArray_Operators_Multiply_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Multiply_Add.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ MULTIPLY

template
<	typename T1 = float
,	typename T2 = float
>
class _MACRO_CLASS_NAME_ {
public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
			<T1,T2>
	; //

	T1 const & I1 ; //
	T2 const & I2 ; //

	_MACRO_CLASS_NAME_
	(	T1 const & _I1
	,	T2 const & _I2
	) : I1(_I1) , I2(_I2) {}

	template
	<	typename S1
	,	typename S2
	> static inline
	_MACRO_CLASS_NAME_ <S1,S2>
	GET (
		S1 const & s1 ,
		S2 const & s2
	) {

		_MACRO_CLASS_NAME_
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

#undef _MACRO_CLASS_NAME_

#endif
