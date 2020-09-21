#ifndef _HEADER_GUARD_CPPFileIO_Polynomial_
#define _HEADER_GUARD_CPPFileIO_Polynomial_

#include "./D1.hh"

#define _MACRO_CLASS_NAME_ polynomial

template <typename T> class _MACRO_CLASS_NAME_ {

  ////////////////////////
  // Definitions BEGIN: //
  ////////////////////////

public:
  using TYPE_DATA = T; //

  using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_DATA>; //

  using TYPE_ARRAY = Dynamic1DArray<TYPE_DATA>; //

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
  inline size_t ORDER() const { return coefficients() - 1; }

  inline TYPE_DATA &COEFFICIENT(size_t const i) { return coefficients(i); }

  inline TYPE_DATA COEFFICIENT(size_t const i) const { return coefficients(i); }

  inline TYPE_SELF MULTIPLY(TYPE_SELF const &other) const {
    TYPE_SELF ret(ORDER() + other.ORDER());
    for (size_t i = 0; i <= ORDER(); i++) {
      for (size_t j = 0; j <= other.ORDER(); j++) {
        size_t const out = i + j;
        ret(out) += COEFFICIENT(i) * other(j);
      }
    }
    return ret;
  }

  inline TYPE_SELF DIFFERENTIATE() const {
    TYPE_SELF ret(ORDER() - 1);
    for (size_t i = 0; i <= ret(); i++) {
      ret(i) = (i + 1) * COEFFICIENT(i + 1);
    }
    return ret;
  }

  inline TYPE_SELF SUBTRACT(TYPE_SELF const &other) const {
    size_t min_order = mymin(ORDER(), other.ORDER());
    size_t max_order = mymax(ORDER(), other.ORDER());
    TYPE_SELF ret(max_order);
    size_t index = 0;
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
    size_t min_order = mymin(ORDER(), other.ORDER());
    size_t max_order = mymax(ORDER(), other.ORDER());
    TYPE_SELF ret(max_order);
    size_t index = 0;
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

  inline TYPE_DATA EVALUATE(TYPE_DATA const in) const {
    TYPE_DATA ret = 0;
    TYPE_DATA xpw = 1;
    for (size_t i = 0; i <= ORDER(); i++) {
      ret += COEFFICIENT(i) * xpw;
      // printf("DEBUG: i = %zu ; ret = %e ; xpw = %e ; coeff = %e ;
      // \n",i,ret,xpw,COEFFICIENT(i));
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
  inline size_t operator()() const { return ORDER(); }

  inline TYPE_DATA &operator()(size_t const i) { return COEFFICIENT(i); }

  inline TYPE_DATA operator()(size_t const i) const { return COEFFICIENT(i); }

  /////////////////////
  // Convinence END. //
  /////////////////////

  /////////////////////////////////////
  // Constructor & Destructor BEGIN: //
  /////////////////////////////////////

public:
  _MACRO_CLASS_NAME_(size_t const order = 1) : coefficients(order + 1) {
    for (size_t i = 0; i < coefficients(); i++) {
      coefficients(i) = 0;
    }

    coefficients(1) = 1;
  }

  ~_MACRO_CLASS_NAME_() {}

  ///////////////////////////////////
  // Constructor & Destructor END. //
  ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
