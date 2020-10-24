#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_Simple_KDE_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_Simple_KDE_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../CPPFileIO.hh"
#include "./Threading_Treshhold.hh"
///////////////////
// Headers END.} //
///////////////////

#define _MACRO_CLASS_NAME_ Simple_KDE

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_FLOAT       = TF;
    using TYPE_INT         = TI;
    using TYPE_DISTANCES   = CPPFileIO::SymmetricMatrix<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ACCUMULATOR = CPPFileIO::Dynamic1DArray<TYPE_FLOAT, TYPE_INT>;
    using TYPE_SELF        = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_DISTANCES const &DISTANCES;
    TYPE_DISTANCES        CONTRIBUTIONS;
    TYPE_FLOAT const      BANDWIDTH;
    TYPE_ACCUMULATOR      ACCUMULATOR;

    TYPE_INT   MAX_INDEX;
    TYPE_FLOAT MAX_VALUE;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline TYPE_INT const SIZE_X() const { return DISTANCES.SIZE_X(); }
    inline TYPE_INT const SIZE_Y() const { return DISTANCES.SIZE_Y(); }

    inline void EVAL_CONTRIBUTIONS() {
        TYPE_INT const limit     = SIZE_X();
        TYPE_INT const looplimit = Threading_Treshhold(limit);

        for (TYPE_INT y = 0; y < limit; y++) {
            auto const &val     = DISTANCES(y, y);
            CONTRIBUTIONS(y, y) = (val >= 0);
        }

#define DO_WORK                                                                \
    TYPE_FLOAT const *distances     = &(DISTANCES(y, 0));                      \
    TYPE_FLOAT *      contributions = &(CONTRIBUTIONS(y, 0));                  \
    for (TYPE_INT x = 0; x < y; x++) {                                         \
        contributions[x] =                                                     \
          (distances[x] >= 0) * std::exp(-distances[x] / BANDWIDTH);           \
    }

        for (TYPE_INT y = 1; y < looplimit; y++) { DO_WORK }
#pragma omp parallel for
        for (TYPE_INT y = looplimit; y < limit; y++) { DO_WORK }

#undef DO_WORK
    }

    inline void EVAL_ACCUMULATOR() {
        TYPE_INT const limit     = SIZE_X();
        TYPE_INT const looplimit = Threading_Treshhold(limit);

        for (TYPE_INT y = 0; y < limit; y++) {
            ACCUMULATOR(y) += CONTRIBUTIONS(y, y);
        }

#define DO_WORK                                                                \
    TYPE_FLOAT const *val = &(CONTRIBUTIONS(y, 0));                            \
    for (TYPE_INT x = 0; x < y; x++) { ACCUMULATOR(y) += val[x]; }             \
    for (TYPE_INT x = 0; x < y; x++) { ACCUMULATOR(x) += val[x]; }

        for (TYPE_INT y = 1; y < looplimit; y++) { DO_WORK }
#pragma omp parallel for
        for (TYPE_INT y = looplimit; y < limit; y++) { DO_WORK }

#undef DO_WORK
    }

    inline void EVAL_MAX_INDEX() {
        MAX_VALUE = 0;
        for (TYPE_INT i = 0; i < ACCUMULATOR(); i++) {
            if (ACCUMULATOR(i) > MAX_VALUE) {
                MAX_VALUE = ACCUMULATOR(i);
                MAX_INDEX = i;
            }
        }
    }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    ////////////////////////
    // Interfaces BEGIN:{ //
    ////////////////////////
  public:
    inline TYPE_INT const get_max_index() const { return MAX_INDEX; }

    inline void debug() const {
        for (TYPE_INT i = 0; i < ACCUMULATOR(); i++) {
            printf("%e\n", ACCUMULATOR(i));
        }
    }

  public:
    _MACRO_CLASS_NAME_(TYPE_DISTANCES const &distances,
                       TYPE_FLOAT const      bandwidth)

      : DISTANCES(distances), CONTRIBUTIONS(SIZE_X()), BANDWIDTH(bandwidth),
        ACCUMULATOR(SIZE_X()) {

        EVAL_CONTRIBUTIONS();
        EVAL_ACCUMULATOR();
        EVAL_MAX_INDEX();
    }

    ~_MACRO_CLASS_NAME_() {}

    static inline TYPE_INT const
    find_kde_center(TYPE_DISTANCES const &distances,
                    TYPE_FLOAT const      bandwidth) {

        TYPE_SELF slave(distances, bandwidth);
        return slave.get_max_index();
    }

    static inline void show_debug(TYPE_DISTANCES const &distances,
                                  TYPE_FLOAT const      bandwidth) {

        TYPE_SELF slave(distances, bandwidth);
        slave.debug();
    }
    //////////////////////
    // Interfaces END.} //
    //////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
