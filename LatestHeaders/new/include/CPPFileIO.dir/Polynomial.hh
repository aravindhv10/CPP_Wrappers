#ifndef _HEADER_GUARD_CPPFileIO_Polynomial_
#define _HEADER_GUARD_CPPFileIO_Polynomial_

#include "./D1.hh"

#define _MACRO_CLASS_NAME_ polynomial

template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_FLOAT = TF;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ARRAY = Dynamic1DArray<TYPE_FLOAT, TYPE_INT>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    TYPE_ARRAY coefficients;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////////////////
    // Accessing Functions BEGIN: //
    ////////////////////////////////
  public:
    inline TYPE_INT    ORDER() const { return coefficients() - 1; }
    inline TYPE_FLOAT &COEFFICIENT(TYPE_INT const i) { return coefficients(i); }

    inline TYPE_FLOAT COEFFICIENT(TYPE_INT const i) const {
        return coefficients(i);
    }

    inline TYPE_SELF MULTIPLY(TYPE_SELF const &other) const {
        TYPE_SELF ret(ORDER() + other.ORDER());
        for (TYPE_INT i = 0; i <= ORDER(); i++) {
            for (TYPE_INT j = 0; j <= other.ORDER(); j++) {
                TYPE_INT const out = i + j;
                ret(out) += COEFFICIENT(i) * other(j);
            }
        }
        return ret;
    }

    inline TYPE_SELF SUBTRACT(TYPE_SELF const &other) const {
        TYPE_INT  min_order = mymin(ORDER(), other.ORDER());
        TYPE_INT  max_order = mymax(ORDER(), other.ORDER());
        TYPE_SELF ret(max_order);
        TYPE_INT  index = 0;
        while (index <= min_order) {
            ret(index) = COEFFICIENT(index) - other(index);
            index++;
        }
        while (index <= ORDER()) {
            ret(index) = COEFFICIENT(index);
            index++;
        }
        while (index <= other.ORDER()) {
            ret(index) = -other(index);
            index++;
        }
        return ret;
    }

    inline TYPE_SELF ADD(TYPE_SELF const &other) const {
        size_t    min_order = mymin(ORDER(), other.ORDER());
        size_t    max_order = mymax(ORDER(), other.ORDER());
        TYPE_SELF ret(max_order);
        size_t    index = 0;
        while (index <= min_order) {
            ret(index) = COEFFICIENT(index) + other(index);
            index++;
        }
        while (index <= ORDER()) {
            ret(index) = COEFFICIENT(index);
            index++;
        }
        while (index <= other.ORDER()) {
            ret(index) = other(index);
            index++;
        }
        return ret;
    }

    inline TYPE_SELF DIFFERENTIATE() const {
        TYPE_SELF ret(ORDER() - 1);
        for (TYPE_INT i = 0; i <= ret(); i++) {
            ret(i) = (i + 1) * COEFFICIENT(i + 1);
        }
        return ret;
    }

    inline TYPE_FLOAT EVALUATE(TYPE_FLOAT const in) const {
        TYPE_FLOAT ret = 0;
        TYPE_FLOAT xpw = 1;
        for (TYPE_INT i = 0; i <= ORDER(); i++) {
            ret += COEFFICIENT(i) * xpw;
            xpw *= in;
        }
        return ret;
    }
    //////////////////////////////
    // Accessing Functions END. //
    //////////////////////////////

    ///////////////////////
    // Convinence BEGIN: //
    ///////////////////////
  public:
    inline TYPE_INT    operator()() const { return ORDER(); }
    inline TYPE_FLOAT &operator()(TYPE_INT const i) { return COEFFICIENT(i); }

    inline TYPE_FLOAT operator()(TYPE_INT const i) const {
        return COEFFICIENT(i);
    }
    /////////////////////
    // Convinence END. //
    /////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_INT const order = 1) : coefficients(order + 1) {
        for (TYPE_INT i = 0; i < coefficients(); i++) { coefficients(i) = 0; }
        coefficients(1) = 1;
    }

    ~_MACRO_CLASS_NAME_() {}
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
