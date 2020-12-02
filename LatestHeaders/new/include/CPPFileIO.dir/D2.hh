#ifndef _HEADER_GUARD_CPPFileIO_D2_
#define _HEADER_GUARD_CPPFileIO_D2_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./D1.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ Dynamic2DArray

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // DEFINITIONS BEGIN: //
    ////////////////////////
  public:
    using TYPE_FLOAT  = TF;
    using TYPE_INT    = TI;
    using TYPE_SELF   = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PARENT = Dynamic1DArray<TYPE_FLOAT, TYPE_INT>;
    //////////////////////
    // DEFINITIONS END. //
    //////////////////////

    /////////////////
    // DATA BEGIN: //
    /////////////////
  private:
    TYPE_PARENT STORE;

  public:
    inline TYPE_FLOAT *GET_DATA() { return STORE.GET_DATA(); }
    inline bool        ALLOCATED() const { return STORE.ALLOCATED(); }
    ///////////////
    // DATA END. //
    ///////////////

    //////////////////////////
    // RETRIVE SIZES BEGIN: //
    //////////////////////////
  private:
    TYPE_INT const S_2;
    TYPE_INT const S_1;
    TYPE_INT const D_2;
    TYPE_INT const D_1;

  public:
    inline TYPE_INT SIZE_1() const { return S_1; }
    inline TYPE_INT SIZE_2() const { return S_2; }
    inline TYPE_INT DIST_1() const { return D_1; }
    inline TYPE_INT DIST_2() const { return D_2; }
    ////////////////////////
    // RETRIVE SIZES END. //
    ////////////////////////

    //////////////////////////////
    // RETRIEVE ELEMENTS BEGIN: //
    //////////////////////////////
  private:
    inline TYPE_INT GET_INDEX(TYPE_INT const _S2, TYPE_INT const _S1) const {
        return (_S2 * DIST_2()) + (_S1 * DIST_1());
    }

    inline TYPE_FLOAT &GET_ELEMENT(TYPE_INT const _S2, TYPE_INT const _S1) {
        return STORE(GET_INDEX(_S2, _S1));
    }

    inline TYPE_FLOAT const &GET_ELEMENT(TYPE_INT const _S2,
                                         TYPE_INT const _S1) const {
        return STORE(GET_INDEX(_S2, _S1));
    }

    inline TYPE_PARENT GET_BAND(TYPE_INT const _S2) {
        return TYPE_PARENT(&(GET_ELEMENT(_S2, 0)), SIZE_1(), DIST_1());
    }
    ////////////////////////////
    // RETRIEVE ELEMENTS END. //
    ////////////////////////////

    ///////////////////////
    // CONVINENCE BEGIN: //
    ///////////////////////
  public:
    inline TYPE_FLOAT &operator()(TYPE_INT const _S2, TYPE_INT const _S1) {
        return GET_ELEMENT(_S2, _S1);
    }

    inline TYPE_FLOAT const &operator()(TYPE_INT const _S2,
                                        TYPE_INT const _S1) const {
        return GET_ELEMENT(_S2, _S1);
    }

    inline void operator=(TYPE_SELF const &other) { STORE = other.STORE; }

    inline TYPE_PARENT operator()(TYPE_INT const _S2) { return GET_BAND(_S2); }

    inline TYPE_SELF CloneRange(TYPE_INT const B2, TYPE_INT const B1,
                                TYPE_INT const E2, TYPE_INT const E1) const {

        TYPE_INT const X2 = E2 - B2 + 1;
        TYPE_INT const X1 = E1 - B1 + 1;

        TYPE_SELF ret(X2, X1);

        for (TYPE_INT s2 = 0; s2 < X2; s2++)
            for (TYPE_INT s1 = 0; s1 < X1; s1++) {
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
    _MACRO_CLASS_NAME_(TYPE_INT const _SIZE_2, TYPE_INT const _SIZE_1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(S_1), D_1(1), STORE(S_2 * D_2) {}

    _MACRO_CLASS_NAME_(TYPE_FLOAT *_STORE, TYPE_INT const _SIZE_2,
                       TYPE_INT const _SIZE_1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(S_1), D_1(1), STORE(_STORE, S_2 * D_2) {
    }

    _MACRO_CLASS_NAME_(TYPE_FLOAT *_STORE, TYPE_INT const _SIZE_2,
                       TYPE_INT const _SIZE_1, TYPE_INT const _DIST_2,
                       TYPE_INT const _DIST_1 = 1)
      : S_2(_SIZE_2), S_1(_SIZE_1), D_2(_DIST_2), D_1(_DIST_1),
        STORE(_STORE, S_2 * D_2) {}

    ~_MACRO_CLASS_NAME_() {}

    static inline TYPE_SELF CLONE(TYPE_SELF const &in) {
        TYPE_SELF
        ret(in.SIZE_2(), in.SIZE_1());

        for (TYPE_INT s2 = 0; s2 < ret.SIZE_2(); s2++) {
            for (TYPE_INT s1 = 0; s1 < ret.SIZE_1(); s1++) {
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
