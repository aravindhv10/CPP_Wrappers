#ifndef _HEADER_GUARD_Store_ND_BinMapper_
#define _HEADER_GUARD_Store_ND_BinMapper_

#include "../StaticArray.hh"

#define _MACRO_CLASS_NAME_ bin_mapper
template <typename TI = long, typename TF = double> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_INT   = TI;
    using TYPE_FLOAT = TF;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_INT, TYPE_FLOAT>;

  private:
    TYPE_INT   N_BINS;
    TYPE_FLOAT BEGIN;
    TYPE_FLOAT END;
    TYPE_FLOAT DIFF;

  public:
    inline void SET_RANGES(TYPE_INT const n_bins, TYPE_FLOAT const begin,
                           TYPE_FLOAT const end) {
        N_BINS = n_bins;
        BEGIN  = begin;
        END    = end;
        DIFF   = END - BEGIN;
    }

    inline TYPE_INT Get_Bin(TYPE_FLOAT const in) const {
        return static_cast<TYPE_FLOAT>((in - BEGIN) * N_BINS) /
               static_cast<TYPE_FLOAT>(DIFF);
    }

  public:
    inline TYPE_INT operator()(TYPE_FLOAT const in) const {
        return Get_Bin(in);
    }

  public:
    _MACRO_CLASS_NAME_(TYPE_INT const n_bins, TYPE_FLOAT const begin,
                       TYPE_FLOAT const end)
      : N_BINS(n_bins), BEGIN(begin), END(end), DIFF(END - BEGIN) {}

    _MACRO_CLASS_NAME_() {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
