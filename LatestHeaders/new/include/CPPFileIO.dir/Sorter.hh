#ifndef _HEADER_GUARD_CPPFileIO_Sorter_
#define _HEADER_GUARD_CPPFileIO_Sorter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
#include "./Basic.hh"
#include "./FileArray.hh"
#include "./FileFD.hh"
#include "./FileWriter.hh"
//////////////////
// Headers END. //
//////////////////

template <typename T> inline void SortFile(std::string const filename) {
    FileArray<T> buffer(filename);
    size_t const limit = buffer.size();
    buffer.writeable(true);
    T *data = &(buffer(0, limit));
    std::sort(&(data[0]), &(data[limit]));
    buffer.size(limit);
}

template <typename T>
inline void MergeFile(std::string const file1, std::string const file2,
                      std::string const fileo) {

    FileArray<T>  reader1(file1);
    FileArray<T>  reader2(file2);
    FileWriter<T> writer(fileo);

    size_t const limit1 = reader1.size();
    size_t const limit2 = reader2.size();

    size_t i1 = 0;
    size_t i2 = 0;

    while ((i1 < limit1) && (i2 < limit2)) {
        T const &V1 = reader1(i1);
        T const &V2 = reader2(i2);

        if (V1 < V2) {
            writer(V1);
            i1++;
        } else {
            writer(V2);
            i2++;
        }
    }

    while (i1 < limit1) {
        writer(reader1(i1));
        i1++;
    }

    while (i2 < limit2) {
        writer(reader2(i2));
        i2++;
    }
}

#define _MACRO_CLASS_NAME_ MergeFileDeDup
template <typename TD, typename TI = long> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_DATA   = TD;
    using TYPE_INT    = TI;
    using TYPE_SELF   = _MACRO_CLASS_NAME_<TYPE_DATA, TYPE_INT>;
    using TYPE_READER = FileArray<TYPE_DATA>;
    using TYPE_WRITER = FileWriter<TYPE_DATA>;

  private:
    std::string const F1, F2, F0;
    TYPE_READER       R1, R2;
    TYPE_WRITER       W;
    size_t const      L1, L2;

    TYPE_DATA P;

  private:
    inline void WRITE(TYPE_DATA const &in) {
        if (in != P) {
            W(in);
            P = in;
        }
    }

    inline void WRITE() {
        size_t i1 = 0;
        size_t i2 = 0;

        while ((i1 < L1) && (i2 < L2)) {
            TYPE_DATA const &V1 = R1(i1);
            TYPE_DATA const &V2 = R2(i2);

            if (V1 < V2) {
                WRITE(V1);
                i1++;
            } else {
                WRITE(V2);
                i2++;
            }
        }

        while (i1 < L1) {
            WRITE(R1(i1));
            i1++;
        }

        while (i2 < L2) {
            WRITE(R2(i2));
            i2++;
        }
    }

  private:
    _MACRO_CLASS_NAME_(std::string const f1, std::string const f2,
                       std::string const f0)
      : F1(f1), F2(f2), F0(f0), R1(F1), R2(F2), W(F0), L1(R1()), L2(R2()) {}

    ~_MACRO_CLASS_NAME_() {}

  public:
    static inline void WORK(std::string const f1, std::string const f2,
                            std::string const f0) {
        TYPE_SELF slave(f1, f2, f0);
        slave.WRITE();
    }
};
#undef _MACRO_CLASS_NAME_

#endif
