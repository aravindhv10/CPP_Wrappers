#ifndef _HEADER_GUARD_StaticArray_SLAVES_SLAVE_L1_NORM_
#define _HEADER_GUARD_StaticArray_SLAVES_SLAVE_L1_NORM_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../../CPPFileIO.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ SLAVE_L1_NORM

	template <typename T>
	class _MACRO_CLASS_NAME_ {

	public:

		using TYPE_DATA =
			T
		;

		inline static TYPE_DATA
		EVAL (
			TYPE_DATA const &
				in
		) {
			return
				CPPFileIO::mymod
					(in)
			;
		}

		inline static TYPE_DATA
		OPERATE
		(	TYPE_DATA const in1
		,	TYPE_DATA const in2
		) {
			return
				in1 + in2
			;
		}

	} ;

#undef _MACRO_CLASS_NAME_

#endif
