#include "CPPFileIO.hh"
int main (int argc, char ** argv) {
    std::vector <std::string> progs ;

    /* Run Rsync */ {
        CPPFileIO::ForkMe forker ;
        if (forker.InKid()) {
            progs.push_back("/usr/bin/mksquashfs");
            for(int i=1;i<argc;i++) {progs.push_back(argv[i]);}
            progs.push_back("-always-use-fragments");
            progs.push_back("-keep-as-directory");
            progs.push_back("-comp");
            progs.push_back("xz");
            progs.push_back("-b");
            progs.push_back("1048576");
            CPPFileIO::starter_self(progs);   
        }
    }

    /* Run sync */ {
        CPPFileIO::ForkMe forker ;
        if (forker.InKid()) {
            progs.push_back("/bin/sync");
            CPPFileIO::starter_self(progs);   
        }
    }

    return 0 ;
}
