#ifndef _HEADER_GUARD_CPPFileIO_D1_
#define _HEADER_GUARD_CPPFileIO_D1_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Basic.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ Dynamic1DArray

template <typename TD = double, typename TI = long> class _MACRO_CLASS_NAME_ {

    /////////////////////////
    // DEFINITIONS BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_DATA = TD;
    using TYPE_INT  = TI;
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_DATA, TYPE_INT>;
    ///////////////////////
    // DEFINITIONS END.} //
    ///////////////////////

    //////////////////
    // DATA BEGIN:{ //
    //////////////////
  private:
    TYPE_DATA *    STORE;
    TYPE_INT const SZ;
    TYPE_INT const DS;
    bool const     ALLOCATED;
    ////////////////
    // DATA END.} //
    ////////////////

    ///////////////////////////////
    // INTERFACE TO DATA BEGIN:{ //
    ///////////////////////////////
  public:
    inline TYPE_DATA *      GET_DATA() { return STORE; }
    inline TYPE_DATA const *GET_DATA() const { return STORE; }
    inline TYPE_INT         DIST() const { return DS; }
    inline TYPE_INT         SIZE() const { return SZ; }
    inline bool             GET_ALLOCATED() const { return ALLOCATED; }
    /////////////////////////////
    // INTERFACE TO DATA END.} //
    /////////////////////////////

    ////////////////////////
    // RETRIEVING BEGIN:{ //
    ////////////////////////
  private:
    inline TYPE_DATA &GET_ELEMENT(TYPE_INT const i) {
        return STORE[i * DIST()];
    }

    inline TYPE_DATA const &GET_ELEMENT(TYPE_INT const i) const {
        return STORE[i * DIST()];
    }
    //////////////////////
    // RETRIEVING END.} //
    //////////////////////

    /////////////////////
    // SLICING BEGIN:{ //
    /////////////////////
  public:
    inline TYPE_SELF CloneRange(TYPE_INT const begin,
                                TYPE_INT const end) const {

        if (end < begin) {

            return CloneRange(end, begin);

        } else if (end < SIZE()) {

            TYPE_INT const NEW_SIZE = end - begin + 1;

            TYPE_SELF ret(NEW_SIZE);

            for (TYPE_INT i = 0; i < NEW_SIZE; i++) {
                ret(i) = GET_ELEMENT(i + begin);
            }

            return ret;

        } else {

            return CloneRange(begin, SIZE() - 1);
        }
    }

    inline TYPE_SELF ViewRange(TYPE_INT const begin, TYPE_INT const end) {

        if (end < begin) {

            return ViewRange(end, begin);

        } else if (end < SIZE()) {

            TYPE_INT const NEW_SIZE = end - begin + 1;

            TYPE_SELF
            ret(&(STORE[begin]), NEW_SIZE, DIST());

            return ret;

        } else {

            return ViewRange(begin, SIZE() - 1);
        }
    }

    ///////////////////
    // SLICING END.} //
    ///////////////////

    ////////////////////////
    // CONVINENCE BEGIN:{ //
    ////////////////////////
  public:
    inline size_t operator()() const { return SIZE(); }

    inline TYPE_DATA &operator()(size_t const i) { return GET_ELEMENT(i); }

    inline TYPE_DATA const &operator()(size_t const i) const {
        return GET_ELEMENT(i);
    }

    inline void operator=(TYPE_SELF const &other) {
        size_t I = mymin(SIZE(), other());
        for (size_t i = 0; i < I; i++) { GET_ELEMENT(i) = other(i); }
    }

    inline void operator=(TYPE_DATA const *in) {
        for (size_t i = 0; i < SIZE(); i++) { GET_ELEMENT(i) = in[i]; }
    }

    inline TYPE_DATA operator*(TYPE_SELF const &other) const {
        TYPE_DATA ret   = 0;
        size_t    limit = mymin(SIZE(), other.SIZE());
        for (size_t i = 0; i < limit; i++) { ret += other(i) * GET_ELEMENT(i); }
        return ret / static_cast<TYPE_DATA>(limit);
    }
    //////////////////////
    // CONVINENCE END.} //
    //////////////////////

    /////////////////////////
    // CONSTRUCTOR BEGIN:{ //
    /////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_INT const size)
      : SZ(size), DS(1), ALLOCATED((SZ * DS) > 0) {
        if (ALLOCATED) { STORE = new TYPE_DATA[SIZE() * DIST()]; }
    }

    _MACRO_CLASS_NAME_(TYPE_DATA *store, TYPE_INT const size,
                       TYPE_INT const dist = 1)
      : SZ(size), DS(dist), ALLOCATED(false) {
        STORE = store;
    }

    static inline TYPE_SELF CLONE(TYPE_SELF const &in) {
        TYPE_SELF ret(in.SIZE());
        ret = in;
        return ret;
    }

    static inline TYPE_SELF CLONE(std::vector<TYPE_DATA> const &in) {
        TYPE_SELF ret(in.size());
        ret = &(in[0]);
        return ret;
    }

    ~_MACRO_CLASS_NAME_() {
        if (ALLOCATED) { delete[] STORE; }
    }
    ///////////////////////
    // CONSTRUCTOR END.} //
    ///////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
