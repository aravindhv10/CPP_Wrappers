//////////////////////////////////////////////////
#define _MACRO_EVAL_O1_A_						\
												\
	auto const	& in	= I1->FLATTEN	() ;	\
	auto		& out	= O1.FLATTEN	() ;	\
	for (size_t i=0;i<TYPE_O1::SIZE();i++)		\
	{ out[i] = TYPE_PARENT::Activate(in[i]) ; }	\
												//
//////////////////////////////////////////////////

//////////////////////////////////////////////////////////
#define _MACRO_EVAL_DI1_A_								\
														\
	for(size_t i=0;i<SIZE();i++){						\
														\
		DI1.FLATTEN()[i] =								\
			DO1->FLATTEN()[i] *							\
			TYPE_PARENT::D_Activate(I1->FLATTEN()[i])	\
		;												\
														\
	}													\
														//
//////////////////////////////////////////////////////////

//////////////////////////////////////////
#define _MACRO_DEF_1_					\
										\
	inline static size_t constexpr		\
	SIZE_X () { return Y ; }			\
										\
	_MACRO_DEF_0_						\
										\
	inline static size_t constexpr		\
	SIZE () {							\
		return							\
			SIZE_Y() *					\
			SIZE_B()					\
		;								\
	}									\
										\
	static inline size_t constexpr		\
	SIZE_1 () { return SIZE_Y () ; }	\
										//
//////////////////////////////////////////

	namespace NN		{

		template <
			size_t		B	= 1	,
			size_t		Y	= 1	,
			typename	TF	= float
		> class SoftLRU {
		public:

			////////////////////////
			// DEFINITIONS BEGIN: //
			////////////////////////

			_MACRO_DEF_1_

			static inline size_t constexpr
			SIZE_2 () { return SIZE_B () ; }

			using TYPE_SELF =
				SoftLRU <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_PARENT =
				TYPE_SELF
			; //

			//////////////////////
			// DEFINITIONS END. //
			//////////////////////

			/////////////////////////////////
			// ACTIVATION_FUNCTIONS BEGIN: //
			/////////////////////////////////

			static inline TYPE_DATA const
			Activate (
				TYPE_DATA const
					inval
			) {

				if (inval<0) {

					return
						inval /
						(1.0-inval)
					; //

				} else {

					return
						inval
					; //

				}

			}

			static inline TYPE_DATA const
			D_Activate (
				TYPE_DATA const
					inval
			) {

				if (inval<0) {

					return
						1.0 /
						std::pow (
							(1.0-inval) ,
							2
						)
					; //

				} else { return 1 ; }

			}

			static inline TYPE_DATA const
			D2_Activate (
				TYPE_DATA const
					inval
			) {

				if (inval<0) {

					return
						2.0 /
						std::pow (
							(1.0-inval) ,
							3
						)
					; //

				} else { return 0 ; }

			}

			///////////////////////////////
			// ACTIVATION_FUNCTIONS END. //
			///////////////////////////////

		private:

			/////////////////
			// EVAL BEGIN: //
			/////////////////

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_A_
			}

			inline void
			EVAL_DI1 () {
				_MACRO_EVAL_DI1_A_
			}

			///////////////
			// EVAL END. //
			///////////////

		public:

			/////////////////////////
			// NN_INTERFACE BEGIN: //
			/////////////////////////

			inline void
			FORWARD ()
			{ EVAL_O1 () ; }

			inline void
			BACKWARD ()
			{ EVAL_DI1 () ; }

			inline void
			operator = (
				TYPE_DATA const
					a
			) {
				O1	=	a	;
				DI1	=	a	;
			}

			template <typename T>
			inline void
			ATTACH_FORWARD (
				T & Layer
			) {
				_MACRO_ATTACH_FORWARD_A_
			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {
				_MACRO_ATTACH_BACKWARD_A_
			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {
				_MACRO_ATTACH_TEACHER_A_
			}

			///////////////////////
			// NN_INTERFACE END. //
			///////////////////////

			SoftLRU(){}
			~SoftLRU(){}

			/////////////////
			// DATA BEGIN: //
			/////////////////

			_MACRO_DATA_A_

			///////////////
			// DATA END. //
			///////////////

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

		} ;

	}

	namespace NN_GRAD	{

		template <
			size_t B , size_t	Z ,
			size_t Y , typename	TF = float
		> class SoftLRU {
		public:

			////////////////////////
			// DEFINITIONS BEGIN: //
			////////////////////////

			_MACRO_DEF_1_

			static inline size_t constexpr
			SIZE_Z(){return Z;}

			using TYPE_OD1 =
				N3D_ARRAY <
					SIZE_B(),
					SIZE_Z(),
					SIZE_Y(),
					TYPE_DATA
				>
			; //

			using TYPE_ID1 =
				TYPE_OD1
			; //

			using TYPE_DOD1 =
				TYPE_OD1
			; //

			using TYPE_DID1 =
				TYPE_DOD1
			; //

			using TYPE_PARENT =
				NN::SoftLRU <
					SIZE_B(),
					SIZE_Y(),
					TYPE_DATA
				>
			; //

			//////////////////////
			// DEFINITIONS END. //
			//////////////////////

		private:

			///////////////
			// O1 BEGIN: //
			///////////////

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_A_
			}

			inline void
			EVAL_OD1 () {

				if (IS_FIRST_LAYER()) {

					OD1 = 0 ;

					for (size_t b=0;b<SIZE_B();b++)
					for (size_t y=0;y<SIZE_Y();y++) {

						OD1[b][y][y] =
							TYPE_PARENT::D_Activate
								(I1[0][b][y])
						; //

					}

				} else {

					for(size_t b=0;b<SIZE_B();b++)
					for(size_t z=0;z<SIZE_Z();z++)
					for(size_t y=0;y<SIZE_Y();y++){

						OD1[b][z][y] =

							ID1[0][b][z][y] *

							TYPE_PARENT::D_Activate
								(I1[0][b][y])

						; //

					}

				}

			}

			/////////////
			// O1 END. //
			/////////////

			////////////////
			// DI1 BEGIN: //
			////////////////

			inline void
			EVAL_DI1_A () {
				_MACRO_EVAL_DI1_A_
			}

			inline void
			EVAL_DI1_B () {

				if(IS_FIRST_LAYER()){

					for(size_t b=0;b<SIZE_B();b++)
					for(size_t y=0;y<SIZE_Y();y++) {

						DI1[b][y] +=

							DOD1[0][b][y][y]			*

							TYPE_PARENT::D2_Activate
								(I1[0][b][y])

						; //

					}

				} else {

					for(size_t b=0;b<SIZE_B();b++)
					for(size_t z=0;z<SIZE_Z();z++)
					for(size_t y=0;y<SIZE_Y();y++) {

						DI1[b][y] +=

							DOD1[0][b][z][y]			*

							TYPE_PARENT::D2_Activate
								(I1[0][b][y])			*

							ID1[0][b][z][y]

						; //

					}

				}

			}

			inline void
			EVAL_DI1 () {
				EVAL_DI1_A () ;
				EVAL_DI1_B () ;
			}

			//////////////
			// DI1 END. //
			//////////////

			/////////////////
			// DID1 BEGIN: //
			/////////////////

			inline void
			EVAL_DID1 () {

				for(size_t b=0;b<SIZE_B();b++)
				for(size_t z=0;z<SIZE_Z();z++)
				for(size_t y=0;y<SIZE_Y();y++) {

					DID1[b][z][y] =

						DOD1[0][b][z][y]		*

						TYPE_PARENT::D_Activate
							(I1[0][b][y])

					; //

				}

			}

			///////////////
			// DID1 END. //
			///////////////


		public:

			/////////////////////////
			// NN_INTERFACE BEGIN: //
			/////////////////////////

			inline void
			FORWARD () {
				EVAL_O1		() ;
				EVAL_OD1	() ;
			}

			inline void
			BACKWARD () {
				EVAL_DI1	() ;
				EVAL_DID1	() ;
			}

			///////////////////////
			// NN_INTERFACE END. //
			///////////////////////

			///////////////////
			// ATTACH BEGIN: //
			///////////////////

			template <typename T>
			inline void
			ATTACH_FORWARD (
				T & Layer
			) {
				_MACRO_ATTACH_FORWARD_B_
			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {
				_MACRO_ATTACH_BACKWARD_B_
			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {
				_MACRO_ATTACH_TEACHER_B_
			}

			/////////////////
			// ATTACH END. //
			/////////////////

			inline bool
			IS_FIRST_LAYER () {
				_MACRO_IS_FIRST_LAYER_
			}

			inline void
			MAKE_FIRST_LAYER () {
				_MACRO_MAKE_FIRST_LAYER_
			}

			SoftLRU()
			{MAKE_FIRST_LAYER();}

			~SoftLRU(){}

			_MACRO_DATA_B_

		} ;

	}

//////////////////////////////////////
#undef _MACRO_DEF_1_				//
#undef _MACRO_EVAL_DI1_A_			//
#undef _MACRO_EVAL_O1_A_			//
//////////////////////////////////////
