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
