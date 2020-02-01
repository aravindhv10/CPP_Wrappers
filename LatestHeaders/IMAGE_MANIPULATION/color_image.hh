﻿#ifdef OPENCV_ALL_HPP

#define _MACRO_CLASS_NAME_ MyColorImage

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
	; //

	using TYPE_PIXEL =
		ColorPixel
	; //

	using TYPE_STORE =
		DYNAMIC_ARRAYS::Dynamic2DArray
			<TYPE_PIXEL>
	; //

	//////////////////////
	// DEFINITIONS END. //
	//////////////////////

	/////////////////
	// DATA BEGIN: //
	/////////////////

public:

	cv::Mat
		MainMat
	; //

	TYPE_STORE
		MainStore
	; //

	///////////////
	// DATA END. //
	///////////////

	////////////////////////////
	// DATA CONVERSION BEGIN: //
	////////////////////////////

public:

	static inline cv::Mat
	CloneStore2Mat (
		TYPE_STORE const &
			in
	) {

		size_t const
			SIZE_Y =
				in.SIZE_2()
		; //
		size_t const
			SIZE_X =
				in.SIZE_1()
		; //

		cv::Mat
			ret =
				cv::Mat::eye (
					SIZE_Y	,
					SIZE_X	,
					CV_8UC3
				)
		; //

		for ( int y = 0 ; y < SIZE_Y ; y++ ) {

			uchar * p =
				ret.ptr<uchar>(y)
			; //

			for ( int x = 0 ; x < SIZE_X ; x++ ) {
				uchar * pix = & (p[x*3]) ;
				pix[0] = in(y,x).B() ;
				pix[1] = in(y,x).G() ;
				pix[2] = in(y,x).R() ;
			}

		}

		return
			ret
		; //

	}

	static inline TYPE_STORE
	CloneMat2Store (
		cv::Mat const &
			in
	) {

		int const Y = in.rows		;
		int const X = in.cols		;
		int const C = in.channels()	;

		if (C!=3) {
			printf (
				"Only written for 3 color matrices, ur doing something wrong...\n"
			) ; //
		}

		TYPE_STORE ret (Y,X) ;

		for ( int y = 0 ; y < Y ; y++ ) {

			uchar const * p =
				in.ptr<uchar const>(y)
			; //

			for ( int x = 0 ; x < X ; x++ ) {

				uchar const *
					pix =
						& ( p[3*x] )
				; //

				ret
					(y,x) (
						pix[0]	,
						pix[1]	,
						pix[2]
					)
				; //

			}

		}

		return ret ;

	}

	static inline TYPE_STORE
	ViewMat2Store (
		cv::Mat & in
	) {

		int const channels	= in.channels()	;

		if (channels!=3) {
			printf (
				"As of now, I have only written this for color images..."
				"You are doing something wrong...\n"
			) ;
		}

		int const Y = in.rows ;
		int const X = in.cols ;

		TYPE_PIXEL * p =
			reinterpret_cast<TYPE_PIXEL*>(
				in.ptr<TYPE_BYTE>(0)
			)
		; //

		TYPE_STORE ret (p,Y,X) ;
		return ret ;

	}

	//////////////////////////
	// DATA CONVERSION END. //
	//////////////////////////

	/////////////////////////////////////////
	// CONSTRUCTION AND DESTRUCTION BEGIN: //
	/////////////////////////////////////////

public:

	_MACRO_CLASS_NAME_(
		cv::Mat const &
			in
	) :
	MainMat		(in)						,
	MainStore	(ViewMat2Store(MainMat))	{}

	_MACRO_CLASS_NAME_(
		size_t sy ,
		size_t sx
	) :
	MainMat		(cv::Mat::eye(sy,sx,CV_8UC3))	,
	MainStore	(ViewMat2Store(MainMat))		{}

	_MACRO_CLASS_NAME_(
		TYPE_STORE const &
			in
	) :
	MainMat		(CloneStore2Mat(in))		,
	MainStore	(ViewMat2Store(MainMat))	{}

	~_MACRO_CLASS_NAME_(){}

	static inline TYPE_SELF
	READ_FROM_FILE (
		std::string const
			filename
	) {

		cv::Mat image =
			cv::imread (
				&(filename[0]) ,
				1
			)
		; //

		return
			TYPE_SELF (image)
		; //

	}

	///////////////////////////////////////
	// CONSTRUCTION AND DESTRUCTION END. //
	///////////////////////////////////////

	///////////////////////////////
	// INTERFACE FOR SIZE BEGIN: //
	///////////////////////////////

public:

	inline size_t const
	SIZE_Y () const {
		return
			MainStore
				.SIZE_2()
		; //
	}

	inline size_t const
	SIZE_X () const {
		return
			MainStore
				.SIZE_1()
		; //
	}

	/////////////////////////////
	// INTERFACE FOR SIZE END. //
	/////////////////////////////

	//////////////////////////
	// IMAGE RELATED BEGIN: //
	//////////////////////////

public:

	inline TYPE_SELF
	CROP (
		size_t AY , size_t AX ,
		size_t BY , size_t BX
	) {
		TYPE_SELF
			ret (
				MainStore.CloneRange(
					AY,AX,
					BY,BX
				)
			)
		; //
		return ret ;
	}

	inline void
	TO_FILE (
		std::string const
			filename
	) {
		imwrite (
			&(filename[0]),
			MainMat
		) ;
	}

	////////////////////////
	// IMAGE RELATED END. //
	////////////////////////

} ;


#undef _MACRO_INHERIT_DEF_
#undef _MACRO_CLASS_NAME_

#endif