#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_Threading_Treshhold_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_Threading_Treshhold_

template <typename T> inline T constexpr Threading_Treshhold(T const in) {
    T constexpr Treshhold = 256;
    if (in > Treshhold) {
        return Treshhold;
    } else {
        return in;
    }
}

template <typename T> inline T constexpr Threading_Treshhold_2(T const in) {
    T constexpr Treshhold = 256 * 256;
    if (in > Treshhold) {
        return Treshhold;
    } else {
        return in;
    }
}

#endif
