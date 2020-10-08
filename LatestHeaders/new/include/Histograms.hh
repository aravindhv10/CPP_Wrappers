#ifndef _HEADER_GUARD_Histograms_
#define _HEADER_GUARD_Histograms_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./StaticArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ MyHist

template <size_t Num = 1, typename T = float> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    static inline size_t constexpr SIZE() { return Num; } //
    using TYPE_DATA = T;                                  //
    using TYPE_HIST = StaticArray::ND_ARRAY<SIZE(), TYPE_DATA>;
    using TYPE_SELF = _MACRO_CLASS_NAME_<SIZE(), TYPE_DATA>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    ///////////////////////////////
    // Main Data Elements BEGIN: //
    ///////////////////////////////
  private:
    TYPE_DATA const Begin, End;
    TYPE_HIST       DATA;
    /////////////////////////////
    // Main Data Elements END. //
    /////////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    static inline size_t constexpr N1() { return 0; }

    static inline size_t constexpr N2() { return SIZE(); }

    static inline TYPE_DATA constexpr N2_Minux_M1() {
        return static_cast<TYPE_DATA>(N2() - N1());
    }

    inline TYPE_DATA const N_to_X(size_t const N) const {
        return End - (((End - Begin) * (N2() - N)) / N2_Minux_M1());
    }

    inline size_t X_to_N(TYPE_DATA const X) const {
        size_t const ret =
          N2() -
          (static_cast<size_t>((End - X) * N2_Minux_M1() / (End - Begin)));

        return CPPFileIO::mymin(ret, N2() - 1);
    }

    inline void Fill(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        size_t const index = X_to_N(X);
        DATA[index] += W;
    }

    inline void Show(FILE *f = stdout) const {
        for (size_t i = N1(); i < N2(); i++) {
            TYPE_DATA const x1 = N_to_X(i);
            TYPE_DATA const x2 = N_to_X(i + 1);
            fprintf(f, "%e %e\n%e %e\n", x1, DATA[i], x2, DATA[i]);
        }
    }

    inline void Show(std::string const Filename) const {
        FILE *f = fopen(&(Filename[0]), "w");
        Show(f);
        fclose(f);
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ///////////////////////////
    // Main Interface BEGIN: //
    ///////////////////////////
  public:
    inline void operator()(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        Fill(X, W);
    }

    inline void operator>>(FILE *f) const { Show(f); }

    inline void operator>>(std::string const Filename) const { Show(Filename); }

    _MACRO_CLASS_NAME_(TYPE_DATA const _Begin, TYPE_DATA const _End)
      : Begin(_Begin), End(_End) {
        DATA = 0;
    }

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////////
    // Main Interface END. //
    /////////////////////////
};
#undef _MACRO_CLASS_NAME_

#endif
