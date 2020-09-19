#define _MACRO_CLASS_NAME_ linearinterpolate
#define AS_DATA(val) static_cast<TYPE_DATA>(val)

class _MACRO_CLASS_NAME_ {

private:

	TYPE_POINT const P1 ;
	TYPE_POINT const P2 ;

public:

	inline TYPE_POINT
	Interpolate (
		TYPE_DATA const t
	) const {
		return
			(P1*AS_DATA(1.0-t)) +
			(P2*AS_DATA(t))
		; //
	}

	inline TYPE_POINT
	operator () (
		TYPE_DATA const t
	) const {
		return
			Interpolate(t)
		; //
	}

	_MACRO_CLASS_NAME_(
		TYPE_POINT const _P1 ,
		TYPE_POINT const _P2
	) : P1(_P1) , P2(_P2) {}

	_MACRO_CLASS_NAME_(
		TYPE_DATA const Y1 ,
		TYPE_DATA const X1 ,
		TYPE_DATA const Y2 ,
		TYPE_DATA const X2
	) : P1(Y1,X1) , P2(Y2,X2) {}

	~_MACRO_CLASS_NAME_(){}

} ;

using TYPE_INTERPOLATOR =
	_MACRO_CLASS_NAME_
; //

#undef AS_DATA
#undef _MACRO_CLASS_NAME_
