#include "./include/CPPFileIO.hh"
#include "./include/StaticArray.hh"
#include "./include/Histograms.hh"
#include "./include/NewNeuralNet.hh"
int main () {
//	testfunc();
	MyHist <10,float> hist(1,100) ;

	StaticArray::N4D_ARRAY <5,5,5,5,float> outs ;
	/**/ {
	CPPFileIO::FileWriter <char> writer ("./dabba") ;
	writer('a');
	writer('1');
	writer('2');
	writer('3');
	writer('4');
	writer('5');
	writer('6');
	}
	return 0 ;
}
