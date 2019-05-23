#include <TH1F.h>
#include <TColor.h>
#include <TCanvas.h>
namespace NewHEPHeaders /*Nice Root Histogramming: */ {
    namespace Histograms {
        template <size_t Num, bool compare=false> class MyHistN {
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
                mkdir((const char*)"./GRAPHS",(mode_t)0755);
                std::string outname  = "./GRAPHS/" + histname + ".pdf" ;
                std::string outnameC = "./GRAPHS/" + histname + ".C"   ;
                C.SaveAs(&(outname[0])); C.SaveAs(&(outnameC[0]));
                DeleteAll () ;
            }
            inline void AllocateAll (size_t nbins, double min, double max) {
                for(size_t ii=0;ii<Num;ii++){
                    char tmp[1024]; sprintf(tmp,"%s_%ld",&(histname[0]),ii);
                    histNnames[ii] = std::string(tmp);
                    Hists[ii] = new TH1F ( & ( histNnames[ii][0] ) , & ( histname [0] ) , nbins , min , max ) ;
                }
            }
            inline void SetLogScale (bool _logscale) {logscale=_logscale;}
            MyHistN  (std::string _histname, size_t nbins, double min, double max) : histname(_histname)
            { AllocateAll (nbins,min,max) ; logscale=false; }
            ~MyHistN () { Write () ; }
        };
    }
}
