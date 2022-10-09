#include "./include/CPPFileIO.hh"

int main(int const argc, char const **argv) {
    std::vector<std::string> args;
    args.push_back("/usr/bin/clang++-12");
    for (int i = 1; i < argc; i++) { args.push_back(argv[i]); }
    args.push_back("-std=c++17");
    args.push_back("-Ofast");
    args.push_back("-mtune=native");
    args.push_back("-march=native");
    args.push_back("-fopenmp");
    args.push_back("-lomp5");
    CPPFileIO::starter_self(args);
}