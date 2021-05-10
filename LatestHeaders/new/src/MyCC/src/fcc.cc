#include "./include/CPPFileIO.hh"

int main(int const argc, char const **argv) {
    std::vector<std::string> args;
    args.push_back("/usr/bin/clang-format");
    args.push_back("-style=file");
    for (int i = 1; i < argc; i++) { args.push_back(argv[i]); }
    CPPFileIO::starter_self(args);
}
