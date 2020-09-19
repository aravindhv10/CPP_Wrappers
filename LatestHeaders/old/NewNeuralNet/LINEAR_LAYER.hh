//////////////////////////////////////
#define _MACRO_EVAL_O1_A_			\
									\
	O1 =							\
		( I1[0] * I2->TRANSPOSE() )	\
		+ I3[0]						\
	;								\
									//
//////////////////////////////////////

//////////////////////////////////
								//
#define _MACRO_EVAL_DI3_A_		\
								\
	DO1->REDUCE_SUM(DI3);		\
	DO1_2.REDUCE_SUM(DI3_2);	\
								//
//////////////////////////////////

//////////////////////////////
#define _MACRO_EVAL_DI1_A_	\
							\
	DI1 =					\
		DO1[0] *			\
		I2[0]				\
	;						\
							//
//////////////////////////////

//////////////////////////////
#define _MACRO_EVAL_DI2_A_	\
							\
	DI2 =					\
		DO1->TRANSPOSE() *	\
		I1[0]				\
	;						\
							\
	DI2_2 =					\
		DO1_2.TRANSPOSE() *	\
		I1_2				\
	;						\
							//
//////////////////////////////

//////////////////////////////////
#define _MACRO_TYPE_PARAMETERS_	\
								\
	using TYPE_I2 =				\
		N2D_ARRAY <				\
			SIZE_Y()	,		\
			SIZE_X()	,		\
			TYPE_DATA			\
		>						\
	;							\
								\
	using TYPE_DI2 =			\
		TYPE_I2					\
	;							\
								\
	using TYPE_I3 =				\
		ND_ARRAY <				\
			SIZE_Y()	,		\
			TYPE_DATA			\
		>						\
	;							\
								\
	using TYPE_DI3 =			\
		TYPE_I3					\
	;							\
								//
//////////////////////////////////

//////////////////////////////////////
#define _MACRO_DATA_PARAMETERS_		\
									\
	TYPE_I2	const	*	I2		;	\
	TYPE_DI2			DI2		;	\
	TYPE_DI2			DI2_2	;	\
									\
	TYPE_I3	const	*	I3		;	\
	TYPE_DI3			DI3		;	\
	TYPE_DI3			DI3_2	;	\
									\
	TYPE_I1				I1_2	;	\
	TYPE_DO1			DO1_2	;	\
									//
//////////////////////////////////////

//////////////////////////////////////////
#define _MACRO_DEFS_1_					\
										\
	inline static size_t constexpr		\
	SIZE_X () { return X ; }			\
										\
	_MACRO_DEF_0_						\
										\
	inline static size_t constexpr		\
	SIZE_1 () { return SIZE_X () ; }	\
										\
	inline static size_t constexpr		\
	SIZE_2 () { return SIZE_Y () ; }	\
										\
	_MACRO_TYPE_PARAMETERS_				\
										//
//////////////////////////////////////////

	namespace NN		{

		template <
			size_t B = 1 , size_t	Y	= 1		,
			size_t X = 1 , typename	TF	= float
		> class LinearLayer {
		public:

			////////////////////////
			// DEFINITIONS BEGIN: //
			////////////////////////

			_MACRO_DEFS_1_

			inline static size_t constexpr
			SIZE_3 () { return SIZE_B () ; }

			using TYPE_SELF =
				LinearLayer <
					SIZE_B()	,
					SIZE_Y()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			using TYPE_NEXT_ACTIVATOR =
				SoftLRU <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

			using TYPE_L2 =
				AdamTrainer <
					TYPE_I2::SIZE(),
					TYPE_DATA
				>
			;

			using TYPE_L3 =
				AdamTrainer <
					TYPE_I3::SIZE(),
					TYPE_DATA
				>
			;

			//////////////////////
			// DEFINITIONS END. //
			//////////////////////

		private:

			/////////////////
			// EVAL BEGIN: //
			/////////////////

			////////////////////
			// FORWARD BEGIN: //
			////////////////////

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_A_
			}

			inline void
			EVAL_O1_BLAS () {

#ifndef CBLAS_H
				EVAL_O1();
#endif

#ifdef CBLAS_H
				O1 = I3[0] ;
				O1.MyGEMM(
					1.0,I1[0],
					I2->MARK_TRANSPOSED(),
					1.0
				) ;
#endif

			}

			//////////////////
			// FORWARD END. //
			//////////////////

			/////////////////////
			// BACKWARD BEGIN: //
			/////////////////////

			inline void
			PREPARE () {
				DO1_2.GET_SQUARED	(DO1[0])	;
				I1_2.GET_SQUARED	(I1[0])		;
			}

			////////////////////
			// I1_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI1 () {
				_MACRO_EVAL_DI1_A_
			}

			//////////////////
			// I1_PART END. //
			//////////////////

			////////////////////
			// I2_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI2 () {
				_MACRO_EVAL_DI2_A_
			}

			//////////////////
			// I2_PART END. //
			//////////////////

			////////////////////
			// I3_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI3 () {
				_MACRO_EVAL_DI3_A_
			}

			//////////////////
			// I3_PART END. //
			//////////////////

			///////////////////
			// BACKWARD END. //
			///////////////////

			///////////////
			// EVAL END. //
			///////////////

		public:

			inline void
			FORWARD ()
			{ EVAL_O1 () ; }

			inline void
			BACKWARD () {
				PREPARE () ;
#ifndef CBLAS_H
				EVAL_DI1 () ;
				EVAL_DI2 () ;
#endif
#ifdef CBLAS_H

				DI1.MyGEMM (
					0.0 ,
					DO1[0] , I2[0] , 1.0
				) ;

				DI2.MyGEMM (
					0.0 ,
					DO1->MARK_TRANSPOSED() , I1[0] , 1.0
				) ;

				DI2_2.MyGEMM (
					0.0 ,
					DO1_2.MARK_TRANSPOSED() , I1_2 , 1.0
				) ;
#endif
				EVAL_DI3();
			}

			inline void
			ASSIGN (
				TYPE_DATA const
					a = 0
			) {
				O1		= a ;
				DI1		= a ;
				DI2		= a ;
				DI3		= a ;
				DI2_2	= a ;
				DI3_2	= a ;
				DO1_2	= a ;
				I1_2	= a ;
			}

			inline void
			operator = (
				TYPE_DATA const
					a
			) { ASSIGN(a); }


			///////////////////
			// ATTACH BEGIN: //
			///////////////////

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

			/////////////////
			// ATTACH END. //
			/////////////////

			LinearLayer(){}
			~LinearLayer(){}

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

			_MACRO_DATA_A_

			_MACRO_DATA_PARAMETERS_

		} ;

	}

	namespace NN_GRAD	{

		template <
			size_t B , size_t Z ,
			size_t Y , size_t X ,
			typename TF = float
		> class LinearLayer {
		public:

			////////////////////////
			// DEFINITIONS BEGIN: //
			////////////////////////

			_MACRO_DEFS_1_

			static inline size_t constexpr
			SIZE_Z() {return Z;}

			using TYPE_ID1 =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_X()	,
					TYPE_DATA
				>
			; //

			using TYPE_DID1 =
				TYPE_ID1
			; //

			using TYPE_OD1 =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

			using TYPE_DOD1 =
				TYPE_OD1
			; //

			//////////////////////
			// DEFINITIONS END. //
			//////////////////////

		private:

			////////////////////
			// FORWARD BEGIN: //
			////////////////////

			inline void
			EVAL_O1 () {
				_MACRO_EVAL_O1_A_
			}

			inline void
			EVAL_O1_BLAS () {
#ifndef CBLAS_H
				EVAL_O1();
#endif
#ifdef CBLAS_H
				O1 = I3[0] ;
				O1.MyGEMM(
					1.0,I1[0],
					I2->MARK_TRANSPOSED(),
					1.0
				) ; //
#endif
			}

			inline void
			EVAL_OD1 () {

				if (IS_FIRST_LAYER()) {

					using TYPE_WEIGHT =
						N2D_ARRAY <
							SIZE_Y ()	,
							SIZE_Z ()	,
							TYPE_DATA
						>
					; //

					TYPE_WEIGHT const * tmp =
						reinterpret_cast
							<TYPE_WEIGHT const *>
								(I2)
					; //

					OD1[0] =
						tmp->TRANSPOSE()
					; //

					for (size_t b=1;b<SIZE_B();b++)
					{ OD1[b] = OD1[0] ; }

				} else {

					for (size_t b=0;b<SIZE_B();b++) {
						OD1[b] =
							ID1[0][b] *
							I2->TRANSPOSE()
						; //
					}

				}

			}

			//////////////////
			// FORWARD END. //
			//////////////////

			/////////////////////
			// BACKWARD BEGIN: //
			/////////////////////

			inline void
			PREPARE () {

				DO1_2.GET_SQUARED
					(DO1[0])
				; //

				I1_2.GET_SQUARED
					(I1[0])
				; //

			}

			////////////////////
			// I1_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI1 () {
				_MACRO_EVAL_DI1_A_
			}

			inline void
			EVAL_DID1 () {

				for(size_t b=0;b<SIZE_B();b++) {

					DID1[b] =
						DOD1[0][b] *
						I2[0]
					; //

				}

			}

			//////////////////
			// I1_PART END. //
			//////////////////

			////////////////////
			// I2_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI2_A () {
				_MACRO_EVAL_DI2_A_
			}

			TYPE_DI2 DI2_TMP ;

			inline void
			EVAL_DI2_B () {

				if(IS_FIRST_LAYER()) {

					using TYPE_TMP_ARRAY =
						Array::N3D_ARRAY <
							SIZE_B()	,
							SIZE_X()	,
							SIZE_Y()	,
							TYPE_DATA
						>
					; //

					TYPE_TMP_ARRAY const * tmp =
						reinterpret_cast
							<TYPE_TMP_ARRAY const *>
								(DOD1)
					; //

					for (size_t b=0;b<SIZE_B();b++) {

						DI2_TMP			=
							tmp[0][b]
								.TRANSPOSE()
						; //

						DI2.FLATTEN()	+=
							DI2_TMP.FLATTEN()
						; //

						DI2_2.ADD_SQUARED
							(DI2_TMP)
						; //

					}

				} else {

					for(size_t b=0;b<SIZE_B();b++) {

						DI2_TMP			=
							DOD1[0][b].TRANSPOSE() *
							ID1[0][b]
						; //

						DI2.FLATTEN()	+=
							DI2_TMP.FLATTEN()
						; //

						DI2_2.ADD_SQUARED
							(DI2_TMP)
						; //

					}

				}

			}

			inline void
			EVAL_DI2 () {
				EVAL_DI2_A () ;
				EVAL_DI2_B () ;
			}

			//////////////////
			// I2_PART END. //
			//////////////////

			////////////////////
			// I3_PART BEGIN: //
			////////////////////

			inline void
			EVAL_DI3 () {
				_MACRO_EVAL_DI3_A_
			}

			//////////////////
			// I3_PART END. //
			//////////////////

			///////////////////
			// BACKWARD END. //
			///////////////////

		public:

			inline void
			FORWARD () {
				EVAL_O1		() ;
				EVAL_OD1	() ;
			}

			inline void
			BACKWARD () {
				PREPARE		() ;
				EVAL_DI1	() ;
				EVAL_DI2	() ;
				EVAL_DI3	() ;
				EVAL_DID1	() ;
			}

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
				ID1 =
					CPPFileIO::get_junk
						<TYPE_ID1>()
				; //
			}

			LinearLayer()
			{MAKE_FIRST_LAYER();}

			~LinearLayer(){}

		public:

			_MACRO_DATA_B_

			_MACRO_DATA_PARAMETERS_

		} ;

	}

//////////////////////////////////
#undef _MACRO_DATA_PARAMETERS_	//
#undef _MACRO_TYPE_PARAMETERS_	//
#undef _MACRO_DEFS_1_			//
#undef _MACRO_EVAL_DI2_A_		//
#undef _MACRO_EVAL_DI1_A_		//
#undef _MACRO_EVAL_DI3_A_		//
#undef _MACRO_EVAL_O1_A_		//
//////////////////////////////////
