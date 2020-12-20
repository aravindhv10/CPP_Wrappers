#ifndef _HEADER_GUARD_CPPFileIO_MyStr_
#define _HEADER_GUARD_CPPFileIO_MyStr_

#include "./D1.hh"

#define _MACRO_CLASS_NAME_ MyStr
template <typename TI = long> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_INT          = TI;
    using TYPE_SELF         = _MACRO_CLASS_NAME_<TYPE_INT>;
    using TYPE_SELVES       = std::vector<TYPE_SELF>;
    using TYPE_BUFFER       = Dynamic1DArray<char, TYPE_INT>;
    using TYPE_BUFFER_CONST = Dynamic1DArray<char const, TYPE_INT>;

  private:
    char *START; // point to 1st address in the array.

    char *END; // Point to the final null character in the array.
               // = START + LENGTH.

    TYPE_INT LENGTH; // length of the string,
                     // excluding the null terminating byte.
                     // = END - START.

  public:
    inline char *      start() { return START; }
    inline char *      end() { return END; }
    inline TYPE_INT    length() const { return LENGTH; }
    inline char const *start() const { return START; }
    inline char const *end() const { return END; }

  private:
    inline void CUT(TYPE_SELVES &ret, char const in) {
        char *start = START;
        char *end;

    MainLoop:
        /* The main cutting loop: */ {
            end = static_cast<char *>(memchr(start, in, END - start));
            if (end) {
                (*end) = 0;
                ret.push_back(TYPE_SELF(start, end));
                start = end + 1;
                if (start < END) { goto MainLoop; }
            } else if (start < END) {
                ret.push_back(TYPE_SELF(start, END));
            }
        }
    }

  public:
    inline TYPE_BUFFER operator()() {
        TYPE_BUFFER ret(START, LENGTH);
        return ret;
    }

    inline TYPE_BUFFER_CONST operator()() const {
        TYPE_BUFFER_CONST const ret(START, LENGTH);
        return ret;
    }

    inline TYPE_SELVES operator()(char const in) {
        TYPE_SELVES ret;
        CUT(ret, in);
        return ret;
    }

    inline void operator()(TYPE_SELVES &ret, char const in) {
        ret.clear();
        CUT(ret, in);
    }

    inline void operator()(char *const buffer, TYPE_INT const length) {
        START  = buffer;
        LENGTH = length;
        END    = START + LENGTH;
    }

    inline void operator()(char *const start, char *const end) {
        START  = start;
        END    = end;
        LENGTH = end - start;
    }

    inline void operator()(char *const buffer) {
        this[0](buffer, strlen(buffer));
    }

  public:
    _MACRO_CLASS_NAME_(char *const buffer) { this[0](buffer); }

    _MACRO_CLASS_NAME_(char *const start, char *const end) {
        this[0](start, end);
    }

    _MACRO_CLASS_NAME_(char *const buffer, TYPE_INT const length) {
        this[0](buffer, length);
    }

    _MACRO_CLASS_NAME_(std::string &in) { this[0](&(in[0]), in.size()); }

    _MACRO_CLASS_NAME_(TYPE_BUFFER &in) { this[0](in.GET_DATA(), in()); }

    _MACRO_CLASS_NAME_() {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
