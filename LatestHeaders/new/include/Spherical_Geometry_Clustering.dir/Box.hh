#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_Box_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_Box_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "./PolarCoordinates.hh"
///////////////////
// Headers END.} //
///////////////////

///////////////////////
// GPS Boxes BEGIN:{ //
///////////////////////

#define _MACRO_CLASS_NAME_ D2GPS_Box

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT = TF;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;

  public:
    TYPE_POINT MIN, MAX;

  public:
    inline TYPE_POINT P1() const { return MIN; }
    inline TYPE_POINT P2() const {
        TYPE_POINT ret;
        ret.latitude  = MAX.latitude;
        ret.longitude = MIN.longitude;
        return ret;
    }
    inline TYPE_POINT P3() const {
        TYPE_POINT ret;
        ret.latitude  = MIN.latitude;
        ret.longitude = MAX.longitude;
        return ret;
    }
    inline TYPE_POINT P4() const { return MAX; }

  public:
    inline void operator=(TYPE_POINT const in) {
        MIN = in;
        MAX = in;
    }
    inline void operator+=(TYPE_POINT const &in) {
        MIN.latitude  = CPPFileIO::mymin(MIN.latitude, in.latitude);
        MIN.longitude = CPPFileIO::mymin(MIN.longitude, in.longitude);
        MAX.latitude  = CPPFileIO::mymax(MAX.latitude, in.latitude);
        MAX.longitude = CPPFileIO::mymax(MAX.longitude, in.longitude);
    }
    inline TYPE_SELF operator+(TYPE_POINT const &in) const {
        TYPE_SELF ret = this[0];
        ret += in;
        return ret;
    }
    inline bool operator()(TYPE_POINT const &in) const {
        bool const ret =
          (MIN.latitude <= in.latitude) && (in.latitude <= MAX.latitude) &&
          (MIN.longitude <= in.longitude) && (in.longitude <= MAX.longitude);
        return ret;
    }

  public:
    inline void operator=(TYPE_SELF const &other) {
        MIN = other.MIN;
        MAX = other.MAX;
    }
    inline void operator+=(TYPE_SELF const &other) {
        MIN.latitude  = CPPFileIO::mymin(MIN.latitude, other.MIN.latitude);
        MIN.longitude = CPPFileIO::mymin(MIN.longitude, other.MIN.longitude);
        MAX.latitude  = CPPFileIO::mymax(MAX.latitude, other.MAX.latitude);
        MAX.longitude = CPPFileIO::mymax(MAX.longitude, other.MAX.longitude);
    }
    inline TYPE_SELF operator+(TYPE_SELF const &other) const {
        TYPE_SELF ret;
        ret.MIN.latitude = CPPFileIO::mymin(MIN.latitude, other.MIN.latitude);
        ret.MIN.longitude =
          CPPFileIO::mymin(MIN.longitude, other.MIN.longitude);
        ret.MAX.latitude = CPPFileIO::mymax(MAX.latitude, other.MAX.latitude);
        ret.MAX.longitude =
          CPPFileIO::mymax(MAX.longitude, other.MAX.longitude);
        return ret;
    }

  public:
    inline bool operator()(TYPE_SELF const &in) const {
        bool const ret = in(this->P1()) || in(this->P2()) || in(this->P3()) ||
                         in(this->P4()) || this[0](in.P1()) ||
                         this[0](in.P2()) || this[0](in.P3()) ||
                         this[0](in.P4());
        return ret;
    }

  public:
    _MACRO_CLASS_NAME_(TYPE_FLOAT const lat1, TYPE_FLOAT const lon1,
                       TYPE_FLOAT const lat2, TYPE_FLOAT const lon2) {

        MIN.latitude  = CPPFileIO::mymin(lat1, lat2);
        MAX.latitude  = CPPFileIO::mymax(lat1, lat2);
        MIN.longitude = CPPFileIO::mymin(lon1, lon2);
        MAX.longitude = CPPFileIO::mymax(lon1, lon2);
    }

    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_

/////////////////////
// GPS Boxes END.} //
/////////////////////

#endif
