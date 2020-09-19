#ifndef _HEADER_GUARD_NewNeuralNet_TRAINER_
#define _HEADER_GUARD_NewNeuralNet_TRAINER_

#include "../StaticArray.hh"
#include "./Headers_start.hh"

	namespace NN {

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
			EPS () { return 0.00001 ; }

			using TYPE_ARRAY	=
				StaticArray::ND_ARRAY <
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

	}

#include "./Headers_end.hh"
#endif
