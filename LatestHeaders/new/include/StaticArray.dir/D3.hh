#ifndef _HEADER_GUARD_StaticArray_D3_
#define _HEADER_GUARD_StaticArray_D3_

/////////////////////
// Headers BEGGIN: //
/////////////////////
#include "./D2.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ N3D_ARRAY

template
<	size_t S_3
,	size_t S_2
,	size_t S_1
,	typename T = float
>
class _MACRO_CLASS_NAME_
: public

	ND_ARRAY
	<	S_3
	,	N2D_ARRAY
		<	S_2
		,	S_1
		,	T
		>
	> {

public:

	using TYPE_DATA = T ;

	inline static constexpr
	size_t SIZE_1 ()
	{ return S_1;}

	inline static constexpr
	size_t SIZE_X ()
	{ return SIZE_1();}

	inline static constexpr
	size_t SIZE_2 ()
	{ return S_2;}

	inline static constexpr
	size_t SIZE_Y ()
	{ return SIZE_2();}

	inline static constexpr
	size_t SIZE_3 ()
	{ return S_3;}

	inline static constexpr
	size_t SIZE_Z ()
	{ return SIZE_3();}

	inline static constexpr
	size_t SIZE () {
		return
			SIZE_1() *
			SIZE_2() *
			SIZE_3()
		; //
	}

	using TYPE_FLATTEN =
		ND_ARRAY <
			SIZE()   ,
			TYPE_DATA
		>
	; //

	inline TYPE_FLATTEN const &
	FLATTEN () const {
		return
			reinterpret_cast
				< TYPE_FLATTEN const & >
				( this[0] )
		; //
	}

	inline TYPE_FLATTEN &
	FLATTEN () {
		return
			reinterpret_cast
				< TYPE_FLATTEN & >
				( this[0] )
		; //
	}

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
		<	SIZE_3()
		,	SIZE_2()
		,	SIZE_1()
		,	TYPE_DATA
		>
	; //

	using TYPE_PARENT =
		ND_ARRAY
		<	SIZE_3()
		,	N2D_ARRAY
			<	SIZE_2()
			,	SIZE_1()
			,	TYPE_DATA
			>
		>
	; //

	inline void GET_SQUARED
	( TYPE_SELF const & other ) {
		FLATTEN().GET_SQUARED
			(other.FLATTEN())
		; //
	}

	_MACRO_REGISTER_ALL_OPERATORS_

	inline TYPE_DATA const
	L1_NORM () const {
		return
			FLATTEN()
			.L1_NORM()
		; //
	}

	inline TYPE_DATA const
	L2_NORM () const {
		return
			FLATTEN()
			.L2_NORM()
		; //
	}

	inline TYPE_DATA *
	GET_DATA () {
		return
			FLATTEN()
			.GET_DATA()
		; //
	}

	inline TYPE_DATA const *
	GET_DATA () const {
		return
			FLATTEN()
			.GET_DATA()
		; //
	}

} ;

#undef _MACRO_CLASS_NAME_

#endif
