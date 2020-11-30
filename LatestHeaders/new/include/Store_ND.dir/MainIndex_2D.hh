#ifndef _HEADER_GUARD_Store_ND_MainIndex_2D_
#define _HEADER_GUARD_Store_ND_MainIndex_2D_

#include "./BinMapper.hh"

#define _MACRO_CLASS_NAME_ MainIndex_2D
template <typename TI = long, typename TF = double, TI ny = 10000,
          TI nx = 10000>
class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_BIN_MAPPER = bin_mapper<TYPE_INT, TYPE_FLOAT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;

    static inline TYPE_INT constexpr SIZE_Y() { return ny; }
    static inline TYPE_INT constexpr SIZE_X() { return nx; }
    static inline TYPE_INT constexpr SIZE() { return SIZE_X() * SIZE_Y(); }

    using TYPE_SELF =
      _MACRO_CLASS_NAME_<TYPE_INT, TYPE_FLOAT, SIZE_Y(), SIZE_X()>;

    using TYPE_COUNTS     = StaticArray::ND_ARRAY<SIZE(), TYPE_INT>;
    using TYPE_CUMULATIVE = StaticArray::ND_ARRAY<SIZE() + 1, TYPE_INT>;

    static inline TYPE_INT constexpr FLATTEN_MAP(TYPE_INT const y,
                                                 TYPE_INT const x) {
        return x + (y * SIZE_X());
    }
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ////////////////////////////////////
    // Mapping float to int X BEGIN:{ //
    ////////////////////////////////////
  public:
    TYPE_BIN_MAPPER BIN_X;
    inline void     SET_X_RANGE(TYPE_FLOAT const x1, TYPE_FLOAT const x2) {
        BIN_X.SET_RANGES(SIZE_X(), x1, x2);
    }
    //////////////////////////////////
    // Mapping float to int X END.} //
    //////////////////////////////////

    ////////////////////////////////////
    // Mapping float to int Y BEGIN:{ //
    ////////////////////////////////////
  public:
    TYPE_BIN_MAPPER BIN_Y;
    inline void     SET_Y_RANGE(TYPE_FLOAT const y1, TYPE_FLOAT const y2) {
        BIN_Y.SET_RANGES(SIZE_Y(), y1, y2);
    }
    //////////////////////////////////
    // Mapping float to int Y END.} //
    //////////////////////////////////

    ////////////////////////////////
    // Mapping to 2D Bins BEGIN:{ //
    ////////////////////////////////
  public:
    inline void SET_RANGE(TYPE_FLOAT const y1, TYPE_FLOAT const x1,
                          TYPE_FLOAT const y2, TYPE_FLOAT const x2) {
        SET_Y_RANGE(y1, y2);
        SET_X_RANGE(x1, x2);
    }

    inline TYPE_PAIR_INT BINS_FLOAT_TO_INT(TYPE_FLOAT const y,
                                           TYPE_FLOAT const x) const {
        TYPE_PAIR_INT ret;
        ret[0] = BIN_Y(y);
        ret[1] = BIN_X(x);
        return ret;
    }

    inline TYPE_INT FLATTEN_MAP(TYPE_FLOAT const y, TYPE_FLOAT const x) const {
        auto const res = BINS_FLOAT_TO_INT(y, x);
        return FLATTEN_MAP(res[0], res[1]);
    }
    //////////////////////////////
    // Mapping to 2D Bins END.} //
    //////////////////////////////

    ////////////////////////////
    // Counts Related BEGIN:{ //
    ////////////////////////////
  public:
    TYPE_COUNTS COUNTS;

    inline void ZERO_COUNTS() { COUNTS = 0; }

    inline TYPE_INT &GET_COUNTS(TYPE_INT const y, TYPE_INT const x) {
        return COUNTS[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT const &GET_COUNTS(TYPE_INT const y,
                                      TYPE_INT const x) const {
        return COUNTS[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT &GET_COUNTS(TYPE_FLOAT const y, TYPE_FLOAT const x) {
        return COUNTS[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT const &GET_COUNTS(TYPE_FLOAT const y,
                                      TYPE_FLOAT const x) const {
        return COUNTS[FLATTEN_MAP(y, x)];
    }
    //////////////////////////
    // Counts Related END.} //
    //////////////////////////

    ////////////////////////////////
    // CUMULATIVE Related BEGIN:{ //
    ////////////////////////////////
  public:
    TYPE_CUMULATIVE CUMULATIVE;

    inline void EVAL_CUMULATIVE() {
        CUMULATIVE[0] = 0;
        for (size_t i = 0; i < COUNTS.SIZE(); i++) {
            CUMULATIVE[i + 1] = COUNTS[i] + CUMULATIVE[i];
        }
    }

    inline TYPE_INT &GET_CUMULATIVE(TYPE_INT const y, TYPE_INT const x) {
        return CUMULATIVE[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT const &GET_CUMULATIVE(TYPE_INT const y,
                                          TYPE_INT const x) const {
        return CUMULATIVE[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT &GET_CUMULATIVE(TYPE_FLOAT const y, TYPE_FLOAT const x) {
        return CUMULATIVE[FLATTEN_MAP(y, x)];
    }

    inline TYPE_INT const &GET_CUMULATIVE(TYPE_FLOAT const y,
                                          TYPE_FLOAT const x) const {
        return CUMULATIVE[FLATTEN_MAP(y, x)];
    }
    //////////////////////////////
    // CUMULATIVE Related END.} //
    //////////////////////////////

    //////////////////////////////
    // LOCATION Related BEGIN:{ //
    //////////////////////////////
  public:
    inline TYPE_INT const GET_LOCATION(TYPE_INT const y,
                                       TYPE_INT const x) const {
        auto const bin = FLATTEN_MAP(y, x);
        return CUMULATIVE[bin] + COUNTS[bin];
    }

    inline TYPE_INT const GET_LOCATION(TYPE_FLOAT const y,
                                       TYPE_FLOAT const x) const {
        auto const bin = FLATTEN_MAP(y, x);
        return CUMULATIVE[bin] + COUNTS[bin];
    }

    inline TYPE_INT const ADD_LOCATION(TYPE_INT const y, TYPE_INT const x) {
        auto const bin = FLATTEN_MAP(y, x);
        auto const ret = CUMULATIVE[bin] + COUNTS[bin];
        COUNTS[bin]++;
        return ret;
    }

    inline TYPE_INT const ADD_LOCATION(TYPE_FLOAT const y, TYPE_FLOAT const x) {
        auto const bin = FLATTEN_MAP(y, x);
        auto const ret = CUMULATIVE[bin] + COUNTS[bin];
        COUNTS[bin]++;
        return ret;
    }
    ////////////////////////////
    // LOCATION Related END.} //
    ////////////////////////////

    ////////////////////////////////////////
    // Constructor and Destructor BEGIN:{ //
    ////////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
    //////////////////////////////////////
    // Constructor and Destructor END.} //
    //////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
