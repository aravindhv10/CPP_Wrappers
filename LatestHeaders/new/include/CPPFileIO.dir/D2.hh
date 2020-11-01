﻿#ifndef _HEADER_GUARD_CPPFileIO_D2_
#define _HEADER_GUARD_CPPFileIO_D2_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./D1.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ Dynamic2DArray

template <typename T> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // DEFINITIONS BEGIN: //
    ////////////////////////
  public:
    using TYPE_DATA   = T;
    using TYPE_SELF   = _MACRO_CLASS_NAME_<TYPE_DATA>;
    using TYPE_PARENT = Dynamic1DArray<TYPE_DATA>;
    //////////////////////
    // DEFINITIONS END. //
    //////////////////////

    /////////////////
    // DATA BEGIN: //
    /////////////////
  private:
    size_t const S_2;
    size_t const S_1;
    size_t const D_2;
    size_t const D_1;
    TYPE_PARENT  STORE;
    ///////////////
    // DATA END. //
    ///////////////

    //////////////////////////
    // RETRIVE SIZES BEGIN: //
    //////////////////////////
  public:
    inline TYPE_DATA *GET_DATA() { return STORE.GET_DATA(); }
    inline size_t     SIZE_1() const { return S_1; }
    inline size_t     SIZE_2() const { return S_2; }
    inline size_t     DIST_1() const { return D_1; }
    inline size_t     DIST_2() const { return D_2; }
    inline bool       ALLOCATED() const { return STORE.ALLOCATED(); }
    ////////////////////////
    // RETRIVE SIZES END. //
    ////////////////////////

    //////////////////////////////
    // RETRIEVE ELEMENTS BEGIN: //
    //////////////////////////////
  private:
    inline size_t GET_INDEX(size_t const _S2, size_t const _S1) const {
        return (_S2 * DIST_2()) + (_S1 * DIST_1());
    }

    inline TYPE_DATA &GET_ELEMENT(size_t const _S2, size_t const _S1) {
        return STORE(GET_INDEX(_S2, _S1));
    }

    inline TYPE_DATA const &GET_ELEMENT(size_t const _S2,
                                        size_t const _S1) const {
        return STORE(GET_INDEX(_S2, _S1));
    }

    inline TYPE_PARENT GET_BAND(size_t const _S2) {
        return TYPE_PARENT(&(GET_ELEMENT(_S2, 0)), SIZE_1(), DIST_1());
    }
    ////////////////////////////
    // RETRIEVE ELEMENTS END. //
    ////////////////////////////

    ///////////////////////
    // CONVINENCE BEGIN: //
    ///////////////////////
  public:
    inline TYPE_DATA &operator()(size_t const _S2, size_t const _S1) {
        return GET_ELEMENT(_S2, _S1);
    }

    inline TYPE_DATA const &operator()(size_t const _S2,
                                       size_t const _S1) const {
        return GET_ELEMENT(_S2, _S1);
    }

    inline void operator=(TYPE_SELF const &other) { STORE = other.STORE; }

    inline TYPE_PARENT operator()(size_t const _S2) { return GET_BAND(_S2); }

    inline TYPE_SELF CloneRange(size_t const B2, size_t const B1,
                                size_t const E2, size_t const E1) const {

        size_t const X2 = E2 - B2 + 1;
        size_t const X1 = E1 - B1 + 1;

        TYPE_SELF ret(X2, X1);

        for (size_t s2 = 0; s2 < X2; s2++)
            for (size_t s1 = 0; s1 < X1; s1++) {
                ret(s2, s1) = GET_ELEMENT(s2 + B2, s1 + B1);
            }

        return ret;
    }
    /////////////////////
    // CONVINENCE END. //
    /////////////////////

    ///////////////////////////////////////
    // CONSTRUCTOR AND DESTRUCTOR BEGIN: //
    ///////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(size_t const _SIZE_2, size_t const _SIZE_1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(S_1), D_1(1), STORE(S_2 * D_2) {}

    _MACRO_CLASS_NAME_(TYPE_DATA *_STORE, size_t const _SIZE_2,
                       size_t const _SIZE_1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(S_1), D_1(1), STORE(_STORE, S_2 * D_2) {
    }

    _MACRO_CLASS_NAME_(TYPE_DATA *_STORE, size_t const _SIZE_2,
                       size_t const _SIZE_1, size_t const _DIST_2,
                       size_t const _DIST_1 = 1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(_DIST_2), D_1(_DIST_1),
        STORE(_STORE, S_2 * D_2) {}

    ~_MACRO_CLASS_NAME_() {}

    static inline TYPE_SELF CLONE(TYPE_SELF const &in) {
        TYPE_SELF
        ret(in.SIZE_2(), in.SIZE_1());

        for (size_t s2 = 0; s2 < ret.SIZE_2(); s2++) {
            for (size_t s1 = 0; s1 < ret.SIZE_1(); s1++) {
                ret(s2, s1) = in(s2, s1);
            }
        }

        return ret;
    }

    /////////////////////////////////////
    // CONSTRUCTOR AND DESTRUCTOR END. //
    /////////////////////////////////////

};

#undef _MACRO_CLASS_NAME_

#endif
