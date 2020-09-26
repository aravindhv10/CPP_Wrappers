#ifndef _HEADER_GUARD_PolarDistance_
#define _HEADER_GUARD_PolarDistance_

#include "./Headers.hh"

#define _MACRO_CLASS_NAME_ PolarPoint
class _MACRO_CLASS_NAME_ {
	public:

		double theta, phi ;
		double latitude, longitude ;

		using TYPE_SELF = _MACRO_CLASS_NAME_ ;

		inline double
		distance (PolarPoint const & other) const {

			double constexpr r = 6367000 ;

			double const ct1 = std::cos(theta);
			double const st1 = std::sin(theta);
			double const ct2 = std::cos(other.theta);
			double const st2 = std::sin(other.theta);

			double const cp1 = std::cos(phi);
			double const sp1 = std::sin(phi);
			double const cp2 = std::cos(other.phi);
			double const sp2 = std::sin(other.phi);

			double const x1 = st1*cp1 ;
			double const y1 = st1*sp1 ;
			double const z1 = ct1 ;

			double const x2 = st2*cp2 ;
			double const y2 = st2*sp2 ;
			double const z2 = ct2 ;

			double ca = (x1*x2)+(y1*y2)+(z1*z2) ;
			if(ca>1){ca=1;}
			else if(ca<-1){ca=-1;}
			double const a = std::acos(ca);
			return 	r*a ;

		}

		_MACRO_CLASS_NAME_(double const _theta, double const _phi):
		theta(_theta), phi(_phi) {}

		static inline TYPE_SELF
		AS_LAT_LONG (double const lt, double const lg) {
			TYPE_SELF ret ( (M_PI/2.0) - (lt*M_PI/180.0) , (lg*M_PI/180.0)  )  ;
			ret.latitude = lt ;
			ret.longitude = lg ;
			return ret ;
		}

		_MACRO_CLASS_NAME_(){}
		~_MACRO_CLASS_NAME_(){}
} ;
#undef _MACRO_CLASS_NAME_

#endif
