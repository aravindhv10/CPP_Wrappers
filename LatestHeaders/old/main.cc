#include "cblas.h"
#include "pcg.hh"
#include "CPPFileIO9.hh"

using namespace Tensors::NN ;

class SlaveNet {
public:
    using TYPE_DATA = double ;
    using TYPE_INPUT =
        N2D_ARRAY
            <1000,1,TYPE_DATA>
    ; //
    using L0 =
        FirstLayer
            <TYPE_INPUT>
    ; //
    using L1 =
        MainNNLayerTrainableGrad
            <30,L0>
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

    TYPE_INPUT  MatIn  ;
    TYPE_OUTPUT MatOut ;

    TYPE_GRAD_OUTPUT Grads ;

    inline void eval () {
        auto & out =
            net
                .FORWARD
                    (MatIn)
                .FLATTEN()
        ; //
        auto outG =
            net
                .FORWARD_GRAD()
        ; //
        //net.BACKWARD_GRAD(Grads,420);
        FILE * f = fopen ("./res","w") ;
        for(size_t i=0;i<out.SIZE();i++){
            fprintf(
                f,
                "%e %e %e\n",
                MatIn.FLATTEN()[i],
                MatOut.FLATTEN()[i],
                outG.FLATTEN()[i]
            ); //
        }
        fclose(f);
    }

    inline void write () {
        CPPFileIO::FileFD writer ("./pars") ;
        writer.writefile();
        par1>>writer;
        par2>>writer;
    }

    inline void read () {
        CPPFileIO::FileFD writer ("./pars") ;
        writer.readfile();
        par1<<writer;
        par2<<writer;
    }

    inline void train () {
        TYPE_DATA error = 0 ;
        size_t constexpr CountLimit = 100 ;
        for(size_t i=0;i<CountLimit;i++){
            net.FORWARD
                (MatIn)
            ; //
            error +=
                net
                    .BACKWARD
                        (MatOut)
                    .L2_NORM()
            ;
            net.UPDATE (0.00001);
        }
        printf("Error = %e\n",(TYPE_DATA)error/CountLimit);
    }

    inline void train_grad () {
        TYPE_DATA error = 0 ;
        size_t constexpr CountLimit = 1 ;
        for(size_t i=0;i<CountLimit;i++){
            net
                .FORWARD_GRAD
                    (MatIn)
            ; //
            error +=
                net
                    .BACKWARD_GRAD
                        (Grads)
                    .L2_NORM()
            ; //
            net.UPDATE (0.00000001);
        }
        printf("Error = %e\n",(TYPE_DATA)error/CountLimit);
    }

    SlaveNet(){
        /* first parameter */ {
            par1.RANDOMIZE();
            L1 * ref ;
            net.GET_LAYER_REFERENCE(ref);
            ref->ATTACH_PARAMETER (par1);
        }
        /* second parameter */ {
            par2.RANDOMIZE();
            L2 * ref ;
            net.GET_LAYER_REFERENCE (ref) ;
            ref->ATTACH_PARAMETER (par2)  ;
        }
        TYPE_DATA x0 = -5.0 ;
        TYPE_DATA x1 =  5.0 ;
        TYPE_DATA dx = ( x1 - x0 ) / ( (TYPE_DATA) MatIn.SIZE_Y() ) ;

        for(size_t y=0;y<MatIn.SIZE_Y();y++){
            MatIn[y][0] =
                x0+(y*dx)
            ; //
            Grads[y][0][0]=
                2.0*MatIn[y][0]
            ; //

        }
        MatOut.GET_SQUARED(MatIn);
    }
    ~SlaveNet(){}
    //
    static inline SlaveNet *
    NEW () {
        return new SlaveNet ;
    }
    //
    static inline SlaveNet *
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
    TRAIN (size_t count) {
        auto * tmp = GET () ;
        for(size_t j=0;j<count;j++){
            for(size_t i=0;i<10;i++)
            {tmp->train();}
            printf("Writing...\n");
            tmp->write();
            printf("Wrote...\n");
        }
        delete tmp ;
    }
    //
    static inline void
    TRAIN_GRAD (size_t count) {
        auto * tmp = GET () ;
        for(size_t j=0;j<count;j++){
            for(size_t i=0;i<10;i++)
            {tmp->train_grad();}
            printf("Writing...\n");
            tmp->write();
            printf("Wrote...\n");
        }
        delete tmp ;
    }
    //
    static inline void
    EVAL () {
        auto * tmp = GET () ;
        tmp->eval ();
        delete tmp ;
    }
} ;

//template <size_t BatchSize>
class SlaveNetGrad {
public:
    using TYPE_DATA =
        double
    ; //
    using TYPE_INPUT =
        N2D_ARRAY
            <200,1,TYPE_DATA>
    ; //
    using L0 =
        FirstLayer
            <TYPE_INPUT>
    ; //
    using L1 =
        MainNNLayerTrainableGrad
            <30,L0>
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

    CPPFileIO::myrandgen <pcg64_fast> randgen ;

    inline void PrepareData () {
        auto & tmp1 = MatIn.FLATTEN  () ;
        auto & tmp2 = MatOut.FLATTEN () ;
        auto & tmp3 = Grads.FLATTEN  () ;
        for(size_t i=0;i<tmp1.SIZE();i++){
            tmp1[i] = randgen[0] ;
            tmp2[i] = tmp1[i] * tmp1[i] ;
            tmp3[i] = 1.0 / tmp1[i] ;
        }
    }

    inline void eval () {
        FILE * f = fopen ("./res","w") ;
        for(size_t E=0;E<100;E++){
            PrepareData() ;
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
            for(size_t i=0;i<out.SIZE();i++){
                fprintf (
                    f,
                    "%e %e %e\n",
                    MatIn.FLATTEN()[i],
                    out[i],
                    outG[i]
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
            net.UPDATE (eta);
        }

        printf("Error = %e\n",(TYPE_DATA)error/countlimit);
    }
    //
    SlaveNetGrad() :
    randgen(1,1.0,10.0) {
        /* first parameter */ {
            par1.RANDOMIZE();
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
            L2 * ref ;
            net.GET_LAYER_REFERENCE (ref) ;
            ref->ATTACH_PARAMETER (par2)  ;
        }
    }
    //
    ~SlaveNetGrad(){}
    //
    static inline SlaveNetGrad *
    NEW () {
        return new SlaveNetGrad ;
    }
    //
    static inline SlaveNetGrad *
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
        auto * tmp = GET () ;
        for (size_t j=0;j<10;j++) {
            tmp->train_grad
                (0.000001)
            ; //
        }
        for (size_t j=0;j<count;j++) {
            for (size_t i=0;i<20;i++) {
                tmp->train_grad
                    (0.000001)
                ; //
            }
            printf("Writing...\n");
            tmp->write();
            printf("Wrote...\n");
            //printf("Evaluating...\n");
            //tmp->eval();
            //EVAL ();
            //printf("Evaluated...\n");
        }
        delete tmp ;
    }
    //
} ;

int main () {
    SlaveNetGrad::START();
    SlaveNetGrad::EVAL();
    SlaveNetGrad::TRAIN(10);

    return 0 ;
}
