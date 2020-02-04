#define _MACRO_CLASS_NAME_ MyPoint

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
	; //

	using TYPE_POINTS =
		std::vector <TYPE_SELF>
	; //

	using TYPE_CV_POINT =
		Tensors::Array::ND_ARRAY
			<2,TYPE_BYTE>
	; //

	//////////////////////
	// DEFINITIONS END. //
	//////////////////////

	/////////////////
	// DATA BEGIN: //
	/////////////////

public:

	TYPE_DATA const  Y ;
	TYPE_DATA const  X ;

	///////////////
	// DATA END. //
	///////////////

	///////////////////////////////////////
	// CONSTRUCTOR AND DESTRUCTOR BEGIN: //
	///////////////////////////////////////

public:

	_MACRO_CLASS_NAME_(
		TYPE_DATA const _Y ,
		TYPE_DATA const _X
	) : Y(_Y) , X(_X) {}

	~_MACRO_CLASS_NAME_(){}

	/////////////////////////////////////
	// CONSTRUCTOR AND DESTRUCTOR END. //
	/////////////////////////////////////

	/////////////////////////////////
	// IMPORTANT INTERFACES BEGIN: //
	/////////////////////////////////

	inline TYPE_CV_POINT
	AS_CV_POINT () const {

		TYPE_CV_POINT
			ret
		; /* Prepare the points: */ {
			ret[0] =
				static_cast<TYPE_BYTE>
					(Y)
			; //
			ret[1] =
				static_cast<TYPE_BYTE>
					(X)
			; //
		}

		return
			ret
		; //

	}

	///////////////////////////////
	// IMPORTANT INTERFACES END. //
	///////////////////////////////

	////////////////////////////////////
	// MATHEMATICAL OPERATIONS BEGIN: //
	////////////////////////////////////

	inline TYPE_SELF
	operator + (
		TYPE_SELF const &
			other
	) const {

		TYPE_SELF
			ret (
				Y + other.Y ,
				X + other.X
			)
		; //
		return ret ;

	}

	inline TYPE_SELF
	operator - (
		TYPE_SELF const &
			other
	) const {

		TYPE_SELF
			ret (
				Y - other.Y ,
				X - other.X
			)
		; //
		return ret ;

	}

	inline TYPE_SELF
	operator * (
		TYPE_DATA const
			other
	) const {

		TYPE_SELF
			ret (
				Y * other ,
				X * other
			)
		; //
		return ret ;

	}

	inline TYPE_SELF
	operator / (
		TYPE_DATA const
			other
	) const {

		TYPE_SELF
			ret (
				Y / other ,
				X / other
			)
		; //
		return ret ;

	}

	inline TYPE_DATA
	operator * (
		TYPE_SELF const &
			other
	) const {

		TYPE_DATA
			ret =
				( X * other.X ) +
				( Y * other.Y )
		; //
		return ret ;

	}

	inline TYPE_DATA
	NORM2 () const {
		return
			(X*X) +
			(Y*Y)
		; //
	}

	inline TYPE_DATA
	NORM () const {
		return
			std::sqrt (
				NORM2()
			)
		; //
	}

	inline TYPE_DATA
	DIST (
		TYPE_SELF const &
			other
	) const {
		return
			(
				this[0] - other
			).NORM()
		; //
	}

	inline TYPE_SELF
	DIR () const {
		return
			this[0] /
			NORM()
		; //
	}

	//////////////////////////////////
	// MATHEMATICAL OPERATIONS END. //
	//////////////////////////////////

} ;

using TYPE_POINT =
	_MACRO_CLASS_NAME_
; //

using TYPE_POINTS =
	typename TYPE_POINT::TYPE_POINTS
; //

#undef _MACRO_CLASS_NAME_

