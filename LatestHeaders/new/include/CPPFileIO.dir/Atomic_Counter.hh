#ifndef _HEADER_GUARD_COMMON_Atomic_Counter_
#define _HEADER_GUARD_COMMON_Atomic_Counter_

#include "../Headers.hh"

#define _MACRO_CLASS_NAME_ Atomic_Counter
class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF  = _MACRO_CLASS_NAME_;
    using TYPE_MUTEX = std::mutex;
    using TYPE_GUARD = std::lock_guard<TYPE_MUTEX>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    long       current;
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

    inline long Increase(long const value=1) {
        Lock();
        size_t const ret = current;
        current += value;
        UnLock();
        return ret;
    }

    inline long Decrease(long const value=1) {
        Lock();
        size_t const ret = current;
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
    inline long operator()() { return Increase(); }

  public:
    _MACRO_CLASS_NAME_() : current(0) {}
    ~_MACRO_CLASS_NAME_() {}
    ///////////////
    // Misc END. //
    ///////////////
};

#undef _MACRO_CLASS_NAME_

#endif
