#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "mxnet-cpp/MxNetCpp.h"

#include "all.hh"
//#include "utils.h"

namespace MXNET_CHECK {
	//
    using MISC::TYPE_DATA		;
    using MISC::imagetype		;
    using MISC::ImageResolution	;
	using namespace mxnet::cpp	;
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
            Tensors::Array::N2D_ARRAY <
                BatchSize , (
                    ImageResolution *
                    ImageResolution
                ) , TYPE_DATA
            >
        ; //
        using LabelArray =
            Tensors::Array::ND_ARRAY <
                BatchSize ,
                TYPE_DATA
            >
        ; //
        using NetOutputArray =
            Tensors::Array::N2D_ARRAY <
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
            Symbol fc1ReLU1 =
                relu(fc1)
            ; //
            //
            // FC PART 2 :
            Symbol
                fc2_w("fc2_w"),
                fc2_b("fc2_b")
            ; //
            Symbol fc2 =
                FullyConnected (
                    "fc2", fc1ReLU1,
                    fc2_w, fc2_b, 2
                )
            ; //
            Symbol fc2ReLU =
                relu(fc2)
            ; //
            // Final Output :
            Symbol lenet =
                SoftmaxOutput (
                    "softmax",
                    fc2ReLU, data_label
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
                        (i<images())&&(i<500000);
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
            Tensors::Array::N2D_ARRAY <
                BatchSize , (
                    ImageResolution *
                    ImageResolution
                ) , TYPE_DATA
            >
        ; //
        using ResultLossType =
            Tensors::Array::ND_ARRAY
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
		std::string const
		ParsFileName () {
			std::string
				ret (
					"./OUTS/PARS/NET_PARS"
				)
			; //
			return
				ret
			; //
		}

        inline void
        WriteParams () {
            printf (
                "Writing Parameters...\n"
            ) ; //
            CPPFileIO::FileFD
                filefd (ParsFileName())
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

        inline void
        ReadParams () {
            printf (
                "Reading Parameters...\n"
            ) ; //
            CPPFileIO::FileFD
                filefd (ParsFileName())
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
                    buf[0]	,
                    bufsize
                ) ; //
                x.second
                    .SyncCopyFromCPU (
                        &(buf[0])	,
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

		inline void
		ReadData () {
			/* Begin Reading: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
				if (false) /* Label Part: */ { // label not needed for AE
					args_map["label"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
			}
			/* Begin Synchronizing: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.WaitToRead()
					; //
				}
				if (false) /* label part: */ { // label not needed for AE
					args_map["label"]
						.WaitToRead()
					; //
				}
			}
		}

		inline void
		train () {
			ReadData		()		;
			exe->Forward	(true)	;
			exe->Backward	()		;
			UpdateParms		()		;
		}

		inline TYPE_DATA
		ValAccuracy () {

			ReadData () ;

			exe->Forward
				(false)
			;
			auto const &
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
					reinterpret_cast
						<ImageArray const &>
						(out_cpu.GetData()[0])
			; //
			ImageArray const diff =
				Buffer[0] -
				output
			; //

            /* Evaluate the loss */ {
				if (
					!CPPFileIO::is_junked
						(Result_Buffer)
				) for (
					size_t i = 0				;
					i < Result_Buffer->SIZE()	;
					i++
				) {
					Result_Buffer[0][i] =
						std::sqrt (
							diff[i].L2_NORM()
						)
					; //
				}
			}

			return
				diff.L2_NORM() /
				static_cast<TYPE_DATA>
					(BatchSize)
			; //

		}

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
                size_t epoch = 0	;
                epoch < 2			;
                epoch++
            ) {
                for (
                    size_t i = 0	;
                    i < Limit		;
                    i++
                ) {
                    Buffer =
                        &(Reader(i))
                    ; //
                    train () ;
                }
                WriteParams () ;
            }
        }

		inline void
		Validate (
			size_t index
		) {

			CPPFileIO::set_junked
				(Result_Buffer)
			;

			char
				tmp[512]
			; /* Prepare the filename */ {
				sprintf (
					tmp,
					"./OUTS/QCD/TEST/%ld/image",
					index
				) ; //
			}

            std::string
                Filename (tmp)
            ; //

            MainImageReader
                Reader (Filename)
            ; //

            TYPE_DATA
                diff = 0
            ; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

            printf("Error = %e\n",diff);
        }

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

            MainImageReader
                Reader (imagename)
            ; //
            CPPFileIO::FileVector
                <ResultLossType>
                    Writer (lossname)
            ; //

            TYPE_DATA
                diff = 0
            ; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

            printf("Error = %e\n",diff);
        }

        inline void
        MakeLoss () {

			ReadParams () ;

            std::vector <std::string>
                filenames
            ; /* Prepare the list of filenames: */ {

				if (true) {
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
				}

				if (true) {
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
				}

				if(false){
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

            }

			for (
				size_t i=0;
				i<filenames.size();
				i++
			) {
				MakeLoss
					(filenames[i])
				; //
			}

        }
        //
        inline void
        GetNetSymbol () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
            size_t
                sizes[3] = {
                    ImageResolution * ImageResolution ,
                    24              * 24              ,
                    10              * 10
                }
            ; //

            ////////////////
            // Bind Data: //
            ////////////////
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

            //////////////
            // Layer 1: //
            //////////////
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

			//////////////
			// Layer 2: //
			//////////////
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

            //////////////
            // Layer 3: //
            //////////////
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

            //////////////
            // Layer 4: //
            //////////////
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

            //////////////
            // Layer 5: //
            //////////////
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

            ////////////////////////////////
            // SoftMaxing (Normalizing) : //
            ////////////////////////////////
            Symbol Out =
                softmax (
                    "softmaxout" ,
                    fc5 , 1
                )
            ; //

			///////////////////////////
            // Not Necessary for AR: //
            ///////////////////////////
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


            /////////////////////////////////////
            // Final Linear Regression output: //
            /////////////////////////////////////
            Symbol FinalNet =
                LinearRegressionOutput (
                    "Teacher" , Out , data ,
                    ImageResolution * ImageResolution
                )
            ; //

            /////////////////
            // Wrapping up //
            /////////////////
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
			//
            /* Optimizer Part : */ {
                opt =
                    OptimizerRegistry::Find
                        ("adam")
                ; //
                opt
                    ->SetParam ( "lr"           , 0.0001    )
                    ->SetParam ( "beta1"        , 0.9       )
                    ->SetParam ( "beta2"        , 0.99      )
                    ->SetParam ( "epsilon"      , 0.0000001 )
                    ->SetParam ( "wd"           , 0.0       )
                    ->SetParam ( "rescale_grad" , 1.0       )
                ; //
            }
            //
            /* Executor part : */ {
                exe =
                    MainNet
                        .SimpleBind (
                            ctx_cpu  ,
                            args_map
                        )
                ;
            }
            //
        }
        //
		inline void
		SetUpNet () {
			GetNetSymbol () ;
			MainNet
				.InferArgsMap (
					ctx_cpu		,
					& args_map	,
					args_map
				)
			; //
			Xavier
				xavier =
					Xavier (
						Xavier::gaussian	,
						Xavier::in			,
						2.34
					)
			; //
			for (
				auto & x :
					args_map
			) {
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
				NDArray::WaitAll()	;
				delete exe			;
				delete opt			;
				NDArray::WaitAll()	;
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
		Start ()
		{ WriteParams () ; }
        //
        AE_MXNET() :
        ctx_cpu (
            Context (
                DeviceType::kCPU ,
                0
            )
        ) { SetUpNet () ; }
        //
        ~AE_MXNET(){
            FreeUpMemory();
            printf(
                "Done Destructor...\n"
            ) ; //
        }
        //
    } ;
	//
	class AE_MXNET_UNBOOSTED {
	public:
		//
		using ImageArray =
			Tensors::Array::N2D_ARRAY <
				BatchSize , (
					ImageResolution *
					ImageResolution
				) , TYPE_DATA
			>
		; //
		using ResultLossType =
			Tensors::Array::ND_ARRAY
				<BatchSize,TYPE_DATA>
		; //
		using MainImageReader =
			CPPFileIO::FullFileReader
				<ImageArray>
		; //

		ImageArray *
			Buffer
		; //
		ResultLossType *
			Result_Buffer
		; //

		Symbol
			MainNet
		; //

		std::map
			<std::string,NDArray>
				args_map
		; //

		Context
			ctx_cpu
		; //

		Optimizer *
			opt
		; //

		Executor *
			exe
		; //

		std::vector
			<std::string>
				arg_names
		; //

		std::string const
		ParsFileName () {
			std::string
				ret (
					"./OUTS/PARS/NET_PARS_UNBOOSTED"
				)
			;
			return ret ;
		}

		inline void
		WriteParams () {
			printf (
				"Writing Parameters...\n"
			) ; //
			CPPFileIO::FileFD
				filefd (ParsFileName())
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
					x.second.GetData	() [0]	,
					x.second.Size		()
				) ; //
			}
			printf (
				"Done Writing...\n"
			) ; //
		}

		inline void
		ReadParams () {
			printf (
				"Reading Parameters...\n"
			) ; //
			CPPFileIO::FileFD
				filefd (ParsFileName())
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
					buf[0]	,
					bufsize
				) ; //
				x.second
					.SyncCopyFromCPU (
						&(buf[0])	,
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

		inline void
		ReadData () {
			/* Begin Reading: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
				if (false) /* Label Part: */ { // label not needed for AE
					args_map["label"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
			}
			/* Begin Synchronizing: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.WaitToRead()
					; //
				}
				if (false) /* label part: */ { // label not needed for AE
					args_map["label"]
						.WaitToRead()
					; //
				}
			}
		}

		inline void
		train () {
			ReadData		()		;
			exe->Forward	(true)	;
			exe->Backward	()		;
			UpdateParms		()		;
		}

		inline TYPE_DATA
		ValAccuracy () {

			ReadData () ;

			exe->Forward
				(false)
			;
			auto const &
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
					reinterpret_cast
						<ImageArray const &>
						(out_cpu.GetData()[0])
			; //
			ImageArray const diff =
				Buffer[0] -
				output
			; //

			/* Evaluate the loss */ {
				if (
					!CPPFileIO::is_junked
						(Result_Buffer)
				) for (
					size_t i = 0				;
					i < Result_Buffer->SIZE()	;
					i++
				) {
					Result_Buffer[0][i] =
						std::sqrt (
							diff[i].L2_NORM()
						)
					; //
				}
			}

			return
				diff.L2_NORM() /
				static_cast<TYPE_DATA>
					(BatchSize)
			; //

		}

		inline void
		Train (
			size_t index
		) {
			//
			char tmp [512] ; //
			sprintf (
				tmp,
				"./OUTS/QCD/TRAIN/%ld/UnBoostedimage",
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
				size_t epoch = 0	;
				epoch < 2			;
				epoch++
			) {
				for (
					size_t i = 0	;
					i < Limit		;
					i++
				) {
					Buffer =
						&(Reader(i))
					; //
					train () ;
				}
				WriteParams () ;
			}
		}

		inline void
		Validate (
			size_t index
		) {

			CPPFileIO::set_junked
				(Result_Buffer)
			;

			char
				tmp[512]
			; /* Prepare the filename */ {
				sprintf (
					tmp,
					"./OUTS/QCD/TEST/%ld/UnBoostedimage",
					index
				) ; //
			}

			std::string
				Filename (tmp)
			; //

			MainImageReader
				Reader (Filename)
			; //

			TYPE_DATA
				diff = 0
			; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

			printf("Error = %e\n",diff);
		}

		inline void
		MakeLoss (
			std::string dirname
		) {

			std::string imagename =
				dirname +
				"/UnBoostedimage"
			; //
			std::string lossname =
				dirname +
				"/UnBoostedloss"
			; //

			MainImageReader
				Reader (imagename)
			; //
			CPPFileIO::FileVector
				<ResultLossType>
					Writer (lossname)
			; //

			TYPE_DATA
				diff = 0
			; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

			printf (
				"Error = %e\n",
				diff
			) ; //
		}

		inline void
		MakeLoss () {

			ReadParams () ;

			std::vector <std::string>
				filenames
			; /* Prepare the list of filenames: */ {

				if (true) {
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
				}

				if (true) {
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
				}

				if(false){
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

			}

			for (
				size_t i=0;
				i<filenames.size();
				i++
			) {
				MakeLoss
					(filenames[i])
				; //
			}

		}
		//
		inline void
		GetNetSymbol () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
			size_t
				sizes[3] = {
					ImageResolution * ImageResolution ,
					24              * 24              ,
					10              * 10
				}
			; //

			////////////////
			// Bind Data: //
			////////////////
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

			//////////////
			// Layer 1: //
			//////////////
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

			//////////////
			// Layer 2: //
			//////////////
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

			//////////////
			// Layer 3: //
			//////////////
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

			//////////////
			// Layer 4: //
			//////////////
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

			//////////////
			// Layer 5: //
			//////////////
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

			////////////////////////////////
			// SoftMaxing (Normalizing) : //
			////////////////////////////////
			Symbol Out =
				softmax (
					"softmaxout" ,
					fc5 , 1
				)
			; //

			///////////////////////////
			// Not Necessary for AR: //
			///////////////////////////
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


			/////////////////////////////////////
			// Final Linear Regression output: //
			/////////////////////////////////////
			Symbol FinalNet =
				LinearRegressionOutput (
					"Teacher" , Out , data ,
					ImageResolution * ImageResolution
				)
			; //

			/////////////////
			// Wrapping up //
			/////////////////
			arg_names =
				FinalNet
					.ListArguments()
			; //
			MainNet =
				FinalNet
			; //

		}
		//
		inline void
		AllocateMemory () {
			//
			/* Optimizer Part : */ {
				opt =
					OptimizerRegistry::Find
						("adam")
				; //
				opt
					->SetParam ( "lr"           , 0.0001    )
					->SetParam ( "beta1"        , 0.9       )
					->SetParam ( "beta2"        , 0.99      )
					->SetParam ( "epsilon"      , 0.0000001 )
					->SetParam ( "wd"           , 0.0       )
					->SetParam ( "rescale_grad" , 1.0       )
				; //
			}
			//
			/* Executor part : */ {
				exe =
					MainNet
						.SimpleBind (
							ctx_cpu  ,
							args_map
						)
				;
			}
			//
		}
		//
		inline void
		SetUpNet () {

			GetNetSymbol () ;

			/* Prepare the parameters: */ {
				MainNet
					.InferArgsMap (
						ctx_cpu		,
						& args_map	,
						args_map
					)
				; //
			}

			/* Randomize The Parameters: */ {
				Xavier
					xavier =
						Xavier (
							Xavier::gaussian	,
							Xavier::in			,
							2.34
						)
				; //
				for (
					auto & x :
						args_map
				) {
					xavier (
						x.first ,
						& x.second
					) ;
				}
				NDArray::WaitAll();
			}

			AllocateMemory () ;

		}
		//
		inline void
		FreeUpMemory () {
			/* Clean memory: */ {
				NDArray::WaitAll()	;
				delete exe			;
				delete opt			;
				NDArray::WaitAll()	;
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
		Start ()
		{ WriteParams () ; }
		//
		AE_MXNET_UNBOOSTED() :
		ctx_cpu (
			Context (
				DeviceType::kCPU ,
				0
			)
		) { SetUpNet () ; }
		//
		~AE_MXNET_UNBOOSTED(){
			FreeUpMemory();
			printf(
				"Done Destructor...\n"
			) ; //
		}
		//
	} ;
	//
	class AE_MXNET_NOGS {
	public:
		//
		using ImageArray =
			Tensors::Array::N2D_ARRAY <
				BatchSize , (
					ImageResolution *
					ImageResolution
				) , TYPE_DATA
			>
		; //
		using ResultLossType =
			Tensors::Array::ND_ARRAY
				<BatchSize,TYPE_DATA>
		; //
		using MainImageReader =
			CPPFileIO::FullFileReader
				<ImageArray>
		; //

		ImageArray *
			Buffer
		; //
		ResultLossType *
			Result_Buffer
		; //

		Symbol
			MainNet
		; //

		std::map
			<std::string,NDArray>
				args_map
		; //

		Context
			ctx_cpu
		; //

		Optimizer *
			opt
		; //

		Executor *
			exe
		; //

		std::vector
			<std::string>
				arg_names
		; //

		std::string const
		ParsFileName () {
			std::string
				ret (
					"./OUTS/PARS/NET_PARS_NOGS"
				)
			;
			return ret ;
		}

		inline void
		WriteParams () {
			printf (
				"Writing Parameters...\n"
			) ; //
			CPPFileIO::FileFD
				filefd (ParsFileName())
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
					x.second.GetData () [0]	,
					x.second.Size    ()
				) ; //
			}
			printf (
				"Done Writing...\n"
			) ; //
		}

		inline void
		ReadParams () {
			printf (
				"Reading Parameters...\n"
			) ; //
			CPPFileIO::FileFD
				filefd (ParsFileName())
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
					buf[0]	,
					bufsize
				) ; //
				x.second
					.SyncCopyFromCPU (
						&(buf[0])	,
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

		inline void
		ReadData () {
			/* Begin Reading: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
				if (false) /* Label Part: */ { // label not needed for AE
					args_map["label"]
						.SyncCopyFromCPU (
							Buffer->GET_DATA() ,
							ImageArray::SIZE ()
						)
					; //
				}
			}
			/* Begin Synchronizing: */ {
				if (true) /* Data Part: */ {
					args_map["data"]
						.WaitToRead()
					; //
				}
				if (false) /* label part: */ { // label not needed for AE
					args_map["label"]
						.WaitToRead()
					; //
				}
			}
		}

		inline void
		train () {
			ReadData		()		;
			exe->Forward	(true)	;
			exe->Backward	()		;
			UpdateParms		()		;
		}

		inline TYPE_DATA
		ValAccuracy () {

			ReadData () ;

			exe->Forward
				(false)
			;
			auto const &
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
					reinterpret_cast
						<ImageArray const &>
						(out_cpu.GetData()[0])
			; //
			ImageArray const diff =
				Buffer[0] -
				output
			; //

			/* Evaluate the loss */ {
				if (
					!CPPFileIO::is_junked
						(Result_Buffer)
				) for (
					size_t i = 0				;
					i < Result_Buffer->SIZE()	;
					i++
				) {
					Result_Buffer[0][i] =
						std::sqrt (
							diff[i].L2_NORM()
						)
					; //
				}
			}

			return
				diff.L2_NORM() /
				static_cast<TYPE_DATA>
					(BatchSize)
			; //

		}

		inline void
		Train (
			size_t index
		) {
			//
			char tmp [512] ; //
			sprintf (
				tmp,
				"./OUTS/QCD/TRAIN/%ld/NoGramSchmidtImageType",
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
				size_t epoch = 0	;
				epoch < 2			;
				epoch++
			) {
				for (
					size_t i = 0	;
					i < Limit		;
					i++
				) {
					Buffer =
						&(Reader(i))
					; //
					train () ;
				}
				WriteParams () ;
			}
		}

		inline void
		Validate (
			size_t index
		) {

			CPPFileIO::set_junked
				(Result_Buffer)
			;

			char
				tmp[512]
			; /* Prepare the filename */ {
				sprintf (
					tmp,
					"./OUTS/QCD/TEST/%ld/NoGramSchmidtImageType",
					index
				) ; //
			}

			std::string
				Filename (tmp)
			; //

			MainImageReader
				Reader (Filename)
			; //

			TYPE_DATA
				diff = 0
			; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

			printf("Error = %e\n",diff);
		}

		inline void
		MakeLoss (
			std::string dirname
		) {

			std::string imagename =
				dirname +
				"/NoGramSchmidtImageType"
			; //
			std::string lossname =
				dirname +
				"/NoGramSchmidtImageTypeLoss"
			; //

			MainImageReader
				Reader (imagename)
			; //
			CPPFileIO::FileVector
				<ResultLossType>
					Writer (lossname)
			; //

			TYPE_DATA
				diff = 0
			; //

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
				static_cast<TYPE_DATA>
					(Limit)
			; //

			printf (
				"Error = %e\n",
				diff
			) ; //
		}

		inline void
		MakeLoss () {

			ReadParams () ;

			std::vector <std::string>
				filenames
			; /* Prepare the list of filenames: */ {

				if (true) {
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
				}

				if (true) {
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
				}

				if(false){
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

			}

			for (
				size_t i=0;
				i<filenames.size();
				i++
			) {
				MakeLoss
					(filenames[i])
				; //
			}

		}
		//
		inline void
		GetNetSymbol () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
			size_t
				sizes[3] = {
					ImageResolution * ImageResolution ,
					24              * 24              ,
					10              * 10
				}
			; //

			////////////////
			// Bind Data: //
			////////////////
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

			//////////////
			// Layer 1: //
			//////////////
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

			//////////////
			// Layer 2: //
			//////////////
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

			//////////////
			// Layer 3: //
			//////////////
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

			//////////////
			// Layer 4: //
			//////////////
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

			//////////////
			// Layer 5: //
			//////////////
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

			////////////////////////////////
			// SoftMaxing (Normalizing) : //
			////////////////////////////////
			Symbol Out =
				softmax (
					"softmaxout" ,
					fc5 , 1
				)
			; //

			///////////////////////////
			// Not Necessary for AR: //
			///////////////////////////
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


			/////////////////////////////////////
			// Final Linear Regression output: //
			/////////////////////////////////////
			Symbol FinalNet =
				LinearRegressionOutput (
					"Teacher" , Out , data ,
					ImageResolution * ImageResolution
				)
			; //

			/////////////////
			// Wrapping up //
			/////////////////
			arg_names =
				FinalNet
					.ListArguments()
			; //
			MainNet =
				FinalNet
			; //

		}
		//
		inline void
		AllocateMemory () {
			//
			/* Optimizer Part : */ {
				opt =
					OptimizerRegistry::Find
						("adam")
				; //
				opt
					->SetParam ( "lr"           , 0.0001    )
					->SetParam ( "beta1"        , 0.9       )
					->SetParam ( "beta2"        , 0.99      )
					->SetParam ( "epsilon"      , 0.0000001 )
					->SetParam ( "wd"           , 0.0       )
					->SetParam ( "rescale_grad" , 1.0       )
				; //
			}
			//
			/* Executor part : */ {
				exe =
					MainNet
						.SimpleBind (
							ctx_cpu  ,
							args_map
						)
				;
			}
			//
		}
		//
		inline void
		SetUpNet () {

			GetNetSymbol () ;

			/* Prepare the parameters: */ {
				MainNet
					.InferArgsMap (
						ctx_cpu		,
						& args_map	,
						args_map
					)
				; //
			}

			/* Randomize The Parameters: */ {
				Xavier
					xavier =
						Xavier (
							Xavier::gaussian	,
							Xavier::in			,
							2.34
						)
				; //
				for (
					auto & x :
						args_map
				) {
					xavier (
						x.first ,
						& x.second
					) ;
				}
				NDArray::WaitAll();
			}

			AllocateMemory () ;

		}
		//
		inline void
		FreeUpMemory () {
			/* Clean memory: */ {
				NDArray::WaitAll()	;
				delete exe			;
				delete opt			;
				NDArray::WaitAll()	;
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
		Start ()
		{ WriteParams () ; }
		//
		AE_MXNET_NOGS() :
		ctx_cpu (
			Context (
				DeviceType::kCPU ,
				0
			)
		) { SetUpNet () ; }
		//
		~AE_MXNET_NOGS(){
			FreeUpMemory();
			printf(
				"Done Destructor...\n"
			) ; //
		}
		//
	} ;
}

namespace MXNET_CNN_AE {
	//
    using MISC::TYPE_DATA ;
    using MISC::imagetype ;
    using MISC::ImageResolution ;
	using namespace mxnet::cpp ;
    //
    size_t constexpr
        BatchSize =
            200
    ; //
    //
    size_t const
    Limit (
		size_t const
			in
	) {
		return
			CPPFileIO::mymin(
				in,
				static_cast <
					size_t
				> (4)
			)
		;
	}
	//
	class AE_MXNET {
    public:
        //
		using ImageArray =
			Tensors::Array::N2D_ARRAY <
				BatchSize							,
				(ImageResolution * ImageResolution)	,
				TYPE_DATA
			>
		; //
        using ResultLossType =
            Tensors::Array::ND_ARRAY
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
        ResultLossType *
            Result_Buffer
        ; //
        //
        Symbol
            MainNet
        ; //
        //
        std::map <
			std::string	,
			NDArray
        > args_map
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
		std::string const
		ParsFileName () {
			std::string
				ret (
					"./OUTS/PARS/NET_PARS_CNN_AE"
				)
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
            CPPFileIO::FileFD
                filefd (ParsFileName())
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
            CPPFileIO::FileFD
                filefd (ParsFileName())
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
                    buf[0]	,
                    bufsize
                ) ; //
                x.second
                    .SyncCopyFromCPU (
                        &(buf[0])	,
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
					Buffer->GET_DATA()	,
					ImageArray::SIZE ()
				)
			; //
			args_map["data"]
				.WaitToRead()
			; //
		}
		//
		inline void
		train () {
			ReadData		()		;
			exe->Forward	(true)	;
			exe->Backward	()		;
			UpdateParms		()		;
		}

		inline TYPE_DATA
		ValAccuracy () {

			ReadData () ;

			exe->Forward
				(false)
			;
			auto const &
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
					reinterpret_cast
						<ImageArray const &>
						(out_cpu.GetData()[0])
			; //
			printf("\nDEBUG ValAccuracy [0]: %e\n",output[0].L1_NORM());
			ImageArray const diff =
				Buffer[0] -
				output
			; //

            /* Evaluate the loss */ {
				if (
					!CPPFileIO::is_junked
						(Result_Buffer)
				) for (
					size_t i = 0				;
					i < Result_Buffer->SIZE()	;
					i++
				) {
					Result_Buffer[0][i] =
						std::sqrt (
							diff[i].L2_NORM()
						)
					; //
				}
			}

			return
				diff.L2_NORM() /
				static_cast<TYPE_DATA>
					(BatchSize)
			; //

		}

        inline void
        Train (
            size_t const
				index
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
            size_t tmp_Limit =
				Limit (Reader())
            ; //
            //
            for (
                size_t epoch = 0	;
                epoch < 2			;
                epoch++
            ) {
                for (
                    size_t i = 0	;
                    i < tmp_Limit	;
                    i++
                ) {
                    Buffer =
                        &(Reader(i))
                    ; //
                    train () ;
                }
                WriteParams () ;
            }
        }

		inline void
		Validate (
			size_t index
		) {

			CPPFileIO::set_junked
				(Result_Buffer)
			;

			char
				tmp[512]
			; /* Prepare the filename */ {
				sprintf (
					tmp,
					"./OUTS/QCD/TEST/%ld/image",
					index
				) ; //
			}

            std::string
                Filename (tmp)
            ; //

            MainImageReader
                Reader (Filename)
            ; //

            TYPE_DATA
                diff = 0
            ; //

            size_t tmp_Limit =
				Limit (Reader())
            ; //

            for (
                size_t i = 0 ;
                i < tmp_Limit ;
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
				static_cast<TYPE_DATA>
					(tmp_Limit)
			; //

            printf("Error = %e\n",diff);
        }

        inline void
        MakeLoss (
            std::string dirname
        ) {

            std::string imagename =
                dirname + "/image"
            ; //
            std::string lossname =
                dirname + "/loss_cnn_ae"
            ; //

            MainImageReader
                Reader (imagename)
            ; //
            CPPFileIO::FileVector
                <ResultLossType>
                    Writer (lossname)
            ; //

            TYPE_DATA
                diff = 0
            ; //

            size_t tmp_Limit =
				Limit (Reader())
            ; //

            ResultLossType
                tmpbuf
            ; //
            Result_Buffer =
                & tmpbuf
            ; //

            for (
                size_t i = 0 ;
                i < tmp_Limit ;
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
				static_cast<TYPE_DATA>
					(tmp_Limit)
			; //

            printf("Error = %e\n",diff);
        }

        inline void
        MakeLoss () {

			ReadParams () ;

            std::vector <std::string>
                filenames
            ; /* Prepare the list of filenames: */ {

				if (true) {
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
				}

				if (true) {
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
				}

				if(false){
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

            }

			for (
				size_t i=0;
				i<filenames.size();
				i++
			) {
				MakeLoss
					(filenames[i])
				; //
			}

        }
        //
		inline void
        GetNetSymbol1 () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
            size_t
                sizes[3] = {
                    ImageResolution * ImageResolution ,
                    24              * 24              ,
                    10              * 10
                }
            ; //

            ////////////////
            // Bind Data: //
            ////////////////
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

            //////////////
            // Layer 1: //
            //////////////
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

			//////////////
			// Layer 2: //
			//////////////
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

            //////////////
            // Layer 3: //
            //////////////
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

            //////////////
            // Layer 4: //
            //////////////
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

            //////////////
            // Layer 5: //
            //////////////
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

            ////////////////////////////////
            // SoftMaxing (Normalizing) : //
            ////////////////////////////////
            Symbol Out =
                softmax (
                    "softmaxout" ,
                    fc5 , 1
                )
            ; //

			///////////////////////////
            // Not Necessary for AR: //
            ///////////////////////////
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


            /////////////////////////////////////
            // Final Linear Regression output: //
            /////////////////////////////////////
            Symbol FinalNet =
                LinearRegressionOutput (
                    "Teacher" , Out , data ,
                    ImageResolution * ImageResolution
                )
            ; //

            /////////////////
            // Wrapping up //
            /////////////////
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
        GetNetSymbol2 () {

            ////////////////
            // Bind Data: //
            ////////////////
			Symbol data =
				Symbol::Variable
					("data")
			; //
			args_map["data"] =
				NDArray (
					Shape (
						BatchSize ,
						(
							ImageResolution *
							ImageResolution
						)
					) ,
					ctx_cpu , false
				)
			; //

			//////////////
			// Layer 0: //
			//////////////
			Symbol img0 =
				Reshape (
					"img0" , data		,
					Shape (
						BatchSize , 1	,
						ImageResolution	,
						ImageResolution
					)
				)
			; //

            //////////////
            // Layer 1: //
            //////////////
            Symbol
                conv1_w("conv1_w") ,
                conv1_b("conv1_b")
            ; //
            Symbol conv1 =
                Convolution (
                    "conv1" , img0    ,
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

			//////////////
			// Layer 2: //
			//////////////
            Symbol
                conv2_w("conv2_w") ,
                conv2_b("conv2_b")
            ; //
            Symbol conv2 =
                Convolution (
                    "conv2" , pool1   ,
                    conv2_w , conv2_b ,
                    Shape(5, 5) , 50
                )
            ; //
            Symbol ReLU2 =
                relu(conv2)
            ; //
            Symbol pool2 =
                Pooling (
                    "pool2", ReLU2, Shape(2, 2),
                    PoolingPoolType::kMax, false,
                    false, PoolingPoolingConvention::kValid,
                    Shape(2, 2)
                )
            ;

            //////////////
            // Layer 3: //
            //////////////
            Symbol flatten =
                Flatten(
                    "flatten",
                    pool2
                )
            ;

            //////////////
            // Layer 4: //
            //////////////
            Symbol
                fc4_w("fc4_w") ,
                fc4_b("fc4_b")
            ; //
            Symbol fc4 =
                FullyConnected (
                    "fc4" , flatten ,
                    fc4_w , fc4_b ,
                    100
                )
            ; //
            Symbol fa4 =
                relu ("fa4",fc4)
            ; //

            //////////////
            // Layer 5: //
            //////////////
            Symbol
                fc5_w("fc5_w") ,
                fc5_b("fc5_b")
            ; //
            Symbol fc5 =
                FullyConnected (
                    "fc5" , fa4   ,
                    fc5_w , fc5_b ,
                    1250
                )
            ; //
            Symbol fa5 =
                relu ("fa5",fc5)
            ; //

            //////////////
            // Layer 6: //
            //////////////
			Symbol img6 =
				Reshape (
					"img6", fa5,
					Shape (
						BatchSize,
						50, 5, 5
					)
				)
			;

			//////////////
            // Layer 7: //
            //////////////
			Symbol
				conv7_w("conv7_w") ,
				conv7_b("conv7_b")
			; //
			Symbol conv7 =
				Deconvolution (
					/* const std::string& symbol_name = */ "conv7" ,
					/* Symbol data = */ img6 ,
					/* Symbol weight = */ conv7_w ,
					/* Symbol bias = */ conv7_b ,
					/* Shape kernel = */ Shape(5,5) ,
					/* uint32_t num_filter = */ 50 ,
					/* Shape stride = */ Shape(2,2),
					/* Shape dilate = */ Shape(),
					/* Shape pad = */ Shape(),
					/* Shape adj = */ Shape(),
					/* Shape target_shape = */ Shape(),
					/* uint32_t num_group = */ 1,
					/* uint64_t workspace = */ 512,
					/* bool no_bias = */ false //,
					// DeconvolutionCudnnTune cudnn_tune = DeconvolutionCudnnTune::kNone,
					// bool cudnn_off = false,
					// DeconvolutionLayout layout = DeconvolutionLayout::kNone
				)
			; //
			Symbol ReLU7 =
				relu(conv7)
			; //

			//////////////
			// Layer 8: //
			//////////////
			Symbol
				conv8_w("conv8_w") ,
				conv8_b("conv8_b")
			; //
			Symbol conv8 =
				Deconvolution (
					/* const std::string& symbol_name = */ "conv8" ,
					/* Symbol data = */ ReLU7 ,
					/* Symbol weight = */ conv8_w ,
					/* Symbol bias = */ conv8_b ,
					/* Shape kernel = */ Shape(10,10) ,
					/* uint32_t num_filter = */ 50 ,
					/* Shape stride = */ Shape(2,2),
					/* Shape dilate = */ Shape(),
					/* Shape pad = */ Shape(),
					/* Shape adj = */ Shape(),
					/* Shape target_shape = */ Shape(),
					/* uint32_t num_group = */ 1,
					/* uint64_t workspace = */ 512,
					/* bool no_bias = */ false //,
					// DeconvolutionCudnnTune cudnn_tune = DeconvolutionCudnnTune::kNone,
					// bool cudnn_off = false,
					// DeconvolutionLayout layout = DeconvolutionLayout::kNone
				)
			; //
			Symbol ReLU8 =
				relu(conv8)
			; //

			//////////////
			// Layer 9: //
			//////////////
			Symbol
				conv9_w("conv9_w") ,
				conv9_b("conv9_b")
			; //
			Symbol conv9 =
				Deconvolution (
					/* const std::string& symbol_name = */ "conv9" ,
					/* Symbol data = */ ReLU8 ,
					/* Symbol weight = */ conv9_w ,
					/* Symbol bias = */ conv9_b ,
					/* Shape kernel = */ Shape(7,7) ,
					/* uint32_t num_filter = */ 1 ,
					/* Shape stride = */ Shape(1,1),
					/* Shape dilate = */ Shape(),
					/* Shape pad = */ Shape(),
					/* Shape adj = */ Shape(),
					/* Shape target_shape = */ Shape(),
					/* uint32_t num_group = */ 1,
					/* uint64_t workspace = */ 512,
					/* bool no_bias = */ false //,
					// DeconvolutionCudnnTune cudnn_tune = DeconvolutionCudnnTune::kNone,
					// bool cudnn_off = false,
					// DeconvolutionLayout layout = DeconvolutionLayout::kNone
				)
			; //
			Symbol ReLU9 =
				relu(conv9)
			; //

			///////////////
			// Layer 10: //
			///////////////
			Symbol flatten10 =
				Flatten(
					"flatten10",
					ReLU9
				)
			;


			/////////////////////////////////////
			// SoftMaxing (Normalizing) - 11 : //
			/////////////////////////////////////
			Symbol Out11 =
				softmax (
					"Out11"		,
					flatten10	,
					1
				)
			; //

            /////////////////////////////////////
            // Final Linear Regression output: //
            /////////////////////////////////////
			Symbol FinalNet =
				LinearRegressionOutput (
					"Teacher"	,
					Out11		,
					data		,
					ImageResolution * ImageResolution * BatchSize
				)
			; //

            /////////////////
            // Wrapping up //
            /////////////////
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
        GetNetSymbol3 () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
            size_t
                sizes[3] = {
                    ImageResolution * ImageResolution ,
                    24              * 24              ,
                    10              * 10
                }
            ; //

            ////////////////
            // Bind Data: //
            ////////////////
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

            ////////////////////
            // TMP JUNK TEST: //
            ////////////////////
            Symbol img0 =
				Reshape (
					"img0"	,	data	,
					Shape (
						BatchSize		,
						1				,
						ImageResolution	,
						ImageResolution
					)
				)
			; //

			Symbol img1 =
				Reshape (
					"img1"	,	img0	,
					Shape (
						BatchSize		,
							ImageResolution *
							ImageResolution
					)
				)
			; //

            //////////////
            // Layer 1: //
            //////////////
            Symbol
                fc1_w("fc1_w") ,
                fc1_b("fc1_b")
            ; //
            Symbol fc1 =
                FullyConnected (
                    "fc1" , img1  ,
                    fc1_w , fc1_b ,
                    sizes[1]
                )
            ; //
            Symbol fa1 =
                relu ("fa1",fc1)
            ;

			//////////////
			// Layer 2: //
			//////////////
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

            //////////////
            // Layer 3: //
            //////////////
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

            //////////////
            // Layer 4: //
            //////////////
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

            //////////////
            // Layer 5: //
            //////////////
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

            ////////////////////////////////
            // SoftMaxing (Normalizing) : //
            ////////////////////////////////
            Symbol Out =
                softmax (
                    "softmaxout" ,
                    fc5 , 1
                )
            ; //

			///////////////////////////
            // Not Necessary for AR: //
            ///////////////////////////
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


            /////////////////////////////////////
            // Final Linear Regression output: //
            /////////////////////////////////////
            Symbol FinalNet =
                LinearRegressionOutput (
                    "Teacher" , Out , data ,
                    ImageResolution * ImageResolution
                )
            ; //

            /////////////////
            // Wrapping up //
            /////////////////
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
        GetNetSymbol4 () {

			//////////////////////////
			// Structure of the AE: //
			//////////////////////////
			size_t constexpr
				sizes[3] = {
					ImageResolution	* ImageResolution ,
					24				* 24              ,
					10				* 10
				}
			; //
			//
            ////////////////
            // Bind Data: //
            ////////////////
			Symbol data =
				Symbol::Variable
					("data")
			; //
			args_map["data"] =
				NDArray (
					Shape (
						BatchSize	,
						sizes[0]
					) ,
					ctx_cpu , false
				)
			; //
            //
            ///////////////////////////////
            // SHAPE APPROPRIATE FOR CNN //
            ///////////////////////////////
			Symbol img0 =
				Reshape (
					"img0"	,	data	,
					Shape (
						BatchSize		,
						1				,
						ImageResolution	,
						ImageResolution
					)
				)
			; //

//			if (false) {
				////////////////////////
				// First Convolution: //
				////////////////////////
				Symbol
					conv1_w("conv1_w") ,
					conv1_b("conv1_b")
				; //
				Symbol img1 =
					Convolution (
						"img1"		,
						img0		,
						conv1_w		,
						conv1_b		,
						Shape(3, 3)	,
						1			,
						Shape(1, 1)	,
						Shape()		,
						Shape(1,1)
					)
				; //
//			}

			/////////////////////
			// FLATTEN IT BACK //
			/////////////////////
			Symbol flatten =
				Flatten(
					"flatten",
					img1
				)
			;


            /////////////////////////////////////
            // Final Linear Regression output: //
            /////////////////////////////////////
            Symbol FinalNet =
                LinearRegressionOutput (
                    "Teacher" , flatten , data ,
                    ImageResolution * ImageResolution
                )
            ; //

            /////////////////
            // Wrapping up //
            /////////////////
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
		GetNetSymbol () {
			GetNetSymbol4 () ;
		}
        //
        inline void
        AllocateMemory () {
			//
            /* Optimizer Part : */ {
                opt =
                    OptimizerRegistry::Find
                        ("adam")
                ; //
                opt
                    ->SetParam ( "lr"           , 0.0001    )
                    ->SetParam ( "beta1"        , 0.9       )
                    ->SetParam ( "beta2"        , 0.99      )
                    ->SetParam ( "epsilon"      , 0.0000001 )
                    ->SetParam ( "wd"           , 0.0       )
                    ->SetParam ( "rescale_grad" , 1.0       )
                ; //
            }
            //
		/* Executor part : */ {
				exe =
					MainNet
						.SimpleBind (
							ctx_cpu		,
							args_map
						)
				;
			}
            //
        }
        //
		inline void
		SetUpNet () {
			GetNetSymbol () ;
			MainNet
				.InferArgsMap (
					ctx_cpu		,
					& args_map	,
					args_map
				)
			; //
			Xavier
				xavier =
					Xavier (
						Xavier::gaussian	,
						Xavier::in			,
						2.34
					)
			; //
			for (
				auto & x :
					args_map
			) {
				xavier (
					x.first ,
					& x.second
				) ;
				LG << x.first ;
			}
			NDArray::WaitAll();
			AllocateMemory () ;
			DEBUG () ;
		}
        //
        inline void
        DEBUG () {
			return ;

			auto * weight = args_map["conv1_w"].GetData() ;
			auto * bias   = args_map["conv1_b"].GetData() ;

			printf("IN DEBUG: %e %e\n",weight[0],weight[1]);


		}
        //
		inline void
		FreeUpMemory () {
			/* Clean memory: */ {
				NDArray::WaitAll()	;
				delete exe			;
				delete opt			;
				NDArray::WaitAll()	;
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
		Start ()
		{ WriteParams () ; }
        //
        AE_MXNET() :
        ctx_cpu (
            Context (
                DeviceType::kCPU ,
                0
            )
        ) { SetUpNet () ; }
        //
        ~AE_MXNET(){
            FreeUpMemory();
            printf(
                "Done Destructor...\n"
            ) ; //
        }
        //
    } ;
	//
}

namespace PLOT {

	using namespace MISC ;

	// X = mass
	// Y = loss

	class D2Hist {
	public:
		std::string histname ;
		TH2F Hist ;
		MyHistN <4,false> hists ;

		static inline size_t const
		Loss2Bin (
			TYPE_DATA const
				loss
		) {
			TYPE_DATA constexpr
				a[4] =
					{ -0.0001, 0.03, 0.06, 0.10 }
			; //
			for(size_t i=0;i<=2;i++)
			if(
				(a[i]<=loss)&&
				(loss<a[i+1])
			) {
				return i ;
			}
			return 3 ;
		}

		inline void
		Fill (
			outvector4	const x ,
			float		const y
		) {
			Hist.Fill (x.m(),y) ;
			hists.Fill(x.m(),Loss2Bin(y));
		}

		inline void
		Fill (
			std::string const & x ,
			std::string const & y
		) {

			CPPFileIO::FullFileReader
				<outvector4>
					file_x (x)
			;

			CPPFileIO::FullFileReader
				<TYPE_DATA>
					file_y (y)
			;

			size_t const Limit =
				CPPFileIO::mymin
				(file_x(),file_y())
			;

			for (size_t i=0;i<Limit;i++) {
				Fill (
					file_x (i) ,
					file_y (i)
				) ;
			}

		}

		inline void
		Fill (
			std::vector <std::string> const & x ,
			std::vector <std::string> const & y
		) {

			size_t Limit =
				CPPFileIO::mymin
				(x.size(),y.size())
			;

			for(size_t i=0;i<Limit;i++)
			{ Fill (x[i],y[i]) ; }

		}

		D2Hist(std::string _histname):
		histname(_histname),
		Hist (
			&(_histname[0]) ,
			&(_histname[0]) ,
			8 , 0.0 , 800.1 ,
			7 , -0.001 , 0.6
		) ,
		hists (
			_histname+"1D",
			100,0.0,1000.0
		) {}

		~D2Hist(){

			TCanvas
				C
			; /* Draw the hist: */ {
				Hist.Draw("colz");

				std::string
					prefix (
						"./OUTS/GRAPHS/"
					)
				;

				std::string pdfname =
					prefix + histname + ".pdf"
				;

				C.SaveAs (&(pdfname[0])) ;

			}



		}
	} ;

	namespace NAME_LIST_FULL {

		inline void
		QCD_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TRAIN/0/vector");
			x.push_back("./OUTS/QCD/TRAIN/1/vector");
			x.push_back("./OUTS/QCD/TRAIN/2/vector");
			x.push_back("./OUTS/QCD/TRAIN/3/vector");
			x.push_back("./OUTS/QCD/TRAIN/4/vector");
			x.push_back("./OUTS/QCD/TRAIN/5/vector");
			x.push_back("./OUTS/QCD/TRAIN/6/vector");
			x.push_back("./OUTS/QCD/TRAIN/7/vector");
			y.push_back("./OUTS/QCD/TRAIN/0/loss");
			y.push_back("./OUTS/QCD/TRAIN/1/loss");
			y.push_back("./OUTS/QCD/TRAIN/2/loss");
			y.push_back("./OUTS/QCD/TRAIN/3/loss");
			y.push_back("./OUTS/QCD/TRAIN/4/loss");
			y.push_back("./OUTS/QCD/TRAIN/5/loss");
			y.push_back("./OUTS/QCD/TRAIN/6/loss");
			y.push_back("./OUTS/QCD/TRAIN/7/loss");
		}

		inline void
		QCD_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TEST/0/vector");
			x.push_back("./OUTS/QCD/TEST/1/vector");
			x.push_back("./OUTS/QCD/TEST/2/vector");
			x.push_back("./OUTS/QCD/TEST/3/vector");
			x.push_back("./OUTS/QCD/TEST/4/vector");
			x.push_back("./OUTS/QCD/TEST/5/vector");
			x.push_back("./OUTS/QCD/TEST/6/vector");
			x.push_back("./OUTS/QCD/TEST/7/vector");
			y.push_back("./OUTS/QCD/TEST/0/loss");
			y.push_back("./OUTS/QCD/TEST/1/loss");
			y.push_back("./OUTS/QCD/TEST/2/loss");
			y.push_back("./OUTS/QCD/TEST/3/loss");
			y.push_back("./OUTS/QCD/TEST/4/loss");
			y.push_back("./OUTS/QCD/TEST/5/loss");
			y.push_back("./OUTS/QCD/TEST/6/loss");
			y.push_back("./OUTS/QCD/TEST/7/loss");
		}

		inline void
		TOP_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TRAIN/0/vector");
			x.push_back("./OUTS/TOP/TRAIN/1/vector");
			x.push_back("./OUTS/TOP/TRAIN/2/vector");
			x.push_back("./OUTS/TOP/TRAIN/3/vector");
			x.push_back("./OUTS/TOP/TRAIN/4/vector");
			x.push_back("./OUTS/TOP/TRAIN/5/vector");
			x.push_back("./OUTS/TOP/TRAIN/6/vector");
			x.push_back("./OUTS/TOP/TRAIN/7/vector");
			y.push_back("./OUTS/TOP/TRAIN/0/loss");
			y.push_back("./OUTS/TOP/TRAIN/1/loss");
			y.push_back("./OUTS/TOP/TRAIN/2/loss");
			y.push_back("./OUTS/TOP/TRAIN/3/loss");
			y.push_back("./OUTS/TOP/TRAIN/4/loss");
			y.push_back("./OUTS/TOP/TRAIN/5/loss");
			y.push_back("./OUTS/TOP/TRAIN/6/loss");
			y.push_back("./OUTS/TOP/TRAIN/7/loss");
		}

		inline void
		TOP_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TEST/0/vector");
			x.push_back("./OUTS/TOP/TEST/1/vector");
			x.push_back("./OUTS/TOP/TEST/2/vector");
			x.push_back("./OUTS/TOP/TEST/3/vector");
			x.push_back("./OUTS/TOP/TEST/4/vector");
			x.push_back("./OUTS/TOP/TEST/5/vector");
			x.push_back("./OUTS/TOP/TEST/6/vector");
			x.push_back("./OUTS/TOP/TEST/7/vector");
			y.push_back("./OUTS/TOP/TEST/0/loss");
			y.push_back("./OUTS/TOP/TEST/1/loss");
			y.push_back("./OUTS/TOP/TEST/2/loss");
			y.push_back("./OUTS/TOP/TEST/3/loss");
			y.push_back("./OUTS/TOP/TEST/4/loss");
			y.push_back("./OUTS/TOP/TEST/5/loss");
			y.push_back("./OUTS/TOP/TEST/6/loss");
			y.push_back("./OUTS/TOP/TEST/7/loss");
		}

	}

	namespace NAME_LIST_NOLB {

		inline void
		QCD_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TRAIN/0/vector");
			x.push_back("./OUTS/QCD/TRAIN/1/vector");
			x.push_back("./OUTS/QCD/TRAIN/2/vector");
			x.push_back("./OUTS/QCD/TRAIN/3/vector");
			x.push_back("./OUTS/QCD/TRAIN/4/vector");
			x.push_back("./OUTS/QCD/TRAIN/5/vector");
			x.push_back("./OUTS/QCD/TRAIN/6/vector");
			x.push_back("./OUTS/QCD/TRAIN/7/vector");
			y.push_back("./OUTS/QCD/TRAIN/0/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/1/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/2/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/3/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/4/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/5/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/6/UnBoostedloss");
			y.push_back("./OUTS/QCD/TRAIN/7/UnBoostedloss");
		}

		inline void
		QCD_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TEST/0/vector");
			x.push_back("./OUTS/QCD/TEST/1/vector");
			x.push_back("./OUTS/QCD/TEST/2/vector");
			x.push_back("./OUTS/QCD/TEST/3/vector");
			x.push_back("./OUTS/QCD/TEST/4/vector");
			x.push_back("./OUTS/QCD/TEST/5/vector");
			x.push_back("./OUTS/QCD/TEST/6/vector");
			x.push_back("./OUTS/QCD/TEST/7/vector");
			y.push_back("./OUTS/QCD/TEST/0/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/1/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/2/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/3/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/4/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/5/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/6/UnBoostedloss");
			y.push_back("./OUTS/QCD/TEST/7/UnBoostedloss");
		}

		inline void
		TOP_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TRAIN/0/vector");
			x.push_back("./OUTS/TOP/TRAIN/1/vector");
			x.push_back("./OUTS/TOP/TRAIN/2/vector");
			x.push_back("./OUTS/TOP/TRAIN/3/vector");
			x.push_back("./OUTS/TOP/TRAIN/4/vector");
			x.push_back("./OUTS/TOP/TRAIN/5/vector");
			x.push_back("./OUTS/TOP/TRAIN/6/vector");
			x.push_back("./OUTS/TOP/TRAIN/7/vector");
			y.push_back("./OUTS/TOP/TRAIN/0/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/1/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/2/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/3/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/4/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/5/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/6/UnBoostedloss");
			y.push_back("./OUTS/TOP/TRAIN/7/UnBoostedloss");
		}

		inline void
		TOP_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TEST/0/vector");
			x.push_back("./OUTS/TOP/TEST/1/vector");
			x.push_back("./OUTS/TOP/TEST/2/vector");
			x.push_back("./OUTS/TOP/TEST/3/vector");
			x.push_back("./OUTS/TOP/TEST/4/vector");
			x.push_back("./OUTS/TOP/TEST/5/vector");
			x.push_back("./OUTS/TOP/TEST/6/vector");
			x.push_back("./OUTS/TOP/TEST/7/vector");
			y.push_back("./OUTS/TOP/TEST/0/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/1/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/2/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/3/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/4/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/5/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/6/UnBoostedloss");
			y.push_back("./OUTS/TOP/TEST/7/UnBoostedloss");
		}

	}

	namespace NAME_LIST_NOGS {
		inline void
		QCD_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TRAIN/0/vector");
			x.push_back("./OUTS/QCD/TRAIN/1/vector");
			x.push_back("./OUTS/QCD/TRAIN/2/vector");
			x.push_back("./OUTS/QCD/TRAIN/3/vector");
			x.push_back("./OUTS/QCD/TRAIN/4/vector");
			x.push_back("./OUTS/QCD/TRAIN/5/vector");
			x.push_back("./OUTS/QCD/TRAIN/6/vector");
			x.push_back("./OUTS/QCD/TRAIN/7/vector");
			y.push_back("./OUTS/QCD/TRAIN/0/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/1/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/2/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/3/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/4/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/5/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/6/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TRAIN/7/NoGramSchmidtImageTypeLoss");
		}
		inline void
		QCD_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/QCD/TEST/0/vector");
			x.push_back("./OUTS/QCD/TEST/1/vector");
			x.push_back("./OUTS/QCD/TEST/2/vector");
			x.push_back("./OUTS/QCD/TEST/3/vector");
			x.push_back("./OUTS/QCD/TEST/4/vector");
			x.push_back("./OUTS/QCD/TEST/5/vector");
			x.push_back("./OUTS/QCD/TEST/6/vector");
			x.push_back("./OUTS/QCD/TEST/7/vector");
			y.push_back("./OUTS/QCD/TEST/0/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/1/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/2/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/3/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/4/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/5/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/6/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/QCD/TEST/7/NoGramSchmidtImageTypeLoss");
		}
		inline void
		TOP_TRAIN_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TRAIN/0/vector");
			x.push_back("./OUTS/TOP/TRAIN/1/vector");
			x.push_back("./OUTS/TOP/TRAIN/2/vector");
			x.push_back("./OUTS/TOP/TRAIN/3/vector");
			x.push_back("./OUTS/TOP/TRAIN/4/vector");
			x.push_back("./OUTS/TOP/TRAIN/5/vector");
			x.push_back("./OUTS/TOP/TRAIN/6/vector");
			x.push_back("./OUTS/TOP/TRAIN/7/vector");
			y.push_back("./OUTS/TOP/TRAIN/0/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/1/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/2/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/3/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/4/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/5/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/6/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TRAIN/7/NoGramSchmidtImageTypeLoss");
		}
		inline void
		TOP_TEST_LIST (
			std::vector <std::string> & x ,
			std::vector <std::string> & y
		) {
			x.push_back("./OUTS/TOP/TEST/0/vector");
			x.push_back("./OUTS/TOP/TEST/1/vector");
			x.push_back("./OUTS/TOP/TEST/2/vector");
			x.push_back("./OUTS/TOP/TEST/3/vector");
			x.push_back("./OUTS/TOP/TEST/4/vector");
			x.push_back("./OUTS/TOP/TEST/5/vector");
			x.push_back("./OUTS/TOP/TEST/6/vector");
			x.push_back("./OUTS/TOP/TEST/7/vector");
			y.push_back("./OUTS/TOP/TEST/0/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/1/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/2/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/3/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/4/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/5/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/6/NoGramSchmidtImageTypeLoss");
			y.push_back("./OUTS/TOP/TEST/7/NoGramSchmidtImageTypeLoss");
		}
	}

	inline void
	FillQCD () {
		/* NORMAL */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_FULL::QCD_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TEST_2D") ;
				slave.Fill (x,y) ;

			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_FULL::QCD_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TRAIN_2D") ;
				slave.Fill (x,y) ;
			}
		}
		/* NOLB */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOLB::QCD_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TEST_2D_NOLB") ;
				slave.Fill (x,y) ;
			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOLB::QCD_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TRAIN_2D_NOLB") ;
				slave.Fill (x,y) ;
			}
		}
		/* NOGS */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOGS::QCD_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TEST_2D_NOGS") ;
				slave.Fill (x,y) ;
			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOGS::QCD_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_QCD_Corr_TRAIN_2D_NOGS") ;
				slave.Fill (x,y) ;
			}
		}
	}

	inline void
	FillTOP () {
		/* NORMAL */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_FULL::TOP_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TEST_2D") ;
				slave.Fill (x,y) ;
			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_FULL::TOP_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TRAIN_2D") ;
				slave.Fill (x,y) ;
			}
		}
		/* NOLB */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOLB::TOP_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TEST_2D_NOLB") ;
				slave.Fill (x,y) ;
			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOLB::TOP_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TRAIN_2D_NOLB") ;
				slave.Fill (x,y) ;
			}
		}
		/* NOGS */ {
			/* Test: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOGS::TOP_TEST_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TEST_2D_NOGS") ;
				slave.Fill (x,y) ;
			}
			/* Train: */ {
				std::vector
					<std::string>
					x , y
				;
				NAME_LIST_NOGS::TOP_TRAIN_LIST (x,y) ;
				D2Hist slave ("Mass_Loss_TOP_Corr_TRAIN_2D_NOGS") ;
				slave.Fill (x,y) ;
			}
		}
	}

}

namespace ROC {

	class ROOT_MAKER {
	public:

		TTree		*	TreeS		;
		TTree		*	TreeB		;
		TFile		*	file		;
		std::string		filename	;
		float			Mass, Loss	;
		float			Weight		;

		inline void
		Fill (
			MISC::outvector4	const		momentum	,
			float				const		loss		,
			TTree						*	tree
		) {
			Mass = momentum.m()	;
			Loss = loss			;
			tree->Fill()		;
		}

		inline void
		Fill (
			std::string		vectorname	,
			std::string		lossname	,
			TTree		*	tree
		) {

			CPPFileIO::FullFileReader
				<MISC::outvector4>
				readerV (vectorname)
			; //

			CPPFileIO::FullFileReader
				<MISC::TYPE_DATA>
				readerL (lossname)
			; //

			size_t limit =
				CPPFileIO::mymin
				(readerL(),readerV())
			; //

			for(size_t i=0;i<limit;i++){
				Fill (
					readerV(i),
					readerL(i),
					tree
				) ;
			}

		}

		inline void
		FillSignal (
			std::string	const	&	vectorname	,
			std::string	const	&	lossname
		) {
			Fill (
				vectorname	,
				lossname	,
				TreeS
			) ;
		}

		inline void
		FillBackground (
			std::string	const	&	vectorname	,
			std::string	const	&	lossname
		) {
			Fill (
				vectorname	,
				lossname	,
				TreeB
			) ;
		}

		inline void
		INIT () {
			/* Prepare the names: */ {
				std::string
					prefix (
						"./OUTS/TMVA/"
					)
				;
				std::string rootname =
					prefix		+
					filename	+
					".root"
				;
				/* Prepare the root file: */ {
					mkdir (
						&(prefix[0])	,
						0755
					) ;
					file =
						new TFile(
							&(rootname[0])	,
							"RECREATE"
						)
					;
				}
				/* Prepare the trees: */ {
					TreeS = new TTree ( "TreeS" , "TreeS" ) ;
					TreeB = new TTree ( "TreeB" , "TreeB" ) ;
					TreeS->Branch ( "Mass"		, & Mass	) ;
					TreeB->Branch ( "Mass"		, & Mass	) ;
					TreeS->Branch ( "Loss"		, & Loss	) ;
					TreeB->Branch ( "Loss"		, & Loss	) ;
					TreeS->Branch ( "Weight"	, & Weight	) ;
					TreeB->Branch ( "Weight"	, & Weight	) ;
				}
				Weight = 1.0 ;
			}
		}

		inline void
		CLEANUP () {
			TreeS->Write()	;
			TreeB->Write()	;
			delete	TreeS	;
			delete	TreeB	;
			delete	file	;
		}

		ROOT_MAKER(
			std::string _filename
		) : filename(_filename)
		{ INIT(); }

		~ROOT_MAKER()
		{ CLEANUP(); }

	} ;

	inline void
	MakeRoot_QCD_TOP_FULL () {
		/* FULL */ {
			ROOT_MAKER
				slave ("QCD_TOP_FULL")
			; /* Fill QCD: */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_FULL::QCD_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillBackground
						(x[i],y[i])
					; //
				}

			} /* Fill TOP */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_FULL::TOP_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillSignal
						(x[i],y[i])
					; //
				}

			}
		}
		/* NOLB */ {
			ROOT_MAKER
				slave ("QCD_TOP_NOLB")
			; /* Fill QCD: */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_NOLB::QCD_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillBackground
						(x[i],y[i])
					; //
				}

			} /* Fill TOP */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_NOLB::TOP_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillSignal
						(x[i],y[i])
					; //
				}

			}
		}
		/* NOGS */ {
			ROOT_MAKER
				slave ("QCD_TOP_NOGS")
			; /* Fill QCD: */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_NOGS::QCD_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillBackground
						(x[i],y[i])
					; //
				}

			} /* Fill TOP */ {

				std::vector
					<std::string>
						x , y
				; //

				PLOT::NAME_LIST_NOGS::TOP_TEST_LIST
					(x,y)
				;

				size_t const limit =
					CPPFileIO::mymin (
						x.size(),
						y.size()
					)
				; //

				for(size_t i=0;i<limit;i++){
					slave.FillSignal
						(x[i],y[i])
					; //
				}

			}
		}
	}

}
