#include "./include/CPPFileIO.hh"

int main(int const argc, char **argv) {
    switch (argc) {
        case 0:
        case 1:
            printf("Need atleast 1 argument...\n");
            printf("\tinput file\n");
            printf("\toutput file\n");
            break;
        case 2:
            CPPFileIO::csv_2_tsv(argv[1], std::string(argv[1]) + ".tsv");
            break;
        default: CPPFileIO::csv_2_tsv(argv[1], argv[2]); break;
    }
}
