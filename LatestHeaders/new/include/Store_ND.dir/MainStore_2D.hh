#ifndef _HEADER_GUARD_Store_ND_MainStore_2D_
#define _HEADER_GUARD_Store_ND_MainStore_2D_

#include "./MainIndex_2D.hh"

#define _MACRO_CLASS_NAME_ MainStore_2D
template <typename TI = long, typename TF = double, TI ny = 10000,
          TI nx = 10000>
class _MACRO_CLASS_NAME_ {
    /////////////////////////////////
    // Definitions related BEGIN:{ //
    /////////////////////////////////
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_INTS       = std::vector<TYPE_INT>;
    using TYPE_BIN_MAPPER = bin_mapper<TYPE_INT, TYPE_FLOAT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;

    static inline TYPE_INT constexpr SIZE_Y() { return ny; }
    static inline TYPE_INT constexpr SIZE_X() { return nx; }
    static inline TYPE_INT constexpr SIZE() { return SIZE_X() * SIZE_Y(); }

    using TYPE_SELF =
      _MACRO_CLASS_NAME_<TYPE_INT, TYPE_FLOAT, SIZE_Y(), SIZE_X()>;

    using TYPE_INDEX = MainIndex_2D<TYPE_INT, TYPE_FLOAT, SIZE_Y(), SIZE_X()>;

    using TYPE_COUNTS     = StaticArray::ND_ARRAY<SIZE(), TYPE_INT>;
    using TYPE_CUMULATIVE = StaticArray::ND_ARRAY<SIZE() + 1, TYPE_INT>;

    static inline TYPE_INT constexpr FLATTEN_MAP(TYPE_INT const y,
                                                 TYPE_INT const x) {
        return x + (y * SIZE_X());
    }

    struct TYPE_STORE_ELEMENT {
        TYPE_PAIR_FLOAT vals;
        TYPE_INT        index;
    };
    ///////////////////////////////
    // Definitions related END.} //
    ///////////////////////////////

    ///////////////////////////
    // Names related BEGIN:{ //
    ///////////////////////////
  public:
    std::string const DIRNAME;

    inline std::string const NAME_DIR() const {
        mkdir(DIRNAME.c_str(), 0755);
        return DIRNAME + "/";
    }

    inline std::string const NAME_INDEX() const { return NAME_DIR() + "index"; }
    inline std::string const NAME_STORE() const { return NAME_DIR() + "store"; }
    /////////////////////////
    // Names related END.} //
    /////////////////////////

    ///////////////////////////
    // INDEX related BEGIN:{ //
    ///////////////////////////
  public:
    CPPFileIO::FileArray<char> BUFFER_INDEX;
    TYPE_INDEX *               INDEX;

    inline void ALLOCATE_READ_INDEX() {
        BUFFER_INDEX.writeable(false);
        INDEX = reinterpret_cast<TYPE_INDEX *>(
          &(BUFFER_INDEX(0, sizeof(TYPE_INDEX))));
    }

    inline void ALLOCATE_WRITE_INDEX() {
        BUFFER_INDEX.writeable(true);
        INDEX = reinterpret_cast<TYPE_INDEX *>(
          &(BUFFER_INDEX(0, sizeof(TYPE_INDEX))));
    }

    inline void TRUNCATE_INDEX() {
        BUFFER_INDEX.size(sizeof(TYPE_INDEX));
        ALLOCATE_READ_INDEX();
    }

    inline void SET_RANGE(TYPE_FLOAT const x1, TYPE_FLOAT const y1,
                          TYPE_FLOAT const x2, TYPE_FLOAT const y2) {
        INDEX->SET_RANGE(x1, y1, x2, y2);
    }

    inline TYPE_PAIR_INT BINS_FLOAT_TO_INT(TYPE_FLOAT const y,
                                           TYPE_FLOAT const x) const {
        return INDEX->BINS_FLOAT_TO_INT(y, x);
    }

    inline TYPE_INT FLATTEN_MAP(TYPE_FLOAT const y, TYPE_FLOAT const x) const {
        return INDEX->FLATTEN_MAP(y, x);
    }

    inline void ZERO_COUNTS() { INDEX->ZERO_COUNTS(); }
    inline void EVAL_CUMULATIVE() { INDEX->EVAL_CUMULATIVE(); }

    inline void DEBUG_INDEX() {
        printf("CAME TO DEBUG FUNCTION...\n");

        printf("INDEX->BIN_X.N_BINS = %ld; INDEX->BIN_X.BEGIN = %lf; "
               "INDEX->BIN_X.END = %lf; INDEX->BIN_X.DIFF = %lf;\n",
               INDEX->BIN_X.N_BINS, INDEX->BIN_X.BEGIN, INDEX->BIN_X.END,
               INDEX->BIN_X.DIFF);

        printf("INDEX->BIN_Y.N_BINS = %ld; INDEX->BIN_Y.BEGIN = %lf; "
               "INDEX->BIN_Y.END = %lf; INDEX->BIN_Y.DIFF = %lf;\n",
               INDEX->BIN_Y.N_BINS, INDEX->BIN_Y.BEGIN, INDEX->BIN_Y.END,
               INDEX->BIN_Y.DIFF);
    }
    /////////////////////////
    // INDEX related END.} //
    /////////////////////////

    ///////////////////////////
    // STORE related BEGIN:{ //
    ///////////////////////////
  public:
    CPPFileIO::FileArray<TYPE_STORE_ELEMENT> STORE;

    template <typename Reader, typename Converter>
    inline void WRITE_STORE(Reader reader, Converter converter) {
        ALLOCATE_WRITE_INDEX();
        size_t const limit = reader();

        /* Setting the range: */ {
            TYPE_PAIR_FLOAT min, max;
            /* check the 1st element: */ {
                auto const &       element = reader(0);
                TYPE_STORE_ELEMENT tmp;
                tmp.vals = converter(element);
                min      = tmp.vals;
                max      = tmp.vals;
            }
            for (size_t i = 1; i < limit; i++) {
                auto const &       element = reader(i);
                TYPE_STORE_ELEMENT tmp;
                tmp.vals = converter(element);
                min[0]   = CPPFileIO::mymin(min[0], tmp.vals[0]);
                min[1]   = CPPFileIO::mymin(min[1], tmp.vals[1]);
                max[0]   = CPPFileIO::mymax(max[0], tmp.vals[0]);
                max[1]   = CPPFileIO::mymax(max[1], tmp.vals[1]);
            }
            SET_RANGE(min[0], min[1], max[0], max[1]);
        }

        /* Pass 1: */ {
            ZERO_COUNTS();
            for (size_t i = 0; i < limit; i++) {
                auto const &       element = reader(i);
                TYPE_STORE_ELEMENT tmp;
                tmp.vals  = converter(element);
                tmp.index = i;
                INDEX->ADD_LOCATION(tmp.vals[0], tmp.vals[1]);
            }
            EVAL_CUMULATIVE();
            ZERO_COUNTS();
        }

        /* Pass 2: */ {
            STORE.writeable(true);
            for (size_t i = 0; i < limit; i++) {
                auto const &       element = reader(i);
                TYPE_STORE_ELEMENT tmp;
                tmp.vals  = converter(element);
                tmp.index = i;
                TYPE_INT const idx_loc =
                  INDEX->ADD_LOCATION(tmp.vals[0], tmp.vals[1]);
                STORE(idx_loc) = tmp;
            }
            STORE.writeable(false);
        }

        STORE.size(limit);
        TRUNCATE_INDEX();
    }
    /////////////////////////
    // STORE related END.} //
    /////////////////////////

    ///////////////////////////
    // Reading STORE BEGIN:{ //
    ///////////////////////////
  public:
    inline void GET_INDICES(TYPE_INTS &indices, TYPE_FLOAT const y,
                            TYPE_FLOAT const x) {
        indices.clear();
        auto const bin = FLATTEN_MAP(y, x);
        for (TYPE_INT i = INDEX->CUMULATIVE[bin];
             i < INDEX->CUMULATIVE[bin + 1]; i++) {
            indices.push_back(STORE(i).index);
        }
    }

    inline void GET_INDICES(TYPE_INTS &indices, TYPE_INT const y,
                            TYPE_INT const x) {
        indices.clear();
        auto const bin = FLATTEN_MAP(y, x);
        for (TYPE_INT i = INDEX->CUMULATIVE[bin];
             i < INDEX->CUMULATIVE[bin + 1]; i++) {
            indices.push_back(STORE(i).index);
        }
    }
    /////////////////////////
    // Reading STORE END.} //
    /////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(std::string const dirname)
      : DIRNAME(dirname), BUFFER_INDEX(NAME_INDEX()), STORE(NAME_STORE()) {}

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
