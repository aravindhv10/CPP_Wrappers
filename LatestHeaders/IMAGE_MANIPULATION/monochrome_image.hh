#ifdef OPENCV_ALL_HPP

#define _MACRO_CLASS_NAME_ MyMonochromeImage

class _MACRO_CLASS_NAME_ {

	////////////////////////
	// DEFINITIONS BEGIN: //
	////////////////////////

public:

	using TYPE_DATA =
		double
	; //

	using TYPE_BYTE =
		unsigned char
	; //

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

	inline void
	Store2Mat () {

		if (MainStore.ALLOCATED()) {

			MainMat =
				cv::Mat::eye (
					SIZE_Y() ,
					SIZE_X() ,
					CV_8UC1
				)
			; //

			for ( int y = 0 ; y < SIZE_Y() ; y++ ) {

				uchar * p =
					MainMat.ptr<uchar>(y)
				; //

				for ( int x = 0 ; x < SIZE_X() ; x++ ) {
					p[x] = MainStore(y,x) ;
				}

			}

		}

	}

	//////////////////////////
	// DATA CONVERSION END. //
	//////////////////////////

	/////////////////////////////////////////
	// CONSTRUCTION AND DESTRUCTION BEGIN: //
	/////////////////////////////////////////

private:

	static inline TYPE_STORE
	GET_STORE (
		cv::Mat & in
	) {

		int const channels	= in.channels()	;

		if (channels!=1) {
			printf (
				"As of now, I have only written this for monochrome images..."
				"You are doing something wrong...\n"
			) ;
		}

		int const Y = in.rows		; // Y
		int const X = in.cols		; // X

		TYPE_PIXEL * p =
			reinterpret_cast<TYPE_PIXEL*>(
				in.ptr<TYPE_BYTE>(0)
			)
		; //

		TYPE_STORE ret (p,Y,X) ;
		return ret ;

	}

public:

	_MACRO_CLASS_NAME_(
		cv::Mat const & in
	) :
	MainMat(in)						,
	MainStore(GET_STORE(MainMat))	{}

	_MACRO_CLASS_NAME_(
		size_t sy ,
		size_t sx
	) : MainStore(sy,sx) {}

	_MACRO_CLASS_NAME_(
		TYPE_STORE const & in
	) : MainStore(TYPE_STORE::CLONE(in))
	{ Store2Mat() ; }


	_MACRO_CLASS_NAME_(
		MyColorImage const & in ,
		char const channel = 'R'
	) :
	MainStore (
		in.SIZE_Y()	,
		in.SIZE_X()
	) {

		if (channel=='B') {
			for(size_t y=0;y<SIZE_Y();y++)
			for(size_t x=0;x<SIZE_X();x++){
				MainStore(y,x) =
					in.MainStore(y,x).B()
				; //
			}
		} else if (channel=='G') {
			for(size_t y=0;y<SIZE_Y();y++)
			for(size_t x=0;x<SIZE_X();x++){
				MainStore(y,x) =
					in.MainStore(y,x).G()
				; //
			}
		} else if (channel=='R') {
			for(size_t y=0;y<SIZE_Y();y++)
			for(size_t x=0;x<SIZE_X();x++){
				MainStore(y,x) =
					in.MainStore(y,x).R()
				; //
			}
		}

	}

	~_MACRO_CLASS_NAME_(){}

	static inline TYPE_SELF
	GET_EDGES (
		MyColorImage	const &	in						,
		double					threshold1		= 150	,
		double					threshold2		= 150	,
		int						apertureSize	= 3		,
		bool					L2gradient		= true
	) {
		cv::Mat Edges	;
		cv::Mat Blur	;
		blur (
			in.MainMat		,
			Blur			,
			cv::Size(2,2)
		) ; //
		Canny (
			/* InputArray	image			= */ Blur			,
			/* OutputArray	edges			= */ Edges			,
			/* double		threshold1		= */ threshold1		,
			/* double		threshold2		= */ threshold2		,
			/* int			apertureSize	= */ apertureSize	,
			/* bool			L2gradient		= */ L2gradient
		) ; //
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
		Store2Mat () ;
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
