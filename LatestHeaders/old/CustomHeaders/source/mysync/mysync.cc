#include "CPPFileIO.hh"
int main (int argc, char ** argv) {
    std::vector <std::string> progs ;

    /* Run Rsync */ {
        CPPFileIO::ForkMe forker ;
        if (forker.InKid()) {
            progs.push_back("/usr/bin/rsync");
            progs.push_back("-avh");
            progs.push_back("--progress");
            for(int i=1;i<argc;i++) {progs.push_back(argv[i]);}
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
