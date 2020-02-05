	inline TYPE_SQUARES
	GET_SQUARES (
		TYPE_COLOR_IMAGE const &
			in
	) {

		using namespace cv ;

		auto edges =
			TYPE_MONOCHROME_IMAGE::GET_EDGES
				(in,0)
		; //

		Mat & canny_output =
			edges.MainMat
		; //

		std::vector <std::vector<Point>>
			contours
		; //
		findContours (
			edges.MainMat		,
			contours			,
			RETR_TREE			,
			CHAIN_APPROX_SIMPLE
		) ; //

		std::vector <std::vector<Point>>
			contours_poly (
				contours.size()
			)
		; //
		std::vector <Rect>
			boundRect (
				contours.size()
			)
		; //

		for ( size_t i = 0; i < contours.size(); i++ ) {
			approxPolyDP (
				contours[i]			,
				contours_poly[i]	,
				3					,
				true
			) ; //
			boundRect[i] =
				boundingRect (
					contours_poly[i]
				)
			; //
		}

		TYPE_SQUARES
			ret
		; /* Prepare the returns */ {
			for (
				size_t i = 0			;
				i < boundRect.size()	;
				i++
			) {
				ret.push_back(
					TYPE_SQUARE (
						boundRect[i]
					)
				) ;
			}
		}

		return ret ;

	}
