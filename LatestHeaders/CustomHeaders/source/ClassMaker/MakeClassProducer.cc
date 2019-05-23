#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

int main (int argc, char **argv) {
    gSystem->Load("libDelphes");
    TFile rootfile(argv[1]);
    TTree * tree = (TTree*) rootfile.Get("Delphes");
    tree->MakeClass(argv[2]) ;
    return 0;
}
