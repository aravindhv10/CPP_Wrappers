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

    inline std::string const NAME_DIR () {
      mkdir(DIRNAME.c_str(),0755);
      return DIRNAME + "/";
    }

    inline std::string const NAME_INDEX () {
      return NAME_DIR() + "index" ;
    }

    inline std::string const NAME_STORE () {
      return NAME_DIR() + "store" ;
    }


  public:
    _MACRO_CLASS_NAME_(std::string const dirname): DIRNAME(dirname){}

    ~_MACRO_CLASS_NAME_(){}
};
#undef _MACRO_CLASS_NAME_

#endif
