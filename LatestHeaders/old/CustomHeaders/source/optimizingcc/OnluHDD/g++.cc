#include "CPPFileIO.hh"
int main (int argc, char ** argv) {
    std::vector <std::string> progs ;
    progs.push_back("/usr/bin/g++");
    for(int i=1;i<argc;i++) {progs.push_back(argv[i]);}
//    progs.push_back("-Ofast");
    progs.push_back("-mtune=native");
    progs.push_back("-march=native");
    CPPFileIO::starter_self(progs);
    return 0 ;
}
