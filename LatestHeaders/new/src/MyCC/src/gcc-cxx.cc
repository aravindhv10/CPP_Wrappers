#include "./include/CPPFileIO.hh"

int main(int const argc, char const **argv) {
    std::vector<std::string> args;
    args.push_back("/usr/bin/g++");
    for (int i = 1; i < argc; i++) { args.push_back(argv[i]); }
    args.push_back("-std=c++17");
    args.push_back("-Ofast");
    args.push_back("-mtune=native");
    args.push_back("-march=native");
    CPPFileIO::starter_self(args);
}
