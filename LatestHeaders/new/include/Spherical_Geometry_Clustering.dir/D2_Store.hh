#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_

#include "./PolarCoordinates.hh"

//////////////////////////
// Storage node BEGIN:{ //
//////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Node
template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT      = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;

    static TYPE_INT constexpr LENGTH_LIMIT() { return 128; }

  public:
    TYPE_BOX      BBOX;
    TYPE_PAIR_INT RANGE;

  public:
    inline TYPE_INT LENGTH() const { return (RANGE[1] - RANGE[0] + 1); }
    inline bool IS_LEAF() const { return (LENGTH() < LENGTH_LIMIT()); }

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_
////////////////////////
// Storage node END.} //
////////////////////////

//////////////////////////////////
// The main store class BEGIN:{ //
//////////////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Store

template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT = TF;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_
////////////////////////////////
// The main store class END.} //
////////////////////////////////

#endif
