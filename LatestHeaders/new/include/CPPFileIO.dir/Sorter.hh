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

template <typename T>
inline void MergeFileDeDup(std::string const file1, std::string const file2,
                           std::string const fileo) {

    FileArray<T>  reader1(file1);
    FileArray<T>  reader2(file2);
    FileWriter<T> writer(fileo);

    size_t const limit1 = reader1.size();
    size_t const limit2 = reader2.size();

    size_t i1 = 0;
    size_t i2 = 0;

    T current1, previous1, current2, previous2;

    if ((i1 < limit1) && (i2 < limit2)) {
        current1 = reader1(i1);
        current2 = reader2(i2);
        i1++;
        i2++;
    } else {
        goto LoopEnd;
    }

LABEL_CONTINUE : {
    char const val = (current1 > current2) - (current1 < current2);

    switch (val) {
        case 1:
            writer(current2);
            goto LABEL_GET2;

        case 0:
            writer(current1);
            goto LABEL_GETBOTH;

        case -1:
            writer(current1);
            goto LABEL_GET1;
    }
}

LABEL_GET1:
    if (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        i1++;
        if (previous1 != current1) {
            goto LABEL_CONTINUE;
        } else {
            goto LABEL_GET1;
        }
    } else {
        goto LoopEnd;
    }

LABEL_GET2:
    if (i2 < limit2) {
        previous2 = current2;
        current2  = reader2(i2);
        i2++;
        if (previous2 != current2) {
            goto LABEL_CONTINUE;
        } else {
            goto LABEL_GET2;
        }
    } else {
        goto LoopEnd;
    }

LABEL_GETBOTH:
    if (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        i1++;
        if (previous1 != current1) {
            goto LABEL_GET2;
        } else {
            goto LABEL_GETBOTH;
        }
    } else {
        goto LoopEnd;
    }

LoopEnd :
    while (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        if (previous1 != current1) { writer(current1); }
        i1++;
    }

    while (i2 < limit2) {
        previous2 = current2;
        current2  = reader2(i2);
        if (previous2 != current2) { writer(current2); }
        i2++;
    }

}

template <typename T, typename T2>
inline void MergeFileDeDup(std::string const file1, std::string const file2,
                           std::string const fileo, T2 compare) {

    FileArray<T>  reader1(file1);
    FileArray<T>  reader2(file2);
    FileWriter<T> writer(fileo);

    size_t const limit1 = reader1.size();
    size_t const limit2 = reader2.size();

    size_t i1 = 0;
    size_t i2 = 0;

    T current1, previous1, current2, previous2;

    if ((i1 < limit1) && (i2 < limit2)) {
        current1 = reader1(i1);
        current2 = reader2(i2);
        i1++;
        i2++;
    } else {
        goto LoopEnd;
    }

LABEL_CONTINUE : {
    char const val = compare(current1, current2);

    switch (val) {
        case 1:
            writer(current2);
            goto LABEL_GET2;

        case 0:
            writer(current1);
            goto LABEL_GETBOTH;

        case -1:
            writer(current1);
            goto LABEL_GET1;
    }
}

LABEL_GET1:
    if (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        i1++;
        if (previous1 != current1) {
            goto LABEL_CONTINUE;
        } else {
            goto LABEL_GET1;
        }
    } else {
        goto LoopEnd;
    }

LABEL_GET2:
    if (i2 < limit2) {
        previous2 = current2;
        current2  = reader2(i2);
        i2++;
        if (previous2 != current2) {
            goto LABEL_CONTINUE;
        } else {
            goto LABEL_GET2;
        }
    } else {
        goto LoopEnd;
    }

LABEL_GETBOTH:
    if (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        i1++;
        if (previous1 != current1) {
            goto LABEL_GET2;
        } else {
            goto LABEL_GETBOTH;
        }
    } else {
        goto LoopEnd;
    }

LoopEnd :
    while (i1 < limit1) {
        previous1 = current1;
        current1  = reader1(i1);
        if (previous1 != current1) { writer(current1); }
        i1++;
    }

    while (i2 < limit2) {
        previous2 = current2;
        current2  = reader2(i2);
        if (previous2 != current2) { writer(current2); }
        i2++;
    }

}

#endif
