#ifndef _HEADER_GUARD_CPPFileIO_symmetric_matrix_
#define _HEADER_GUARD_CPPFileIO_symmetric_matrix_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../Headers.hh"
#include "./D1.hh"
///////////////////
// Headers END.} //
///////////////////

#define _MACRO_CLASS_NAME_ SymmetricMatrix

template <typename TD = double, typename TI = long> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_DATA  = TD;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_DATA, TYPE_INT>;
    using TYPE_STORE = Dynamic1DArray<TYPE_DATA, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    //////////////////////////
    // Data Element BEGIN:{ //
    //////////////////////////
  public:
    TYPE_INT const N;
    TYPE_STORE     STORE;
    ////////////////////////
    // Data Element END.} //
    ////////////////////////

    //////////////////////////////////////
    // Data accessing functions BEGIN:{ //
    //////////////////////////////////////
  private:
    static inline TYPE_INT MAKE_MAP(TYPE_INT const y, TYPE_INT const x) {
        return x + ((y * (y + 1)) >> 1);
    }

    static inline TYPE_INT MAKE_MAP_SAFE(TYPE_INT const y, TYPE_INT const x) {
        if (y >= x) {
            return MAKE_MAP(y, x);
        } else {
            return MAKE_MAP(x, y);
        }
    }

  public:
    inline TYPE_DATA &GET(TYPE_INT const y, TYPE_INT const x) {
        return STORE(MAKE_MAP(y, x));
    }

    inline TYPE_DATA const GET(TYPE_INT const y, TYPE_INT const x) const {
        return STORE(MAKE_MAP(y, x));
    }

    inline TYPE_DATA &GET_SAFE(TYPE_INT const y, TYPE_INT const x) {
        return STORE(MAKE_MAP_SAFE(y, x));
    }

    inline TYPE_DATA const GET_SAFE(TYPE_INT const y, TYPE_INT const x) const {
        return STORE(MAKE_MAP_SAFE(y, x));
    }

    inline TYPE_DATA &operator()(TYPE_INT const y, TYPE_INT const x) {
        return GET_SAFE(y, x);
    }

    inline TYPE_DATA const operator()(TYPE_INT const y,
                                      TYPE_INT const x) const {
        return GET_SAFE(x, y);
    }

    inline TYPE_INT          SIZE_Y() const { return N; }
    inline TYPE_INT          SIZE_X() const { return N; }
    inline TYPE_INT          SIZE() const { return STORE(); }
    inline TYPE_STORE &      operator()() { return STORE; }
    inline TYPE_STORE const &operator()() const { return STORE; }
    ////////////////////////////////////
    // Data accessing functions END.} //
    ////////////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
    _MACRO_CLASS_NAME_(TYPE_INT const n) : N(n), STORE((N * (N + 1)) >> 2) {}
    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
