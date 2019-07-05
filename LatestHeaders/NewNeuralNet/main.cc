//#include "/usr/local/include/cblas.h"
#include "LatestHeaders/CPPFileIO/main.hh"

namespace Tensors {

	namespace FUNCTIONS {

		template <
			size_t SIZE_Y,
			size_t SIZE_X,
			typename TYPE_DATA
		> inline void
		RANDOMIZE (
			Array::N2D_ARRAY <
				SIZE_Y,
				SIZE_X,
				TYPE_DATA
			> & in
		) {

			constexpr TYPE_DATA Var =
				2.0 /
				static_cast<TYPE_DATA>
					(SIZE_X+SIZE_Y)
			; //

			std::normal_distribution
				<double>
				dist (0.0,Var)
			; //

			#ifdef PCG_RAND_HPP_INCLUDED
				pcg64_fast engine ;
			#else
				std::mt19937 engine ;
			#endif

			for ( size_t y = 0 ; y < SIZE_Y ; y++ )
			for ( size_t x = 0 ; x < SIZE_X ; x++ )
			{ in[y][x] = dist(engine) ; }

		}

		template <
			size_t SIZE,
			typename TYPE_DATA
		> inline void
		RANDOMIZE (
			Array::ND_ARRAY <
				SIZE,
				TYPE_DATA
			> & in
		) {

			constexpr TYPE_DATA Var =
				2.0 /
				static_cast<TYPE_DATA>
					(SIZE)
			; //

			std::normal_distribution
				<double>
				dist (0.0,Var)
			; //

			#ifdef PCG_RAND_HPP_INCLUDED
				pcg64_fast engine ;
			#else
				std::mt19937 engine ;
			#endif

			for ( size_t i = 0 ; i < SIZE ; i++ )
			{ in[i] = dist(engine) ; }

		}

	}

	namespace NN {

		using namespace Array ;

		template <
			size_t		I	,
			typename	TF
		> class AdamTrainer {
		public:

			using TYPE_DATA = TF ;

			static inline size_t constexpr
			SIZE() { return I ; }

			static inline TYPE_DATA constexpr
			BETA_1 () { return 0.9 ; }

			static inline TYPE_DATA constexpr
			BETA_2 () { return 0.99 ; }

			static inline TYPE_DATA constexpr
			EPS () { return 0.0000001 ; }

			using TYPE_ARRAY	=
				ND_ARRAY <
					SIZE()		,
					TYPE_DATA
				>
			;

		private:

			inline void
			EVAL_M () {

				for(size_t i=0;i<SIZE();i++){

					M[i] =
						(M[i]*BETA_1()) +
						((1.0-BETA_1())*DP[0][i])
					; //

				}

			}

			inline void
			EVAL_V () {

				for(size_t i=0;i<SIZE();i++){

					V[i] =
						(V[i]*BETA_2()) +
						((1.0-BETA_2())*DP_2[0][i])
					; //

				}

			}

		public:

			inline void
			UPDATE (
				TYPE_DATA const
					eta = 0.001
			) {

				EVAL_M () ;
				EVAL_V () ;

				for(size_t i=0;i<SIZE();i++){

					P[0][i] =
						P[0][i] - (
							( eta * M[i] ) / (
								EPS() +
								std::sqrt(V[i])
							)
						)
					; //

				}

			}

			inline void
			operator = (
				TYPE_DATA const
					a
			) {
				V = a ;
				M = a ;
			}

			AdamTrainer(){
				V = 0 ;
				M = 0 ;
			}

			~AdamTrainer(){}

		private:

			TYPE_ARRAY V, M ;

		public:

			TYPE_ARRAY			* P		;
			TYPE_ARRAY const	* DP	;
			TYPE_ARRAY const	* DP_2	;

		} ;

		template <
			size_t		S_2	,
			size_t		S_1	,
			typename	TF
		> class SoftLRU {
		public:

			static inline size_t constexpr
			SIZE_1 () { return S_1 ; }

			static inline size_t constexpr
			SIZE_2 () { return S_2 ; }

			inline static size_t constexpr
			SIZE_Y () { return SIZE_1 () ; }

			inline static size_t constexpr
			SIZE_B () { return SIZE_2 () ; }

			inline static size_t constexpr
			SIZE () {
				return
					SIZE_1() *
					SIZE_2()
				; //
			}

			using TYPE_DATA = TF ;

			using TYPE_SELF =
				SoftLRU <
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			;

			using TYPE_I1 =
				N2D_ARRAY <
					SIZE_2(),
					SIZE_1(),
					TYPE_DATA
				>
			;

			using TYPE_O1 =
				TYPE_I1
			; //

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

			inline void
			FORWARD () {

				auto const	& in	= I1->FLATTEN	() ;
				auto		& out	= O1.FLATTEN	() ;
				for (size_t i=0;i<TYPE_O1::SIZE();i++)
				{ out[i] = Activate(in[i]) ; }

			}

			inline void
			BACKWARD () {

				for(size_t i=0;i<SIZE();i++){

					DI1.FLATTEN()[i] =
						DO1->FLATTEN()[i] *
						D_Activate(I1->FLATTEN()[i])
					; //

				}

			}

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

				Layer.I1	= & O1			;
				DO1			= & Layer.DI1	;

			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {

				I1			= & Layer.O1	;
				Layer.DO1	= & DI1			;

			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {

				Layer.I1	= & O1			;
				DO1			= & Layer.O1	;

			}

			SoftLRU(){}
			~SoftLRU(){}

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

			TYPE_O1				O1	;
			TYPE_I1				DI1	;

			TYPE_O1	const	*	DO1	;
			TYPE_I1	const	*	I1	;

		} ;

		template <
			size_t S_3 = 1 , size_t S_2 = 1 ,
			size_t S_1 = 1 , typename TF = float
		> class LinearLayer {
		public:

			inline static size_t constexpr
			SIZE_1 () { return S_1 ; }
			//
			inline static size_t constexpr
			SIZE_2 () { return S_2 ; }
			//
			inline static size_t constexpr
			SIZE_3 () { return S_3 ; }

			inline static size_t constexpr
			SIZE_X () { return SIZE_1() ; }

			inline static size_t constexpr
			SIZE_Y () { return SIZE_2() ; }

			inline static size_t constexpr
			SIZE_B () { return SIZE_3() ; }

			using TYPE_DATA = TF ;

			using TYPE_SELF =
				LinearLayer <
					SIZE_3()	,
					SIZE_2()	,
					SIZE_1()	,
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

			// O
			using TYPE_O1 =
				N2D_ARRAY <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			// I
			using TYPE_I1 =
				N2D_ARRAY <
					SIZE_B()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			// W
			using TYPE_I2 =
				N2D_ARRAY <
					SIZE_Y()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			// B
			using TYPE_I3 =
				ND_ARRAY <
					SIZE_Y()	,
					TYPE_DATA
				>
			;

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

		private:

			inline void
			PREPARE () {
				DO1_2.GET_SQUARED	(DO1[0])	;
				I1_2.GET_SQUARED	(I1[0])		;
			}

			inline void
			EVAL_O1 () {
				( O1 = I3[0] ) +=
					I1[0] *
					I2->TRANSPOSE()
				; //
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

			inline void
			EVAL_DI3 () {

				DI3		= 0 ;
				DI3_2	= 0 ;

				for(size_t y=0;y<SIZE_Y();y=y+SKIP_SIZE)
				for(size_t b=0;b<SIZE_B();b++) {

					size_t const
						limit_y2 =
							CPPFileIO::mymin (
								SIZE_Y()	,
								SKIP_SIZE+y
							)
					; //

					for(size_t y2=y;y2<limit_y2;y2++) {

						DI3[y2] +=
							DO1[0][b][y2]
						; //

					}
				}

				for(size_t y=0;y<SIZE_Y();y=y+SKIP_SIZE)
				for(size_t b=0;b<SIZE_B();b++) {

					size_t const
						limit_y2 =
							CPPFileIO::mymin (
								SIZE_Y()	,
								SKIP_SIZE+y
							)
					; //

					for(size_t y2=y;y2<limit_y2;y2++) {

						DI3_2[y2] +=
							DO1_2[b][y2]
						; //

					}
				}

			}

			inline void
			EVAL_DI1 () {

				DI1 =
					DO1[0] *
					I2[0]
				; //

			}

			inline void
			EVAL_DI2 () {

				DI2 =
					DO1->TRANSPOSE() *
					I1[0]
				; //

				DI2_2 =
					DO1_2.TRANSPOSE() *
					I1_2
				; //

			}

		public:

			inline void
			FORWARD ()
			{ EVAL_O1_BLAS () ; }

			inline void
			BACKWARD () {

				// D_INPUT_1 = D_OUTPUT_1[0] * INPUT_2[0] ;

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

			template <typename T>
			inline void
			ATTACH_FORWARD (
				T & Layer
			) {

				Layer.I1	= & O1			;
				DO1			= & Layer.DI1	;

			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {

				I1			= & Layer.O1	;
				Layer.DO1	= & DI1			;

			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {

				Layer.I1	= & O1			;
				DO1			= & Layer.O1	;

			}

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

			TYPE_O1				O1		;
			TYPE_I1				DI1		;
			TYPE_I2				DI2		;
			TYPE_I3				DI3		;
			TYPE_I2				DI2_2	;
			TYPE_I3				DI3_2	;
			TYPE_O1				DO1_2	;
			TYPE_I1				I1_2	;

			TYPE_I1	const	*	I1		;
			TYPE_I2	const	*	I2		;
			TYPE_I3	const	*	I3		;
			TYPE_O1	const	*	DO1		;

		} ;

		template <
			size_t		S_2	, size_t S_1 ,
			typename	TF
		> class L2Teacher {
		public:

			using TYPE_DATA = TF ;

			static inline size_t constexpr
			SIZE_1 () {return S_1;}

			static inline size_t constexpr
			SIZE_2 () {return S_2;}

			static inline size_t constexpr
			SIZE_B () {return SIZE_2();}

			static inline size_t constexpr
			SIZE_Y () {return SIZE_1();}

			inline static size_t constexpr
			SIZE() {

				return
					SIZE_1()	*
					SIZE_2()
				; //

			}

			using TYPE_SELF =
				L2Teacher <
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			; //

			using TYPE_O1 =
				N2D_ARRAY <
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			; //

			using TYPE_I1 =
				TYPE_O1
			;

			using TYPE_I2 =
				TYPE_O1
			;

			inline void
			BACKWARD () {

				auto & out = O1.FLATTEN		() ;
				auto & in1 = I1->FLATTEN	() ;
				auto & in2 = I2->FLATTEN	() ;

				for(size_t i=0;i<SIZE();i++)
				{ out[i] = in1[i] - in2[i] ; }

			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {

				I1			= & Layer.O1	;
				Layer.DO1	= & O1			;

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

			TYPE_O1				O1	;
			TYPE_I1	const	*	I1	;
			TYPE_I2	const	*	I2	;

		} ;

	}

	namespace NN_GRAD {

		using namespace Array ;

		template <
			size_t B , size_t	Z ,
			size_t Y , typename	TF = float
		> class SoftLRU {
		public:

			static inline size_t constexpr
			SIZE_B(){return B;}

			static inline size_t constexpr
			SIZE_Z(){return Z;}

			static inline size_t constexpr
			SIZE_Y(){return Y;}

			inline static size_t constexpr
			SIZE () {

				return
					SIZE_Y() *
					SIZE_B()
				; //

			}

			using TYPE_DATA =
				TF
			; //

			using TYPE_O1 =
				N2D_ARRAY <
					SIZE_B(),
					SIZE_Y(),
					TYPE_DATA
				>
			; //

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

			using TYPE_I1 =
				TYPE_O1
			; //

			using TYPE_DO1 =
				TYPE_O1
			; //

			using TYPE_DI1 =
				TYPE_I1
			; //

			using TYPE_PARENT =
				NN::SoftLRU <
					SIZE_B(),
					SIZE_Y(),
					TYPE_DATA
				>
			; //

		private:

			inline void
			EVAL_O1 () {

				auto const	& in	= I1->FLATTEN	() ;
				auto		& out	= O1.FLATTEN	() ;

				for (size_t i=0;i<TYPE_O1::SIZE();i++) {

					out[i] =
						TYPE_PARENT::Activate
							(in[i])
					; //

				}

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

			inline void
			EVAL_DI1 () {

				/* Simple Part: */ {

					auto const	& in1 = DO1->FLATTEN	() ;
					auto const	& in2 = I1->FLATTEN	() ;
					auto		& out = DI1.FLATTEN	() ;

					for(size_t i=0;i<SIZE();i++){

						out[i] =
							in1[i] *
							TYPE_PARENT::D_Activate(in2[i])
						; //

					}

				}

				/* Complicated Part: */ {

					for(size_t b=0;b<SIZE_B();b++)
					for(size_t z=0;z<SIZE_Z();z++)
					for(size_t y=0;y<SIZE_Y();y++) {

						DI1[b][y] +=
							DOD1[0][b][z][y] *
							TYPE_PARENT::D2_Activate(I1[0][b][y]) *
							ID1[0][b][z][y]
						; //

					}
				}

			}

			inline void
			EVAL_DID1 () {

				DID1 = 0 ;

				for(size_t b=0;b<SIZE_B();b++)
				for(size_t z=0;z<SIZE_Z();z++)
				for(size_t y=0;y<SIZE_Y();y++) {

					DID1[b][z][y] +=

						DOD1[0][b][z][y] *

						TYPE_PARENT::D_Activate
							(I1[0][b][y])

					; //

				}

			}

		public:

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

			template <typename T>
			inline void
			ATTACH_FORWARD (
				T & Layer
			) {

				Layer.I1	= & O1			;
				Layer.ID1	= & OD1			;
				DO1			= & Layer.DI1	;
				DOD1		= & Layer.DID1	;

			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {

				I1			= & Layer.O1	;
				ID1			= & Layer.OD1	;
				Layer.DO1	= & DI1			;
				Layer.DOD1	= & DID1		;

			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {

				Layer.I1	= & O1			;
				Layer.ID1	= & OD1			;
				DO1			= & Layer.O1	;
				DOD1		= & Layer.OD1	;

			}

			inline bool
			IS_FIRST_LAYER () {

				return

					CPPFileIO::is_junked
						(ID1)

				; //

			}

			inline void
			MAKE_FIRST_LAYER () {

				ID1 =
					CPPFileIO::get_junk
						<TYPE_ID1>()
				; //

			}

			SoftLRU()
			{MAKE_FIRST_LAYER();}

			~SoftLRU(){}

			TYPE_O1					O1		;
			TYPE_OD1				OD1		;
			TYPE_DI1				DI1		;
			TYPE_DID1				DID1	;

			TYPE_I1		const	*	I1		;
			TYPE_ID1	const	*	ID1		;
			TYPE_DO1	const	*	DO1		;
			TYPE_DOD1	const	*	DOD1	;

		} ;

		template <
			size_t B , size_t Z ,
			size_t Y , size_t X ,
			typename TF = float
		> class LinearLayer {
		public:

			static inline size_t constexpr
			SIZE_B() {return B;}

			static inline size_t constexpr
			SIZE_Z() {return Z;}

			static inline size_t constexpr
			SIZE_Y() {return Y;}

			static inline size_t constexpr
			SIZE_X() {return X;}

			using TYPE_DATA = TF ;

			using TYPE_OD1 =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_DOD1 =
				TYPE_OD1
			;

			using TYPE_O1 =
				N2D_ARRAY <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_DO1 =
				TYPE_O1
			;

			using TYPE_ID1 =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			using TYPE_DID1 =
				TYPE_ID1
			;

			using TYPE_I1 =
				N2D_ARRAY <
					SIZE_B()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			using TYPE_DI1 =
				TYPE_I1
			;

			using TYPE_I2 =
				N2D_ARRAY <
					SIZE_Y()	,
					SIZE_X()	,
					TYPE_DATA
				>
			;

			using TYPE_DI2 =
				TYPE_I2
			;

			using TYPE_I3 =
				ND_ARRAY <
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_DI3 =
				TYPE_I3
			;

		private:

			inline void
			PREPARE () {

				DO1_2.GET_SQUARED
					(DO1[0])
				; //

				I1_2.GET_SQUARED
					(I1[0])
				; //

			}

			inline void
			EVAL_O1 () {

				( O1 = I3[0] ) +=
					I1[0] *
					I2->TRANSPOSE()
				; //

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

			inline void
			EVAL_OD1 () {

				if (SIZE_Z()==SIZE_X()) {

					if (IS_FIRST_LAYER()) {

						using TYPE_WEIGHT =
							N2D_ARRAY <
								SIZE_Y () ,
								SIZE_Z () ,
								TYPE_DATA
							>
						; //

						auto tmp =
							reinterpret_cast
								<TYPE_WEIGHT const *>
									(I2)
						; //

						OD1[0] =
							tmp->TRANSPOSE()
						; //

						for (size_t b=1;b<SIZE_B();b++)
						{ OD1[b] = OD1[0] ; }

					}

				} else {

					for (size_t b=0;b<SIZE_B();b++) {

						OD1[b] =
							ID1[0][b] *
							I2->TRANSPOSE()
						; //

					}

				}

			}

			inline void
			EVAL_DI1 () {

				DI1 =
					DO1[0] *
					I2[0]
				;

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

			TYPE_DI2 DI2_TMP ;

			inline void
			EVAL_DI2 () {

				DI2		=
					DO1->TRANSPOSE()	*
					I1[0]
				; //

				DI2_2	=
					DO1_2.TRANSPOSE()	*
					I1_2
				; //

				for(size_t b=0;b<SIZE_B();b++) {

					DI2_TMP =
						DOD1[0][b].TRANSPOSE()	*
						ID1[0][b]
					; //

					DI2.FLATTEN() +=
						DI2_TMP.FLATTEN()
					; //

					DI2_2.ADD_SQUARED
						(DI2_TMP)
					; //

				}

			}

			inline void
			EVAL_DI3 () {

				DI3		= 0 ;
				DI3_2	= 0 ;

				for(size_t y=0;y<SIZE_Y();y=y+SKIP_SIZE)
				for(size_t b=0;b<SIZE_B();b++)
				for(size_t y2=y;(y2<y+SKIP_SIZE)&&(y2<SIZE_Y());y2++) {
					DI3_2[y2]	+= DO1[0][b][y2] * DO1[0][b][y2]	;
				}

				for(size_t y=0;y<SIZE_Y();y=y+SKIP_SIZE)
				for(size_t b=0;b<SIZE_B();b++)
				for(size_t y2=y;(y2<y+SKIP_SIZE)&&(y2<SIZE_Y());y2++) {
					DI3[y2]		+= DO1[0][b][y2]					;
				}
			}

		public:

			inline void
			FORWARD () {
				EVAL_O1_BLAS	() ;
				EVAL_OD1		() ;
			}

			inline void
			BACKWARD () {
				PREPARE		() ;
				EVAL_DID1	() ;
				EVAL_DI1	() ;
				EVAL_DI2	() ;
				EVAL_DI3	() ;
			}

			template <typename T>
			inline void
			ATTACH_FORWARD (
				T & Layer
			) {
				Layer.I1	= & O1			;
				Layer.ID1	= & OD1			;
				DO1			= & Layer.DI1	;
				DOD1		= & Layer.DID1	;
			}

			template <typename T>
			inline void
			ATTACH_BACKWARD (
				T & Layer
			) {
				I1			= & Layer.O1	;
				ID1			= & Layer.OD1	;
				Layer.DO1	= & DI1			;
				Layer.DOD1	= & DID1		;
			}

			template <typename T>
			inline void
			ATTACH_TEACHER (
				T & Layer
			) {
				Layer.I1	= & O1			;
				Layer.ID1	= & OD1			;
				DO1			= & Layer.O1	;
				DOD1		= & Layer.OD1	;
			}

			inline bool
			IS_FIRST_LAYER () {

				return

					CPPFileIO::is_junked
						(ID1)

				; //

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

		private:

			TYPE_DO1				DO1_2	;
			TYPE_I1					I1_2	;

		public:

			TYPE_O1					O1		;
			TYPE_OD1				OD1		;
			TYPE_DI1				DI1		;
			TYPE_DID1				DID1	;
			TYPE_DI2				DI2		;
			TYPE_DI3				DI3		;
			TYPE_DI2				DI2_2	;
			TYPE_DI3				DI3_2	;

			TYPE_DO1	const	*	DO1		;
			TYPE_DOD1	const	*	DOD1	;
			TYPE_I1		const	*	I1		;
			TYPE_ID1	const	*	ID1		;
			TYPE_I2		const	*	I2		;
			TYPE_I3		const	*	I3		;

		} ;

		template <
			size_t B , size_t	Z  ,
			size_t Y , typename	TF
		> class L2Teacher {
		public:

			static inline size_t constexpr
			SIZE_B(){return B;}

			static inline size_t constexpr
			SIZE_Z(){return Z;}

			static inline size_t constexpr
			SIZE_Y(){return Y;}

			using TYPE_DATA = TF ;

			using TYPE_2D_ARRAY =
				N2D_ARRAY <
					SIZE_B()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

			using TYPE_3D_ARRAY =
				N3D_ARRAY <
					SIZE_B()	,
					SIZE_Z()	,
					SIZE_Y()	,
					TYPE_DATA
				>
			;

		private:

			inline void
			EVAL_O1 () {

				auto const & in1 =
					I1->FLATTEN()
				; //

				auto const & in2 =
					I2->FLATTEN()
				; //

				auto & out =
					O1.FLATTEN()
				; //

				for(size_t i=0;i<TYPE_2D_ARRAY::SIZE();i++)
				{ out[i] = in1[i] - in2[i] ; }

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

				for(size_t i=0;i<TYPE_3D_ARRAY::SIZE();i++)
				{ out[i] = in1[i] - in2[i] ; }

			}

		public:

			inline void
			BACKWARD () {
				EVAL_O1		() ;
				EVAL_OD1	() ;
			}

			L2Teacher(){}
			~L2Teacher(){}

			TYPE_2D_ARRAY				O1	;
			TYPE_3D_ARRAY				OD1	;

			TYPE_2D_ARRAY	const	*	I1	;
			TYPE_2D_ARRAY	const	*	I2	;
			TYPE_3D_ARRAY	const	*	ID1	;
			TYPE_3D_ARRAY	const	*	ID2	;

		} ;

	}

}

namespace Tensors {

	namespace Threaded_NN {

		using namespace NN ;

		template <
			size_t		S_3  ,
			size_t		S_2  ,
			size_t		S_1  ,
			typename	TF
		> class Sample_Pars {
		public:

			static inline size_t constexpr
			SIZE_3 () {return S_3;}

			static inline size_t constexpr
			SIZE_2 () {return S_2;}

			static inline size_t constexpr
			SIZE_1 () {return S_1;}

			static inline size_t constexpr
			SIZE () {return SIZE_1()*SIZE_2();}

			static inline size_t constexpr
			SIZE_X () {return SIZE_1();}

			static inline size_t constexpr
			SIZE_Y () {return SIZE_2();}

			static inline size_t constexpr
			SIZE_T () {return SIZE_3();}

			using TYPE_DATA =
				TF
			; //

			using TYPE_SELF =
				Sample_Pars <
					SIZE_3()	,
					SIZE_2()	,
					SIZE_1()	,
					TYPE_DATA
				>
			; //

			using TYPE_WEIGHT_TRAINER =
				AdamTrainer <
					SIZE()		,
					TYPE_DATA
				>
			; //

			using TYPE_BIAS_TRAINER =
				AdamTrainer <
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

			using TYPE_WEIGHT =
				N2D_ARRAY <
					SIZE_Y()	,
					SIZE_X()	,
					TYPE_DATA
				>
			; //

			using TYPE_BIAS =
				ND_ARRAY <
					SIZE_Y()	,
					TYPE_DATA
				>
			; //

		private:

			inline void UPDATE (

				size_t const
					t			,

				TF const
					eta = 0.001

			) {
				L_W.DP		= &(DW[t][0]->FLATTEN()) ;
				L_W.DP_2	= &(DW[t][1]->FLATTEN()) ;
				L_B.DP		= &(DB[t][0]->FLATTEN()) ;
				L_B.DP_2	= &(DB[t][1]->FLATTEN()) ;
				L_W.UPDATE (eta) ;
				L_B.UPDATE (eta) ;
			}

		public:

			inline void
			UPDATE (
				TF const
					eta = 0.001
			) {
				for (size_t t=0;t<SIZE_T();t++)
				{ UPDATE (t,eta) ; }
			}

			template <typename T>
			inline void
			TO_LAYER (

				size_t const
					t			,

				T & Layer

			) {
				Layer.I2	= & W			;
				Layer.I3	= & B			;
				DW[t][0]	= & Layer.DI2	;
				DW[t][1]	= & Layer.DI2_2	;
				DB[t][0]	= & Layer.DI3	;
				DB[t][1]	= & Layer.DI3_2	;
			}

			template <typename T>
			static inline TYPE_SELF
			GET_PARS (
				T & in
			) {

				TYPE_SELF
					ret
				; /* Initialize the layer: */ {
					for(size_t t=0;t<SIZE_T();t++)
					{ ret.TO_LAYER (t,in[t]) ; }
				}

				return
					ret
				;

			}

			Sample_Pars(){
				FUNCTIONS::RANDOMIZE (W) ;
				FUNCTIONS::RANDOMIZE (B) ;
				for(size_t t=0;t<SIZE_T();t++){
					L_W.P = & (W.FLATTEN()) ;
					L_B.P = & (B.FLATTEN()) ;
				}
			}

			~Sample_Pars(){}

			TYPE_WEIGHT_TRAINER	L_W	;
			TYPE_BIAS_TRAINER	L_B	;
			TYPE_WEIGHT			W	;
			TYPE_BIAS			B	;

			N2D_ARRAY < SIZE_T() , 2 , TYPE_WEIGHT	const * >	DW ;
			N2D_ARRAY < SIZE_T() , 2 , TYPE_BIAS	const * >	DB ;

		} ;
	}

	namespace SAMPLE_NN {

		using namespace Array		;
		using namespace FUNCTIONS	;
		using namespace NN			;

		template <
			size_t		Y  ,
			size_t		X  ,
			typename	TF
		> class Sample_Pars {
		public:

			inline void UPDATE (
				TF const
					eta = 0.001
			) {
				L_W.UPDATE(eta);
				L_B.UPDATE(eta);
			}

			template <typename T>
			inline void
			TO_LAYER (
				T & Layer
			) {
				Layer.I2 = & W ;
				Layer.I3 = & B ;

				L_W.DP = & Layer.DI2.FLATTEN() ;
				L_B.DP = & Layer.DI3 ;

				L_W.DP_2 = & Layer.DI2_2.FLATTEN() ;
				L_B.DP_2 = & Layer.DI3_2 ;
			}

			Sample_Pars(){
				L_W.P = &(W.FLATTEN()) ;
				L_B.P = &(B.FLATTEN()) ;
				RANDOMIZE (W) ;
				RANDOMIZE (B) ;
			}

			~Sample_Pars(){}

			AdamTrainer	<Y*X,TF>	L_W	;
			AdamTrainer	<Y,TF>		L_B	;
			N2D_ARRAY	<Y,X,TF>	W	;
			ND_ARRAY	<Y,TF>		B	;

		} ;
	}

}

namespace X2_2 {

	inline size_t constexpr
	SIZE_T () {return 4;}

	inline size_t constexpr
	SIZE_B () {return 10000;}

	using TYPE_DATA = float ;

	size_t constexpr sizes[3] = {1,30,1} ;

	class Net {
	public:

		using TYPE_L1 =
			Tensors::NN::LinearLayer <
				SIZE_B()	,
				sizes[1]	,
				sizes[0]	,
				TYPE_DATA
			>
		; //

		using TYPE_L2 =
			Tensors::NN::SoftLRU <
				SIZE_B()	,
				sizes[1]	,
				TYPE_DATA
			>
		; //

		using TYPE_L3 =
			Tensors::NN::LinearLayer <
				SIZE_B()	,
				sizes[2]	,
				sizes[1]	,
				TYPE_DATA
			>
		; //

		using TYPE_L4 =
			Tensors::NN::L2Teacher <
				SIZE_B()	,
				sizes[2]	,
				TYPE_DATA
			>
		; //

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY <
				SIZE_B()	,
				sizes[0]	,
				TYPE_DATA
			>
		; //

		using TYPE_OUTPUT =
			Tensors::Array::N2D_ARRAY <
				SIZE_B()	,
				sizes[2]	,
				TYPE_DATA
			>
		; //

		inline TYPE_OUTPUT const &
		FORWARD (
			TYPE_INPUT const &
				in
		) {
			L1.I1 = & in ;
			L1.FORWARD() ;
			L2.FORWARD() ;
			L3.FORWARD() ;
			return L3.O1 ;
		}

		inline void
		BACKWARD (
			TYPE_OUTPUT const &
				in
		) {
			L4.I2 = & in ;
			L4.BACKWARD();
			L3.BACKWARD();
			L2.BACKWARD();
			L1.BACKWARD();
			printf("Error = %e\n",L4.O1.L2_NORM());
		}

		template <
			typename	T1	,
			typename	T2
		> inline void
		ATTACH_PARS (
			size_t	const 		t	,
			T1				&	in1	,
			T2				&	in2
		) {
			in1.TO_LAYER(t,L1) ;
			in2.TO_LAYER(t,L3) ;
		}

		Net() :
		L2(TYPE_L2::FROM_LAYER(L1)) ,
		L3(TYPE_L3::FROM_LAYER(L2)) ,
		L4(TYPE_L4::FROM_LAYER(L3))
		{}

		~Net(){}

		TYPE_L1 L1 ;
		TYPE_L2 L2 ;
		TYPE_L3 L3 ;
		TYPE_L4 L4 ;

	} ;

	class NetBig {
	public:

		using TYPE_P1 =
			Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),sizes[1],sizes[0],TYPE_DATA>
		; //

		using TYPE_P2 =
			Tensors::Threaded_NN::Sample_Pars
			<SIZE_T(),sizes[2],sizes[1],TYPE_DATA>
		; //

		using TYPE_INPUT =
			Tensors::Array::N3D_ARRAY
			<SIZE_T(),SIZE_B(),sizes[0],TYPE_DATA>
		; //

		using TYPE_OUTPUT =
			Tensors::Array::N3D_ARRAY
			<SIZE_T(),SIZE_B(),sizes[2],TYPE_DATA>
		; //

		inline TYPE_DATA const
		GET_VAL (size_t const n) {

			size_t		constexpr N		= SIZE_T() * SIZE_B()					;
			TYPE_DATA	constexpr x2	= 10									;
			TYPE_DATA	constexpr x1	= -10									;
			TYPE_DATA	constexpr dx	= (x2-x1) / static_cast<TYPE_DATA>(N)	;

			return
				x1 +
				(n*dx)
			; //

		}

		inline void
		SETUP_DATA () {

			if (false) {
				for(size_t t=0;t<SIZE_T();t++){
					for(size_t b=0;b<SIZE_B();b++){
						size_t const N = (SIZE_T()*b) + t ;
						INPUT[t][b][0] = GET_VAL(N) ;
					}
				}
			} else {
				auto & tmp = INPUT.FLATTEN() ;
				for(size_t i=0;i<tmp.SIZE();i++){
					tmp[i] = GET_VAL(i) ;
				}
			}

			OUTPUT.GET_SQUARED(INPUT);
		}

		inline void
		FORWARD (
			size_t const t
		) {
			mainnet[t]
				.FORWARD (
					INPUT[t]
				)
			;
		}

		inline void
		BACKWARD (
			size_t const t
		) {
			//printf("thread = %ld: ",t);
			mainnet[t]
				.BACKWARD (
					OUTPUT[t]
				)
			;
		}

		inline void UPDATE (
			TYPE_DATA const eta =
				0.01
		) {
			P1.UPDATE (eta) ;
			P2.UPDATE (eta) ;
		}

		inline void TRAIN (
			size_t t
		) {
			FORWARD		(t) ;
			BACKWARD	(t) ;
		}

		inline void TRAIN (
			TYPE_DATA eta =
				0.01
		) {

			#pragma omp parallel for
			for(size_t i=0;i<SIZE_T();i++)
			{ TRAIN (i) ; }

			UPDATE (eta) ;

		}

		inline void
		EVALUATE () {

			for(size_t t=0;t<INPUT.SIZE_3();t++){

				auto & out =
					mainnet[0]
						.FORWARD(INPUT[t])
						.FLATTEN()
				;

				for(size_t i=0;i<out.SIZE();i++){
					printf (
						"DIFF: %e %e %e\n"				,
						INPUT[t].FLATTEN()[i] , out[i]	,
						OUTPUT[t].FLATTEN()[i] - out[i]
					) ;
				}

			}

		}

		NetBig(){
			SETUP_DATA();
			for(size_t t=0;t<SIZE_T();t++)
			{ mainnet[t].ATTACH_PARS(t,P1,P2) ; }
		}

		~NetBig(){}

		TYPE_P1		P1		;
		TYPE_P2		P2		;
		TYPE_INPUT	INPUT	;
		TYPE_OUTPUT	OUTPUT	;

		Net mainnet[SIZE_T()] ;

	} ;

}

//#ifdef asd
namespace Log_X {

	inline size_t constexpr
	SIZE_T () {return 4;}

	inline size_t constexpr
	SIZE_B () {return 500;}

	using TYPE_DATA = float ;

	class Pars {
	public:

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T (),30,1,TYPE_DATA>
			P1
		; //

		Tensors::Threaded_NN::Sample_Pars
			<SIZE_T (),1,30,TYPE_DATA>
			P3
		; //

		inline void
		UPDATE (
			TYPE_DATA const
				eta = 0.001
		) {
			return ;
			P1.UPDATE(eta);
			P3.UPDATE(eta);
		}

		Pars(){}
		~Pars(){}
	} ;

	class Net {
	public:

		using TYPE_INPUT =
			Tensors::Array::N2D_ARRAY
				<SIZE_B(),1,TYPE_DATA>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		inline void FORWARD (
			TYPE_INPUT const &
				in
		) {
			L1.I1 = & in ;
			L1.FORWARD();
			L2.FORWARD();
			L3.FORWARD();
		}

		inline void BACKWARD (
			TYPE_OUTPUT const &
				out
		) {
			L4.I2 = & out	;
			L4.BACKWARD ()	;
			L3.BACKWARD ()	;
			L2.BACKWARD ()	;
			L1.BACKWARD ()	;
			printf (
				"ERROR = %e\n"	,
				L4.O1.L2_NORM()
			) ; //
		}

		inline void
		TO_LAYER (
			size_t	const	t		,
			Pars	&		InPars
		) {
			InPars.P1.TO_LAYER(t,L1);
			InPars.P3.TO_LAYER(t,L3);
		}

		inline void ATTACH_ALL () {
			L1.ATTACH_FORWARD(L2);
			L2.ATTACH_FORWARD(L3);
			L3.ATTACH_TEACHER(L4);
			L4.ID2 = L4.ID1 ;
			L1.MAKE_FIRST_LAYER();
		}

		Net () {ATTACH_ALL();}

		~Net(){}

		Tensors::NN_GRAD::LinearLayer
			<SIZE_B(),1,30,1,TYPE_DATA>
				L1
		; //

		Tensors::NN_GRAD::SoftLRU
			<SIZE_B(),1,30,TYPE_DATA>
				L2
		; //

		Tensors::NN_GRAD::LinearLayer
			<SIZE_B(),1,1,30,TYPE_DATA>
				L3
		; //

		Tensors::NN_GRAD::L2Teacher
			<SIZE_B(),1,1,TYPE_DATA>
				L4
		; //

	} ;

	class NetAll {
	public:

		using TYPE_INPUT =
			Tensors::Array::N3D_ARRAY <
				SIZE_T()	,
				SIZE_B()	,
				1			,
				TYPE_DATA
			>
		; //

		using TYPE_OUTPUT =
			TYPE_INPUT
		; //

		Net		net[SIZE_T()]	;
		Pars	p				;

		inline void TRAIN (
			TYPE_INPUT const &
				in ,

			TYPE_INPUT const &
				out
		) {

			#pragma omp parallel for
			for(size_t i=0;i<4;i++){
				net[i].FORWARD(in[i]);
				net[i].BACKWARD(out[i]);
			}

			p.UPDATE(0.0);

		}

		NetAll(){
			net[0].TO_LAYER(0,p);
			net[1].TO_LAYER(1,p);
			net[2].TO_LAYER(2,p);
			net[3].TO_LAYER(3,p);
		}

		~NetAll(){}

	} ;

	class DataTrainer {
	public:
		using TYPE_MATRIX =
			Tensors::Array::N4D_ARRAY <
				100 , SIZE_T() ,
				SIZE_B() , 1 ,
				TYPE_DATA
			>
		; //

		TYPE_MATRIX DATA_IN		;
		TYPE_MATRIX DATA_OUT	;

		inline void
		SetUp () {

			TYPE_DATA constexpr x2 = 10.0 ;
			TYPE_DATA constexpr x1 = -5.0 ;

			TYPE_DATA constexpr dx =
				( x2 - x1 ) / (
					static_cast<TYPE_DATA>
						(TYPE_MATRIX::SIZE())
				)
			; //

			auto & tmp =
				DATA_IN.FLATTEN()
			; //

			for(size_t i=0;i<tmp.SIZE();i++) {
				tmp[i] =
					x1 + (
						static_cast<TYPE_DATA>(i) *
						dx
					)
				; //
			}

			DATA_OUT.FLATTEN().GET_SQUARED
				(DATA_IN.FLATTEN())
			; //

		}

		NetAll mainnet ;

		inline void TRAIN () {
			for(size_t i=0;i<1000;i++){
				mainnet.TRAIN (
					DATA_IN[i]	,
					DATA_OUT[i]
				) ; //
			}
		}

		DataTrainer(){SetUp();}

		~DataTrainer(){}

	} ;

}
//#endif

int main () {
	if(true){
		auto * slave = new X2_2::NetBig ;
		for(size_t i=0;i<1000;i++){
			slave->TRAIN ( static_cast<float>(0.001) ) ;
		}
		delete slave ;
	}

	if(false){
		Log_X::DataTrainer * slave =
			new Log_X::DataTrainer
		; //

		for(size_t e=0;e<1000;e++)
		{slave->TRAIN();}

		delete slave ;
	}

	return 0 ;
}
