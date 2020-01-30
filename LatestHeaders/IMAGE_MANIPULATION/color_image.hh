#ifdef OPENCV_ALL_HPP

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

	inline void
	Store2Mat () {
		if(MainStore.ALLOCATED()){
			MainMat =
				cv::Mat::eye (
					SIZE_Y(),
					SIZE_X(),
					CV_8UC3
				)
			; /* Prepare the return matrix: */ {
				for ( int y = 0 ; y < SIZE_Y() ; y++ ) {
					uchar * p =
						MainMat.ptr<uchar>(y)
					; //
					for ( int x = 0 ; x < SIZE_X() ; x++ ) {
						uchar * pix = & (p[x*3]) ;
						pix[0] = MainStore(y,x).B() ;
						pix[1] = MainStore(y,x).G() ;
						pix[2] = MainStore(y,x).R() ;
					}
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

		if (channels!=3) {
			printf (
				"As of now, I have only written this for color images..."
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
