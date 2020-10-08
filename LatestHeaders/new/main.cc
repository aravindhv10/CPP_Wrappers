#include "./include/CPPFileIO.hh"
#include "./include/StaticArray.hh"
#include "./include/Histograms.hh"
#include "./include/NewNeuralNet.hh"
#include "./include/PolarCoordinates.hh"
int main () {
	D2GPS_Coordinates <double> slave ;
	for(double x=-179.0;x<180.0;x=x+1.0){
		for(double y=-89.0;y<90.0;y=y+1.0){
			slave.latitude = y ;
			slave.longitude = x ;
			auto const theta = slave.THETA() ;
			auto const phi = slave.PHI() ;
			slave.SET_PHI(phi);
			slave.SET_THETA(theta);
			double const diff1 = slave.latitude - y ;
			double const diff2 = slave.longitude - x ;
			printf("FinalDiff = %e %e\n",diff1, diff2);

		}
	}

	return 0 ;
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
