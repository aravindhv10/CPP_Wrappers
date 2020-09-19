#ifndef _HEADER_GUARD_NewNeuralNet_WRAPPERS_headers_
#define _HEADER_GUARD_NewNeuralNet_WRAPPERS_headers_

//////////////////////////////////////
#define _MACRO__DEFINE_0_			\
									\
	using TYPE_DATA = TF ;			\
									\
	inline static size_t constexpr	\
	SIZE_T () {return S_T;}			\
									\
	inline static size_t constexpr	\
	SIZE_B () {return S_B;}			\
									\
	inline static size_t constexpr	\
	SIZE_Y () {return S_Y;}			\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO__DEFINE_0_B_			\
									\
	_MACRO__DEFINE_0_				\
									\
	inline static size_t constexpr	\
	SIZE_Z() {return S_Z;}			\
									//
//////////////////////////////////////

//////////////////////////////////////
#define _MACRO_INHERIT_(Type_Name)	\
									\
	using Type_Name =				\
		typename					\
			TYPE_NET::Type_Name		\
	;								\
									//
//////////////////////////////////////

//////////////////////////////////
#define _MACRO_DEFINE_1_		\
								\
	using TYPE_NETS =			\
		StaticArray::ND_ARRAY <	\
			SIZE_T()	,		\
			TYPE_NET			\
		>						\
	;							\
								\
	_MACRO_INHERIT_(TYPE_I1)	\
	_MACRO_INHERIT_(TYPE_DI1)	\
	_MACRO_INHERIT_(TYPE_O1)	\
	_MACRO_INHERIT_(TYPE_DO1)	\
								//
//////////////////////////////////

//////////////////////////////////
#define _MACRO_DEFINE_1_B_		\
								\
	_MACRO_DEFINE_1_			\
								\
	_MACRO_INHERIT_(TYPE_ID1)	\
	_MACRO_INHERIT_(TYPE_DID1)	\
	_MACRO_INHERIT_(TYPE_OD1)	\
	_MACRO_INHERIT_(TYPE_DOD1)	\
								//
//////////////////////////////////

//////////////////////////////////////////
#define _MACRO_DEFINE_2_A_				\
										\
	using TYPE_FORWARD_INPUT =			\
		struct {						\
			size_t		const	i	;	\
			TYPE_I1		const &	I1	;	\
		}								\
	;									\
										\
	using TYPE_FORWARD_OUTPUT =			\
		struct {						\
			size_t		const	i	;	\
			TYPE_O1		const &	O1	;	\
		}								\
	;									\
										\
	using TYPE_BACKWARD_INPUT =			\
		struct {						\
			size_t		const	i	;	\
			TYPE_DO1	const &	DO1	;	\
		}								\
	;									\
										\
	using TYPE_BACKWARD_OUTPUT =		\
		struct {						\
			size_t		const	i	;	\
			TYPE_DI1	const &	DI1	;	\
		}								\
	;									\
										\
	inline TYPE_FORWARD_OUTPUT const	\
	FORWARD (							\
		TYPE_FORWARD_INPUT const		\
			in							\
	) {									\
										\
		NETS[in.i].I1 =					\
			& in.I1						\
		;								\
										\
		NETS[in.i]						\
			.FORWARD()					\
		;								\
										\
		TYPE_FORWARD_OUTPUT ret =		\
			{in.i, NETS[in.i].O1}		\
		;								\
										\
		return							\
			ret							\
		;								\
										\
	}									\
										\
	template <typename T1>				\
	inline TYPE_FORWARD_OUTPUT const	\
	FWD_FROM (							\
		T1 const in						\
	) {									\
		return							\
			FORWARD (					\
				{ in.i , in.O1 }		\
			)							\
		;								\
	}									\
										\
										\
	inline TYPE_BACKWARD_OUTPUT const	\
	BACKWARD (							\
		TYPE_BACKWARD_INPUT const		\
			in							\
	) {									\
										\
		NETS[in.i].DO1 =				\
			& in.DO1					\
		;								\
										\
		NETS[in.i]						\
			.BACKWARD()					\
		;								\
										\
		TYPE_BACKWARD_OUTPUT ret =		\
			{in.i,NETS[in.i].DI1}		\
		;								\
										\
		return							\
			ret							\
		;								\
										\
	}									\
										\
	template <typename T1>				\
	inline TYPE_BACKWARD_OUTPUT const	\
	BKW_FROM (							\
		T1 const in						\
	) {									\
		return							\
			BACKWARD (					\
				{ in.i , in.DI1 }		\
			)							\
		;								\
	}									\
										//
//////////////////////////////////////////

//////////////////////////////////////////////
#define _MACRO_DEFINE_2_B_					\
											\
	using TYPE_FORWARD_INPUT =				\
		struct {							\
			size_t		const	i		;	\
			TYPE_I1		const &	I1		;	\
			TYPE_ID1	const &	ID1		;	\
		}									\
	;										\
											\
	using TYPE_FORWARD_INPUT_FIRST =		\
		struct {							\
			size_t		const	i		;	\
			TYPE_I1		const &	I1		;	\
		}									\
	;										\
											\
	using TYPE_FORWARD_OUTPUT =				\
		struct {							\
			size_t		const	i		;	\
			TYPE_O1		const &	O1		;	\
			TYPE_OD1	const &	OD1		;	\
		}									\
	;										\
											\
	using TYPE_BACKWARD_INPUT =				\
		struct {							\
			size_t		const	i		;	\
			TYPE_DO1	const &	DO1		;	\
			TYPE_DOD1	const &	DOD1	;	\
		}									\
	;										\
											\
	using TYPE_BACKWARD_OUTPUT =			\
		struct {							\
			size_t		const	i		;	\
			TYPE_DI1	const &	DI1		;	\
			TYPE_DID1	const &	DID1	;	\
		}									\
	;										\
											\
	inline TYPE_FORWARD_OUTPUT const		\
	FORWARD (								\
		TYPE_FORWARD_INPUT const			\
			in								\
	) {										\
											\
		NETS[in.i].I1 =						\
			& in.I1							\
		;									\
											\
		NETS[in.i].ID1 =					\
			& in.ID1						\
		;									\
											\
		NETS[in.i]							\
			.FORWARD()						\
		;									\
											\
		TYPE_FORWARD_OUTPUT ret =			\
			{								\
				in.i			,			\
				NETS[in.i].O1	,			\
				NETS[in.i].OD1				\
			}								\
		;									\
											\
		return								\
			ret								\
		;									\
											\
	}										\
											\
	template <typename T1>					\
	inline TYPE_FORWARD_OUTPUT const		\
	FWD_FROM (								\
		T1 const in							\
	) {										\
		return								\
			FORWARD (						\
				{ in.i , in.O1 , in.OD1 }	\
			)								\
		;									\
	}										\
											\
	inline TYPE_FORWARD_OUTPUT const		\
	FORWARD (								\
		TYPE_FORWARD_INPUT_FIRST const		\
			in								\
	) {										\
											\
		NETS[in.i]							\
			.MAKE_FIRST_LAYER()				\
		;									\
											\
		NETS[in.i].I1 =						\
			& in.I1							\
		;									\
											\
		NETS[in.i]							\
			.FORWARD()						\
		;									\
											\
		TYPE_FORWARD_OUTPUT ret =			\
			{								\
				in.i			,			\
				NETS[in.i].O1	,			\
				NETS[in.i].OD1				\
			}								\
		;									\
											\
		return								\
			ret								\
		;									\
											\
	}										\
											\
	inline TYPE_BACKWARD_OUTPUT const		\
	BACKWARD (								\
		TYPE_BACKWARD_INPUT const			\
			in								\
	) {										\
											\
		NETS[in.i].DO1 =					\
			& in.DO1						\
		;									\
											\
		NETS[in.i].DOD1 =					\
			& in.DOD1						\
		;									\
											\
		NETS[in.i]							\
			.BACKWARD()						\
		;									\
											\
		TYPE_BACKWARD_OUTPUT ret =			\
			{								\
				in.i			,			\
				NETS[in.i].DI1	,			\
				NETS[in.i].DID1				\
			}								\
		;									\
											\
		return								\
			ret								\
		;									\
											\
	}										\
											\
	template <typename T1>					\
	inline TYPE_BACKWARD_OUTPUT const		\
	BKW_FROM (								\
		T1 const in							\
	) {										\
		return								\
			BACKWARD (						\
				{ in.i , in.DI1 , in.DID1 }	\
			)								\
		;									\
	}										\
											//
//////////////////////////////////////////////

//////////////////////////////////
#define _MACRO_IO_				\
								\
	inline void					\
	operator >> (				\
		CPPFileIO::FileFD &		\
			file				\
	) {							\
		PARS >> file ;			\
	}							\
								\
	inline void					\
	operator << (				\
		CPPFileIO::FileFD &		\
			file				\
	) {							\
		PARS << file ;			\
	}							\
								\
	inline void					\
	operator >> (				\
		std::string const		\
			filename			\
	) {							\
								\
		CPPFileIO::FileFD		\
			outfile (filename)	\
		;						\
								\
		outfile.writefile();	\
								\
		this[0] >> outfile ;	\
								\
	}							\
								\
	inline void					\
	operator << (				\
		std::string const		\
			filename			\
	) {							\
								\
		CPPFileIO::FileFD		\
			outfile (filename)	\
		;						\
								\
		outfile.readfile();		\
								\
		this[0] << outfile ;	\
								\
	}							\
								//
//////////////////////////////////
#endif
