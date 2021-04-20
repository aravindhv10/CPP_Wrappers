#ifndef _HEADER_GUARD_CPPFileIO_translate_csv_
#define _HEADER_GUARD_CPPFileIO_translate_csv_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../Headers.hh"
#include "./Basic.hh"
#include "./FileFD.hh"
#include "./FileArray.hh"
#include "./FullFileReader.hh"
#include "./FileWriter.hh"
///////////////////
// Headers END.} //
///////////////////

template <typename TYPE_INT>
inline void translate_csv(std::string const infile, std::string const outfile,
                          char const seperator) {
    FullFileReader<char>       reader(infile);
    FileWriter<char, TYPE_INT> writer(outfile);

    unsigned char constexpr REGULAR = 1;
    unsigned char constexpr ESCAPED = 2;
    unsigned char status            = REGULAR;

    for (size_t i = 0; i < reader(); i++) {

        char const &buf = reader(i);

        switch (status) {
            case REGULAR:
                switch (buf) {
                    case ',': writer(seperator); break;
                    case '"':
                        status = ESCAPED;
                        // writer(buf);
                        break;
                    default: writer(buf); break;
                }
                break;
            case ESCAPED:
                switch (buf) {
                    case '"':
                        status = REGULAR;
                        // writer(buf);
                        break;
                    default: writer(buf); break;
                }
                break;
        }
    }
}

template <typename TYPE_INT = TYPE_U64>
inline void csv_2_tsv(std::string const infile, std::string const outfile) {
    translate_csv<TYPE_INT>(infile, outfile, '\t');
}

template <typename TYPE_INT = TYPE_U64>
inline void csv_2_null(std::string const infile, std::string const outfile) {
    translate_csv<TYPE_INT>(infile, outfile, 0);
}

#endif
