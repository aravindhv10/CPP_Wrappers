	namespace Threaded_NN {

		using NN::AdamTrainer ;

		template <
			size_t		T  ,
			size_t		Y  ,
			size_t		X  ,
			typename	TF
		> class Sample_Pars {
		public:

			using TYPE_DATA =
				TF
			; //

			static inline size_t constexpr
			SIZE_X () {return X;}

			static inline size_t constexpr
			SIZE_Y () {return Y;}

			static inline size_t constexpr
			SIZE_T () {return T;}

			static inline size_t constexpr
			SIZE_3 () {return SIZE_T();}

			static inline size_t constexpr
			SIZE_2 () {return SIZE_Y();}

			static inline size_t constexpr
			SIZE_1 () {return SIZE_X();}

			static inline size_t constexpr
			SIZE () {
				return
					SIZE_X() *
					SIZE_Y()
				; //
			}

			using TYPE_SELF =
				Sample_Pars <
					SIZE_T()	,
					SIZE_Y()	,
					SIZE_X()	,
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
				for (
					size_t t=0;
					t<SIZE_T();
					t++
				) {
					UPDATE (t,eta) ;
				}
			}

			template <typename LT>
			inline void
			TO_LAYER (

				size_t const
					t			,

				LT & Layer

			) {
				Layer.I2 = & W				;
				Layer.I3 = & B				;
				DW[t][0] = & Layer.DI2		;
				DW[t][1] = & Layer.DI2_2	;
				DB[t][0] = & Layer.DI3		;
				DB[t][1] = & Layer.DI3_2	;
			}

			template <typename LT>
			static inline TYPE_SELF
			GET_PARS (
				LT & in
			) {

				TYPE_SELF
					ret
				; /* Initialize the layer: */ {
					for(
						size_t t = 0	;
						t < SIZE_T()	;
						t++
					) {
						ret.TO_LAYER
							(t,in[t])
						; //
					}
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

			N2D_ARRAY <
				SIZE_T()			,
				2					,
				TYPE_WEIGHT const	*
			>	DW					;

			N2D_ARRAY <
				SIZE_T()		,
				2				,
				TYPE_BIAS const	*
			>	DB				;

		} ;
	}
