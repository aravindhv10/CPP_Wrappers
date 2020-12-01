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
        size_t constexpr req_size = sizeof(TYPE_INDEX);
        INDEX = reinterpret_cast<TYPE_INDEX *>(&(BUFFER_INDEX(0, req_size)));
    }

    inline TYPE_PAIR_INT BINS_FLOAT_TO_INT(TYPE_FLOAT const y,
                                           TYPE_FLOAT const x) const {
        return INDEX->BINS_FLOAT_TO_INT(y, x);
    }

    inline TYPE_INT FLATTEN_MAP(TYPE_FLOAT const y, TYPE_FLOAT const x) const {
        return INDEX->FLATTEN_MAP(y, x);
    }

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
    CPPFileIO::FileArray<TYPE_INT> STORE;

    template <typename Reader, typename Converter>
    inline void WRITE_STORE(Reader reader) {

        //////////////////////////////////////////
        // Allocate the writeable index BEGIN:{ //
        //////////////////////////////////////////
        TYPE_INDEX *               index_rw;
        CPPFileIO::FileArray<char> buffer_index_rw(NAME_INDEX());
        size_t constexpr req_size = sizeof(TYPE_INDEX);
        /* Allocate the buffer: */ {
            buffer_index_rw.writeable(true);
            char *ptr = &(buffer_index_rw(0, req_size));
            index_rw  = reinterpret_cast<TYPE_INDEX *>(ptr);
        }
        ////////////////////////////////////////
        // Allocate the writeable index END.} //
        ////////////////////////////////////////

        size_t const limit = reader();

        /* Setting the range: */ {
            TYPE_PAIR_FLOAT min, max;
            /* check the 1st element: */ {
                auto const &          element = reader(0);
                TYPE_PAIR_FLOAT const tmp     = converter(element);
                min                           = tmp;
                max                           = tmp;
            }
            for (size_t i = 1; i < limit; i++) {
                TYPE_PAIR_FLOAT const tmp = reader(i);
                min[0]                    = CPPFileIO::mymin(min[0], tmp[0]);
                min[1]                    = CPPFileIO::mymin(min[1], tmp[1]);
                max[0]                    = CPPFileIO::mymax(max[0], tmp[0]);
                max[1]                    = CPPFileIO::mymax(max[1], tmp[1]);
            }
            index_rw->SET_RANGE(min[0], min[1], max[0], max[1]);
        }

        /* pass-1): Count elements in the bins: */ {
            index_rw->ZERO_COUNTS();
            for (size_t i = 0; i < limit; i++) {
                TYPE_PAIR_FLOAT const tmp = reader(i);
                index_rw->ADD_LOCATION(tmp[0], tmp[1]);
            }
            index_rw->EVAL_CUMULATIVE();
            index_rw->ZERO_COUNTS();
        }

        /* pass-2): Write the elements in the bins */ {
            CPPFileIO::FileArray<TYPE_INT> store_rw(NAME_STORE());
            store_rw.writeable(true);
            for (size_t i = 0; i < limit; i++) {
                TYPE_PAIR_FLOAT const tmp = reader(i);
                TYPE_INT const idx_loc = index_rw->ADD_LOCATION(tmp[0], tmp[1]);
                store_rw(idx_loc)      = i;
            }
            store_rw.size(limit);
        }

        buffer_index_rw.size(req_size);
    }
    /////////////////////////
    // STORE related END.} //
    /////////////////////////

    ///////////////////////////
    // Reading STORE BEGIN:{ //
    ///////////////////////////
  public:
#define MAKE_GET_INDICES(TYPE_IN)                                              \
    inline void GET_INDICES(TYPE_INTS &indices, TYPE_IN const y,               \
                            TYPE_IN const x) {                                 \
        indices.clear();                                                       \
        auto const     bin   = FLATTEN_MAP(y, x);                              \
        TYPE_INT const start = INDEX->CUMULATIVE[bin];                         \
        TYPE_INT const end   = INDEX->CUMULATIVE[bin + 1];                     \
        for (TYPE_INT i = start; i < end; i++) {                               \
            indices.push_back(STORE(i));                                       \
        }                                                                      \
    }

    MAKE_GET_INDICES(TYPE_FLOAT)
    MAKE_GET_INDICES(TYPE_INT)

#undef MAKE_GET_INDICES
    /////////////////////////
    // Reading STORE END.} //
    /////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(std::string const dirname)
      : DIRNAME(dirname), BUFFER_INDEX(NAME_INDEX()), STORE(NAME_STORE()) {
        ALLOCATE_READ_INDEX();
    }

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
