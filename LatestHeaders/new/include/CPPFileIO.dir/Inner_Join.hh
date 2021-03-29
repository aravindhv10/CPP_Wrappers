#ifndef _HEADER_GUARD_CPPFileIO_Inner_Join_
#define _HEADER_GUARD_CPPFileIO_Inner_Join_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileWriter.hh"
//////////////////
// Headers END. //
//////////////////

template <typename T1, typename T2, typename O>
inline void Unique_Inner_Join(std::string const filename1,
                              std::string const filename2,
                              std::string const outfilename) {

    FileArray<T1> R1(filename1);
    FileArray<T2> R2(filename2);
    FileWriter<O> W(outfilename);

    size_t const L1 = R1();
    size_t const L2 = R2();
    size_t       I1 = 0;
    size_t       I2 = 0;

    while ((I1 < L1) && (I2 < L2)) {

        O          tmp;
        char const val = tmp(R1(I1), R2(I2));

        switch (val) {

            case 1: I2++; break;

            case 0: W(tmp);

            case -1: I1++; break;
        }
    }
}

template <typename T1, typename T2, typename O>
inline void Inner_Join(std::string const filename1, std::string const filename2,
                       std::string const outfilename) {

    FileArray<T1> R1(filename1);
    FileArray<T2> R2(filename2);
    FileWriter<O> W(outfilename);

    size_t const start1 = 0;
    size_t const start2 = 0;
    size_t const end1   = R1();
    size_t const end2   = R2();

    for (size_t i1 = start1; i1 < end1; i1++) {
        auto const &element1 = R1(i1);
        for (size_t i2 = start2; i2 < end2; i2++) {
            O           tmp;
            auto const &element2 = R2(i2);
            char const  cmp      = tmp(element1, element2);
            if (cmp == 0) { W(tmp); }
        }
    }
}

#endif
