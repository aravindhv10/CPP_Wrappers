#ifndef _HEADER_GUARD_Store_ND_MainStore_
#define _HEADER_GUARD_Store_ND_MainStore_

#include "./MainIndex.hh"

#define _MACRO_CLASS_NAME_ MainStore
template <typename TI = long, typename TF = double, TI ny = 10000,
          TI nx = 10000>
class _MACRO_CLASS_NAME_ {
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

    using TYPE_INDEX = MainIndex<TYPE_INT, TYPE_FLOAT, SIZE_Y(), SIZE_X()>;

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

  public:
    std::string const DIRNAME;

    inline std::string const NAME_DIR() {
        mkdir(DIRNAME.c_str(), 0755);
        return DIRNAME + "/";
    }

    inline std::string const NAME_INDEX() { return NAME_DIR() + "index"; }

    inline std::string const NAME_STORE() { return NAME_DIR() + "store"; }

  public:
    CPPFileIO::FileArray<char> BUFFER_INDEX;
    TYPE_INDEX *               INDEX;

    inline void ALLOCATE_READ_INDEX() {
        BUFFER_INDEX.writeable(false);
        INDEX =
          reinterpret_cast<TYPE_INDEX>(&(BUFFER_INDEX(0, sizeof(TYPE_INDEX))));
    }

    inline void ALLOCATE_WRITE_INDEX() {
        BUFFER_INDEX.writeable(true);
        INDEX =
          reinterpret_cast<TYPE_INDEX>(&(BUFFER_INDEX(0, sizeof(TYPE_INDEX))));
    }

    inline void TRUNCATE_INDEX() {
        BUFFER_INDEX.size(sizeof(TYPE_INDEX));
        ALLOCATE_READ_INDEX();
    }

    inline void SET_RANGE(TYPE_FLOAT const x1, TYPE_FLOAT const x2,
                          TYPE_FLOAT const y1, TYPE_FLOAT const y2) {
        INDEX->SET_RANGE(x1, x2, y1, y2);
    }

    inline TYPE_PAIR_INT GET_BINS(TYPE_FLOAT const y,
                                  TYPE_FLOAT const x) const {
        return INDEX->GET_BINS(y, x);
    }

    inline TYPE_INT FLATTEN_MAP(TYPE_FLOAT const y, TYPE_FLOAT const x) const {
        return INDEX->FLATTEN_MAP(y, x);
    }

    inline void ZERO_COUNTS() { INDEX->ZERO_COUNTS(); }

    inline void EVAL_CUMULATIVE() { INDEX->EVAL_CUMULATIVE(); }

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
            INDEX->SET_RANGE(min[0], min[1], max[0], max[1]);
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

  public:
    _MACRO_CLASS_NAME_(std::string const dirname)
      : DIRNAME(dirname), BUFFER_INDEX(NAME_INDEX), STORE(NAME_STORE()) {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
