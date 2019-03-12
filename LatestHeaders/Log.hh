#include "cblas.h"
#include "pcg.hh"
#include "CPPFileIO9.hh"

using namespace Tensors::NN ;
template <size_t BatchSize>
class SlaveNetGrad {
public:
    //
    using TYPE_SELF =
        SlaveNetGrad <BatchSize>
    ; //
    using TYPE_DATA =
        double
    ; //
    using TYPE_INPUT =
        N2D_ARRAY
            <BatchSize,1,TYPE_DATA>
    ; //
    using L0 =
        FirstLayer
            <TYPE_INPUT>
    ; //
    using L1 =
        MainNNLayerTrainableGrad
            <40,L0>
    ; //
    using LA1 =
        ActivateSoftLRUPTrainableGrad
            <L1>
    ; //
    using L2 =
        MainNNLayerTrainableGrad
            <1,LA1>
    ; //
    using Net =
        TeacherGrad
            <L2>
    ; //
    using TYPE_GRAD_OUTPUT =
        typename
            Net::TYPE_GRAD_OUTPUT
    ; //
    using LP1 =
        typename
            L1::TYPE_PARAMETER
    ; //
    using LP2 =
        typename
            L2::TYPE_PARAMETER
    ; //
    using TYPE_OUTPUT =
        typename
            Net::TYPE_MATRIX_OUTPUT
    ; //

    Net net  ;
    LP1 par1 ;
    LP2 par2 ;

    TYPE_INPUT       MatIn  ;
    TYPE_OUTPUT      MatOut ;
    TYPE_GRAD_OUTPUT Grads  ;

    TYPE_INPUT  One  ;
    TYPE_OUTPUT Zero ;

    CPPFileIO::myrandgen
        <pcg64_fast> randgen
    ; //

    inline void PrepareData () {
        auto & tmp1 = MatIn.FLATTEN  () ;
        auto & tmp2 = MatOut.FLATTEN () ;
        auto & tmp3 = Grads.FLATTEN  () ;
        for(size_t i=0;i<tmp1.SIZE();i++){
            tmp1[i] = randgen[0] ;
            tmp2[i] = log(tmp1[i]) ;
            tmp3[i] = 1.0 / tmp1[i] ;
        }
    }
    //
    inline void eval () {
        FILE * f = fopen ("./res","w") ;
        TYPE_DATA Offset = 0 ;
        for(size_t E=0;E<100;E++){
            PrepareData() ;
            if(E==0){
                MatIn.FLATTEN()[0] =
                    1.0
                ; //
            }
            auto & out =
                net
                    .FORWARD
                        (MatIn)
                    .FLATTEN()
            ; //
            auto outG =
                net
                    .FORWARD_GRAD()
                    .FLATTEN()
            ; //
            if(E==0){
                Offset =
                    out.FLATTEN()[0]
                ; //
            } //
            for(size_t i=0;i<out.SIZE();i++){
                fprintf (
                    f,
                    "%e %e %e %e\n",
                    MatIn.FLATTEN()[i],
                    out[i] - Offset,
                    outG[i],
                    MatOut.FLATTEN()[i]
                ) ; //
            }
        }
        fclose(f);
    }

    inline void
    write () {
        CPPFileIO::FileFD writer
            ("./pars")
        ; //
        writer.writefile();
        par1>>writer;
        par2>>writer;
    }
    //
    inline void
    read () {
        CPPFileIO::FileFD writer
            ("./pars")
        ; //
        writer.readfile();
        par1<<writer;
        par2<<writer;
    }
    //
    inline void
    train_grad (
        TYPE_DATA eta =
            0.00001
    ) {
        TYPE_DATA error = 0 ;
        size_t constexpr
            countlimit =
                1000
        ; //
        for(size_t i=0;i<countlimit;i++){
            PrepareData();
            error +=
                net.TRAIN
                        (MatIn,Grads)
                    .L2_NORM()
            ; //
            net.UPDATE
                (eta)
            ;
        }
        printf("Error = %e\n",(TYPE_DATA)error/(countlimit*BatchSize));
    }
    inline void
    constrain (
        TYPE_DATA eta =
            0.0001
    ) {
        net.TRAIN
            (One,Zero,10.0)
        ; //
        net.UPDATE
            (eta)
        ; //
    }
    //
    SlaveNetGrad() :
    randgen(1,1.0,10.0) {
        One.FLATTEN  () = 1.0 ;
        Zero.FLATTEN () = 0.0 ;
        /* first parameter */ {
            par1.RANDOMIZE();
            par1*=0.3;
            L1 * ref ;
            net.GET_LAYER_REFERENCE
                (ref)
            ; //
            ref->ATTACH_PARAMETER
                (par1)
            ; //
        }
        /* second parameter */ {
            par2.RANDOMIZE();
            par2*=0.3;
            L2 * ref ;
            net.GET_LAYER_REFERENCE
                (ref)
            ; //
            ref->ATTACH_PARAMETER
                (par2)
            ; //
        }
    }
    //
    ~SlaveNetGrad(){}
    //
    static inline TYPE_SELF *
    NEW () {
        return new TYPE_SELF ;
    }
    //
    static inline TYPE_SELF *
    GET () {
        auto * tmp = NEW() ;
        tmp->read();
        return tmp ;
    }
    //
    static inline void
    START () {
        auto * tmp = NEW () ;
        tmp->write ();
        delete tmp ;
    }
    //
    static inline void
    EVAL () {
        auto * tmp = GET () ;
        tmp->eval();
        delete tmp ;
    }
    //
    static inline void
    TRAIN (
        size_t count =
            1000
    ) {
        TYPE_DATA small = 0.00001 ;
        auto * tmp = GET () ;
        for (size_t j=0;j<10;j++) {
            tmp->train_grad
                (small)
            ; //
        }
        for (size_t j=0;j<count;j++) {
            for (size_t i=0;i<20;i++) {
//                tmp->constrain
//                    (small)
//                ; //
                tmp->train_grad
                    (small)
                ; //
            }
            printf("Writing...\n");
            tmp->write();
            printf("Wrote...\n");
        }
        for (size_t j=0;j<count;j++) {
            for (size_t i=0;i<20;i++) {
                tmp->train_grad
                    (small)
                ; //
            }
            printf("Writing...\n");
            tmp->write();
            printf("Wrote...\n");
        }
        delete tmp ;
    }
    //
} ;
