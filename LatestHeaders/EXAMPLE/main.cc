#include "HEP_MATRIX/main.hh"

int main () {
	using TYPE_COMPLEX_DATA = std::complex<double> ;
	Tensors::NN::N2D_ARRAY <2,2,TYPE_COMPLEX_DATA> matrix ;
	matrix[0][0] = TYPE_COMPLEX_DATA(1.0,10.0) ;
	matrix[0][1] = TYPE_COMPLEX_DATA(2.0,20.0) ;
	matrix[1][0] = TYPE_COMPLEX_DATA(3.0,30.0) ;
	matrix[1][1] = TYPE_COMPLEX_DATA(4.0,40.0) ;
	auto ret = Tensors::FUNCTIONS::INVERT_MATRIX(matrix) ;
	Tensors::FUNCTIONS::ShowMatrix(matrix);
	printf("\n");
	Tensors::FUNCTIONS::ShowMatrix(ret);
	printf("\n");
	auto ret2 = matrix * ret ;
	Tensors::FUNCTIONS::ShowMatrix(ret2);
	printf("\n");

	VectorLoop <double> slave ;
	slave.EVAL(1.0,1.0,1.0,slave.slave.GET_VECTOR(10.0,0.0,0.0,+3.0),slave.slave.GET_VECTOR(10.0,0.0,0.0,-3.0));

    return 0 ;
}
