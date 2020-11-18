#ifndef _HEADER_GUARD_Read_Show_Functions_Show_
#define _HEADER_GUARD_Read_Show_Functions_Show_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../StaticArray.hh"
//////////////////
// Headers END. //
//////////////////

static inline void Show(char const *c, FILE *f = stdout) {
    fprintf(f, "%s", c);
}

template <size_t n>
static inline void Show(StaticArray::ND_ARRAY<n, char> const &in,
                        FILE *                                f = stdout) {
    Show(in.GET_DATA(), f);
}

static inline void Show(FILE *f = stdout) { fprintf(f, "\t"); }

static inline void Show(char const in, FILE *f = stdout) {
    fprintf(f, "%c", in);
}

static inline void Show(unsigned char const in, FILE *f = stdout) {
    fprintf(f, "%d", static_cast<int>(in));
}

static inline void Show(int const in, FILE *f = stdout) {
    fprintf(f, "%d", in);
}

static inline void Show(long const in, FILE *f = stdout) {
    fprintf(f, "%ld", in);
}

static inline void Show(size_t const in, FILE *f = stdout) {
    fprintf(f, "%zu", in);
}

static inline void Show(float const in, FILE *f = stdout) {
    fprintf(f, "%f", in);
}

static inline void Show(double const in, FILE *f = stdout) {
    fprintf(f, "%lf", in);
}

static inline void Show_Next(FILE *f = stdout) { fprintf(f, "\n"); }

#endif
