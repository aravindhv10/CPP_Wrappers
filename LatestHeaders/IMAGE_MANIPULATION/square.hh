
#define _MACRO_CLASS_NAME_ MySquare

//////////////////////////////////////
#define _MACRO_MAKE_DEFINITIONS_	\
									\
	TYPE_POINT const P1 (Y1,X1) ;	\
	TYPE_POINT const P2 (Y1,X2) ;	\
	TYPE_POINT const P3 (Y2,X1) ;	\
	TYPE_POINT const P4 (Y2,X2) ;	\
									\
	TYPE_LINE const L12 (P1,P2) ;	\
	TYPE_LINE const L13 (P1,P3) ;	\
	TYPE_LINE const L34 (P3,P4) ;	\
	TYPE_LINE const L24 (P2,P4) ;	\
									//
//////////////////////////////////////

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
	; //

	using TYPE_SQUARES =
		std::vector <TYPE_SELF>
	; //

	//////////////////////
	// DEFINITIONS END. //
	//////////////////////

	/////////////////
	// DATA BEGIN: //
	/////////////////

private:

	TYPE_DATA X1, Y1 ;
	TYPE_DATA X2, Y2 ;

	///////////////
	// DATA END. //
	///////////////

	/////////////////////////////////
	// RETRIEVING FUNCTIONS BEGIN: //
	/////////////////////////////////

public:

	inline TYPE_DATA
	GET_X1() const {
		return X1 ;
	}

	inline TYPE_DATA
	GET_X2() const {
		return X2 ;
	}

	inline TYPE_DATA
	GET_Y1() const {
		return Y1 ;
	}

	inline TYPE_DATA
	GET_Y2() const {
		return Y2 ;
	}

	inline long
	X1_N () const {
		return
			static_cast<long>(X1)
		; //
	}

	inline long
	X2_N () const {
		return
			static_cast<long>(X2)
		; //
	}

	inline long
	Y1_N () const {
		return
			static_cast<long>(Y1)
		; //
	}

	inline long
	Y2_N () const {
		return
			static_cast<long>(Y2)
		; //
	}

	inline TYPE_POINT const
	GET_P1 () const {
		_MACRO_MAKE_DEFINITIONS_
		return P1 ;
	}

	inline TYPE_POINT const
	GET_P2 () const {
		_MACRO_MAKE_DEFINITIONS_
		return P2 ;
	}

	inline TYPE_POINT const
	GET_P3 () const {
		_MACRO_MAKE_DEFINITIONS_
		return P3 ;
	}

	inline TYPE_POINT const
	GET_P4 () const {
		_MACRO_MAKE_DEFINITIONS_
		return P4 ;
	}

	inline TYPE_LINE const
	GET_L12 () const {
		_MACRO_MAKE_DEFINITIONS_
		return L12 ;
	}

	inline TYPE_LINE const
	GET_L13 () const {
		_MACRO_MAKE_DEFINITIONS_
		return L13 ;
	}

	inline TYPE_LINE const
	GET_L34 () const {
		_MACRO_MAKE_DEFINITIONS_
		return L34 ;
	}

	inline TYPE_LINE const
	GET_L24 () const {
		_MACRO_MAKE_DEFINITIONS_
		return L24 ;
	}

	///////////////////////////////
	// RETRIEVING FUNCTIONS END. //
	///////////////////////////////

	//////////////////////////////
	// CHECK CONTAINMENT BEGIN: //
	//////////////////////////////

public:

	inline bool
	IsContainedIn (
		TYPE_POINT const
			in
	) const {
		return
			(X1<=in.X) && (in.X<=X2) &&
			(Y1<=in.Y) && (in.Y<=Y2)
		; //
	}

	inline bool
	IsContainedIn (
		TYPE_LINE const &
			in
	) const {
		return
			IsContainedIn(in.P1) ||
			IsContainedIn(in.P2)
		; //
	}

	inline bool
	IsContainedIn (
		TYPE_SELF const &
			in
	) const {
		return
			IsContainedIn ( in.GET_P1() ) ||
			IsContainedIn ( in.GET_P2() ) ||
			IsContainedIn ( in.GET_P3() ) ||
			IsContainedIn ( in.GET_P4() )
		; //
	}

	inline bool
	IsCompletelyContainedIn (
		TYPE_LINE const &
			in
	) const {
		return
			IsContainedIn(in.P1) &&
			IsContainedIn(in.P2)
		; //
	}

	inline bool
	IsCompletelyContainedIn (
		TYPE_SELF const &
			in
	) const {
		return
			IsContainedIn ( in.GET_P1() ) &&
			IsContainedIn ( in.GET_P2() ) &&
			IsContainedIn ( in.GET_P3() ) &&
			IsContainedIn ( in.GET_P4() )
		; //
	}

	////////////////////////////
	// CHECK CONTAINMENT END. //
	////////////////////////////

	///////////////////////////////////
	// IMPORTANT CALCULATIONS BEGIN: //
	///////////////////////////////////

public:

	inline TYPE_SELF
	MergeWith (
		TYPE_SELF const &
			other
	) const {

		TYPE_SELF
			ret (
				TYPE_POINT ( CPPFileIO::mymin(Y1,other.Y1) , CPPFileIO::mymin(X1,other.X1) ) ,
				TYPE_POINT ( CPPFileIO::mymax(Y2,other.Y2) , CPPFileIO::mymax(X2,other.X2) )
			)
		; //

		return ret ;

	}

	inline TYPE_DATA
	Area () const {
		return
			(X2-X1) *
			(Y2-Y1)
		; //
	}

	inline TYPE_DATA
	HarmonicLength () const {
		return
			std::sqrt (
				Area()
			)
		; //
	}

	inline bool
	IsValid () const {
		return
			( X1 < X2 ) &&
			( Y1 < Y2 ) &&
			( X1 >= 0 ) &&
			( X2 >= 0 ) &&
			( Y1 >= 0 ) &&
			( Y2 >= 0 )
		; //
	}

	inline void
	InValidate ()  {
		X1 = -1000 ;
		X2 = -1000 ;
		Y1 = -1000 ;
		Y2 = -1000 ;
	}

	/////////////////////////////////
	// IMPORTANT CALCULATIONS END. //
	/////////////////////////////////

	//////////////////////
	// DISTANCES BEGIN: //
	//////////////////////

public:

	inline TYPE_DATA
	Distance (
		TYPE_POINT const
			in
	) const {

		if (IsContainedIn(in)) {

			return 0 ;

		} else {

			_MACRO_MAKE_DEFINITIONS_

			TYPE_DATA const ret =
				CPPFileIO::mymin (
					CPPFileIO::mymin ( L12(in) , L13(in) ) ,
					CPPFileIO::mymin ( L34(in) , L24(in) )
				)
			; //

			return ret ;

		}

	}

	inline TYPE_DATA
	Distance (
		TYPE_POINTS const &
			in
	) const {

		TYPE_DATA ret = 1000000000 ;

		for(size_t i=0;i<in.size();i++){
			ret =
				CPPFileIO::mymin (
					Distance(in[i])	,
					ret
				)
			; //
		}

		return ret ;

	}

	inline TYPE_DATA
	Distance (
		TYPE_LINE const &
			in
	) const {

		if (IsContainedIn(in)) {

			return 0 ;

		} else {

			_MACRO_MAKE_DEFINITIONS_

			TYPE_DATA const ret =
				CPPFileIO::mymin (
					CPPFileIO::mymin(in(P1),in(P2)) ,
					CPPFileIO::mymin(in(P3),in(P4))
				)
			;

			return ret ;

		}

	}

	inline TYPE_DATA
	Distance (
		TYPE_LINES const &
			in
	) const {

		TYPE_DATA ret =
			1000000000
		; //

		for (size_t i=0;i<in.size();i++) {
			ret =
				CPPFileIO::mymin (
					ret				,
					Distance(in[i])
				)
			; //
		}

		return ret ;

	}

	inline TYPE_DATA
	Distance (
		TYPE_SELF const &
			in
	) const {

		TYPE_DATA const
			A1 = Area()
		; //
		TYPE_DATA const
			A2 = in.Area()
		; //

		TYPE_DATA const
			sum =
				A1 + A2
		; //

		TYPE_DATA const
			har =
				std::sqrt(A1*A2)
		; //

		TYPE_SELF const
			merged_square =
				MergeWith(in)
		; //

		TYPE_DATA const
			mergedarea =
				merged_square
					.Area()
		; //

		TYPE_DATA const
			diff =
				mergedarea -
				sum
		; //


		TYPE_DATA ret =
			CPPFileIO::mymax (
				diff ,
				static_cast<TYPE_DATA>(0)
			) / har
		; //

		return ret ;

	}

	////////////////////
	// DISTANCES END. //
	////////////////////

public:

	////////////////////////////////
	// CONVINENT FUNCTIONS BEGIN: //
	////////////////////////////////


	inline TYPE_DATA
	operator () (
		TYPE_LINE const &
			in
	) const {
		return
			Distance
				(in)
		; //
	}

	inline TYPE_DATA
	operator () (
		TYPE_LINES const &
			in
	) const {
		return
			Distance
				(in)
		; //
	}

	inline TYPE_DATA
	operator () (
		TYPE_POINT const &
			in
	) const {
		return
			Distance
				(in)
		; //
	}

	inline TYPE_DATA
	operator () (
		TYPE_POINTS const &
			in
	) const {
		return
			Distance
				(in)
		; //
	}

	inline TYPE_DATA
	operator () (
		TYPE_SELF const &
			in
	) const {
		return
			Distance
				(in)
		; //
	}

	//////////////////////////////
	// CONVINENT FUNCTIONS END. //
	//////////////////////////////

	////////////////////////
	// CONSTRUCTOR BEGIN: //
	////////////////////////

public:

	_MACRO_CLASS_NAME_(
		TYPE_POINT const P1 ,
		TYPE_POINT const P2
	) {
		X1 = P1.X ; X2 = P2.X ;
		Y1 = P1.Y ; Y2 = P2.Y ;
	}

	_MACRO_CLASS_NAME_(
		TYPE_DATA const y1 , TYPE_DATA const x1 ,
		TYPE_DATA const y2 , TYPE_DATA const x2
	) {
		X1 = x1 ; X2 = x2 ;
		Y1 = y1 ; Y2 = y2 ;
	}

	~_MACRO_CLASS_NAME_(){}

	//////////////////////
	// CONSTRUCTOR END. //
	//////////////////////

	// CONVINENT FUNCTIONS BEGIN: //

	static inline TYPE_SQUARES
	MERGE_SQUARES (
		TYPE_SQUARES const &
			in
	) {
		bool merged = false ;

		TYPE_SQUARES tmp ;
		for(size_t i=0;i<in.size();i++){
			if (in[i].IsValid()) {
				tmp.push_back(in[i]);
			}
		}

		for (size_t i=1;i<tmp.size();i++) if(tmp[i].IsValid()) {
			for (size_t j=0;j<i;j++) if(tmp[j].IsValid()) {

				auto const
					treshhold =
						std::sqrt (
							tmp[i].HarmonicLength() *
							tmp[j].HarmonicLength()
						) * 0.1
				; //

				if ( tmp[i](tmp[j]) < treshhold ) {
					merged = true ;
					tmp.push_back (tmp[i].MergeWith(tmp[j])) ;
					tmp[j].InValidate();
					tmp[i].InValidate();
				}

			}
		}

		if (merged) {

			return
				MERGE_SQUARES
					(tmp)
			; //

		} else {

			return
				tmp
			; //

		}

	}

	//////////////////////////////
	// CONVINENT FUNCTIONS END. //
	//////////////////////////////

} ;

using TYPE_SQUARE =
	_MACRO_CLASS_NAME_
; //

using TYPE_SQUARES =
	typename TYPE_SQUARE::TYPE_SQUARES
; //

#undef _MACRO_CLASS_NAME_
#undef _MACRO_MAKE_DEFINITIONS_
