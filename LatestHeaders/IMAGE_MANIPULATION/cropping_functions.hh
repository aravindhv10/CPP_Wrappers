	inline TYPE_COLOR_IMAGE
	LINE_CROP (
		TYPE_COLOR_IMAGE	const & in_img		,
		TYPE_LINES			const & in_lines
	) {

		size_t xmin = in_img.SIZE_X()	;
		size_t xmax = 0					;
		size_t ymin = in_img.SIZE_Y()	;
		size_t ymax = 0					;

		for (
			size_t i = 0		;
			i < in_lines.size()	;
			i++
		) {
			xmin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P1.X) , xmin ) ;
			xmin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P2.X) , xmin ) ;
			ymin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P1.Y) , ymin ) ;
			ymin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P2.Y) , ymin ) ;
			xmax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P1.X) , xmax ) ;
			xmax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P2.X) , xmax ) ;
			ymax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P1.Y) , ymax ) ;
			ymax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P2.Y) , ymax ) ;
		}

		return
			in_img.CROP (
				ymin, xmin ,
				ymax, xmax
			)
		; //

	}

	inline void
	LINE_CLEAN (
		TYPE_COLOR_IMAGE			& in_img	,
		TYPE_LINES			const	& in_lines
	) {

		size_t xmin = in_img.SIZE_X()	;
		size_t xmax = 0					;
		size_t ymin = in_img.SIZE_Y()	;
		size_t ymax = 0					;

		for (
			size_t i = 0		;
			i < in_lines.size()	;
			i++
		) {
			xmin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P1.X) , xmin ) ;
			xmin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P2.X) , xmin ) ;
			ymin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P1.Y) , ymin ) ;
			ymin = CPPFileIO::mymin ( static_cast <size_t> (in_lines[i].P2.Y) , ymin ) ;
			xmax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P1.X) , xmax ) ;
			xmax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P2.X) , xmax ) ;
			ymax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P1.Y) , ymax ) ;
			ymax = CPPFileIO::mymax ( static_cast <size_t> (in_lines[i].P2.Y) , ymax ) ;
		}

		TYPE_PIXEL Black(0,0,0) ;

		for (size_t x=0;x<xmin;x++) {
			for (size_t y=0;y<in_img.SIZE_Y();y++) {
				in_img.MainStore(y,x) = Black ;
			}
		}
		for (size_t x=xmax+1;x<in_img.SIZE_X();x++) {
			for (size_t y=0;y<in_img.SIZE_Y();y++) {
				in_img.MainStore(y,x) = Black ;
			}
		}
		for (size_t y=0;y<ymin;y++) {
			for (size_t x=0;x<in_img.SIZE_X();x++) {
				in_img.MainStore(y,x) = Black ;
			}
		}
		for (size_t y=ymax+1;y<in_img.SIZE_Y();y++) {
			for (size_t x=0;x<in_img.SIZE_X();x++) {
				in_img.MainStore(y,x) = Black ;
			}
		}
	}

	inline TYPE_COLOR_IMAGE
	MIN_MAX_CROP (
		TYPE_COLOR_IMAGE const &
			in_img
	) {

		size_t xmin = in_img.SIZE_X()	;
		size_t xmax = 0					;
		size_t ymin = in_img.SIZE_Y()	;
		size_t ymax = 0					;

		for (size_t y=0;y<in_img.SIZE_Y();y++)
		for (size_t x=0;x<in_img.SIZE_X();x++) {

			bool const IsBlack =
				(in_img.MainStore(y,x).R()==0) &&
				(in_img.MainStore(y,x).G()==0) &&
				(in_img.MainStore(y,x).B()==0)
			; //

			if (!IsBlack) {
				xmin =
					CPPFileIO::mymin (
						x , xmin
					)
				; //
				xmax =
					CPPFileIO::mymax (
						x , xmax
					)
				; //
				ymax =
					CPPFileIO::mymax (
						y , ymax
					)
				; //
				ymin =
					CPPFileIO::mymin (
						y , ymin
					)
				; //
			}

		}

		return
			in_img.CROP (
				ymin, xmin ,
				ymax, xmax
			)
		; //

	}
