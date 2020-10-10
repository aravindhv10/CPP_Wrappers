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
  public:
    inline void Lock() { locker.lock(); }
    inline void UnLock() { locker.unlock(); }

    inline TYPE_DATA Increase(TYPE_DATA const value = 1) {
        Lock();
        TYPE_DATA const ret = current;
        current += value;
        UnLock();
        return ret;
    }

    inline TYPE_DATA Decrease(TYPE_DATA const value = 1) {
        Lock();
        TYPE_DATA const ret = current;
        current -= value;
        UnLock();
        return ret;
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    /////////////////
    // Misc BEGIN: //
    /////////////////
  public:
    inline TYPE_DATA operator()() { return Increase(); }

  public:
    _MACRO_CLASS_NAME_() : current(0) {}
    ~_MACRO_CLASS_NAME_() {}
    ///////////////
    // Misc END. //
    ///////////////
};

#undef _MACRO_CLASS_NAME_

#endif
