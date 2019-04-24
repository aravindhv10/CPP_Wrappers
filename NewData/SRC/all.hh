#define USE_ALL
#include "pcg.hh"
#include "NewHEPHeaders10.hh"
#include "./HEPTopTagger/main.hh"
////////////////////////////////////////////////////////////////
namespace MISC {
    //
    size_t constexpr
        ImageResolution =
            40
    ;
    //
    using TYPE_DATA =
        float
    ; //
    //
    using imagetype =
        NewHEPHeaders::BoxImageGen <
            ImageResolution ,
            TYPE_DATA
        >
    ; //
    //
    using imagetypeflip =
        NewHEPHeaders::ImageGenFlip <
            ImageResolution ,
            TYPE_DATA
        >
    ; //
    //
    using outvector4 =
        NewHEPHeaders::VECTORS::lorentz4vector
            <TYPE_DATA>
    ; //
    //
    using TYPE_NSub =
        Tensors::NN::ND_ARRAY
            <5,TYPE_DATA>
    ;
    //
    using TYPE_PREDICT =
        Tensors::NN::ND_ARRAY
            <ImageResolution*ImageResolution,TYPE_DATA>
    ;
    //
    using vector4  =
        fastjet::PseudoJet
    ; //
    //
    using vector4s =
        std::vector
            <fastjet::PseudoJet>
    ; //
    //
    inline void
    Executor (
        std::string
            ProgramName
    ) {
        CPPFileIO::ForkMe forker ;
        if(forker.InKid()){
            std::vector
                <std::string>
                    program
            ; //
            program.push_back
                (ProgramName)
            ; //
            CPPFileIO::starter_self
                (program)
            ; //
        }
    }
    //
    class MyPythia :
    public Pythia8::Pythia {
    public:
        //
        inline long
        FindParticle (
            long pid
        ) {
            long ret = 0 ;
            for (
                long i = 0 ;
                (i<this->event.size()) && (ret==0) ;
                i++
            ) if (
                this->event[i].id() == pid
            ) { ret = i ; }
            return ret ;
        }
        //
        inline long
        FindParticleMod (
            long pid
        ) {
            long ret = 0 ;
            pid = CPPFileIO::mymod (pid) ;
            for (
                long i=0;
                (i<this->event.size())&&(ret==0);
                i++
            ) if (
                CPPFileIO::mymod(
                    this->event[i].id()
                ) == pid
            ) { ret = i ; }
            return ret ;
        }
        //
        inline long
        Recurse (
            long index
        ) {
            auto ppid =
                this->event[index].id ()
            ; //
            long dt[2] = {
                    this->event[index]
                        .daughter1 () ,
                    this->event[index]
                        .daughter2 ()
                }
            ; //
            long dpid[2] = {
                    this->event[dt[0]]
                        .id () ,
                    this->event[dt[1]]
                        .id ()
                }
            ; //
            if (dpid[0]==ppid) {
                return
                    Recurse (dt[0])
                ;
            }
            else if (dpid[1]==ppid) {
                return
                    Recurse (dt[1])
                ;
            }
            else {
                return
                    index
                ;
            }
        }
        //
        inline long
        FindDaughter (
            long ParentID ,
            long DPID
        ) {
            ParentID =
                Recurse (ParentID)
            ; //
            long dt[2] = {
                    this->event[ParentID]
                        .daughter1() ,
                    this->event[ParentID]
                        .daughter2()
                }
            ; //
            long dpid[2] = {
                    this->event[dt[0]]
                        .id() ,
                    this->event[dt[1]]
                        .id()
                }
            ; //
            if (dpid[0]==DPID) {
                return
                    dt[0]
                ;
            }
            else if (dpid[1]==DPID) {
                return
                    dt[1]
                ;
            }
            else {
                return
                    0
                ;
            }
        }
        //
        inline vector4
        GetVector (
            long id
        ) {
            return
                vector4 (
                    this->event[id].px () ,
                    this->event[id].py () ,
                    this->event[id].pz () ,
                    this->event[id].e  ()
                )
            ;
        }
        //
    } ;
    //
    class EventWriter {
    public:
        //
        static inline TYPE_NSub
        EvalNSub (
            vector4 const &
                injet
        ) {
            constexpr double
                beta = 2.0
            ; //
            using namespace std              ;
            using namespace fastjet          ;
            using namespace fastjet::contrib ;
            TYPE_NSub ret ;
            /* TAU 1 */ {
                Nsubjettiness
                    nSub1_beta2 (
                        1 ,
                        OnePass_WTA_KT_Axes() ,
                        UnnormalizedMeasure(beta)
                    )
                ; //
                ret[0] =
                    nSub1_beta2
                        (injet)
                ; //
            }
            /* TAU 2 */ {
                Nsubjettiness
                    nSub2_beta2 (
                        2 ,
                        OnePass_WTA_KT_Axes() ,
                        UnnormalizedMeasure(beta)
                    )
                ; //
                ret[1] =
                    nSub2_beta2
                        (injet)
                ; //
            }
            /* TAU 3 */ {
                Nsubjettiness
                    nSub3_beta2 (
                        3 ,
                        OnePass_WTA_KT_Axes() ,
                        UnnormalizedMeasure(beta)
                    )
                ; //
                ret[2] =
                    nSub3_beta2
                        (injet)
                ; //
            }
            /* TAU 4 */ {
                Nsubjettiness
                    nSub4_beta2 (
                        4 ,
                        OnePass_WTA_KT_Axes() ,
                        UnnormalizedMeasure(beta)
                    )
                ; //
                ret[3] =
                    nSub4_beta2
                        (injet)
                ; //
            }
            /* TAU 5 */ {
                Nsubjettiness
                    nSub5_beta2 (
                        5 ,
                        OnePass_WTA_KT_Axes() ,
                        UnnormalizedMeasure(beta)
                    )
                ; //
                ret[4] =
                    nSub5_beta2
                        (injet)
                ; //
            }
            return ret ;
        }
        //
        static inline bool
        CheckTopTag (
            vector4s const &
                particles
        ) {
            fastjet::JetAlgorithm algo =
                fastjet::cambridge_algorithm
            ; //
            fastjet::JetDefinition
                jet_def (
                    algo,
                    100.0
                )
            ;
            fastjet::ClusterSequence
                clust_seq (
                    particles,
                    jet_def
                )
            ; //
            vector4s jets =
                clust_seq
                .exclusive_jets(1)
            ; //
            HEPTopTagger::HEPTopTagger
                tagger (jets[0])
            ; /* Prepare and run the tagger: */ {
                tagger.set_max_subjet_mass (30.0) ;
                tagger.set_mass_drop_threshold (0.8) ;
                tagger.set_filtering_R (0.3) ;
                tagger.set_filtering_n (5) ;
                tagger.set_filtering_minpt_subjet (30.0) ;
                tagger.set_mode (HEPTopTagger::TWO_STEP_FILTER) ;
                tagger.set_top_minpt (200) ;
                tagger.set_top_mass_range (150.0,200.0) ;
                tagger.set_fw (0.15) ;
                tagger.run();
            }
            return tagger.is_tagged() ;
        }
        //
        inline void
        Eval (
            vector4 const &
                injet
        ) {
            /* Evaluate NSubjettiness  : */ {
                auto tmptau =
                    EvalNSub(injet)
                ;
                NSubWriter.push_back
                    (tmptau)
                ; //
            }
            /* Evaluate Normal Image   : */ {
                imagetype img (
                    injet.constituents() ,
                    0.5
                ) ; //
                ImageWriter.push_back
                    (img)
                ; //
            }
            /* Write Top Tagged bool   : */ {
                TopTagWriter.push_back(
                    CheckTopTag (
                        injet.constituents()
                    )
                ) ; //
            }
            /* Write actual jet vector : */ {
                outvector4 tmp (
                    injet
                ) ;
                VectorWriter.push_back
                    (tmp)
                ; //
            }
        }
        //
        inline void
        operator () (
            vector4 const &
                injet
        ) { Eval (injet) ; }
        //
        std::string prefixname ;
        //
        CPPFileIO::FileVector < imagetype >
            ImageWriter
        ; //
        CPPFileIO::FileVector < outvector4 >
            VectorWriter
        ; //
        CPPFileIO::FileVector < bool >
            TopTagWriter
        ; //
        CPPFileIO::FileVector < TYPE_NSub >
            NSubWriter
        ; //
        //
        EventWriter     ( std::string _prefixname   ) :
        prefixname      ( _prefixname               ) ,
        ImageWriter     ( prefixname + "/image"     ) ,
        VectorWriter    ( prefixname + "/vector"    ) ,
        TopTagWriter    ( prefixname + "/toptag"    ) ,
        NSubWriter      ( prefixname + "/nsub"      ) {}
        //
        ~EventWriter(){}
        //
    } ;
    //
    template <
        size_t Num,
        bool compare=false
    > class MyHistN {
    private:
        std::string   histname         ;
        std::string   histNnames [Num] ;
        TH1F        * Hists      [Num] ;
        bool logscale ;
    public:
        template <size_t Ii> inline void Fill (double a) { if (a>-90.0) {Hists[Ii]->Fill(a);} }
        inline void NormalizeHist (TH1F * inhist) { inhist->Scale(1.0/inhist->Integral()); inhist->SetLineWidth(3); }
        inline void DeleteAll () { for (size_t ii=0;ii<Num;ii++) { delete Hists[ii] ; } }
        inline void Write () {
            //printf("DEBUG: Called Write();");
            TCanvas C ; if(logscale){C.SetLogy();}
            Style_t styles[Num] ; /* Set the line style. */ {for(size_t i=0;i<Num;i++){styles[i]=kSolid;}}
            std::vector <int> Colors ; /* Decide Colors: */ {
                if ((Num==6)&&compare) {
                    styles[0] = kSolid  ;
                    styles[1] = kSolid  ;
                    styles[2] = kSolid  ;
                    styles[3] = kDashed ;
                    styles[4] = kDashed ;
                    styles[5] = kDashed ;
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#009900"));
                    Colors.push_back(TColor::GetColor("#000099"));
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#009900"));
                    Colors.push_back(TColor::GetColor("#000099"));
                } else if (Num==3) {
                    styles[0] = kSolid  ;
                    styles[1] = kSolid  ;
                    styles[2] = kSolid  ;
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#009900"));
                    Colors.push_back(TColor::GetColor("#000099"));
                } else if ((Num==4)&&compare) {
                    styles[0] = kSolid  ;
                    styles[1] = kSolid  ;
                    styles[2] = kDashed ;
                    styles[3] = kDashed ;
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#000099"));
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#000099"));
                } else if (Num==2) {
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#000099"));
                } else {
                    Colors.push_back(TColor::GetColor("#990000"));
                    Colors.push_back(TColor::GetColor("#009900"));
                    Colors.push_back(TColor::GetColor("#000099"));
                    Colors.push_back(TColor::GetColor("#999900"));
                    Colors.push_back(TColor::GetColor("#990099"));
                    Colors.push_back(TColor::GetColor("#009999"));
                    Colors.push_back(TColor::GetColor("#000000"));
                }
            }
            double YMax = 0 ;
            for(size_t i=0;i<Num;i++){
                NormalizeHist(Hists[i]);
                int    binmax = Hists[i]->GetMaximumBin ()            ;
                double TmpMax = Hists[i]->GetBinContent (binmax)      ;
                YMax          = CPPFileIO::mymax        (YMax,TmpMax) ;
            }
            for(size_t i=0;i<Num;i++){
                Hists[i]->SetMaximum   ( YMax        ) ;
                Hists[i]->SetLineColor ( Colors[i]   ) ;
                Hists[i]->SetLineStyle ( styles[i]   ) ;
                Hists[i]->Draw         ( "hist same" ) ;
            }
            mkdir ( "./OUTS/"        , 0755 ) ;
            mkdir ( "./OUTS/GRAPHS/" , 0755 ) ;
            std::string outfilename ("./OUTS/GRAPHS/") ;
            std::string outname  = outfilename + histname + ".pdf" ;
            std::string outnameC = outfilename + histname + ".C"   ;
            C.SaveAs ( & ( outname[0]  ) ) ;
            C.SaveAs ( & ( outnameC[0] ) ) ;
            DeleteAll () ;
        }
        inline void AllocateAll (size_t nbins, double min, double max) {
            for(size_t ii=0;ii<Num;ii++){
                char tmp[1024];
                sprintf(tmp,"%s_%ld",&(histname[0]),ii);
                histNnames[ii] = std::string(tmp);
                Hists[ii] = new TH1F ( & ( histNnames[ii][0] ) , & ( histname [0] ) , nbins , min , max ) ;
            }
        }
        inline void SetLogScale (bool _logscale) {logscale=_logscale;}
        MyHistN  (std::string _histname, size_t nbins, double min, double max) : histname(_histname)
        { AllocateAll (nbins,min,max) ; logscale=false; }
        ~MyHistN () { Write () ; }
    };
    //
    class Plot2D {
    public:
        //
        using FileReader =
            CPPFileIO::FullFileReader
                <imagetype>
        ; //
        //
        std::string histname ;
        //
        TH2F Hist ;
        //
        inline void
        operator () (
            std::string filename
        ) {
            FileReader reader (filename) ;
            for (size_t i=0;i<reader();i++)
            for (size_t y=0;y<ImageResolution;y++)
            for (size_t x=0;x<ImageResolution;x++) {
                Hist.Fill (
                    x , y ,
                    reader(i)
                        .Image[y][x]
                ) ; //
            }
        }
        //
        Plot2D (
            std::string filename
        ) :
        Hist (
            &(filename[0]) ,
            &(filename[0]) ,
            ImageResolution , 0 , ImageResolution ,
            ImageResolution , 0 , ImageResolution
        ) { histname = filename ; }
        //
        ~Plot2D(){
            mkdir("./OUTS/",0755);
            mkdir("./OUTS/GRAPHS/",0755);
            std::string outfilename("./OUTS/GRAPHS/");
            outfilename =
                outfilename +
                histname +
                ".pdf"
            ; //
            TCanvas C;
            Hist.Draw("colz");
            C.SaveAs(
                &(outfilename[0])
            );
        }
        //
    } ;
    //
    class ROC_ELEMENT {
    public:
        //
        TYPE_DATA value ;
        bool signal ;
        //
        ROC_ELEMENT (
            TYPE_DATA _value = 0.0 ,
            bool _signal = false
        ) {
            value  = _value  ;
            signal = _signal ;
        }
        //
        ~ROC_ELEMENT(){}
        //
        inline bool
        operator > (
            ROC_ELEMENT const &
                other
        ) {
            return
                value > other.value
            ; //
        }
        //
        inline bool
        operator < (
            ROC_ELEMENT const &
                other
        ) {
            return
                value < other.value
            ; //
        }
        //
    } ;
    //
    using ROC_ELEMENTs =
        std::vector
            <ROC_ELEMENT>
    ; //
    //
    inline void
    WriteROC (
        std::vector <ROC_ELEMENT> &
            invals ,
        std::string filename
    ) {
        std::sort (
            invals.begin () ,
            invals.end   ()
        ) ; //
        size_t TotalSig = 0 ;
        size_t TotalBkg = 0 ;
        for(size_t i=0;i<invals.size();i++){
            if(invals[i].signal){TotalSig++;}
            else{TotalBkg++;}
        }
        size_t CurrentSig = 0 ;
        size_t CurrentBkg = 0 ;
        FILE *f =
            fopen (
                &(filename[0]),
                "w"
            )
        ; /* Write to the file: */ {
            for(size_t i=0;i<invals.size();i++){
                if(invals[i].signal){CurrentSig++;}
                else{CurrentBkg++;}
                TYPE_DATA EpsS =
                    ((TYPE_DATA)CurrentSig) /
                    ((TYPE_DATA)TotalSig)
                ; //
                TYPE_DATA EpsB =
                    ((TYPE_DATA)CurrentBkg) /
                    ((TYPE_DATA)TotalBkg)
                ; //
                fprintf(f,"%e %e\n",EpsS,1.0/EpsB);
            }
        } fclose(f) ;
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP1_GENERATION {
    //
    using namespace MISC ;
    //
    inline void
    Generate_QCD (
        size_t      index                ,
        std::string MidName              ,
        size_t      random_seed = 123    ,
        size_t      N_Events    = 100000
    ) {
        std::string prefixname ;
        /* Make the directories: */ {
            std::string dirname = "./OUTS/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/QCD/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/QCD/" + MidName + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            char tmp = '0' ;
            tmp = tmp + ( (char) index ) ;
            dirname = dirname + tmp + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            prefixname = dirname ;
        }
        size_t count = 0 ;
        EventWriter
            writer (
                prefixname
            )
        ; //
        MyPythia pythia ;
        /* Configure pythia: */ {
            pythia.readString ( "Beams:eCM = 13000"         ) ;
            pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
            pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
            pythia.readString ( "PartonLevel:MPI = off"     ) ;
            pythia.readString ( "Random:setSeed = on"       ) ;
            /* Set the random number Seed */ {
                char tmp[256] ;
                sprintf (
                    tmp                 ,
                    "Random:seed = %ld" ,
                    index + random_seed
                ) ; //
                pythia.readString
                    (tmp)
                ; //
            }
            pythia.readString ( "HardQCD:all = on" ) ;
            pythia.init () ;
        }
        while (
            count<N_Events
        ) if (
            pythia.next()
        ) {
            vector4s hadrons ;
            /* Prepare the list of hadrons: */ {
                for (
                    size_t i=0;
                    i<pythia.event.size();
                    i++
                ) if (
                    pythia.event[i]
                        .isFinal()
                ) {
                    int pid =
                        pythia
                        .event[i]
                        .id()
                    ; //
                    if (pid<0) {pid=-pid;}
                    if (
                        ( pid != 12 ) &&
                        ( pid != 14 ) &&
                        ( pid != 16 )
                    ) {
                        vector4 tmp (
                            pythia.event[i].px() ,
                            pythia.event[i].py() ,
                            pythia.event[i].pz() ,
                            pythia.event[i].e()
                        ) ;
                        auto tmpeta =
                            tmp.rapidity()
                        ; //
                        if (
                            ( -2.5     < tmpeta ) &&
                            ( tmpeta   < 2.5    ) &&
                            ( tmp.pt() > 0.5    )
                        ) {
                            hadrons.push_back
                                (tmp)
                            ; //
                        }
                    }
                }
            }
            fastjet::JetAlgorithm algo =
                fastjet::antikt_algorithm
            ; //
            fastjet::JetDefinition
                jet_def (
                    algo,
                    1.0
                )
            ; //
            fastjet::ClusterSequence
                clust_seq (
                    hadrons,
                    jet_def
                )
            ; //
            vector4s jets =
                clust_seq
                .inclusive_jets
                    (800.0)
            ; //
            if (
                (jets.size()>0) &&
                (jets[0].constituents().size()>2)
            ) {
                auto tmppt =
                    jets[0].pt ()
                ; //
                auto tmpet =
                    jets[0].rapidity ()
                ; //
                if (
                    (
                        ( 800 < tmppt ) &&
                        ( tmppt < 900 )
                    ) && (
                        ( -2.5 < tmpet ) &&
                        ( tmpet < 2.5 )
                    )
                ) {
                    writer(jets[0]);
                    count++ ;
                }
            }
        }
    }
    //
    inline void
    Generate_Train () {
        CPPFileIO::ForkMe forker ;
        for(size_t i=0;i<8;i++)
        if(forker.InKid()){
            Generate_QCD (
                i       ,
                "TRAIN" ,
                123     ,
                300000
            ) ; //
        }
    }
    //
    inline void
    Generate_Test () {
        CPPFileIO::ForkMe forker ;
        for(size_t i=0;i<8;i++)
        if(forker.InKid()){
            Generate_QCD (
                i      ,
                "TEST" ,
                456    ,
                50000
            ) ; //
        }
    }
    //
    inline void
    PlotImages () {
        //
        /* TRAIN */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("QCD_TRAIN_IMAGES");
                tmp("./OUTS/QCD/TRAIN/0/image");
                tmp("./OUTS/QCD/TRAIN/1/image");
                tmp("./OUTS/QCD/TRAIN/2/image");
                tmp("./OUTS/QCD/TRAIN/3/image");
                tmp("./OUTS/QCD/TRAIN/4/image");
                tmp("./OUTS/QCD/TRAIN/5/image");
                tmp("./OUTS/QCD/TRAIN/6/image");
                tmp("./OUTS/QCD/TRAIN/7/image");
            }

        }
        //
        /* TEST */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("QCD_TEST_IMAGES");
                tmp("./OUTS/QCD/TEST/0/image");
                tmp("./OUTS/QCD/TEST/1/image");
                tmp("./OUTS/QCD/TEST/2/image");
                tmp("./OUTS/QCD/TEST/3/image");
                tmp("./OUTS/QCD/TEST/4/image");
                tmp("./OUTS/QCD/TEST/5/image");
                tmp("./OUTS/QCD/TEST/6/image");
                tmp("./OUTS/QCD/TEST/7/image");
            }
        }
        //
    }
    //
    inline void
    PlotNsub () {
        //
        /* TRAIN */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_QCD_TRAIN",100,
                    -0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/QCD/TRAIN/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (
                    size_t i=0;
                    i<Reader();
                    i++
                ) {
                    //
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                    //
                }
            }
        }
        //
        /* TEST */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_QCD_TEST",100,
                    -0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/QCD/TEST/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (
                    size_t i=0;
                    i<Reader();
                    i++
                ) {
                    //
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                    //
                }
            }
        }
        //
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP2_GENERATEWBS {
    //
    using namespace MISC ;
    //
    inline void
    Generate_WBS (
        size_t      index                ,
        std::string MidName              ,
        size_t      random_seed = 123    ,
        size_t      N_Events    = 100000
    ) {
        std::string prefixname ;
        /* Make the directories: */ {
            std::string dirname = "./OUTS/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/WBS/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/WBS/" + MidName + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            char tmp = '0' ;
            tmp = tmp + ( (char) index ) ;
            dirname = dirname + tmp + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            prefixname = dirname ;
        }
        size_t count = 0 ;
        EventWriter writer (prefixname) ;
        MyPythia pythia ; /* Configure pythia: */ {
            pythia.readString ( "Beams:eCM = 13000"         ) ;
            pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
            pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
            pythia.readString ( "PartonLevel:MPI = off"     ) ;
            pythia.readString ( "Random:setSeed = on"       ) ;
            /* Set the random number Seed */ {
                char tmp[256] ;
                sprintf (
                    tmp                 ,
                    "Random:seed = %ld" ,
                    index + random_seed
                ) ; //
                pythia.readString
                    (tmp)
                ; //
            }
            pythia.readString ( "WeakDoubleBoson:ffbar2ZW = on" ) ;
            pythia.readString ( "23:onMode = off"               ) ;
            pythia.readString ( "23:onIfAny = 12"               ) ;
            pythia.readString ( "24:onMode = off"               ) ;
            pythia.readString ( "24:onIfAny = 1 2 3 4"          ) ;
            pythia.init () ;
        }
        while (count<N_Events)
        if (pythia.next()) {
            vector4s hadrons ;
            /* Prepare the list of hadrons: */ {
                for (
                    size_t i=0;
                    i<pythia.event.size();
                    i++
                ) if (
                    pythia.event[i]
                        .isFinal()
                ) {
                    int pid =
                        pythia
                        .event[i]
                        .id()
                    ;
                    if (pid<0) {pid=-pid;}
                    if (
                        ( pid != 12 ) &&
                        ( pid != 14 ) &&
                        ( pid != 16 )
                    ) {
                        vector4 tmp (
                            pythia.event[i].px () ,
                            pythia.event[i].py () ,
                            pythia.event[i].pz () ,
                            pythia.event[i].e  ()
                        ) ;
                        auto tmpeta =
                            tmp.rapidity()
                        ;
                        if (
                            ( -2.5     < tmpeta ) &&
                            ( tmpeta   < 2.5    ) &&
                            ( tmp.pt() > 0.5    )
                        ) {
                            hadrons.push_back
                                (tmp)
                            ; //
                        }
                    }
                }
            }
            //
            vector4 WBS , JT[2] ;
            /* Get the GEN Vectors: */ {
                long wid =
                    pythia.Recurse (
                        pythia.FindParticleMod
                            (24)
                    ) ;
                WBS = pythia.GetVector(wid);
                long j1id[2] = {
                    pythia.event[wid]
                        .daughter1 () ,
                    pythia.event[wid]
                        .daughter1 ()
                } ;
                JT[0] =
                    pythia
                    .GetVector
                        (j1id[0])
                ;
                JT[1] =
                    pythia
                    .GetVector
                        (j1id[1])
                ;
            }
            //
            fastjet::JetAlgorithm algo =
                fastjet::antikt_algorithm
            ; //
            fastjet::JetDefinition
                jet_def (
                    algo,
                    1.0
                )
            ;
            fastjet::ClusterSequence
                clust_seq (
                    hadrons,
                    jet_def
                )
            ;
            vector4s jets =
                clust_seq
                .inclusive_jets
                        (800.0)
            ;
            //
            if (jets.size()>0) {
                //
                bool genmatched = (
                    ( jets[0].delta_R (   WBS ) < 0.6 ) &&
                    ( jets[0].delta_R ( JT[0] ) < 0.6 ) &&
                    ( jets[0].delta_R ( JT[1] ) < 0.6 )
                ) ;
                //
                if (
                    (genmatched) &&
                    (jets[0].constituents().size()>2)
                ) {
                    auto tmppt =
                        jets[0]
                        .pt()
                    ; //
                    auto tmpet =
                        jets[0]
                        .rapidity()
                    ; //
                    if (
                        (
                            ( 800 < tmppt ) &&
                            ( tmppt < 900 )
                        ) && (
                            ( -2.5 < tmpet ) &&
                            ( tmpet < 2.5 )
                        )
                    ) {
                        writer(jets[0]);
                        count++ ;
                    }
                }
            }
        }
    }
    //
    inline void
    Generate_Train_WBS () {
        CPPFileIO::ForkMe forker ;
        for (size_t i=0;i<8;i++)
        if (forker.InKid()) {
            Generate_WBS (
                i       ,
                "TRAIN" ,
                123     ,
                300000
            ) ; //
        }
    }
    //
    inline void
    Generate_Test_WBS () {
        CPPFileIO::ForkMe forker ;
        for (size_t i=0;i<8;i++)
        if (forker.InKid()) {
            Generate_WBS (
                i      ,
                "TEST" ,
                456    ,
                50000
            ) ; //
        }
    }
    //
    inline void
    PlotImages () {
        //
        /* TRAIN */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("WBS_TRAIN_IMAGES");
                tmp("./OUTS/WBS/TRAIN/0/image");
                tmp("./OUTS/WBS/TRAIN/1/image");
                tmp("./OUTS/WBS/TRAIN/2/image");
                tmp("./OUTS/WBS/TRAIN/3/image");
                tmp("./OUTS/WBS/TRAIN/4/image");
                tmp("./OUTS/WBS/TRAIN/5/image");
                tmp("./OUTS/WBS/TRAIN/6/image");
                tmp("./OUTS/WBS/TRAIN/7/image");
            }
        }
        //
        /* TEST */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("WBS_TEST_IMAGES");
                tmp("./OUTS/WBS/TEST/0/image");
                tmp("./OUTS/WBS/TEST/1/image");
                tmp("./OUTS/WBS/TEST/2/image");
                tmp("./OUTS/WBS/TEST/3/image");
                tmp("./OUTS/WBS/TEST/4/image");
                tmp("./OUTS/WBS/TEST/5/image");
                tmp("./OUTS/WBS/TEST/6/image");
                tmp("./OUTS/WBS/TEST/7/image");
            }
        }
        //
    }
    //
    inline void
    PlotNsub () {
        //
        /* TRAIN */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_WBS_TRAIN",100,
                    -0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/WBS/TRAIN/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (
                    size_t i=0;
                    i<Reader();
                    i++
                ) {
                    //
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                    //
                }
            }
        }
        //
        /* TEST */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_WBS_TEST",100,
                    -0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/WBS/TEST/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (
                    size_t i=0;
                    i<Reader();
                    i++
                ) {
                    //
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                    //
                }
            }
        }
        //
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP3_GENERATETOP {
    //
    using namespace MISC ;
    //
    inline void
    Generate_TOP (
        size_t      index                ,
        std::string MidName              ,
        size_t      random_seed = 123    ,
        size_t      N_Events    = 100000
    ) {
        std::string prefixname ;
        /* Make the directories: */ {
            std::string dirname = "./OUTS/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/TOP/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            dirname = "./OUTS/TOP/" + MidName + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            char tmp = '0' ;
            tmp = tmp + ( (char) index ) ;
            dirname = dirname + tmp + "/" ;
            mkdir ( &(dirname[0]) , 0755 ) ;
            prefixname = dirname ;
        }
        size_t count = 0 ;
        EventWriter writer (prefixname) ;
        MyPythia pythia ;
        /* Configure pythia: */ {
            pythia.readString ( "Beams:eCM = 13000"         ) ;
            pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
            pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
            pythia.readString ( "PartonLevel:MPI = off"     ) ;
            pythia.readString ( "Random:setSeed = on"       ) ;
            /* Set the random number Seed */ {
                char tmp[256] ;
                sprintf (
                    tmp                 ,
                    "Random:seed = %ld" ,
                    index + random_seed
                ) ; //
                pythia.readString
                    (tmp)
                ; //
            }
            pythia.readString ( "Top:gg2ttbar = on"    ) ;
            pythia.readString ( "24:onMode = off"      ) ;
            pythia.readString ( "24:onIfAny = 1 2 3 4" ) ;
            pythia.init () ;
        }
        while (count<N_Events)
        if (pythia.next()) {
            vector4s hadrons ;
            /* Prepare the list of hadrons: */ {
                for (
                    size_t i=0;
                    i<pythia.event.size();
                    i++
                ) if (
                    pythia.event[i]
                        .isFinal()
                ) {
                    int pid =
                        pythia
                        .event[i]
                        .id()
                    ;
                    if (pid<0) {pid=-pid;}
                    if (
                        ( pid != 12 ) &&
                        ( pid != 14 ) &&
                        ( pid != 16 )
                    ) {
                        vector4 tmp (
                            pythia.event[i].px () ,
                            pythia.event[i].py () ,
                            pythia.event[i].pz () ,
                            pythia.event[i].e  ()
                        ) ;
                        auto tmpeta =
                            tmp.rapidity()
                        ;
                        if (
                            ( -2.5     < tmpeta ) &&
                            ( tmpeta   < 2.5    ) &&
                            ( tmp.pt() > 0.5    )
                        ) {
                            hadrons.push_back
                                (tmp)
                            ; //
                        }
                    }
                }
            }
            //
            vector4
                TOP[2] , WBS[2] , BOT[2] ,
                JT1[2] , JT2[2]
            ; /* Get the GEN Vectors: */ {
                long tid[2] = {
                    pythia.Recurse (
                        pythia.FindParticle
                            (+6)
                    ) ,
                    pythia.Recurse (
                        pythia.FindParticle
                            (-6)
                    )
                } ;
                TOP[0] =
                    pythia
                    .GetVector
                        (tid[0])
                ; //
                TOP[1] =
                    pythia
                    .GetVector
                        (tid[1])
                ; //
                long wid[2] = {
                    pythia.Recurse (
                        pythia.FindDaughter (
                            (long) /* ParentID = */ tid[0] ,
                            (long) /* DPID     = */ +24
                        )
                    ) ,
                    pythia.Recurse (
                        pythia.FindDaughter (
                            (long) /* ParentID = */ tid[1] ,
                            (long) /* DPID     = */ -24
                        )
                    )
                } ;
                WBS[0] =
                    pythia
                    .GetVector
                        (wid[0])
                ; //
                WBS[1] =
                    pythia
                    .GetVector
                        (wid[1])
                ; //
                long j1id[2] = {
                    pythia.event[wid[0]]
                        .daughter1 () ,
                    pythia.event[wid[1]]
                        .daughter1 ()
                } ;
                JT1[0] =
                    pythia
                    .GetVector
                        (j1id[0])
                ; //
                JT1[1] =
                    pythia
                    .GetVector
                        (j1id[1])
                ; //
                long j2id[2] = {
                    pythia.event[wid[0]]
                        .daughter2 () ,
                    pythia.event[wid[1]]
                        .daughter2 ()
                } ;
                JT2[0] =
                    pythia
                    .GetVector
                        (j2id[0])
                ; //
                JT2[1] =
                    pythia
                    .GetVector
                        (j2id[1])
                ; //
                long bid[2] = {
                    pythia.FindDaughter (
                        (long) /* ParentID = */ tid[0] ,
                        (long) /* DPID     = */ +5
                    ) ,
                    pythia.FindDaughter (
                        (long) /* ParentID = */ tid[1] ,
                        (long) /* DPID     = */ -5
                    )
                } ;
                BOT[0] =
                    pythia
                    .GetVector
                        (bid[0])
                ;
                BOT[1] =
                    pythia
                    .GetVector
                        (bid[1])
                ;
            }
            //
            fastjet::JetAlgorithm algo =
                fastjet::antikt_algorithm
            ; //
            fastjet::JetDefinition
                jet_def (
                    algo,
                    1.0
                )
            ;
            fastjet::ClusterSequence
                clust_seq (
                    hadrons,
                    jet_def
                )
            ;
            vector4s jets =
                clust_seq
                .inclusive_jets
                        (800.0)
            ;
            //
            if (jets.size()>0) {
                //
                bool genmatched =
                    false
                ; //
                for (size_t kk=0;kk<2;kk++) {
                    genmatched = genmatched || (
                        (jets[0].delta_R(TOP[kk])<0.6) &&
                        (jets[0].delta_R(WBS[kk])<0.6) &&
                        (jets[0].delta_R(BOT[kk])<0.6) &&
                        (jets[0].delta_R(JT1[kk])<0.6) &&
                        (jets[0].delta_R(JT2[kk])<0.6)
                    ) ;
                }
                //
                if (
                    (genmatched) &&
                    (jets[0].constituents().size()>2)
                ) {
                    auto tmppt =
                        jets[0]
                        .pt()
                    ; //
                    auto tmpet =
                        jets[0]
                        .rapidity()
                    ; //
                    if (
                        (
                            ( 800 < tmppt ) &&
                            ( tmppt < 900 )
                        ) && (
                            ( -2.5 < tmpet ) &&
                            ( tmpet < 2.5 )
                        )
                    ) {
                        writer(jets[0]);
                        count++ ;
                    }
                }
            }
        }
    }
    //
    inline void
    Generate_Train_Top () {
        CPPFileIO::ForkMe forker ;
        for (size_t i=0;i<8;i++)
        if (forker.InKid()) {
            Generate_TOP (
                i       ,
                "TRAIN" ,
                123     ,
                300000
            ) ; //
        }
    }
    //
    inline void
    Generate_Test_Top () {
        CPPFileIO::ForkMe forker ;
        for (size_t i=0;i<8;i++)
        if (forker.InKid()) {
            Generate_TOP (
                i      ,
                "TEST" ,
                456    ,
                50000
            ) ; //
        }
    }
    //
    inline void
    PlotNsub () {
        /* TRAIN */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_TRAIN_TOP",
                    100,-0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/TOP/TRAIN/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (size_t i=0;i<Reader();i++) {
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                }
            }
        }
        /* TEST */ if(true) {
            MyHistN <5,false>
                nsubcompare (
                    "TAU_TEST_TOP",
                    100,-0.01,100.01
                )
            ; //
            for(
                size_t index=0;
                index<8;
                index++
            ) {
                char tmp[256] ;
                sprintf (
                    tmp,
                    "./OUTS/TOP/TEST/%ld/nsub",
                    index
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_NSub>
                        Reader (tmp)
                ; //
                for (size_t i=0;i<Reader();i++) {
                    nsubcompare.Fill<0>
                        (Reader(i)[0])
                    ; //
                    nsubcompare.Fill<1>
                        (Reader(i)[1])
                    ; //
                    nsubcompare.Fill<2>
                        (Reader(i)[2])
                    ; //
                    nsubcompare.Fill<3>
                        (Reader(i)[3])
                    ; //
                    nsubcompare.Fill<4>
                        (Reader(i)[4])
                    ; //
                }
            }
        }
    }
    //
    inline void
    PlotImages () {
        /* TRAIN */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("TOP_TRAIN_IMAGES");
                tmp("./OUTS/TOP/TRAIN/0/image");
                tmp("./OUTS/TOP/TRAIN/1/image");
                tmp("./OUTS/TOP/TRAIN/2/image");
                tmp("./OUTS/TOP/TRAIN/3/image");
                tmp("./OUTS/TOP/TRAIN/4/image");
                tmp("./OUTS/TOP/TRAIN/5/image");
                tmp("./OUTS/TOP/TRAIN/6/image");
                tmp("./OUTS/TOP/TRAIN/7/image");
            }
        }
        /* TEST */ if(true) {
            /* NORMAL */ {
                Plot2D tmp("TOP_TEST_IMAGES");
                tmp("./OUTS/TOP/TEST/0/image");
                tmp("./OUTS/TOP/TEST/1/image");
                tmp("./OUTS/TOP/TEST/2/image");
                tmp("./OUTS/TOP/TEST/3/image");
                tmp("./OUTS/TOP/TEST/4/image");
                tmp("./OUTS/TOP/TEST/5/image");
                tmp("./OUTS/TOP/TEST/6/image");
                tmp("./OUTS/TOP/TEST/7/image");
            }
        }
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP4_UNITEDATA {
    //
    using namespace MISC ;
    //
    inline void
    UniteData () {
        Executor(
            "./unite"
        ) ;
    }
    //
    inline void
    DataMixer (
        std::string src1 ,
        std::string src2 ,
        std::string dst
    ) {
        CPPFileIO::FileVector
            <imagetype>
                outimage (
                    dst +
                    "/image"
                )
        ;
        CPPFileIO::FileVector
            <TYPE_DATA>
                outlabel (
                    dst +
                    "/label"
                )
        ;
        CPPFileIO::FullFileReader
            <imagetype>
                SigRead (
                    src1 +
                    "/image"
                )
        ; //
        CPPFileIO::FullFileReader
            <imagetype>
                BkgRead (
                    src2 +
                    "/image"
                )
        ; //
        size_t Limit =
            CPPFileIO::mymin (
                BkgRead() ,
                SigRead()
            )
        ;
        for(size_t i=0;i<Limit;i++){
            outimage.push_back
                (SigRead(i))
            ;
            outlabel.push_back
                (1.0)
            ;
            outimage.push_back
                (BkgRead(i))
            ;
            outlabel.push_back
                (0.0)
            ;
        }
    }
    //
    inline void
    MixDataTopQCD () {
        /* Create directories: */ {
            mkdir (
                "./OUTS/TMP/"               ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_TOP/"       ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_TOP/TRAIN/" ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_TOP/TEST/"  ,
                0755
            ) ; //
        }
        for (size_t index=0;index<8;index++) {
            /* Train Part : */ if ( true ) {
                char
                    tmp1[512]
                ; /* prepare the destination: */ {
                    sprintf (
                        tmp1,
                        "./OUTS/TMP/QCD_TOP/TRAIN/%ld/",
                        index
                    ) ; //
                }
                mkdir
                    (tmp1,0755)
                ; //
                char
                    tmp2[512], tmp3[512]
                ; /* prepare the sources: */ {
                    sprintf(tmp2,"./OUTS/TOP/TRAIN/%ld/",index);
                    sprintf(tmp3,"./OUTS/QCD/TRAIN/%ld/",index);
                }
                DataMixer
                    (tmp2,tmp3,tmp1)
                ; //
            }
            /* Test  Part : */ if ( true ) {
                char
                    tmp1[512]
                ; /* prepare the destination: */ {
                    sprintf (
                        tmp1,
                        "./OUTS/TMP/QCD_TOP/TEST/%ld/",
                        index
                    ) ; //
                }
                mkdir
                    (tmp1,0755)
                ; //
                char
                    tmp2[512], tmp3[512]
                ; /* prepare the sources: */ {
                    sprintf(tmp2,"./OUTS/TOP/TEST/%ld/",index);
                    sprintf(tmp3,"./OUTS/QCD/TEST/%ld/",index);
                }
                DataMixer
                    (tmp2,tmp3,tmp1)
                ; //
            }
        }
    }
    //
    inline void
    MixDataWBSQCD () {
        /* Create directories: */ {
            mkdir (
                "./OUTS/TMP/"               ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_WBS/"       ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_WBS/TRAIN/" ,
                0755
            ) ; //
            mkdir (
                "./OUTS/TMP/QCD_WBS/TEST/"  ,
                0755
            ) ; //
        }
        for (size_t index=0;index<8;index++) {
            /* Train Part : */ if ( true ) {
                char
                    tmp1[512]
                ; /* prepare the destination: */ {
                    sprintf (
                        tmp1,
                        "./OUTS/TMP/QCD_WBS/TRAIN/%ld/",
                        index
                    ) ; //
                    mkdir (
                        tmp1,
                        0755
                    ) ; //
                }
                char
                    tmp2[512], tmp3[512]
                ; /* prepare the sources: */ {
                    sprintf(
                        tmp2,
                        "./OUTS/WBS/TRAIN/%ld/",
                        index
                    ) ;
                    sprintf(
                        tmp3,
                        "./OUTS/QCD/TRAIN/%ld/",
                        index
                    );
                }
                DataMixer
                    (tmp2,tmp3,tmp1)
                ; //
            }
            /* Test  Part : */ if ( true ) {
                char
                    tmp1[512]
                ; /* prepare the destination: */ {
                    sprintf (
                        tmp1,
                        "./OUTS/TMP/QCD_WBS/TEST/%ld/",
                        index
                    ) ; //
                    mkdir (
                        tmp1,
                        0755
                    ) ; //
                }
                char
                    tmp2[512], tmp3[512]
                ; /* prepare the sources: */ {
                    sprintf(tmp2,"./OUTS/WBS/TEST/%ld/",index);
                    sprintf(tmp3,"./OUTS/QCD/TEST/%ld/",index);
                }
                DataMixer
                    (tmp2,tmp3,tmp1)
                ; //
            }
        }
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP5_EVALERROR {
    //
    using namespace MISC ;
    //
    inline void
    RunPredict () {
        CPPFileIO::ForkMe forker ;
        if(forker.InKid()){
            std::vector
                <std::string>
                    program
            ; //
            program.push_back (
                "./MXNET/PREDICT.py"
            ) ; //
            CPPFileIO::starter_self
                (program)
            ; //
        }
    }
    //
    inline void
    EVAL_PREDICT (
        std::string dirname
    ) {
        CPPFileIO::FullFileReader
            <TYPE_PREDICT>
                Reader (
                    dirname +
                    "/predict"
                )
        ; //
        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer (
                    dirname +
                    "/loss"
                )
        ; //
        for (size_t i=0;i<Reader();i++) {
            Writer.push_back (
                sqrt (
                    Reader(i)
                    .L2_NORM()
                )
            ) ; //
        }
    }
    //
    inline void
    MakeLosses () {
        if (true) {
            RunPredict () ;
        }
        CPPFileIO::ForkMe forks ;
        if (true) if (forks.InKid()) {
            EVAL_PREDICT("./OUTS/QCD/TRAIN/0");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/1");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/2");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/3");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/4");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/5");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/6");
            EVAL_PREDICT("./OUTS/QCD/TRAIN/7");
        }
        if (true) if (forks.InKid()) {
            EVAL_PREDICT("./OUTS/QCD/TEST/0");
            EVAL_PREDICT("./OUTS/QCD/TEST/1");
            EVAL_PREDICT("./OUTS/QCD/TEST/2");
            EVAL_PREDICT("./OUTS/QCD/TEST/3");
            EVAL_PREDICT("./OUTS/QCD/TEST/4");
            EVAL_PREDICT("./OUTS/QCD/TEST/5");
            EVAL_PREDICT("./OUTS/QCD/TEST/6");
            EVAL_PREDICT("./OUTS/QCD/TEST/7");
        }
        if (true) if (forks.InKid()) {
            EVAL_PREDICT("./OUTS/TOP/TRAIN/0");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/1");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/2");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/3");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/4");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/5");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/6");
            EVAL_PREDICT("./OUTS/TOP/TRAIN/7");
        }
        if (true) if (forks.InKid()) {
            EVAL_PREDICT("./OUTS/TOP/TEST/0");
            EVAL_PREDICT("./OUTS/TOP/TEST/1");
            EVAL_PREDICT("./OUTS/TOP/TEST/2");
            EVAL_PREDICT("./OUTS/TOP/TEST/3");
            EVAL_PREDICT("./OUTS/TOP/TEST/4");
            EVAL_PREDICT("./OUTS/TOP/TEST/5");
            EVAL_PREDICT("./OUTS/TOP/TEST/6");
            EVAL_PREDICT("./OUTS/TOP/TEST/7");
        }
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP6_PLOTLOSSES {
    using namespace
        MISC
    ; //
    //
    inline void
    PlotLosses () {
        MyHistN <4,true>
            Hists ("Losses",100,-0.01,0.41)
        ; //
        for(size_t i=0;i<8;i++){
            char tmp[512] ;
            /* QCD TRAIN */ if(true) {
                sprintf (
                    tmp,
                    "./OUTS/QCD/TRAIN/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    ROC_ELEMENT tmp () ;
                    Hists.Fill<0>(Reader(j));
                }
            }
            /* QCD TEST */ if(true) {
                sprintf (
                    tmp,
                    "./OUTS/QCD/TEST/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    printf("debug: %e\n",Reader(j));
                    Hists.Fill<2>(Reader(j));
                }
            }
            /* TOP TRAIN */ if(true) {
                sprintf (
                    tmp,
                    "./OUTS/TOP/TRAIN/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    Hists.Fill<1>(Reader(j));
                }
            }
            /* TOP TEST */ if(true) {
                sprintf (
                    tmp,
                    "./OUTS/TOP/TEST/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    printf("debug: %e\n",Reader(j));
                    Hists.Fill<3>(Reader(j));
                }
            }
        }
    }
    //
    inline void
    PlotTestLosses () {
        MyHistN <2,true>
            Hists (
                "Losses",
                100,-0.01,
                0.41
            )
        ; //
        ROC_ELEMENTs roclist ;
        for(size_t i=0;i<8;i++){
            char tmp[512] ;
            /* QCD TEST */ {
                sprintf (
                    tmp,
                    "./OUTS/QCD/TEST/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    ROC_ELEMENT
                        tmproc (
                            Reader(j),
                            false
                        )
                    ;
                    roclist.push_back
                        (tmproc)
                    ;
                    Hists.Fill<0>(Reader(j));
                }
            }
            /* TOP TEST */ {
                sprintf (
                    tmp,
                    "./OUTS/TOP/TEST/%ld/loss",
                    i
                ) ; //
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        Reader (
                            tmp
                        )
                ; //
                for(size_t j=0;j<Reader();j++){
                    ROC_ELEMENT
                        tmproc (
                            Reader(j),
                            true
                        )
                    ;
                    roclist.push_back
                        (tmproc)
                    ;
                    Hists.Fill<1>(Reader(j));
                }
            }
            WriteROC (
                roclist ,
                "./OUTS/ROC/AE.txt"
            ) ; //
        }
    }
    //
    inline void
    MakeTMVARoot () {
        /* Create the directory structure: */ {
            mkdir("./OUTS/",0755);
            mkdir("./OUTS/TMP/",0755);
            mkdir("./OUTS/TMP/tmva/",0755);
        }
        TFile
            outfile (
                "./OUTS/TMP/tmva/tmva_class_example.root",
                "RECREATE"
            )
        ; //
        TTree signalTree ("TreeS","TreeS") ;
        TTree background ("TreeB","TreeB") ;
        float mass, loss ;
        signalTree.Branch ( "mass" , & mass ) ;
        signalTree.Branch ( "loss" , & loss ) ;
        background.Branch ( "mass" , & mass ) ;
        background.Branch ( "loss" , & loss ) ;
        /* QCD TEST */ {
            std::vector <std::string>
                lossfiles
            ; /* Create the loss list */ {
                lossfiles.push_back ("./OUTS/QCD/TEST/0/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/1/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/2/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/3/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/4/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/5/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/6/loss") ;
                lossfiles.push_back ("./OUTS/QCD/TEST/7/loss") ;
            }
            std::vector <std::string>
                vectfiles
            ; /* Create the vector list: */ {
                vectfiles.push_back ("./OUTS/QCD/TRAIN/0/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/1/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/2/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/3/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/4/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/5/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/6/vector") ;
                vectfiles.push_back ("./OUTS/QCD/TRAIN/7/vector") ;

            }
            for(size_t index=0;index<8;index++){
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        READER_LOSS (
                            lossfiles[index]
                        )
                ; //
                CPPFileIO::FullFileReader
                    <outvector4>
                        READER_VECTOR (
                            vectfiles[index]
                        )
                ; //
                for(
                    size_t i = 0 ;
                    i < READER_LOSS() ;
                    i++
                ) {
                    loss =
                        READER_LOSS(i)
                    ; //
                    mass =
                        READER_VECTOR(i).m()
                    ;
                    background.Fill();
                }
            }
        }
        /* TOP TEST */ {
            std::vector <std::string>
                lossfiles
            ; /* Create the loss list */ {
                lossfiles.push_back ("./OUTS/TOP/TEST/0/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/1/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/2/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/3/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/4/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/5/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/6/loss") ;
                lossfiles.push_back ("./OUTS/TOP/TEST/7/loss") ;
            }
            std::vector <std::string>
                vectfiles
            ; /* Create the vector list: */ {
                vectfiles.push_back ("./OUTS/TOP/TRAIN/0/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/1/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/2/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/3/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/4/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/5/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/6/vector") ;
                vectfiles.push_back ("./OUTS/TOP/TRAIN/7/vector") ;
            }
            for(size_t index=0;index<8;index++){
                CPPFileIO::FullFileReader
                    <TYPE_DATA>
                        READER_LOSS (
                            lossfiles[index]
                        )
                ; //
                CPPFileIO::FullFileReader
                    <outvector4>
                        READER_VECTOR (
                            vectfiles[index]
                        )
                ; //
                for(
                    size_t i = 0 ;
                    i < READER_LOSS() ;
                    i++
                ) {
                    loss =
                        READER_LOSS(i)
                    ; //
                    mass =
                        READER_VECTOR(i).m()
                    ;
                    signalTree.Fill();
                }
            }
        }
        signalTree.Write();
        background.Write();
    }
    //
}
////////////////////////////////////////////////////////////////
namespace STEP7_CNN_RESPONSE {
    using namespace MISC ;
    //
    inline void
    Plotter () {
        using TYPE_RESPONSE =
            Tensors::NN::ND_ARRAY
                <2,TYPE_DATA>
        ;
        std::vector
            <ROC_ELEMENT>
                elements
        ;
        MyHistN <4,true>
            Hist (
                "CNNResponse",
                50,-0.01,1.01
            )
        ; //
        for(size_t index=0;index<8;index++){
            char tmp[512] ;
            /* TEST */ {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TEST/%ld/CNNPredict",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back(
                            ROC_ELEMENT(reader(i)[0],false)
                        ) ; //
                        Hist.Fill<0>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/TOP/TEST/%ld/CNNPredict",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back(
                            ROC_ELEMENT(reader(i)[0],true)
                        ) ; //
                        Hist.Fill<1>(reader(i)[0]);
                    }
                }
            }
            /* TRAIN */ {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TRAIN/%ld/CNNPredict",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<2>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/TOP/TRAIN/%ld/CNNPredict",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<3>(reader(i)[0]);
                    }
                }
            }
        }
        mkdir ("./OUTS/ROC/",0755) ;
        WriteROC (
            elements,
            "./OUTS/ROC/QCD_TOP.txt"
        ) ; //
    }
    //
    inline void
    PlotterDCGAN () {
        using TYPE_RESPONSE =
            Tensors::NN::ND_ARRAY
                <2,TYPE_DATA>
        ;
        std::vector
            <ROC_ELEMENT>
                elements
        ;
        MyHistN <4,true>
            Hist (
                "DCGANResponse",
                50,-0.01,1.01
            )
        ; //
        for(size_t index=0;index<8;index++){
            char tmp[512] ;
            /* TEST */ {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TEST/%ld/DCGAN_RESPONSE",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back (
                            ROC_ELEMENT(reader(i)[0],false)
                        ) ; //
                        Hist.Fill<0>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/TOP/TEST/%ld/DCGAN_RESPONSE",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back (
                            ROC_ELEMENT(reader(i)[0],true)
                        ) ; //
                        Hist.Fill<1>(reader(i)[0]);
                    }
                }
            }
            /* TRAIN */ if(false) {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TRAIN/%ld/DCGAN_RESPONSE",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<2>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/TOP/TRAIN/%ld/DCGAN_RESPONSE",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<3>(reader(i)[0]);
                    }
                }
            }
        }
        mkdir ("./OUTS/ROC/",0755) ;
        WriteROC (
            elements,
            "./OUTS/ROC/QCD_TOP_DCGAN.txt"
        ) ; //
    }
    //
    inline void
    PlotterQCDWBS () {
        using TYPE_RESPONSE =
            Tensors::NN::ND_ARRAY
                <2,TYPE_DATA>
        ;
        std::vector
            <ROC_ELEMENT>
                elements
        ;
        MyHistN <4,true>
            Hist (
                "CNNResponseQCDWBS",
                50,-0.01,1.01
            )
        ; //
        for(size_t index=0;index<8;index++){
            char tmp[512] ;
            /* TEST */ {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TEST/%ld/CNNPredictQCDWBS",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back(
                            ROC_ELEMENT(reader(i)[0],false)
                        ) ; //
                        Hist.Fill<0>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/WBS/TEST/%ld/CNNPredictQCDWBS",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        elements.push_back(
                            ROC_ELEMENT(reader(i)[0],true)
                        ) ; //
                        Hist.Fill<1>(reader(i)[0]);
                    }
                }
            }
            /* TRAIN */ {
                /* QCD */ {
                    sprintf (
                        tmp,
                        "./OUTS/QCD/TRAIN/%ld/CNNPredictQCDWBS",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<2>(reader(i)[0]);
                    }
                }
                /* TOP */ {
                    sprintf (
                        tmp,
                        "./OUTS/WBS/TRAIN/%ld/CNNPredictQCDWBS",
                        index
                    ) ; //
                    std::string filename (tmp) ;
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader (filename)
                    ;
                    for(size_t i=0;i<reader();i++){
                        Hist.Fill<3>(reader(i)[0]);
                    }
                }
            }
        }
        mkdir ("./OUTS/ROC/",0755) ;
        WriteROC (
            elements,
            "./OUTS/ROC/QCD_WBS.txt"
        ) ; //
    }
    //
    inline void
    MakeTMVARoot () {
        /* Create the directory structure: */ {
            mkdir("./OUTS/",0755);
            mkdir("./OUTS/TMP/",0755);
            mkdir("./OUTS/TMP/tmva/",0755);
        }
        TFile
            outfile (
                "./OUTS/TMP/tmva/tmva_class_example.root",
                "RECREATE"
            )
        ; //
        TTree signalTree ("TreeS","TreeS") ;
        TTree background ("TreeB","TreeB") ;
        float mass, cnnres, random ;
        signalTree.Branch ( "mass"   , & mass   ) ;
        signalTree.Branch ( "cnnres" , & cnnres ) ;
        signalTree.Branch ( "random" , & random ) ;
        background.Branch ( "mass"   , & mass   ) ;
        background.Branch ( "cnnres" , & cnnres ) ;
        background.Branch ( "random" , & random ) ;
        using TYPE_RESPONSE =
            Tensors::NN::ND_ARRAY
                <2,TYPE_DATA>
        ; //
        /* TEST: */ {
            /* QCD: */ {
                std::vector <std::string>
                    ListOfNamesCNN ,
                    ListOfNamesVectors
                ; /* Prepare the list: */ {
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/0/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/1/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/2/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/3/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/4/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/5/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/6/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/QCD/TEST/7/CNNPredict") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/0/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/1/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/2/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/3/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/4/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/5/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/6/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/QCD/TEST/7/vector") ;
                }
                CPPFileIO::myrandgen <pcg64>
                    randgen (1,0.1,0.2)
                ; //
                for (size_t index=0;index<8;index++) {
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader_res (
                                ListOfNamesCNN[index]
                            )
                    ; //
                    CPPFileIO::FullFileReader
                        <outvector4>
                            reader_vector (
                                ListOfNamesVectors[index]
                            )
                    ; //
                    size_t Limit =
                        CPPFileIO::mymin (
                            reader_res    () ,
                            reader_vector ()
                        )
                    ; //
                    for (size_t i=0;i<Limit;i++) {
                        mass   = reader_vector(i).m() ;
                        cnnres = reader_res(i)[1]     ;
                        random = randgen[0]           ;
                        background.Fill ()            ;
                    }
                }
            }
            /* TOP: */ {
                std::vector <std::string>
                    ListOfNamesCNN ,
                    ListOfNamesVectors
                ; /* Prepare the list: */ {
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/0/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/1/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/2/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/3/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/4/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/5/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/6/CNNPredict") ;
                    ListOfNamesCNN.push_back ("./OUTS/TOP/TEST/7/CNNPredict") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/0/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/1/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/2/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/3/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/4/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/5/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/6/vector") ;
                    ListOfNamesVectors.push_back ("./OUTS/TOP/TEST/7/vector") ;
                }
                CPPFileIO::myrandgen <pcg64>
                    randgen (1,0.8,0.9)
                ; //
                for (size_t index=0;index<8;index++) {
                    CPPFileIO::FullFileReader
                        <TYPE_RESPONSE>
                            reader_res (
                                ListOfNamesCNN[index]
                            )
                    ; //
                    CPPFileIO::FullFileReader
                        <outvector4>
                            reader_vector (
                                ListOfNamesVectors[index]
                            )
                    ; //
                    size_t Limit =
                        CPPFileIO::mymin (
                            reader_res    () ,
                            reader_vector ()
                        )
                    ; //
                    for (size_t i=0;i<Limit;i++) {
                        mass   = reader_vector(i).m() ;
                        cnnres = reader_res(i)[1]     ;
                        random = randgen[0]           ;
                        signalTree.Fill ()            ;
                    }
                }
            }
        }
        signalTree.Write () ;
        background.Write () ;
    }
    //
}
////////////////////////////////////////////////////////////////
