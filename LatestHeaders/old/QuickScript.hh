CPPFileIO::SetCPUAffinity(1);
SlaveNetGrad<1>::START();
//SlaveNetGrad<1000>::EVAL();
SlaveNetGrad<1000>::TRAIN(1);
using namespace Tensors ;
using namespace Tensors::NN ;
using namespace Tensors::FUNCTIONS ;

/*
CBLAS_ORDER Order ;
printf("%d %d\n",sizeof(Order),sizeof(int));
C.MyGEMM((float)0.0,A,B,(float)1.0);
*/

N2D_ARRAY <2,2,float> A ;
A[0][0]=1.0;
A[0][1]=2.0;
A[1][0]=3.0;
A[1][1]=4.0;
N2D_ARRAY <2,2,float> B ;
B[0][0]=10.0;
B[0][1]=20.0;
B[1][0]=30.0;
B[1][1]=40.0;
N2D_ARRAY <2,2,float> C ;
N2D_ARRAY <2,2,float> D ;
D = 0.0 ;
for(size_t x=0;x<2;x++){
    for(size_t y=0;y<2;y++){
        for(size_t z=0;z<2;z++){
            D[x][y] += A[x][z]*B[z][y] ;
        }
    }
}
ShowMatrix(C);
ShowMatrix(D);
