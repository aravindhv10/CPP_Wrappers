#ifndef _HEADER_GUARD_PolarCoordinates_
#define _HEADER_GUARD_PolarCoordinates_

#include "./Headers.hh"
#include "./StaticArray.hh"

#define _MACRO_CLASS_NAME_ D2GPS_Coordinates
template <typename T> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_DATA = T;

    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_DATA>;

    using TYPE_CARTITION = StaticArray::ND_ARRAY<3, TYPE_DATA>;

    inline static TYPE_DATA constexpr PI() { return M_PI; }

    inline static TYPE_DATA constexpr PIB2() { return M_PI / 2.0; }

    inline static TYPE_DATA constexpr EarthRadius() { return 6367000; }

    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  public:
    TYPE_DATA latitude, longitude;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

  public:
    inline TYPE_DATA THETA() const {
        return (M_PI / 2.0) - (latitude * M_PI / 180.0); //
    }

    inline TYPE_DATA PHI() const { return (longitude * M_PI / 180.0); }

    inline TYPE_DATA SIN_THETA() const { return std::sin(THETA()); }

    inline TYPE_DATA COS_THETA() const { return std::cos(THETA()); }

    inline TYPE_DATA SIN_PHI() const { return std::sin(PHI()); }

    inline TYPE_DATA COS_PHI() const { return std::cos(PHI()); }

    inline TYPE_DATA X() const { return SIN_THETA() * COS_PHI(); }
    inline TYPE_DATA Y() const { return SIN_THETA() * SIN_PHI(); }
    inline TYPE_DATA Z() const { return COS_THETA(); }

    inline TYPE_CARTITION GET_CARTITION() const {
        if (!IS_VALID()) {
            TYPE_CARTITION ret;
            ret[0] = -9999;
            ret[1] = -9999;
            ret[2] = -9999;
            return ret;
        }
        double const   ct1 = COS_THETA();
        double const   st1 = SIN_THETA();
        double const   cp1 = COS_PHI();
        double const   sp1 = SIN_PHI();
        double const   x1  = st1 * cp1;
        double const   y1  = st1 * sp1;
        double const   z1  = ct1;
        TYPE_CARTITION ret;
        ret[0] = x1;
        ret[1] = y1;
        ret[2] = z1;
        return ret;
    }

    inline TYPE_DATA HaverSineDistance(TYPE_SELF const &other) const {
        if (!IS_VALID()) { return -9999.0; }

        double constexpr r = EarthRadius();

        double const ct1 = COS_THETA();
        double const st1 = SIN_THETA();
        double const cp1 = COS_PHI();
        double const sp1 = SIN_PHI();

        double const ct2 = other.COS_THETA();
        double const st2 = other.SIN_THETA();
        double const cp2 = other.COS_PHI();
        double const sp2 = other.SIN_PHI();

        double const x1 = st1 * cp1;
        double const y1 = st1 * sp1;
        double const z1 = ct1;

        double const x2 = st2 * cp2;
        double const y2 = st2 * sp2;
        double const z2 = ct2;

        double ca = (x1 * x2) + (y1 * y2) + (z1 * z2);
        if (ca > 1) {
            ca = 1;
        } else if (ca < -1) {
            ca = -1;
        }
        double const a = std::acos(ca);

        return CPPFileIO::mymod(r * a);
    }

    inline bool IS_VALID() const {
        return (!std::isnan(latitude)) && (!std::isnan(longitude)) &&
               ((-90.0 <= latitude) && (latitude <= 90.0)) &&
               ((-180.0 <= longitude) && (longitude <= 180.0));
    }

    inline void SET_THETA(TYPE_DATA const in) {
        if ((0 <= in) && (in <= M_PI)) {
            latitude = 90.0 - (in * 180.0 / M_PI);
        } else {
            latitude = -9999.0;
        }
    }

    inline void SET_PHI(TYPE_DATA const in) {
        if ((-M_PI <= in) && (in <= M_PI)) {
            longitude = (in * 180.0 / M_PI);
        } else {
            longitude = -9999.0;
        }
    }

    inline void FROM_CARTITION(TYPE_CARTITION const &in) {

        TYPE_DATA const RT2 = std::pow(in[0], 2) + std::pow(in[1], 2); //
        TYPE_DATA const RT  = std::sqrt(RT2);

        TYPE_DATA const R2 = RT2 + std::pow(in[2], 2); //
        TYPE_DATA const R  = std::sqrt(R2);

        TYPE_DATA theta = -9999;
        if (R != 0) { theta = std::acos(in[2] / R); }
        SET_THETA(theta);

        TYPE_DATA phi = -9999;
        if (RT != 0) {
            phi = std::acos(in[0] / RT);
            if (in[1] < 0) { phi = -phi; }
        }
        SET_PHI(phi);
    }

    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
