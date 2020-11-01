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

template <typename T> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF          = _MACRO_CLASS_NAME_;
    using TYPE_RANDOM_ENGINE = T;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::vector<T *>                       engines;
    std::uniform_real_distribution<double> dist;
    double                                 low, high;
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
            engines[i] = new T(random_seeds());
        }
    }

  public:
    inline double operator[](size_t th) {
        double ret = dist(engines[th][0]);
        return ret;
    }

    //////////////////////////////
    // Main Work Functions END. //
    //////////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////

  public:
    _MACRO_CLASS_NAME_(size_t threads, double _low = 0, double _high = 1)
      : low(_low), high(_high), dist(_low, _high) {
        engines.resize(threads);
        allocate();
    }

    ~_MACRO_CLASS_NAME_() {
        for (size_t i = 0; i < engines.size(); i++) { delete engines[i]; }
    }

    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
