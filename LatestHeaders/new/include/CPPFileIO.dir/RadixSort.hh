#ifndef _HEADER_GUARD_CPPFileIO_RadixSort_
#define _HEADER_GUARD_CPPFileIO_RadixSort_

#include "./D1.hh"
#include "./FileArray.hh"

#define _MACRO_CLASS_NAME_ RadisSorter

template <typename T, typename TI = long> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_INT    = TI;
    using TYPE_BUFFER = T;
    using TYPE_SELF   = _MACRO_CLASS_NAME_<TYPE_BUFFER, TYPE_INT>;
    using TYPE_BYTE   = unsigned char;

  private:
    TYPE_BUFFER &BUFFER1;
    TYPE_BUFFER &BUFFER2;
    TYPE_INT     COUNTS[256];

  private:
    inline void ZERO_COUNTS() {
        for (TYPE_INT i = 0; i < 256; i++) { COUNTS = 0; }
    }

  public:
    inline void DO_SCAN(TYPE_BUFFER &buffer1, TYPE_BUFFER &buffer2,
                        TYPE_INT const digit) {

        TYPE_INT const limit1 = buffer1();
        TYPE_INT const limit2 = buffer2();

        if (limit1 != limit2) { return; }

        /* Evaluating the prefix sum: */ {
            ZERO_COUNTS();
            for (TYPE_INT i = 0; i < limit1; i++) {
                auto const &    element = buffer1(i);
                TYPE_BYTE const val     = element[digit];
                COUNTS[val]++;
            }
        }

        /* Accumulate the prefix sum: */ {
            TYPE_INT current = 0;
            TYPE_INT sum     = 0;
            for (TYPE_INT i = 0; i < 256; i++) {
                current   = COUNTS[i];
                COUNTS[i] = sum;
                sum += current;
            }
        }

        /* Perform the copy: */ {
            for (TYPE_INT i = 0; i < limit1; i++) {
                auto const &    element = buffer1(i);
                TYPE_BYTE const val     = element[digit];
                buffer2(COUNTS[val])    = element;
                COUNTS[val]++;
            }
        }
    }

  public:
    _MACRO_CLASS_NAME_(TYPE_BUFFER &buffer1, TYPE_BUFFER &buffer2)
      : BUFFER1(buffer1), BUFFER2(buffer2) {}

    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_

#endif
