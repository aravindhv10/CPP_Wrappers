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
    TYPE_INT              NUM_CLUSTERS;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline TYPE_INT const SIZE() const { return DISTANCES.SIZE_X(); }

    inline void RESET() {
        TYPE_INT const limit = SIZE();
        for (TYPE_INT i = 0; i < limit; i++) {
            ELEMENT_CLUSTER(i) = NOT_CLASSIFIED();
        }

        for (TYPE_INT i = 0; i < limit; i++) { NUM_NEIGHBOURS(i) = 0; }

        NUM_CLUSTERS = 0;
    }

    inline bool IS_INSIDE(TYPE_INT const y, TYPE_INT const x) const {
        TYPE_FLOAT const val = DISTANCES(y, x);
        return (0 <= val) && (val < EPSILON);
    }

    inline void COUNT_NEIGHBOURS() {
        TYPE_INT constexpr treshhold = 100;
        TYPE_INT const limit         = SIZE();
        TYPE_INT const loop_limit    = CPPFileIO::mymin(limit, treshhold);

        for (TYPE_INT y = 0; y < limit; y++) {
            ADJ_POINTS(y, y) = (DISTANCES(y, y) >= 0);
        }

        for (TYPE_INT y = 1; y < loop_limit; y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                ADJ_POINTS(y, x) = IS_INSIDE(y, x);
            }
        }

#pragma omp parallel for
        for (TYPE_INT y = loop_limit; y < limit; y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                ADJ_POINTS(y, x) = IS_INSIDE(y, x);
            }
        }

        for (TYPE_INT y = 1; y < limit; y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                NUM_NEIGHBOURS(y) += ADJ_POINTS(y, x);
                NUM_NEIGHBOURS(x) += ADJ_POINTS(y, x);
            }
        }
    }

    inline bool IS_CORE_OBJECT(TYPE_INT idx) {
        return NUM_NEIGHBOURS(idx) >= MIN_PTS;
    }

    inline bool OPEN_2_CLASSIFY(TYPE_INT const i) const {
        return ELEMENT_CLUSTER(i) < 0;
    }

    inline void DFS(TYPE_INT now, TYPE_INT const c) {
        std::stack<TYPE_INT> s;
    MainLoopPart:
        /* The main looping part: */ {
            for (TYPE_INT i = 0; i < SIZE(); i++) {
                TYPE_BYTE const status = (OPEN_2_CLASSIFY(i)) +
                                         (2 * ADJ_POINTS(now, i)) +
                                         (4 * IS_CORE_OBJECT(now));

                switch (status) {
                    case 7: s.push(i);
                    case 3: ELEMENT_CLUSTER(i) = c;
                    default: break;
                }
            }

            if (!s.empty()) {
                now = s.top();
                s.pop();
                goto MainLoopPart;
            }
        }
    }

    inline void DFS() {
        for (TYPE_INT i = 0; i < SIZE(); i++) {
            TYPE_BYTE const status =
              (1 * (ELEMENT_CLUSTER(i) == NOT_CLASSIFIED())) +
              (4 * IS_CORE_OBJECT(i)) + (16 * (ELEMENT_CLUSTER(i) == NOISE()));

            switch (status) {
                case 5:
                    ELEMENT_CLUSTER(i) = NUM_CLUSTERS;
                    DFS(i, NUM_CLUSTERS);
                    NUM_CLUSTERS += 1;
                    break;

                case 1: ELEMENT_CLUSTER(i) = NOISE();
                default: break;
            }
        }
    }

    inline void CONSTRUCT() {
        RESET();
        COUNT_NEIGHBOURS();
        DFS();
    }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    /////////////////////////////
    // Main Interfaces BEGIN:{ //
    /////////////////////////////
  public:
    inline TYPE_INT const get_num_clusters() const { return NUM_CLUSTERS; }

    inline TYPE_COUNTS const &get_element_cluster() const {
        return ELEMENT_CLUSTER;
    }

    inline TYPE_COUNTS const &operator()() const {
        return get_element_cluster();
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
