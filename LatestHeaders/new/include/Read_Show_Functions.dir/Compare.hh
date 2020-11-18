

#ifndef _HEADER_GUARD_Read_Show_Functions_Compare
#define _HEADER_GUARD_Read_Show_Functions_Compare

///////////////////
// Header BEGIN: //
///////////////////
#include "../StaticArray.hh"
/////////////////
// Header END. //
/////////////////

#define COMPARE(intype)                                                        \
    inline char Compare(intype const a, intype const b) {                      \
        return (a > b) - (a < b);                                              \
    }

COMPARE(char)
COMPARE(unsigned char)
COMPARE(int)
COMPARE(unsigned int)
COMPARE(long)
COMPARE(unsigned long)
COMPARE(float)
COMPARE(double)

#undef COMPARE

inline char Compare(char const *S1, char const *S2) {
    int const val = strcmp(S1, S2);
    return Compare(val, 0);
}

template <size_t m, size_t n>
inline char Compare(StaticArray::ND_ARRAY<m, char> const &S1,
                    StaticArray::ND_ARRAY<n, char> const &S2) {

    return Compare(S1.GET_DATA(), S2.GET_DATA());
}

template <size_t n>
inline char Compare(StaticArray::ND_ARRAY<n, char> const &S1,
                    std::string const &                   S2) {

    return Compare(S1.GET_DATA(), S2.c_str());
}

template <size_t n>
inline char Compare(std::string const &                   S1,
                    StaticArray::ND_ARRAY<n, char> const &S2) {

    return Compare(S2, S1) * -1;
}

inline char Compare(std::string const &S1, std::string const &S2) {
    return Compare(S1.c_str(), S2.c_str());
}

#endif
