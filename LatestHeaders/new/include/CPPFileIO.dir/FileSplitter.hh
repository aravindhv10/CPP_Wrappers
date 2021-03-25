#ifndef _HEADER_GUARD_CPPFileIO_FileSplitter_
#define _HEADER_GUARD_CPPFileIO_FileSplitter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileWriter.hh"
#include "./Atomic_Counter.hh"
//////////////////
// Headers END. //
//////////////////

template <typename T>
inline void SplitFile_old(std::string const               infilename,
                          std::vector<std::string> const &outnames) {

    FileArray<T const> reader(infilename);
    size_t const       limit = reader.size();
    for (size_t i = 0; i < outnames.size(); i++) {
        FileWriter<T> writer(outnames[i]);
        size_t const  begin = (limit * i) / outnames.size();       //
        size_t const  end   = (limit * (i + 1)) / outnames.size(); //
        for (size_t j = begin; j < end; j++) { writer(reader(j)); }
    }
}

template <typename T>
inline void SplitFile(std::string const               infilename,
                      std::vector<std::string> const &outnames,
                      size_t const NTH = 64) {

    size_t limit;
    /* Read the file size: */ {
        FileArray<T const> reader(infilename);
        limit = reader.size();
    }

    Atomic_Counter<size_t> counter;
    counter = 0 ;

#pragma omp parallel for
    for (size_t nth=0; nth<NTH;nth++) {

        FileArray<T const> reader(infilename);
        size_t i;

    MainLoop:

        i = counter();

        if(i<outnames.size() ) {
            FileWriter <T> writer(outnames[i]);
            size_t const  begin = (limit * i) / outnames.size();
            size_t const  end   = (limit * (i + 1)) / outnames.size();
            for (size_t j = begin; j < end; j++) { writer(reader(j)); }
            goto MainLoop;
        }
    }

}

#endif
