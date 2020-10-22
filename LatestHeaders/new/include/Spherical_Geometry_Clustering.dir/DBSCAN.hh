#ifndef _HEADER_GUARD_FINALS_DBSCAN_
#define _HEADER_GUARD_FINALS_DBSCAN_

#include "./StaticArray.hh"

#define _MACRO_CLASS_NAME_ Point
#define _MACRO_BASE_NAME_ D2GPS_Coordinates<TR>

template <typename TR = double, typename TI = int>
class _MACRO_CLASS_NAME_ : public _MACRO_BASE_NAME_ {
  public:
    using TYPE_FLOAT = TR;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BASE  = _MACRO_BASE_NAME_;
    using TYPE_PAIR  = typename TYPE_BASE::TYPE_PAIR;

    static inline TYPE_INT constexpr NOISE() { return -1; }
    static inline TYPE_INT constexpr NOT_CLASSIFIED() { return -2; }

  public:
    TYPE_INT pts_cnt, cluster;

  public:
    inline TYPE_BASE &AS_BASE() { return static_cast<TYPE_BASE &>(this[0]); }

    inline TYPE_BASE const &AS_BASE() const {
        return static_cast<TYPE_BASE const &>(this[0]);
    }

    inline TYPE_FLOAT operator()(TYPE_SELF const &ot) const {
        return AS_BASE().HaverSineDistance(ot.AS_BASE());
    }

    inline TYPE_FLOAT operator()(TYPE_BASE const &ot) const {
        return AS_BASE().HaverSineDistance(ot);
    }

    inline void operator=(TYPE_PAIR const &in) {
        AS_BASE() = in;
        pts_cnt   = 0;
        cluster   = NOT_CLASSIFIED();
    }

    inline void operator=(TYPE_BASE const &in) { this[0] = in.AS_PAIR(); }
};

#undef _MACRO_BASE_NAME_
#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ SizeSortableVector
#define _MACRO_BASE_NAME_ std::vector<T>

template <typename T> class _MACRO_CLASS_NAME_ : public _MACRO_BASE_NAME_ {
  public:
    using TYPE_DATA = T;
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_DATA>;
    using TYPE_BASE = _MACRO_BASE_NAME_;

    inline TYPE_BASE &AS_BASE() { return static_cast<TYPE_BASE &>(this[0]); }

    inline TYPE_BASE const &AS_BASE() const {
        return static_cast<TYPE_BASE const &>(this[0]);
    }

    inline bool operator>(TYPE_SELF const &other) const {
        return this->size() < other.size();
    }
    inline bool operator<(TYPE_SELF const &other) const {
        return this->size() > other.size();
    }
};

#undef _MACRO_BASE_NAME_
#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ DBSCAN

template <typename TR = double, typename TI = int> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_FLOAT       = TR;
    using TYPE_FLOATS      = std::vector<TYPE_FLOAT>;
    using TYPE_INT         = TI;
    using TYPE_INTS        = std::vector<TYPE_INT>;
    using TYPE_SELF        = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_COORDINATE  = D2GPS_Coordinates<TYPE_FLOAT>;
    using TYPE_COORDINATES = std::vector<TYPE_COORDINATE>;
    using TYPE_PAIR        = typename TYPE_COORDINATE::TYPE_PAIR;
    using TYPE_PAIRS       = std::vector<TYPE_PAIR>;
    using TYPE_POINT       = Point<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINTS      = std::vector<TYPE_POINT>;
    using TYPE_DISTANCES   = CPPFileIO::SymmetricMatrix<TYPE_FLOAT>;

    using TYPE_OUTPUT = std::vector<SizeSortableVector<TYPE_COORDINATE>>;

    static inline TYPE_INT constexpr NOISE() { return TYPE_POINT::NOISE(); }

    static inline TYPE_INT constexpr NOT_CLASSIFIED() {
        return TYPE_POINT::NOT_CLASSIFIED();
    }
    ///////////////////////
    // definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_FLOAT const        EPS;
    TYPE_INT const          MIN_PTS;
    TYPE_INT const          SIZE;
    TYPE_COORDINATES const &INPUTS;
    TYPE_POINTS             POINTS;
    std::vector<TYPE_INTS>  ADJ_POINTS;
    std::vector<TYPE_INTS>  CLUSTER;
    TYPE_DISTANCES          DISTANCES;
    TYPE_OUTPUT             OUTPUT;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // Main working functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline void EVAL_DISTANCES() {
		TYPE_INT constexpr treshhold = 1000 ;
		TYPE_INT const looplimit = CPPFileIO::mymin(treshhold,SIZE);

		for(TYPE_INT y=0;y<looplimit;y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                DISTANCES(y, x) = POINTS[y](POINTS[x]);
            }
		}

#pragma omp parallel for
		for(TYPE_INT y=looplimit;y<SIZE;y++){
            for (TYPE_INT x = 0; x < y; x++) {
                DISTANCES(y, x) = POINTS[y](POINTS[x]);
            }
		}

        for (TYPE_INT y = 1; y < SIZE; y++) { DISTANCES(y, y) = 0; }
    }

    inline void CHECK_NEAR_POINTS() {
        for (TYPE_INT y = 1; y < SIZE; y++) {
            for (TYPE_INT x = 0; x < y; x++) {
                if (DISTANCES(y, x) <= EPS) {
                    POINTS[y].pts_cnt++;
                    POINTS[x].pts_cnt++;
                    ADJ_POINTS[y].push_back(x);
                    ADJ_POINTS[x].push_back(y);
                }
            }
        }
    }

    inline bool IS_CORE_OBJECT(TYPE_INT idx) {
        return POINTS[idx].pts_cnt >= MIN_PTS;
    }

    inline void DFS(TYPE_INT const now, TYPE_INT const c) {
        POINTS[now].cluster = c;
        if (!IS_CORE_OBJECT(now)) { return; }

        for (auto &next : ADJ_POINTS[now]) {
            if (POINTS[next].cluster == NOT_CLASSIFIED()) { DFS(next, c); }
        }
    }

    inline void RUN() {
        EVAL_DISTANCES();
        CHECK_NEAR_POINTS();

        TYPE_INT CLUSTER_IDX = 0;
        for (TYPE_INT i = 0; i < SIZE; i++) {
            if (POINTS[i].cluster == NOT_CLASSIFIED()) {
                if (IS_CORE_OBJECT(i)) {
                    DFS(i, CLUSTER_IDX);
                    CLUSTER_IDX++;
                } else {
                    POINTS[i].cluster = NOISE();
                }
            }
        }

        CLUSTER.resize(CLUSTER_IDX + 1);
        for (TYPE_INT i = 0; i < SIZE; i++) {
            if (POINTS[i].cluster != NOISE()) {
                CLUSTER[POINTS[i].cluster].push_back(i);
            }
        }

        OUTPUT.resize(CLUSTER.size());

        for (TYPE_INT i = 0; i < CLUSTER.size(); i++) {
            OUTPUT[i].resize(CLUSTER[i].size());
            for (TYPE_INT j = 0; j < CLUSTER[i].size(); j++) {
                OUTPUT[i][j] = INPUTS[CLUSTER[i][j]];
            }
        }
        std::sort(OUTPUT.begin(), OUTPUT.end());
    }

    inline void DEBUG() {
        for (TYPE_INT i = 0; i < OUTPUT.size(); i++) {
            printf("%zu ", OUTPUT[i].size());
        }
        printf("\n");
    }

    inline void DO_ALL() {
        ADJ_POINTS.resize(SIZE);

        POINTS.resize(SIZE);
        for (TYPE_INT i = 0; i < SIZE; i++) { POINTS[i] = INPUTS[i]; }

        RUN();
        if (false) { DEBUG(); }
    }
    //////////////////////////////////
    // Main working functions END.} //
    //////////////////////////////////

    /////////////////////////////
    // Main Interfaces BEGIN:{ //
    /////////////////////////////
  public:
    inline std::vector<TYPE_INTS> const &get_cluster() const { return CLUSTER; }
    inline TYPE_OUTPUT const &           get_output() { return OUTPUT; }
    ///////////////////////////
    // Main Interfaces END.} //
    ///////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_FLOAT const eps, TYPE_INT const minpts,
                       TYPE_COORDINATES const &points)
      : EPS(eps), MIN_PTS(minpts), SIZE(points.size()), INPUTS(points),
        DISTANCES(SIZE) {
        DO_ALL();
    }

    _MACRO_CLASS_NAME_(TYPE_FLOAT const eps, TYPE_INT const minpts,
                       TYPE_PAIRS const &points)
      : EPS(eps), MIN_PTS(minpts), SIZE(points.size()),
        INPUTS(reinterpret_cast<TYPE_COORDINATES const &>(points)),
        DISTANCES(SIZE) {
        DO_ALL();
    }

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
