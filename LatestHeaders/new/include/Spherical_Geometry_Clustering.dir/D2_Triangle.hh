#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Triangle_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Triangle_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../StaticArray.hh"
#include "./PolarCoordinates.hh"
#include "./D2_Vector.hh"
///////////////////
// Headers END.} //
///////////////////

/////////////////////////////
// GPS Coordinates BEGIN:{ //
/////////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Triangle
template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_COORDINATE = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR       = StaticArray::ND_ARRAY<2, TYPE_COORDINATE>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    TYPE_COORDINATE START;
    TYPE_COORDINATE END;
    TYPE_COORDINATE VEC;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////////////////////
    // Main Working functions BEGIN:{ //
    ////////////////////////////////////
  public:
    inline void assign(TYPE_COORDINATE const start, TYPE_COORDINATE const end) {
        START         = start;
        END           = end;
        VEC.latitude  = END.latitude - START.latitude;
        VEC.longitude = END.longitude - START.longitude;
    }

    inline TYPE_FLOAT dot(TYPE_SELF const &other) const {
        return (VEC.latitude * other.VEC.latitude) +
               (VEC.longitude * other.VEC.longitude);
    }

    inline TYPE_FLOAT cross(TYPE_SELF const &other) const {
        return (VEC.latitude * other.VEC.longitude) -
               (VEC.longitude * other.VEC.latitude);
    }

    static inline char orientation(TYPE_COORDINATE const a,
                                   TYPE_COORDINATE const b,
                                   TYPE_COORDINATE const c) {
        TYPE_SELF  ab(a, b);
        TYPE_SELF  ac(a, c);
        TYPE_FLOAT res = ab.cross(ac);
        return (res > 0) - (res < 0);
    }

    inline bool intersects(TYPE_SELF const &other) const {
        char const val[4] = {orientation(START, END, other.START),
                             orientation(START, END, other.END),
                             orientation(other.START, other.END, START),
                             orientation(other.START, other.END, END)};

        bool const res[2] = { ((val[0]*val[1])<0) , ((val[2]*val[3])<0) } ;
        return res[0] && res[1];
    }

    inline void extend (TYPE_FLOAT const in=2.0) {
      END.latitude = START.latitude + (VEC.latitude*in) ;
      END.longitude = START.longitude + (VEC.longitude*in) ;
    }

    inline TYPE_FLOAT distance () const {
      return START.HaverSineDistance(END);
    }
    //////////////////////////////////
    // Main Working functions END.} //
    //////////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
    _MACRO_CLASS_NAME_(TYPE_COORDINATE const a, TYPE_COORDINATE const b) {
        assign(a, b);
    }

    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_
///////////////////////////
// GPS Coordinates END.} //
///////////////////////////

#endif
