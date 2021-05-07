#ifndef _HEADER_GUARD_CPPFileIO_Left_Join_
#define _HEADER_GUARD_CPPFileIO_Left_Join_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileWriter.hh"
//////////////////
// Headers END. //
//////////////////

template <typename T1, typename T2, typename O>
inline void Left_Join(std::string const filename1, std::string const filename2,
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
            case 1:
              I2++;
              break;

            case 0:
            case -1:
              W(tmp);
              I1++;
              break;
        }
    }
}

#endif
