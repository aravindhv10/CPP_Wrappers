#ifndef _HEADER_GUARD_ParseLineString_
#define _HEADER_GUARD_ParseLineString_

/////////////////////////////
// Headers Related BEGIN:{ //
/////////////////////////////
#include "./Spherical_Geometry_Clustering.hh"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
///////////////////////////
// Headers Related END.} //
///////////////////////////

////////////////////////////
// Main Functions BEGIN:{ //
////////////////////////////

inline bool Is_Valid_Line_String(char const *a) {
    char const *res = strstr(a, "LINESTRING");
    if (res == NULL) { return false; }
    long count_open = 0;
    while ((*a) != 0) {
        switch (*a) {
            case '(': count_open++; break;
            case ')': count_open--; break;
        }
        if (count_open < 0) { return false; }
        a++;
    }
    if (count_open == 0) {
        return true;
    } else {
        return false;
    }
}

template <typename TF = double, typename TI = long>
inline std::vector<Spherical_Geometry_Clustering::D2GPS_Coordinates<TF, TI>>
Parse_Line_String(char const *in) {

    typedef boost::geometry::model::d2::point_xy<double> point_type;
    using TYPE_COORDINATE =
      Spherical_Geometry_Clustering::D2GPS_Coordinates<TF, TI>;
    using TYPE_COORDINATES = std::vector<TYPE_COORDINATE>;

    TYPE_COORDINATES ret;

    if (Is_Valid_Line_String(in)) {
        boost::geometry::model::linestring<point_type> b;
        boost::geometry::read_wkt(in, b);
        ret.reserve(b.end() - b.begin());
        for (auto a = b.begin(); a != b.end(); a++) {
            TYPE_COORDINATE tmp;
            tmp.longitude = a->get<0>();
            tmp.latitude  = a->get<1>();
            ret.push_back(tmp);
        }
    }

    return ret;
}

//////////////////////////
// Main Functions END.} //
//////////////////////////

#endif
