namespace Tensors {

    namespace FUNCTIONS /* The neural network multiplication function: */ {
        using namespace NN ;

        // Function to evaluate neural network activation:
        template <
            size_t B , size_t   Y ,
            size_t X , typename T
        > inline void NeuralMultiply (
            N2D_ARRAY < B , Y , T >       & Output ,
            N2D_ARRAY < Y , X , T > const & Weight ,
            N2D_ARRAY < B , X , T > const & Input ,
            ND_ARRAY  < Y ,     T > const & Bias
        ) {
            Output = Bias ;
            for(size_t b=0;b<B;b++)
            for(size_t y=0;y<Y;y++){
                Output[b][y] +=
                    Weight[y] *
                    Input[b]
                ; //
            }
            //
        }

//////////////////////////////////////////////////////////////////
#ifdef CBLAS_H                                                  //
#define _MACRO_BLAS_NeuralMultiply_(FLOAT_TYPE)                 \
        template < size_t B , size_t Y , size_t X >             \
        inline void NeuralMultiply (                            \
            N2D_ARRAY < B , Y , FLOAT_TYPE >       & Output ,   \
            N2D_ARRAY < Y , X , FLOAT_TYPE > const & Weight ,   \
            N2D_ARRAY < B , X , FLOAT_TYPE > const & Input  ,   \
            ND_ARRAY  < Y ,     FLOAT_TYPE > const & Bias       \
        ) {                                                     \
            Output = Bias ;                                     \
            Output.MyGEMM (                                     \
                1.0,                                            \
                Input, Weight.MARK_TRANSPOSED(), 1.0            \
            ) ;                                                 \
        }                                                       //
//////////////////////////////////////////////////////////////////
_MACRO_BLAS_NeuralMultiply_(float)                              //
_MACRO_BLAS_NeuralMultiply_(double)                             //
#undef _MACRO_BLAS_NeuralMultiply_                              //
#endif                                                          //
//////////////////////////////////////////////////////////////////

    }

    namespace FUNCTIONS /* Defining EvalW for Backpropagation: */ {
        // Functions for evaluating changes in weight:
        template <
            size_t Y , size_t   X ,
            size_t B , typename T
        > inline void EvalW (
            N2D_ARRAY < Y , X , T >       & DW    ,
            N2D_ARRAY < B , Y , T > const & Delta ,
            N2D_ARRAY < B , X , T > const & Input
        ) {
            if(false){
                for(size_t b=0;b<B;b++){
                    for(size_t y=0;y<Y;y++){
                        for(size_t x=0;x<X;x++) {
                            DW[y][x] +=
                                Delta[b][y] *
                                Input[b][x]
                            ; //
                        }
                    }
                }
            } else if (false) {
                size_t constexpr SKIP_X = SKIP_SIZE ;
                for(size_t b=0;b<B;b++){
                    for(size_t x=0;x<X;x=x+SKIP_X) {
                        for(size_t y=0;y<Y;y++){
                            for(size_t x2=x;(x2<(x+SKIP_X))&&(x2<X);x2++){
                                DW[y][x2] +=
                                    Delta[b][y] *
                                    Input[b][x2]
                                ; //
                            }
                        }
                    }
                }
            } else {
				DW.MULTIPLY (
					Delta.MARK_TRANSPOSED() ,
					Input
				) ; //
			}
        }
        #ifdef CBLAS_H
        template <
            size_t Y ,
            size_t X ,
            size_t B
        > inline void EvalW (
            N2D_ARRAY < Y , X , float >       & DW    ,
            N2D_ARRAY < B , Y , float > const & Delta ,
            N2D_ARRAY < B , X , float > const & Input
        ) {
            cblas_sgemm (
                CblasRowMajor, CblasTrans, CblasNoTrans,
                (const int) /*M=*/Y, (const int) /*N=*/X, (const int) /*K=*/B,
                (const float) /*alpha=*/1.0,
                (const float *) Delta.GET_DATA(), (const int) /*lda=*/Y,
                (const float *) Input.GET_DATA(), (const int) /*ldb=*/X,
                (const float) /*beta=*/1.0,
                (float *) DW.GET_DATA(), (const int) /*ldc=*/X
            ) ;
        }
        template <
            size_t Y ,
            size_t X ,
            size_t B
        > inline void EvalW (
            N2D_ARRAY < Y , X , double >       & DW    ,
            N2D_ARRAY < B , Y , double > const & Delta ,
            N2D_ARRAY < B , X , double > const & Input
        ) {
            cblas_dgemm (
                CblasRowMajor, CblasTrans, CblasNoTrans,
                (const int) /*M=*/Y, (const int) /*N=*/X, (const int) /*K=*/B,
                (const double) /*alpha=*/1.0,
                (const double *) Delta.GET_DATA(), (const int) /*lda=*/Y,
                (const double *) Input.GET_DATA(), (const int) /*ldb=*/X,
                (const double) /*beta=*/1.0,
                (double *) DW.GET_DATA(), (const int) /*ldc=*/X
            ) ;
        }
        #endif
    }

    namespace FUNCTIONS /* Defining the DELTA_PRIME evaluation: */ {
        // function to evaluate DELTA_PRIME of the back propagation chain:
        template <
            size_t   B , size_t   X ,
            size_t   Y , typename T
        > inline void BackProp (
            N2D_ARRAY < B , X , T >       & DeltaPrime ,
            N2D_ARRAY < B , Y , T > const & Delta      ,
            N2D_ARRAY < Y , X , T > const & Weight
        ) {
            DeltaPrime = 0 ;
            if(false){
                for(size_t b=0;b<B;b++){
                    for(size_t y=0;y<Y;y++){
                        for(size_t x=0;x<X;x++){
                            DeltaPrime[b][x] +=
                                Delta[b][y] *
                                Weight[y][x]
                            ; //
                        }
                    }
                }
            } else if (false) {
                size_t constexpr SKIP_X = SKIP_SIZE ;
                for(size_t b=0;b<B;b++){
                    for(size_t x=0;x<X;x=x+SKIP_X){
                        for(size_t y=0;y<Y;y++){
                            for(size_t x2=x;(x2<x+SKIP_X)&&(x2<X);x2++) {
                                DeltaPrime[b][x2] +=
                                    Delta[b][y] *
                                    Weight[y][x2]
                                ; //
                            }
                        }
                    }
                }
            } else {
				DeltaPrime.MULTIPLY(Delta,Weight);
			}

        }
        #ifdef CBLAS_H
        template <
            size_t B ,
            size_t X ,
            size_t Y
        > inline void BackProp (
            N2D_ARRAY < B , X , float >       & DeltaPrime ,
            N2D_ARRAY < B , Y , float > const & Delta      ,
            N2D_ARRAY < Y , X , float > const & Weight
        ) {
            cblas_sgemm (
                CblasRowMajor, CblasNoTrans, CblasNoTrans,
                (const int) /*M=*/B, (const int) /*N=*/X, (const int) /*K=*/Y,
                (const float) /*alpha=*/1.0,
                (const float *) Delta.GET_DATA(), (const int) /*lda=*/Y,
                (const float *) Weight.GET_DATA(), (const int) /*ldb=*/X,
                (const float) /*beta=*/0,
                (float *) DeltaPrime.GET_DATA(), (const int) /*ldc=*/X
            );
        }
        template <
            size_t B ,
            size_t X ,
            size_t Y
        > inline void BackProp (
            N2D_ARRAY < B , X , double >       & DeltaPrime ,
            N2D_ARRAY < B , Y , double > const & Delta      ,
            N2D_ARRAY < Y , X , double > const & Weight
        ) {
            cblas_dgemm (
                CblasRowMajor, CblasNoTrans, CblasNoTrans,
                (const int) /*M=*/B, (const int) /*N=*/X , (const int) /*K=*/Y,
                (const double) /*alpha=*/1.0 ,
                (const double *) Delta.GET_DATA () , (const int) /*lda=*/Y,
                (const double *) Weight.GET_DATA () , (const int) /*ldb=*/X,
                (const double) /*beta=*/0,
                (double *) DeltaPrime.GET_DATA () , (const int) /*ldc=*/X
            );
        }
        #endif
    }

    namespace FUNCTIONS /* The Gradient Propagator: */ {
        template <
            size_t   B , size_t   M ,
            size_t   Y , size_t   X ,
            typename T
        > inline void
        GRAD_PROPAGATE (
            N3D_ARRAY < B , Y , M , T >       & OK ,
            N2D_ARRAY < Y , X ,     T > const & WK ,
            N3D_ARRAY < B , X , M , T > const & IK
        ) {
            OK=0.0;
            if(false){
                for(size_t b=0;b<B;b++){
                    for(size_t y=0;y<Y;y++){
                        for(size_t x=0;x<X;x++){
                            for(size_t m=0;m<M;m++){
                                OK[b][y][m] +=
                                    WK[y][x]    *
                                    IK[b][x][m]
                                ; //
                            }
                        }
                    }
                }
            } else if(false) {
                size_t constexpr SKIP_M = SKIP_SIZE ;
                for(size_t b=0;b<B;b++){
                    for(size_t y=0;y<Y;y++){
                        for(size_t m=0;m<M;m=m+SKIP_M){
                            for(size_t x=0;x<X;x++){
                                for(size_t m2=m;(m2<(m+SKIP_M))&&(m2<M);m2++){
                                    OK[b][y][m2] +=
                                        WK[y][x]    *
                                        IK[b][x][m2]
                                    ; //
                                }
                            }
                        }
                    }
                }
            } else {
				for(size_t b=0;b<B;b++){
					OK[b].MULTIPLY(WK,IK[b]);
				}
			}

        }
        //
        #ifdef CBLAS_H
        template <
            size_t   B , size_t   M ,
            size_t   Y , size_t   X
        > inline void
        GRAD_PROPAGATE (
            N3D_ARRAY < B , Y , M , float >       & OK ,
            N2D_ARRAY < Y , X ,     float > const & WK ,
            N3D_ARRAY < B , X , M , float > const & IK
        ) {
            for(size_t b=0;b<B;b++){
                cblas_sgemm (
                    CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    (const int) /*M=*/Y, (const int) /*N=*/M, (const int) /*K=*/X,
                    (const float) /*alpha=*/1.0,
                    (const float *) WK.GET_DATA(), (const int) /*lda=*/X,
                    (const float *) IK[b].GET_DATA(), (const int) /*ldb=*/M,
                    (const float) /*beta=*/0,
                    (float *) OK[b].GET_DATA(), (const int) /*ldc=*/M
                ) ;
            }
        }
        template <
            size_t   B , size_t   M ,
            size_t   Y , size_t   X
        > inline void
        GRAD_PROPAGATE (
            N3D_ARRAY < B , Y , M , double >       & OK ,
            N2D_ARRAY < Y , X ,     double > const & WK ,
            N3D_ARRAY < B , X , M , double > const & IK
        ) {
            for(size_t b=0;b<B;b++){
                cblas_dgemm (
                    CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    (const int) /*M=*/Y, (const int) /*N=*/M, (const int) /*K=*/X,
                    (const double) /*alpha=*/1.0,
                    (const double *) WK.GET_DATA(), (const int) /*lda=*/X,
                    (const double *) IK[b].GET_DATA(), (const int) /*ldb=*/M,
                    (const double) /*beta=*/0,
                    (double *) OK[b].GET_DATA(), (const int) /*ldc=*/M
                ) ;
            }
        }
        #endif
        //
        template <
            size_t   B , size_t   M ,
            size_t   Y , size_t   X ,
            typename T
        > inline void
        GRAD_PROPAGATE (
            N3D_ARRAY < B , Y , M , T >       & OK ,
            N2D_ARRAY < Y , X ,     T > const & WK ,
            bool                        const   IK
        ) {
            for(size_t b=0;b<B;b++)
            { OK[b] = WK ; }
        }
    }

    namespace FUNCTIONS /* Function for back propagation on gradient */ {
        //
        template <
            size_t   B , size_t   X ,
            size_t   Y , size_t   M ,
            typename T
        > inline void GRAD_BACKWARD (
            N3D_ARRAY < B , X , M , T >       & DELTA_PRIME ,
            N2D_ARRAY <     Y , X , T > const & WEIGHT      ,
            N3D_ARRAY < B , Y , M , T > const & DELTA
        ) {
            DELTA_PRIME = 0.0 ;
            if(false){
                for(size_t b=0;b<DELTA_PRIME.SIZE_Z();b++) {
                    for(size_t x=0;x<DELTA_PRIME.SIZE_Y();x++) {
                        for(size_t y=0;y<DELTA.SIZE_Y();y++) {
                            for(size_t m=0;m<DELTA_PRIME.SIZE_X();m++) {
                                DELTA_PRIME[b][x][m] +=
                                    WEIGHT[y][x] *
                                    DELTA[b][y][m]
                                ; //
                            }
                        }
                    }
                }
            } else if (false) {
                size_t constexpr SKIP_M = SKIP_SIZE ;
                for(size_t b=0;b<DELTA_PRIME.SIZE_Z();b++) {
                    for(size_t x=0;x<DELTA_PRIME.SIZE_Y();x++) {
                        for(size_t m=0;m<DELTA_PRIME.SIZE_X();m=m+SKIP_M) {
                            for(size_t y=0;y<DELTA.SIZE_Y();y++) {
                                for(size_t m2=m;(m2<(SKIP_M+m))&&(m2<DELTA_PRIME.SIZE_X());m2++){
                                    DELTA_PRIME[b][x][m2] +=
                                        WEIGHT[y][x] *
                                        DELTA[b][y][m2]
                                    ; //
                                }
                            }
                        }
                    }
                }
            } else {
				for(size_t b=0;b<DELTA_PRIME.SIZE_Z();b++) {
					DELTA_PRIME[b].MULTIPLY (
						WEIGHT.MARK_TRANSPOSED(),
						DELTA[b]
					) ;
				}
			}
        }
        //
        #ifdef CBLAS_H
        template <
            size_t   B , size_t   X ,
            size_t   Y , size_t   M
        > inline void GRAD_BACKWARD (
            N3D_ARRAY < B , X , M , float >       & DELTA_PRIME ,
            N2D_ARRAY <     Y , X , float > const & WEIGHT      ,
            N3D_ARRAY < B , Y , M , float > const & DELTA
        ) {
            for(
                size_t b=0;
                b<DELTA_PRIME.SIZE_Z();
                b++
            ) {
                cblas_sgemm (
                    CblasRowMajor, CblasTrans, CblasNoTrans,
                    (const int) /*M=*/X , (const int) /*N=*/M , (const int) /*K=*/Y ,
                    (const float) /*alpha=*/1.0 ,
                    (const float *) WEIGHT.GET_DATA   () , (const int) /*lda=*/X ,
                    (const float *) DELTA[b].GET_DATA () , (const int) /*ldb=*/M ,
                    (const float) /*beta=*/0 ,
                    (float *) DELTA_PRIME[b].GET_DATA() , (const int) /*ldc=*/M
                ) ;
            }
        }
        template <
            size_t   B , size_t   X ,
            size_t   Y , size_t   M
        > inline void GRAD_BACKWARD (
            N3D_ARRAY < B , X , M , double >       & DELTA_PRIME ,
            N2D_ARRAY <     Y , X , double > const & WEIGHT      ,
            N3D_ARRAY < B , Y , M , double > const & DELTA
        ) {
            for(
                size_t b=0;
                b<DELTA_PRIME.SIZE_Z();
                b++
            ) {
                cblas_dgemm (
                    CblasRowMajor, CblasTrans, CblasNoTrans,
                    (const int) /*M=*/X , (const int) /*N=*/M , (const int) /*K=*/Y ,
                    (const double) /*alpha=*/1.0 ,
                    (const double *) WEIGHT.GET_DATA   () , (const int) /*lda=*/X ,
                    (const double *) DELTA[b].GET_DATA () , (const int) /*ldb=*/M ,
                    (const double) /*beta=*/0 ,
                    (double *) DELTA_PRIME[b].GET_DATA() , (const int) /*ldc=*/M
                ) ;
            }
        }
        #endif
        //
    }

    namespace FUNCTIONS /* Function to evaluate change to W with grad */ {
        //
        template <
            size_t   Y , size_t   X ,
            size_t   M , typename T
        > inline void
        EVAL_DW_GRAD (
            N2D_ARRAY < Y , X , T >       & DW      ,
            N2D_ARRAY < Y , M , T > const & DELTA   ,
            N2D_ARRAY < X , M , T > const & GRAD_IN
        ) {
            DW = 0.0 ;
            for(size_t y=0;y<Y;y++)
			for(size_t x=0;x<X;x++){
				DW[y][x] =
					DELTA[y] *
					GRAD_IN[x]
				; //
			}

        }
        //
        #ifdef CBLAS_H
        //
        template <
            size_t   Y , size_t   X ,
            size_t   M
        > inline void
        EVAL_DW_GRAD (
            N2D_ARRAY < Y , X , float >       & DW      ,
            N2D_ARRAY < Y , M , float > const & DELTA   ,
            N2D_ARRAY < X , M , float > const & GRAD_IN
        ) {
            cblas_sgemm (
                CblasRowMajor, CblasNoTrans, CblasTrans,
                (const int) /*M=*/Y , (const int) /*N=*/X , (const int) /*K=*/M ,
                (const float) /*alpha=*/1.0 ,
                (const float *) DELTA.GET_DATA   () , (const int) /*lda=*/M ,
                (const float *) GRAD_IN.GET_DATA () , (const int) /*ldb=*/M ,
                (const float) /*beta=*/0 ,
                (float *) DW.GET_DATA() , (const int) /*ldc=*/X
            ) ;
        }
        //
        template <
            size_t   Y , size_t   X ,
            size_t   M
        > inline void
        EVAL_DW_GRAD (
            N2D_ARRAY < Y , X , double >       & DW      ,
            N2D_ARRAY < Y , M , double > const & DELTA   ,
            N2D_ARRAY < X , M , double > const & GRAD_IN
        ) {
            cblas_dgemm (
                CblasRowMajor, CblasNoTrans, CblasTrans,
                (const int) /*M=*/Y , (const int) /*N=*/X , (const int) /*K=*/M ,
                (const double) /*alpha=*/1.0 ,
                (const double *) DELTA.GET_DATA   () , (const int) /*lda=*/M ,
                (const double *) GRAD_IN.GET_DATA () , (const int) /*ldb=*/M ,
                (const double) /*beta=*/0 ,
                (double *) DW.GET_DATA() , (const int) /*ldc=*/X
            ) ;
        }
        #endif
        //
        template <
            size_t   B , size_t   Y ,
            size_t   M , size_t   X ,
            typename T
        > inline void
        EVAL_DW_GRAD (
            N2D_ARRAY < Y , X ,     T >       & DW1     ,
            N2D_ARRAY < Y , X ,     T >       & DW2     ,
            N2D_ARRAY < Y , X ,     T >       & tmpDW   ,
            N3D_ARRAY < B , Y , M , T > const & DELTA   ,
            N3D_ARRAY < B , X , M , T > const & GRAD_IN
        ) {
            auto & tmp1 = DW1.FLATTEN   () ;
            auto & tmp2 = DW2.FLATTEN   () ;
            auto & tmp  = tmpDW.FLATTEN () ;
            for(size_t b=0;b<B;b++){
                //
                EVAL_DW_GRAD (
                /* DW = */      tmpDW       ,
                /* DELTA = */   DELTA[b]    ,
                /* GRAD_IN = */ GRAD_IN[b]
                ) ; //
                //
                for(size_t i=0;i<tmp1.SIZE();i++){
                    //
                    tmp1[i] +=
                        tmp[i]
                    ; //
                    //
                    tmp2[i] +=
                        tmp[i] *
                        tmp[i]
                    ; //
                    //
                }
            }
        }
        //
        template <
            size_t   B , size_t   Y ,
            size_t   M , size_t   X ,
            typename T
        > inline void
        EVAL_DW_GRAD (
            N2D_ARRAY < Y , X ,     T >       & DW1     ,
            N2D_ARRAY < Y , X ,     T >       & DW2     ,
            N2D_ARRAY < Y , X ,     T >       & tmpDW   ,
            N3D_ARRAY < B , Y , M , T > const & DELTA   ,
            bool                        const   GRAD_IN
        ) {
            auto & tmp1 = DW1.FLATTEN   () ;
            auto & tmp2 = DW2.FLATTEN   () ;
            for(size_t b=0;b<B;b++){
                auto & tmp =
                    DELTA[b].FLATTEN()
                ; //
                for(size_t i=0;i<tmp1.SIZE();i++){
                    tmp1[i] +=
                        tmp[i]
                    ; //
                    tmp2[i] +=
                        tmp[i] *
                        tmp[i]
                    ; //
                }
            }
        }
        //
    }

    namespace FUNCTIONS /* Show Functions */ {
        template <size_t Y, typename T>
        inline void
        ShowMatrix (
            ND_ARRAY <Y,T> const &
                in
        ) {
            for (size_t y=0;y<Y;y++) {
                printf("%e ",in[y]);
            }
            printf("\n");
        }
        template <size_t Y, size_t X, typename T>
        inline void ShowMatrix
        ( N2D_ARRAY <Y,X,T> const & in ) {
            for (size_t y=0;y<Y;y++) {
                for (size_t x=0;x<X;x++) {
                    printf("%e ",in[y][x]);
                }
                printf("\n");
            }
        }
    }

    namespace FUNCTIONS /* Show Functions */ {
        template <size_t Y, typename T>
        inline void ShowMatrix
        ( ND_ARRAY <Y,std::complex<T>> const & in ) {
            for (size_t y=0;y<Y;y++) {
                printf("(%e,%e) ",in[y].real(),in[y].imag());
            }
            printf("\n");
        }
        template <size_t Y, size_t X, typename T>
        inline void ShowMatrix
        ( N2D_ARRAY <Y,X,std::complex<T>> const & in ) {
            for (size_t y=0;y<Y;y++) {
                for (size_t x=0;x<X;x++) {
                    printf("(%e,%e) ",in[y][x].real(),in[y][x].imag());
                }
                printf("\n");
            }
        }
    }

//////////////////////////////////////////////////////////////////////////////////
#define _MACRO_INHERIT_(DefnName) typename TYPE_SELF::DefnName					//
#define _MACRO_DEFINE_(DefnName) using DefnName = _MACRO_INHERIT_(DefnName) ;	//
//////////////////////////////////////////////////////////////////////////////////

    namespace NN /* PARAMETER */ {
        template <
            size_t   Y ,
            size_t   X ,
            typename T
        > class PARAMETER {
        public:
            //
            typedef T TYPE_DATA ;
            //
            inline static constexpr size_t
            SIZE_X ()
            { return X ; }
            //
            inline static constexpr size_t
            SIZE_Y ()
            { return Y ; }
            //
            inline static size_t constexpr
            SIZE () {
                return
                    SIZE_X() *
                    SIZE_Y()
                ; //
            }
            //
            using TYPE_SELF =
                PARAMETER
                < SIZE_Y() , SIZE_X() , TYPE_DATA >
            ; //
            //
            using TYPE_WEIGHT =
                N2D_ARRAY
                < SIZE_Y() , SIZE_X() , TYPE_DATA >
            ; //
            //
            using TYPE_BIAS =
                ND_ARRAY
                < SIZE_Y() , TYPE_DATA >
            ; //
            //
            inline TYPE_DATA &
            operator ()
            ( size_t y , size_t x )
            { return WEIGHT[y][x] ; }
            //
            inline TYPE_DATA const
            operator ()
            ( size_t y , size_t x ) const
            { return WEIGHT[y][x] ; }
            //
            inline TYPE_DATA &
            operator ()
            (size_t y)
            { return BIAS[y] ; }
            //
            inline TYPE_DATA const
            operator ()
            (size_t y) const
            { return BIAS[y] ; }
            //
            inline void
            RANDOMIZE () {
                //
                constexpr TYPE_DATA Var =
                    2.0 /
                    ((TYPE_DATA)SIZE_X()+SIZE_Y())
                ; //
                std::normal_distribution
                <double>
                    dist (0.0,Var)
                ; //
                #ifdef PCG_RAND_HPP_INCLUDED
                    pcg64_fast engine ;
                #else
                    std::mt19937 engine ;
                #endif
                //
                for ( size_t y = 0 ; y < Y ; y++ ) {
                    for ( size_t x = 0 ; x < X ; x++ )
                    { WEIGHT[y][x] = dist(engine) ; }
                }
                for ( size_t y = 0 ; y < Y ; y++ )
                { BIAS[y] = dist(engine) ; }
                //
            }
            //
            inline void
            RELEASE_LOCK ()
            { pthread_mutex_unlock (&lock) ; }
            //
            inline void
            ACQUIRE_LOCK ()
            { pthread_mutex_lock (&lock) ; }
            //
            inline void
            operator =
            (TYPE_DATA const & a)
            { WEIGHT = a ; BIAS = a ; }
            //
            inline void
            operator /=
            (TYPE_DATA const & a)
            { WEIGHT /= a ; BIAS /= a ; }
            //
            inline void
            operator *=
            (TYPE_DATA const & a)
            { WEIGHT *= a ; BIAS *= a ; }
            //
            inline void
            operator +=
            (TYPE_DATA const & a)
            { WEIGHT += a ; BIAS += a ; }
            //
            inline void
            operator -=
            (TYPE_DATA const & a)
            { WEIGHT -= a ; BIAS -= a ; }
            //
            inline void
            operator =
            (TYPE_SELF const & other) {
                WEIGHT = other.WEIGHT ;
                BIAS = other.BIAS ;
            }
            //
        private:
            //
            inline ssize_t
            WriteToFile (
                CPPFileIO::FileFD &
                    file
            ) const {
                ssize_t ret = 0 ;
                ret += file.multiwrite2file
                    ( WEIGHT )
                ; //
                ret += file.multiwrite2file
                    ( BIAS )
                ; //
                return ret;
            }
            //
            inline ssize_t
            WriteToFile
            (std::string filename) const {
                CPPFileIO::FileFD Writer
                    (filename) ;
                //
                Writer.writefile();
                return WriteToFile(Writer);
            }
            //
            inline ssize_t
            ReadFromFile
            (CPPFileIO::FileFD & file) {
                ssize_t ret = 0 ;
                ret += file.multiread2file
                    ( WEIGHT ) ;
                //
                ret += file.multiread2file
                    (   BIAS ) ;
                //
                return ret;
            }
            //
            inline ssize_t
            ReadFromFile
            (std::string filename) {
                CPPFileIO::FileFD Reader
                    (filename) ;
                //
                Reader.readfile();
                return ReadFromFile(Reader);
            }
            //
        public:
            //
            inline ssize_t
            operator >>
            ( CPPFileIO::FileFD & file ) const
            { return WriteToFile  (file) ; }
            //
            inline ssize_t
            operator <<
            ( CPPFileIO::FileFD & file )
            { return ReadFromFile (file) ; }
            //
            inline ssize_t
            operator >>
            ( std::string filename ) const
            { return WriteToFile (filename) ; }
            //
            inline ssize_t
            operator <<
            ( std::string filename )
            { return ReadFromFile (filename) ; }
            //
            TYPE_WEIGHT     WEIGHT ;
            TYPE_BIAS       BIAS   ;
            //
        private:
            pthread_mutex_t lock   ;
        } ;
    }

    namespace NN /* PARAMETER_ADAM */ {
        template <typename T>
        class PARAMETER_ADAM : public T {
        public:
            //
            using TYPE_PARENT = T ;
            //
            PARAMETER_ADAM  () {
                Beta[0] = GET_BETA1 () ;
                Beta[1] = GET_BETA2 () ;
                M = 0 ; V = 0 ;
            }
            //
            using TYPE_SELF =
                PARAMETER_ADAM
                <TYPE_PARENT>
            ;//
            //
            _MACRO_DEFINE_(TYPE_DATA)
            //
        private:
            //
            inline static
            TYPE_DATA constexpr
            GET_BETA1 ()
            { return 0.90 ; }
            //
            inline static
            TYPE_DATA constexpr
            GET_BETA2 ()
            { return 0.99 ; }
            //
            inline static
            TYPE_DATA constexpr
            Eps ()
            { return 0.001 ; }
            //
            template <typename InType>
            inline void AddParameter (
                InType          & DST ,
                InType    const & SRC ,
                TYPE_DATA const   beta
            ) {
                auto const & TMP_SRC =
                    SRC.FLATTEN()
                ; //
                auto & TMP_DST =
                    DST.FLATTEN()
                ; //
                size_t constexpr limit =
                    InType::SIZE()
                ; //
                for(size_t i=0;i<limit;i++) {
                    TMP_DST[i] =
                        (beta*TMP_DST[i]) +
                        ((1.0-beta)*TMP_SRC[i])
                    ; //
                }
            }
            //
            inline void AddBoth (
                TYPE_PARENT       & DST  ,
                TYPE_PARENT const & SRC  ,
                TYPE_DATA   const   beta
            ) {
                AddParameter (
                    DST.WEIGHT ,
                    SRC.WEIGHT ,
                    beta
                ) ;
                AddParameter (
                    DST.BIAS ,
                    SRC.BIAS ,
                    beta
                ) ;
            }
            //
            template <typename InType>
            inline void Apply (
                InType          & DST  ,
                InType    const & SRCM ,
                InType    const & SRCV ,
                TYPE_DATA const   Eta
            ) {
                size_t constexpr limit =
                    InType::SIZE()
                ; //
                auto & dst =
                    DST.FLATTEN()
                ; //
                auto const & srcm =
                    SRCM.FLATTEN ()
                ;
                auto const & srcv =
                    SRCV.FLATTEN ()
                ;
                //
                for (size_t i=0;i<limit;i++) {
                    dst[i] = dst[i] - (
                        Eta * srcm[i] /
                        ( Eps() + sqrt(srcv[i]) )
                    ) ;
                }
                //
            }
            //
            inline void ApplyBoth (
                TYPE_PARENT       & DST  ,
                TYPE_PARENT const & SRCM ,
                TYPE_PARENT const & SRCV ,
                TYPE_DATA   const   Eta
            ) {
                Apply (
                    DST.WEIGHT , SRCM.WEIGHT ,
                    SRCV.WEIGHT , Eta
                ) ;
                Apply (
                    DST.BIAS , SRCM.BIAS ,
                    SRCV.BIAS , Eta
                ) ;
            }

        public:
            inline void UPDATE (
                TYPE_PARENT const & _M  ,
                TYPE_PARENT const & _V  ,
                TYPE_DATA   const   Eta
            ) {
                this->ACQUIRE_LOCK();
                AddBoth(M,_M,Beta[0]);
                AddBoth(V,_V,Beta[1]);
                ApplyBoth(this[0],M,V,Eta);
                this->RELEASE_LOCK();
            }
        private:
            TYPE_DATA Beta[2];
            TYPE_PARENT M, V;
        };
    }

//////////////////////////////////////////
#define _MACRO_DEFINE_ALL_				\
    _MACRO_DEFINE_(TYPE_FIRST_INPUT)	\
    _MACRO_DEFINE_(TYPE_DATA)			\
    _MACRO_DEFINE_(TYPE_MATRIX_OUTPUT)	\
    _MACRO_DEFINE_(TYPE_MATRIX_INPUT)	\
    _MACRO_DEFINE_(TYPE_PARAMETER)		\
    _MACRO_DEFINE_(TYPE_GRAD_OUTPUT)	\
    _MACRO_DEFINE_(TYPE_GRAD_INPUT)		//
//////////////////////////////////////////

//////////////////////////////////////////////
#define _MACRO_DEFINE_ALL_2_				\
    _MACRO_DEFINE_ALL_						\
    _MACRO_DEFINE_(TYPE_DELTA_PARAMETER)	//
//////////////////////////////////////////////

    namespace NN /* FirstLayer */ {
        template <typename T>
        class FirstLayer {
        public:
            using TYPE_FIRST_INPUT = T ;
            //
            using TYPE_DATA =
                typename
                    TYPE_FIRST_INPUT::TYPE_DATA
            ; //
            using TYPE_INPUT =
                TYPE_FIRST_INPUT
            ; //
            using TYPE_OUTPUT =
                TYPE_INPUT
            ; //
            using TYPE_PARAMETER =
                Nothing
            ; //
            using TYPE_SELF =
                FirstLayer
                    <TYPE_FIRST_INPUT>
            ;
            //
            inline static bool constexpr
            FORWARD_GRAD () {
                return false ;
            }
            //
            inline static bool constexpr
            FORWARD_GRAD (
                TYPE_FIRST_INPUT const & in
            ) {
                return GET_GRAD () ;
            }
            //
            inline static size_t constexpr
            SIZE_X () {
                return
                    TYPE_FIRST_INPUT::SIZE_X()
                ; //
            }
            //
            inline static constexpr
            size_t SIZE_Y () {
                return
                    TYPE_FIRST_INPUT::SIZE_Y()
                ; //
            }
            //
            inline static constexpr
            size_t SIZE () {
                return
                    TYPE_FIRST_INPUT::SIZE()
                ; //
            }
            //
            inline static constexpr
            size_t INDEX ()
            { return 0 ; }
            //
            static inline bool constexpr GET_GRAD () {
                return false ;
            }
            //
            inline void
            GET_LAYER_REFERENCE (
                TYPE_SELF *& ref
            ) { ref = this ; }
            //
            inline void
            SET_FIRST_INPUT (
                TYPE_FIRST_INPUT * _INPUT
            ) { INPUT = _INPUT ; }
            //
            inline TYPE_OUTPUT const &
            GET_OUTPUT () const {
                return INPUT[0];
            }
            //
            inline void FORWARD () const {}
            //
            inline TYPE_FIRST_INPUT const &
            FORWARD (
                TYPE_FIRST_INPUT const & in
            ) {
                INPUT = & in ;
                return INPUT[0] ;
            }
            //
            template <typename Tin>
            inline void BACKWARD (
                Tin const & in
            ) const {}
            //
            template <typename Tin>
            inline void
            BACKWARD_GRAD (
                Tin const & in
            ) const {}
            //
            inline void ATTACH (
                TYPE_PARAMETER const * _PARS
            ) const {}
            //
            inline void ATTACH (
                TYPE_PARAMETER const & _PARS
            ) const {}
            //
            inline void UPDATE (
                TYPE_DATA const Eta=0.01
            ) const {}
            //
            //
            TYPE_FIRST_INPUT const * INPUT ;
            //
            inline void CONSTRUCT () {}
            //
        } ;
    }

    namespace NN /* BaseNNType */ {
        template <
            size_t   X         ,
            typename inputtype ,
            typename selftype
        > class BaseNNType {
            //
        public:
            //
            using TYPE_INPUT = inputtype ;
            using TYPE_FIRST_INPUT =
                typename TYPE_INPUT::TYPE_FIRST_INPUT
            ; //
            using TYPE_DATA =
                typename TYPE_INPUT::TYPE_DATA
            ; //
            typedef selftype TYPE_PROXY ;
            //
            inline TYPE_PROXY &
            SELF () {
                return
                    static_cast
                        < TYPE_PROXY & >
                        ( this[0] )
                ;
            }
            //
            inline TYPE_PROXY const &
            SELF () const {
                return
                    static_cast
                        < TYPE_PROXY const & >
                        ( this[0] )
                ; //
            }
            //
            static inline size_t constexpr
            SIZE_Y () {
                return
                    TYPE_INPUT::SIZE_Y ()
                ; //
            }
            //
            static inline size_t constexpr
            SIZE_X ()
            { return X ; }
            //
            using TYPE_SELF =
                BaseNNType <
                    SIZE_Y ()  ,
                    TYPE_INPUT ,
                    TYPE_PROXY
                >
            ; //
            //
            using TYPE_MATRIX_INPUT =
                N2D_ARRAY <
                    TYPE_INPUT::SIZE_Y(),
                    TYPE_INPUT::SIZE_X(),
                    TYPE_DATA
                >
            ; //
            using TYPE_MATRIX_OUTPUT =
                N2D_ARRAY <
                    SIZE_Y(),
                    SIZE_X(),
                    TYPE_DATA
                >
            ; //
            using TYPE_PARAMETER =
                PARAMETER <
                    TYPE_MATRIX_OUTPUT::SIZE_X () ,
                    TYPE_MATRIX_INPUT::SIZE_X  () ,
                    TYPE_DATA
                >
            ; //
            using TYPE_GRAD_OUTPUT =
                N3D_ARRAY <
                    SIZE_Y                   () ,
                    SIZE_X                   () ,
                    TYPE_FIRST_INPUT::SIZE_X () ,
                    TYPE_DATA
                >
            ; //
            using TYPE_GRAD_INPUT =
                N3D_ARRAY <
                    SIZE_Y                   () ,
                    TYPE_INPUT::SIZE_X       () ,
                    TYPE_FIRST_INPUT::SIZE_X () ,
                    TYPE_DATA
                >
            ; //
            //
            inline TYPE_MATRIX_OUTPUT const &
            FORWARD
            ( TYPE_FIRST_INPUT const & in ) {
                return
                    SELF()
                    .MAIN_FORWARD (
                        INPUT.FORWARD(in)
                    )
                ; //
            }
            //
            inline TYPE_MATRIX_OUTPUT &
            GET_OUTPUT ()
            { return OUTPUT ; }
            //
            inline TYPE_MATRIX_OUTPUT const &
            GET_OUTPUT () const
            { return OUTPUT ; }
            //
            inline void
            SET_FIRST_INPUT
            (TYPE_FIRST_INPUT * _INPUT)
            { INPUT.SET_FIRST_INPUT (_INPUT) ; }
            //
            inline static size_t constexpr
            INDEX ()
            { return TYPE_INPUT::INDEX() + 1 ; }
            //
            template <typename T>
            inline void
            GET_LAYER_REFERENCE
            (T *& ref) {
                INPUT.GET_LAYER_REFERENCE(ref);
            }
            //
            inline void
            GET_LAYER_REFERENCE
            ( TYPE_PROXY * & ref )
            { ref = & SELF () ; }
            //
            inline void
            CONSTRUCT () {
                SELF().MAIN_CONSTRUCT () ;
                INPUT.CONSTRUCT () ;
            }
            //
            inline void MAIN_CONSTRUCT () {}
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD
            ( TYPE_MATRIX_INPUT const & in ) {
                /* Do Stuff Here... */
                printf(" This should not have been called... MAIN_FORWARD ");
                return
                    this->GET_OUTPUT()
                ; //
            }
            //
            ////////////////////////////////////////////////////////////////
            // y = TYPE_PARENT::SIZE_X()           /////////////////////////
            // b = TYPE_PARENT::SIZE_Y()           /////////////////////////
            // m = TYPE_FIRST_INPUT::SIZE_X()      /////////////////////////
            // x = TYPE_SELF::TYPE_INPUT::SIZE_X() /////////////////////////
            ////////////////////////////////////////////////////////////////
            //
            TYPE_INPUT         INPUT  ;
            TYPE_MATRIX_OUTPUT OUTPUT ;
            //
        } ;
    }

    namespace NN /* BaseNNTypeTrainable */ {
        template <
            size_t   X         ,
            typename inputtype ,
            typename selftype
        > class BaseNNTypeTrainable :
        public BaseNNType <
            X , inputtype ,
            selftype
        > {
            //
        public:
            //
            using TYPE_SELF =
                BaseNNTypeTrainable
                    <X,inputtype,selftype>
            ; //
            _MACRO_DEFINE_(TYPE_FIRST_INPUT)
            _MACRO_DEFINE_(TYPE_DATA)
            _MACRO_DEFINE_(TYPE_MATRIX_OUTPUT)
            _MACRO_DEFINE_(TYPE_MATRIX_INPUT)
            _MACRO_DEFINE_(TYPE_GRAD_OUTPUT)
            _MACRO_DEFINE_(TYPE_GRAD_INPUT)
            //
            using TYPE_DELTA_PARAMETER =
                _MACRO_INHERIT_(TYPE_PARAMETER)
            ; //
            using TYPE_PARAMETER =
                PARAMETER_ADAM
                    <TYPE_DELTA_PARAMETER>
            ; //
            inline TYPE_MATRIX_INPUT const &
            BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                TYPE_MATRIX_INPUT const &
                    delta_prime =
                        this->SELF()
                        .MAIN_BACKWARD(delta)
                ; //
                if (false) {
                    auto norm =
                        DELTA_PRIME
                            .L2_NORM()
                    ; //
                    DELTA_PRIME /=
                        norm
                    ; //
                }
                //
                this->INPUT.BACKWARD
                    ( delta_prime )
                ; //
                return delta_prime ;
                //
            }
            //
            inline TYPE_MATRIX_INPUT &
            GET_DELTA_PRIME ()
            { return DELTA_PRIME ; }
            //
            inline TYPE_MATRIX_INPUT const &
            GET_DELTA_PRIME () const
            { return DELTA_PRIME ; }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD
            ( TYPE_MATRIX_OUTPUT const & in ) {
                printf(" This should not have been called... MAIN_BACKWARD ");
                return GET_DELTA_PRIME () ;
            }
            //
            inline void UPDATE
            ( TYPE_DATA const Eta=0.01 ) {
                this->SELF()
                    .MAIN_UPDATE(Eta)
                ; //
                this->INPUT
                    .UPDATE
                        (Eta)
                ; //
            }
            //
            inline void MAIN_UPDATE
            ( TYPE_DATA const Eta=0.01 ) {}
            //
            TYPE_MATRIX_INPUT
                DELTA_PRIME
            ; //
        } ;
    }

    namespace NN /* BaseNNTypeTrainableGrad */ {
        template <
            size_t   X         ,
            typename inputtype ,
            typename selftype
        > class BaseNNTypeTrainableGrad :
        public BaseNNTypeTrainable <
            X , inputtype ,
            selftype
        > {
            //
        public:
            //
            using TYPE_SELF =
                BaseNNTypeTrainableGrad <
                    X , inputtype ,
                    selftype
                >
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            using TYPE_PROXY = selftype ;
            //
            inline TYPE_GRAD_OUTPUT &
            GET_GRAD () {
                return GRAD ;
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            GET_GRAD () const {
                return GRAD ;
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            FORWARD_GRAD () {
                return
                    this->SELF()
                        .MAIN_FORWARD_GRAD(
                            this->INPUT
                                .FORWARD_GRAD()
                        )
                ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            FORWARD_GRAD (
                TYPE_FIRST_INPUT const & in
            ) {
                this->FORWARD(in) ; //
                return FORWARD_GRAD () ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                TYPE_GRAD_INPUT const & in
            ) {
                printf("This should not have been called... MAIN_FORWARD_GRAD\n");
                return GET_GRAD();
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                bool const in
            ) {
                printf("This should not have been called... MAIN_FORWARD_GRAD\n");
                return GET_GRAD();
            }
            //
            inline TYPE_GRAD_INPUT const &
            BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                TYPE_GRAD_INPUT const & tmp =
                    this->SELF()
                        .MAIN_BACKWARD_GRAD
                            (DELTA)
                ;
                this->INPUT
                    .BACKWARD_GRAD
                        (tmp)
                ; //
                return tmp ;
            }
            //
            // DELTA is TYPE_GRAD_OUTPUT
            TYPE_GRAD_OUTPUT GRAD ;
            TYPE_GRAD_INPUT  DELTA_PRIME ;
            //
        private:
            //
            inline TYPE_GRAD_INPUT const &
            MAIN_BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                printf("MAIN_BACKWARD_GRAD: this should not have been called... implement it in the inherited class...\n");
                return DELTA_PRIME ;
            }
            //
        } ;
    }

    namespace NN /* Main multiplying layer */ {
        template <size_t Y, typename inputtype>
        class MainNNLayer : public
        BaseNNType <
            Y , inputtype ,
            MainNNLayer
                < Y , inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                MainNNLayer
                    <Y,inputtype>
            ; //
            _MACRO_DEFINE_ALL_
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                FUNCTIONS::NeuralMultiply (
                    this->GET_OUTPUT () ,
                    PARAMETER->WEIGHT   ,
                    in                  ,
                    PARAMETER->BIAS
                ) ; //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
            inline void
            ATTACH_PARAMETER (
                TYPE_PARAMETER & in
            ) {
                PARAMETER =
                    & in
                ; //
            }
            //
            TYPE_PARAMETER * PARAMETER ;
            //
        } ;
    }

    namespace NN /* Activation layer: SoftLRU */ {
        template <typename inputtype>
        class ActivateSoftLRU : public
        BaseNNType <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftLRU < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftLRU
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_
            //
            static inline TYPE_DATA
            SOFT_LRU (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    return
                        x /
                        ( 1.0 - x ) ;
                } else {
                    return x ;
                }
            }

            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                auto & tmp_in =
                    in.FLATTEN ()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                for ( size_t i=0 ; i<tmp_in.SIZE() ; i++ )
                { tmp_out[i] = SOFT_LRU (tmp_in[i]) ; }
                //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
        } ;
    }

    namespace NN /* Activation layer: SoftSign */ {
        template <typename inputtype>
        class ActivateSoftSign : public
        BaseNNType <
            inputtype::SIZE_Y() , inputtype ,
            ActivateSoftSign < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftSign <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_
            //
            static inline TYPE_DATA
            SOFT_SIGN (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    return
                        x /
                        ( 1.0 - x )
                    ; //
                } else {
                    return
                        x /
                        ( 1.0 + x )
                    ; //
                }
            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                auto & tmp_in =
                    in.FLATTEN()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                for ( size_t i=0 ; i<tmp_in.SIZE() ; i++ )
                { tmp_out[i] = SOFT_SIGN (tmp_in[i]) ; }
                //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
        } ;
    }

    namespace NN /* Main trainable layer */ {
        template <size_t Y, typename inputtype>
        class MainNNLayerTrainable : public
        BaseNNTypeTrainable <
            Y , inputtype ,
            MainNNLayerTrainable
                < Y , inputtype >
        > {
            //
        public:
            //
            using TYPE_SELF =
                MainNNLayerTrainable
                    <Y,inputtype>
            ; //
            _MACRO_DEFINE_ALL_2_
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                FUNCTIONS::NeuralMultiply (
                    this->GET_OUTPUT () ,
                    PARAMETER->WEIGHT   ,
                    in                  ,
                    PARAMETER->BIAS
                ) ;
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
            inline void
            ATTACH_PARAMETER (
                TYPE_PARAMETER & in
            ) {
                PARAMETER = & in ;
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & in
            ) {
                //
                in2.GET_SQUARED(in);
                //
                INPUT2.GET_SQUARED
                    (this->INPUT.GET_OUTPUT())
                ; //
                FUNCTIONS::BackProp (
                    this->GET_DELTA_PRIME() ,
                    in , PARAMETER->WEIGHT
                ) ;
                //
                FUNCTIONS::EvalW (
                    DP[0].WEIGHT , in ,
                    this->INPUT.GET_OUTPUT()
                ) ;
                //
                FUNCTIONS::EvalW (
                    DP[1].WEIGHT ,
                    in2 , INPUT2
                ) ;
                //
                for(
                    size_t b=0;
                    b<TYPE_MATRIX_OUTPUT::SIZE_Y();
                    b++
                ) { DP[0].BIAS+=in[b]; }
                //
                for(
                    size_t b=0;
                    b<TYPE_MATRIX_OUTPUT::SIZE_Y();
                    b++
                ) { DP[1].BIAS+=in2[b]; }
                //
                return
                    this->GET_DELTA_PRIME()
                ; //
            }
            //
            inline void
            MAIN_CONSTRUCT () {
                DP[0] = 0.0 ;
                DP[1] = 0.0 ;
            }
            //
            inline void
            MAIN_UPDATE (
                TYPE_DATA const Eta=0.01
            ) {
                PARAMETER->UPDATE (
                    /* _M  = */ DP[0] ,
                    /* _V  = */ DP[1] ,
                    /* Eta = */ Eta
                ) ; //
                DP[0] = 0 ;
                DP[1] = 0 ;
            }
            //
            MainNNLayerTrainable()
            { MAIN_CONSTRUCT () ; }
            //
            TYPE_PARAMETER * PARAMETER ;
            //
        private:
            //
            TYPE_MATRIX_INPUT    INPUT2 ;
            TYPE_MATRIX_OUTPUT   in2    ;
            TYPE_DELTA_PARAMETER DP[2]  ;
            //
        } ;
    }

    namespace NN /* Activation layer: SoftLRUTrainable */ {
        template <typename inputtype>
        class ActivateSoftLRUTrainable :
        public BaseNNTypeTrainable <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftLRUTrainable
                < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftLRUTrainable
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            static inline TYPE_DATA
            REAL_ACTIVATE (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    return x / ( 1.0 - x ) ;
                } else {
                    return x ;
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    TYPE_DATA tmp = 1.0 - x ;
                    return 1.0 / ( tmp * tmp ) ;
                } else {
                    return 1.0 ;
                }
            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const &
                    in
            ) {
                //
                auto & tmp_in =
                    in.FLATTEN ()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                for (
                    size_t i=0 ;
                    i<tmp_in.SIZE() ;
                    i++
                ) {
                    tmp_out[i] =
                        REAL_ACTIVATE
                            (tmp_in[i])
                    ;
                }
                //
                return
                    this->GET_OUTPUT()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                auto & tmp_in =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & tmp_delta =
                    delta.FLATTEN ()
                ; //
                auto & tmp_delta_prime =
                    this->GET_DELTA_PRIME()
                        .FLATTEN()
                ; //
                //
                for (
                    size_t i=0 ;
                    i<tmp_delta_prime.SIZE() ;
                    i++
                ) {
                    tmp_delta_prime[i] =
                        REAL_ACTIVATE_D (tmp_in[i]) *
                        tmp_delta[i]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME()
                ; //
            }
            //
        } ;
    }

    namespace NN /* Activation layer: ActivateSoftLRUPTrainable */ {
        template <typename inputtype>
        class ActivateSoftLRUPTrainable :
        public BaseNNTypeTrainable <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftLRUPTrainable
                < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftLRUPTrainable
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            static inline TYPE_DATA
            REAL_ACTIVATE (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    return
                        1.0 /
                        ( 1.0 - x )
                    ; //
                } else {
                    return 1.0 + x ;
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    TYPE_DATA tmp = 1.0 - x ;
                    return
                        1.0 /
                        ( tmp * tmp )
                    ; //
                } else {
                    return 1.0 ;
                }
            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                //
                auto & tmp_in =
                    in.FLATTEN () ;
                //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                for (
                    size_t i=0 ;
                    i<tmp_in.SIZE() ;
                    i++
                ) {
                    tmp_out[i] =
                        REAL_ACTIVATE
                            (tmp_in[i])
                    ; //
                }
                //
                return
                    this->GET_OUTPUT()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                auto & tmp_in =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN() ;
                //
                auto & tmp_delta =
                    delta.FLATTEN ()
                ; //
                auto & tmp_delta_prime =
                    this->GET_DELTA_PRIME()
                        .FLATTEN()
                ; //
                for (
                    size_t i=0 ;
                    i<tmp_delta_prime.SIZE() ;
                    i++
                ) {
                    tmp_delta_prime[i] =
                        REAL_ACTIVATE_D (tmp_in[i]) *
                        tmp_delta[i]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME () ;
                //
            }
            //
        } ;
    }

    namespace NN /* Activation layer: ActivateSoftSignTrainable */ {
        template <typename inputtype>
        class ActivateSoftSignTrainable :
        public BaseNNTypeTrainable <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftSignTrainable
                < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftSignTrainable
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            static inline TYPE_DATA
            REAL_ACTIVATE (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    return
                        x /
                        ( 1.0 - x )
                    ; //
                } else {
                    return
                        x /
                        ( 1.0 + x )
                    ; //
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D (
                TYPE_DATA const x
            ) {
                if ( x < 0 ) {
                    TYPE_DATA tmp =
                        1.0 - x
                    ; //
                    return
                        1.0 /
                        ( tmp * tmp )
                    ; //
                } else {
                    TYPE_DATA tmp =
                        1.0 + x ;
                    ; //
                    return
                        1.0 /
                        ( tmp * tmp )
                    ; //
                }
            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                //
                auto & tmp_in =
                    in.FLATTEN ()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                for (
                    size_t i=0 ;
                    i<tmp_in.SIZE() ;
                    i++
                ) {
                    tmp_out[i] =
                        REAL_ACTIVATE (
                            tmp_in[i]
                        )
                    ; //
                }
                //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                auto & tmp_in =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & tmp_delta =
                    delta.FLATTEN ()
                ; //
                auto & tmp_delta_prime =
                    this->GET_DELTA_PRIME()
                        .FLATTEN()
                ; //
                for (
                    size_t i=0 ;
                    i<tmp_delta_prime.SIZE() ;
                    i++
                ) {
                    tmp_delta_prime[i] =
                        REAL_ACTIVATE_D (tmp_in[i]) *
                        tmp_delta[i]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME()
                ; //
            }
            //
        } ;
    }

    namespace NN /* Activation layer: ActivateL2NormalizerTrainable */ {
        template <typename inputtype>
        class ActivateL2NormalizerTrainable :
        public BaseNNTypeTrainable <
            inputtype::SIZE_X() ,
            inputtype ,
            ActivateL2NormalizerTrainable
                < inputtype >
        > {
            //
        public:
            //
            using TYPE_SELF =
                ActivateL2NormalizerTrainable
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            typedef ND_ARRAY <
                TYPE_MATRIX_INPUT::SIZE_X(),
                TYPE_DATA
            > TYPE_ACTIVATOR_ARRAY ;
            //
            typedef ND_ARRAY <
                TYPE_MATRIX_INPUT::SIZE_Y(),
                TYPE_DATA
            > TYPE_DENOMINATOR_ARRAY ;
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                for(
                    size_t i=0;
                    i<DENOMINATOR.SIZE();
                    i++
                ) {
                    DENOMINATOR[i] =
                        in[i].L2_NORM ()
                    ; //
                }
                //
                auto & out =
                    this->GET_OUTPUT ()
                ; //
                for(
                    size_t y=0;
                    y<out.SIZE_Y();
                    y++
                ){
                    for(
                        size_t x=0;
                        x<out.SIZE_X();
                        x++
                    ){
                        out[y][x] =
                            (in[y][x]*in[y][x]) /
                            DENOMINATOR[y]
                        ; //
                    }
                }
                return out ;
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                auto & delta_prime =
                    this->GET_DELTA_PRIME ()
                ; //
                auto const & in =
                    this->INPUT.GET_OUTPUT()
                ; //
                for(
                    size_t b=0;
                    b<SUM.SIZE();
                    b++
                ) {
                    SUM[b]=0.0;
                    for(
                        size_t i=0;
                        i<in.SIZE_X();
                        i++
                    ){
                        SUM[b]+=
                            delta[b][i] *
                            in[b][i]    *
                            in[b][i]
                        ; //
                    }
                }
                for(
                    size_t b=0;
                    b<SUM.SIZE();
                    b++
                ) {
                    SUM[b]/=(
                        DENOMINATOR[b]*
                        DENOMINATOR[b]
                    );
                }
                //
                for(
                    size_t b=0;
                    b<TYPE_MATRIX_INPUT::SIZE_Y();
                    b++
                ) {
                    for(
                        size_t k=0;
                        k<TYPE_MATRIX_INPUT::SIZE_X();
                        k++
                    ) {
                        delta_prime[b][k] =
                            2.0 * in[b][k] * (
                                +(delta[b][k]/DENOMINATOR[b])
                                -SUM[b]
                            )
                        ; //
                    }
                    //
                }
                //
                return
                    this->GET_DELTA_PRIME()
                ; //
            }
            //
            TYPE_DENOMINATOR_ARRAY
                DENOMINATOR
            ; //
            TYPE_DENOMINATOR_ARRAY
                SUM
            ; //
        } ;
    }

    namespace NN /* The Last Layer: */ {
        template <typename input>
        class Teacher :
        public BaseNNType <
            input::SIZE_X() ,
            input ,
            Teacher <input>
        > {
            //
        public:
            //
            using TYPE_SELF =
                Teacher <input>
            ; //
            _MACRO_DEFINE_ALL_
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const &
                    in
            ) { return in ; }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const &
                    in ,
                TYPE_DATA weight
            ) {
                //
                auto & delta =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & INP =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & flat_in =
                    in.FLATTEN()
                ; //
                for(size_t i=0;i<delta.SIZE();i++) {
                    delta[i] =
                        INP[i] -
                        flat_in[i]
                    ; //
                }
                //
                delta *=
                    weight
                ; //
                //
                return
                    this->GET_OUTPUT()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            BACKWARD (
                TYPE_MATRIX_OUTPUT const &
                    delta ,
                TYPE_DATA weight = 1.0
            ) {
                //
                TYPE_MATRIX_INPUT const &
                    delta_prime =
                        MAIN_BACKWARD
                            (delta,weight)
                ; //
                //
                this->INPUT.BACKWARD
                    ( delta_prime )
                ; //
                //
                return
                    delta_prime
                ; //
                //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            TRAIN (
                TYPE_FIRST_INPUT  const &
                    in
                ,
                TYPE_MATRIX_OUTPUT const &
                    delta
                ,
                TYPE_DATA weight = 1.0
            ) {
                this->FORWARD
                    (in)
                ; //
                return
                    this->BACKWARD
                        (delta,weight)
                ; //
            }
            //
            inline void
            UPDATE (
                TYPE_DATA const
                    Eta = 0.01
            ) {
                this->INPUT
                    .UPDATE
                        (Eta)
                ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            FORWARD_GRAD () {
                return
                    this->INPUT
                        .FORWARD_GRAD()
                ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            FORWARD_GRAD (
                TYPE_FIRST_INPUT const &
                    in
            ) {
                this->FORWARD(in);
                return
                    FORWARD_GRAD()
                ; //
            }
            //
        } ;
    }

    namespace NN /* Main trainable layer with gradient */ {
        template <size_t Y, typename inputtype>
        class MainNNLayerTrainableGrad : public
        BaseNNTypeTrainableGrad <
            Y , inputtype ,
            MainNNLayerTrainableGrad
                < Y , inputtype >
        > {
            //
        public:
            //
            using TYPE_SELF =
                MainNNLayerTrainableGrad
                    < Y , inputtype >
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            using TYPE_WEIGHT =
                typename
                    TYPE_DELTA_PARAMETER::TYPE_WEIGHT
            ; //
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const &
                    in
            ) {
                FUNCTIONS::NeuralMultiply (
                    this->GET_OUTPUT () ,
                    PARAMETER->WEIGHT   ,
                    in                  ,
                    PARAMETER->BIAS
                ) ;
                return this->GET_OUTPUT () ;
            }
            //
            inline void
            ATTACH_PARAMETER (
                TYPE_PARAMETER &
                    in
            ) {
                PARAMETER = & in ;
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const &
                    in
            ) {
                //
                in2.GET_SQUARED(in);
                //
                INPUT2.GET_SQUARED (
                    this->INPUT
                        .GET_OUTPUT()
                ) ;
                //
                FUNCTIONS::BackProp (
                    this->GET_DELTA_PRIME() ,
                    in , PARAMETER->WEIGHT
                ) ;
                //
                FUNCTIONS::EvalW (
                    DP[0].WEIGHT ,
                    in ,
                    this->INPUT
                        .GET_OUTPUT()
                ) ;
                //
                FUNCTIONS::EvalW (
                    DP[1].WEIGHT ,
                    in2 ,
                    INPUT2
                ) ;
                //
                for(
                    size_t b=0;
                    b<TYPE_MATRIX_OUTPUT::SIZE_Y();
                    b++
                ) {
                    DP[0].BIAS +=
                        in[b]
                    ; //
                }
                //
                for(
                    size_t b=0;
                    b<TYPE_MATRIX_OUTPUT::SIZE_Y();
                    b++
                ) {
                    DP[1].BIAS +=
                        in2[b]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME()
                ; //
            }
            //
            inline void
            MAIN_CONSTRUCT () {
                DP[0]=0.0;
                DP[1]=0.0;
            }
            //
            inline void
            MAIN_UPDATE (
                TYPE_DATA const
                    Eta = 0.01
            ) {
                //auto norm = DP[0].WEIGHT.L2_NORM () ;
                //DP[0] /= norm ;
                //DP[1] /= (norm*norm) ;
                PARAMETER->UPDATE (
                    /* _M  = */ DP[0] ,
                    /* _V  = */ DP[1] ,
                    /* Eta = */ Eta
                ) ;
                DP[0]=0;
                DP[1]=0;
            }
            //
            MainNNLayerTrainableGrad()
            { MAIN_CONSTRUCT () ; }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                TYPE_GRAD_INPUT const &
                    in
            ) {
                FUNCTIONS::GRAD_PROPAGATE(
                    this->GET_GRAD()  ,
                    PARAMETER->WEIGHT ,
                    in
                );
                return this->GET_GRAD();
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                bool const
                    in
            ) {
                FUNCTIONS::GRAD_PROPAGATE(
                    this->GET_GRAD()  ,
                    PARAMETER->WEIGHT ,
                    in
                );
                return this->GET_GRAD();
            }
            //
            inline TYPE_GRAD_INPUT const &
            MAIN_BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                //
                auto & DELTA_PRIME =
                    this->DELTA_PRIME
                ; //
                //
                FUNCTIONS::GRAD_BACKWARD (
                    DELTA_PRIME       ,
                    PARAMETER->WEIGHT ,
                    DELTA
                ) ; //
                //
                FUNCTIONS::EVAL_DW_GRAD (
                /* DW1 = */     DP[0].WEIGHT            ,
                /* DW2 = */     DP[1].WEIGHT            ,
                /* tmpDW = */   tmpDW                   ,
                /* DELTA = */   DELTA                   ,
                /* GRAD_IN = */ this->INPUT.GET_GRAD()
                ) ; //
                //
                return
                    this->DELTA_PRIME
                ; //
                //
            }
            //
        private:
            //
            TYPE_PARAMETER *
                PARAMETER
            ; //
            //
            TYPE_MATRIX_INPUT
                INPUT2
            ; //
            //
            TYPE_MATRIX_OUTPUT
                in2
            ; //
            //
            TYPE_DELTA_PARAMETER
                DP[2]
            ; //
            //
            TYPE_WEIGHT
                tmpDW
            ; //
            //
        } ;
    }

    namespace NN /* Activation layer: SoftLRUTrainableGrad */ {
        template <typename inputtype>
        class ActivateSoftLRUTrainableGrad :
        public BaseNNTypeTrainableGrad <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftLRUTrainableGrad
                < inputtype >
        > {
        public:
            //
            using TYPE_SELF =
                ActivateSoftLRUTrainableGrad
                    <inputtype>
            ;
            //
            _MACRO_DEFINE_ALL_2_
            //
            static inline TYPE_DATA
            REAL_ACTIVATE (
                TYPE_DATA const
                    x
            ) {
                if ( x < 0 ) {
                    return x / ( 1.0 - x ) ;
                } else {
                    return x ;
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D (
                TYPE_DATA const
                    x
            ) {
                if ( x < 0 ) {
                    TYPE_DATA tmp = 1.0 - x ;
                    return 1.0 / ( tmp * tmp ) ;
                } else {
                    return 1.0 ;
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D_2 (
                TYPE_DATA const
                    x
            ) {
                return 0 ;
            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                //
                auto & tmp_in =
                    in.FLATTEN ()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                //
                for (
                    size_t i=0 ;
                    i<tmp_in.SIZE() ;
                    i++
                ) {
                    tmp_out[i] =
                        REAL_ACTIVATE
                            (tmp_in[i])
                    ; //
                }
                //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                auto & tmp_in =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & tmp_delta =
                    delta.FLATTEN ()
                ; //
                auto & tmp_delta_prime =
                    this->GET_DELTA_PRIME()
                        .FLATTEN()
                ; //
                //
                for (
                    size_t i=0 ;
                    i<tmp_delta_prime.SIZE() ;
                    i++
                ) {
                    tmp_delta_prime[i] =
                        REAL_ACTIVATE_D (tmp_in[i]) *
                        tmp_delta[i]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME ()
                ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                TYPE_GRAD_INPUT const & in
            ) {
                auto & Input =
                    this->INPUT
                        .GET_OUTPUT()
                ; //
                auto & Output =
                    this->GET_GRAD()
                ; //
                for(size_t b=0;b<in.SIZE_Z();b++){
                    for(size_t i=0;i<in.SIZE_Y();i++){
                        TYPE_DATA tmp =
                            REAL_ACTIVATE_D
                                (Input[b][i])
                        ; //
                        for(size_t m=0;m<in.SIZE_X();m++){
                            Output[b][i][m] =
                                tmp *
                                in[b][i][m]
                            ; //
                        }
                    }
                }
                //
                return Output;
                //
            }
            //
            inline TYPE_GRAD_INPUT const &
            MAIN_BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                if (true) {
                    auto & Input =
                        this->INPUT
                            .GET_OUTPUT()
                    ; //
                    auto & ingrad =
                        this->INPUT
                            .GET_GRAD()
                    ; //
                    auto & DELTA_PRIME =
                        this->DELTA_PRIME
                    ; //
                    //
                    size_t const M =
                        DELTA_PRIME
                            .SIZE_X()
                    ; //
                    size_t const I =
                        DELTA_PRIME
                            .SIZE_Y()
                    ; //
                    size_t const B =
                        DELTA_PRIME
                            .SIZE_Z()
                    ; //
                    //
                    for(size_t b=0;b<B;b++){
                        for(size_t i=0;i<I;i++){
                            //
                            auto tmp1 =
                                REAL_ACTIVATE_D
                                    (Input[b][i])
                            ; //
                            auto tmp2 =
                                REAL_ACTIVATE_D_2
                                    (Input[b][i])
                            ; //
                            //
                            DELTA_PRIME_2[b][i] =
                                DELTA[b][i][0] *
                                ingrad[b][i][0] *
                                tmp2
                            ; //
                            for(size_t m=1;m<M;m++){
                                DELTA_PRIME_2[b][i] +=
                                    DELTA[b][i][m] *
                                    ingrad[b][i][m] *
                                    tmp2
                                ; //
                            }
                            //
                            for(size_t m=0;m<M;m++){
                                DELTA_PRIME[b][i][m] =
                                    DELTA[b][i][m] *
                                    tmp1
                                ; //
                            }
                            //
                        }
                    }
                    //
                    this->BACKWARD
                        (DELTA_PRIME_2)
                    ; //
                    return
                        DELTA_PRIME
                    ; //
                }
                //
            }
            //
            TYPE_MATRIX_INPUT
                DELTA_PRIME_2
            ;
            //
        } ;
    }

    namespace NN /* Activation layer: SoftLRUPTrainableGrad */ {
        template <typename inputtype>
        class ActivateSoftLRUPTrainableGrad :
        public BaseNNTypeTrainableGrad <
            inputtype::SIZE_X() , inputtype ,
            ActivateSoftLRUPTrainableGrad
                < inputtype >
        > {
            //
        public:
            //
            using TYPE_SELF =
                ActivateSoftLRUPTrainableGrad
                    <inputtype>
            ; //
            //
            _MACRO_DEFINE_ALL_2_
            //
            static inline TYPE_DATA
            REAL_ACTIVATE (
                TYPE_DATA const
                    x
            ) {
                if ( x < 0 ) {
                    return
                        1.0  /
                        ( 1.0 - x )
                    ; //
                } else {
                    return
                        ( 1.0 + x )
                    ; //
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D (
                TYPE_DATA const
                    x
            ) {
                if ( x < 0 ) {
                    TYPE_DATA tmp =
                        1.0 - x
                    ; //
                    return
                        1.0 /
                        ( tmp * tmp )
                    ; //
                } else {
                    return
                        1.0
                    ; //
                }
            }
            //
            static inline TYPE_DATA
            REAL_ACTIVATE_D_2 (
                TYPE_DATA const
                    x
            ) {
                if(x<0){
                    TYPE_DATA tmp =
                        ( 1.0 - x )
                    ; //
                    return
                        2.0 /
                        ( tmp * tmp * tmp )
                    ; //
                } else {
                    return
                        0
                    ; //
                }

            }
            //
            inline TYPE_MATRIX_OUTPUT const &
            MAIN_FORWARD (
                TYPE_MATRIX_INPUT const & in
            ) {
                //
                auto & tmp_in =
                    in.FLATTEN ()
                ; //
                auto & tmp_out =
                    this->GET_OUTPUT()
                        .FLATTEN()
                ; //
                //
                for (
                    size_t i=0 ;
                    i<tmp_in.SIZE() ;
                    i++
                ) {
                    tmp_out[i] =
                        REAL_ACTIVATE
                            (tmp_in[i])
                    ; //
                }
                //
                return
                    this->GET_OUTPUT ()
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            MAIN_BACKWARD (
                TYPE_MATRIX_OUTPUT const & delta
            ) {
                //
                auto & tmp_in =
                    this->INPUT
                        .GET_OUTPUT()
                        .FLATTEN()
                ; //
                auto & tmp_delta =
                    delta.FLATTEN ()
                ; //
                auto & tmp_delta_prime =
                    this->GET_DELTA_PRIME()
                        .FLATTEN()
                ; //
                //
                for (
                    size_t i=0 ;
                    i<tmp_delta_prime.SIZE() ;
                    i++
                ) {
                    tmp_delta_prime[i] =
                        REAL_ACTIVATE_D (tmp_in[i]) *
                        tmp_delta[i]
                    ; //
                }
                //
                return
                    this->GET_DELTA_PRIME ()
                ; //
            }
            //
            inline TYPE_GRAD_OUTPUT const &
            MAIN_FORWARD_GRAD (
                TYPE_GRAD_INPUT const & in
            ) {
                auto & Input =
                    this->INPUT
                        .GET_OUTPUT()
                ; //
                auto & Output =
                    this->GET_GRAD()
                ; //
                for(size_t b=0;b<in.SIZE_Z();b++){
                    for(size_t i=0;i<in.SIZE_Y();i++){
                        TYPE_DATA tmp =
                            REAL_ACTIVATE_D
                                (Input[b][i])
                        ; //
                        for(size_t m=0;m<in.SIZE_X();m++){
                            Output[b][i][m] =
                                tmp *
                                in[b][i][m]
                            ; //
                        }
                    }
                }
                //
                return Output;
                //
            }
            //
            inline TYPE_GRAD_INPUT const &
            MAIN_BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                if (true) {
                    auto & Input =
                        this->INPUT
                            .GET_OUTPUT()
                    ; //
                    auto & ingrad =
                        this->INPUT
                            .GET_GRAD()
                    ; //
                    auto & DELTA_PRIME =
                        this->DELTA_PRIME
                    ; //
                    //
                    size_t const M =
                        DELTA_PRIME
                            .SIZE_X()
                    ; //
                    size_t const I =
                        DELTA_PRIME
                            .SIZE_Y()
                    ; //
                    size_t const B =
                        DELTA_PRIME
                            .SIZE_Z()
                    ; //
                    //
                    for(size_t b=0;b<B;b++){
                        for(size_t i=0;i<I;i++){
                            //
                            auto tmp1 =
                                REAL_ACTIVATE_D
                                    (Input[b][i])
                            ; //
                            auto tmp2 =
                                REAL_ACTIVATE_D_2
                                    (Input[b][i])
                            ; //
                            //
                            DELTA_PRIME_2[b][i] =
                                DELTA[b][i][0] *
                                ingrad[b][i][0] *
                                tmp2
                            ; //
                            for(size_t m=1;m<M;m++){
                                DELTA_PRIME_2[b][i] +=
                                    DELTA[b][i][m] *
                                    ingrad[b][i][m] *
                                    tmp2
                                ; //
                            }
                            //
                            for(size_t m=0;m<M;m++){
                                DELTA_PRIME[b][i][m] =
                                    DELTA[b][i][m] *
                                    tmp1
                                ; //
                            }
                            //
                        }
                    }
                    //
                    this->BACKWARD
                        (DELTA_PRIME_2)
                    ; //
                    return
                        DELTA_PRIME
                    ; //
                }
                //
            }
            //
            TYPE_MATRIX_INPUT
                DELTA_PRIME_2
            ;
            //
        } ;
    }

    namespace NN /* The Last Layer Grad: */ {
        template <typename input>
        class TeacherGrad :
        public Teacher <input> {
            //
        public:
            //
            using TYPE_SELF =
                TeacherGrad
                    <input>
            ; //
            //
            using TYPE_PARENT =
                Teacher <input>
            ; //
            //
            _MACRO_DEFINE_ALL_
            //
            inline TYPE_PARENT &
            AS_PARENT () {
                return
                    reinterpret_cast
                        <TYPE_PARENT &>
                            (this[0])
                ; //
            }
            //
            inline TYPE_PARENT const &
            AS_PARENT () const {
                return
                    reinterpret_cast
                        <TYPE_PARENT const &>
                            (this[0])
                ; //
            }
            //
            inline TYPE_GRAD_INPUT const &
            BACKWARD_GRAD (
                TYPE_GRAD_OUTPUT const &
                    DELTA
            ) {
                //
                auto & tmp1 =
                    this->INPUT
                        .GET_GRAD()
                        .FLATTEN()
                ; //
                //
                auto & tmp2 =
                    DELTA
                        .FLATTEN()
                ; //
                //
                auto & tmp =
                    DELTA_GRAD
                        .FLATTEN()
                ; //
                //
                for(
                    size_t i=0;
                    i<tmp.SIZE();
                    i++
                ) {
                    tmp[i] =
                        tmp1[i] -
                        tmp2[i]
                    ; //

                }
                //
                this->INPUT
                    .BACKWARD_GRAD
                        (DELTA_GRAD)
                ; //
                //
                return
                    DELTA_GRAD
                ; //
                //
            }
            //
            inline TYPE_GRAD_INPUT const &
            TRAIN (
                TYPE_FIRST_INPUT const &
                    in
                ,
                TYPE_GRAD_OUTPUT  const &
                    delta
            ) {
                this->FORWARD_GRAD
                    (in)
                ; //
                return
                    this->BACKWARD_GRAD
                        (delta)
                ; //
            }
            //
            inline TYPE_MATRIX_INPUT const &
            TRAIN (
                TYPE_FIRST_INPUT  const &
                    in
                ,
                TYPE_MATRIX_OUTPUT const &
                    delta
                ,
                TYPE_DATA weight = 1.0
            ) {
                return
                    AS_PARENT()
                    .TRAIN(in,delta,weight)
                ; //
            }
            //
            TYPE_GRAD_INPUT
                DELTA_GRAD
            ; //
            //
        } ;
    }

//////////////////////////////
#undef _MACRO_DEFINE_ALL_2_	//
#undef _MACRO_DEFINE_ALL_	//
#undef _MACRO_DEFINE_		//
#undef _MACRO_INHERIT_		//
//////////////////////////////

}
