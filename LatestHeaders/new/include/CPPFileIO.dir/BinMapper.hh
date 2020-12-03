#ifndef _HEADER_GUARD_CPPFileIO_BinMapper_
#define _HEADER_GUARD_CPPFileIO_BinMapper_

#include "./Basic.hh"

#define _MACRO_CLASS_NAME_ BinMapper
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

  private:
    inline void Set_Range(TYPE_INT const n_bins, TYPE_FLOAT const begin,
                          TYPE_FLOAT const end) {
        N_BINS = n_bins;
        BEGIN  = begin;
        END    = end;
        DIFF   = END - BEGIN;
    }

    inline TYPE_INT Get_Bin(TYPE_FLOAT const in) const {
        TYPE_FLOAT const T1  = in - BEGIN;
        TYPE_FLOAT const T2  = T1 * static_cast<TYPE_FLOAT>(N_BINS);
        TYPE_FLOAT const T3  = T2 / DIFF;
        TYPE_INT const   ret = static_cast<TYPE_INT>(T3);
        return ret;
    }

  public:
    inline TYPE_FLOAT n_bins() const { return N_BINS; }
    inline TYPE_FLOAT begin() const { return begin; }
    inline TYPE_FLOAT end() const { return end; }

    inline void operator()(TYPE_INT const n_bins, TYPE_FLOAT const begin,
                           TYPE_FLOAT const end) {
        Set_Ranges(n_bins, begin, end);
    }

    inline TYPE_INT operator()(TYPE_FLOAT const in) const {
        return Get_Bin(in);
    }

  public:
    _MACRO_CLASS_NAME_(TYPE_INT const n_bins, TYPE_FLOAT const begin,
                       TYPE_FLOAT const end) {
        Set_Range(n_bins, begin, end);
    }

    _MACRO_CLASS_NAME_() {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
