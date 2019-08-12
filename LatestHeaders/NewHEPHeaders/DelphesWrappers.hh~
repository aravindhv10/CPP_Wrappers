#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include "TClonesArray.h"
#include "TObject.h"
#include "classes/DelphesClasses.h"

namespace NewHEPHeaders /* The Delphes part: */ {

    template <typename T> class DelphesReader : public T {
    public:
		size_t limit ;
        typedef DelphesReader<T> TYPE_Self ;
        template <typename T2> inline void operator () (T2&analyzer) {
            if (this->fChain != 0) {
                long nentries =
					this->fChain->GetEntriesFast()
				; //
                long nbytes = 0, nb = 0;
                for (
					long jentry=0;
					((jentry<nentries)&&(jentry<limit));
					jentry++
				) {
                    long ientry = this->LoadTree(jentry);
                    if((jentry%1000)==0){printf("Analyzed %ld events\n",jentry);}
                    if (ientry >= 0) {
                        nb = this->fChain->GetEntry(jentry);
                        nbytes += nb;
                        analyzer(this[0]);
                    }
                }
            }
        }
    } ;

    template < typename TR=double, typename TI=int >
    class DelphesVectorsList : public std::vector <VECTORS::DelphesVectors<TR,TI>> {
    public:
        typedef VECTORS::DelphesVectors <TR,TI> TYPE_Element ;
        typedef DelphesVectorsList      <TR,TI> TYPE_Self    ;
        VECTORS::GenParticles <TR,TI> GenParts ;
        inline double CalcPtSum (size_t index, double cone) const {
            double ret = 0;
            for(size_t i=0;i<this->size();i++) if(i!=index) if(this[0][i](this[0][index])<cone)
            {ret+=this[0][i].pt();}
            return ret ;
        }
        inline size_t Count_Tracks ( fastjet::PseudoJet & injet ) {
            TYPE_Self & self = this[0] ;
            size_t ret = 0 ;
            pseudojets cons = injet.constituents();
            for(size_t i=0;i<cons.size();i++)
            { int j = cons[i].user_index() ; ret += CPPFileIO::mymod (self[j].Charge) ; }
            return ret ;
        }
        inline double HCal_Fraction ( fastjet::PseudoJet & injet ) {
            TYPE_Self & self = this[0] ;
            double ret = 0 ;
            pseudojets cons = injet.constituents();
            for(size_t i=0;i<cons.size();i++)
            { int j=cons[i].user_index() ; ret += self[j].Ehad ; }
            return ret ;
        }
        inline double ECal_Fraction ( fastjet::PseudoJet & injet ) {
            TYPE_Self & self = this[0] ;
            double ret = 0 ;
            pseudojets cons = injet.constituents();
            for(size_t i=0;i<cons.size();i++)
            { int j=cons[i].user_index() ; ret += self[j].Eem ; }
            return ret ;
        }
        template <typename T1> inline void ReadFromDelphes (T1 const & inref) {
            GenParts.ReadFromDelphes(inref);
            size_t limit_EFlowNeutralHadron = inref.EFlowNeutralHadron_ ;
            size_t limit_EFlowPhoton        = inref.EFlowPhoton_        ;
            size_t limit_EFlowTrack         = inref.EFlowTrack_         ;
            /* Prepare the vectors: */ {
                this->clear();
                this->reserve(limit_EFlowNeutralHadron+limit_EFlowPhoton+limit_EFlowTrack);
            }
            /* Read Neutral Hadrons : */ {
                for(size_t i=0;i<limit_EFlowNeutralHadron;i++){
                    TYPE_Element tmp2 ; /* Read in the track vectors: */ {
                        tmp2.SetPtEtaPhiM (
                            inref.EFlowNeutralHadron_ET  [i] ,
                            inref.EFlowNeutralHadron_Eta [i] ,
                            inref.EFlowNeutralHadron_Phi [i] ,
                            0
                        );
                        /* Set the energy fractions: */ {
                            tmp2.Eem    = inref.EFlowNeutralHadron_Eem  [i] ;
                            tmp2.Ehad   = inref.EFlowNeutralHadron_Ehad [i] ;
                            tmp2.Emu    = 0                                 ;
                            tmp2.Charge = 0                                 ;
                        }
                    }
                    this->push_back(tmp2);
                }
            }
            /* Read Photons         : */ {
                for(size_t i=0;i<limit_EFlowPhoton;i++){
                    TYPE_Element tmp2 ; /* Read in the track vectors: */ {
                        tmp2.SetPtEtaPhiM (
                            inref.EFlowPhoton_ET  [i] ,
                            inref.EFlowPhoton_Eta [i] ,
                            inref.EFlowPhoton_Phi [i] ,
                            0
                        );
                        /* Set the energy fractions: */ {
                            tmp2.Eem    = inref.EFlowPhoton_Eem  [i] ;
                            tmp2.Ehad   = inref.EFlowPhoton_Ehad [i] ;
                            tmp2.Emu    = 0                          ;
                            tmp2.Charge = 0                          ;
                        }
                    }
                    this->push_back (tmp2) ;
                }
            }
            /* Read tracks          : */ {
                for(size_t i=0;i<limit_EFlowTrack;i++){
                    TYPE_Element tmp2 ; /* Read in the track vectors: */ {
                        tmp2.SetPtEtaPhiM (
                            inref.EFlowTrack_PT  [i] ,
                            inref.EFlowTrack_Eta [i] ,
                            inref.EFlowTrack_Phi [i] ,
                            0
                        ); /* Set the energy fractions: */ {
                            if ( CPPFileIO::mymod (inref.EFlowTrack_PID[i]) == PID::MUON ) {
                                tmp2.Eem  = tmp2[3] * 0.0 ;
                                tmp2.Ehad = tmp2[3] * 0.0 ;
                                tmp2.Emu  = tmp2[3] * 1.0 ;
                            } else if ( CPPFileIO::mymod (inref.EFlowTrack_PID[i]) == PID::ELECTRON ) {
                                tmp2.Eem  = tmp2[3] * 1.0 ;
                                tmp2.Ehad = tmp2[3] * 0.0 ;
                                tmp2.Emu  = tmp2[3] * 0.0 ;
                            } else {
                                tmp2.Eem  = tmp2[3] * 0.01 ;
                                tmp2.Ehad = tmp2[3] * 0.99 ;
                                tmp2.Emu  = tmp2[3] * 0.00 ;
                            }
                        }
                    }
                    tmp2.Charge = inref.EFlowTrack_Charge [i] ;
                    this->push_back ( tmp2 ) ;
                }
            }
        }
    } ;

    template < typename TR=double, typename TI=int >
    class FullDelphesReader : public DelphesVectorsList <TR,TI> {
    public:
        pseudojets tofastjet ;
        inline void Prepare (double const isocone=0.3, double const isoratio=0.15) {
            tofastjet.clear();
            for (size_t i=0;i<this->size();i++) {
                bool isolepton = (this[0][i].IsLepton())&&(this[0][i].pt()>20)&&(this[0][i].meta()<2.5) ;
                if (isolepton) {
                    double ptratio = this->CalcPtSum(i,isocone) / this[0][i].pt();
                    isolepton = (ptratio<isoratio) ;
                }
                if (!isolepton){
                    fastjet::PseudoJet tmpjet = this[0][i].getpseudojet () ;
                    tmpjet.set_user_index ((int)i) ;
                    tofastjet.push_back (tmpjet) ;
                }
            }
        }
    } ;

}
