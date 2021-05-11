#include "./include/CPPFileIO.hh"

int main(int const argc, char const **argv) {
    std::vector<std::string> args;
    args.push_back("/lib64/ld-linux-x86-64.so.2");
    args.push_back("--library-path");
    args.push_back(".");
    args.push_back("./exe");
    for (int i = 1; i < argc; i++) { args.push_back(argv[i]); }
    CPPFileIO::starter_self(args);
}
