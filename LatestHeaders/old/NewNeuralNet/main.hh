#include "./CPPFileIO/main.hh"

namespace Tensors {

	using namespace Array ;

//////////////////////////////////////
#define _MACRO_IS_FIRST_LAYER_		\
									\
	if (SIZE_Z()==SIZE_X()) {		\
		return						\
			CPPFileIO::is_junked	\
				(ID1)				\
		;							\
	} else {						\
		return false ;				\
	}								\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_MAKE_FIRST_LAYER_	\
									\
	ID1 =							\
		CPPFileIO::get_junk			\
			<TYPE_ID1>()			\
	;								\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_DATA_A_				\
									\
	TYPE_I1		const	*	I1	;	\
	TYPE_DI1				DI1	;	\
									\
	TYPE_O1					O1	;	\
	TYPE_DO1	const	*	DO1	;	\
									//
//////////////////////////////////////

//////////////////////////////////////////
#define _MACRO_DATA_B_					\
										\
	_MACRO_DATA_A_						\
										\
	TYPE_ID1	const	*	ID1		;	\
	TYPE_DID1				DID1	;	\
										\
	TYPE_OD1				OD1		;	\
	TYPE_DOD1	const	*	DOD1	;	\
										//
//////////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_FORWARD_A_	\
									\
	Layer.I1	= & O1			;	\
	DO1			= & Layer.DI1	;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_FORWARD_B_	\
									\
	_MACRO_ATTACH_FORWARD_A_		\
									\
	Layer.ID1	= & OD1			;	\
	DOD1		= & Layer.DID1	;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_BACKWARD_A_	\
									\
	I1			= & Layer.O1	;	\
	Layer.DO1	= & DI1			;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_BACKWARD_B_	\
									\
	_MACRO_ATTACH_BACKWARD_A_		\
									\
	ID1			= & Layer.OD1	;	\
	Layer.DOD1	= & DID1		;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_TEACHER_A_	\
									\
	Layer.I1	= & O1			;	\
	DO1			= & Layer.O1	;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_TEACHER_B_	\
									\
	_MACRO_ATTACH_TEACHER_A_		\
									\
	Layer.ID1	= & OD1			;	\
	DOD1		= & Layer.OD1	;	\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_DEF_0_				\
									\
	using TYPE_DATA = TF ;			\
									\
	inline static size_t constexpr	\
	SIZE_Y () { return Y ; }		\
									\
	inline static size_t constexpr	\
	SIZE_B () { return B ; }		\
									\
	using TYPE_I1 =					\
		N2D_ARRAY <					\
			SIZE_B(),				\
			SIZE_X(),				\
			TYPE_DATA				\
		>							\
	;								\
									\
	using TYPE_O1 =					\
		N2D_ARRAY <					\
			SIZE_B(),				\
			SIZE_Y(),				\
			TYPE_DATA				\
		>							\
	;								\
									\
	using TYPE_DO1 =				\
		TYPE_O1						\
	;								\
									\
	using TYPE_DI1 =				\
		TYPE_I1						\
	;								\
									//
//////////////////////////////////////

	#include "./RANDOMIZE.hh"
	#include "./TRAINER.hh"
	#include "./ACTIVATION_LAYER.hh"
	#include "./ACTIVATION_LAYER_SOFTSIGN.hh"
	#include "./LINEAR_LAYER.hh"
	#include "./TEACHER.hh"
	#include "./PARAMETERS.hh"

//////////////////////////////////////
#undef _MACRO_ATTACH_BACKWARD_B_	//
#undef _MACRO_ATTACH_FORWARD_B_		//
#undef _MACRO_DEF_0_				//
#undef _MACRO_ATTACH_TEACHER_B_		//
#undef _MACRO_ATTACH_TEACHER_A_		//
#undef _MACRO_ATTACH_BACKWARD_A_	//
#undef _MACRO_ATTACH_FORWARD_A_		//
#undef _MACRO_DATA_B_				//
#undef _MACRO_DATA_A_				//
#undef _MACRO_MAKE_FIRST_LAYER_		//
#undef _MACRO_IS_FIRST_LAYER_		//
//////////////////////////////////////

	#include "./WRAPPERS/main.hh"

}
