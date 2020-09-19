#ifdef OPENCV_ALL_HPP

#define _MACRO_CLASS_NAME_ MyMonochromeImage

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_SELF =
		_MACRO_CLASS_NAME_
	; //

	using TYPE_PIXEL =
		TYPE_BYTE
	; //

	using TYPE_STORE =
		DYNAMIC_ARRAYS::Dynamic2DArray
			<TYPE_PIXEL>
	; //

	using TYPE_LINE =
		MyLine
	; //

	using TYPE_LINES =
		typename TYPE_LINE::TYPE_LINES
	; //

	using TYPE_CV_LINES =
		typename TYPE_LINE::TYPE_CV_LINES
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
					CV_8UC1
				)
		; //

		for ( int y = 0 ; y < SIZE_Y ; y++ ) {

			uchar * p =
				ret.ptr<uchar>(y)
			; //

			for ( int x = 0 ; x < SIZE_X ; x++ )
			{ p[x] = in(y,x) ; }

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

		if (C!=1) {
			printf (
				"Only written for monochrome matrices, ur doing something wrong...\n"
			) ; //
		}

		TYPE_STORE ret (Y,X) ;

		for ( int y = 0 ; y < Y ; y++ ) {

			uchar const * p =
				in.ptr<uchar const>(y)
			; //

			for ( int x = 0 ; x < X ; x++ )
			{ ret(y,x) = p[x] ; }

		}

		return ret ;

	}

	static inline TYPE_STORE
	ViewMat2Store (
		cv::Mat & in
	) {

		int const channels	= in.channels()	;

		if (channels!=1) {
			printf (
				"As of now, I have only written this for monochrome images..."
				"You are doing something wrong...\n"
			) ;
		}

		int const Y = in.rows ; // Y
		int const X = in.cols ; // X

		TYPE_PIXEL * p =
			reinterpret_cast<TYPE_PIXEL*>(
				in.ptr<TYPE_BYTE>(0)
			)
		; //

		TYPE_STORE	ret (p,Y,X)	;
		return		ret			;

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
		TYPE_STORE const & in
	) :
	MainMat		(CloneStore2Mat(in))		,
	MainStore	(ViewMat2Store(MainMat))	{}

	~_MACRO_CLASS_NAME_(){}

	static inline TYPE_SELF
	GET_EDGES (

		MyColorImage	const &	in						,
		int				const	NumPixel		= 3		,
		double			const	threshold1		= 50	,
		double			const	threshold2		= 150	,
		int				const	apertureSize	= 3		,
		bool			const	L2gradient		= true

	) {

		cv::Mat
			Blur
		; /* Prepare the blur image: */ {

			if (NumPixel>0) {

				blur (
					in.MainMat		,
					Blur			,
					cv::Size(NumPixel,NumPixel)
				) ; //

			} else {

				Blur =
					in.MainMat
				; //

			}

		}

		cv::Mat
			Edges
		; /* Prepare the edges: */ {

			Canny (
				/* InputArray	image			= */ Blur			,
				/* OutputArray	edges			= */ Edges			,
				/* double		threshold1		= */ threshold1		,
				/* double		threshold2		= */ threshold2		,
				/* int			apertureSize	= */ apertureSize	,
				/* bool			L2gradient		= */ L2gradient
			) ; //

		}

		TYPE_SELF ret(Edges) ;
		return ret ;

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
	) const {
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
	) const {
		imwrite (
			&(filename[0]),
			MainMat
		) ;
	}

	////////////////////////
	// IMAGE RELATED END. //
	////////////////////////

	inline TYPE_PIXEL
	operator () (
		size_t const Y ,
		size_t const X
	) const {
		return
			MainStore(Y,X)
		; //
	}

	inline void
	DRAW_LINES (
		TYPE_CV_LINES const &
			in
	) {

		using namespace cv ;

		for (
			size_t i=0;
			i<in.size();
			i++
		) {

			auto const &
				l = in[i]
			; //

			line (
				MainMat				,
				Point(l[0], l[1])	,
				Point(l[2], l[3])	,
				Scalar(255,10,10)	,
				1, 8
			) ; //

		}

	}

	inline TYPE_LINES
	GET_LINES (

		int		const NumPixel		= 3			,
		double	const rho			= 1			,
		double	const theta			= CV_PI/180	,
		int		const threshold		= 50		,
		double	const minLineLength	= 500		,
		double	const maxLineGap	= 10

	) const {
		using namespace cv ;

		TYPE_CV_LINES	lines		;
		Mat				BlurInImage	;

		if(NumPixel>0){
			blur (
				MainMat		,
				BlurInImage	,
				Size(NumPixel,NumPixel)
			) ; //
		} else {
			BlurInImage =
				MainMat
			; //
		}

		HoughLinesP (
			/* InputArray	image			= */ BlurInImage	,
			/* OutputArray	lines			= */ lines			,
			/* double		rho				= */ rho			,
			/* double		theta			= */ theta			,
			/* int			threshold		= */ threshold		,
			/* double		minLineLength	= */ minLineLength	,
			/* double		maxLineGap		= */ maxLineGap
		) ;

		TYPE_LINES ret ;

		for (
			size_t i = 0		;
			i < lines.size()	;
			i++
		) {

			Vec4i const &
				l =
					lines[i]
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
					{y1,x1} ,
					{y2,x2}
				)
			; //

			ret.push_back (
				theline
			) ; //

		}

		return ret ;

	}

} ;

using TYPE_MONOCHROME_IMAGE =
	_MACRO_CLASS_NAME_
; //

#undef _MACRO_INHERIT_DEF_
#undef _MACRO_CLASS_NAME_

#endif
