#ifndef _HEADER_GUARD_COMMON_Atomic_Counter_
#define _HEADER_GUARD_COMMON_Atomic_Counter_

#include "../Headers.hh"

#define _MACRO_CLASS_NAME_ Atomic_Counter
template <typename T = long> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_DATA  = T;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_DATA>;
    using TYPE_MUTEX = std::mutex;
    using TYPE_GUARD = std::lock_guard<TYPE_MUTEX>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    TYPE_DATA  current;
    std::mutex locker;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void LOCK() { locker.lock(); }
    inline void UN_LOCK() { locker.unlock(); }

    inline TYPE_DATA ADD(TYPE_DATA const value = 1) {
        LOCK();
        TYPE_DATA const ret = current;
        current += value;
        UN_LOCK();
        return ret;
    }

    inline TYPE_DATA SUBTRACT(TYPE_DATA const value = 1) {
        LOCK();
        TYPE_DATA const ret = current;
        current -= value;
        UN_LOCK();
        return ret;
    }

    inline void ASSIGN(TYPE_DATA const value = 0) {
        LOCK();
        current = value;
        UN_LOCK();
    }

    inline bool CHECK(TYPE_DATA const value) {
        LOCK();
        bool const ret = current == value;
        UN_LOCK();
        return ret;
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    /////////////////
    // Misc BEGIN: //
    /////////////////
  public:
    inline TYPE_DATA operator()() { return current; }

    inline void operator=(TYPE_DATA const in) { Assign(in); }

    inline TYPE_DATA operator+=(TYPE_DATA const val) { return ADD(val); }

    inline TYPE_DATA operator-=(TYPE_DATA const val) { return SUBTRACT(val); }

    inline TYPE_DATA operator++() { return ADD(1); }

    inline TYPE_DATA operator--() { return SUBTRACT(1); }

    inline bool operator==(TYPE_DATA const val) { return CHECK(val); }

  public:
    _MACRO_CLASS_NAME_() : current(0) {}
    ~_MACRO_CLASS_NAME_() {}
    ///////////////
    // Misc END. //
    ///////////////
};
#undef _MACRO_CLASS_NAME_

#endif
