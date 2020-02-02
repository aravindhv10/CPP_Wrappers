#define _MACRO_CLASS_NAME_ MyLine

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
	; //

	using TYPE_LINES =
		std::vector <TYPE_SELF>
	; //

	using TYPE_POINT =
		MyPoint
	; //

	using TYPE_CV_LINES =
		std::vector	<cv::Vec4i>
	; //

	//////////////////////
	// DEFINITIONS END. //
	//////////////////////

	/////////////////
	// DATA BEGIN: //
	/////////////////

public:

	TYPE_POINT	const P1 ;
	TYPE_POINT	const P2 ;
	TYPE_POINT	const VC ;
	TYPE_POINT	const DR ;

	///////////////
	// DATA END. //
	///////////////

	///////////////////////////////////////
	// CONSTRUCTOR AND DESTRUCTOR BEGIN: //
	///////////////////////////////////////

public:

	_MACRO_CLASS_NAME_(
		TYPE_POINT const _P1 ,
		TYPE_POINT const _P2
	) :
	P1(_P1)			,
	P2(_P2)			,
	VC(P2-P1)		,
	DR(VC.DIR())	{}

	~_MACRO_CLASS_NAME_(){}

	static inline TYPE_CV_LINES
	CONVERT_LINES (
		TYPE_LINES const &
			in
	) {

		TYPE_CV_LINES
			ret (in.size())
		; //

		for(size_t i=0;i<in.size();i++){
			ret[i][0] = in[i].P1.X ;
			ret[i][1] = in[i].P1.Y ;
			ret[i][2] = in[i].P2.X ;
			ret[i][3] = in[i].P2.Y ;
		}

		return ret ;

	}

	static inline TYPE_LINES
	CONVERT_LINES (
		TYPE_CV_LINES const &
			in
	) {
		using namespace cv ;

		TYPE_LINES ret ;

		for( size_t i = 0; i < in.size(); i++ ) {

			Vec4i const &
				l =
					in[i]
			; //

			double const x1 =
				static_cast <double>
					(l[0])
			; //
			double const y1 =
				static_cast <double>
					(l[1])
			; //
			double const x2 =
				static_cast <double>
					(l[2])
			; //
			double const y2 =
				static_cast <double>
					(l[3])
			; //

			MyLine
				theline (
					{x1,y1} ,
					{x2,y2}
				)
			; //

			ret.push_back (
				theline
			) ; //

		}

		return ret ;

	}

	/////////////////////////////////////
	// CONSTRUCTOR AND DESTRUCTOR END. //
	/////////////////////////////////////

	///////////////////
	// SLOPES BEGIN: //
	///////////////////

	inline TYPE_DATA const
	M_H () const {

		TYPE_DATA const N =
			VC.Y
		; //
		TYPE_DATA const D =
			VC.X
		; //

		if (
			CPPFileIO::mymod(D) >
			0.0000001
		) {
			return N / D ;
		} else {
			return 1000000 ;
		}

	}

	inline TYPE_DATA const
	M_V () const {

		TYPE_DATA const N =
			VC.X
		; //
		TYPE_DATA const D =
			VC.Y
		; //

		if (
			CPPFileIO::mymod(D) >
			0.0000001
		) {
			return N / D ;
		} else {
			return 1000000 ;
		}

	}

	/////////////////
	// SLOPES END. //
	/////////////////

	///////////////////////////////////
	// IMPORTANT CALCULATIONS BEGIN: //
	///////////////////////////////////

	inline TYPE_DATA const
	ShortestDistnace (
		TYPE_POINT const P
	) const {

		auto const PP1 = P - P1 ;
		auto const PP2 = P - P2 ;
		auto const CT1 = PP1.DIR() * DR ;
		auto const CT2 = PP2.DIR() * DR ;
		auto const ST1 = std::sqrt( 1.0 - std::pow(CT1,2) ) ;
		auto const ST2 = std::sqrt( 1.0 - std::pow(CT2,2) ) ;
		auto const dist1 = PP1.NORM() * ST1 ;
		auto const dist2 = PP2.NORM() * ST2 ;

		return ( dist1 + dist2 ) / 2.0 ;

	}

	inline TYPE_DATA const
	operator () (
		TYPE_POINT const P
	) const {
		return
			ShortestDistnace
				(P)
		; //
	}

	inline TYPE_DATA const
	operator () (
		double const y ,
		double const x
	) const {
		return
			ShortestDistnace({y,x})
		; //
	}

	/////////////////////////////////
	// IMPORTANT CALCULATIONS END. //
	/////////////////////////////////

} ;

#undef _MACRO_CLASS_NAME_
