#ifndef _HEADER_GUARD_StaticArray_UNROLL_UNROLLER_
#define _HEADER_GUARD_StaticArray_UNROLL_UNROLLER_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../../Headers.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ UNROLLER

	template
	<	typename WORK_TYPE
	,	size_t I
	>
	class _MACRO_CLASS_NAME_ {

	public:

		using TYPE_DATA =
			typename WORK_TYPE::TYPE_DATA
		; //

		template <typename T>
		inline static TYPE_DATA
		ENSLAVE (
			T const &
				in
		) {
			return
				WORK_TYPE::OPERATE
				(	WORK_TYPE::EVAL(in[I])
				,	UNROLLER<WORK_TYPE,I-1>::ENSLAVE(in)
				)
			; //
		}

		template <typename T>
		inline static TYPE_DATA
		ENSLAVE
		(	T const & in1
		,	T const & in2
		) {
			return
				WORK_TYPE::OPERATE
				(	WORK_TYPE::EVAL(in1[I],in2[I])
				,	UNROLLER<WORK_TYPE,I-1>::ENSLAVE(in1,in2)
				)
			; //
		}

	} ;

	template <typename WORK_TYPE>
	class _MACRO_CLASS_NAME_ <WORK_TYPE,0> {
	public:

		using TYPE_DATA =
			typename WORK_TYPE::TYPE_DATA
		;

		template <typename T>
		inline static TYPE_DATA
		ENSLAVE (
			T const &
				in
		) {
			return
				WORK_TYPE::EVAL(in[0])
			; //
		}

		template <typename T>
		inline static TYPE_DATA
		ENSLAVE
		(	T const & in1
		,	T const & in2
		) {
			return
				WORK_TYPE::EVAL(in1[0],in2[0])
			; //
		}

	} ;

#undef _MACRO_CLASS_NAME_

#endif
