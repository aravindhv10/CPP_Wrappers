#ifndef _HEADER_GUARD_CPPFileIO_FileDivider_
#define _HEADER_GUARD_CPPFileIO_FileDivider_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./D1.hh"
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ FileDivider
template <char endline> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF       = _MACRO_CLASS_NAME_<endline>;
    using TYPE_BOUNDARIES = std::vector<size_t>;
    using TYPE_READER     = FileArray<char const>;
    using TYPE_BUFFER     = Dynamic1DArray<char>;
    using TYPE_LOCKER     = std::mutex;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const FILENAME;
    TYPE_READER       READER;
    TYPE_BOUNDARIES   BOUNDARIES;
    size_t const      LIMIT;
    TYPE_LOCKER       LOCKER;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void LOCK() { LOCKER.lock(); }
    inline void UNLOCK() { LOCKER.unlock(); }

    inline void CRAWL(size_t const i) {
        if (i < (BOUNDARIES.size() - 1)) {
            size_t const start = BOUNDARIES[i];
            size_t const end   = BOUNDARIES[i + 1];
            for (size_t j = start; j < end; j++) {
                if (READER(j) == endline) {
                    BOUNDARIES[i] = j + 1;
                    return;
                }
            }
        }
    }

    inline TYPE_BOUNDARIES const &GET_BOUNDARIES(size_t const nums = 1) {
        BOUNDARIES.resize(nums + 1);
        for (size_t i = 0; i <= nums; i++) {
            BOUNDARIES[i] = (LIMIT * i) / nums;
        }
        for (size_t i = 1; i < nums; i++) { CRAWL(i); }
        return BOUNDARIES;
    }

    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ///////////////////////
    // Interfaces BEGIN: //
    ///////////////////////
  public:
    inline TYPE_BOUNDARIES const &operator()(size_t const nums = 1) {
        return GET_BOUNDARIES(nums);
    }

    inline void GET_BUFFER(size_t const val, std::vector<char> &in) {
        if (BOUNDARIES.size() > val + 1) {
            size_t const start  = BOUNDARIES[val];
            size_t const length = BOUNDARIES[val + 1] - BOUNDARIES[val];
            in.resize(length+1);
            LOCK();
            char const *buf = &(READER(start, length));
            memcpy(&(in[0]), buf, length);
            UNLOCK();
            in[length] = 0;
        }
    }

    inline void GET_BUFFER(size_t const val, TYPE_BUFFER &in) {
        if (BOUNDARIES.size() > val + 1) {
            size_t const start  = BOUNDARIES[val];
            size_t const length = BOUNDARIES[val + 1] - BOUNDARIES[val];
            in.~Dynamic1DArray();
            new (&in) TYPE_BUFFER(length+1);
            LOCK();
            char const * buf    = &(READER(start, length));
            memcpy(in.GET_DATA(), buf, length);
            UNLOCK();
            in(length)=0;
        }
    }

    inline void operator()(size_t const val, TYPE_BUFFER &in) {
        return GET_BUFFER(val, in);
    }

    _MACRO_CLASS_NAME_(std::string const filename)
      : FILENAME(filename), READER(FILENAME), LIMIT(READER()) {}

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////
    // Interfaces END. //
    /////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
