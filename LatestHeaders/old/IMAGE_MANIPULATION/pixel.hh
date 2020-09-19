#define _MACRO_CLASS_NAME_ ColorPixel

	class _MACRO_CLASS_NAME_ {

		////////////////////////
		// DEFINITIONS BEGIN: //
		////////////////////////

	public:

		using TYPE_PIXEL_DATA =
			Tensors::Array::ND_ARRAY
				<3,TYPE_BYTE>
		; //

		using TYPE_SELF =
			_MACRO_CLASS_NAME_
		; //

		//////////////////////
		// DEFINITIONS END. //
		//////////////////////

		/////////////////
		// DATA BEGIN: //
		/////////////////

	private:

		TYPE_PIXEL_DATA DATA ;

		///////////////
		// DATA END. //
		///////////////

		/////////////////////////////
		// Important Values BEGIN: //
		/////////////////////////////

	public:

		static inline size_t constexpr
		MAX_INTENSITY () {
			return
				255 * 255 * 3
			; //
		}

		static inline TYPE_DATA constexpr
		MAX_INTENSITY_D () {
			return
				static_cast <TYPE_DATA> (
					MAX_INTENSITY ()
				)
			; //
		}

		static inline TYPE_DATA constexpr
		SQRT_MAX_INTENSITY () {

			// std::sqrt(MAX_INTENSITY_D())
			return
				4.416729559300637220e+02
			; //

		}

		static inline TYPE_DATA const
		CalcD (
			TYPE_DATA const _R ,
			TYPE_DATA const _G ,
			TYPE_DATA const _B
		) {
			TYPE_DATA const _D =
				std::sqrt (
					MAX_INTENSITY_D () - (
						(_R*_R) +
						(_G*_G) +
						(_B*_B)
					)
				)
			; //
			return _D ;
		}

		///////////////////////////
		// Important Values END. //
		///////////////////////////

		/////////////////////////////
		// ACCESS INTERFACE BEGIN: //
		/////////////////////////////

	private:

		inline TYPE_BYTE &
		GET_R () {
			return
				DATA[2]
			; //
		}

		inline TYPE_BYTE &
		GET_G () {
			return
				DATA[1]
			; //
		}

		inline TYPE_BYTE &
		GET_B () {
			return
				DATA[0]
			; //
		}

		inline TYPE_BYTE const
		GET_R () const {
			return
				DATA[2]
			; //
		}

		inline TYPE_BYTE const
		GET_G () const {
			return
				DATA[1]
			; //
		}

		inline TYPE_BYTE const
		GET_B () const {
			return
				DATA[0]
			; //
		}

	public:

		inline TYPE_BYTE const
		R () const {
			return
				GET_R()
			; //
		}

		inline TYPE_BYTE const
		G () const {
			return
				GET_G()
			; //
		}

		inline TYPE_BYTE const
		B () const {
			return
				GET_B()
			; //
		}

		inline TYPE_DATA const
		R_D () const {
			return
				static_cast<TYPE_DATA>
					(R())
			; //
		}

		inline TYPE_DATA const
		G_D () const {
			return
				static_cast<TYPE_DATA>
					(G())
			; //
		}

		inline TYPE_DATA const
		B_D () const {
			return
				static_cast<TYPE_DATA>
					(B())
			; //
		}

		inline TYPE_DATA const
		D_D () const {
			return
				CalcD (
					/* double const _R = */ R_D() ,
					/* double const _G = */ G_D() ,
					/* double const _B = */ B_D()
				)
			; //
		}

	private:

		inline void
		SetVals (
			TYPE_BYTE const _R ,
			TYPE_BYTE const _G ,
			TYPE_BYTE const _B
		) {
			GET_R() = _R ;
			GET_G() = _G ;
			GET_B() = _B ;
		}

	public:

		inline void
		operator () (
			TYPE_BYTE const _R ,
			TYPE_BYTE const _G ,
			TYPE_BYTE const _B
		) {
			SetVals (_R,_G,_B) ;
		}

		///////////////////////////
		// ACCESS INTERFACE END. //
		///////////////////////////

		////////////////////////////////////////////
		// IMPORTANT CALCULATION FUNCTIONS BEGIN: //
		////////////////////////////////////////////

	public:

		inline TYPE_DATA const
		ColorNorm2 () const {
			return
				(R_D()*R_D()) +
				(G_D()*G_D()) +
				(B_D()*B_D())
			; //
		}

		inline TYPE_DATA const
		ColorNorm () const  {
			return
				std::sqrt (
					ColorNorm2()
				)
			; //
		}

		inline TYPE_DATA
		ColorDot (
			TYPE_SELF const &
				other
		) const {

			TYPE_DATA const Norm1 =
				ColorNorm2()
			; //

			TYPE_DATA const Norm2 =
				other.ColorNorm2()
			; //

			TYPE_DATA const Norm =
				Norm1 * Norm2
			; //

			if (Norm>0.0001) {

				TYPE_DATA const N =
					( R_D() * other.R_D() ) +
					( G_D() * other.G_D() ) +
					( B_D() * other.B_D() )
				; //

				TYPE_DATA const D =
					std::sqrt(Norm)
				; //

				return N / D ;

			} else {

				return 0 ;

			}

		}

		inline TYPE_DATA
		FullDot (
			TYPE_SELF const &
				other
		) const {

			return
				(
					( R_D() * other.R_D() ) +
					( G_D() * other.G_D() ) +
					( B_D() * other.B_D() ) +
					( D_D() * other.D_D() )
				) / MAX_INTENSITY ()
			; //

		}

		inline double const
		operator * (
			TYPE_SELF const & other
		) const {
			return
				FullDot(other)
			; //
		}

		inline TYPE_SELF
		operator / (
			TYPE_BYTE const
				other
		) {

			TYPE_SELF
				ret (
					R() / other ,
					G() / other ,
					B() / other
				)
			; //

			return
				ret
			; //

		}

		//////////////////////////////////////////
		// IMPORTANT CALCULATION FUNCTIONS END. //
		//////////////////////////////////////////

		///////////////////////////////////////
		// CONSTRUCTOR AND DESTRUCTOR BEGIN: //
		///////////////////////////////////////

	public:

		_MACRO_CLASS_NAME_ ()
		{ DATA = 0 ; }

		_MACRO_CLASS_NAME_ (
			TYPE_BYTE const _R ,
			TYPE_BYTE const _G ,
			TYPE_BYTE const _B
		) {
			SetVals (_R,_G,_B) ;
		}

		~_MACRO_CLASS_NAME_ () {}

		/////////////////////////////////////
		// CONSTRUCTOR AND DESTRUCTOR END. //
		/////////////////////////////////////

		/////////////////////////////////////
		// FOR DEBUGGING CONVINENCE BEGIN: //
		/////////////////////////////////////

		inline void
		SHOW () const {
            printf (
				"(%e,%e,%e,%e)\n"	,
				R_D() , G_D()		,
				B_D() , D_D()
			) ;
        }

        ///////////////////////////////////
        // FOR DEBUGGING CONVINENCE END. //
        ///////////////////////////////////

	} ;

	using TYPE_PIXEL =
		_MACRO_CLASS_NAME_
	; //

#undef _MACRO_CLASS_NAME_
