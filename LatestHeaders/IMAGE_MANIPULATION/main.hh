#ifndef _MACRO_IMAGE_MANIPULATION_
#define _MACRO_IMAGE_MANIPULATION_

#include "../CPPFileIO/main.hh"

#ifdef USE_CV
#include <opencv2/opencv.hpp>
#endif

namespace IMAGE_MANIPULATION {

	using TYPE_DATA =
		double
	; //

	using TYPE_BYTE =
		unsigned char
	; //

	#include "./point.hh"
	#include "./line.hh"
	#include "./pixel.hh"
	#include "./color_image.hh"
	#include "./monochrome_image.hh"

}

#endif
