#include "./include/StaticArray.hh"

int main () {
	CPPFileIO::Dynamic1DArray <double> data (10) ;
	for(size_t i=0;i<data();i++){
		data(i) = i * i * 1.654 / std::sqrt(i+1) ;
	}
	for(size_t i=0;i<10;i++){
		printf("%e\n",data(i));
	}
	auto data2 = CPPFileIO::GetNormalized (data) ;
	double sum[2] = {0,0} ;
	for(size_t i=0;i<10;i++){
		sum[0] += data2(i) ;
		sum[1] += std::pow(data2(i),2) ;
		printf("%e\n",data2(i));
	}
	double mu = sum[0] / static_cast <double> (data2());
	printf("mu = %e %e %e\n",sum[0],sum[1],data2*data2);

	return 0 ;
}

