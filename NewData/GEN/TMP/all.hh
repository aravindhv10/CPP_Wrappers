#include "cblas.h"
#include "pcg.hh"
#define USENN
#define USE_ALL
#include "NewHEPHeaders8.hh"
#include <omp.h>
#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include "TTree.h"

#include "./HEPTopTagger2/main.hh"

typedef float FloatType ;
typedef FloatType TYPE_DATA ;
FloatType constexpr TrainRate = 0.001 ;
size_t constexpr SCALE = 100 ;
class Paths {
public:
    long j ;

    Paths (size_t _j=5) {j=_j;}

    static inline void MakeDirectories () {
        mkdir ( (const char *) /*pathname=*/ "./OUTS/"                , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/QCD/"            , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/TOP/"            , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/WBS/"            , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/PARS/"           , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/PARS_SAE/"       , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/PARS_SAE_MXNET/" , (mode_t) /*mode=*/ 0755 ) ;
        mkdir ( (const char *) /*pathname=*/ "./OUTS/SHIH/"           , (mode_t) /*mode=*/ 0755 ) ;
    }

    static inline std::string Path_MXNET_Pars ()
    { MakeDirectories () ; return std::string ("./OUTS/PARS_SAE_MXNET/") ; }

    static inline std::string PATH_Pars_SAE ()
    { MakeDirectories () ; return std::string ("./OUTS/PARS_SAE/") ; }

    static inline void Path_QCD ( char * path , size_t const i )
    { MakeDirectories () ; sprintf ( path , "./OUTS/QCD/%ld" , i ) ; }

    static inline void Path_TOP ( char * path , size_t const i )
    { MakeDirectories () ; sprintf ( path , "./OUTS/TOP/%ld" , i ) ; }

    static inline void Path_WBS ( char * path , size_t const i )
    { MakeDirectories () ; sprintf ( path , "./OUTS/WBS/%ld" , i ) ; }

    static inline void Path_QCD ( char * path )
    { MakeDirectories () ; sprintf ( path , "./OUTS/QCD.dat" ) ; }

    static inline void Path_TOP ( char * path )
    { MakeDirectories () ; sprintf ( path , "./OUTS/TOP.dat" ) ; }

    static inline void Path_QCD_SHIH ( char * path )
    { MakeDirectories () ; sprintf ( path , "./OUTS/SHIH/QCD.dat" ) ; }

    static inline void Path_TOP_SHIH ( char * path )
    { MakeDirectories () ; sprintf ( path , "./OUTS/SHIH/TOP.dat" ) ; }

    static inline std::string Path_TOP_SHIH ()
    { MakeDirectories () ; char tmp[128] ; Path_TOP_SHIH (tmp) ; return std::string (tmp) ; }

    static inline std::string Path_QCD_SHIH ()
    { MakeDirectories () ; char tmp[128] ; Path_QCD_SHIH (tmp) ; return std::string (tmp) ; }

    static inline std::string Path_QCD (size_t const i)
    { MakeDirectories () ; char tmp[256] ; Path_QCD (tmp,i) ; return std::string (tmp) ; }

    static inline std::string Path_TOP (size_t const i)
    { MakeDirectories () ; char tmp[256] ; Path_TOP (tmp,i) ; return std::string (tmp) ; }

    static inline std::string Path_WBS (size_t const i)
    { MakeDirectories () ; char tmp[256] ; Path_WBS (tmp,i) ; return std::string (tmp) ; }

    static inline std::string Path_TOP ()
    { MakeDirectories () ; char tmp[128] ; Path_TOP (tmp) ; return std::string (tmp) ; }

    static inline std::string Path_QCD ()
    { MakeDirectories () ; char tmp[128] ; Path_QCD (tmp) ; return std::string (tmp) ; }

    static inline std::string Batch_Path_QCD ()
    { MakeDirectories () ; return std::string ("./OUTS/QCD.batch") ; }

    static inline std::string Batch_Path_TOP ()
    { MakeDirectories () ; return std::string ("./OUTS/TOP.batch") ; }

    static inline std::string UNITED_QCD_PATH ()
    { MakeDirectories () ; return std::string ("./OUTS/QCD.united") ; }

    static inline std::string UNITED_TOP_PATH ()
    { MakeDirectories () ; return std::string ("./OUTS/TOP.united") ; }

    static inline std::string UNITED_WBS_PATH ()
    { MakeDirectories () ; return std::string ("./OUTS/WBS.united") ; }

    inline std::string TOP_ERROR_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/TOP.error.%ld",j); return std::string (tmp) ; }

    static inline std::string SHIH_TOP_ERROR_FILE ()
    { MakeDirectories () ; return std::string ("./OUTS/SHIH/TOP.error.dat");}

    static inline std::string SHIH_QCD_ERROR_FILE ()
    { MakeDirectories () ; return std::string ("./OUTS/SHIH/QCD.error.dat");}

    inline std::string QCD_ERROR_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/QCD.error.%ld",j); return std::string (tmp) ; }

    inline std::string TOP_ERROR_BATCH_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/TOP.error.%ld.batch",j); return std::string (tmp) ; }

    inline std::string QCD_ERROR_BATCH_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/QCD.error.%ld.batch",j); return std::string (tmp) ; }

    inline std::string ROC_PARS_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/PARS/ROC_Pars.%ld",j); return std::string (tmp) ; }

    static inline std::string SHIH_ROC_PARS_FILE ()
    { MakeDirectories () ; return std::string("./OUTS/SHIH/ROC_PARS"); }

    inline std::string TOP_ROC_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/TOP_ROC.%ld",j); return std::string (tmp) ; }

    inline std::string QCD_ROC_FILE ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/QCD_ROC.%ld",j); return std::string (tmp) ; }

    static inline std::string SHIH_TOP_ROC_FILE ()
    { MakeDirectories () ; return std::string("./OUTS/SHIH/TOP_ROC"); }

    static inline std::string SHIH_QCD_ROC_FILE ()
    { MakeDirectories () ; return std::string("./OUTS/SHIH/QCD_ROC"); }

    inline std::string ROC_FILE_TXT ()
    { MakeDirectories () ; char tmp[256] ; sprintf(tmp,"./OUTS/ROC.txt.%ld",j); return std::string (tmp) ; }

    static inline std::string SHIH_ROC_FILE_TXT ()
    { MakeDirectories () ; return std::string("./OUTS/SHIH/ROC_FILE.txt"); }
} ;
inline void MakeDirectories () { return Paths::MakeDirectories () ; }
inline void Path_QCD ( char * path , size_t i ) { Paths::Path_QCD (path,i) ; }
inline void Path_TOP ( char * path , size_t i ) { Paths::Path_TOP (path,i) ; }
inline void Path_QCD ( char * path ) { Paths::Path_QCD (path) ; }
inline void Path_TOP ( char * path ) { Paths::Path_TOP (path) ; }
inline std::string Path_MXNET_Pars () { return Paths::Path_MXNET_Pars () ; }
inline std::string PATH_Pars_SAE   () { return Paths::PATH_Pars_SAE   () ; }
inline std::string Path_QCD (size_t const i) { return Paths::Path_QCD (i) ; }
inline std::string Path_TOP (size_t const i) { return Paths::Path_TOP (i) ; }
inline std::string Path_TOP () { return Paths::Path_TOP () ; }
inline std::string Path_QCD () { return Paths::Path_QCD () ; }
inline std::string Batch_Path_QCD () { return Paths::Batch_Path_QCD () ; }
inline std::string Batch_Path_TOP () { return Paths::Batch_Path_TOP () ; }
inline std::string UNITED_QCD_PATH () { return Paths::UNITED_QCD_PATH () ; }
inline std::string UNITED_TOP_PATH () { return Paths::UNITED_TOP_PATH () ; }
inline std::string TOP_ERROR_FILE       (size_t i=5) { Paths tmp(i) ; return tmp.TOP_ERROR_FILE       () ; }
inline std::string QCD_ERROR_FILE       (size_t i=5) { Paths tmp(i) ; return tmp.QCD_ERROR_FILE       () ; }
inline std::string TOP_ERROR_BATCH_FILE (size_t i=5) { Paths tmp(i) ; return tmp.TOP_ERROR_BATCH_FILE () ; }
inline std::string QCD_ERROR_BATCH_FILE (size_t i=5) { Paths tmp(i) ; return tmp.QCD_ERROR_BATCH_FILE () ; }
inline std::string ROC_PARS_FILE        (size_t i=5) { Paths tmp(i) ; return tmp.ROC_PARS_FILE        () ; }
inline FloatType AlliedAngle (FloatType Phi) {
    using namespace NewHEPHeaders::CONSTANTS ;
    while(Phi>PI){Phi-=PI;}
    while(Phi<0){Phi+=PI;}
    return Phi;
}
typedef NewHEPHeaders::VECTORS::lorentz4vector < FloatType > Vector4 ;
typedef NewHEPHeaders::VECTORS::euclid3vector  < FloatType > Vector3 ;
typedef NewHEPHeaders::VECTORS::plane2vector   < FloatType > Vector2 ;
typedef Tensors::Arrays::N2DArray <40,40,FloatType> InputArrayType ;
typedef Tensors::Arrays::NDArray <InputArrayType::Size(),InputArrayType::TYPE_Data> FlatArrayType ;
typedef Tensors::Arrays::NDArray <5,InputArrayType::TYPE_Data> NSubjettinessArray ;
inline bool CheckTopTag (fastjet::PseudoJet const & inref) {
    HEPTopTagger::HEPTopTagger tagger(inref);
    tagger.set_max_subjet_mass(30.0);
    tagger.set_mass_drop_threshold(0.8);
    tagger.set_filtering_R(0.3);
    tagger.set_filtering_n(5);
    tagger.set_filtering_minpt_subjet(30.);

    // How to select among candidates
    tagger.set_mode(HEPTopTagger::TWO_STEP_FILTER);

    // Requirements to accept a candidate
    tagger.set_top_minpt(200);
    tagger.set_top_mass_range(150., 200.);
    tagger.set_fw(0.15);

    // Run the tagger
    tagger.run();
    return tagger.is_tagged();
}
inline NSubjettinessArray EvalNSub (fastjet::PseudoJet const & injet) {
    using namespace std;
    using namespace fastjet;
    using namespace fastjet::contrib;
    constexpr double beta = 2.0;
    NSubjettinessArray ret ;
    Nsubjettiness nSub1_beta2 ( 1 , OnePass_WTA_KT_Axes() , UnnormalizedMeasure(beta) ) ;
    Nsubjettiness nSub2_beta2 ( 2 , OnePass_WTA_KT_Axes() , UnnormalizedMeasure(beta) ) ;
    Nsubjettiness nSub3_beta2 ( 3 , OnePass_WTA_KT_Axes() , UnnormalizedMeasure(beta) ) ;
    Nsubjettiness nSub4_beta2 ( 4 , OnePass_WTA_KT_Axes() , UnnormalizedMeasure(beta) ) ;
    Nsubjettiness nSub5_beta2 ( 5 , OnePass_WTA_KT_Axes() , UnnormalizedMeasure(beta) ) ;
    ret[0] = nSub1_beta2 (injet) ;
    ret[1] = nSub2_beta2 (injet) ;
    ret[2] = nSub3_beta2 (injet) ;
    ret[3] = nSub4_beta2 (injet) ;
    ret[4] = nSub5_beta2 (injet) ;
    return ret ;
}
//
template <size_t Num, bool compare=false>
class MyHistN {
private:
    std::string   histname         ;
    std::string   histNnames [Num] ;
    TH1F        * Hists      [Num] ;
    bool logscale ;
public:
    //
    template <size_t Ii>
    inline void
    Fill (
        double a
    ) {
        if (a>-90.0) {
            Hists[Ii]->Fill(a);
        }
    }
    //
    inline void
    NormalizeHist (
        TH1F * inhist
    ) {
        inhist->Scale(
            1.0 /
            inhist->Integral()
        ) ; //
        inhist->SetLineWidth(3);
    }
    //
    inline void
    DeleteAll () {
        for (size_t ii=0;ii<Num;ii++)
        { delete Hists[ii] ; }
    }
    //
    inline void Write () {
        //printf("DEBUG: Called Write();");
        TCanvas C ;
        if(logscale){
            C.SetLogy();
        }
        Style_t styles[Num] ;
        {for(size_t i=0;i<Num;i++){styles[i]=kSolid;}}
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
        double YMax = 0.21 ;
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
        mkdir((const char*)"./GRAPHS",(mode_t)0755);
        std::string outname  = "./GRAPHS/" + histname + ".pdf" ;
        std::string outnameC = "./GRAPHS/" + histname + ".C"   ;
        C.SaveAs(&(outname[0])); C.SaveAs(&(outnameC[0]));
        DeleteAll () ;
    }
    //
    inline void
    AllocateAll (
        size_t nbins,
        double min,
        double max
    ) {
        for(size_t ii=0;ii<Num;ii++) {
            char tmp[1024];
            sprintf(tmp,"%s_%ld",&(histname[0]),ii);
            histNnames[ii] =
                std::string
                    (tmp)
            ; //
            Hists[ii] =
                new TH1F (
                    & ( histNnames[ii][0] ) ,
                    & ( histname [0] ) ,
                    nbins , min , max
                )
            ; //
        }
    }
    //
    inline void
    SetLogScale (
        bool _logscale
    ) {
        logscale=_logscale;
    }
    //
    MyHistN (
        std::string _histname,
        size_t nbins,
        double min,
        double max
    ) : histname(_histname) {
        AllocateAll
            (nbins,min,max)
        ; //
        logscale=false;
    }
    //
    ~MyHistN () { Write () ; }
};
//
template <size_t N=6>
inline MyHistN <N>
GetHist ( std::string name ) {
    MyHistN <N>
            hists (
                name, 100,
                0.0, 0.6
            )
    ; //
    return hists ;
}
//
namespace Datas {

    constexpr FloatType epsilon =
        NewHEPHeaders::SMALL_EPSILON
    ;

    class QCDPreProcessor_SIM {
    public:
        InputArrayType     MainStore  ;
        bool               eventpass  ;
        Vector4            FullJet    ;
        Vector4            BoostedJet ;
        bool               HTT        ;
        NSubjettinessArray Tau        ;
    private:
        inline void Reader (fastjet::PseudoJet & injet) {
            Tau = EvalNSub (injet) ;
            std::vector <Vector4> JetVectors ;
            /* The initialization part: */ {
                eventpass = false ;
                FullJet   = injet ;
                MainStore = 0.0   ;
                HTT       = false ;
            }
            HTT = CheckTopTag(injet);
            constexpr FloatType E_0 = 2.0 ;
            /* The reading and lorentz boosting part: */ {
                NewHEPHeaders::pseudojets jetvectors = injet.constituents () ;
                JetVectors.resize (jetvectors.size()) ;
                const FloatType M_J = CPPFileIO::mymax(FullJet.m(),(FloatType)epsilon);
                const FloatType E_J = FullJet[3] / M_J ;
                const FloatType P_J = FullJet.p () / M_J ;
                constexpr FloatType P_0 = sqrt ( (E_0*E_0) - 1.0 ) ;
                const FloatType gamma = ( (E_0*E_J) - (P_J*P_0) ) ;
                Vector3 dir ;
                if ( E_J > E_0 ) { dir = FullJet.xyz.dir() *   gamma          ; }
                else             { dir = FullJet.xyz.dir() * ( gamma * -1.0 ) ; }
                BoostedJet = FullJet.LorentzBoostGamma(dir) / M_J ;
                for (size_t i=0;i<JetVectors.size();i++) {
                    Vector4 tmpvec ;
                    tmpvec[0] = jetvectors[i].px () / M_J ;
                    tmpvec[1] = jetvectors[i].py () / M_J ;
                    tmpvec[2] = jetvectors[i].pz () / M_J ;
                    tmpvec[3] = jetvectors[i].e  () / M_J ;
                    JetVectors[i] = tmpvec.LorentzBoostGamma(dir);
                }
            }
            eventpass = (JetVectors.size()>2) ;
            if (eventpass) {
                std::sort(JetVectors.begin(),JetVectors.end());
                Vector3 Basis[3] ; /* Evaluate the ortho normal basis: */ {
                    Basis[0] = BoostedJet.xyz.dir    () ;
                    Basis[1] = JetVectors[0].xyz.dir () ;
                    Basis[2] = JetVectors[1].xyz.dir () ;
                    Basis[1] = Basis[1] - (Basis[0]*(Basis[1]*Basis[0])) ;
                    Basis[1] = Basis[1].dir () ;
                    Basis[2] = Basis[2] - (Basis[0]*(Basis[0]*Basis[2])) - (Basis[1]*(Basis[1]*Basis[2])) ;
                    Basis[2] = Basis[2].dir () ;
                }
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t i=0;i<JetVectors.size();i++){
                    FloatType z = JetVectors[i].xyz * Basis[0] ;
                    FloatType x = JetVectors[i].xyz * Basis[1] ;
                    FloatType y = JetVectors[i].xyz * Basis[2] ;
                    FloatType e = JetVectors[i][3]             ;
                    jetvectors.push_back(JetVectors[i].getpseudojet());
                    JetVectors[i].xyz[0] = z ;
                    JetVectors[i].xyz[1] = x ;
                    JetVectors[i].xyz[2] = y ;
                    JetVectors[i].xyz = JetVectors[i].xyz.dir() ;
                }
                NewHEPHeaders::JetClusterExclusive jet(jetvectors,1,0.0,1);
                //Tau = EvalNSub(jet[0]);
                //printf("TauN: (%e %e %e %e %E)\n",Tau[0],Tau[1],Tau[2],Tau[3],Tau[4]);
                //Vector4 checkingjet(0,0,0,0) ;
                MainStore = 0.0 ;
                for(size_t i=0;i<JetVectors.size();i++){
                    constexpr FloatType dx = ( 2.0 + epsilon ) / ((FloatType)InputArrayType::SizeX()) ;
                    constexpr FloatType dy = ( 2.0 + epsilon ) / ((FloatType)InputArrayType::SizeY()) ;
                    long x = (long) ( ( JetVectors[i][1] + 1.0 ) / dx ) ;
                    long y = (long) ( ( JetVectors[i][2] + 1.0 ) / dy ) ;
                    if (
                        ( (0<=x) && (x<InputArrayType::SizeX()) ) &&
                        ( (0<=y) && (y<InputArrayType::SizeY()) )
                    ) { MainStore[y][x] += JetVectors[i][3] / E_0 ; }
                    else { printf("DISASTER... y:%ld x:%ld\n",y,x); }
                }
                //printf("DEBUG: %e\n",MainStore.L1Norm());
            }
        }
    public:
        inline FloatType mass () {return FullJet.m();}
        inline bool pass () {
            FloatType tmpmass = mass();
            return eventpass&&(150.0<tmpmass)&&(tmpmass<200.0);
            //return eventpass&&((150.0<tmpmass)&&(tmpmass<200.0));
            //return eventpass;
        }
        inline void PrintOut (FILE * f) {
            for(size_t y=0;y<InputArrayType::SizeY();y++){
                for(size_t x=0;x<InputArrayType::SizeX();x++) {
                    fprintf(f,"%e",(FloatType)MainStore[y][x]);
                    bool donespace =
                        (y==(InputArrayType::SizeY()-1)) &&
                        (x==(InputArrayType::SizeX()-1)) ;
                    if(!donespace) {fprintf(f," ");}
                }
            }
        }
        inline void operator () (TH2F & inhist) {
            for(size_t y=0;y<InputArrayType::SizeY();y++){
                for(size_t x=0;x<InputArrayType::SizeX();x++) {
                    FloatType e = MainStore[y][x];
                    inhist.Fill(x,y,e);
                }
            }
        }
        inline InputArrayType & GetArray () { return MainStore ; }
        inline void operator () (FlatArrayType & in) {
            typedef FlatArrayType::TYPE_Data TYPE_Data ;
            TYPE_Data * Dst = (TYPE_Data*) in.GET_DATA        () ;
            TYPE_Data * Src = (TYPE_Data*) MainStore.GET_DATA () ;
            for (size_t i=0;i<FlatArrayType::Size();i++) { Dst[i] = Src[i] ; }
        }
        inline void operator () (fastjet::PseudoJet & injet) { Reader (injet) ; }
        QCDPreProcessor_SIM     (fastjet::PseudoJet & injet) { Reader (injet) ; }
        QCDPreProcessor_SIM     ()                           {}
        ~QCDPreProcessor_SIM    ()                           {}
    } ;

    //typedef QCDPreProcessor_SIM  QCDPreProcessor ;

    class QCDPreProcessor_SHIH {
    public:
        InputArrayType     MainStore      ;
        InputArrayType     MainStore_SHIH ;
        bool               eventpass      ;
        Vector4            FullJet        ;
        Vector4            BoostedJet     ;
        bool               HTT            ;
        NSubjettinessArray Tau            ;
    private:
        void Process_My_Method (fastjet::PseudoJet const & injet) {
            MainStore      = 0.0   ;
            Tau = EvalNSub    (injet) ;
            HTT = CheckTopTag (injet) ;
            constexpr FloatType E_0 = 2.0 ;
            std::vector <Vector4> JetVectors ;
            /* The reading and lorentz boosting part: */ if(true) {
                NewHEPHeaders::pseudojets jetvectors = injet.constituents () ;
                JetVectors.resize (jetvectors.size()) ;
                const FloatType M_J = CPPFileIO::mymax
                    (FullJet.m(),(FloatType)epsilon);
                const FloatType E_J = FullJet[3] / M_J ;
                const FloatType P_J = FullJet.p () / M_J ;
                constexpr FloatType P_0 = sqrt ( (E_0*E_0) - 1.0 ) ;
                const FloatType gamma = ( (E_0*E_J) - (P_J*P_0) ) ;
                Vector3 dir ;
                if ( E_J > E_0 ) { dir = FullJet.xyz.dir() *   gamma          ; }
                else             { dir = FullJet.xyz.dir() * ( gamma * -1.0 ) ; }
                BoostedJet = FullJet.LorentzBoostGamma (dir) / M_J ;
                for (size_t i=0;i<JetVectors.size();i++) {
                    Vector4 tmpvec ;
                    tmpvec[0] = jetvectors[i].px () / M_J ;
                    tmpvec[1] = jetvectors[i].py () / M_J ;
                    tmpvec[2] = jetvectors[i].pz () / M_J ;
                    tmpvec[3] = jetvectors[i].e  () / M_J ;
                    JetVectors[i] = tmpvec.LorentzBoostGamma(dir);
                }
            }
            eventpass = (JetVectors.size()>2) ;
            if (eventpass) {
                std::sort(JetVectors.begin(),JetVectors.end());
                Vector3 Basis[3] ; /* Evaluate the ortho normal basis: */ {
                    Basis[0] = BoostedJet.xyz.dir    () ;
                    Basis[1] = JetVectors[0].xyz.dir () ;
                    Basis[2] = JetVectors[1].xyz.dir () ;
                    Basis[1] = Basis[1] - (Basis[0]*(Basis[1]*Basis[0])) ;
                    Basis[1] = Basis[1].dir () ;
                    Basis[2] = Basis[2] -
                        (Basis[0]*(Basis[0]*Basis[2])) -
                        (Basis[1]*(Basis[1]*Basis[2])) ;
                    Basis[2] = Basis[2].dir () ;
                }
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t i=0;i<JetVectors.size();i++){
                    FloatType z = JetVectors[i].xyz * Basis[0] ;
                    FloatType x = JetVectors[i].xyz * Basis[1] ;
                    FloatType y = JetVectors[i].xyz * Basis[2] ;
                    FloatType e = JetVectors[i][3]             ;
                    jetvectors.push_back(JetVectors[i].getpseudojet());
                    JetVectors[i].xyz[0] = z ;
                    JetVectors[i].xyz[1] = x ;
                    JetVectors[i].xyz[2] = y ;
                    JetVectors[i].xyz = JetVectors[i].xyz.dir() ;
                }
                NewHEPHeaders::JetClusterExclusive jet(jetvectors,1,0.0,1);
                MainStore = 0.0 ;
                for(size_t i=0;i<JetVectors.size();i++){
                    constexpr FloatType dx = ( 2.0 + epsilon ) /
                        ((FloatType)InputArrayType::SizeX()) ;
                    constexpr FloatType dy = ( 2.0 + epsilon ) /
                        ((FloatType)InputArrayType::SizeY()) ;
                    long x = (long) ( ( JetVectors[i][1] + 1.0 ) / dx ) ;
                    long y = (long) ( ( JetVectors[i][2] + 1.0 ) / dy ) ;
                    if(
                        ((0<=x)&&(x<InputArrayType::SizeX())) &&
                        ((0<=y)&&(y<InputArrayType::SizeY()))
                    ) { MainStore[y][x] += JetVectors[i][3] / E_0 ; }
                    else { printf("DISASTER... y:%ld x:%ld\n",y,x); }
                }
            }
        }

        static inline long IntMap ( long Y2 , FloatType X2 , FloatType X )
        { return (long) Y2 + ( ( (FloatType) Y2 / ( 2.0 * X2 ) ) * ( X - X2 ) ) ; }

        void Process_Shih_Method (fastjet::PseudoJet const & injet) {
            MainStore_SHIH = 0.0 ;
            NewHEPHeaders::pseudojets jetvectors = injet.constituents () ;

            Vector2 BasisVectors[3] ; /* Find good basis vectors: */ {
                NewHEPHeaders::JetClusterExclusive jets3 (
                    /* _invectors = */ jetvectors ,
                    (long)      /* _algo = */ 1 ,
                    (double)    /* _Pt   = */ 1 ,
                    (int const) /* _N    = */ 3
                ) ;
                for (size_t i=0;i<3;i++) {
                    Vector2 tmp ; /* Evaluate the coordinates: */ {
                        tmp[0] = ( injet.eta() - jets3[i].eta() ) ;
                        tmp[1] = ( injet.phi() - jets3[i].phi() ) ;
                    }
                    BasisVectors[i] = tmp ;
                }
            }

            // Initialization Part:
            std::vector <Vector2>   D2JetVectors ;
            std::vector <FloatType> D2JetPt      ;
            Vector2 Centroid(0,0) ;
            MainStore_SHIH = 0.0 ;
            D2JetVectors.resize (jetvectors.size()) ;
            D2JetPt.resize      (jetvectors.size()) ;
            for (size_t i=0;i<D2JetVectors.size();i++) {
                Vector2 tmp ; /* Evaluate the coordinates: */ {
                    tmp[0] = ( injet.eta() - jetvectors[i].eta() ) ;
                    tmp[1] = ( injet.phi() - jetvectors[i].phi() ) ;
                }
                D2JetVectors[i]  = tmp ;
                Centroid        += tmp ;
                D2JetPt[i]       = jetvectors[i].pt() ;
            }
            Centroid /= (FloatType) D2JetVectors.size() ;

            // Centralize the vectors:
            for (size_t i=0;i<D2JetVectors.size();i++) { D2JetVectors[i] -= Centroid ; }
            for (size_t i=0;i<3;i++) { BasisVectors[i] -= Centroid ; }

            // Perform Gram Smidt Orthonormalization:
            BasisVectors[0] = BasisVectors[0].dir() ;
            BasisVectors[1] = (
                BasisVectors[1] -
                ( BasisVectors[0] *
                    ( BasisVectors[0] * BasisVectors[1] )
                )
            ).dir() ;

            // Show it for debugging purposes:
            //printf("DEBUG: Printing basis vectors: \n");
            //printf("DEBUG:     %e %e \n",BasisVectors[0][0],BasisVectors[0][1]);
            //printf("DEBUG:     %e %e \n",BasisVectors[1][0],BasisVectors[1][1]);

            // Now Evaluate the maps:
            FloatType constexpr LimitX = 1.6 ;
            FloatType constexpr LimitY = 1.6 ;
            for (size_t i=0;i<D2JetVectors.size();i++) {
                FloatType y =   (D2JetVectors[i]*BasisVectors[0]) ;
                FloatType x = - (D2JetVectors[i]*BasisVectors[1]) ;
                long X = IntMap (
                    (long)      /* Y2 = */ InputArrayType::SizeX() ,
                    (FloatType) /* X2 = */ LimitX ,
                    (FloatType) /* X  = */ x
                ) ;
                long Y = IntMap (
                    (long)      /* Y2 = */ InputArrayType::SizeY() ,
                    (FloatType) /* X2 = */ LimitY ,
                    (FloatType) /* X  = */ y
                ) ;
                if (((0<=Y)&&(Y<40))&&((0<=X)&&(X<40)))
                {MainStore_SHIH[Y][X]+=D2JetPt[i];}
            }

            // Normalize the image:
            auto norm = MainStore_SHIH.L1Norm() + NewHEPHeaders::SMALL_EPSILON;
            //printf("DEBUG: %e",norm);
            for(size_t y=0;y<MainStore_SHIH.SizeY();y++)
            for(size_t x=0;x<MainStore_SHIH.SizeX();x++)
            {MainStore_SHIH[y][x]/=norm;}
            //printf(" %e\n",MainStore_SHIH.L1Norm());

        }
        inline void Reader (fastjet::PseudoJet & injet) {
            eventpass = false ;
            HTT       = false ;
            FullJet   = injet ;
            if ( CPPFileIO::mymod(injet.eta()) < 2.0 ) {
                Process_My_Method (injet) ;
                if (eventpass) {Process_Shih_Method(injet);}
            }
        }
    public:
        inline FloatType mass () {return FullJet.m();}
        inline bool pass_mass () {
            FloatType tmpmass = mass();
            //return eventpass &&
                //(400.0<tmpmass) &&
                //(tmpmass<500.0) ;
            //return eventpass&&(150.0<tmpmass);
            //return eventpass&&((150.0<tmpmass)&&(tmpmass<200.0));
            return eventpass;
        }
        inline bool pass () {
            FloatType tmpmass = mass();
            //return eventpass &&
                //(450.0<tmpmass) &&
                //(tmpmass<500.0) ;
            //return eventpass&&(150.0<tmpmass);
            //return eventpass&&((150.0<tmpmass)&&(tmpmass<200.0));
            return eventpass;
        }
        inline bool pass_shih () {
            bool normpass = pass () &&
                (0.5<MainStore_SHIH.L1Norm()) &&
                (MainStore_SHIH.L1Norm()<1.5) ;
            return normpass ;
        }
        inline bool pass_shih_mass () {
            bool normpass = pass_mass () &&
                (0.5<MainStore_SHIH.L1Norm()) &&
                (MainStore_SHIH.L1Norm()<1.5) ;
            return normpass ;
        }
        inline void PrintOut (FILE * f) {
            for(size_t y=0;y<InputArrayType::SizeY();y++){
                for(size_t x=0;x<InputArrayType::SizeX();x++) {
                    fprintf(f,"%e",(FloatType)MainStore[y][x]);
                    bool donespace =
                        (y==(InputArrayType::SizeY()-1)) &&
                        (x==(InputArrayType::SizeX()-1)) ;
                    if(!donespace) {fprintf(f," ");}
                }
            }
        }
        inline void operator () (TH2F & inhist) {
            for (size_t y=0;y<InputArrayType::SizeY();y++) {
                for (size_t x=0;x<InputArrayType::SizeX();x++)
                { FloatType e = MainStore[y][x]; inhist.Fill(x,y,e); }
            }
        }
        inline void operator () (TH2F & inhist1, TH2F & inhist2) {
            for(size_t y=0;y<InputArrayType::SizeY();y++){
                for(size_t x=0;x<InputArrayType::SizeX();x++) {
                    FloatType e1 = MainStore[y][x];
                    FloatType e2 = MainStore_SHIH[y][x];
                    inhist1.Fill(x,y,e1);
                    inhist2.Fill(x,y,e2);
                }
            }
        }
        inline InputArrayType & GetArray () { return MainStore ; }
        inline void operator () (FlatArrayType & in) {
            typedef FlatArrayType::TYPE_Data TYPE_Data ;
            TYPE_Data * Dst = (TYPE_Data*) in.GET_DATA        () ;
            TYPE_Data * Src = (TYPE_Data*) MainStore.GET_DATA () ;
            for (size_t i=0;i<FlatArrayType::Size();i++) { Dst[i] = Src[i] ; }
        }
        inline void operator () (fastjet::PseudoJet & injet) { Reader (injet) ; }
        QCDPreProcessor_SHIH    ()                           {}
        QCDPreProcessor_SHIH    (fastjet::PseudoJet & injet) { Reader (injet) ; }
        ~QCDPreProcessor_SHIH   ()                           {}
    } ;

    typedef QCDPreProcessor_SHIH QCDPreProcessor ;

    class My_Image_Writer {
    public:

        CPPFileIO::FileVector
            <InputArrayType>
                Writer_Image
        ; //

        CPPFileIO::FileVector
            <InputArrayType>
                Writer_ImageShih
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Mass
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Energy
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Tau0
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Tau1
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Tau2
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Tau3
        ; //

        CPPFileIO::FileVector
            <TYPE_DATA>
                Writer_Tau4
        ; //

        inline void operator () (std::string infilename) {
            CPPFileIO::FullFileReader
                <QCDPreProcessor>
                    Reader(
                        infilename
                    )
            ; //
            for(size_t i=0;i<Reader();i++)if(Reader(i).eventpass){
                Writer_Image.push_back(
                    Reader(i).MainStore
                ) ; //
                Writer_ImageShih.push_back(
                    Reader(i).MainStore_SHIH
                ) ; //
                Writer_Mass.push_back(
                    Reader(i).FullJet.m()
                ) ; //
                Writer_Energy.push_back(
                    Reader(i).FullJet[3]
                ) ; //
                Writer_Tau0.push_back(
                    Reader(i).Tau[0]
                ) ; //
                Writer_Tau1.push_back(
                    Reader(i).Tau[1]
                ) ; //
                Writer_Tau2.push_back(
                    Reader(i).Tau[2]
                ) ; //
                Writer_Tau3.push_back(
                    Reader(i).Tau[3]
                ) ; //
                Writer_Tau4.push_back(
                    Reader(i).Tau[4]
                ) ; //
            }
        }

        My_Image_Writer(
            std::string OutFileName
        ) : Writer_Image(OutFileName+".images") ,
        Writer_Mass(OutFileName+".masses") ,
        Writer_Energy(OutFileName+".energies") ,
        Writer_Tau0(OutFileName+".tau0") ,
        Writer_Tau1(OutFileName+".tau1") ,
        Writer_Tau2(OutFileName+".tau2") ,
        Writer_Tau3(OutFileName+".tau3") ,
        Writer_Tau4(OutFileName+".tau4") ,
        Writer_ImageShih(OutFileName+".shihimages") {}

        ~My_Image_Writer(){}

    } ;

    class TxTWriter {
    public:
        std::string outfilename ;
        FILE * f ;
        inline void operator () (std::string inname) {
            CPPFileIO::FileArray <QCDPreProcessor> reader(inname);
            size_t limit = reader.size() ;
            QCDPreProcessor * Ptr = &(reader(0,limit)) ;
            for(size_t i=0;(i<limit)&&(i<1000000);i++)
            if(Ptr[i].pass()) {Ptr[i].PrintOut(f);fprintf(f,"\n");}
        }

        TxTWriter(std::string _outfilename):
        outfilename(_outfilename)
        {f=fopen(&(outfilename[0]),"w");}

        ~TxTWriter(){fclose(f);}
    } ;
    class Histogrammer {
    public:
        std::string outfilename ;
        TFile * outfile ;
        TH2F * hist[2] ;
        inline void operator () (std::string inname) {
            CPPFileIO::FileArray <QCDPreProcessor> reader(inname);
            size_t limit = reader.size() ;
            QCDPreProcessor * Ptr = &(reader(0,limit)) ;
            for(size_t i=0;(i<limit)&&(i<1000000);i++){
                QCDPreProcessor & tmp = Ptr[i] ;
                if(tmp.pass()){tmp(hist[0][0],hist[1][0]);}
            }
        }
        Histogrammer(std::string outfilename){
            outfile = new TFile(&(outfilename[0]),"RECREATE");
            hist[0] = new TH2F (
                "image","image",
                InputArrayType::SizeX(),0,InputArrayType::SizeX(),
                InputArrayType::SizeY(),0,InputArrayType::SizeY()
            ) ;
            hist[1] = new TH2F (
                "image_SHIH","image_SHIH",
                InputArrayType::SizeX(),0,InputArrayType::SizeX(),
                InputArrayType::SizeY(),0,InputArrayType::SizeY()
            ) ;
        }
        ~Histogrammer(){
            hist[0]->Write(); delete hist[0] ;
            hist[1]->Write(); delete hist[1] ;
            delete outfile ;
        }
    } ;
}

inline void MakeFiles () {
    /* Make images from QCD SEPARATE Sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD_SEPARATE");
        writer("./OUTS/QCD_SEPARATE/out.8.dat");
        writer("./OUTS/QCD_SEPARATE/out.9.dat");
        writer("./OUTS/QCD_SEPARATE/out.10.dat");
        writer("./OUTS/QCD_SEPARATE/out.11.dat");
        writer("./OUTS/QCD_SEPARATE/out.12.dat");
        writer("./OUTS/QCD_SEPARATE/out.13.dat");
        writer("./OUTS/QCD_SEPARATE/out.14.dat");
        writer("./OUTS/QCD_SEPARATE/out.15.dat");
        writer("./OUTS/QCD_SEPARATE/out.0.dat");
        writer("./OUTS/QCD_SEPARATE/out.1.dat");
        writer("./OUTS/QCD_SEPARATE/out.2.dat");
        writer("./OUTS/QCD_SEPARATE/out.3.dat");
        writer("./OUTS/QCD_SEPARATE/out.4.dat");
        writer("./OUTS/QCD_SEPARATE/out.5.dat");
        writer("./OUTS/QCD_SEPARATE/out.6.dat");
        writer("./OUTS/QCD_SEPARATE/out.7.dat");
    }
    /* Make images from QCD MASSIVE Sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD_MASSIVE");
        writer("./OUTS/QCD_MASSIVE/0");
        writer("./OUTS/QCD_MASSIVE/1");
        writer("./OUTS/QCD_MASSIVE/2");
        writer("./OUTS/QCD_MASSIVE/3");
        writer("./OUTS/QCD_MASSIVE/4");
        writer("./OUTS/QCD_MASSIVE/5");
        writer("./OUTS/QCD_MASSIVE/6");
        writer("./OUTS/QCD_MASSIVE/7");
    }
    /* Make images from QCD SEPARATE Sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/Z_JETS");
        writer("./OUTS/Z_JETS/out.0.dat");
        writer("./OUTS/Z_JETS/out.1.dat");
        writer("./OUTS/Z_JETS/out.2.dat");
        writer("./OUTS/Z_JETS/out.3.dat");
        writer("./OUTS/Z_JETS/out.4.dat");
        writer("./OUTS/Z_JETS/out.5.dat");
        writer("./OUTS/Z_JETS/out.6.dat");
        writer("./OUTS/Z_JETS/out.7.dat");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD");
        writer("./OUTS/DATA/NO_MPI/NO_DET/QCD/0");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/TOP");
        writer("./OUTS/DATA/NO_MPI/NO_DET/TOP/0");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/WBS");
        writer("./OUTS/DATA/NO_MPI/NO_DET/WBS/0");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD_TESTING_2");
        writer("./OUTS/QCD_TESTING_2/out.0.dat");
        writer("./OUTS/QCD_TESTING_2/out.1.dat");
        writer("./OUTS/QCD_TESTING_2/out.2.dat");
        writer("./OUTS/QCD_TESTING_2/out.3.dat");
        writer("./OUTS/QCD_TESTING_2/out.4.dat");
        writer("./OUTS/QCD_TESTING_2/out.5.dat");
        writer("./OUTS/QCD_TESTING_2/out.6.dat");
        writer("./OUTS/QCD_TESTING_2/out.7.dat");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer ("./OUTS/H4J") ;
        writer ("./OUTS/DATA/NO_MPI/NO_DET/H4J/0") ;
    }
    /* Make images from QCD ORIGINAL sample */ if(true) {
        Datas::My_Image_Writer writer("./OUTS/QCD_DET");
        writer("./OUTS/DATA/NO_MPI/DET/QCD/0");
        writer("./OUTS/DATA/NO_MPI/DET/QCD/1");
        writer("./OUTS/DATA/NO_MPI/DET/QCD/2");
    }
    /* Make images from QCD ORIGINAL sample */ if(true) {
        Datas::My_Image_Writer writer("./OUTS/QCD_DET_TEST");
        writer("./OUTS/DATA/NO_MPI/DET/QCD/3");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/TOP_DET");
        writer("./OUTS/DATA/NO_MPI/DET/TOP/0");
        writer("./OUTS/DATA/NO_MPI/DET/TOP/1");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD_FULL_TRAIN");
        writer("./OUTS/DATA/MPI/DET/QCD/0");
        writer("./OUTS/DATA/MPI/DET/QCD/1");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/QCD_FULL_TEST");
        writer("./OUTS/DATA/MPI/DET/QCD/2");
    }
    /* Make images from QCD ORIGINAL sample */ if(false) {
        Datas::My_Image_Writer writer("./OUTS/TOP_FULL");
        writer("./OUTS/DATA/MPI/DET/TOP/0");
    }
}

class UnifiedData {
public:
    typedef CPPFileIO::FullFileReader
        <Datas::QCDPreProcessor> TYPE_Output
    ; //
    TYPE_Output QCD, TOP, WBS, H4J ;
    //
    inline bool H4J_PASS
    (size_t i) {
        return H4J(i)
            .pass() ;
    }
    //
    inline bool WBS_PASS
    (size_t i) {
        return WBS(i)
            .pass() ;
    }
    //
    inline bool TOP_PASS
    (size_t i) {
        return TOP(i)
            .pass() ;
    }
    //
    inline bool QCD_PASS
    (size_t i) {
        return QCD(i)
            .pass_mass() ;
    }
    //
    inline bool H4J_PASS_SHIH
    (size_t i) {
        return H4J(i)
            .pass_shih() ;
    }
    //
    inline bool WBS_PASS_SHIH
    (size_t i) {
        return WBS(i)
            .pass_shih() ;
    }
    //
    inline bool TOP_PASS_SHIH
    (size_t i) {
        return TOP(i)
            .pass_shih() ;
    }
    //
    inline bool QCD_PASS_SHIH
    (size_t i) {
        return QCD(i)
            .pass_shih_mass() ;
    }
    //
    inline size_t H4J_SIZE ()
        { return H4J() ; }
    //
    inline size_t WBS_SIZE ()
        { return WBS() ; }
    //
    inline size_t QCD_SIZE ()
        { return QCD() ; }
    //
    inline size_t TOP_SIZE ()
        { return TOP() ; }
    //
    UnifiedData() :
    QCD("./OUTS/QCD.united") ,
    WBS("./OUTS/WBS.united") ,
    H4J("./OUTS/H4J.united") ,
    TOP("./OUTS/TOP.united") {}
    //
    static inline UnifiedData
    GET () {
        UnifiedData ret ;
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_QCD () {
        TYPE_Output ret
            ("./OUTS/QCD.united") ;
        //
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_TOP () {
        TYPE_Output ret
            ("./OUTS/TOP.united") ;
        //
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_WBS () {
        TYPE_Output ret
            ("./OUTS/WBS.united");
        //
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_H4J () {
        TYPE_Output ret
            ("./OUTS/H4J.united");
        //
        return ret ;
    }
    //
} ;
class UnifiedDataDet {
public:
    typedef CPPFileIO::FullFileReader
        <Datas::QCDPreProcessor> TYPE_Output ;
    //
    TYPE_Output QCD, TOP ;
    //
    inline bool TOP_PASS
    (size_t i) {
        return TOP(i)
            .pass() ;
    }
    //
    inline bool QCD_PASS
    (size_t i) {
        return QCD(i)
            .pass_mass() ;
    }
    //
    inline bool TOP_PASS_SHIH
    (size_t i) {
        return TOP(i)
            .pass_shih() ;
    }
    //
    inline bool QCD_PASS_SHIH
    (size_t i) {
        return QCD(i)
            .pass_shih_mass() ;
    }
    //
    inline size_t QCD_SIZE ()
        { return QCD() ; }
    //
    inline size_t TOP_SIZE ()
        { return TOP() ; }
    //
    UnifiedDataDet() :
    QCD("./OUTS/QCD_DET.united") ,
    TOP("./OUTS/TOP_DET.united") {}
    //
    static inline UnifiedData
    GET () {
        UnifiedData ret ;
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_QCD () {
        TYPE_Output ret
            ("./OUTS/QCD_DET.united") ;
        //
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_TOP () {
        TYPE_Output ret
            ("./OUTS/TOP_DET.united") ;
        //
        return ret ;
    }
    //
} ;
class UnifiedDataFull {
public:
    typedef CPPFileIO::FullFileReader
        <Datas::QCDPreProcessor> TYPE_Output ;
    //
    TYPE_Output QCD, TOP ;
    //
    inline bool TOP_PASS
    (size_t i) {
        return TOP(i)
            .pass() ;
    }
    //
    inline bool QCD_PASS
    (size_t i) {
        return QCD(i)
            .pass_mass() ;
    }
    //
    inline bool TOP_PASS_SHIH
    (size_t i) {
        return TOP(i)
            .pass_shih() ;
    }
    //
    inline bool QCD_PASS_SHIH
    (size_t i) {
        return QCD(i)
            .pass_shih_mass() ;
    }
    //
    inline size_t QCD_SIZE ()
        { return QCD() ; }
    //
    inline size_t TOP_SIZE ()
        { return TOP() ; }
    //
    UnifiedDataFull() :
    QCD("./OUTS/QCD_FULL.united") ,
    TOP("./OUTS/TOP_FULL.united") {}
    //
    static inline UnifiedData
    GET () {
        UnifiedData ret ;
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_QCD () {
        TYPE_Output ret
            ("./OUTS/QCD_FULL.united") ;
        //
        return ret ;
    }
    //
    static inline TYPE_Output
    GET_TOP () {
        TYPE_Output ret
            ("./OUTS/TOP_FULL.united") ;
        //
        return ret ;
    }
    //
} ;
inline void JetImages (
    std::string imagefilename,
    std::string outrootname
) {
    CPPFileIO::FullFileReader <InputArrayType> reader (imagefilename) ;
    TFile outrootfile (&(outrootname[0]),"RECREATE") ;
    /* Making the image histogram */ {
        TH2F JetImage(
            "image","image",
            InputArrayType::SizeX(),0,InputArrayType::SizeX(),
            InputArrayType::SizeY(),0,InputArrayType::SizeY()
        );
        for(size_t i=0;i<reader();i++){
            for(size_t y=0;y<InputArrayType::SizeY();y++){
                for(size_t x=0;x<InputArrayType::SizeX();x++){
                    JetImage.Fill(x,y,reader(i)[y][x]);
                }
            }
        }
        JetImage.Write();
    }
}
namespace GeneralHistogram {
    namespace Cards {
        template <size_t N> class NSub {
        public:
            static inline void Name (char * buf) {sprintf(buf,"NSub%ld",N);}
            static inline size_t NBins () { return 100   ; }
            static inline double XMin  () { return  -0.1 ; }
            static inline double XMax  () { return  10.1 ; }
            static inline double Eval  (Datas::QCDPreProcessor const & in) {return in.Tau[N-1];}
        } ;
        template <size_t M, size_t N> class NSubRatio {
        public:
            static inline void Name (char * buf) {sprintf(buf,"NSub%ld_BY_NSub%ld",M,N);}
            static inline size_t NBins () {return 100   ; }
            static inline double XMin  () {return  -0.1 ; }
            static inline double XMax  () {return   1.1 ; }
            static inline double Eval  (Datas::QCDPreProcessor const & in) {return in.Tau[M-1]/in.Tau[N-1];}
        } ;
    }
    template <typename T> inline void PlotVar () {
        char tmpbuf[128] ;
        T::Name(tmpbuf);

        NewHEPHeaders::Histograms::MyHistN <3> filler
        (tmpbuf,T::NBins(),T::XMin(),T::XMax());

        for(size_t i=0;i<8;i++){
            sprintf(tmpbuf,"./OUTS/QCD/%ld",i);
            CPPFileIO::FullFileReader <Datas::QCDPreProcessor>
                Reader (tmpbuf) ;
            for(size_t j=0;j<Reader();j++)
            { filler.Fill<0>(T::Eval(Reader(j))); }
        }
        for(size_t i=0;i<8;i++){
            sprintf(tmpbuf,"./OUTS/WBS/%ld",i);
            CPPFileIO::FullFileReader <Datas::QCDPreProcessor>
                Reader (tmpbuf) ;
            for(size_t j=0;j<Reader();j++)
            { filler.Fill<1>(T::Eval(Reader(j))); }
        }
        for(size_t i=0;i<8;i++){
            sprintf(tmpbuf,"./OUTS/TOP/%ld",i);
            CPPFileIO::FullFileReader <Datas::QCDPreProcessor>
                Reader (tmpbuf) ;
            for(size_t j=0;j<Reader();j++)
            { filler.Fill<2>(T::Eval(Reader(j))); }
        }
    }
    inline void PlotAll () {
        PlotVar<Cards::NSub<1>>();
        PlotVar<Cards::NSub<2>>();
        PlotVar<Cards::NSub<3>>();
        PlotVar<Cards::NSub<4>>();
        PlotVar<Cards::NSub<5>>();
        PlotVar<Cards::NSubRatio<2,1>>();
        PlotVar<Cards::NSubRatio<3,2>>();
        PlotVar<Cards::NSubRatio<4,3>>();
        PlotVar<Cards::NSubRatio<5,4>>();
    }
}
namespace Generator {
    namespace QCD_MASSIVE {
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            Pythia8::Pythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "HardQCD:all = on"           ) ;
                pythia.readString ( "PartonLevel:MPI = off"      ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 10000" ) ;
                pythia.readString ( "Beams:eCM = 100000"          ) ;
                pythia.rndm.init  ( i + 1                        ) ;
                pythia.init();
            }
            char tmpbuf[512] ; {
                MakeDirectories();
                sprintf(
                    tmpbuf,
                    "./OUTS/QCD_MASSIVE/%ld",
                    i
                ) ; //
            }
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable =
                        (mpid!=12) && (mpid!=14) &&
                        (mpid!=16) && (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0) ;
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++){
                        QCDPreProcessor processor (jets[ii]) ;
                        if(processor.eventpass)
                        {Writer.push_back(processor);}
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<2000*SCALE>(i);
            }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            /* PARTONIC: */ {
                Histogrammer slave ("./OUTS/QCD_M.root") ;
                slave("./OUTS/QCD_MASSIVE/0");
                slave("./OUTS/QCD_MASSIVE/1");
                slave("./OUTS/QCD_MASSIVE/2");
                slave("./OUTS/QCD_MASSIVE/3");
                slave("./OUTS/QCD_MASSIVE/4");
                slave("./OUTS/QCD_MASSIVE/5");
                slave("./OUTS/QCD_MASSIVE/6");
                slave("./OUTS/QCD_MASSIVE/7");
            }
            /* DELPHES: */ {
                //Histogrammer slave ("./OUTS/QCD_DET.root") ;
                //slave("./OUTS/QCD_DET.united");
            }

        }
    }
    namespace QCD {
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            Pythia8::Pythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "HardQCD:all = on"          ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 1                       ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            { MakeDirectories(); Path_QCD(tmpbuf,i); }
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable =
                        (mpid!=12) && (mpid!=14) &&
                        (mpid!=16) && (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0) ;
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    QCDPreProcessor processor (jets[ii]) ;
                    if(processor.eventpass){Writer.push_back(processor);}
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<600*SCALE>(i);
            }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            /* PARTONIC: */ {
                Histogrammer slave ("./OUTS/QCD.root") ;
                slave("./OUTS/QCD.united");
            }
            /* DELPHES: */ {
                Histogrammer slave ("./OUTS/QCD_DET.root") ;
                slave("./OUTS/QCD_DET.united");
            }

        }
    }

    namespace QCD_SEPARATE {
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            mkdir ("./OUTS/QCD_SEPARATE",0755) ;
            Pythia8::Pythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "HardQCD:all = on"          ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 12345                   ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            sprintf(tmpbuf,"./OUTS/QCD_SEPARATE/out.%ld.dat",i);
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable =
                        (mpid!=12) && (mpid!=14) &&
                        (mpid!=16) && (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0)
                    ; //
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    QCDPreProcessor processor (jets[ii]) ;
                    if(processor.eventpass){Writer.push_back(processor);}
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<4000*SCALE>(i);
            }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            /* PARTONIC: */ {
                Histogrammer slave ("./OUTS/QCD.root") ;
                slave("./OUTS/QCD.united");
            }
            /* DELPHES: */ {
                Histogrammer slave ("./OUTS/QCD_DET.root") ;
                slave("./OUTS/QCD_DET.united");
            }
        }
    }

    namespace QCD_TESTING_2 {
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            mkdir ("./OUTS/QCD_TESTING_2",0755) ;
            Pythia8::Pythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "HardQCD:all = on"          ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 5678                    ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            sprintf(tmpbuf,"./OUTS/QCD_TESTING_2/out.%ld.dat",i);
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable =
                        (mpid!=12) && (mpid!=14) &&
                        (mpid!=16) && (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0)
                    ; //
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    QCDPreProcessor processor (jets[ii]) ;
                    if(processor.eventpass){Writer.push_back(processor);}
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<500*SCALE>(i);
            }
        }
    }

    namespace Z_JETS {
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            mkdir ("./OUTS/Z_JETS",0755) ;
            Pythia8::Pythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "WeakZ0:gmZmode = 2"        ) ;
                pythia.readString ( "WeakBosonAndParton:qqbar2gmZg = on" ) ;
                pythia.readString ( "WeakBosonAndParton:qg2gmZq = on" ) ;
                pythia.readString ( "23:onMode = off" ) ;
                pythia.readString ( "23:onIfAny = 12 -12" ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 1                       ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            sprintf(tmpbuf,"./OUTS/Z_JETS/out.%ld.dat",i);
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable =
                        (mpid!=12) && (mpid!=14) &&
                        (mpid!=16) && (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0)
                    ; //
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    QCDPreProcessor processor (jets[ii]) ;
                    if(processor.eventpass){Writer.push_back(processor);}
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<500*SCALE>(i);
            }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            /* PARTONIC: */ {
                Histogrammer slave ("./OUTS/QCD.root") ;
                slave("./OUTS/QCD.united");
            }
            /* DELPHES: */ {
                Histogrammer slave ("./OUTS/QCD_DET.root") ;
                slave("./OUTS/QCD_DET.united");
            }
        }
    }



    namespace TOP {
        class MyPythia : public Pythia8::Pythia {
        public:
            inline long FindParticle (long pid) {
                long ret = 0 ;
                for(long i=0;(i<this->event.size())&&(ret==0);i++)
                if(this->event[i].id()==pid){ret=i;}
                return ret ;
            }
            inline long Recurse (long index) {
                auto ppid     = this->event[index].id () ;
                long dt   [2] = { this->event[index].daughter1 () , this->event[index].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id        () , this->event[dt[1]].id        () } ;
                if      (dpid[0]==ppid) { return Recurse (dt[0]) ; }
                else if (dpid[1]==ppid) { return Recurse (dt[1]) ; }
                else                    { return index           ; }
            }
            inline long FindDaughter (long ParentID, long DPID) {
                ParentID = Recurse (ParentID) ;
                long dt   [2] = { this->event[ParentID].daughter1 () , this->event[ParentID].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id           () , this->event[dt[1]].id           () } ;
                if      (dpid[0]==DPID) { return dt[0] ; }
                else if (dpid[1]==DPID) { return dt[1] ; }
                else                    { return 0     ; }
            }
            inline Vector4 GetVector (long id)
            { return Vector4 (this->event[id].px(),this->event[id].py(),this->event[id].pz(),this->event[id].e()) ; }
        } ;
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            MyPythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "Top:gg2ttbar = on"         ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "24:onMode = off"           ) ;
                pythia.readString ( "24:onIfAny = 1 2 3 4"      ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 1                       ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            { MakeDirectories(); Path_TOP(tmpbuf,i); }
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                Vector4 TOP[2] , WBS[2] , BOT[2] , JT1[2] , JT2[2] ; /* Get These Vectors: */ {
                    long tid[2] = {
                        pythia.Recurse ( pythia.FindParticle (  6 ) ),
                        pythia.Recurse ( pythia.FindParticle ( -6 ) )
                    } ;
                    { TOP[0] = pythia.GetVector(tid[0]); TOP[1] = pythia.GetVector(tid[1]); }
                    long wid[2] = {
                        pythia.Recurse ( pythia.FindDaughter ( (long) /*ParentID=*/ tid[0] , (long) /*DPID=*/  24 ) ) ,
                        pythia.Recurse ( pythia.FindDaughter ( (long) /*ParentID=*/ tid[1] , (long) /*DPID=*/ -24 ) )
                    } ;
                    { WBS[0] = pythia.GetVector(wid[0]); WBS[1] = pythia.GetVector(wid[1]); }
                    long j1id[2] = {
                        pythia.event[wid[0]].daughter1 () ,
                        pythia.event[wid[1]].daughter1 ()
                    } ;
                    { JT1[0] = pythia.GetVector(j1id[0]); JT1[1] = pythia.GetVector(j1id[1]); }
                    long j2id[2] = {
                        pythia.event[wid[0]].daughter2 () ,
                        pythia.event[wid[1]].daughter2 ()
                    } ;
                    { JT2[0] = pythia.GetVector(j2id[0]); JT2[1] = pythia.GetVector(j2id[1]); }
                    long bid[2] = {
                        pythia.FindDaughter ( (long) /*ParentID=*/ tid[0] , (long) /*DPID=*/  5 ) ,
                        pythia.FindDaughter ( (long) /*ParentID=*/ tid[1] , (long) /*DPID=*/ -5 )
                    } ;
                    { BOT[0] = pythia.GetVector(bid[0]); BOT[1] = pythia.GetVector(bid[1]); }
                }
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable = (mpid!=12) && (mpid!=14) && (mpid!=16) &&
                        (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0) ;
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    bool goodevent = false ;
                    Vector4 tmpjet = jets [ii] ;
                    for (size_t kk=0;kk<2;kk++) {
                        goodevent =
                            goodevent || (
                                (tmpjet(TOP[kk])<0.6) &&
                                (tmpjet(BOT[kk])<0.6) &&
                                (tmpjet(JT1[kk])<0.6) &&
                                (tmpjet(JT2[kk])<0.6)
                            ) ;
                    }
                    if (goodevent) {
                        //printf("Passed a top jet...\n");
                        QCDPreProcessor processor (jets[ii]) ;
                        if(processor.eventpass)
                        {Writer.push_back(processor);}
                    }
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<1000*SCALE>(i);
            }
        }
        inline void WriteTxt () {
            MakeDirectories () ; TxTWriter    slave ("./OUTS/TOP.txt") ;
            for ( size_t i=0 ; i<8 ; i++ )
            { char tmpbuf[128]; Path_TOP(tmpbuf,i); slave(tmpbuf); }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            /* PARTONIC: */ {
                Histogrammer slave ("./OUTS/TOP.root") ;
                slave("./OUTS/TOP.united");
            }
            /* DELPHES: */ {
                Histogrammer slave ("./OUTS/TOP_DET.root") ;
                slave("./OUTS/TOP_DET.united");
            }
        }
    }
    namespace WBS {
        class MyPythia : public Pythia8::Pythia {
        public:
            inline long FindParticle (long pid) {
                long ret = 0 ;
                for(long i=0;(i<this->event.size())&&(ret==0);i++)
                if(this->event[i].id()==pid){ret=i;}
                return ret ;
            }
            inline long FindParticleMod (long pid) {
                pid = CPPFileIO::mymod(pid);
                long ret = 0 ;
                for(long i=0;(i<this->event.size())&&(ret==0);i++)
                if(CPPFileIO::mymod(this->event[i].id())==pid){ret=i;}
                return ret ;
            }
            inline long Recurse (long index) {
                auto ppid     = this->event[index].id () ;
                long dt   [2] = { this->event[index].daughter1 () , this->event[index].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id        () , this->event[dt[1]].id        () } ;
                if      (dpid[0]==ppid) { return Recurse (dt[0]) ; }
                else if (dpid[1]==ppid) { return Recurse (dt[1]) ; }
                else                    { return index           ; }
            }
            inline long FindDaughter (long ParentID, long DPID) {
                ParentID = Recurse (ParentID) ;
                long dt   [2] = { this->event[ParentID].daughter1 () , this->event[ParentID].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id           () , this->event[dt[1]].id           () } ;
                if      (dpid[0]==DPID) { return dt[0] ; }
                else if (dpid[1]==DPID) { return dt[1] ; }
                else                    { return 0     ; }
            }
            inline Vector4 GetVector (long id)
            { return Vector4 (this->event[id].px(),this->event[id].py(),this->event[id].pz(),this->event[id].e()) ; }
        } ;
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            MyPythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "WeakDoubleBoson:ffbar2ZW = on" ) ;
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "23:onMode = off"           ) ;
                pythia.readString ( "23:onIfAny = 12"      ) ;
                pythia.readString ( "24:onMode = off"           ) ;
                pythia.readString ( "24:onIfAny = 1 2 3 4"      ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 1                       ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            { MakeDirectories(); Paths::Path_WBS(tmpbuf,i); }
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                Vector4 WBS , JT[2] ; /* Get These Vectors: */ {
                    long wid = pythia.Recurse ( pythia.FindParticleMod ( 24 ) ) ;
                    WBS = pythia.GetVector(wid);
                    long j1id[2] = {
                        pythia.event[wid].daughter1 () ,
                        pythia.event[wid].daughter1 ()
                    } ;
                    JT[0] = pythia.GetVector(j1id[0]);
                    JT[1] = pythia.GetVector(j1id[1]);
                }
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable = (mpid!=12) && (mpid!=14) && (mpid!=16) &&
                        (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0) ;
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    bool goodevent = false ;
                    Vector4 tmpjet = jets [ii] ;
                    goodevent = (tmpjet(JT[0])<0.6) && (tmpjet(JT[1])<0.6) ;
                    if (goodevent) {
                        //printf("Passed a top jet...\n");
                        QCDPreProcessor processor (jets[ii]) ;
                        if(processor.eventpass)
                        {Writer.push_back(processor);}
                    }
                }
            }
        }
        inline void Generate () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<1000*SCALE>(i);
            }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            Histogrammer slave ("./OUTS/WBS.root") ;
            slave("./OUTS/WBS.united");
        }
    }
    namespace H4J {
        class MyPythia : public Pythia8::Pythia {
        public:
            inline long FindParticle (long pid) {
                long ret = 0 ;
                for(long i=0;(i<this->event.size())&&(ret==0);i++)
                if(this->event[i].id()==pid){ret=i;}
                return ret ;
            }
            inline long Recurse (long index) {
                auto ppid     = this->event[index].id () ;
                long dt   [2] = { this->event[index].daughter1 () , this->event[index].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id        () , this->event[dt[1]].id        () } ;
                if      (dpid[0]==ppid) { return Recurse (dt[0]) ; }
                else if (dpid[1]==ppid) { return Recurse (dt[1]) ; }
                else                    { return index           ; }
            }
            inline long FindDaughter (long ParentID, long DPID) {
                ParentID = Recurse (ParentID) ;
                long dt   [2] = { this->event[ParentID].daughter1 () , this->event[ParentID].daughter2 () } ;
                long dpid [2] = { this->event[dt[0]].id           () , this->event[dt[1]].id           () } ;
                if      (dpid[0]==DPID) { return dt[0] ; }
                else if (dpid[1]==DPID) { return dt[1] ; }
                else                    { return 0     ; }
            }
            inline Vector4 GetVector (long id)
            { return Vector4 (this->event[id].px(),this->event[id].py(),this->event[id].pz(),this->event[id].e()) ; }
        } ;
        using namespace Datas ;
        template <size_t GenSize=10000> inline void Generate (size_t i) {
            MyPythia pythia ; /* initialize the pythia object: */ {
                pythia.readString ( "PartonLevel:MPI = off"     ) ;
                pythia.readString ( "Higgs:useBSM = on"         ) ;
                pythia.readString ( "HiggsBSM:ffbar2H2Z = on"   ) ;
                //pythia.readString ( "PartonLevel:ISR = off"     ) ;
                pythia.readString ( "35:m0 = 180.0"             ) ;
                pythia.readString ( "35:onMode = off"           ) ;
                pythia.readString ( "35:onIfAny = 24 24"        ) ;
                pythia.readString ( "23:onMode = off"           ) ;
                pythia.readString ( "23:onIfAny = 12"           ) ;
                pythia.readString ( "24:onMode = off"           ) ;
                pythia.readString ( "24:onIfAny = 1 2 3 4"      ) ;
                pythia.readString ( "PhaseSpace:pTHatMin = 750" ) ;
                pythia.readString ( "PhaseSpace:pTHatMax = 950" ) ;
                pythia.rndm.init  ( i + 1                       ) ;
                pythia.init();
            }
            char tmpbuf[512] ;
            { MakeDirectories(); Path_TOP(tmpbuf,i); }
            CPPFileIO::FileVector <QCDPreProcessor> Writer (tmpbuf) ;
            for(size_t j=0;j<GenSize;j++) if(pythia.next()) {
                Vector4 HG2 , WBS[2] , JT1[2] , JT2[2] ; /* Get These Vectors: */ {
                    long hid = pythia.Recurse (
                        pythia.FindParticle ( 35 )
                    ) ;  HG2 = pythia.GetVector (hid) ;
                    long wid[2] = {
                        pythia.Recurse ( pythia.FindDaughter ( (long) /*ParentID=*/ hid , (long) /*DPID=*/  24 ) ) ,
                        pythia.Recurse ( pythia.FindDaughter ( (long) /*ParentID=*/ hid , (long) /*DPID=*/ -24 ) )
                    } ; {
                        WBS[0] = pythia.GetVector (wid[0]) ;
                        WBS[1] = pythia.GetVector (wid[1]) ;
                    }
                    long j1id[2] = {
                        pythia.event[wid[0]].daughter1 () ,
                        pythia.event[wid[1]].daughter1 ()
                    } ; {
                        JT1[0] = pythia.GetVector (j1id[0]) ;
                        JT1[1] = pythia.GetVector (j1id[1]) ;
                    }
                    long j2id[2] = {
                        pythia.event[wid[0]].daughter2 () ,
                        pythia.event[wid[1]].daughter2 ()
                    } ; {
                        JT2[0] = pythia.GetVector (j2id[0]) ;
                        JT2[1] = pythia.GetVector (j2id[1]) ;
                    }
                }
                NewHEPHeaders::pseudojets jetvectors ;
                for(size_t ii=0;ii<(pythia.event.size());ii++)
                if(pythia.event[ii].isFinal()) {
                    int mpid = CPPFileIO::mymod (pythia.event[ii].id()) ;
                    bool detectable = (mpid!=12) && (mpid!=14) && (mpid!=16) &&
                        (pythia.event[ii].pT()>0.5) &&
                        (CPPFileIO::mymod(pythia.event[ii].eta())<5.0) ;
                    if (detectable) {
                        jetvectors.push_back (
                            fastjet::PseudoJet (
                                pythia.event[ii].px (),
                                pythia.event[ii].py (),
                                pythia.event[ii].pz (),
                                pythia.event[ii].e  ()
                            )
                        );
                    }
                }
                fastjet::JetDefinition   definition   (fastjet::antikt_algorithm,1.0) ;
                fastjet::ClusterSequence reclusterseq (jetvectors,definition) ;
                NewHEPHeaders::pseudojets jets = reclusterseq.inclusive_jets (800.0) ;
                for(size_t ii=0;(ii<jets.size())&&(ii<1);ii++)
                if((800.0<jets[ii].pt())&&(jets[ii].pt()<900.0)) {
                    bool goodevent = false ;
                    Vector4 tmpjet = jets [ii] ;
                    for (size_t kk=0;kk<2;kk++) {
                        goodevent =
                            goodevent || (
                                (tmpjet(HG2)<0.6) &&
                                (tmpjet(WBS[kk])<0.6) &&
                                (tmpjet(JT1[kk])<0.6) &&
                                (tmpjet(JT2[kk])<0.6)
                            ) ;
                    }
                    if (goodevent) {
                        //printf("Passed a top jet...\n");
                        QCDPreProcessor processor (jets[ii]) ;
                        if(processor.eventpass)
                        {Writer.push_back(processor);}
                    }
                }
            }
        }
        inline void Generate  () {
            CPPFileIO::ForkMe forker ;
            int masks[8] = {0,1,2,3,4,5,6,7} ;
            for(size_t i=0;i<8;i++) if(forker.InKid()) {
                CPPFileIO::SetCPUAffinity(masks[i]);
                Generate<1000*SCALE>(i);
            }
        }
        inline void WriteTxt  () {
            MakeDirectories () ; TxTWriter    slave ("./OUTS/TOP.txt") ;
            for ( size_t i=0 ; i<8 ; i++ )
            { char tmpbuf[128]; Path_TOP(tmpbuf,i); slave(tmpbuf); }
        }
        inline void WriteHist () {
            MakeDirectories () ;
            Histogrammer slave ("./OUTS/H4J.root") ;
            slave("./OUTS/H4J.united");
        }
    }
}
class Array4Python {
public:
    typedef typename
        UnifiedData::TYPE_Output
            TYPE_Input ;
    //
    typedef CPPFileIO::FullFileReader
        <FlatArrayType> TYPE_Output ;
    //
    Array4Python () {}
    //
    static inline void MAKE_DET () {

        auto FullData = UnifiedDataDet::GET () ;
        /* FOR QCD: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/QCD_DET.dat" ) ;
                //
                for (size_t i=0;i<FullData.QCD();i++) {
                    WRITER.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/QCD_DET.dat" ) ;
                //
                for (size_t i=0;i<FullData.QCD();i++) {
                    WRITER.push_back (
                        FullData.QCD(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
        /* FOR TOP: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/TOP_DET.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/TOP_DET.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
    }
    //
    static inline void MAKE_FULL () {
        auto FullData = UnifiedDataFull::GET () ;
        /* FOR QCD: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/QCD_FULL.dat" ) ;
                //
                for (size_t i=0;i<FullData.QCD();i++) {
                    WRITER.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/QCD_FULL.dat" ) ;
                //
                for (size_t i=0;i<FullData.QCD();i++) {
                    WRITER.push_back (
                        FullData.QCD(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
        /* FOR TOP: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/TOP_FULL.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/TOP_FULL.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
    }
    //
    static inline void MASS_BINS_ALL_SPLIT () {
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL1 (
                    "./OUTS/QCD_M_ALL_1.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL2 (
                    "./OUTS/QCD_M_ALL_2.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL3 (
                    "./OUTS/QCD_M_ALL_3.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL4 (
                    "./OUTS/QCD_M_ALL_4.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL5 (
                    "./OUTS/QCD_M_ALL_5.dat"
                )
        ; //
        typedef CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                    TYPE_Output
        ; //
        TYPE_Output MASSIVE_QCD (
            "./OUTS/QCD_MASSIVE.dat"
        ) ; //
        for(size_t i=0;i<MASSIVE_QCD();i++){
            if(i%5==0){
                WRITER_ALL1.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            else if(i%5==1){
                WRITER_ALL2.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            else if(i%5==2){
                WRITER_ALL3.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            else if(i%5==3){
                WRITER_ALL4.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            else if(i%5==4){
                WRITER_ALL5.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
        }
    }
    //
    static inline void MAKE_MASS_BINS () {
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_ALL (
                    "./OUTS/QCD_M_ALL.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_0_200 (
                    "./OUTS/QCD_M_0_200.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_200_400 (
                    "./OUTS/QCD_M_200_400.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_400_600 (
                    "./OUTS/QCD_M_400_600.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_600_800 (
                    "./OUTS/QCD_M_600_800.dat"
                )
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
                WRITER_800_INF (
                    "./OUTS/QCD_M_800_INF.dat"
                )
        ; //
        if (false) {
            auto FullData =
                    UnifiedData::GET ()
            ; //
            for (
                size_t i=0;
                i<FullData.QCD();
                i++
            ) {
                auto mass =
                    FullData.QCD(i)
                        .FullJet.m()
                ; //
                if(
                    (0<mass) &&
                    (mass<200)
                ) {
                    WRITER_0_200.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ; //
                }
                if(
                    (200<mass) &&
                    (mass<400)
                ) {
                    WRITER_200_400.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ; //
                }
                if(
                    (400<mass) &&
                    (mass<600)
                ) {
                    WRITER_400_600.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ; //
                }
                if(
                    (600<mass) &&
                    (mass<800)
                ) {
                    WRITER_600_800.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ; //
                }
                if(
                    800<mass
                ) {
                    WRITER_800_INF.push_back (
                        FullData.QCD(i)
                            .MainStore
                    ) ; //
                }
            }
        }
        typedef CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                    TYPE_Output
        ; //
        TYPE_Output MASSIVE_QCD (
            "./OUTS/QCD_MASSIVE.dat"
        ) ; //
        for (
            size_t i=0;
            i<MASSIVE_QCD();
            i++
        ) {
            auto mass =
                MASSIVE_QCD(i)
                    .FullJet.m()
            ; //
            if(
               true
            ) {
                WRITER_ALL.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            if(
                (0<mass) &&
                (mass<200)
            ) {
                WRITER_0_200.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            if(
                (200<mass) &&
                (mass<400)
            ) {
                WRITER_200_400.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            if(
                (400<mass) &&
                (mass<600)
            ) {
                WRITER_400_600.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            if(
                (600<mass) &&
                (mass<800)
            ) {
                WRITER_600_800.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
            if(
                800<mass
            ) {
                WRITER_800_INF.push_back (
                    MASSIVE_QCD(i)
                        .MainStore
                ) ; //
            }
        }
    }
    //
    static inline void SEPARATE_QCD () {
        CPPFileIO::FullFileReader
            <Datas::QCDPreProcessor>
            Reader ("./OUTS/QCD_SEPARATE.united")
        ; //
        CPPFileIO::FileVector
            <InputArrayType>
            WRITER ( "./OUTS/QCD_SEPARATE.images" )
        ; //
        for(size_t i=0;i<Reader();i++){
            WRITER.push_back (Reader(i).MainStore);
        }
    }
    //
    static inline void MAKE () {
        auto FullData = UnifiedData::GET () ;
        /* FOR QCD: */ {
            //MAKE_MASS_BINS();
            /* My Style: */ {
                CPPFileIO::FileVector
                    <InputArrayType>
                        WRITER (
                            "./OUTS/QCD.dat"
                        )
                ; //
                for (
                    size_t i=0;
                    i<FullData.QCD();
                    i++
                ) {
                    // auto mass =
                    //     FullData.QCD(i)
                    //         .FullJet.m()
                    // ; //
                    if(
                        true
                    ) {
                        WRITER.push_back (
                            FullData.QCD(i)
                                .MainStore
                        ) ; //
                    }
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/QCD.dat" ) ;
                //
                for (size_t i=0;i<FullData.QCD();i++) {
                    WRITER.push_back (
                        FullData.QCD(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
        /* FOR TOP: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/TOP.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/TOP.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.TOP(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
        /* FOR WBS: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/WBS.dat" ) ;
                //
                for (size_t i=0;i<FullData.WBS();i++) {
                    WRITER.push_back (
                        FullData.WBS(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/WBS.dat" ) ;
                //
                for (size_t i=0;i<FullData.TOP();i++) {
                    WRITER.push_back (
                        FullData.WBS(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
        /* FOR H4J: */ {
            /* My Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/H4J.dat" ) ;
                //
                for (size_t i=0;i<FullData.H4J();i++) {
                    WRITER.push_back (
                        FullData.H4J(i)
                            .MainStore
                    ) ;
                }
            }
            /* SHIH Style: */ {
                CPPFileIO::FileVector <InputArrayType>
                    WRITER ( "./OUTS/SHIH/H4J.dat" ) ;
                //
                for (size_t i=0;i<FullData.H4J();i++) {
                    WRITER.push_back (
                        FullData.H4J(i)
                            .MainStore_SHIH
                    ) ;
                }
            }
        }
    }

    static inline TYPE_Output
    GET_QCD () {
        TYPE_Output ret (
            Paths::Path_QCD ()
        ) ;
        return ret ;
    }
    static inline TYPE_Output
    GET_TOP () {
        TYPE_Output ret (
            Paths::Path_TOP ()
        ) ;
        return ret ;
    }
    static inline TYPE_Output
    GET_QCD_SHIH () {
        TYPE_Output ret (
            Paths::Path_QCD_SHIH ()
        ) ;
        return ret ;
    }
    static inline TYPE_Output
    GET_TOP_SHIH () {
        TYPE_Output ret (
            Paths::Path_TOP_SHIH ()
        ) ;
        return ret ;
    }
} ;
namespace AutoEncoder {
    size_t constexpr BatchSize = 512 ;
    size_t constexpr Sizes[2] = {
        FlatArrayType::Size(),
        FlatArrayType::Size()
    } ;
    typedef Tensors::Arrays::N2DArray <Sizes[0],BatchSize,FloatType> TYPE_Input ;
    using namespace Tensors::NeuralNetwork::NeuralLayers ;

    typedef InputRelated::FirstLayer <TYPE_Input> L0 ;
    typedef MainLayers::NNLayerD2 <Sizes[1],L0>  NL1 ; // 1st bottleneck
    typedef MainLayers::Teacher <NL1> MainNet ;
    typedef NL1::TYPE_Parameter NP1 ;

    inline CPPFileIO::FullFileReader <TYPE_Input> MakeBatch () {
        /* The Writing part: */ {
            Tensors::Arrays::Batcher <TYPE_Input> Writer (Batch_Path_QCD()) ;
            for(size_t i=0;i<8;i++){Writer=Path_QCD(i);}
        }
        CPPFileIO::FullFileReader <TYPE_Input> ret (Batch_Path_QCD()) ;
        return ret ;
    }

    class MainTrainer {
    private:
        MainNet   Net  ;
        NP1       par1 ;
        NL1     * ref1 ;
        CPPFileIO::FullFileReader <TYPE_Input> data ;
        inline void PREPARE(){
            par1.RANDOMIZE();
            Net.GET_LAYER_REFERENCE(ref1);
            ref1->ATTACH(par1);
        }
        inline FloatType EVAL (size_t i) {
            Net.SET_FIRST_INPUT(&(data(i)));
            Net.SET_ANSWER(&(data(i)));
            Net.ACTIVATE();
            auto error=Net.BACKPROP();
            printf("    error = %e\n",error/BatchSize);
            Net.UPDATE(0.0001/BatchSize);
        }
    public:
        inline void WRITE (std::string PREFIX = PATH_Pars_SAE ()) const {
            par1 >> PREFIX + "/LastIdentity" ;
        }
        inline void READ (std::string PREFIX = PATH_Pars_SAE ()) {
            par1 << PREFIX + "/LastIdentity" ;
        }
        inline void EVAL () { for(size_t i=0;i<data();i++){EVAL(i);} }
        MainTrainer() : data(MakeBatch()) {PREPARE();}
    } ;
}
namespace FinalTFPlotter {
    namespace SlaveSpace {
        inline void PlotMap (
            std::string name         ,
            std::string originalname ,
            std::string outfloatname
        ) {
            printf(
                "DEBUG: PlotMap(%s,%s,%s);\n",
                &(name[0]), &(originalname[0]), &(outfloatname[0])
            );
            CPPFileIO::FileVector     < FloatType     > Writer ( outfloatname ) ;
            CPPFileIO::FullFileReader < FlatArrayType > diff   ( name         ) ;
            CPPFileIO::FullFileReader < FlatArrayType > orig   ( originalname ) ;
            if (true) for (size_t i=0;i<diff();i++) {
                Writer.push_back
                    ( sqrt ( diff(i).L2Norm() ) ) ;
            }
            if (false) for (size_t i=0;i<diff();i++) {
                Writer.push_back (
                    diff(i).L1Norm() /
                    orig(i).L1Norm()
                ) ;
            }
        }
    }
    inline void PlotAll (size_t i) {
        printf("DEBUG: PlotAll(%ld);\n",i);
        char tmp[256] ;

        sprintf ( tmp , "./OUTS/QCD.predict.dat.%ld" , i ) ; std::string PQ (tmp) ;
        sprintf ( tmp , "./OUTS/QCD.error.%ld"       , i ) ; std::string EQ (tmp) ;
        SlaveSpace::PlotMap ( PQ , Path_QCD() , EQ ) ;

        sprintf ( tmp , "./OUTS/TOP.predict.dat.%ld" , i ) ; std::string PT (tmp) ;
        sprintf ( tmp , "./OUTS/TOP.error.%ld"       , i ) ; std::string ET (tmp) ;
        SlaveSpace::PlotMap ( PT , Path_TOP() , ET ) ;

        sprintf ( tmp , "./OUTS/WBS.predict.dat.%ld" , i ) ; std::string PW (tmp) ;
        sprintf ( tmp , "./OUTS/WBS.error.%ld"       , i ) ; std::string EW (tmp) ;
        SlaveSpace::PlotMap ( PW , "./OUTS/WBS.dat" , EW ) ;

        sprintf ( tmp , "./OUTS/H4J.predict.dat.%ld" , i ) ; std::string PH (tmp) ;
        sprintf ( tmp , "./OUTS/H4J.error.%ld"       , i ) ; std::string EH (tmp) ;
        SlaveSpace::PlotMap ( PH , "./OUTS/H4J.dat" , EH ) ;

        sprintf ( tmp , "Compare_Error_%ld"          , i ) ; std::string CE (tmp) ;

        CPPFileIO::FullFileReader <FloatType> qcd (EQ) ;
        CPPFileIO::FullFileReader <FloatType> top (ET) ;
        CPPFileIO::FullFileReader <FloatType> wbs (EW) ;
        CPPFileIO::FullFileReader <FloatType> h4j (EH) ;

        NewHEPHeaders::Histograms::MyHistN <4> filler (CE,100,-0.001,0.35) ;
        auto FullData = UnifiedData::GET() ;
        for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
        for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
        for (size_t i=0;i<wbs();i++) if (FullData.WBS_PASS(i)) {filler.Fill<2>(wbs(i));}
        for (size_t i=0;i<h4j();i++) if (FullData.H4J_PASS(i)) {filler.Fill<3>(h4j(i));}
    }
    inline void PlotShih () {
        printf("DEBUG: PlotShih ()\n");
        SlaveSpace::PlotMap (
            /* name         = */ "./OUTS/SHIH/QCD.predict.dat" ,
            /* originalname = */ "./OUTS/SHIH/QCD.dat"         ,
            /* outfloatname = */ "./OUTS/SHIH/QCD.error.dat"
        ) ;
        SlaveSpace::PlotMap (
            /* name         = */ "./OUTS/SHIH/TOP.predict.dat" ,
            /* originalname = */ "./OUTS/SHIH/TOP.dat"         ,
            /* outfloatname = */ "./OUTS/SHIH/TOP.error.dat"
        ) ;
        SlaveSpace::PlotMap (
            /* name         = */ "./OUTS/SHIH/WBS.predict.dat" ,
            /* originalname = */ "./OUTS/SHIH/WBS.dat"         ,
            /* outfloatname = */ "./OUTS/SHIH/WBS.error.dat"
        ) ;
        SlaveSpace::PlotMap (
            /* name         = */ "./OUTS/SHIH/H4J.predict.dat" ,
            /* originalname = */ "./OUTS/SHIH/H4J.dat"         ,
            /* outfloatname = */ "./OUTS/SHIH/H4J.error.dat"
        ) ;
        auto FullData = UnifiedData::GET() ;
        NewHEPHeaders::Histograms::MyHistN <4> filler ("Compare_Error_SHIH",100,-0.1,2.1) ;
        /* THE QCD PART: */ {
            CPPFileIO::FullFileReader <FloatType> qcd ("./OUTS/SHIH/QCD.error.dat") ;
            for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
        }
        /* THE TOP PART: */ {
            CPPFileIO::FullFileReader <FloatType> top ("./OUTS/SHIH/TOP.error.dat") ;
            for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
        }
        /* THE WBS PART: */ {
            CPPFileIO::FullFileReader <FloatType> wbs ("./OUTS/SHIH/WBS.error.dat") ;
            for (size_t i=0;i<wbs();i++) if (FullData.WBS_PASS(i)) {filler.Fill<2>(wbs(i));}
        }
        /* THE H4J PART: */ {
            CPPFileIO::FullFileReader <FloatType> h4j ("./OUTS/SHIH/H4J.error.dat") ;
            for (size_t i=0;i<h4j();i++) if (FullData.H4J_PASS(i)) {filler.Fill<3>(h4j(i));}
        }
    }
    inline void PlotDet () {
        printf("DEBUG: PlotDet ()\n");
        /* EvalError My Part: */ {
            //
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/QCD_DET.predict.dat.5" ,
                /* originalname = */ "./OUTS/QCD_DET.dat"           ,
                /* outfloatname = */ "./OUTS/QCD_DET.error.dat"
            ) ;
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/TOP_DET.predict.dat.5" ,
                /* originalname = */ "./OUTS/TOP_DET.dat"           ,
                /* outfloatname = */ "./OUTS/TOP_DET.error.dat"
            ) ;
            //
        }
        /* EvalError Shih part: */ {
            //
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/SHIH/QCD_DET.predict.dat" ,
                /* originalname = */ "./OUTS/SHIH/QCD_DET.dat"         ,
                /* outfloatname = */ "./OUTS/SHIH/QCD_DET.error.dat"
            ) ;
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/SHIH/TOP_DET.predict.dat" ,
                /* originalname = */ "./OUTS/SHIH/TOP_DET.dat"         ,
                /* outfloatname = */ "./OUTS/SHIH/TOP_DET.error.dat"
            ) ;
            //
        }
        auto FullData = UnifiedDataDet::GET() ;
        /* Plot Error My Part: */ {
            NewHEPHeaders::Histograms::MyHistN <2> filler ("Compare_Error_DET_mine",100,-0.1,2.1) ;
            /* THE QCD PART: */ {
                CPPFileIO::FullFileReader <FloatType> qcd ("./OUTS/QCD_DET.error.dat") ;
                for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
            }
            /* THE TOP PART: */ {
                CPPFileIO::FullFileReader <FloatType> top ("./OUTS/TOP_DET.error.dat") ;
                for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
            }
        }
        /* Plot Error SHIH Part: */ {
            NewHEPHeaders::Histograms::MyHistN <2> filler ("Compare_Error_DET_shih",100,-0.1,2.1) ;
            /* THE QCD PART: */ {
                CPPFileIO::FullFileReader <FloatType> qcd ("./OUTS/SHIH/QCD_DET.error.dat") ;
                for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
            }
            /* THE TOP PART: */ {
                CPPFileIO::FullFileReader <FloatType> top ("./OUTS/SHIH/TOP_DET.error.dat") ;
                for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
            }
        }

    }
    inline void PlotFull () {
        printf("DEBUG: PlotFull ()\n");
        /* EvalError My Part: */ {
            //
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/QCD_FULL.predict.dat.5" ,
                /* originalname = */ "./OUTS/QCD_FULL.dat"           ,
                /* outfloatname = */ "./OUTS/QCD_FULL.error.dat"
            ) ;
            SlaveSpace::PlotMap (
                /* name         = */ "./OUTS/TOP_FULL.predict.dat.5" ,
                /* originalname = */ "./OUTS/TOP_FULL.dat"           ,
                /* outfloatname = */ "./OUTS/TOP_FULL.error.dat"
            ) ;
            //
        }
        auto FullData = UnifiedDataFull::GET() ;
        /* Plot Error My Part: */ {
            NewHEPHeaders::Histograms::MyHistN <2> filler ("Compare_Error_FULL_mine",100,-0.1,2.1) ;
            /* THE QCD PART: */ {
                CPPFileIO::FullFileReader <FloatType> qcd ("./OUTS/QCD_FULL.error.dat") ;
                for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
            }
            /* THE TOP PART: */ {
                CPPFileIO::FullFileReader <FloatType> top ("./OUTS/TOP_FULL.error.dat") ;
                for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
            }
        }
    }
    inline void PlotAll () {
        printf("DEBUG: PlotAll();\n");
        PlotAll(5); PlotShih(); PlotDet(); PlotFull();
        return;
        PlotAll(1); PlotAll(2); PlotAll(3); PlotAll(4);
        PlotAll(5); PlotAll(6); PlotAll(7); PlotAll(8);
    }
    inline void PlotTF () {
        printf("DEBUG: PlotTF();\n");
        char tmp[256] ;
        sprintf ( tmp , "./OUTS/QCD.predict.tf" ) ; std::string PQ (tmp) ;
        sprintf ( tmp , "./OUTS/QCD.error.tf"   ) ; std::string EQ (tmp) ;
        sprintf ( tmp , "./OUTS/TOP.predict.tf" ) ; std::string PT (tmp) ;
        sprintf ( tmp , "./OUTS/TOP.error.tf"   ) ; std::string ET (tmp) ;
        sprintf ( tmp , "Compare_Error_TF"      ) ; std::string CE (tmp) ;
        SlaveSpace::PlotMap ( PQ , Path_QCD () , EQ ) ;
        SlaveSpace::PlotMap ( PT , Path_TOP () , ET ) ;
        CPPFileIO::FullFileReader <FloatType> qcd (EQ) ;
        CPPFileIO::FullFileReader <FloatType> top (ET) ;
        NewHEPHeaders::Histograms::MyHistN <2> filler (CE,100,-0.1,2.1) ;
        auto FullData = UnifiedData::GET () ;
        for (size_t i=0;i<qcd();i++) if (FullData.QCD_PASS(i)) {filler.Fill<0>(qcd(i));}
        for (size_t i=0;i<top();i++) if (FullData.TOP_PASS(i)) {filler.Fill<1>(top(i));}
    }
}
namespace ROC_NN {
    using namespace Tensors::NeuralNetwork::NeuralLayers ;
    template <size_t batchsize=4096> class SampleNN {
    public:
        typedef Tensors::Arrays::N2DArray <1,batchsize,FloatType> TYPE_Input ;
        typedef InputRelated::FirstLayer        < TYPE_Input > NL0     ;
        typedef MainLayers::NNLayerD2           < 10  , NL0  > NL1     ;
        typedef Activators::ActivatorSoftLRUD2  <       NL1  > NA1     ;
        typedef MainLayers::NNLayerD2           < 10  , NA1  > NL2     ;
        typedef Activators::ActivatorSoftLRUD2  <       NL2  > NA2     ;
        typedef MainLayers::NNLayerD2           <  1  , NA2  > NL3     ;
        typedef Activators::ActivatorSoftSignD2 <       NL3  > NA3     ;
        typedef MainLayers::Teacher             <       NA3  > MainNet ;
        typedef typename MainNet::TYPE_Output  TYPE_Output;
        typedef typename NL1::TYPE_Parameter   NP1 ;
        typedef typename NL2::TYPE_Parameter   NP2 ;
        typedef typename NL3::TYPE_Parameter   NP3 ;
        typedef typename TYPE_Input::TYPE_Data TYPE_Data ;
    private:
        NP1   par1 ;
        NP2   par2 ;
        NP3   par3 ;
        NL1 * Ref1 ;
        NL2 * Ref2 ;
        NL3 * Ref3 ;
        NA3 * RefOut ;
        CPPFileIO::FullFileReader <TYPE_Input> BKG ;
        CPPFileIO::FullFileReader <TYPE_Input> SIG ;
        TYPE_Output ANS_SIG, ANS_BKG ;
        MainNet net ;
    public:
        static inline void MakeBatch ( std::string NAME_SIG , std::string NAME_BKG ) {
            /* The top part: */ {
                Tensors::Arrays::Batcher <TYPE_Input>
                    out (NAME_SIG+".batch") ;
                out = NAME_SIG ;
            }
            /* The QCD Part: */ {
                Tensors::Arrays::Batcher <TYPE_Input>
                    out (NAME_BKG+".batch") ;
                out = NAME_BKG ;
            }
        }
        inline ssize_t WRITE (std::string filename) {
            CPPFileIO::FileFD writer (filename) ;
            writer.writefile () ;
            ssize_t ret = 0 ;
            ret += ( par1 >> writer ) ;
            ret += ( par2 >> writer ) ;
            ret += ( par3 >> writer ) ;
            return ret ;
        }
        inline ssize_t READ  (std::string filename) {
            CPPFileIO::FileFD reader (filename) ;
            reader.readfile () ;
            ssize_t ret = 0 ;
            ret += ( par1 << reader ) ;
            ret += ( par2 << reader ) ;
            ret += ( par3 << reader ) ;
            return ret ;
        }
    private:
        inline void RANDOMIZE(){
            par1.RANDOMIZE();
            par2.RANDOMIZE();
            par3.RANDOMIZE();
        }
        inline void GET_LAYER_REFERENCE(){
            net.GET_LAYER_REFERENCE(Ref1);
            net.GET_LAYER_REFERENCE(Ref2);
            net.GET_LAYER_REFERENCE(Ref3);
            net.GET_LAYER_REFERENCE(RefOut);
        }
        inline void ATTACH () {
            Ref1->ATTACH(&par1);
            Ref2->ATTACH(&par2);
            Ref3->ATTACH(&par3);
        }
        inline void INIT () {
            RANDOMIZE           () ;
            GET_LAYER_REFERENCE () ;
            ATTACH              () ;
            ANS_SIG =   1.0 ;
            ANS_BKG = - 1.0 ;
            printf ( "DEBUG: BKG() = %ld\n" , BKG () );
            printf ( "DEBUG: SIG() = %ld\n" , SIG () );
        }
        inline void EVAL_BATCH (size_t i, TYPE_Data eta) {
            TYPE_Data SE, BE ;
            bool update = false ;
            if(i<BKG()){
                net.SET_FIRST_INPUT(&(BKG(i)));
                net.SET_ANSWER(&ANS_BKG);
                net.ACTIVATE();
                BE = net.BACKPROP();
                update = true ;
            }
            if(i<SIG()){
                net.SET_FIRST_INPUT(&(SIG(i)));
                net.SET_ANSWER(&ANS_SIG);
                net.ACTIVATE();
                SE = net.BACKPROP();
                update = true ;
            }
            if(update){
                printf(
                    "ERRORS: SE = %e, BE = %e, TE=%e\n",
                    SE,BE,SE+BE
                );
                net.UPDATE(eta);
            }
        }
    public:
        inline void EVAL_BATCH (TYPE_Data eta=0.01) {
            auto min = CPPFileIO::mymin ( SIG () , BKG () ) ;
            for (size_t i=0;i<min;i++) { EVAL_BATCH (i,eta) ; }
        }
        inline void EVAL_ROC (std::string bkgname, std::string signame) {
            /* The bankground part: */ {
                CPPFileIO::FileVector <TYPE_Output> BKG_RES (bkgname) ;
                for ( size_t i = 0 ; i < BKG () ; i++ ) {
                    net.SET_FIRST_INPUT (&(BKG(i))) ; net.ACTIVATE () ;
                    TYPE_Output OUTBKG = RefOut->GET_OUTPUT () [0] ;
                    BKG_RES.push_back (OUTBKG) ;
                }
            }
            /* The Signal Part: */ {
                CPPFileIO::FileVector <TYPE_Output> SIG_RES (signame) ;
                for ( size_t i = 0 ; i < SIG () ; i++ ) {
                    net.SET_FIRST_INPUT (&(SIG(i))) ; net.ACTIVATE () ;
                    TYPE_Output OUTSIG = RefOut->GET_OUTPUT () [0] ;
                    SIG_RES.push_back (OUTSIG) ;
                }
            }
        }
        SampleNN ( std::string NAME_SIG, std::string NAME_BKG ) :
        BKG(NAME_BKG), SIG(NAME_SIG) {INIT();}
        ~SampleNN(){}

        static inline SampleNN <batchsize> * NEW (
            std::string NAME_SIG ,
            std::string NAME_BKG
        ) {
            MakeBatch (NAME_SIG,NAME_BKG) ;
            SampleNN <batchsize> * ret =
                new SampleNN <batchsize> (
                    NAME_SIG+".batch",
                    NAME_BKG+".batch"
                ) ;
            return ret ;
        }
    } ;
    template <typename TF=FloatType> class ROCElement {
    public:
        typedef ROCElement <TF> TYPE_Self ;
        int    type  ;
        size_t index ;
        TF     value ;
        inline bool operator > (TYPE_Self const & other) const
        {return value>other.value;}
        inline bool operator < (TYPE_Self const & other) const
        {return value<other.value;}
        ROCElement(TF _value, int _type, size_t _index):
        value(_value), index(_index), type(_type) {}
        ROCElement(){}
    } ;
}
namespace ROC {
    inline void MakeROC () {
        using namespace ROC_NN;
        Paths tmp ;
        //printf("CAME HERE...\n");
        /* The NN Part: */ {
            auto * slave = SampleNN<>::NEW (
                tmp.TOP_ERROR_FILE(),
                tmp.QCD_ERROR_FILE()
            );
            slave->READ (tmp.ROC_PARS_FILE()) ;
            slave->EVAL_ROC (
                tmp.QCD_ROC_FILE(),
                tmp.TOP_ROC_FILE()
            );
            delete slave ;
        }
        /* The actual ROC Part: */ {

            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata = UnifiedData::GET();
                CPPFileIO::FullFileReader <FloatType>
                    SIG (tmp.TOP_ROC_FILE()) ;
                CPPFileIO::FullFileReader <FloatType>
                    BKG (tmp.QCD_ROC_FILE()) ;
                arrays.reserve(SIG()+BKG());
                //printf("DEBUG: %ld %ld\n",BKG(),unifieddata.QCD());
                for(size_t i=0;i<SIG();i++) {
                    //printf("TTBAR RESPONSE: %e \n",SIG(i));
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.TOP_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }
            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES ("MyNNRES",100,-1.1,1.1) ;
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++){
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ; size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename = tmp.ROC_FILE_TXT () ;
            FILE * f = fopen (&(outfilename[0]),"w") ;
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        auto * slave = SampleNN<>::NEW(
            TOP_ERROR_FILE(),QCD_ERROR_FILE()
        );
        slave->WRITE(ROC_PARS_FILE());
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW(
            TOP_ERROR_FILE(),QCD_ERROR_FILE()
        );
        slave->READ(ROC_PARS_FILE());
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH(TrainRate);
            printf("Writing...\n");
            slave->WRITE(ROC_PARS_FILE());
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_QCD_TTBAR_DET {
    inline std::string FILE_WBS ()
    { return std::string("./OUTS/TOP_DET.error.dat"); }

    inline std::string FILE_QCD ()
    { return std::string("./OUTS/QCD_DET.error.dat"); }

    inline std::string ROC_PARS_FILE_WBS ()
    { return std::string("./OUTS/PARS_DET/ROC_Pars"); }

    inline std::string WBS_ROC_FILE_WBS ()
    { return std::string("./OUTS/DET_ROC_TOP") ; }

    inline std::string WBS_ROC_FILE_QCD ()
    { return std::string("./OUTS/DET_ROC_QCD") ; }

    inline std::string WBS_ROC_FILE_TXT ()
    { return std::string("./OUTS/DET_ROC.txt"); }

    inline std::string RES_NAME ()
    { return std::string ("MyNNRES_DET") ; }

    inline void MakeROC () {
        using namespace ROC_NN;
        /* The NN Part: */ {
            //
            auto * slave =
                SampleNN<>::NEW (
                    FILE_WBS() ,
                    FILE_QCD()
                );
            //
            slave->READ (
                ROC_PARS_FILE_WBS()
            ) ;
            //
            slave->EVAL_ROC (
                WBS_ROC_FILE_QCD() ,
                WBS_ROC_FILE_WBS()
            ) ;
            //
            delete slave ;
            //
        }
        /* The actual ROC Part: */ if (true) {
            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata =
                    UnifiedData::GET();
                //
                CPPFileIO::FullFileReader <FloatType>
                    SIG (WBS_ROC_FILE_WBS()) ;
                //
                CPPFileIO::FullFileReader <FloatType>
                    BKG (WBS_ROC_FILE_QCD()) ;
                //
                arrays.reserve(
                    SIG() + BKG()
                );
                //
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.TOP_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }

            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES (RES_NAME(),100,-1.1,1.1) ;
            //
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++) {
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ;
            size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename =
                WBS_ROC_FILE_TXT () ;
            //
            FILE * f = fopen
                (&(outfilename[0]),"w") ;
            //
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 )
                    { NEW_SIG-- ; }
                //
                else if ( arrays[i].type == -1 )
                    { NEW_BKG-- ; }
                //
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        //
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS () , FILE_QCD () ) ;
        //
        slave->WRITE
            ( ROC_PARS_FILE_WBS () ) ;
        //
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS() , FILE_QCD() ) ;
        //
        slave->READ
            ( ROC_PARS_FILE_WBS() ) ;
        //
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH
                (TrainRate);
            //
            printf("Writing...\n");
            slave->WRITE
                (ROC_PARS_FILE_WBS());
            //
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_QCD_TTBAR_DET_SHIH {
    inline std::string FILE_WBS ()
    { return std::string("./OUTS/SHIH/TOP_DET.error.dat"); }

    inline std::string FILE_QCD ()
    { return std::string("./OUTS/SHIH/QCD_DET.error.dat"); }

    inline std::string ROC_PARS_FILE_WBS ()
    { return std::string("./OUTS/SHIH/ROC_PARS_DET"); }

    inline std::string WBS_ROC_FILE_WBS ()
    { return std::string("./OUTS/SHIH/DET_ROC_TOP") ; }

    inline std::string WBS_ROC_FILE_QCD ()
    { return std::string("./OUTS/SHIH/DET_ROC_QCD") ; }

    inline std::string WBS_ROC_FILE_TXT ()
    { return std::string("./OUTS/SHIH/DET_ROC.txt"); }

    inline std::string RES_NAME ()
    { return std::string ("ShihNNRES_DET") ; }

    inline void MakeROC () {
        using namespace ROC_NN;
        /* The NN Part: */ {
            //
            auto * slave =
                SampleNN<>::NEW (
                    FILE_WBS() ,
                    FILE_QCD()
                );
            //
            slave->READ (
                ROC_PARS_FILE_WBS()
            ) ;
            //
            slave->EVAL_ROC (
                WBS_ROC_FILE_QCD() ,
                WBS_ROC_FILE_WBS()
            ) ;
            //
            delete slave ;
            //
        }
        /* The actual ROC Part: */ if (true) {
            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata =
                    UnifiedData::GET();
                //
                CPPFileIO::FullFileReader <FloatType>
                    SIG (WBS_ROC_FILE_WBS()) ;
                //
                CPPFileIO::FullFileReader <FloatType>
                    BKG (WBS_ROC_FILE_QCD()) ;
                //
                arrays.reserve(
                    SIG() + BKG()
                );
                //
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.TOP_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }

            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES (RES_NAME(),100,-1.1,1.1) ;
            //
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++) {
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ;
            size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename =
                WBS_ROC_FILE_TXT () ;
            //
            FILE * f = fopen
                (&(outfilename[0]),"w") ;
            //
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        //
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS () , FILE_QCD () ) ;
        //
        slave->WRITE
            ( ROC_PARS_FILE_WBS () ) ;
        //
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS() , FILE_QCD() ) ;
        //
        slave->READ
            ( ROC_PARS_FILE_WBS() ) ;
        //
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH
                (TrainRate);
            //
            printf("Writing...\n");
            slave->WRITE
                (ROC_PARS_FILE_WBS());
            //
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_QCD_W {
    inline std::string FILE_WBS ()
    { return std::string("./OUTS/WBS.error.5"); }

    inline std::string FILE_QCD ()
    { return std::string("./OUTS/QCD.error.5"); }

    inline std::string ROC_PARS_FILE_WBS ()
    { return std::string("./OUTS/PARS/ROC_Pars_WBS"); }

    inline std::string WBS_ROC_FILE_WBS ()
    { return std::string("./OUTS/WBS_ROC_WBS") ; }

    inline std::string WBS_ROC_FILE_QCD ()
    { return std::string("./OUTS/WBS_ROC_QCD") ; }

    inline std::string WBS_ROC_FILE_TXT ()
    { return std::string("./OUTS/ROC_WBS.txt"); }

    inline void MakeROC () {
        using namespace ROC_NN;
        /* The NN Part: */ {
            //
            auto * slave =
                SampleNN<>::NEW (
                    FILE_WBS() ,
                    FILE_QCD()
                );
            //
            slave->READ (
                ROC_PARS_FILE_WBS()
            ) ;
            //
            slave->EVAL_ROC (
                WBS_ROC_FILE_QCD() ,
                WBS_ROC_FILE_WBS()
            ) ;
            //
            delete slave ;
            //
        }
        /* The actual ROC Part: */ if (true) {
            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata =
                    UnifiedData::GET();
                //
                CPPFileIO::FullFileReader <FloatType>
                    SIG (WBS_ROC_FILE_WBS()) ;
                //
                CPPFileIO::FullFileReader <FloatType>
                    BKG (WBS_ROC_FILE_QCD()) ;
                //
                arrays.reserve(
                    SIG() + BKG()
                );
                //
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.WBS_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }

            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES ("MyNNRES_WBS",100,-1.1,1.1) ;
            //
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++) {
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ;
            size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename =
                WBS_ROC_FILE_TXT () ;
            //
            FILE * f = fopen
                (&(outfilename[0]),"w") ;
            //
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        //
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS () , FILE_QCD () ) ;
        //
        slave->WRITE
            ( ROC_PARS_FILE_WBS () ) ;
        //
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW
            ( FILE_WBS() , FILE_QCD() ) ;
        //
        slave->READ
            ( ROC_PARS_FILE_WBS() ) ;
        //
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH
                (TrainRate);
            //
            printf("Writing...\n");
            slave->WRITE
                (ROC_PARS_FILE_WBS());
            //
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_QCD_H4J {
    inline std::string FILE_SIG ()
    { return std::string("./OUTS/H4J.error.5"); }

    inline std::string FILE_BKG ()
    { return std::string("./OUTS/QCD.error.5"); }

    inline std::string ROC_PARS_FILE ()
    { return std::string("./OUTS/PARS/ROC_Pars_H4J"); }

    inline std::string ROC_FILE_SIG ()
    { return std::string("./OUTS/H4J_ROC_H4J") ; }

    inline std::string ROC_FILE_BKG ()
    { return std::string("./OUTS/H4J_ROC_QCD") ; }

    inline std::string ROC_FILE_TXT ()
    { return std::string("./OUTS/ROC_H4J.txt"); }

    inline void MakeROC () {
        using namespace ROC_NN;
        /* The NN Part: */ {
            //
            auto * slave =
                SampleNN<>::NEW (
                    FILE_SIG() ,
                    FILE_BKG()
                );
            //
            slave->READ (
                ROC_PARS_FILE()
            ) ;
            //
            slave->EVAL_ROC (
                ROC_FILE_BKG() ,
                ROC_FILE_SIG()
            ) ;
            //
            delete slave ;
            //
        }
        /* The actual ROC Part: */ if (true) {
            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata =
                    UnifiedData::GET();
                //
                CPPFileIO::FullFileReader <FloatType>
                    SIG (ROC_FILE_SIG()) ;
                //
                CPPFileIO::FullFileReader <FloatType>
                    BKG (ROC_FILE_BKG()) ;
                //
                arrays.reserve(
                    SIG() + BKG()
                );
                //
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.H4J_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }

            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES ("MyNNRES_H4J",100,-1.1,1.1) ;
            //
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++) {
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ;
            size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename =
                ROC_FILE_TXT () ;
            //
            FILE * f = fopen
                (&(outfilename[0]),"w") ;
            //
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        //
        auto * slave = SampleNN<>::NEW
            ( FILE_SIG () , FILE_BKG () ) ;
        //
        slave->WRITE
            ( ROC_PARS_FILE () ) ;
        //
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW
            ( FILE_SIG() , FILE_BKG() ) ;
        //
        slave->READ
            ( ROC_PARS_FILE() ) ;
        //
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH
                (TrainRate);
            //
            printf("Writing...\n");
            slave->WRITE
                ( ROC_PARS_FILE() );
            //
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_QCD_TOP_FULL {
    inline std::string FILE_SIG ()
    { return std::string("./OUTS/TOP_FULL.error.dat"); }

    inline std::string FILE_BKG ()
    { return std::string("./OUTS/QCD_FULL.error.dat"); }

    inline std::string ROC_PARS_FILE ()
    { return std::string("./OUTS/PARS/ROC_Pars_FULL"); }

    inline std::string ROC_FILE_SIG ()
    { return std::string("./OUTS/FULL_ROC_TOP") ; }

    inline std::string ROC_FILE_BKG ()
    { return std::string("./OUTS/FULL_ROC_QCD") ; }

    inline std::string ROC_FILE_TXT ()
    { return std::string("./OUTS/ROC_FULL.txt"); }

    inline void MakeROC () {
        using namespace ROC_NN;
        /* The NN Part: */ {
            //
            auto * slave =
                SampleNN<>::NEW (
                    FILE_SIG() ,
                    FILE_BKG()
                );
            //
            slave->READ (
                ROC_PARS_FILE()
            ) ;
            //
            slave->EVAL_ROC (
                ROC_FILE_BKG() ,
                ROC_FILE_SIG()
            ) ;
            //
            delete slave ;
            //
        }
        /* The actual ROC Part: */ if (true) {
            std::vector <ROCElement<>> arrays ;
            /* Read in the data */ {
                auto unifieddata =
                    UnifiedData::GET();
                //
                CPPFileIO::FullFileReader <FloatType>
                    SIG (ROC_FILE_SIG()) ;
                //
                CPPFileIO::FullFileReader <FloatType>
                    BKG (ROC_FILE_BKG()) ;
                //
                arrays.reserve(
                    SIG() + BKG()
                );
                //
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.H4J_PASS(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (
                    arrays.begin(),
                    arrays.end()
                ) ;
            }

            NewHEPHeaders::Histograms::MyHistN <2>
                NNRES ("MyNNRES_FULL",100,-1.1,1.1) ;
            //
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            for(size_t i=0;i<arrays.size();i++) {
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill <0>
                        (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill <1>
                        (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ;
            size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename =
                ROC_FILE_TXT () ;
            //
            FILE * f = fopen
                (&(outfilename[0]),"w") ;
            //
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf (
                    f , "%e %e\n" ,
                    ( (double) NEW_SIG / TOTAL_SIG ) ,
                    1.0 / ( (double) NEW_BKG / TOTAL_BKG )
                ) ;
            }
            fclose(f);
        }
    }
    inline void Create  () {
        using namespace ROC_NN;
        //
        auto * slave = SampleNN<>::NEW
            ( FILE_SIG () , FILE_BKG () ) ;
        //
        slave->WRITE
            ( ROC_PARS_FILE () ) ;
        //
        delete slave ;
    }
    inline void Train   () {
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW
            ( FILE_SIG() , FILE_BKG() ) ;
        //
        slave->READ
            ( ROC_PARS_FILE() ) ;
        //
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH
                (TrainRate);
            //
            printf("Writing...\n");
            slave->WRITE
                ( ROC_PARS_FILE() );
            //
            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace ROC_SHIH {
    inline void MakeROC () {
        using namespace ROC_NN;
        Paths tmp ;
        /* The NN Part: */ {
            auto * slave = SampleNN<>::NEW(
                tmp.SHIH_TOP_ERROR_FILE(),
                tmp.SHIH_QCD_ERROR_FILE()
            );
            slave->READ(tmp.SHIH_ROC_PARS_FILE());
            slave->EVAL_ROC(
                tmp.SHIH_QCD_ROC_FILE(),
                tmp.SHIH_TOP_ROC_FILE()
            );
            delete slave ;
        }
        /* The actual ROC Part: */ {
            std::vector <ROCElement<>> arrays ; /* Read in the data */ {
                auto unifieddata = UnifiedData::GET();

                CPPFileIO::FullFileReader <FloatType>
                SIG (tmp.SHIH_TOP_ROC_FILE()) ;

                CPPFileIO::FullFileReader <FloatType>
                BKG (tmp.SHIH_QCD_ROC_FILE()) ;

                arrays.reserve(SIG()+BKG());
                for(size_t i=0;i<SIG();i++) {
                    ROCElement <> tmp(SIG(i),1,i);
                    if(unifieddata.TOP_PASS_SHIH(i))
                    {arrays.push_back(tmp);}
                }
                for(size_t i=0;i<BKG();i++) {
                    ROCElement <> tmp(BKG(i),-1,i);
                    if(unifieddata.QCD_PASS_SHIH(i))
                    {arrays.push_back(tmp);}
                }
                std::sort (arrays.begin(),arrays.end()) ;
            }
            size_t TOTAL_SIG = 0 ; size_t TOTAL_BKG = 0 ;
            NewHEPHeaders::Histograms::MyHistN <2>
            NNRES ("ShihNNRES",100,-1.1,1.1) ;

            for(size_t i=0;i<arrays.size();i++){
                if      (arrays[i].type ==  1 ) {
                    TOTAL_SIG++;
                    NNRES.Fill<0>
                    (arrays[i].value);
                }
                else if (arrays[i].type == -1 ) {
                    TOTAL_BKG++;
                    NNRES.Fill<1>
                    (arrays[i].value);
                }
            }
            size_t NEW_BKG = TOTAL_BKG ; size_t NEW_SIG = TOTAL_SIG ;
            std::string outfilename = tmp.SHIH_ROC_FILE_TXT () ;
            FILE * f = fopen (&(outfilename[0]),"w") ;
            for ( size_t i = 0 ; i < arrays.size() ; i++ ) {
                if      ( arrays[i].type ==  1 ) { NEW_SIG-- ; }
                else if ( arrays[i].type == -1 ) { NEW_BKG-- ; }
                fprintf(
                    f,"%e %e\n",
                    ((double)NEW_SIG/TOTAL_SIG),
                    1.0/((double)NEW_BKG/TOTAL_BKG)
                );
            }
            fclose(f);
        }
    }
    inline void Create  () {
        Paths tmp ;
        using namespace ROC_NN;
        auto * slave = SampleNN<>::NEW (
            tmp.SHIH_TOP_ERROR_FILE(),
            tmp.SHIH_QCD_ERROR_FILE()
        ) ;
        slave->WRITE(tmp.SHIH_ROC_PARS_FILE());
        delete slave ;
    }
    inline void Train   () {
        Paths tmp ;
        using namespace ROC_NN;
        printf("Came here...\n");
        auto * slave = SampleNN<>::NEW (
            tmp.SHIH_TOP_ERROR_FILE(),
            tmp.SHIH_QCD_ERROR_FILE()
        ) ;
        slave->READ(tmp.SHIH_ROC_PARS_FILE());
        for(size_t j=0;j<100;j++){
            slave->EVAL_BATCH(TrainRate);
            printf("Writing...\n");

            slave->WRITE
            (tmp.SHIH_ROC_PARS_FILE());

            printf("Wrote...\n");
        }
        delete slave ;
    }
}
namespace HTT_ROC_Point {
    inline void GetPoint () {
        size_t QCD_Full = 0 ;
        size_t QCD_Selected = 0 ;
        /*QCD PART:*/ {
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/QCD.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass_mass()) {
                QCD_Full++;
                if(FileReader(i).HTT)
                {QCD_Selected++;}
            }
        }
        size_t TOP_Full = 0 ;
        size_t TOP_Selected = 0 ;
        /*TOP PART:*/ {
            //
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/TOP.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass()) {
                TOP_Full++;
                if(FileReader(i).HTT)
                {TOP_Selected++;}
            }
            //
        }
        FILE *f = fopen (
            "./OUTS/HTT_ROC_PT","w"
        ) ;
        fprintf (
            f , "%e %e\n" ,
            ((double)TOP_Selected/TOP_Full) ,
            1.0 / ((double)QCD_Selected/QCD_Full)
        ) ;
        fclose(f);
    }
}
namespace HTT_ROC_Point_DET {
    inline void GetPoint () {
        size_t QCD_Full = 0 ;
        size_t QCD_Selected = 0 ;
        /*QCD PART:*/{
            //
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/QCD_DET.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass_mass()) {
                QCD_Full++;
                if(FileReader(i).HTT)
                {QCD_Selected++;}
            }
            //
        }
        size_t TOP_Full = 0 ;
        size_t TOP_Selected = 0 ;
        /*TOP PART:*/ {
            //
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/TOP_DET.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass()) {
                TOP_Full++;
                if(FileReader(i).HTT)
                {TOP_Selected++;}
            }
            //
        }
        FILE *f = fopen (
            "./OUTS/HTT_ROC_PT_DET","w"
        ) ;
        fprintf (
            f , "%e %e\n" ,
            ((double)TOP_Selected/TOP_Full) ,
            1.0 / ((double)QCD_Selected/QCD_Full)
        ) ;
        fclose(f);
    }
}
namespace HTT_ROC_Point_FULL {
    inline void GetPoint () {
        size_t QCD_Full = 0 ;
        size_t QCD_Selected = 0 ;
        /*QCD PART:*/{
            //
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/QCD_FULL.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass_mass()) {
                QCD_Full++;
                if(FileReader(i).HTT)
                {QCD_Selected++;}
            }
            //
        }
        size_t TOP_Full = 0 ;
        size_t TOP_Selected = 0 ;
        /*TOP PART:*/ {
            //
            CPPFileIO::FullFileReader
                <Datas::QCDPreProcessor>
                FileReader ("./OUTS/TOP_FULL.united") ;
            //
            for (size_t i=0;i<FileReader();i++)
            if (FileReader(i).pass()) {
                TOP_Full++;
                if(FileReader(i).HTT)
                {TOP_Selected++;}
            }
            //
        }
        FILE *f = fopen (
            "./OUTS/HTT_ROC_PT_FULL","w"
        ) ;
        fprintf (
            f , "%e %e\n" ,
            ((double)TOP_Selected/TOP_Full) ,
            1.0 / ((double)QCD_Selected/QCD_Full)
        ) ;
        fclose(f);
    }
}
namespace Error_Ratio_Plotter {
    inline void EvalError (size_t i) {
        printf("DEBUG: EvalError(%ld);\n",i);
        CPPFileIO::FullFileReader <FlatArrayType> QCDORIG ("./OUTS/QCD.dat") ;
        CPPFileIO::FullFileReader <FlatArrayType> TOPORIG ("./OUTS/TOP.dat") ;
        char tmp[256] ;
        sprintf(tmp,"./OUTS/QCD.error.%ld",i);
        CPPFileIO::FileVector     <FloatType>     QCDWriter (tmp) ;
        sprintf(tmp,"./OUTS/TOP.error.%ld",i);
        CPPFileIO::FileVector     <FloatType>     TOPWriter (tmp) ;
        sprintf(tmp,"./OUTS/QCD.predict.dat.%ld",i);
        CPPFileIO::FullFileReader <FlatArrayType> QCDREADER (tmp) ;
        sprintf(tmp,"./OUTS/TOP.predict.dat.%ld",i);
        CPPFileIO::FullFileReader <FlatArrayType> TOPREADER (tmp) ;
        for(size_t j=0;j<QCDREADER();j++){
            FloatType tmp = QCDREADER(j).L1Norm() / QCDORIG(j).L1Norm() ;
            QCDWriter.push_back(tmp);
        }
        for(size_t j=0;j<TOPREADER();j++){
            FloatType tmp = TOPREADER(j).L1Norm() / TOPORIG(j).L1Norm() ;
            TOPWriter.push_back(tmp);
        }
    }
    inline void PlotHist () {
        printf("DEBUG: PlotHist();\n");

        FinalTFPlotter::PlotAll() ;

        CPPFileIO::FullFileReader <FloatType> QCD1 ("./OUTS/QCD.error.1") ;
        CPPFileIO::FullFileReader <FloatType> TOP1 ("./OUTS/TOP.error.1") ;

        CPPFileIO::FullFileReader <FloatType> QCD2 ("./OUTS/QCD.error.2") ;
        CPPFileIO::FullFileReader <FloatType> TOP2 ("./OUTS/TOP.error.2") ;

        CPPFileIO::FullFileReader <FloatType> QCD3 ("./OUTS/QCD.error.3") ;
        CPPFileIO::FullFileReader <FloatType> TOP3 ("./OUTS/TOP.error.3") ;

        CPPFileIO::FullFileReader <FloatType> QCD4 ("./OUTS/QCD.error.4") ;
        CPPFileIO::FullFileReader <FloatType> TOP4 ("./OUTS/TOP.error.4") ;

        CPPFileIO::FullFileReader <FloatType> QCD5 ("./OUTS/QCD.error.5") ;
        CPPFileIO::FullFileReader <FloatType> TOP5 ("./OUTS/TOP.error.5") ;

        CPPFileIO::FullFileReader <FloatType> QCD6 ("./OUTS/QCD.error.6") ;
        CPPFileIO::FullFileReader <FloatType> TOP6 ("./OUTS/TOP.error.6") ;

        NewHEPHeaders::Histograms::MyHistN <2> filler12 (
            /*_histname=*/std::string("AUTOENCODER_1_BY_2"),
            (size_t) /*nbins=*/100,
            (double) /*min=*/-0.1,
            (double) /*max=*/5.1
        );
        NewHEPHeaders::Histograms::MyHistN <2> filler23 (
            /*_histname=*/ std::string("AUTOENCODER_2_BY_3"),
            (size_t) /*nbins=*/ 100,
            (double) /*min=*/ -0.1,
            (double) /*max=*/ 5.1
        );
        NewHEPHeaders::Histograms::MyHistN <2> filler34 (
            /*_histname=*/ std::string("AUTOENCODER_3_BY_4"),
            (size_t) /*nbins=*/ 100,
            (double) /*min=*/ -0.1,
            (double) /*max=*/ 5.1
        );
        NewHEPHeaders::Histograms::MyHistN <2> filler45 (
            /*_histname=*/ std::string("AUTOENCODER_4_BY_5"),
            (size_t) /*nbins=*/ 100,
            (double) /*min=*/ -0.1,
            (double) /*max=*/ 5.1
        );
        NewHEPHeaders::Histograms::MyHistN <2> filler56 (
            /*_histname=*/ std::string("AUTOENCODER_5_BY_6"),
            (size_t) /*nbins=*/ 100,
            (double) /*min=*/ -0.1,
            (double) /*max=*/ 5.1
        );
        for(size_t i=0;i<QCD1();i++){
            filler12.Fill<0>(QCD1(i)/QCD2(i));
            filler23.Fill<0>(QCD2(i)/QCD3(i));
            filler34.Fill<0>(QCD3(i)/QCD4(i));
            filler45.Fill<0>(QCD4(i)/QCD5(i));
            filler56.Fill<0>(QCD5(i)/QCD6(i));
        }
        for(size_t i=0;i<TOP1();i++){
            filler12.Fill<1>(TOP1(i)/TOP2(i));
            filler23.Fill<1>(TOP2(i)/TOP3(i));
            filler34.Fill<1>(TOP3(i)/TOP4(i));
            filler45.Fill<1>(TOP4(i)/TOP5(i));
            filler56.Fill<1>(TOP5(i)/TOP6(i));
        }
    }

    int PlotAll () {
        printf("DEBUG: Error_Ratio_Plotter::PlotAll()\n");
        PlotHist();
    }
}
namespace LightPredicts {

    using TYPE_DATA_READER =
        CPPFileIO::FullFileReader
            <Datas::QCDPreProcessor>
    ; //

    using TYPE_FLOAT_READER =
        CPPFileIO::FullFileReader
            <float>
    ; //

    inline void
    MakePlots () {
        MyHistN <6,false>
            MassSculptingMine (
                "MassSculptingMine",
                100,0.0,0.6
            )
        ; //
        MyHistN <6,false>
            MassSculptingShih (
                "MassSculptingShih",
                100,0.0,0.6
            )
        ; //
        TYPE_DATA_READER
            QCD(
                "./OUTS/QCD.united"
            )
        ; //
        TYPE_FLOAT_READER
            MYQCD(
                "./OUTS/QCD.error.5"
            )
        ; //
        TYPE_FLOAT_READER
            SHIHQCD(
                "./OUTS/SHIH/QCD.error.dat"
            )
        ; //
        for(size_t i=0;i<SHIHQCD();i++)
        if(QCD(i).pass_shih()){
            auto mass =
                QCD(i).mass()
            ; //
            if((0<mass)&&(mass<100)){
                MassSculptingMine
                    .Fill<0>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<0>
                        (SHIHQCD(i))
                ; //
            }
            if((100<mass)&&(mass<200)){
                MassSculptingMine
                    .Fill<1>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<1>
                        (SHIHQCD(i))
                ; //
            }
            if((200<mass)&&(mass<300)){
                MassSculptingMine
                    .Fill<2>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<2>
                        (SHIHQCD(i))
                ; //
            }
            if((300<mass)&&(mass<400)){
                MassSculptingMine
                    .Fill<3>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<3>
                        (SHIHQCD(i))
                ; //
            }
            if((400<mass)&&(mass<500)){
                MassSculptingMine
                    .Fill<4>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<4>
                        (SHIHQCD(i))
                ; //
            }
            if(500<mass){
                MassSculptingMine
                    .Fill<5>
                        (MYQCD(i))
                ; //
                MassSculptingShih
                    .Fill<5>
                        (SHIHQCD(i))
                ; //
            }
        }

    }

}
namespace LIGHT_PREDICT_MASSIVE {
    typedef
        CPPFileIO::FullFileReader
        <InputArrayType>
            MAIN_READER
    ; //
    typedef
        CPPFileIO::FileVector
        <FloatType>
            MAIN_WRITER
    ; //
    typedef
        CPPFileIO::FullFileReader
        <FloatType>
            ERROR_READER
    ; //
    inline ERROR_READER
    eval_predict (
        std::string infilename
    ) {
        std::string outfilename =
            infilename + ".error"
        ; //
        /* The prepration part: */ {
            MAIN_READER
                file_in (
                    infilename
                )
            ; //
            MAIN_WRITER
                file_out (
                    outfilename
                )
            ; //
            for(size_t i=0;i<file_in();i++){
                FloatType error =
                    sqrt(
                        file_in(i)
                            .L2Norm()
                    )
                ; //
                file_out
                    .push_back (
                        error
                    )
                ; //
            }
        }
        return
            ERROR_READER (
                outfilename
            )
        ; //
    }

    inline void
    eval_predict () {
        auto hists =
            GetHist<7>(
                "LIGHT_PREDICT_MASSIVE"
            )
        ; //
        auto n0 = eval_predict (
            "./OUTS/QCD.predict.dat.5"
        ) ; //
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        auto n1 = eval_predict (
            "./OUTS/QCD_M_0_200.dat.predict"
        ) ; //
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
            hists.Fill<6>(n1(i));
        }
        auto n2 = eval_predict (
            "./OUTS/QCD_M_200_400.dat.predict"
        ) ; //
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
            hists.Fill<6>(n2(i));
        }
        auto n3 = eval_predict (
            "./OUTS/QCD_M_400_600.dat.predict"
        ) ; //
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
            hists.Fill<6>(n3(i));
        }
        auto n4 = eval_predict (
            "./OUTS/QCD_M_600_800.dat.predict"
        ) ; //
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
            hists.Fill<6>(n4(i));
        }
        auto n5 = eval_predict (
            "./OUTS/QCD_M_800_INF.dat.predict"
        ) ; //
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
            hists.Fill<6>(n5(i));
        }
    }
}
namespace MASSIVE_PREDICTS {
    typedef
    CPPFileIO::FullFileReader
        <InputArrayType>
            MAIN_READER
    ; //
    typedef
    CPPFileIO::FileVector
        <FloatType>
            MAIN_WRITER
    ; //
    typedef CPPFileIO::FullFileReader
        <FloatType>
            ERROR_READER
    ; //

    inline ERROR_READER
    eval_predict (
        std::string infilename
    ) {
        std::string outfilename =
            infilename + ".error"
        ; //
        /* the main part: */ {
            MAIN_READER file_in (
                infilename
            ) ; //
            MAIN_WRITER file_out (
                outfilename
            ) ; //
            for(size_t i=0;i<file_in();i++){
                float error =
                        sqrt(
                            file_in(i).L2Norm()
                        )
                ; //
                file_out.push_back(
                    error
                ) ; //
            }
        }
        return
            ERROR_READER(
                outfilename
            )
        ; //
    }

    inline void HeavyPredictLight() {
        auto hists =
            GetHist<2>(
                "MASSIVE_PREDICT_LIGHT"
            )
        ; //
        auto n0 = eval_predict (
            "./OUTS/QCD.dat.predict_from_massive"
        ) ; //
        auto n1 = eval_predict (
            "./OUTS/QCD_M_0_200.dat.predict"
        ) ; //
        auto n2 = eval_predict (
            "./OUTS/QCD_M_200_400.dat.predict"
        ) ; //
        auto n3 = eval_predict (
            "./OUTS/QCD_M_400_600.dat.predict"
        ) ; //
        auto n4 = eval_predict (
            "./OUTS/QCD_M_600_800.dat.predict"
        ) ; //
        auto n5 = eval_predict (
            "./OUTS/QCD_M_800_INF.dat.predict"
        ) ; //
        for(size_t i=0;i<n0();i++){
            hists.Fill<1>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<0>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<0>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<0>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<0>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<0>(n5(i));
        }
    }
    inline void PREDICT_ALL () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/0/QCD.dat"
                )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/1/QCD.dat"
                )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/2/QCD.dat"
                )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/3/QCD.dat"
                )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/4/QCD.dat"
                )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/ALL/5/QCD.dat"
                )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                )
        ; //
        auto hists = GetHist <> ("MASSIVE_PREDICT_ALL") ; //
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }
    inline void PREDICT_0_200 () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/0/QCD.dat"
                    )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/1/QCD.dat"
                    )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/2/QCD.dat"
                    )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/3/QCD.dat"
                    )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/4/QCD.dat"
                    )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/0_200/5/QCD.dat"
                    )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                    )
        ; //
        auto hists = GetHist <> ("MASSIVE_PREDICT_0_200") ;//
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }
    inline void PREDICT_200_400 () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/0/QCD.dat"
                    )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/1/QCD.dat"
                    )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/2/QCD.dat"
                    )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/3/QCD.dat"
                    )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/4/QCD.dat"
                    )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/200_400/5/QCD.dat"
                    )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                    )
        ; //
        auto hists = GetHist <> ("MASSIVE_PREDICT_200_400") ;//
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }
    inline void PREDICT_400_600 () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/0/QCD.dat"
                    )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/1/QCD.dat"
                    )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/2/QCD.dat"
                    )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/3/QCD.dat"
                    )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/4/QCD.dat"
                    )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/400_600/5/QCD.dat"
                    )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                    )
        ; //
        auto hists = GetHist <> ("MASSIVE_PREDICT_400_600") ;//
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }
    inline void PREDICT_600_800 () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/0/QCD.dat"
                )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/1/QCD.dat"
                )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/2/QCD.dat"
                )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/3/QCD.dat"
                )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/4/QCD.dat"
                )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/600_800/5/QCD.dat"
                )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                )
        ; //
        auto hists = GetHist <> ("MASSIVE_PREDICT_600_800") ;//
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }
    inline void PREDICT_800_INF () {
        auto n0 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/0/QCD.dat"
                )
        ; //
        auto n1 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/1/QCD.dat"
                )
        ; //
        auto n2 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/2/QCD.dat"
                )
        ; //
        auto n3 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/3/QCD.dat"
                )
        ; //
        auto n4 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/4/QCD.dat"
                )
        ; //
        auto n5 =
                eval_predict(
                    "./OUTS/PREDICT_MASSIVE/800_INF/5/QCD.dat"
                )
        ; //
        auto n6 =
                eval_predict(
                    "./OUTS/TOP.predict.dat.5"
                )
        ; //
        auto hists =
            GetHist <6> (
                "MASSIVE_PREDICT_800_INF"
            )
        ;//
        hists.SetLogScale(false);
        for(size_t i=0;i<n0();i++){
            hists.Fill<0>(n0(i));
        }
        for(size_t i=0;i<n1();i++){
            hists.Fill<1>(n1(i));
        }
        for(size_t i=0;i<n2();i++){
            hists.Fill<2>(n2(i));
        }
        for(size_t i=0;i<n3();i++){
            hists.Fill<3>(n3(i));
        }
        for(size_t i=0;i<n4();i++){
            hists.Fill<4>(n4(i));
        }
        for(size_t i=0;i<n5();i++){
            hists.Fill<5>(n5(i));
        }
//        for(size_t i=0;i<n6();i++){
//            hists.Fill<6>(n6(i));
//        }
    }

    inline void
    PREDICT_FULL_FROM_MASSIVE () {
        HeavyPredictLight () ;
        PREDICT_ALL       () ;
        PREDICT_0_200     () ;
        PREDICT_200_400   () ;
        PREDICT_400_600   () ;
        PREDICT_600_800   () ;
        PREDICT_800_INF   () ;
    }
}

#ifdef USE_MXNET
#include "mxnet-cpp/MxNetCpp.h"
namespace MainMXNet {
    using namespace mxnet::cpp;

    using myreader = CPPFileIO::FullFileReader <InputArrayType> ;

    inline void WriteNDArrayToFile ( NDArray const & InArray, std::string const FileName ) {
        size_t limit = InArray.Size () ;
        const mx_float * data = InArray.GetData () ;
        CPPFileIO::FileFD Writer (FileName) ; Writer.writefile () ;
        Writer.multiwrite2file(data[0],limit);
    }

    inline void ReadNDArrayToFile ( NDArray & InArray, std::string FileName ) {
        CPPFileIO::FullFileReader <mx_float> data (FileName) ;
        InArray.SyncCopyFromCPU ( &(data(0)) , data() ) ;
    }

    constexpr int Sizes[6] = {
        InputArrayType::SizeX()*InputArrayType::SizeY(),
        33 * 33 ,
        26 * 26 ,
        19 * 19 ,
        12 * 12 ,
         5 *  5
    };

    namespace SA {
        class MXNETAutoEncoder {
        private:
            Context ctx ;
            NDArray * MainQCDData ;
            size_t batch_size ;
            std::map < std::string , NDArray > args_map ;
            myreader RQCD ;
        private:
            inline void WritePars ( std::string name )
            { WriteNDArrayToFile ( args_map[name] , Path_MXNET_Pars() + name ) ; }
            inline void ReadPars ( std::string name )
            { ReadNDArrayToFile ( args_map[name] , Path_MXNET_Pars() + name ) ; }
            inline void WritePars () {
                WritePars ( "L1_weight"    ) ; WritePars ( "L1_bias"    ) ;
                WritePars ( "L2_weight"    ) ; WritePars ( "L2_bias"    ) ;
                WritePars ( "L3_weight"    ) ; WritePars ( "L3_bias"    ) ;
                WritePars ( "L4_weight"    ) ; WritePars ( "L4_bias"    ) ;
                WritePars ( "L5_weight"    ) ; WritePars ( "L5_bias"    ) ;
                WritePars ( "L1Bar_weight" ) ; WritePars ( "L1Bar_bias" ) ;
                WritePars ( "L2Bar_weight" ) ; WritePars ( "L2Bar_bias" ) ;
                WritePars ( "L3Bar_weight" ) ; WritePars ( "L3Bar_bias" ) ;
                WritePars ( "L4Bar_weight" ) ; WritePars ( "L4Bar_bias" ) ;
                WritePars ( "L5Bar_weight" ) ; WritePars ( "L5Bar_bias" ) ;
                WritePars ( "Id_weight"    ) ; WritePars ( "Id_bias"    ) ;
            }
            inline void ReadPars () {
                ReadPars ( "L1_weight"    ) ; ReadPars ( "L1_bias"    ) ;
                ReadPars ( "L2_weight"    ) ; ReadPars ( "L2_bias"    ) ;
                ReadPars ( "L3_weight"    ) ; ReadPars ( "L3_bias"    ) ;
                ReadPars ( "L4_weight"    ) ; ReadPars ( "L4_bias"    ) ;
                ReadPars ( "L5_weight"    ) ; ReadPars ( "L5_bias"    ) ;
                ReadPars ( "L1Bar_weight" ) ; ReadPars ( "L1Bar_bias" ) ;
                ReadPars ( "L2Bar_weight" ) ; ReadPars ( "L2Bar_bias" ) ;
                ReadPars ( "L3Bar_weight" ) ; ReadPars ( "L3Bar_bias" ) ;
                ReadPars ( "L4Bar_weight" ) ; ReadPars ( "L4Bar_bias" ) ;
                ReadPars ( "L5Bar_weight" ) ; ReadPars ( "L5Bar_bias" ) ;
                ReadPars ( "Id_weight"    ) ; ReadPars ( "Id_bias"    ) ;
            }
            inline void PrepareData (size_t i) {
                args_map["data"].SyncCopyFromCPU
                ( &(RQCD(i)[0][0]) , InputArrayType::Size()*batch_size ) ;
                args_map["data_label"].SyncCopyFromCPU
                ( &(RQCD(i)[0][0]) , InputArrayType::Size()*batch_size ) ;
                NDArray::WaitAll();
            }
            inline void PrepareDataLabel () {
                Shape batchshape ( batch_size , FlatArrayType::Size() ) ;
                args_map ["data"]       = NDArray (batchshape,ctx) ;
                args_map ["data_label"] = NDArray (batchshape,ctx) ;
                PrepareData (0) ;
                NDArray::WaitAll();
            }
            inline Symbol GetNet () {

                batch_size = 256 ;
                PrepareDataLabel () ;

                auto input_data   = Symbol::Variable ( "data"       ) ;
                auto target_label = Symbol::Variable ( "data_label" ) ;

                auto L1 = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[1])
                            .SetParam("no_bias",false)
                            .SetInput("data",input_data)
                            .CreateSymbol("L1");
                auto A1 = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L1)
                            .CreateSymbol("A1");

                auto L2 = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[2])
                            .SetParam("no_bias",false)
                            .SetInput("data",A1)
                            .CreateSymbol("L2");
                auto A2 = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L2)
                            .CreateSymbol("A2");

                auto L3 = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[3])
                            .SetParam("no_bias",false)
                            .SetInput("data",A2)
                            .CreateSymbol("L3");
                auto A3 = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L3)
                            .CreateSymbol("A3");

                auto L4 = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[4])
                            .SetParam("no_bias",false)
                            .SetInput("data",A3)
                            .CreateSymbol("L4");
                auto A4 = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L4)
                            .CreateSymbol("A4");

                auto L5 = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[5])
                            .SetParam("no_bias",false)
                            .SetInput("data",A4)
                            .CreateSymbol("L5");
                auto A5 = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L5)
                            .CreateSymbol("A5");

                auto L5Bar = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[4])
                            .SetParam("no_bias",false)
                            .SetInput("data",A5)
                            .CreateSymbol("L5Bar");
                auto A5Bar = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L5Bar)
                            .CreateSymbol("A5Bar");

                auto L4Bar = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[3])
                            .SetParam("no_bias",false)
                            .SetInput("data",A5Bar)
                            .CreateSymbol("L4Bar");
                auto A4Bar = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L4Bar)
                            .CreateSymbol("A4Bar");

                auto L3Bar = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[2])
                            .SetParam("no_bias",false)
                            .SetInput("data",A4Bar)
                            .CreateSymbol("L3Bar");
                auto A3Bar = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L3Bar)
                            .CreateSymbol("A3Bar");

                auto L2Bar = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[1])
                            .SetParam("no_bias",false)
                            .SetInput("data",A3Bar)
                            .CreateSymbol("L2Bar");
                auto A2Bar = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L2Bar)
                            .CreateSymbol("A2Bar");

                auto L1Bar = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[0])
                            .SetParam("no_bias",false)
                            .SetInput("data",A2Bar)
                            .CreateSymbol("L1Bar");
                auto A1Bar = Operator("Activation")
                            .SetParam("act_type","relu")
                            .SetInput("data",L1Bar)
                            .CreateSymbol("A1Bar");

                auto Id = Operator("FullyConnected")
                            .SetParam("num_hidden",Sizes[0])
                            .SetParam("no_bias",false)
                            .SetInput("data",A1Bar)
                            .CreateSymbol("Id");

                auto Sm = Operator("softmax")
                            .SetParam("axis", 1)
                            .SetInput("data", Id)
                            .CreateSymbol("Sm");

                auto Net = Operator("LinearRegressionOutput")
                            //.SetParam("ignore_label", -1)
                            .SetParam("grad_scale", 1)
                            //.SetParam("use_ignore", false)
                            //.SetParam("normalization", "null") /*batch,null,valid */
                            .SetInput("data", Sm)
                            .SetInput("label",target_label)
                            .CreateSymbol("Net");

                for (auto s : Net.ListArguments()) { LG << s; }
                Net.InferArgsMap ( ctx , &args_map , args_map ) ;
                return Net ;
            }

            inline float ValAccuracy ( Symbol lenet ) {
                float correct_count = 0;
                size_t all_count = 0;
                size_t start_index = 0;
                while (start_index < RQCD()) {
                    if ( start_index + batch_size > RQCD() ) { start_index = RQCD() - batch_size; }
                    PrepareData(start_index);
                    NDArray::WaitAll();
                    Executor * exe = lenet.SimpleBind(ctx, args_map);
                    exe->Forward(false);
                    const auto & out = exe->outputs;
                    NDArray out_cpu = out[0].Copy(ctx);
                    NDArray::WaitAll();
                    FlatArrayType * dptr_out   = (FlatArrayType *) out_cpu.GetData();
                    FlatArrayType * dptr_label = (FlatArrayType *) &(RQCD(start_index)) ;
                    for (int i = 0; i < batch_size; ++i) {
                        FlatArrayType & pred = dptr_out   [i] ;
                        FlatArrayType & in   = dptr_label [i] ;
                        pred -= in ;
                        correct_count += pred.L1Norm()/in.L1Norm() ;
                    }
                    delete exe ;
                    all_count += batch_size ;
                    start_index += batch_size ;
                }
                return correct_count / (float) all_count ;
            }
            inline void DebugPRT ( NDArray & in , size_t limit ) {
                float const * ptr = in.GetData();
                for(size_t i=0;i<limit;i++)if(CPPFileIO::mymod(ptr[i])>0.0000001){
                    printf("%e\n",ptr[i]);
                }
            }
        public:
            inline void Create () {
                batch_size = 256 ;
                LG << RQCD() ;
                auto Net = GetNet () ;

                Optimizer * opt = OptimizerRegistry::Find ("adam") ;
                opt->SetParam("lr", 0.0001);

                auto * exe = Net.SimpleBind(ctx, args_map);
                auto arg_names = Net.ListArguments();

                for(size_t asd=0;asd<1;asd++){
                    LG << "Iter " << asd << ", accuracy: " << ValAccuracy(Net);
                    for (int ITER = 0; ITER < 1; ++ITER) {
                        printf("TRAINING...\n");
                        size_t start_index = 0;
                         while (start_index < RQCD()) {
                            if ( start_index + batch_size > RQCD() )
                            { start_index = RQCD() - batch_size ; }
                            PrepareData(start_index);
                            exe->Forward(true); exe->Backward();
                            if (false) for (size_t i = 0; i < arg_names.size(); ++i) {
                                if (arg_names[i] == "data" || arg_names[i] == "data_label") continue;
                                opt->Update(i, exe->arg_arrays[i], exe->grad_arrays[i]);
                            }
                            start_index += batch_size;
                        }
                        WritePars () ;
                    }
                    opt->SetParam("lr", 0.0001);
                }
                delete exe;
                printf("Came here...\n");
            }
            inline void Train () {
                LG << RQCD() ;
                auto Net = GetNet () ;
                Optimizer * opt = OptimizerRegistry::Find ("adam") ;
                opt->SetParam("lr", 0.00000001);
                auto * exe = Net.SimpleBind(ctx, args_map);
                auto arg_names = Net.ListArguments();
                ReadPars () ;
                for(size_t asd=0;asd<100;asd++){
                    LG << "Iter " << asd << ", accuracy: " << ValAccuracy(Net);
                    for (int ITER = 0; ITER < 1; ++ITER) {
                        printf("TRAINING...\n");
                        size_t start_index = 0;
                        while (start_index < RQCD()) {
                            if ( start_index + batch_size > RQCD() )
                            { start_index = RQCD() - batch_size ; }
                            PrepareData(start_index);
                            exe->Forward(true); exe->Backward();
                            for (size_t i = 0; i < arg_names.size(); ++i) {
                                if (
                                    ( arg_names[i] != "data" ) ||
                                    ( arg_names[i] != "data_label" )
                                ) {
                                    opt->Update ( i , exe->arg_arrays[i] , exe->grad_arrays[i] ) ;
                                    //DebugPRT (exe->grad_arrays[i],10) ;
                                }
                            }
                            start_index += batch_size;
                        }
                        WritePars () ;
                    }
                    opt->SetParam("lr", 0.00000001);
                }
                delete exe;
                printf("Came here...\n");
            }
            MXNETAutoEncoder() : ctx(Context::cpu()) , RQCD("./OUTS/QCD.dat") { batch_size = 256 ; }
            ~MXNETAutoEncoder(){}
        } ;
    }

    namespace S {
        inline void Create () {
            using namespace SA ;
            MXNETAutoEncoder slave ;
            slave.Create();
        }
        inline void Train (int i) {
            if (i==0) {
                using namespace SA ;
                MXNETAutoEncoder slave ;
                slave.Train();
            }
        }
    }
}
#endif
