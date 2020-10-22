#ifndef _HEADER_GUARD_Simple_DBSCAN_
#define _HEADER_GUARD_Simple_DBSCAN_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../CPPFileIO.hh"
///////////////////
// Headers END.} //
///////////////////

#define _MACRO_CLASS_NAME_ Simple_DBSCAN

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_BYTE       = unsigned char;
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_DISTANCES  = CPPFileIO::SymmetricMatrix<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ADJ_POINTS = CPPFileIO::SymmetricMatrix<bool, TYPE_INT>;
    using TYPE_COUNTS     = CPPFileIO::Dynamic1DArray<TYPE_INT, TYPE_INT>;

    static inline TYPE_INT constexpr NOISE() { return -1; }
    static inline TYPE_INT constexpr NOT_CLASSIFIED() { return -2; }
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_DISTANCES const &DISTANCES;
    TYPE_COUNTS           NUM_NEIGHBOURS;
    TYPE_FLOAT const      EPSILON;
    TYPE_INT const        MIN_PTS;
    TYPE_ADJ_POINTS       ADJ_POINTS;
    TYPE_COUNTS           ELEMENT_CLUSTER;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline TYPE_INT const SIZE() const { return DISTANCES.SIZE_X(); }

    inline void RESET() {
        for (TYPE_INT i = 0; i < ELEMENT_CLUSTER(); i++) {
            ELEMENT_CLUSTER(i) = NOT_CLASSIFIED();
        }

        for (TYPE_INT i = 0; i < SIZE(); i++) { NUM_NEIGHBOURS(i) = 0; }
    }

    inline void COUNT_NEIGHBOURS() {
        for (TYPE_INT y = 1; y < SIZE(); y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                bool const val = (DISTANCES(y, x) < EPSILON);
                NUM_NEIGHBOURS(y) += val;
                NUM_NEIGHBOURS(x) += val;
                ADJ_POINTS(y, x) = val;
            }
        }
    }

    inline bool IS_CORE_OBJECT(TYPE_INT idx) {
        return NUM_NEIGHBOURS(idx) >= MIN_PTS;
    }

    inline void DFS(TYPE_INT const now, TYPE_INT const c) {
        ELEMENT_CLUSTER(now) = c;

        if (!IS_CORE_OBJECT(now)) { return; }

        for (TYPE_INT i = 0; i < SIZE(); i++) {
            bool const val =
              ADJ_POINTS(now, i) && ((ELEMENT_CLUSTER(i) == NOT_CLASSIFIED()) ||
                                     (ELEMENT_CLUSTER(i) == NOISE()));

            if (val) { DFS(i, c); }
        }
    }

    inline void DFS() {
        TYPE_INT cluster_idx = 0;
        for (TYPE_INT i = 0; i < SIZE(); i++) {
            TYPE_BYTE const status =
              (1 * (ELEMENT_CLUSTER(i) == NOT_CLASSIFIED())) +
              (4 * IS_CORE_OBJECT(i)) + (16 * (ELEMENT_CLUSTER(i) == NOISE()));

            switch (status) {
                case 5:
                    DFS(i, cluster_idx);
                    cluster_idx += 1;
                    break;
                case 1: ELEMENT_CLUSTER(i) = NOISE();
                case 4:
                case 0:
                case 16:
                case 17:
                case 20:
                case 21: break;
            }
        }
    }

    inline void CONSTRUCT() {
        RESET();
        COUNT_NEIGHBOURS();
        DFS();
    }

    inline TYPE_COUNTS const &GET_ELEMENT_CLUSTER() const {
        return ELEMENT_CLUSTER;
    }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

	/////////////////////////////
	// Main Interfaces BEGIN:{ //
	/////////////////////////////
  public:
    inline TYPE_COUNTS const &operator()() const {
        return GET_ELEMENT_CLUSTER();
    }

    _MACRO_CLASS_NAME_(TYPE_DISTANCES const &distances, TYPE_INT const epsilon,
                       TYPE_INT const min_pts)
      : DISTANCES(distances), NUM_NEIGHBOURS(SIZE()), EPSILON(epsilon),
        MIN_PTS(min_pts), ADJ_POINTS(SIZE()), ELEMENT_CLUSTER(SIZE()) {
        CONSTRUCT();
    }

    ~_MACRO_CLASS_NAME_() {}
	///////////////////////////
	// Main Interfaces END.} //
	///////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
