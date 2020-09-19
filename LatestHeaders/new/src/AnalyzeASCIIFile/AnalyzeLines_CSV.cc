#include "./include/CPPFileIO.hh"

int main (int argc, char ** argv) {
	CPPFileIO::GetArgs const args(argc,argv) ;
	CPPFileIO::AnalyzeLines analyzer ;
	if (args()<2) {
		printf("FAILED!!! you need to provide input file.\n");
		return 1;
	}
	CPPFileIO::FasterLineReader <',','\n'> read (args(1)) ;
	analyzer.Read_Labels(read());
	MainLoop: {
		auto const & lines = read() ;
		if(lines.size()>0){
			analyzer(lines);
			goto MainLoop;
		}
	}
	analyzer.show();
	return 0 ;
}
