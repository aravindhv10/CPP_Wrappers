#ifndef _HEADER_GUARD_CPPFileIO_RandomNumberGenerator_
#define _HEADER_GUARD_CPPFileIO_RandomNumberGenerator_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ myrandgen

template <typename T, typename TF = double, typename TI = long>
class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_RANDOM_ENGINE = T;
    using TYPE_FLOAT         = TF;
    using TYPE_INT           = TI;
    using TYPE_SELF =
      _MACRO_CLASS_NAME_<TYPE_RANDOM_ENGINE, TYPE_FLOAT, TYPE_INT>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::vector<TYPE_RANDOM_ENGINE *>          engines;
    std::uniform_real_distribution<TYPE_FLOAT> dist;
    TYPE_FLOAT                                 low, high;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////////////////
    // Main Work Functions BEGIN: //
    ////////////////////////////////
  private:
    inline void allocate() {
        std::random_device random_seeds;
        for (size_t i = 0; i < engines.size(); i++) {
            engines[i] = new TYPE_RANDOM_ENGINE(random_seeds());
        }
    }

  public:
    inline TYPE_FLOAT operator[](TYPE_INT th) {
        TYPE_FLOAT ret = dist(engines[th][0]);
        return ret;
    }
    //////////////////////////////
    // Main Work Functions END. //
    //////////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_INT threads=1, TYPE_FLOAT const _low = 0, TYPE_FLOAT const _high = 1)
      : low(_low), high(_high), dist(_low, _high) {
        engines.resize(threads);
        allocate();
    }

    ~_MACRO_CLASS_NAME_() {
        for (TYPE_INT i = 0; i < engines.size(); i++) { delete engines[i]; }
    }
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
