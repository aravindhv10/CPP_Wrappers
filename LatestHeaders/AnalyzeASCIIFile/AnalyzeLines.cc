#include "./LatestHeaders/CPPFileIO/main.hh"

int main (int argc, char ** argv) {
	CPPFileIO::GetArgs const args(argc,argv) ;
	CPPFileIO::AnalyzeLines analyzer ;
	if(args()<2){printf("FAILED!!! you need to provide input file.\n");return 1;}
	CPPFileIO::FasterLineReader<'\t','\n'> read(args(1));
	analyzer.Read_Labels(read.next());
	bool loopover = true ;
	while(loopover){
		auto const & lines = read.next();
		loopover = (lines.size()>0) ;
		if(loopover){
			analyzer (lines) ;
		}
	}
	analyzer.show();
	return 0 ;
}
