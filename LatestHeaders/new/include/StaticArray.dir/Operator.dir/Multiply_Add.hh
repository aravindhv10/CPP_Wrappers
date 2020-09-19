#ifndef _HEADER_GUARD_StaticArray_Operators_Multiply_Add_
#define _HEADER_GUARD_StaticArray_Operators_Multiply_Add_

#define _MACRO_CLASS_NAME_ MULTIPLY_ADD

template
<	typename T1 = float
,	typename T2 = float
,	typename T3 = float
> class _MACRO_CLASS_NAME_ {

public:

	using TYPE_SELF =
		MULTIPLY_ADD
			<T1,T2,T3>
	; //

	T1 const & I1 ; //
	T2 const & I2 ; //
	T3 const & I3 ; //

	_MACRO_CLASS_NAME_
	(	T1 const & _I1
	,	T2 const & _I2
	,	T3 const & _I3
	) :	I1(_I1)
	,	I2(_I2)
	,	I3(_I3)
	{}

	template
	<	typename S1
	,	typename S2
	,	typename S3
	> static inline
	_MACRO_CLASS_NAME_ <S1,S2,S3>
	GET
	(	S1 const & s1
	,	S2 const & s2
	,	S3 const & s3
	) {

		_MACRO_CLASS_NAME_ <S1,S2,S3>
			ret (s1,s2,s3)
		; //

		return
			ret
		; //

	}

} ;

#undef _MACRO_CLASS_NAME_

#endif
