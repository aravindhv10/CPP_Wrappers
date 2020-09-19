//////////////////////////////////////////
#define _MACRO_EVAL_O1_					\
										\
	auto & out = O1.FLATTEN		() ;	\
	auto & in1 = I1->FLATTEN	() ;	\
	auto & in2 = I2->FLATTEN	() ;	\
										\
	for(size_t i=0;i<SIZE();i++)		\
	{ out[i] = in1[i] - in2[i] ; }		\
										//
//////////////////////////////////////////

//////////////////////////////////////
#define _MACRO_DEF_1_				\
									\
	static inline size_t constexpr	\
	SIZE_X () {return Y;}			\
									\
	_MACRO_DEF_0_					\
									\
	static inline size_t constexpr	\
	SIZE_1 () {return SIZE_X();}	\
									\
	static inline size_t constexpr	\
	SIZE_2 () {return SIZE_Y();}	\
									\
	inline static size_t constexpr	\
	SIZE() {						\
		return						\
			SIZE_Y()	*			\
			SIZE_B()				\
		;							\
	}								\
									\
	using TYPE_I2 =					\
		TYPE_O1						\
	;								\
									//
//////////////////////////////////////

//////////////////////////////////
#define _MACRO_DATA_C_			\
								\
	TYPE_O1				O1	;	\
	TYPE_I1	const	*	I1	;	\
	TYPE_I2	const	*	I2	;	\
								//
//////////////////////////////////

//////////////////////////////////////
#define _MACRO_ATTACH_BACKWARD_C_	\
									\
	Layer.DO1	= & O1			;	\
	I1			= & Layer.O1	;	\
									//
//////////////////////////////////////

	namespace NN {

		template <
			size_t		B	, size_t Y ,
			typename	TF
		> class L2Teacher {
		public:

			_MACRO_DEF_1_

			using TYPE_SELF =
				L2Teacher <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_
			}

			inline void
			BACKWARD () {
				EVAL_O1 () ;
			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {
				_MACRO_ATTACH_BACKWARD_C_
			}

			inline void
			SHORT () {
				I2 = I1 ;
			}

			L2Teacher(){}
			~L2Teacher(){}

			template <typename T>
			static inline TYPE_SELF
			FROM_LAYER (T & in) {

				TYPE_SELF
					ret
				; /* Attach the layer: */ {
					ret.ATTACH_BACKWARD
						(in)
					; //
				}

				return
					ret
				; //

			}

			_MACRO_DATA_C_

		} ;

	}

	namespace NN_GRAD {

		template <
			size_t B , size_t	Z  ,
			size_t Y , typename	TF
		> class L2Teacher {
		public:

			static inline size_t constexpr
			SIZE_Z(){return Z;}

			_MACRO_DEF_1_

			using TYPE_3D_ARRAY =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_ID1 =
				TYPE_3D_ARRAY
			; //

			using TYPE_ID2 =
				TYPE_3D_ARRAY
			; //

			using TYPE_OD1 =
				TYPE_3D_ARRAY
			; //

		private:

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_
			}

			inline void
			EVAL_OD1 () {

				auto const & in1 =
					ID1->FLATTEN()
				; //

				auto const & in2 =
					ID2->FLATTEN()
				; //

				auto & out =
					OD1.FLATTEN()
				; //

				for(
					size_t i = 0 ;
					i<TYPE_3D_ARRAY::SIZE() ;
					i++
				) {
					out[i] =
						in1[i] -
						in2[i]
					; //
				}

			}

		public:

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {
				_MACRO_ATTACH_BACKWARD_C_
				ID1			= & Layer.OD1	;
				Layer.DOD1	= & OD1			;
			}

			inline void
			BACKWARD () {
				EVAL_O1		() ;
				EVAL_OD1	() ;
			}

			inline void
			SHORT () {
				I2 = I1 ;
				ID2 = ID1 ;
			}

			L2Teacher(){}
			~L2Teacher(){}

			_MACRO_DATA_C_

			TYPE_ID1	const	*	ID1	;
			TYPE_ID2	const	*	ID2	;
			TYPE_OD1				OD1	;

		} ;

	}

//////////////////////////////////////
#undef _MACRO_ATTACH_BACKWARD_C_	//
#undef _MACRO_DATA_C_				//
#undef _MACRO_DEF_1_				//
#undef _MACRO_EVAL_O1_				//
//////////////////////////////////////
