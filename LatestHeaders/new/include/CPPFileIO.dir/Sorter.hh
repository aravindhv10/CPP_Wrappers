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
    size_t const  limit1 = reader1.size();
    size_t const  limit2 = reader2.size();
    size_t        i1     = 0;
    size_t        i2     = 0;
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

#endif
