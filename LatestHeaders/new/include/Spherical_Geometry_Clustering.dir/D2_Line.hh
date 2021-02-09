#include "./PolarCoordinates.hh"

#define _MACRO_CLASS_NAME_ D2GPS_Line
template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT = TF;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;

  public:
    TYPE_POINT P1, P2;

  public:
    inline TYPE_POINT get_perpendicular(TYPE_POINT const in) const {
        TYPE_POINT const p1_p2 = {P2.latitude - P1.latitude,
                                  P2.longitude - P1.longitude};
        TYPE_FLOAT const mag_2_p1_p2 =
          std::pow(p1_p2.latitude, 2) + std::pow(p1_p2.longitude, 2);
        TYPE_FLOAT const mag_p1_p2  = std::sqrt(mag_2_p1_p2);
        TYPE_POINT const unit_p1_p2 = {p1_p2.latitude / mag_p1_p2,
                                       p1_p2.longitude / mag_p1_p2};

        TYPE_POINT const in_p1 = {P1.latitude - in.latitude,
                                  P1.longitude - in.longitude};
        TYPE_FLOAT const dot   = (unit_p1_p2.latitude * in_p1.latitude) +
                               (unit_p1_p2.longitude * in_p1.longitude);
        TYPE_POINT const along = {unit_p1_p2.latitude * dot,
                                  unit_p1_p2.longitude * dot};
        TYPE_POINT const perp  = {in.latitude + along.latitude,
                                 in.longitude + along.longitude};
        return perp;
    }

    inline TYPE_POINT get_perpendicular_distance(TYPE_POINT const in) const {
        auto const       perp = get_perpendicular(in);
        TYPE_FLOAT const mag_perp_2 =
          std::pow(perp.latitude, 2) + std::pow(perp.longitude, 2);
        TYPE_FLOAT const mag_perp = std::sqrt(mag_perp_2);
        return mag_perp;
    }

  public:
    _MACRO_CLASS_NAME_() {}

    _MACRO_CLASS_NAME_(TYPE_POINT const p1, TYPE_POINT const p2)
      : P1(p1), P2(p2) {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_
