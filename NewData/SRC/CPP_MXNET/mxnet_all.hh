#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include "all.hh"
#include "utils.h"
#include "mxnet-cpp/MxNetCpp.h"

namespace MXNET_CHECK {
    using MISC::TYPE_DATA ;
    using MISC::imagetype ;
    using MISC::ImageResolution ;
    //
    size_t constexpr
        BatchSize =
            200
    ; //
    //
    class MainNet {
    public:
        //
        using ImageArray =
            Tensors::NN::N2D_ARRAY <
                BatchSize , (
                    ImageResolution *
                    ImageResolution
                ) , TYPE_DATA
            >
        ; //
        using LabelArray =
            Tensors::NN::ND_ARRAY <
                BatchSize ,
                TYPE_DATA
            >
        ; //
        using NetOutputArray =
            Tensors::NN::N2D_ARRAY <
                BatchSize , 2 ,
                TYPE_DATA
            >
        ; //
        using MainImageReader =
            CPPFileIO::FullFileReader
                <ImageArray>
        ; //
        using MainLabelReader =
            CPPFileIO::FullFileReader
                <LabelArray>
        ; //
        //
        std::map
            <std::string,NDArray>
                args_map
        ; //
        //
        Context
            ctx_cpu
        ; //
        //
        Optimizer *
            opt
        ; //
        Symbol
            lenet
        ; //
        Executor *
            exe
        ; //
        static inline MainImageReader
        READER_DATA (
            size_t i
        ) {
            char tmp[256] ;
            sprintf (
                tmp ,
                "./OUTS/TMP/QCD_TOP/TRAIN/%ld/image",
                i
            ) ; //
            std::string
                name(tmp)
            ; //
            MainImageReader
                ret(name)
            ; //
            return ret ;
        }
        //
        static inline MainLabelReader
        READER_LABEL (
            size_t i
        ) {
            char tmp[256] ;
            sprintf (
                tmp ,
                "./OUTS/TMP/QCD_TOP/TRAIN/%ld/label" ,
                i
            ) ; //
            std::string
                name(tmp)
            ; //
            MainLabelReader
                ret(name)
            ; //
            return ret ;
        }
        //
        static inline MainImageReader
        READER_TEST_DATA (
            size_t i
        ) {
            char tmp[256] ;
            sprintf (
                tmp ,
                "./OUTS/TMP/QCD_TOP/TEST/%ld/image",
                i
            ) ; //
            std::string
                name(tmp)
            ; //
            MainImageReader
                ret(name)
            ; //
            return ret ;
        }
        //
        static inline MainLabelReader
        READER_TEST_LABEL (
            size_t i
        ) {
            char tmp[256] ;
            sprintf (
                tmp ,
                "./OUTS/TMP/QCD_TOP/TEST/%ld/label" ,
                i
            ) ; //
            std::string
                name(tmp)
            ; //
            MainLabelReader
                ret(name)
            ; //
            return ret ;
        }
        //
        // CONSTRUCT THE MAIN SYMBOL OF THE NETWORK:
        //
        static Symbol
        GetNetSymbol() {
            //
            // INPUT AND OUTPUT :
            Symbol data =
                Symbol::Variable
                    ("data")
            ; //
            Symbol data_label =
                Symbol::Variable
                    ("data_label")
            ; //
            // FIRST LAYER :
            Symbol
                conv1_w("conv1_w") ,
                conv1_b("conv1_b")
            ; //
            Symbol conv1 =
                Convolution (
                    "conv1" , data    ,
                    conv1_w , conv1_b ,
                    Shape(10, 10) , 50
                )
            ; //
            Symbol ReLU1 =
                relu(conv1)
            ; //
            Symbol pool1 =
                Pooling(
                    "pool1", ReLU1, Shape(2, 2),
                    PoolingPoolType::kMax, false,
                    false, PoolingPoolingConvention::kValid,
                    Shape(2, 2)
                )
            ;
            //
            // SECOND LAYER :
            Symbol
                conv2_w("conv2_w") ,
                conv2_b("conv2_b")
            ; //
            Symbol conv2 =
                Convolution (
                    "conv2" , pool1    ,
                    conv2_w , conv2_b ,
                    Shape(5, 5) , 30
                )
            ; //
            Symbol ReLU2 =
                relu(conv2)
            ; //
            Symbol pool2 =
                Pooling(
                    "pool2", ReLU2, Shape(2, 2),
                    PoolingPoolType::kMax, false,
                    false, PoolingPoolingConvention::kValid,
                    Shape(2, 2)
                )
            ;
            //
            // THIRD LAYER :
            Symbol
                conv3_w("conv3_w") ,
                conv3_b("conv3_b")
            ; //
            Symbol conv3 =
                Convolution (
                    "conv3" , pool2   ,
                    conv3_w , conv3_b ,
                    Shape(2, 2) , 10
                )
            ; //
            Symbol ReLU3 =
                relu(conv3)
            ; //
            Symbol pool3 =
                Pooling (
                    "pool3", ReLU3, Shape(2, 2),
                    PoolingPoolType::kMax, false,
                    false, PoolingPoolingConvention::kValid,
                    Shape(2, 2)
                )
            ;
            //
            // FLATTEN :
            Symbol flatten =
                Flatten(
                    "flatten",
                    pool3
                )
            ;
            //
            // FC PART 1 :
            Symbol
                fc1_w("fc1_w"),
                fc1_b("fc1_b")
            ;
            Symbol fc1 =
                FullyConnected (
                    "fc1", flatten,
                    fc1_w, fc1_b, 256
                )
            ; //
            //
            // FC PART 2 :
            Symbol
                fc2_w("fc2_w"),
                fc2_b("fc2_b")
            ; //
            Symbol fc2 =
                FullyConnected (
                    "fc2", fc1,
                    fc2_w, fc2_b, 2
                )
            ; //
            // Final Output :
            Symbol lenet =
                SoftmaxOutput (
                    "softmax",
                    fc2, data_label
                )
            ; //
            return lenet ;
        }
        //
        inline void
        PreapreArrays () {
            args_map["data"] =
                NDArray (
                    Shape (
                        BatchSize , 1 ,
                        ImageResolution ,
                        ImageResolution
                    ) ,
                    ctx_cpu , false
                )
            ;
            args_map["data_label"] =
                NDArray (
                    Shape (BatchSize) ,
                    ctx_cpu , false
                )
            ; //
            NDArray::WaitAll();
        }
        //
        inline void
        InitParams () {
            lenet
                .InferArgsMap (
                    ctx_cpu    ,
                    & args_map ,
                    args_map
                )
            ; //
            Xavier
                xavier =
                    Xavier (
                        Xavier::gaussian,
                        Xavier::in,
                        2.34
                    )
            ; //
            for ( auto & x : args_map ) {
                xavier (
                    x.first ,
                    & x.second
                ) ;
            }
            NDArray::WaitAll();
        }
        //
        inline TYPE_DATA
        ValAccuracy (
            size_t index = 0
        ) {
            auto images =
                READER_TEST_DATA
                    (index)
            ; //
            auto label =
                READER_TEST_LABEL
                    (index)
            ; //
            size_t correct_count = 0 ;
            size_t all_count     = 0 ;
            for (
                size_t i=0;
                (i<images()) && (i<1000000) ;
                i++
            ) {
                auto dptr =
                    images (i)
                        .FLATTEN  ()
                        .GET_DATA ()
                ; //
                auto lptr =
                    label (i)
                        .FLATTEN  ()
                        .GET_DATA ()
                ; //
                args_map["data"]
                    .SyncCopyFromCPU (
                        dptr ,
                        ImageArray::SIZE ()
                    )
                ; //
                args_map["data_label"]
                    .SyncCopyFromCPU (
                        lptr ,
                        LabelArray::SIZE()
                    )
                ; //
                args_map["data"]
                    .WaitToRead()
                ; //
                args_map["data_label"]
                    .WaitToRead()
                ; //
                exe->Forward(false);
                const auto & out =
                    exe->outputs
                ; //
                NDArray out_cpu =
                    out[0].Copy(ctx_cpu)
                ; //
                NDArray::WaitAll();
                NetOutputArray const & output =
                    (NetOutputArray const &)
                        out_cpu.GetData()[0]
                ; //
                LabelArray Predicts ;
                for(size_t j=0;j<output.SIZE_Y();j++){
                    if(output[j][0]>output[j][1]){Predicts[j]=0.0;}
                    else{Predicts[j]=1.0;}
                }
                Predicts -=
                    label(i)
                ; //
                correct_count +=
                    BatchSize -
                    ((size_t)Predicts.L1_NORM())
                ; //
                all_count +=
                    BatchSize
                ; //
            }
            TYPE_DATA ret =
                TYPE_DATA(correct_count) /
                TYPE_DATA(all_count)
            ; //
            return
                ret
            ; //
        }
        //
        inline void
        WriteParams () {
            printf (
                "Writing Parameters...\n"
            ) ; //
            std::string
                outparsname (
                    "./OUTS/PARS/CNN_PARS"
                )
            ; //
            CPPFileIO::FileFD
                filefd (outparsname)
            ; //
            filefd.writefile();
            NDArray::WaitAll();
            for (auto & x: args_map) {
                if (
                    ( x.first != "data"       ) &&
                    ( x.first != "data_label" )
                ) {
                    filefd.multiwrite2file(
                        x.second.GetData()[0] ,
                        x.second.Size()
                    ) ; //
                }
            }
            printf (
                "Done Writing...\n"
            ) ; //
        }
        //
        inline void
        ReadParams () {
            printf (
                "Reading Parameters...\n"
            ) ; //
            std::string
                outparsname (
                    "./OUTS/PARS/CNN_PARS"
                )
            ; //
            CPPFileIO::FileFD
                filefd (outparsname)
            ; //
            filefd.readfile();
            NDArray::WaitAll();
            for ( auto & x : args_map ) {
                if (
                    ( x.first != "data"       ) &&
                    ( x.first != "data_label" )
                ) {
                    size_t const bufsize =
                        x.second.Size()
                    ; //
                    std::vector <TYPE_DATA>
                        buf (bufsize)
                    ; //
                    filefd.multiread2file (
                        buf[0],
                        bufsize
                    ) ; //
                    x.second
                        .SyncCopyFromCPU (
                            &(buf[0]) ,
                            bufsize
                        )
                    ; //
                    x.second.WaitToRead() ;
                }
            }
            NDArray::WaitAll();
            printf (
                "Done Reading...\n"
            ) ; //
        }
        //
        inline void
        Train (
            size_t const N_Epoch ,
            size_t const index
        ) {
            printf
                ("Started Training: %ld epoch on %ld index\n",N_Epoch,index)
            ; //
            /* Checking Accuracy: */ if (true) {
                printf (
                    "Checking accuracy... BEFORE:\n"
                ) ; //
                auto acc =
                    ValAccuracy (
                        index
                    )
                ; //
                printf (
                    "Accuracy = %e\n" ,
                    acc
                ) ; //
            }
            auto images =
                READER_DATA (index)
            ; //
            auto label =
                READER_LABEL (index)
            ; //
            /* Use executor from network: */ {
                auto arg_names =
                    lenet.ListArguments()
                ; //
                for (
                    size_t epoch = 0 ;
                    epoch < N_Epoch  ;
                    epoch++
                ) {
                    for (
                        size_t i=0;
                        (i<images())&&(i<5000000);
                        i++
                    ) {
                        auto dptr =
                            images (i)
                                .FLATTEN  ()
                                .GET_DATA ()
                        ; //
                        auto lptr =
                            label (i)
                                .FLATTEN  ()
                                .GET_DATA ()
                        ; //
                        args_map["data"]
                            .SyncCopyFromCPU (
                                dptr ,
                                ImageArray::SIZE ()
                            )
                        ; //
                        args_map["data_label"]
                            .SyncCopyFromCPU (
                                lptr ,
                                LabelArray::SIZE()
                            )
                        ; //
                        args_map["data"]
                            .WaitToRead()
                        ; //
                        args_map["data_label"]
                            .WaitToRead()
                        ; //
                        exe->Forward(true);
                        exe->Backward();
                        for (
                            size_t i = 0 ;
                            i < arg_names.size() ;
                            i++
                        ) {
                            if (
                                ( arg_names[i] != "data"       ) &&
                                ( arg_names[i] != "data_label" )
                            ) {
                                opt->Update (
                                    i, exe->arg_arrays[i],
                                    exe->grad_arrays[i]
                                ) ; //
                            }
                        }
                        NDArray::WaitAll();
                    }
                    WriteParams();
                }
            }
            /* Checking Accuracy: */ if (true) {
                printf("Checking accuracy... AFTER:\n");
                auto acc =
                    ValAccuracy (
                        index
                    )
                ; //
                printf("Accuracy = %e\n",acc);
            }
        }
        //
        inline void
        Start () {
            WriteParams();
        }
        //
        inline void
        Train () {
            ReadParams () ;
            for (
                size_t i = 0 ;
                i < 8        ;
                i = i + 2
            ) {
                Train ( 2 , i   ) ;
                Train ( 2 , i+1 ) ;
                Train ( 2 , i   ) ;
                Train ( 2 , i+1 ) ;
            }
        }
        //
        MainNet () :
        ctx_cpu (
            Context (
                DeviceType::kCPU,
                0
            )
        ) {
            /* Network Part: */ {
                PreapreArrays
                    ()
                ; //
                lenet =
                    GetNetSymbol ()
                ; //
                InitParams();
            }
            /* Optimizer Part: */ {
                opt =
                    OptimizerRegistry::Find
                        ("adam")
                ; //
                opt
                    ->SetParam ( "lr"           , 0.001     )
                    ->SetParam ( "beta1"        , 0.9       )
                    ->SetParam ( "beta2"        , 0.99      )
                    ->SetParam ( "epsilon"      , 0.0000001 )
                    ->SetParam ( "wd"           , 0.001     )
                    ->SetParam ( "rescale_grad" , 1.0       )
                ; //
            }
            /* Executor part: */ {
                exe =
                    lenet
                        .SimpleBind (
                            ctx_cpu  ,
                            args_map
                        )
                ;
            }
        }
        //
        ~MainNet() {
            printf("Started Destructor...\n");
            NDArray::WaitAll();
            /* Clean memory: */ {
                delete
                    exe
                ; //
                delete
                    opt
                ; //
            }
            NDArray::WaitAll();
            MXNotifyShutdown();
            printf("Done Destructor...\n");
        }
        //
        static inline void
        START () {
            CPPFileIO::ForkMe
                forker
            ; //
            if (forker.InKid ()) {
                MainNet slave ;
                slave.Start() ;
            }
        }
        //
        static inline void
        TRAIN () {
            MainNet slave ;
            slave.Train() ;
            if(false){
                CPPFileIO::ForkMe
                    forker
                ; //
                if (forker.InKid ()) {
                    MainNet slave ;
                    slave.Train() ;
                }
            }

        }
        //
    } ;
    //
    class AE_MXNET {
    public:
        //
        using ImageArray =
            Tensors::NN::N2D_ARRAY <
                BatchSize , (
                    ImageResolution *
                    ImageResolution
                ) , TYPE_DATA
            >
        ; //
        //
        using ResultLossType =
            Tensors::NN::ND_ARRAY
                <BatchSize,TYPE_DATA>
        ; //
        using MainImageReader =
            CPPFileIO::FullFileReader
                <ImageArray>
        ; //
        //
        ImageArray *
            Buffer
        ; //
        //
        ResultLossType *
            Result_Buffer
        ; //
        //
        Symbol
            MainNet
        ; //
        //
        std::map
            <std::string,NDArray>
                args_map
        ; //
        //
        Context
            ctx_cpu
        ; //
        //
        Optimizer *
            opt
        ; //
        //
        Executor *
            exe
        ; //
        //
        std::vector
            <std::string>
                arg_names
        ; //
        //
        inline void
        WriteParams () {
            printf (
                "Writing Parameters...\n"
            ) ; //
            std::string
                outparsname (
                    "./OUTS/PARS/NET_PARS"
                )
            ; //
            CPPFileIO::FileFD
                filefd (outparsname)
            ; //
            filefd.writefile () ;
            NDArray::WaitAll () ;
            for (
                auto & x :
                    args_map
            ) if (
                ( x.first != "data"  ) &&
                ( x.first != "label" )
            ) {
                filefd.multiwrite2file (
                    x.second.GetData () [0] ,
                    x.second.Size    ()
                ) ; //
            }
            printf (
                "Done Writing...\n"
            ) ; //
        }
        //
        inline void
        ReadParams () {
            printf (
                "Reading Parameters...\n"
            ) ; //
            std::string
                outparsname (
                    "./OUTS/PARS/NET_PARS"
                )
            ; //
            CPPFileIO::FileFD
                filefd (outparsname)
            ; //
            filefd.readfile  () ;
            NDArray::WaitAll () ;
            for (
                auto & x :
                    args_map
            ) if (
                ( x.first != "data"  ) &&
                ( x.first != "label" )
            ) {
                size_t const bufsize =
                    x.second.Size()
                ; //
                std::vector <TYPE_DATA>
                    buf (bufsize)
                ; //
                filefd.multiread2file (
                    buf[0],
                    bufsize
                ) ; //
                x.second
                    .SyncCopyFromCPU (
                        &(buf[0]) ,
                        bufsize
                    )
                ; //
                x.second.WaitToRead() ;
            }
            NDArray::WaitAll();
            printf (
                "Done Reading...\n"
            ) ; //
        }
        //
        inline void
        UpdateParms () {
            for (
                size_t i = 0 ;
                i < arg_names.size() ;
                i++
            ) if (
                ( arg_names[i] != "data"  ) &&
                ( arg_names[i] != "label" )
            ) {
                opt->Update (
                    i, exe->arg_arrays[i],
                    exe->grad_arrays[i]
                ) ; //
            }
            NDArray::WaitAll();
        }
        //
        inline void
        ReadData () {
            args_map["data"]
                .SyncCopyFromCPU (
                    Buffer->GET_DATA() ,
                    ImageArray::SIZE ()
                )
            ; //
            if (false) {
                args_map["label"]
                    .SyncCopyFromCPU (
                        Buffer->GET_DATA() ,
                        ImageArray::SIZE ()
                    )
                ; //
                args_map["label"]
                    .WaitToRead()
                ; //
            }
            args_map["data"]
                .WaitToRead()
            ; //
        }
        //
        inline void
        train () {
            ReadData () ;
            exe->Forward(true);
            exe->Backward();
            UpdateParms () ;
        }
        //
        inline TYPE_DATA
        ValAccuracy () {
            ReadData () ;
            exe->Forward
                (false)
            ;
            const auto &
                out =
                    exe->outputs
            ; //
            NDArray out_cpu =
                out[0].Copy
                (ctx_cpu)
            ; //
            NDArray::WaitAll();
            ImageArray const &
                output =
                    (ImageArray const &)
                        out_cpu.GetData()[0]
            ; //
            ImageArray diff =
                Buffer[0] -
                output
            ; //
            /* Evaluate the loss */ {
                if (
                    !CPPFileIO::is_junked
                        (Result_Buffer)
                ) {
                    for (
                        size_t i = 0              ;
                        i < Result_Buffer->SIZE() ;
                        i++
                    ) {
                        Result_Buffer[0][i] =
                            std::sqrt (
                                diff[i].L2_NORM()
                            )
                        ; //
                    }
                }
            }
            return
                diff.L2_NORM() /
                ((TYPE_DATA)BatchSize)
            ; //
        }
        //
        inline void
        Train (
            size_t index
        ) {
            //
            char tmp [512] ; //
            sprintf (
                tmp,
                "./OUTS/QCD/TRAIN/%ld/image",
                index
            ) ; //
            std::string
                filename (tmp)
            ;
            //
            MainImageReader
                Reader (filename)
            ; //
            //
            size_t Limit =
                CPPFileIO::mymin (
                    Reader() ,
                    (size_t)5000000
                )
            ; //
            //
            for (
                size_t epoch = 0 ;
                epoch < 2        ;
                epoch++
            ) {
                for (
                    size_t i = 0 ;
                    i < Limit ;
                    i++
                ) {
                    Buffer =
                        &(Reader(i))
                    ; //
                    train () ;
                    //
                }
                WriteParams();
            }
            //
        }
        //
        inline void
        Validate (
            size_t index
        ) {
            //
            CPPFileIO::set_junked
                (Result_Buffer)
            ;
            //
            char tmp[512];
            //
            sprintf (
                tmp,
                "./OUTS/QCD/TEST/%ld/image",
                index
            ) ; //
            //
            std::string
                Filename (tmp)
            ; //
            //
            MainImageReader
                Reader (Filename)
            ; //
            //
            TYPE_DATA
                diff =
                    0
            ; //
            //
            size_t Limit =
                CPPFileIO::mymin (
                    Reader() ,
                    (size_t)5000000
                )
            ; //
            for (
                size_t i = 0 ;
                i < Limit ;
                i++
            ) {
                Buffer =
                    &(Reader(i))
                ; //
                diff +=
                    ValAccuracy ()
                ; //
            }
            diff /=
                ((TYPE_DATA)Limit)
            ; //
            printf("Error = %e\n",diff);
        }
        //
        inline void
        MakeLoss (
            std::string dirname
        ) {
            std::string imagename =
                dirname + "/image"
            ; //
            std::string lossname =
                dirname + "/loss"
            ; //
            //
            MainImageReader
                Reader (imagename)
            ; //
            CPPFileIO::FileVector
                <ResultLossType>
                    Writer (lossname)
            ; //
            //
            TYPE_DATA
                diff =
                    0
            ; //
            //
            size_t Limit =
                CPPFileIO::mymin (
                    Reader() ,
                    (size_t)5000000
                )
            ; //
            ResultLossType
                tmpbuf
            ; //
            Result_Buffer =
                & tmpbuf
            ; //
            for (
                size_t i = 0 ;
                i < Limit ;
                i++
            ) {
                Buffer =
                    &(Reader(i))
                ; //
                diff +=
                    ValAccuracy ()
                ; //
                Writer.push_back
                    (tmpbuf)
                ; //
            }
            diff /=
                ((TYPE_DATA)Limit)
            ; //
            printf("Error = %e\n",diff);
        }
        //
        inline void
        MakeLoss () {
            ReadParams () ;
            std::vector <std::string>
                filenames
            ; //
            /* Prepare the list of filenames: */ {
                filenames.push_back("./OUTS/QCD/TEST/0");
                filenames.push_back("./OUTS/QCD/TEST/1");
                filenames.push_back("./OUTS/QCD/TEST/2");
                filenames.push_back("./OUTS/QCD/TEST/3");
                filenames.push_back("./OUTS/QCD/TEST/4");
                filenames.push_back("./OUTS/QCD/TEST/5");
                filenames.push_back("./OUTS/QCD/TEST/6");
                filenames.push_back("./OUTS/QCD/TEST/7");
                filenames.push_back("./OUTS/QCD/TRAIN/0");
                filenames.push_back("./OUTS/QCD/TRAIN/1");
                filenames.push_back("./OUTS/QCD/TRAIN/2");
                filenames.push_back("./OUTS/QCD/TRAIN/3");
                filenames.push_back("./OUTS/QCD/TRAIN/4");
                filenames.push_back("./OUTS/QCD/TRAIN/5");
                filenames.push_back("./OUTS/QCD/TRAIN/6");
                filenames.push_back("./OUTS/QCD/TRAIN/7");
                filenames.push_back("./OUTS/TOP/TEST/0");
                filenames.push_back("./OUTS/TOP/TEST/1");
                filenames.push_back("./OUTS/TOP/TEST/2");
                filenames.push_back("./OUTS/TOP/TEST/3");
                filenames.push_back("./OUTS/TOP/TEST/4");
                filenames.push_back("./OUTS/TOP/TEST/5");
                filenames.push_back("./OUTS/TOP/TEST/6");
                filenames.push_back("./OUTS/TOP/TEST/7");
                filenames.push_back("./OUTS/TOP/TRAIN/0");
                filenames.push_back("./OUTS/TOP/TRAIN/1");
                filenames.push_back("./OUTS/TOP/TRAIN/2");
                filenames.push_back("./OUTS/TOP/TRAIN/3");
                filenames.push_back("./OUTS/TOP/TRAIN/4");
                filenames.push_back("./OUTS/TOP/TRAIN/5");
                filenames.push_back("./OUTS/TOP/TRAIN/6");
                filenames.push_back("./OUTS/TOP/TRAIN/7");
                filenames.push_back("./OUTS/WBS/TEST/0");
                filenames.push_back("./OUTS/WBS/TEST/1");
                filenames.push_back("./OUTS/WBS/TEST/2");
                filenames.push_back("./OUTS/WBS/TEST/3");
                filenames.push_back("./OUTS/WBS/TEST/4");
                filenames.push_back("./OUTS/WBS/TEST/5");
                filenames.push_back("./OUTS/WBS/TEST/6");
                filenames.push_back("./OUTS/WBS/TEST/7");
                filenames.push_back("./OUTS/WBS/TRAIN/0");
                filenames.push_back("./OUTS/WBS/TRAIN/1");
                filenames.push_back("./OUTS/WBS/TRAIN/2");
                filenames.push_back("./OUTS/WBS/TRAIN/3");
                filenames.push_back("./OUTS/WBS/TRAIN/4");
                filenames.push_back("./OUTS/WBS/TRAIN/5");
                filenames.push_back("./OUTS/WBS/TRAIN/6");
                filenames.push_back("./OUTS/WBS/TRAIN/7");
            }
            for (size_t i=0;i<filenames.size();i++) {
                MakeLoss(filenames[i]);
            }
        }
        //
        inline void
        GetNetSymbol () {
            size_t
                sizes[3] = {
                    ImageResolution * ImageResolution ,
                    24              * 24              ,
                    10              * 10
                }
            ; //
            //
            Symbol data =
                Symbol::Variable
                    ("data")
            ; //
            args_map["data"] =
                NDArray (
                    Shape (
                        BatchSize ,
                        sizes[0]
                    ) ,
                    ctx_cpu , false
                )
            ; //
            //
            Symbol
                fc1_w("fc1_w") ,
                fc1_b("fc1_b")
            ; //
            Symbol fc1 =
                FullyConnected (
                    "fc1" , data  ,
                    fc1_w , fc1_b ,
                    sizes[1]
                )
            ; //
            Symbol fa1 =
                relu ("fa1",fc1)
            ;
            //
            Symbol
                fc2_w("fc2_w") ,
                fc2_b("fc2_b")
            ; //
            Symbol fc2 =
                FullyConnected (
                    "fc2" , fa1   ,
                    fc2_w , fc2_b ,
                    sizes[2]
                )
            ; //
            Symbol fa2 =
                relu ("fa2",fc2)
            ; //
            //
            Symbol
                fc3_w("fc3_w") ,
                fc3_b("fc3_b")
            ; //
            Symbol fc3 =
                FullyConnected (
                    "fc3" , fa2   ,
                    fc3_w , fc3_b ,
                    sizes[1]
                )
            ; //
            Symbol fa3 =
                relu ("fa3",fc3)
            ; //
            //
            Symbol
                fc4_w("fc4_w") ,
                fc4_b("fc4_b")
            ; //
            Symbol fc4 =
                FullyConnected (
                    "fc4" , fa3   ,
                    fc4_w , fc4_b ,
                    sizes[0]
                )
            ; //
            Symbol fa4 =
                relu ("fa4",fc4)
            ; //
            //
            Symbol
                fc5_w("fc5_w") ,
                fc5_b("fc5_b")
            ; //
            Symbol fc5 =
                FullyConnected (
                    "fc5" , fa4   ,
                    fc5_w , fc5_b ,
                    sizes[0]
                )
            ; //
            Symbol Out =
                softmax (
                    "softmaxout" ,
                    fc5 , 1
                )
            ; //
            //
            if (false) {
                Symbol label =
                    Symbol::Variable
                        ("label")
                ; //
                args_map["label"] =
                    NDArray (
                        Shape (
                            BatchSize ,
                            sizes[0]
                        ) ,
                        ctx_cpu , false
                    )
                ; //
            }
            Symbol FinalNet =
                LinearRegressionOutput (
                    "Teacher" , Out , data ,
                    ImageResolution * ImageResolution
                )
            ; //
            //
            arg_names =
                FinalNet
                    .ListArguments()
            ; //
            //
            MainNet =
                FinalNet
            ; //
        }
        //
        inline void
        AllocateMemory () {
            /* Optimizer Part : */ {
                opt =
                    OptimizerRegistry::Find
                        ("adam")
                ; //
                opt
                    ->SetParam ( "lr"           , 0.001     )
                    ->SetParam ( "beta1"        , 0.9       )
                    ->SetParam ( "beta2"        , 0.99      )
                    ->SetParam ( "epsilon"      , 0.0000001 )
                    ->SetParam ( "wd"           , 0.0       )
                    ->SetParam ( "rescale_grad" , 1.0       )
                ; //
            }
            /* Executor part : */ {
                exe =
                    MainNet
                        .SimpleBind (
                            ctx_cpu  ,
                            args_map
                        )
                ;
            }
        }
        //
        inline void
        SetUpNet () {
            GetNetSymbol () ;
            MainNet
                .InferArgsMap (
                    ctx_cpu    ,
                    & args_map ,
                    args_map
                )
            ; //
            Xavier
                xavier =
                    Xavier (
                        Xavier::gaussian ,
                        Xavier::in       ,
                        2.34
                    )
            ; //
            for ( auto & x : args_map ) {
                xavier (
                    x.first ,
                    & x.second
                ) ;
            }
            NDArray::WaitAll();
            AllocateMemory () ;
        }
        //
        inline void
        FreeUpMemory () {
            /* Clean memory: */ {
                NDArray::WaitAll();
                delete exe ;
                delete opt ;
                NDArray::WaitAll();
            }
            MXNotifyShutdown();
        }
        //
        inline void
        BeginTrain (
            size_t const
                limit = 6
        ) {
            ReadParams () ;
            for(size_t e=0;e<limit;e++){
                for(size_t i=0;i<4;i++){
                    printf   ( "Before: " ) ;
                    Validate (  2*i       ) ;
                    Train    (  2*i       ) ;
                    printf   ( "After: "  ) ;
                    Validate (  2*i       ) ;
                    printf   ( "Before: " ) ;
                    Validate ( (2*i) + 1  ) ;
                    Train    ( (2*i) + 1  ) ;
                    printf   ( "After: "  ) ;
                    Validate ( (2*i) + 1  ) ;
                }
            }
        }
        //
        inline void
        Start () {
            WriteParams () ;
        }
        //
        AE_MXNET() :
        ctx_cpu (
            Context (
                DeviceType::kCPU ,
                0
            )
        ) {
            SetUpNet () ;
        }
        //
        ~AE_MXNET(){
            FreeUpMemory();
            printf(
                "Done Destructor...\n"
            ) ; //
        }
        //
    } ;
}
