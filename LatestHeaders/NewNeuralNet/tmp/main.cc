#include "./LatestHeaders/CPPFileIO/main.hh"

inline void show (
	Tensors::Array::N2D_ARRAY
		<2,2,float> const & in
) {
	printf("    %e %e \n    %e %e \n",in[0][0],in[0][1],in[1][0],in[1][1]);
}

int main () {

	Tensors::Array::N2D_ARRAY
		<2,2,float> a
	; /* Evaluate the a: */ {
		a[0][0] = 1 ;
		a[0][1] = 2 ;
		a[1][0] = 3 ;
		a[1][1] = 4 ;
	}

	Tensors::Array::N2D_ARRAY
		<2,2,float> b
	; /* Evaluate the b: */ {
		b[0][0] = 5 ;
		b[0][1] = 6 ;
		b[1][0] = 7 ;
		b[1][1] = 8 ;
	}


	Tensors::Array::N2D_ARRAY
		<2,2,float> c
	; //

	c = a*b.TRANSPOSE();

	show(c) ;

    return 0 ;
}
