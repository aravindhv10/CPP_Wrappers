﻿#ifndef _HEADER_GUARD_CPPFileIO_Basic_
#define _HEADER_GUARD_CPPFileIO_Basic_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

using TYPE_U8  = uint8_t;
using TYPE_U16 = uint16_t;
using TYPE_U32 = uint32_t;
using TYPE_U64 = uint64_t;
using TYPE_I8  = int8_t;
using TYPE_I16 = int16_t;
using TYPE_I32 = int32_t;
using TYPE_I64 = int64_t;

using TYPE_BYTE = TYPE_U8;

inline TYPE_U8 constexpr MAX_U8() { return 0xFF; }

inline TYPE_U16 constexpr MAX_U16() { return 0xFFFF; }

inline TYPE_U32 constexpr MAX_U32() { return 0xFFFFFFFF; }

inline TYPE_U64 constexpr MAX_U64() { return 0xFFFFFFFFFFFFFFFF; }

template <typename TypeInt>
inline TypeInt constexpr get_bit(TYPE_BYTE const i) {
    TypeInt ret = (1 << i);
    return ret;
}

template <typename TypeInt>
inline TypeInt constexpr get_bit(TypeInt const num, TYPE_BYTE const i) {
    return ((num >> i) & 1);
}

inline TYPE_U16 constexpr get_z_curve(TYPE_U8 const a1, TYPE_U8 const a2) {
    TYPE_U16 const b1 = a1;
    TYPE_U16 const b2 = a2;
    TYPE_U16 const c1 = (get_bit(b1, 0)) | (get_bit(b1, 1) << 2) |
                        (get_bit(b1, 2) << 4) | (get_bit(b1, 3) << 6) |
                        (get_bit(b1, 4) << 8) | (get_bit(b1, 5) << 10) |
                        (get_bit(b1, 6) << 12) | (get_bit(b1, 7) << 14);
    TYPE_U16 const c2 = (get_bit(b2, 0) << 1) | (get_bit(b2, 1) << 3) |
                        (get_bit(b2, 2) << 5) | (get_bit(b2, 3) << 7) |
                        (get_bit(b2, 4) << 9) | (get_bit(b2, 5) << 11) |
                        (get_bit(b2, 6) << 13) | (get_bit(b2, 7) << 15);
    return (c1 | c2);
}

inline TYPE_U32 get_z_curve(TYPE_U16 const a1, TYPE_U16 const a2) {
    TYPE_U32 const b1 = a1;
    TYPE_U32 const b2 = a2;
    TYPE_U32 const c1 = (get_bit(b1, 0)) | (get_bit(b1, 1) << 2) |
                        (get_bit(b1, 2) << 4) | (get_bit(b1, 3) << 6) |
                        (get_bit(b1, 4) << 8) | (get_bit(b1, 5) << 10) |
                        (get_bit(b1, 6) << 12) | (get_bit(b1, 7) << 14) |
                        (get_bit(b1, 8) << 16) | (get_bit(b1, 9) << 18) |
                        (get_bit(b1, 10) << 20) | (get_bit(b1, 11) << 22) |
                        (get_bit(b1, 12) << 24) | (get_bit(b1, 13) << 26) |
                        (get_bit(b1, 14) << 28) | (get_bit(b1, 15) << 30);
    TYPE_U32 const c2 = (get_bit(b2, 0) << 1) | (get_bit(b2, 1) << 3) |
                        (get_bit(b2, 2) << 5) | (get_bit(b2, 3) << 7) |
                        (get_bit(b2, 4) << 9) | (get_bit(b2, 5) << 11) |
                        (get_bit(b2, 6) << 13) | (get_bit(b2, 7) << 15) |
                        (get_bit(b2, 8) << 17) | (get_bit(b2, 9) << 19) |
                        (get_bit(b2, 10) << 21) | (get_bit(b2, 11) << 23) |
                        (get_bit(b2, 12) << 25) | (get_bit(b2, 13) << 27) |
                        (get_bit(b2, 14) << 29) | (get_bit(b2, 15) << 31);
    return (c1 | c2);
}

inline TYPE_U64 get_z_curve(TYPE_U32 const a1, TYPE_U32 const a2) {
    TYPE_U64 const b1 = a1;
    TYPE_U64 const b2 = a2;
    TYPE_U64 const c1 = (get_bit(b1, 0)) | (get_bit(b1, 1) << 2) |
                        (get_bit(b1, 2) << 4) | (get_bit(b1, 3) << 6) |
                        (get_bit(b1, 4) << 8) | (get_bit(b1, 5) << 10) |
                        (get_bit(b1, 6) << 12) | (get_bit(b1, 7) << 14) |
                        (get_bit(b1, 8) << 16) | (get_bit(b1, 9) << 18) |
                        (get_bit(b1, 10) << 20) | (get_bit(b1, 11) << 22) |
                        (get_bit(b1, 12) << 24) | (get_bit(b1, 13) << 26) |
                        (get_bit(b1, 14) << 28) | (get_bit(b1, 15) << 30) |
                        (get_bit(b1, 16) << 32) | (get_bit(b1, 17) << 34) |
                        (get_bit(b1, 18) << 36) | (get_bit(b1, 19) << 38) |
                        (get_bit(b1, 20) << 40) | (get_bit(b1, 21) << 42) |
                        (get_bit(b1, 22) << 44) | (get_bit(b1, 23) << 46) |
                        (get_bit(b1, 24) << 48) | (get_bit(b1, 25) << 50) |
                        (get_bit(b1, 26) << 52) | (get_bit(b1, 27) << 54) |
                        (get_bit(b1, 28) << 56) | (get_bit(b1, 29) << 58) |
                        (get_bit(b1, 30) << 60) | (get_bit(b1, 31) << 62);
    TYPE_U64 const c2 = (get_bit(b2, 0) << 1) | (get_bit(b2, 1) << 3) |
                        (get_bit(b2, 2) << 5) | (get_bit(b2, 3) << 7) |
                        (get_bit(b2, 4) << 9) | (get_bit(b2, 5) << 11) |
                        (get_bit(b2, 6) << 13) | (get_bit(b2, 7) << 15) |
                        (get_bit(b2, 8) << 17) | (get_bit(b2, 9) << 19) |
                        (get_bit(b2, 10) << 21) | (get_bit(b2, 11) << 23) |
                        (get_bit(b2, 12) << 25) | (get_bit(b2, 13) << 27) |
                        (get_bit(b2, 14) << 29) | (get_bit(b2, 15) << 31) |
                        (get_bit(b2, 16) << 33) | (get_bit(b2, 17) << 35) |
                        (get_bit(b2, 18) << 37) | (get_bit(b2, 19) << 39) |
                        (get_bit(b2, 20) << 41) | (get_bit(b2, 21) << 43) |
                        (get_bit(b2, 22) << 45) | (get_bit(b2, 23) << 47) |
                        (get_bit(b2, 24) << 49) | (get_bit(b2, 25) << 51) |
                        (get_bit(b2, 26) << 53) | (get_bit(b2, 27) << 55) |
                        (get_bit(b2, 28) << 57) | (get_bit(b2, 29) << 59) |
                        (get_bit(b2, 30) << 61) | (get_bit(b2, 31) << 63);
    return (c1 | c2);
}

TYPE_BYTE  junk_address = 0;
const bool DEBUG        = false;

template <typename T> inline T *get_junk() {
    return reinterpret_cast<T *>(&junk_address);
}

template <typename T> inline T *get_junk(T *in) { return get_junk<T>(); }

template <typename T> inline void set_junked(T *&inptr) {
    inptr = get_junk<T>();
}

template <typename T> inline bool is_junked(T *inptr) {
    return (inptr == get_junk<T>());
} //

template <typename T> inline bool safe_delete(T *&inptr) {
    if (!is_junked(inptr)) {
        delete inptr;
        set_junked(inptr);
        return true;
    } else {
        return false;
    }
}

template <typename T> inline bool safe_delete_array(T *&inptr) {
    if (!is_junked(inptr)) {
        delete[] inptr;
        set_junked(inptr);
        return true;
    } else {
        return false;
    }
}

inline size_t constexpr shifter(size_t in) { return (1 << in); }

inline bool checkbit(size_t const inbits, size_t const checkbits) {
    return ((inbits & checkbits) == checkbits); //
}

size_t constexpr NTHREADS = 4;

template <typename T> inline T const mymod(T const a) {
    if (a < 0) {
        return -a;
    } else {
        return a;
    }
}

template <typename T> inline T const mymax(T const a, T const b) {
    if (a < b) {
        return b;
    } else {
        return a;
    }
}

template <typename T> inline T const mymin(T const a, T const b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

template <typename T> inline T const mysign(T const a) {
    if (a < 0) {
        return -1;
    } else {
        return 1;
    }
}

template <typename T> inline void myswap(T &a, T &b) {
    T tmp = b;
    b     = a;
    a     = tmp;
}

template <typename T> inline T constexpr GCD(T a, T b) {
    T ret = 1;
    while ((a != 0) && (b != 0)) {
        if (a > b) {
            a   = a % b;
            ret = b;
        } else {
            b   = b % a;
            ret = a;
        }
    }
    return ret;
}

template <typename T> inline T constexpr LCM(T const a, T const b) {
    T ret = GCD(a, b);
    ret   = a * (b / ret);
    return ret;
}

inline void starter_self(std::vector<std::string> &program) {
    std::vector<char *> arrs;
    int                 j = program.size();
    arrs.resize(j + 1);
    for (int i = 0; i < j; i++) { arrs[i] = &(program[i][0]); }
    arrs[j] = static_cast<char *>(NULL);
    execvp(static_cast<const char *>(arrs[0]), &(arrs[0]));
}

inline pid_t starter_fork(std::vector<std::string> &program) {
    pid_t const tmp_pid = fork();
    if (tmp_pid == 0) {
        starter_self(program);
    } else {
        return tmp_pid;
    }
    return tmp_pid;
}

inline void waitonall(std::vector<pid_t> &childs) {
    for (size_t i = 0; i < childs.size(); i++) {
        waitpid(childs[i], static_cast<int *>(NULL), static_cast<int>(0));
    }
}

class GetArgs {

  private:
    std::vector<std::string> args;

  public:
    GetArgs(int _argc, char **_argv) {
        for (int i = 0; i < _argc; i++) { args.push_back(_argv[i]); }
    }

    ~GetArgs() {}

    inline std::string &operator()(size_t i) { return args[i]; }

    inline std::string const &operator()(size_t i) const { return args[i]; }

    inline size_t operator()() const { return args.size(); }
};

inline void SetCPUAffinity(int const cpunum) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpunum, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
        printf(" Error setting affinity...\n ");
    }
}

#endif
