#ifndef _HEADER_GUARD_StaticArray_Operators_Transpose_
#define _HEADER_GUARD_StaticArray_Operators_Transpose_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Multiply.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ TRANSPOSE

template <typename T=float>
class _MACRO_CLASS_NAME_ {

public:

	using TYPE_ELEMENT =
		T
	; //

	using TYPE_SELF =
		_MACRO_CLASS_NAME_ <T>
	; //

	template <typename T2>
	inline MULTIPLY
	<	TYPE_SELF
	,	T2
	> const
	operator * (
		T2 const &
			other
	) const {

		MULTIPLY
		<	TYPE_SELF
		,	T2
		> const
			ret
			(	this[0]
			,	other
			)
		; //

		return ret ;

	}

	_MACRO_CLASS_NAME_
	(	T const & _I1
	) :	I1(_I1)
	{}

	T const & I1 ;

} ;

#undef _MACRO_CLASS_NAME_

#endif
