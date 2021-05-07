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

    ////////////////////////////////
    // Main lock Functions BEGIN: //
    ////////////////////////////////
  private:
    inline void LOCK() { locker.lock(); }
    inline void UN_LOCK() { locker.unlock(); }
    //////////////////////////////
    // Main lock Functions END. //
    //////////////////////////////

    /////////////////
    // Misc BEGIN: //
    /////////////////
  public:
    inline TYPE_DATA operator()() {
        LOCK();
        TYPE_DATA ret = current;
        UN_LOCK();
        return ret;
    }

#define _MACRO_OPERATOR_(op)                                                   \
    inline TYPE_DATA operator op(TYPE_DATA const in) {                         \
        LOCK();                                                                \
        TYPE_DATA const ret = current;                                         \
        current op in;                                                         \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }                                                                          \
    inline TYPE_DATA operator op(TYPE_SELF &in) {                              \
        LOCK();                                                                \
        in.LOCK();                                                             \
        TYPE_DATA const ret = current;                                         \
        current op in.current;                                                 \
        in.UN_LOCK();                                                          \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }

    _MACRO_OPERATOR_(+=)
    _MACRO_OPERATOR_(-=)
    _MACRO_OPERATOR_(=)

#undef _MACRO_OPERATOR_

#define _MACRO_OPERATOR_(op)                                                   \
    inline bool operator op(TYPE_DATA const in) {                              \
        LOCK();                                                                \
        bool const ret = (current op in);                                      \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }                                                                          \
    inline bool operator op(TYPE_SELF &in) {                                   \
        LOCK();                                                                \
        in.LOCK();                                                             \
        bool const ret = (current op in.current);                              \
        in.UN_LOCK();                                                          \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }

    _MACRO_OPERATOR_(>)
    _MACRO_OPERATOR_(<)
    _MACRO_OPERATOR_(==)
    _MACRO_OPERATOR_(>=)
    _MACRO_OPERATOR_(<=)

#undef _MACRO_OPERATOR_

#define _MACRO_OPERATOR_(op)                                                   \
    inline TYPE_DATA operator op(int) {                                        \
        LOCK();                                                                \
        TYPE_DATA ret = current;                                               \
        current   op;                                                          \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }                                                                          \
    inline TYPE_DATA operator op() {                                           \
        LOCK();                                                                \
        current   op;                                                          \
        TYPE_DATA ret = current;                                               \
        UN_LOCK();                                                             \
        return ret;                                                            \
    }

    _MACRO_OPERATOR_(++)
    _MACRO_OPERATOR_(--)

#undef _MACRO_OPERATOR_

    ///////////////
    // Misc END. //
    ///////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_DATA const &in) : current(in) {}
    _MACRO_CLASS_NAME_(TYPE_DATA const &&in) : current(in) {}
    _MACRO_CLASS_NAME_() : current(0) {}
    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
