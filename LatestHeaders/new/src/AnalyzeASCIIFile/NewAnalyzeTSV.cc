#include "./include/CPPFileIO.hh"

int main (int argc, char ** argv) {
	if(argc<2){
		printf("Not using correctly...\n");
		return 1;
	}
	std::string const infilename = argv[1];
	std::string const outfile = infilename + ".out" ;
	CPPFileIO::AnalyzeSlave::PrepareFileSchema<'\t','\n'>(infilename,outfile,16,4);
	return 0 ;
}
