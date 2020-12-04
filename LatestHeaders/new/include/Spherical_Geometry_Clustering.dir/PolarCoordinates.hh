#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_PolarCoordinates_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_PolarCoordinates_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../StaticArray.hh"
#include "./Simple_DBSCAN.hh"
#include "./Simple_KDE.hh"
#include "./Threading_Treshhold.hh"
///////////////////
// Headers END.} //
///////////////////

/////////////////////////////
// GPS Coordinates BEGIN:{ //
/////////////////////////////

#define _MACRO_CLASS_NAME_ D2GPS_Coordinates

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_FLOAT     = TF;
    using TYPE_INT       = TI;
    using TYPE_SELF      = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR      = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;
    using TYPE_CARTITION = StaticArray::ND_ARRAY<3, TYPE_FLOAT>;
    using TYPE_UINT      = unsigned int;
    using TYPE_ULONG     = unsigned long;

    inline static TYPE_FLOAT constexpr PI() { return M_PI; }
    inline static TYPE_FLOAT constexpr PIB2() { return M_PI / 2.0; }
    inline static TYPE_FLOAT constexpr EarthRadius() { return 6367000; }
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  public:
    TYPE_FLOAT latitude, longitude;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////////////////
    // Dealing with pairs BEGIN:{ //
    ////////////////////////////////
  public:
    inline TYPE_PAIR &AS_PAIR() {
        return reinterpret_cast<TYPE_PAIR &>(this[0]);
    }

    inline TYPE_PAIR const &AS_PAIR() const {
        return reinterpret_cast<TYPE_PAIR const &>(this[0]);
    }

    inline void operator=(TYPE_PAIR const &in) { AS_PAIR() = in; }
    //////////////////////////////
    // Dealing with pairs END.} //
    //////////////////////////////

    /////////////////////////////////////////
    // Spherical polar coordinates BEGIN:{ //
    /////////////////////////////////////////
  public:
    inline TYPE_FLOAT THETA() const {
        return (M_PI / 2.0) - (latitude * M_PI / 180.0);
    }

    inline TYPE_FLOAT PHI() const { return (longitude * M_PI / 180.0); }

    inline TYPE_FLOAT SIN_THETA() const { return std::sin(THETA()); }
    inline TYPE_FLOAT COS_THETA() const { return std::cos(THETA()); }
    inline TYPE_FLOAT SIN_PHI() const { return std::sin(PHI()); }
    inline TYPE_FLOAT COS_PHI() const { return std::cos(PHI()); }

    inline void SET_THETA(TYPE_FLOAT const in) {
        if ((0 <= in) && (in <= M_PI)) {
            latitude = 90.0 - (in * 180.0 / M_PI);
        } else {
            latitude = -9999.0;
        }
    }

    inline void SET_PHI(TYPE_FLOAT const in) {
        if ((-M_PI <= in) && (in <= M_PI)) {
            longitude = (in * 180.0 / M_PI);
        } else {
            longitude = -9999.0;
        }
    }
    ///////////////////////////////////////
    // Spherical polar coordinates END.} //
    ///////////////////////////////////////

    //////////////////////////////////////////
    // Carition coordinates related BEGIN:{ //
    //////////////////////////////////////////
  public:
    inline TYPE_FLOAT X() const { return SIN_THETA() * COS_PHI(); }
    inline TYPE_FLOAT Y() const { return SIN_THETA() * SIN_PHI(); }
    inline TYPE_FLOAT Z() const { return COS_THETA(); }

    inline TYPE_CARTITION GET_CARTITION() const {
        if (!IS_VALID()) {
            TYPE_CARTITION ret;
            ret[0] = -9999;
            ret[1] = -9999;
            ret[2] = -9999;
            return ret;
        }
        TYPE_FLOAT const ct1 = COS_THETA();
        TYPE_FLOAT const st1 = SIN_THETA();
        TYPE_FLOAT const cp1 = COS_PHI();
        TYPE_FLOAT const sp1 = SIN_PHI();
        TYPE_FLOAT const x1  = st1 * cp1;
        TYPE_FLOAT const y1  = st1 * sp1;
        TYPE_FLOAT const z1  = ct1;
        TYPE_CARTITION   ret;
        ret[0] = x1;
        ret[1] = y1;
        ret[2] = z1;
        return ret;
    }

    inline void FROM_CARTITION(TYPE_CARTITION const &in) {

        TYPE_FLOAT const RT2 = std::pow(in[0], 2) + std::pow(in[1], 2);
        TYPE_FLOAT const RT  = std::sqrt(RT2);

        TYPE_FLOAT const R2 = RT2 + std::pow(in[2], 2);
        TYPE_FLOAT const R  = std::sqrt(R2);

        TYPE_FLOAT theta = -9999;
        if (R != 0) { theta = std::acos(in[2] / R); }
        SET_THETA(theta);

        TYPE_FLOAT phi = -9999;
        if (RT != 0) {
            phi = std::acos(in[0] / RT);
            if (in[1] < 0) { phi = -phi; }
        }
        SET_PHI(phi);
    }
    ////////////////////////////////////////
    // Carition coordinates related END.} //
    ////////////////////////////////////////

    ////////////////////////////
    // Main Functions BEGIN:{ //
    ////////////////////////////
  public:
    inline TYPE_FLOAT HaverSineDistance(TYPE_SELF const &other) const {
        if (!IS_VALID()) { return -9999.0; }

        TYPE_FLOAT constexpr r = EarthRadius();

        TYPE_FLOAT const ct1 = COS_THETA();
        TYPE_FLOAT const st1 = SIN_THETA();
        TYPE_FLOAT const cp1 = COS_PHI();
        TYPE_FLOAT const sp1 = SIN_PHI();

        TYPE_FLOAT const ct2 = other.COS_THETA();
        TYPE_FLOAT const st2 = other.SIN_THETA();
        TYPE_FLOAT const cp2 = other.COS_PHI();
        TYPE_FLOAT const sp2 = other.SIN_PHI();

        TYPE_FLOAT const x1 = st1 * cp1;
        TYPE_FLOAT const y1 = st1 * sp1;
        TYPE_FLOAT const z1 = ct1;

        TYPE_FLOAT const x2 = st2 * cp2;
        TYPE_FLOAT const y2 = st2 * sp2;
        TYPE_FLOAT const z2 = ct2;

        TYPE_FLOAT ca = (x1 * x2) + (y1 * y2) + (z1 * z2);
        if (ca > 1) {
            ca = 1;
        } else if (ca < -1) {
            ca = -1;
        }
        TYPE_FLOAT const a = std::acos(ca);

        return CPPFileIO::mymod(r * a);
    }

    inline bool IS_VALID() const {
        return (!std::isnan(latitude)) && (!std::isnan(longitude)) &&
               ((-90.0 <= latitude) && (latitude <= 90.0)) &&
               ((-180.0 <= longitude) && (longitude <= 180.0));
    }

    inline TYPE_UINT uint_longitude() const {
        CPPFileIO::BinMapper<TYPE_INT, TYPE_FLOAT> mapper;
        mapper(CPPFileIO::MAX_UINT(), -180.0, 180.0);
        TYPE_UINT const ret = mapper(longitude);
        return ret;
    }

    inline TYPE_UINT uint_latitude() const {
        CPPFileIO::BinMapper<TYPE_INT, TYPE_FLOAT> mapper;
        mapper(CPPFileIO::MAX_UINT(), -90.0, 90.0);
        TYPE_UINT const ret = mapper(latitude);
        return ret;
    }

    inline TYPE_ULONG z_curve() const {
        TYPE_UINT const lon = uint_longitude();
        TYPE_UINT const lat = uint_latitude();
        TYPE_ULONG      res = 0;
        for (TYPE_INT i = 0; i < 32; i++) {
            TYPE_UINT const  val     = (1 << i);
            TYPE_ULONG const out_lat = static_cast<TYPE_ULONG>(lat & val);
            TYPE_ULONG const out_lon =
              (static_cast<TYPE_ULONG>(lon & val) << 1);
            TYPE_ULONG const sum = ((out_lat + out_lon) << i);
            res += sum;
        }
        return res;
    }
    //////////////////////////
    // Main Functions END.} //
    //////////////////////////

    ////////////////////////
    // Convinence BEGIN:{ //
    ////////////////////////
  public:
    inline TYPE_FLOAT operator()(TYPE_SELF const &in) const {
        return this->HaverSineDistance(in);
    }

    _MACRO_CLASS_NAME_(TYPE_PAIR const &in) { this[0] = in; }

    _MACRO_CLASS_NAME_() {}

    ~_MACRO_CLASS_NAME_() {}
    //////////////////////
    // Convinence END.} //
    //////////////////////
};

#undef _MACRO_CLASS_NAME_

///////////////////////////
// GPS Coordinates END.} //
///////////////////////////

/////////////////////////////////
// Distance Evaluation BEGIN:{ //
/////////////////////////////////

#define _MACRO_CLASS_NAME_ VectorHaversineDistance

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {

    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_FLOAT    = TF;
    using TYPE_INT      = TI;
    using TYPE_SELF     = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ARRAY    = CPPFileIO::Dynamic1DArray<TYPE_FLOAT, TYPE_INT>;
    using TYPE_OUTPUTS  = CPPFileIO::SymmetricMatrix<TYPE_FLOAT, TYPE_INT>;
    using TYPE_KDE      = Simple_KDE<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENT  = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENTS = std::vector<TYPE_ELEMENT>;
    using TYPE_INPUTS = CPPFileIO::Dynamic1DArray<TYPE_ELEMENT const, TYPE_INT>;
    using TYPE_VALIDITY = CPPFileIO::Dynamic1DArray<bool, TYPE_INT>;

    struct TYPE_KDE_RET {
        TYPE_ELEMENT place;
        TYPE_FLOAT   score;
    };
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_INPUTS  INPUTS;
    TYPE_OUTPUTS OUTPUTS;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ///////////////////////////////////
    // Convinence / Wrappers BEGIN:{ //
    ///////////////////////////////////
  private:
    inline bool IS_VALID(TYPE_INT const i) const {
        return INPUTS(i).IS_VALID();
    }

    static inline TYPE_FLOAT CLAMP(TYPE_FLOAT const in) {
        bool const val1 = (1 < in);
        bool const val2 = (in < -1);
        bool const val3 = (!val1) && (!val2);
        return val1 - val2 + (in * val3);
    }
    /////////////////////////////////
    // Convinence / Wrappers END.} //
    /////////////////////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline void EVAL_OUTPUTS() {
        TYPE_FLOAT constexpr r = TYPE_ELEMENT::EarthRadius();

        TYPE_INT const limit        = INPUTS();
        TYPE_INT const loop_limit   = Threading_Treshhold(limit);
        TYPE_INT const loop_limit_2 = Threading_Treshhold_2(limit);

        TYPE_VALIDITY VALIDITY(INPUTS());
        for (TYPE_INT i = 0; i < limit; i++) {
            VALIDITY(i) = INPUTS(i).IS_VALID();
        }

        TYPE_ARRAY sin_theta(INPUTS());
        for (TYPE_INT i = 0; i < limit; i++) {
            sin_theta(i) = INPUTS(i).SIN_THETA();
        }

        TYPE_ARRAY cos_theta(INPUTS());
        for (TYPE_INT i = 0; i < limit; i++) {
            cos_theta(i) = INPUTS(i).COS_THETA();
        }

        TYPE_ARRAY sin_phi(INPUTS());
        for (TYPE_INT i = 0; i < limit; i++) {
            sin_phi(i) = INPUTS(i).SIN_PHI();
        }

        TYPE_ARRAY cos_phi(INPUTS());
        for (TYPE_INT i = 0; i < limit; i++) {
            cos_phi(i) = INPUTS(i).COS_PHI();
        }

#define _MACRO_X_(i) (sin_theta(i) * cos_phi(i))
#define _MACRO_Y_(i) (sin_theta(i) * sin_phi(i))
#define _MACRO_Z_(i) (cos_theta(i))

#define _MACRO_DOT_(i, j)                                                      \
    (_MACRO_X_(i) * _MACRO_X_(j)) + (_MACRO_Y_(i) * _MACRO_Y_(j)) +            \
      (_MACRO_Z_(i) * _MACRO_Z_(j))

#define DO_WORK                                                                \
    TYPE_FLOAT *outputs = &(OUTPUTS(y, 0));                                    \
    for (TYPE_INT x = 0; x < y; x++) { outputs[x] = CLAMP(_MACRO_DOT_(y, x)); }

        for (TYPE_INT y = 1; y < loop_limit; y++) { DO_WORK }
#pragma omp parallel for
        for (TYPE_INT y = loop_limit; y < limit; y++) { DO_WORK }

#undef DO_WORK

#undef _MACRO_DOT_
#undef _MACRO_Z_
#undef _MACRO_Y_
#undef _MACRO_X_

        /* Evaluate the arc length: */ {
            auto &         tmp   = OUTPUTS();
            TYPE_INT const total = tmp();
            if (total > loop_limit_2) {
#pragma omp parallel for
                for (TYPE_INT i = 0; i < total; i++) {
                    tmp(i) = std::acos(tmp(i)) * r;
                }
            } else {
                for (TYPE_INT i = 0; i < total; i++) {
                    tmp(i) = std::acos(tmp(i)) * r;
                }
            }
        }

#define DO_WORK                                                                \
    TYPE_FLOAT *outputs = &(OUTPUTS(y, 0));                                    \
    if (VALIDITY(y)) {                                                         \
        for (TYPE_INT x = 0; x < y; x++) {                                     \
            if (VALIDITY(x)) {                                                 \
                outputs[x] = CPPFileIO::mymod(outputs[x]);                     \
            } else {                                                           \
                outputs[x] = -9999;                                            \
            }                                                                  \
        }                                                                      \
    } else {                                                                   \
        for (TYPE_INT x = 0; x < y; x++) { outputs[x] = -9999; }               \
    }

        for (TYPE_INT y = 1; y < loop_limit; y++) { DO_WORK }
#pragma omp paralllel for
        for (TYPE_INT y = loop_limit; y < limit; y++) { DO_WORK }

#undef DO_WORK

        for (TYPE_INT y = 0; y < limit; y++) {
            if (VALIDITY(y)) {
                OUTPUTS(y, y) = 0;
            } else {
                OUTPUTS(y, y) = -9999;
            }
        }
    }

  public:
    inline TYPE_OUTPUTS const &get_distances() const { return OUTPUTS; }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    ////////////////////////////////
    // Convinent wrappers BEGIN:{ //
    ////////////////////////////////
  public:
    inline TYPE_OUTPUTS const &operator()() const { return get_distances(); }

    inline TYPE_KDE_RET const find_kde_center(TYPE_FLOAT const bandwidth) {
        auto const   res = TYPE_KDE::find_kde_center(OUTPUTS, bandwidth);
        TYPE_KDE_RET ret = {INPUTS(res.idx), res.val};
        return ret;
    }

    inline void find_dbscan_clusters(std::vector<TYPE_ELEMENTS> &in,
                                     TYPE_FLOAT const            eps,
                                     TYPE_INT const              min_pts) {

        in.clear();
        Simple_DBSCAN<TYPE_FLOAT, TYPE_INT> dbscan(OUTPUTS, eps, min_pts);
        TYPE_INT const num_clusters = dbscan.get_num_clusters();
        if (num_clusters > 0) {
            in.resize(num_clusters);
            auto const &element_clusters = dbscan.get_element_cluster();
            for (TYPE_INT i = 0; i < element_clusters(); i++) {
                TYPE_INT const cluster_idx = element_clusters(i);
                if ((cluster_idx >= 0) && (INPUTS(i).IS_VALID())) {
                    in[cluster_idx].push_back(INPUTS(i));
                }
            }
        }
    }

    using TYPE_DBSCAN_RET = std::vector<TYPE_KDE_RET>;
    inline void find_dbscan_kde_centers(TYPE_DBSCAN_RET &in,
                                        TYPE_FLOAT const eps,
                                        TYPE_INT const   min_pts,
                                        TYPE_FLOAT const bandwidth) {

        in.clear();
        std::vector<TYPE_ELEMENTS> clusters;
        find_dbscan_clusters(clusters, eps, min_pts);
        for (TYPE_INT i = 0; i < clusters.size(); i++) {
            TYPE_SELF  tmp(clusters[i]);
            auto const res = tmp.find_kde_center(bandwidth);
            in.push_back(res);
        }
    }
    //////////////////////////////
    // Convinent wrappers END.} //
    //////////////////////////////

    ////////////////////////////////////////
    // Constructors & destructors BEGIN:{ //
    ////////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_ELEMENT const *inputs, TYPE_INT const n)
      : INPUTS(inputs, n), OUTPUTS(INPUTS()) {
        EVAL_OUTPUTS();
    }

    _MACRO_CLASS_NAME_(std::vector<TYPE_ELEMENT> const &inputs)
      : INPUTS(&(inputs[0]), inputs.size()), OUTPUTS(INPUTS()) {
        EVAL_OUTPUTS();
    }

    _MACRO_CLASS_NAME_(TYPE_INPUTS &inputs)
      : INPUTS(inputs.GET_DATA(), inputs()), OUTPUTS(inputs()) {
        EVAL_OUTPUTS();
    }

    ~_MACRO_CLASS_NAME_() {}
    //////////////////////////////////////
    // Constructors & destructors END.} //
    //////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

///////////////////////////////
// Distance Evaluation END.} //
///////////////////////////////

#define _MACRO_CLASS_NAME_ VectorHaversineDistance_2Sets

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {

    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_FLOAT    = TF;
    using TYPE_INT      = TI;
    using TYPE_SELF     = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENT  = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENTS = std::vector<TYPE_ELEMENT>;
    using TYPE_INPUTS = CPPFileIO::Dynamic1DArray<TYPE_ELEMENT const, TYPE_INT>;
    using TYPE_ARRAY  = CPPFileIO::Dynamic1DArray<TYPE_FLOAT, TYPE_INT>;
    using TYPE_OUTPUTS  = CPPFileIO::Dynamic2DArray<TYPE_FLOAT, TYPE_INT>;
    using TYPE_VALIDITY = CPPFileIO::Dynamic1DArray<bool, TYPE_INT>;

    static inline TYPE_FLOAT CLAMP(TYPE_FLOAT const in) {
        bool const val1 = (1 < in);
        bool const val2 = (in < -1);
        bool const val3 = (!val1) && (!val2);
        return val1 - val2 + (in * val3);
    }
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

  private:
    TYPE_INPUTS  INPUTS1, INPUTS2;
    TYPE_OUTPUTS OUTPUTS;

  public:
    inline TYPE_OUTPUTS const &get_distances() const { return OUTPUTS; }
    inline TYPE_OUTPUTS const &operator()() const { return get_distances(); }

    inline void EVAL_OUTPUTS() {
        TYPE_FLOAT constexpr r = TYPE_ELEMENT::EarthRadius();

        TYPE_INT const limit1 = INPUTS1();
        TYPE_INT const limit2 = INPUTS2();
        TYPE_VALIDITY  validity1(limit1);
        TYPE_VALIDITY  validity2(limit2);
        TYPE_ARRAY     x1(limit1);
        TYPE_ARRAY     x2(limit2);
        TYPE_ARRAY     y1(limit1);
        TYPE_ARRAY     y2(limit2);
        TYPE_ARRAY     z1(limit1);
        TYPE_ARRAY     z2(limit2);

        /* Evaluate these arrays: */ {
            for (TYPE_INT th = 0; th < 2; th++) {
                switch (th) {
                    case 0:
                        /* Evaluate the 1st array: */ {
#pragma omp parallel for
                            for (TYPE_INT i = 0; i < limit1; i++) {
                                bool const valid = INPUTS1(i).IS_VALID();
                                validity1(i)     = valid;
                                if (valid) {
                                    TYPE_FLOAT sin_theta =
                                      INPUTS1(i).SIN_THETA();
                                    TYPE_FLOAT cos_theta =
                                      INPUTS1(i).COS_THETA();
                                    TYPE_FLOAT sin_phi = INPUTS1(i).SIN_PHI();
                                    TYPE_FLOAT cos_phi = INPUTS1(i).COS_PHI();
                                    x1(i)              = sin_theta * cos_phi;
                                    y1(i)              = sin_theta * sin_phi;
                                    z1(i)              = cos_theta;
                                }
                            }
                        }
                        break;
                    case 1:
                        /* Evaluate the 2nd array: */ {
#pragma omp parallel for
                            for (TYPE_INT i = 0; i < limit2; i++) {
                                bool const valid = INPUTS2(i).IS_VALID();
                                validity2(i)     = valid;
                                if (valid) {
                                    TYPE_FLOAT sin_theta =
                                      INPUTS2(i).SIN_THETA();
                                    TYPE_FLOAT cos_theta =
                                      INPUTS2(i).COS_THETA();
                                    TYPE_FLOAT sin_phi = INPUTS2(i).SIN_PHI();
                                    TYPE_FLOAT cos_phi = INPUTS2(i).COS_PHI();
                                    x2(i)              = sin_theta * cos_phi;
                                    y2(i)              = sin_theta * sin_phi;
                                    z2(i)              = cos_theta;
                                }
                            }
                        }
                        break;
                }
            }
        }

#define DOT(i, j) (x1(i) * x2(j)) + (y1(i) * y2(j)) + (z1(i) * z2(j))

#pragma omp parallel for
        for (TYPE_INT i = 0; i < limit1; i++) {
            for (TYPE_INT j = 0; j < limit2; j++) {
                OUTPUTS(i, j) = CLAMP(DOT(i, j));
            }
        }

#undef DOT

#pragma omp parallel for
        for (TYPE_INT i = 0; i < limit1; i++) {
            for (TYPE_INT j = 0; j < limit2; j++) {
                bool const valid = validity1(i) && validity2(j);
                if (valid) {
                    OUTPUTS(i, j) = std::acos(OUTPUTS(i, j)) * r;
                } else {
                    OUTPUTS(i, j) = -9999;
                }
            }
        }
    }

    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    ////////////////////////////////////////
    // Constructors & destructors BEGIN:{ //
    ////////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_ELEMENT const *inputs1, TYPE_INT const n1,
                       TYPE_ELEMENT const *inputs2, TYPE_INT const n2)
      : INPUTS1(inputs1, n1), INPUTS2(inputs2, n2),
        OUTPUTS(INPUTS1(), INPUTS2()) {
        EVAL_OUTPUTS();
    }

    _MACRO_CLASS_NAME_(std::vector<TYPE_ELEMENT> const &inputs1,
                       std::vector<TYPE_ELEMENT> const &inputs2)
      : INPUTS1(&(inputs1[0]), inputs1.size()),
        INPUTS2(&(inputs2[0]), inputs2.size()), OUTPUTS(INPUTS1(), INPUTS2()) {
        EVAL_OUTPUTS();
    }

    _MACRO_CLASS_NAME_(TYPE_INPUTS &inputs1, TYPE_INPUTS &inputs2)
      : INPUTS1(inputs1.GET_DATA(), inputs1()),
        INPUTS2(inputs2.GET_DATA(), inputs2()), OUTPUTS(INPUTS1(), INPUTS2()) {
        EVAL_OUTPUTS();
    }

    ~_MACRO_CLASS_NAME_() {}
    //////////////////////////////////////
    // Constructors & destructors END.} //
    //////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
