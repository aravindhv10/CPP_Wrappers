#ifndef _HEADER_GUARD_CPPFileIO_Basic_
#define _HEADER_GUARD_CPPFileIO_Basic_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

char       junk_address = 0;
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
