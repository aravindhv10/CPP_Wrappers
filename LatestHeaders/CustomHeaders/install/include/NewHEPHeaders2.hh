#ifndef NewHEPHeaders2_HH
#define NewHEPHeaders2_HH
#include <CPPFileIO.hh>
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <fastjet/Selector.hh>
#include <fastjet/tools/JHTopTagger.hh>
#include "HEPTopTagger.cc"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TCanvas.h"
namespace NewHEPHeaders {
    const bool DEBUG = false;
    namespace CONSTANTS {
        const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
        const double PI2 = PI * 2.0;
    }
    namespace PID {
        const long DOWN = 1;
        const long UP = 2;
        const long STRANGE = 3;
        const long CHARM = 4;
        const long BOTTOM = 5;
        const long TOP = 6;
        const long GAMMA = 22;
        const long Z = 23;
        const long W = 24;
        const long H0 = 25;
        const long H1 = 35;
        const long A0 = 36;
        const long CHARGED_HIGGS = 37;
        const long GLUON = 21;
        const long ELECTRON_NU = 12;
        const long MUON_NU = 14;
        const long TAU_NU = 16;
        const long ELECTRON = 11;
        const long MUON = 13;
        const long TAU = 15;
        const long CHI10 = 1000022;
        const long CHI20 = 1000023;
    }
    namespace MASS {
        const double TOP = 173.340;
        const double W = 80.385;
        const double Z = 91.190;
        const double H0 = 125.000;
    }
    inline bool detectable (long pid) {
        pid = CPPFileIO::mymod (pid);
        if (pid == PID::ELECTRON_NU) {
            return false;
        }
        if (pid == PID::MUON_NU) {
            return false;
        }
        if (pid == PID::TAU_NU) {
            return false;
        }
        if (pid == PID::CHI10) {
            return false;
        }
        if (pid == PID::CHI20) {
            return false;
        }
        return true;
    }
    inline bool islepton (long pid) {
        pid = CPPFileIO::mymod (pid);
        if (pid == PID::ELECTRON) {
            return true;
        }
        if (pid == PID::MUON) {
            return true;
        }
        if (pid == PID::TAU) {
            return true;
        }
        return false;
    }
    inline bool isblike (long pid) {
        pid=CPPFileIO::mymod(pid);
        if(pid<100) {
            if(pid==PID::BOTTOM) {return true;}
            else {return false;}
        } else {
            while(pid>0) {
                long tmp = pid%10;
                if(tmp==PID::BOTTOM) {return true;}
                pid=pid/10;
            }
            return false ;
        }
    }
    namespace VECTORS {
        double VECTOR_EQUALITY_LIMIT = 50.0;
        template < typename TR > class plane2vector {
        private:
        public:
            TR x[2];
            void construct (TR _x = 0, TR _y = 0) {
                x[0] = _x;
                x[1] = _y;
            }
            plane2vector (TR _x = 0, TR _y = 0) {
                construct (_x, _y);
            }
            plane2vector (const plane2vector < TR > &c) {
                construct (c.x[0], c.x[1]);
            }
            ~plane2vector () {
            }
            inline TR pt2 () {
                return (x[0] * x[0]) + (x[1] * x[1]);
            }
            inline double pt () {
                return (sqrt (pt2 ()));
            }
            inline TR safenorm2 () {
                TR mag = pt2 ();

                if (CPPFileIO::mymod (mag) < 0.0000000001) {
                    mag = CPPFileIO::mysign (mag) * 0.0000000001;
                }
                return mag;
            }
            inline double phi () {
                double ret = acos (x[0] / sqrt (safenorm2 ()));

                if (x[1] < 0) {
                    ret = CONSTANTS::PI2 - ret;
                }
                return ret;
            }
            inline double dphi (plane2vector < TR > b) {
                double ret = CPPFileIO::mymod (b.phi () - phi ());

                if (ret > CONSTANTS::PI) {
                    ret = CONSTANTS::PI2 - ret;
                }
                return ret;
            }
            inline plane2vector < TR > operator + (plane2vector < TR > b) {
                return plane2vector < TR > (x[0] + b.x[0], x[1] + b.x[1]);
            }
            inline plane2vector < TR > operator - (plane2vector < TR > b) {
                return plane2vector < TR > (x[0] - b.x[0], x[1] - b.x[1]);
            }
            inline TR operator * (plane2vector < TR > b) {
                return (x[0] * b.x[0]) + (x[1] * b.x[1]);
            }
            inline plane2vector < TR > operator * (TR b) {
                return plane2vector < TR > (x[0] * b, x[1] * b);
            }
            inline plane2vector < TR > operator / (TR b) {
                return plane2vector < TR > (x[0] / b, x[1] / b);
            }
            inline double operator  () (plane2vector < TR > b) {
                return dphi (b);
            }
            inline plane2vector < TR > flip () {
                return plane2vector < TR > (-x[0], -x[1]);
            }
            inline plane2vector < TR > dir () {
                plane2vector < TR > ret (x[0], x[1]);
                double mag = sqrt (ret.safenorm2 ());

                ret = ret / mag;
                return ret;
            }
            inline bool operator > (plane2vector < TR > b) {
                return pt2 () > b.pt2 ();
            }
            inline bool operator < (plane2vector < TR > b) {
                return pt2 () < b.pt2 ();
            }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) {
                return f.multiwrite2file (*this);
            }
            inline ssize_t operator << (CPPFileIO::FileFD & f) {
                return f.multiread2file (*this);
            }
            inline TR & operator [] (size_t i) {
                return x[i];
            }
            inline void clearthis () {
                x[0] = 0;
                x[1] = 0;
            }
            inline bool operator == (plane2vector < TR > b) {
                plane2vector < TR > tmp (x[0], x[1]);
                tmp = tmp - b;
                TR diff = tmp.pt2 ();

                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }
        };
        template < typename TR > class euclid3vector {
        private:
        public:
            plane2vector < TR > xy;
            TR z;

            euclid3vector (TR _x = 0, TR _y = 0, TR _z = 0):xy (_x, _y) {
                z = _z;
            }
            euclid3vector (plane2vector < TR > a, TR _z = 0):xy (a) {
                z = _z;
            }
            euclid3vector (const euclid3vector < TR > &a):xy (a.xy) {
                z = a.z;
            }
            inline double phi () {
                return xy.phi ();
            }
            inline TR pt2 () {
                return xy.pt2 ();
            }
            inline double pt () {
                return xy.pt ();
            }
            inline euclid3vector < TR > operator + (euclid3vector < TR > a) {
                return euclid3vector < TR > (xy + a.xy, z + a.z);
            }
            inline euclid3vector < TR > operator - (euclid3vector < TR > a) {
                return euclid3vector < TR > (xy - a.xy, z - a.z);
            }
            inline TR operator * (euclid3vector < TR > a) {
                return (xy * a.xy) + (z * a.z);
            }
            inline euclid3vector < TR > operator * (TR a) {
                return euclid3vector < TR > (xy * a, z * a);
            }
            inline euclid3vector < TR > operator / (TR a) {
                return euclid3vector < TR > (xy / a, z / a);
            }
            inline TR p2 () {
                return xy.pt2 () + (z * z);
            }
            inline double p () {
                return sqrt (p2 ());
            }
            inline TR & operator [] (size_t ret) {
                if (ret > 1) {
                    return z;
                } else {
                    return xy[ret];
                }
            }
            inline double eta () {
                double tmp_p = p ();

                return 0.5 * log ((tmp_p + z) / (tmp_p - z));
            }
            inline double meta () {
                return CPPFileIO::mymod (eta ());
            }
            inline double cone2 (euclid3vector < TR > b) {
                double tphi = xy.dphi (b.xy);

                tphi = tphi * tphi;
                double teta = eta () - b.eta ();

                teta = teta * teta;
                double ret = teta + tphi;

                return ret;
            }
            inline double cone (euclid3vector < TR > b) {
                return sqrt (cone2 (b));
            }
            inline double dphi (euclid3vector < TR > b) {
                double tphi = xy.dphi (b.xy);

                return tphi;
            }
            inline double operator  () (euclid3vector < TR > b) {
                return cone (b);
            }
            inline TR safenorm2 () {
                TR mag = xy.pt2 () + (z * z);

                if (CPPFileIO::mymod (mag) < 0.0000000001) {
                    mag = CPPFileIO::mysign (mag) * 0.0000000001;
                }
                return mag;
            }
            inline euclid3vector < TR > flip () {
                return euclid3vector < TR > (xy.flip (), -z);
            }
            inline euclid3vector < TR > trans () {
                return euclid3vector < TR > (xy, 0);
            }
            inline euclid3vector < TR > dir () {
                euclid3vector < TR > ret (*this);
                double mag = sqrt (ret.safenorm2 ());

                ret = ret / mag;
                return ret;
            }
            inline bool operator > (euclid3vector < TR > b) {
                return pt2 () > b.pt2 ();
            }
            inline bool operator < (euclid3vector < TR > b) {
                return pt2 () < b.pt2 ();
            }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) {
                return f.multiwrite2file (*this);
            }
            inline ssize_t operator << (CPPFileIO::FileFD & f) {
                return f.multiread2file (*this);
            }
            inline void clearthis () {
                xy = plane2vector < TR > (0, 0);
                z = 0;
            }
            inline bool operator == (euclid3vector < TR > b) {
                euclid3vector < TR > tmp = (*this) - b;
                TR diff = tmp.p2 ();

                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }
        };
        template < typename TR > class lorentz4vector {
        private:
        public:
            euclid3vector < TR > xyz;
            TR t;

            lorentz4vector (TR _x = 0, TR _y = 0, TR _z = 0, TR _t = -1):xyz (_x, _y, _z) {
                t = _t;
            }
            lorentz4vector (const euclid3vector < TR > a, TR _t = -1):xyz (a) {
                t = _t;
            }
            lorentz4vector (const lorentz4vector < TR > &a):xyz (a.xyz) {
                t = a.t;
            }
            inline TR & operator [] (size_t ref) {
                if (ref > 2) {
                    return t;
                } else {
                    return xyz[ref];
                }
            }
            inline TR pt2 () {
                return xyz.pt2 ();
            }
            inline double pt () {
                return xyz.pt ();
            }
            inline TR p2 () {
                return xyz.p2 ();
            }
            inline double p () {
                return xyz.p ();
            }
            inline double phi () {
                return xyz.phi ();
            }
            inline TR m2 () {
                return CPPFileIO::mymod ((t * t) - p2 ());
            }
            inline TR n2 () {
                return CPPFileIO::mymod ((t * t) + p2 ());
            }
            inline double eta () {
                return (0.5 * log ((t + xyz.z) / (t - xyz.z)));
            }
            inline double meta () {
                return CPPFileIO::mymod (eta ());
            }
            inline double peta () {
                return xyz.eta ();
            }
            inline double pmeta () {
                return xyz.meta ();
            }
            inline double m () {
                return sqrt (m2 ());
            }
            inline double n () {
                return sqrt (n2 ());
            }
            inline double dphi (lorentz4vector < TR > b) {
                return xyz.dphi (b.xyz);
            }
            inline lorentz4vector < TR > operator + (lorentz4vector < TR > b) {
                return lorentz4vector < TR > (xyz + b.xyz, t + b.t);
            }
            inline lorentz4vector < TR > operator - (lorentz4vector < TR > b) {
                return lorentz4vector < TR > (xyz - b.xyz, t - b.t);
            }
            inline TR operator * (lorentz4vector < TR > b) {
                return (t * b.t) - (xyz * b.xyz);
            }
            inline lorentz4vector < TR > operator * (TR b) {
                return lorentz4vector < TR > (xyz * b, t * b);
            }
            inline lorentz4vector < TR > operator / (TR b) {
                return lorentz4vector < TR > (xyz / b, t / b);
            }
            inline double pcone2 (lorentz4vector < TR > b) {
                double tphi = xyz.dphi (b.xyz);

                tphi = tphi * tphi;
                double teta = peta () - b.peta ();

                teta = teta * teta;
                double ret = teta + tphi;

                return ret;
            }
            inline double pcone (lorentz4vector < TR > b) {
                return sqrt (pcone2 (b));
            }
            inline double cone2 (lorentz4vector < TR > b) {
                double tphi = xyz.dphi (b.xyz);

                tphi = tphi * tphi;
                double teta = eta () - b.eta ();

                teta = teta * teta;
                double ret = teta + tphi;

                return ret;
            }
            inline double cone (lorentz4vector < TR > b) {
                return sqrt (cone2 (b));
            }
            inline double operator  () (lorentz4vector < TR > b) {
                return cone (b);
            }
            inline lorentz4vector < TR > flip () {
                return lorentz4vector < TR > (xyz.flip (), t);
            }
            inline lorentz4vector < TR > trans () {
                return lorentz4vector < TR > (xyz.trans (), t);
            }
            inline lorentz4vector < TR > dir () {
                return lorentz4vector < TR > (xyz.dir (), t);
            }
            inline bool operator > (lorentz4vector < TR > b) {
                return pt2 () > b.pt2 ();
            }
            inline bool operator < (lorentz4vector < TR > b) {
                return pt2 () < b.pt2 ();
            }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) {
                return f.multiwrite2file (*this);
            }
            inline ssize_t operator << (CPPFileIO::FileFD & f) {
                return f.multiread2file (*this);
            }
            inline bool cleared () {
                return (t < 0);
            }
            inline bool pass () {
                return (t > 0);
            }
            inline void clearthis () {
                t = -1;
                xyz = euclid3vector < TR > (0, 0, 0);
            }
            inline bool operator == (lorentz4vector < TR > b) {
                lorentz4vector < TR > tmp = (*this) - b;
                TR diff = tmp.n2 ();

                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }
            inline double gamma () {
                return (double) t / m ();
            }
            inline TR gamma2 () {
                return t * t / m2 ();
            }
            inline double beta () {
                double g = (double) gamma2 ();

                g = 1.0 / g;
                g = 1.0 - g;
                return sqrt (g);
            }
            inline euclid3vector < TR > Velocity () {
                return xyz.dir () * beta ();
            }
            inline lorentz4vector < TR > LorentzBoost (euclid3vector < TR > booster) {
                euclid3vector < TR > parallel = booster * ((xyz * booster) / booster.p2 ());
                euclid3vector < TR > perpendicular = xyz - parallel;
                double gm = booster.p2 ();

                gm = 1.0 - gm;
                gm = (1.0 / gm);
                gm = sqrt (gm);
                lorentz4vector < TR > ret;
                ret.t = gm * (t - (parallel * booster));
                parallel = (parallel - (booster * t)) * (TR) gm;
                ret.xyz = parallel + perpendicular;
                return ret;
            }
            inline fastjet::PseudoJet getpseudojet () {
                return fastjet::PseudoJet (xyz[0], xyz[1], xyz[2], t);
            }
        };
        template < typename TRF, typename TRI > class ParticleNode {
        private:
            lorentz4vector <TRF> momentum;
            TRI d1, d2, pid;
        public:
            ~ParticleNode () {}
            ParticleNode () {
                d1 = -1;
                d2 = -1;
                pid = 0;
                momentum.clearthis ();
            }
            ParticleNode (TRF _x, TRF _y, TRF _z, TRF _t, TRF _d1, TRF _d2, TRF _pid):momentum (_x, _y, _z, _t) {
                d1 = _d1;
                d2 = _d2;
                pid = _pid;
            }
            ParticleNode (const Pythia8::Particle & part) {
                momentum = lorentz4vector <TRF> (part.px (), part.py (), part.pz (), part.e ());
                pid = part.id ();
                if (part.isFinal ()) {
                    d1 = -1;
                    d2 = -1;
                } else {
                    d1 = part.daughter1 ();
                    d2 = part.daughter2 ();
                }
            }
            inline int id () {
                return pid;
            }
            inline bool isFinal () {
                return (d1 < 1) && (d2 < 1);
            }
            inline int daughter1 () {
                return d1;
            }
            inline int daughter2 () {
                return d2;
            }
            inline float px () {
                return momentum[0];
            }
            inline float py () {
                return momentum[1];
            }
            inline float pz () {
                return momentum[2];
            }
            inline float e () {
                return momentum[3];
            }
            inline float pt () {
                return momentum.pt ();
            }
            inline float eta () {
                return momentum.eta ();
            }
            inline float modeta () {
                return CPPFileIO::mymod (eta());
            }
            inline bool isDetectable () {
                bool ret = (pt () > 0.5) && (modeta () < 5.0);
                ret = ret && detectable (pid);
                return ret;
            }
            inline bool IsGood () {
                return isFinal () && isDetectable ();
            }
            inline bool IsLepton () {
                return islepton (pid);
            }
            inline bool IsBLike () {
                return isblike(pid);
            }
            inline bool IsBMeson () {
                long tmppid = CPPFileIO::mymod(pid) ;
                return ((tmppid>100)&&isblike(tmppid));
            }
            inline bool IsBQuakr () {
                long tmppid = CPPFileIO::mymod(pid) ;
                return (tmppid==5);
            }
            inline TRF operator [] (size_t i) {
                return momentum[i] ;
            }
            inline lorentz4vector <TRF> & getvec () {
                return momentum;
            }
            inline double operator  () (ParticleNode b) {
                return momentum (b.momentum);
            }
            inline double pcone (ParticleNode b) {
                return momentum.pcone (b.momentum);
            }
            inline fastjet::PseudoJet getpseudojet () {
                return momentum.getpseudojet ();
            }
        };
    }
    typedef VECTORS::plane2vector < float >vector2;
    typedef VECTORS::euclid3vector < float >vector3;
    typedef VECTORS::lorentz4vector < float >vector4;
    typedef VECTORS::ParticleNode <float,int> pythia_node;
    typedef std::vector < vector2 > vector2s;
    typedef std::vector < vector3 > vector3s;
    typedef std::vector < vector4 > vector4s;
    typedef std::vector < pythia_node > pythia_nodes;
    typedef std::vector < fastjet::PseudoJet > pseudojets;
    void cleanup_vectors (vector4s & thevectors) {
        vector4s tmp_thevectors;
        for (size_t i = 0; i < thevectors.size (); i++)
            if (!thevectors[i].cleared ()) {
                tmp_thevectors.push_back (thevectors[i]);
            }
        if (tmp_thevectors.size () > 0) {
            thevectors.resize (tmp_thevectors.size ());
            size_t n = thevectors.size () * sizeof (vector4);

            memcpy ((void *) &(thevectors[0]), (const void *) &(tmp_thevectors[0]), (size_t) n);
        } else {
            thevectors.clear ();
        }
    }
    class sortorder {
      private:
      public:
        bool ret;
        sortorder (bool _ret = true) {ret = _ret;}
        ~sortorder () {}
        inline bool operator  () (vector4 a, vector4 b) {
            if (a > b) {
                return ret;
            } else {
                return !ret;
            }
        }
        inline bool operator  () (vector4 * a, vector4 * b) {
            if ((*a) > (*b)) {
                return ret;
            } else {
                return !ret;
            }
        }
    };
    namespace MainEventData {

        const size_t BTAG = 1;
        const size_t TAUTAG = 2;
        const size_t NOTHING = 0;

        class JetContainer:public vector4 {
        private:
            inline vector4 & CstGet (size_t i) { return vectors[0][constituents[i]]; }
            std::vector <int> constituents;
            vector4s *vectors;
        public:
            size_t TAG;
            fastjet::PseudoJet * injet;
            bool tau_tag () {
                if ((TAG & TAUTAG) == TAUTAG) {
                    return true;
                }
                return false;
            }
            bool bot_tag () {
                if ((TAG & BTAG) == BTAG) {
                    return true;
                }
                return false;
            }
            JetContainer (vector4s & _vectors, fastjet::PseudoJet & _injet) {
                TAG = NOTHING;
                /* Get some values: */  {
                    injet = &_injet;
                    vectors = &_vectors;
                    this[0][0] = _injet.px ();
                    this[0][1] = _injet.py ();
                    this[0][2] = _injet.pz ();
                    this[0][3] = _injet.e ();
                }
                /* Get the constituents: */  {
                    constituents.clear ();
                    std::vector < fastjet::PseudoJet > _constituents = _injet.constituents ();
                    for (size_t j = 0; j < _constituents.size (); j++) {
                        constituents.push_back (_constituents[j].user_index ());
                    }
                }
            }
            ~JetContainer () {
            }
            inline vector4 & operator  () (size_t i) {
                return CstGet (i);
            }
            inline size_t operator  () () {
                return constituents.size ();
            }
        };
        typedef std::vector < JetContainer > JetContainers;

        class EventData {
        private:
            pythia_nodes store ;
            float event_weight ;
            float event_sigma  ;
        public:
            ssize_t operator >> (CPPFileIO::FileFD & f) {
                ssize_t ret = 0;
                ret = ret + (f.multiwrite2file (event_sigma));
                ret = ret + (f.multiwrite2file (event_weight));
                ret = ret + (f << store);
                return ret;
            }
            ssize_t operator << (CPPFileIO::FileFD & f) {
                ssize_t ret = 0;
                ret = ret + (f.multiread2file (event_sigma));
                ret = ret + (f.multiread2file (event_weight));
                ret = ret + (f >> store);
                return ret;
            }

            vector4s     bquarks ;
            vector4s     bmesons ;
            vector4s     hadrons ;
            pseudojets   tojets  ;
            pseudojets   alljets ;
            pseudojets   bjets   ;
            pseudojets   njets   ;
            pythia_nodes leptons ;
            vector4s     gamma   ;

            inline pythia_node & operator () (size_t i) { return store[i] ; }
            inline size_t operator () () { return store.size() ; }
            inline void clear_derived () {
                leptons.clear();
                gamma.clear();
                bquarks.clear();
                bmesons.clear();
                hadrons.clear();
                tojets.clear();
            }
            inline void clear_all () {
                store.clear();
                clear_derived();
                event_weight=0;
                event_sigma=0;
            }
            inline double CalcIso (size_t idx, double cone_rad = 0.3) {
                double sum_pt = 0.0;
                if ((idx > 0) && (idx < store.size ())) {
                    for (size_t i = 0; i < store.size (); i++)
                        if (i != idx) {
                            if (store[i].IsGood ()) {
                                if (store[i] (store[idx]) < cone_rad) {
                                    sum_pt = sum_pt + store[i].pt ();
                                }
                                sum_pt = sum_pt / store[idx].pt ();
                            }
                        }
                }
                return sum_pt;
            }
            inline size_t recurse (size_t idx) {
                if (idx > 0) {
                    int PID = store[idx].id ();
                    size_t d1 = store[idx].daughter1 ();
                    size_t d2 = store[idx].daughter2 ();
                    if ((d1 > idx) && (store[d1].id () == PID)) {
                        return recurse (d1);
                    } else if ((d2 > idx) && (store[d2].id () == PID)) {
                        return recurse (d2);
                    }
                }
                return idx;
            }
            inline size_t findparticle (long PID, bool takemod=false) {
                size_t ret = 0;
                if(takemod) { PID = CPPFileIO::mymod(PID) ; }
                for (size_t i = 0; i < store.size (); i++) {
                    long ppid = store[i].id ();
                    if(takemod) { ppid = CPPFileIO::mymod(ppid) ; }
                    if (ppid == PID) { return i; }
                }
                return ret;
            }
            inline size_t getdaughter (size_t idx, long pid) {
                size_t ret = 0;
                if (idx > 0) {
                    idx = recurse (idx);
                    size_t d1 = store[idx].daughter1 ();
                    size_t d2 = store[idx].daughter2 ();
                    if ((d1 > 0) || (d2 > 0)) {
                        if (d2 > d1)
                            for (size_t i = d1; (i >= d1) && (i <= d2) && (i > 0); i++) {
                                long PPID = (int) store[i].id ();
                                long MPID = CPPFileIO::mymod (PPID);
                                if (PPID == pid) {
                                    ret = i;
                                    i = d2 + 1;
                                } else if (MPID == pid) {
                                    ret = i;
                                }
                            } else {
                                /* Check the first daughter: */  {
                                    size_t i = d1;
                                    long PPID = (int) store[i].id ();
                                    long MPID = CPPFileIO::mymod (PPID);
                                    if (PPID == pid) {
                                        ret = i;
                                    } else if (MPID == pid) {
                                        ret = i;
                                    }
                                }
                                /* Check the second daughter: */  {
                                    size_t i = d2;
                                    long PPID = (int) store[i].id ();
                                    long MPID = CPPFileIO::mymod (PPID);
                                    if (PPID == pid) {
                                        ret = i;
                                        i = d2 + 1;
                                    } else if (MPID == pid) {
                                        ret = i;
                                    }
                                }
                            }
                    }
                }
                return ret;
            }
            inline bool isisolatedlepton (size_t idx) {
                bool ret = ( (store[idx].IsGood()) && (store[idx].modeta()<2.5) && (store[idx].IsLepton()) ) ;
                if (ret) {
                    ret = (CalcIso(idx,0.3)<0.3) ;
                }
                return ret ;
            }
            inline bool isisolatedgamma (size_t idx) {
                bool ret = ( (store[idx].IsGood()) && (store[idx].modeta()<2.5) && (store[idx].id()==(int)PID::GAMMA) ) ;
                if (ret) {
                    ret = (CalcIso(idx,0.3)<0.3) ;
                }
                return ret ;
            }
            inline void prepare () {
                clear_derived();
                std::vector <size_t> tmp_bmeson ;
                std::vector <size_t> tmp_bquark ;
                for(size_t i=0;i<store.size();i++){
                    if(isisolatedlepton(i)) {leptons.push_back(store[i]);}
                    else if (isisolatedgamma(i)) {gamma.push_back(store[i].getvec());}
                    else if (store[i].IsGood()) {hadrons.push_back(store[i].getvec());}
                    else if (store[i].IsBMeson()) {tmp_bmeson.push_back(recurse(i));}
                    else if (CPPFileIO::mymod(store[i].id())==PID::BOTTOM) {tmp_bquark.push_back(recurse(i));}
                }
                for(size_t i=0;i<hadrons.size();i++){tojets.push_back(hadrons[i].getpseudojet());}
                CPPFileIO::deduplicate(tmp_bmeson);
                CPPFileIO::deduplicate(tmp_bquark);
                for (size_t i=0;i<tmp_bmeson.size();i++) {bmesons.push_back(store[tmp_bmeson[i]].getvec());}
                for (size_t i=0;i<tmp_bquark.size();i++) {bquarks.push_back(store[tmp_bquark[i]].getvec());}
            }

            inline void FromPythia (Pythia8::Event & event) {
                for (int i = 0; i < event.size (); i++) {
                    store.push_back (pythia_node (event[i]));
                }
                event_sigma = 0.0;
                event_weight = 1.0;
            }
            template < typename T > void ReadFromDelphes (T & delin) {
                clear_all ();
                event_weight = delin.Event_Weight[0];
                for (int i = 0; i < delin.Particle_; i++) {
                    float _x = delin.Particle_Px[i];
                    float _y = delin.Particle_Py[i];
                    float _z = delin.Particle_Pz[i];
                    float _t = delin.Particle_E[i];
                    int _d1 = delin.Particle_D1[i];
                    int _d2 = delin.Particle_D2[i];
                    int _pid = delin.Particle_PID[i];
                    pythia_node tmp (_x, _y, _z, _t, _d1, _d2, _pid);
                    store.push_back (tmp);
                }
            }
            EventData(){}
            EventData (Pythia8::Event & event) { FromPythia (event); }
            ~EventData(){}
        };

    }
    class processed_data {
      private:
        template < typename T > void sortthis (T & refs) {
            sortorder mysort;
            if (refs.size () > 1) {
                std::sort (refs.begin (), refs.end (), mysort);
            }
      } public:
          vector4s njets, bjets;
        vector4s eln, elp;
        vector4s mun, mup;
        vector4s gamma;
        vector4 met, remet;
        double ht, reht;
        double mht;
        vector4 VectorZ;
        vector4 VectorW;

        std::vector < vector4 * >lpn;
        std::vector < vector4 * >lpp;
        std::vector < vector4 * >lps;
        std::vector < vector4 * >ajets;
        void clearall () {
            njets.clear ();
            bjets.clear ();
            eln.clear ();
            elp.clear ();
            mun.clear ();
            mup.clear ();
            met.clearthis ();
            remet.clearthis ();
            lpn.clear ();
            lpp.clear ();
            lps.clear ();
            gamma.clear ();
            VectorZ.clearthis ();
            VectorW.clearthis ();
            ht = 0;
        }
        void FilterVectors () {
            cleanup_vectors (njets);
            cleanup_vectors (bjets);
            cleanup_vectors (eln);
            cleanup_vectors (elp);
            cleanup_vectors (mun);
            cleanup_vectors (mup);
            prepare ();
        }
        processed_data () {
            clearall ();
        }
        ~processed_data () {
        }
        bool check_clean_leptons (vector4 lept) {
            for (size_t i = 0; i < ajets.size (); i++)
                if ((*(ajets[i])) (lept) < 0.4) {
                    return false;
                }
            return true;
        }
        void cleanup_leptons (vector4s & lepts) {
            vector4s tmp;

            for (size_t i = 0; i < lepts.size (); i++)
                if (check_clean_leptons (lepts[i])) {
                    tmp.push_back (lepts[i]);
                }
            lepts.resize (tmp.size ());
            memcpy ((void *) &(lepts[0]), (const void *) &(tmp[0]), (size_t) tmp.size () * sizeof (vector4));
        }
        void clean_all_leptons () {
            cleanup_leptons (eln);
            cleanup_leptons (elp);
            cleanup_leptons (mun);
            cleanup_leptons (mup);
        }
        void JetPointer () {
            sortthis (njets);
            sortthis (bjets);
            ajets.clear ();
            for (size_t i = 0; i < njets.size (); i++) {
                ajets.push_back (&njets[i]);
            }
            for (size_t i = 0; i < bjets.size (); i++) {
                ajets.push_back (&bjets[i]);
            }
            sortthis (ajets);
        }
        void LeptonPointer () {
            clean_all_leptons ();
            sortthis (eln);
            sortthis (elp);
            sortthis (mun);
            sortthis (mup);
            lpn.clear ();
            lpp.clear ();
            lps.clear ();
            for (size_t i = 0; i < eln.size (); i++) {
                lpn.push_back (&eln[i]);
                lps.push_back (&eln[i]);
            }
            for (size_t i = 0; i < mun.size (); i++) {
                lpn.push_back (&mun[i]);
                lps.push_back (&mun[i]);
            }
            for (size_t i = 0; i < elp.size (); i++) {
                lpp.push_back (&elp[i]);
                lps.push_back (&elp[i]);
            }
            for (size_t i = 0; i < mup.size (); i++) {
                lpp.push_back (&mup[i]);
                lps.push_back (&mup[i]);
            }
            sortthis (lpn);
            sortthis (lpp);
            sortthis (lps);
        }
        void RePt () {
            remet = vector4 (0, 0, 0, 0);
            reht = 0;
            for (size_t i = 0; i < lps.size (); i++) {
                remet = remet + (*(lps[i]));
                reht = reht + lps[i]->pt ();
            }
            for (size_t i = 0; i < ajets.size (); i++) {
                remet = remet + (*(ajets[i]));
                reht = reht + ajets[i]->pt ();
            }
            for (size_t i = 0; i < gamma.size (); i++) {
                remet = remet + (gamma[i]);
                reht = reht + gamma[i].pt ();
            }
            mht = remet.pt () / reht;
        }
        void prepare () {
            JetPointer ();
            LeptonPointer ();
            RePt ();
        }
        template < typename T > bool ReadFromDelphes (T & delin) {
            bool ret = false;

            clearall ();
            for (int i = 0; i < delin.Electron_; i++) {
                TLorentzVector v4ltmp;

                v4ltmp.SetPtEtaPhiM (delin.Electron_PT[i], delin.Electron_Eta[i], delin.Electron_Phi[i], 0);
                vector4 tmp (v4ltmp.Px (), v4ltmp.Py (), v4ltmp.Pz (), v4ltmp.E ());

                if (delin.Electron_Charge[i] > 0.001) {
                    eln.push_back (tmp);
                } else if (delin.Electron_Charge[i] < 0.001) {
                    elp.push_back (tmp);
                }
            }
            for (int i = 0; i < delin.Muon_; i++) {
                TLorentzVector v4ltmp;

                v4ltmp.SetPtEtaPhiM (delin.Muon_PT[i], delin.Muon_Eta[i], delin.Muon_Phi[i], 0);
                vector4 tmp (v4ltmp.Px (), v4ltmp.Py (), v4ltmp.Pz (), v4ltmp.E ());

                if (delin.Muon_Charge[i] > 0.001) {
                    mun.push_back (tmp);
                } else if (delin.Muon_Charge[i] < 0.001) {
                    mup.push_back (tmp);
                }
            }
            for (int i = 0; i < delin.Jet_; i++) {
                TLorentzVector v4ltmp;

                v4ltmp.SetPtEtaPhiM (delin.Jet_PT[i], delin.Jet_Eta[i], delin.Jet_Phi[i], 0);
                vector4 tmp (v4ltmp.Px (), v4ltmp.Py (), v4ltmp.Pz (), v4ltmp.E ());

                if (delin.Jet_BTag[i] == 1) {
                    bjets.push_back (tmp);
                } else {
                    njets.push_back (tmp);
                }
            }
            for (int i = 0; i < delin.Photon_; i++) {
                TLorentzVector v4ltmp;

                v4ltmp.SetPtEtaPhiM (delin.Photon_PT[i], delin.Photon_Eta[i], delin.Photon_Phi[i], 0);
                vector4 tmp (v4ltmp.Px (), v4ltmp.Py (), v4ltmp.Pz (), v4ltmp.E ());

                gamma.push_back (tmp);
            }
/* Get the met: */  {
                TLorentzVector tmp;

                tmp.SetPtEtaPhiM (delin.MissingET_MET[0], delin.MissingET_Eta[0], delin.MissingET_Phi[0], 0);
                met = vector4 (tmp.Px (), tmp.Py (), tmp.Pz (), tmp.E ());
            }
            ht = delin.ScalarHT_;
            prepare ();
        }
        size_t ngoodleptons () {
            size_t t1 = mup.size () + mun.size ();
            size_t t2 = elp.size () + eln.size ();

            return CPPFileIO::mymax (t1, t2);
        }
        double error_bestzmass () {
            double ret = 1000;

            if (ngoodleptons () >= 2) {
                for (size_t i = 0; i < elp.size (); i++)
                    for (size_t j = 0; j < eln.size (); j++) {
                        double tmp_mass = (elp[i] + eln[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                        }
                    }
                for (size_t i = 0; i < mup.size (); i++)
                    for (size_t j = 0; j < mun.size (); j++) {
                        double tmp_mass = (mup[i] + mun[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                        }
                    }
            }
            return ret;
        }
        vector4 ReBuildW (double allower_error = 15) {
            if (VectorW.pass ()) {
                return VectorW;
            }
            double BestWMass = 1000;
            vector4 BestWVector;

            BestWVector.clearthis ();
            vector4 *c1, *c2;

            for (size_t i = 1; i < njets.size (); i++)
                for (size_t j = 0; j < i; j++) {
                    vector4 TmpWVector = njets[i] + njets[j];
                    double TmpWMass = TmpWVector.m ();

                    TmpWMass = CPPFileIO::mymod (TmpWMass - MASS::W);
                    if (TmpWMass < BestWMass) {
                        BestWMass = TmpWMass;
                        BestWVector = TmpWVector;
                        c1 = &(njets[i]);
                        c2 = &(njets[j]);
                    }
                }
            if (BestWMass < allower_error) {
                VectorW = BestWVector;
                if (BestWVector.m () > 135) {
                    printf ("SCREWEEEEEEEEEDDDDDDDDDDDDDDD");
                }
                c1->clearthis ();
                c2->clearthis ();
                cleanup_vectors (njets);
                JetPointer ();
            } else {
                BestWVector.clearthis ();
            }
            return VectorW;
        }
        vector4 ReBuildZ (double allower_error = 15) {
            if (VectorZ.pass ()) {
                return VectorZ;
            }
            double ret = 1000;
            vector4 *c1, *c2;
            vector4 Zvector;
            char type = 'N';

            Zvector.clearthis ();
            if (ngoodleptons () >= 2) {
                for (size_t i = 0; i < elp.size (); i++)
                    for (size_t j = 0; j < eln.size (); j++) {
                        double tmp_mass = (elp[i] + eln[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                            c1 = &(elp[i]);
                            c2 = &(eln[j]);
                            type = 'E';
                        }
                    }
                for (size_t i = 0; i < mup.size (); i++)
                    for (size_t j = 0; j < mun.size (); j++) {
                        double tmp_mass = (mup[i] + mun[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                            c1 = &(mup[i]);
                            c2 = &(mun[j]);
                            type = 'M';
                        }
                    }
                if (ret < allower_error) {
                    Zvector = (*c1) + (*c2);
                    c1->clearthis ();
                    c2->clearthis ();
                    if (type == 'E') {
                        cleanup_vectors (eln);
                        cleanup_vectors (elp);
                        LeptonPointer ();
                    } else if (type == 'M') {
                        cleanup_vectors (mun);
                        cleanup_vectors (mup);
                        LeptonPointer ();
                    }
                    VectorZ = Zvector;
                } else {
                    Zvector.clearthis ();
                }
            }
            return VectorZ;
        }
    };
    class eventstore {
      private:
        bool getfastjet () {
            tojets.clear ();
            for (size_t i = 0; i < hads.size (); i++) {
                fastjet::PseudoJet tmp (hads[i][0], hads[i][1], hads[i][2], hads[i][3]);
                tojets.push_back (tmp);
            } return true;
        }
        void clear_derived () {
/* Clearing jets: */  {
                tojets.clear ();
                njets.clear ();
                bjets.clear ();
            }
/* Clear the wrapper classes: */  {
                bots.clear ();
                hads.clear ();
            }
/* Clearing leptons: */  {
                elep.clear ();
                elen.clear ();
                munp.clear ();
                munn.clear ();
                leps.clear ();
                plep.clear ();
                nlep.clear ();
            }
            ht = 0;
        }
        void clear_all () {
/* Clearing the core data elements: */  {
                weight = 0;
                sigma = 0;
                store.clear ();
            }
            clear_derived ();
        }
        double CalcIso (size_t idx, double cone_rad = 0.3) {
            double sum_pt = 0.0;

            if ((idx > 0) && (idx < store.size ())) {
                for (size_t i = 0; i < store.size (); i++)
                    if (i != idx) {
                        if (store[i].IsGood ()) {
                            if (store[i] (store[idx]) < cone_rad) {
                                sum_pt = sum_pt + store[i].pt ();
                            }
                            sum_pt = sum_pt / store[idx].pt ();
                        }
                    }
            }
            return sum_pt;
        }
        bool IsIsolatedLepton (size_t idx) {
            bool ret = store[idx].IsLepton () && store[idx].IsGood ();

            if (ret) {
                double pt_ratio = CalcIso (idx, 0.3);

                ret = (pt_ratio < 0.3) && (store[idx].pt () > 10.0)
                    && (store[idx].modeta () < 2.5);
            }
            return ret;
        }
        bool IsIsolatedPhoton (size_t idx) {
            return false;
            bool ret = (store[idx].id () == PID::GAMMA)
                && (store[idx].IsGood ());

            if (ret) {
                double pt_ratio = CalcIso (idx, 0.5);

                ret = (pt_ratio < 0.12) && (store[idx].pt () > 10.0)
                    && (store[idx].modeta () < 2.5);
            }
            return ret;
        }
        void makenjets () {
            if (tojets.size () > 1) {
                double R = 0.5;

                fastjet::JetDefinition jet_def (fastjet::antikt_algorithm, R);
                fastjet::ClusterSequence clust_seq (tojets, jet_def);
                pseudojets jets = sorted_by_pt (clust_seq.inclusive_jets (20.0));

                for (size_t i = 0; i < jets.size (); i++) {
                    vector4 tmp (jets[i].px (), jets[i].py (), jets[i].pz (), jets[i].e ());

                    njets.push_back (tmp);
                }
            }
        }
        void makebjets () {
            vector4s NewBots;

            CPPFileIO::clone_vector (bots, NewBots);
            for (size_t i = 0; i < NewBots.size (); i++)
                if ((NewBots[i].pt () < 5) || (NewBots[i].meta () > 5)) {
                    NewBots[i].clearthis ();
                }
            cleanup_vectors (NewBots);
            size_t times = CPPFileIO::mymin (njets.size (), NewBots.size ());
            bool repeater = true;

            for (size_t k = 0; (k < times) && (repeater); k++) {
                double nearest = 300.0;
                size_t nearest_i = 0;
                size_t nearest_j = 0;

                for (size_t i = 0; i < njets.size (); i++)
                    for (size_t j = 0; j < NewBots.size (); j++)
                        if ((njets[i].pass ()) && (NewBots[j].pass ()))
                            if (njets[i].meta () < 2.5) {
                                double tmp_dist = njets[i].cone (NewBots[j]);

                                if (tmp_dist < nearest) {
                                    nearest = tmp_dist;
                                    nearest_i = i;
                                    nearest_j = j;
                                }
                            }
                if (nearest < 0.375) {
                    bjets.push_back (njets[nearest_i]);
                    njets[nearest_i].clearthis ();
                    NewBots[nearest_j].clearthis ();
                } else {
                    repeater = false;
                }
            }
            cleanup_vectors (njets);
            cleanup_vectors (NewBots);
        }
      public:
        double sigma, weight;
        pythia_nodes store;

        ssize_t operator >> (CPPFileIO::FileFD & f) {
            ssize_t ret = 0;

            ret = ret + (f.multiwrite2file (sigma));
            ret = ret + (f.multiwrite2file (weight));
            ret = ret + (f << store);
            return ret;
        }
        ssize_t operator << (CPPFileIO::FileFD & f) {
            ssize_t ret = 0;

            ret = ret + (f.multiread2file (sigma));
            ret = ret + (f.multiread2file (weight));
            ret = ret + (f >> store);
            return ret;
        }
        vector4s bots;
        vector4s hads;
        vector4 met;
        pseudojets tojets;
        vector4s njets;
        vector4s bjets;
        vector4s leps;
        vector4s plep;
        vector4s nlep;
        vector4s elep;
        vector4s elen;
        vector4s munp;
        vector4s munn;
        vector4s gamma;
        double ht;
        processed_data gendata;
        processed_data deldata;

        size_t findparticle (long PID) {
            size_t ret = 0;
            long MPID = CPPFileIO::mymod (PID);

            for (size_t i = 0; i < store.size (); i++) {
                long ppid = store[i].id ();
                long mpid = CPPFileIO::mymod (ppid);

                if (ppid == PID) {
                    return i;
                } else if (mpid == MPID) {
                    ret = i;
                }
            }
            return ret;
        }
        size_t recurse (size_t idx) {
            if (idx > 0) {
                int PID = store[idx].id ();
                size_t d1 = store[idx].daughter1 ();
                size_t d2 = store[idx].daughter2 ();

                if ((d1 > idx) && (store[d1].id () == PID)) {
                    return recurse (d1);
                } else if ((d2 > idx) && (store[d2].id () == PID)) {
                    return recurse (d2);
                }
            }
            return idx;
        }
        size_t getdaughter (size_t idx, long pid) {
            size_t ret = 0;

            if (idx > 0) {
                idx = recurse (idx);
                size_t d1 = store[idx].daughter1 ();
                size_t d2 = store[idx].daughter2 ();

                if ((d1 > 0) || (d2 > 0)) {
                    if (d2 > d1)
                        for (size_t i = d1; (i >= d1) && (i <= d2) && (i > 0); i++) {
                            long PPID = (int) store[i].id ();
                            long MPID = CPPFileIO::mymod (PPID);

                            if (PPID == pid) {
                                ret = i;
                                i = d2 + 1;
                            } else if (MPID == pid) {
                                ret = i;
                            }
                    } else {
/* Check the first daughter: */  {
                            size_t i = d1;
                            long PPID = (int) store[i].id ();
                            long MPID = CPPFileIO::mymod (PPID);

                            if (PPID == pid) {
                                ret = i;
                            } else if (MPID == pid) {
                                ret = i;
                            }
                        }
/* Check the second daughter: */  {
                            size_t i = d2;
                            long PPID = (int) store[i].id ();
                            long MPID = CPPFileIO::mymod (PPID);

                            if (PPID == pid) {
                                ret = i;
                                i = d2 + 1;
                            } else if (MPID == pid) {
                                ret = i;
                            }
                        }
                    }
                }
            }
            return ret;
        }
        size_t ngoodleptons () {
            size_t t1 = munp.size () + munn.size ();
            size_t t2 = elep.size () + elen.size ();

            return CPPFileIO::mymax (t1, t2);
        }
        double error_bestzmass () {
            double ret = 1000;

            if (ngoodleptons () >= 2) {
                for (size_t i = 0; i < elep.size (); i++)
                    for (size_t j = 0; j < elen.size (); j++) {
                        double tmp_mass = (elep[i] + elen[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                        }
                    }
                for (size_t i = 0; i < munp.size (); i++)
                    for (size_t j = 0; j < munn.size (); j++) {
                        double tmp_mass = (munp[i] + munn[j]).m ();

                        tmp_mass = CPPFileIO::mymod (tmp_mass - MASS::Z);
                        if (tmp_mass < ret) {
                            ret = tmp_mass;
                        }
                    }
            }
            return ret;
        }
        void prepare () {
            size_t EVT_SZ = store.size ();

/* Get the bottoms: */  {
                std::vector < size_t > i_bots;
                for (size_t i = 0; i < EVT_SZ; i++)
                    if (CPPFileIO::mymod (store[i].id ()) == PID::BOTTOM) {
                        size_t tmp_i = recurse (i);

                        i_bots.push_back (tmp_i);
                    }
                CPPFileIO::deduplicate (i_bots);
                size_t bots_SZ = i_bots.size ();

                for (size_t j = 0; j < bots_SZ; j++) {
                    size_t i = i_bots[j];

                    bots.push_back (vector4 (store[i].px (), store[i].py (), store[i].pz (), store[i].e ()));
                }
            }
            for (size_t i = 0; i < EVT_SZ; i++)
                if (store[i].IsGood ()) {
                    vector4 tmp = store[i].getvec ();

                    met = met + tmp;
                    ht = ht + tmp.pt ();
                    if (IsIsolatedLepton (i)) {
                        leps.push_back (tmp);
                        int tmp_pid = store[i].id ();

                        if (tmp_pid > 0) {
                            plep.push_back (tmp);
                        } else {
                            nlep.push_back (tmp);
                        }
                        if (tmp_pid == PID::ELECTRON) {
                            elep.push_back (tmp);
                            gendata.elp.push_back (tmp);
                        } else if (tmp_pid == -PID::ELECTRON) {
                            elen.push_back (tmp);
                            gendata.eln.push_back (tmp);
                        } else if (tmp_pid == PID::MUON) {
                            munp.push_back (tmp);
                            gendata.mup.push_back (tmp);
                        } else if (tmp_pid == -PID::MUON) {
                            munn.push_back (tmp);
                            gendata.mun.push_back (tmp);
                        }
                    } else if (IsIsolatedPhoton (i)) {
                        gamma.push_back (tmp);
                        gendata.gamma.push_back (tmp);
                    } else {
                        hads.push_back (tmp);
                    }
                }
            met = met.flip ().trans ();
            getfastjet ();
            makenjets ();
            makebjets ();
            if (leps.size () > 1) {
                std::sort (leps.begin (), leps.end ());
            }
            for (size_t i = 0; i < njets.size (); i++) {
                gendata.njets.push_back (njets[i]);
            }
            for (size_t i = 0; i < bjets.size (); i++) {
                gendata.bjets.push_back (bjets[i]);
            }
            gendata.met = met;
            gendata.ht = ht;
            gendata.prepare ();
        }
        inline pseudojets & getjetref () {
            return tojets;
        }
        void FromPythia (Pythia8::Event & event) {
            for (int i = 0; i < event.size (); i++) {
                store.push_back (pythia_node (event[i]));
            }
            sigma = 0.0;
            weight = 1.0;
        }
        template < typename T > bool ReadFromDelphes (T & delin) {
            bool ret = false;

            clear_all ();
            weight = delin.Event_Weight[0];
            for (int i = 0; i < delin.Particle_; i++) {
                float _x = delin.Particle_Px[i];
                float _y = delin.Particle_Py[i];
                float _z = delin.Particle_Pz[i];
                float _t = delin.Particle_E[i];
                int _d1 = delin.Particle_D1[i];
                int _d2 = delin.Particle_D2[i];
                int _pid = delin.Particle_PID[i];
                pythia_node tmp (_x, _y, _z, _t, _d1, _d2, _pid);

                store.push_back (tmp);
            }
            prepare ();
            deldata.ReadFromDelphes (delin);
        }
        eventstore () {
        }
        eventstore (Pythia8::Event & event) {
            FromPythia (event);
        }
        ~eventstore () {
        }
    };
    namespace DELPHES_DETDATA {
        const size_t BTAG = 1;
        const size_t TAUTAG = 2;
        const size_t NOTHING = 0;
        const char JunkAddress = 0;
        class DetVector:public vector4 {
          private:
          public:
            int charge;
            float H_fraction;
            float E_fraction;
            float M_fraction;
              DetVector () {
                charge = 0;
                H_fraction = 0;
                E_fraction = 0;
                M_fraction = 0;
            } ~DetVector () {
        }};
        typedef std::vector < DetVector > DetVectors;

        class JetContainer:public vector4 {
          private:
          public:
            double weight;
              std::vector < int >constituents;
            size_t TAG;
            DetVectors *vectors;
              fastjet::PseudoJet * injet;
            bool tau_tag () {
                if ((TAG & TAUTAG) == TAUTAG) {
                    return true;
                }
                return false;
            }
            bool bot_tag () {
                if ((TAG & BTAG) == BTAG) {
                    return true;
                }
                return false;
            }
            JetContainer (DetVectors & _vectors, fastjet::PseudoJet & _injet) {
                weight = 1.0;
                TAG = NOTHING;
/* Get some values: */  {
                    injet = &_injet;
                    vectors = &_vectors;
                    this[0][0] = _injet.px ();
                    this[0][1] = _injet.py ();
                    this[0][2] = _injet.pz ();
                    this[0][3] = _injet.e ();
                }
/* Get the constituents: */  {
                    constituents.clear ();
                    std::vector < fastjet::PseudoJet > _constituents = _injet.constituents ();
                    for (size_t j = 0; j < _constituents.size (); j++) {
                        constituents.push_back (_constituents[j].user_index ());
                    }
                }
            }
            ~JetContainer () {
            }
            inline DetVector & CstGet (size_t i) {
                return vectors[0][constituents[i]];
            }
            inline DetVector & operator  () (size_t i) {
                return CstGet (i);
            }
            inline size_t operator  () () {
                return constituents.size ();
            }
            int count_tracks () {
                int ret = 0;

                for (size_t i = 0; i < constituents.size (); i++) {
                    if (CstGet (i).charge != 0) {
                        ret++;
                    }
                }
                return ret;
            }
            int count_neutrals () {
                int ret = 0;

                for (size_t i = 0; i < constituents.size (); i++) {
                    if (CstGet (i).charge == 0) {
                        ret++;
                    }
                }
                return ret;
            }
            int count_hadrons () {
                int ret = 0;

                for (size_t i = 0; i < constituents.size (); i++) {
                    if ((CstGet (i).H_fraction > CstGet (i).M_fraction) && (CstGet (i).H_fraction > CstGet (i).E_fraction)
                        ) {
                        ret++;
                    }
                }
                return ret;
            }
            double Lambda () {
                std::vector < fastjet::PseudoJet > tmp_const = injet->constituents ();
                fastjet::JetDefinition jet_def (fastjet::antikt_algorithm, 2.0 * 2.0 / pt ());
                fastjet::ClusterSequence clust_seq (tmp_const, jet_def);
                std::vector < fastjet::PseudoJet > inclusive_jets = sorted_by_pt (clust_seq.inclusive_jets (10.0));
                double ret = 0;

                if (inclusive_jets.size () > 0) {
                    ret = log (1.0 - (inclusive_jets[0].pt () / injet->pt ()));
                }
                return ret;
            }
            double project () {
                NewHEPHeaders::vector3 tmp = xyz.dir ();
                double ret = 0;

                for (size_t i = 0; i < constituents.size (); i++) {
                    ret = ret + (CstGet (i).xyz.dir () * tmp);
                }
                ret = ret / constituents.size ();
                //ret = ret * 2.0 * 2.0 / pt ();
                return ret;
            }
        };
        typedef std::vector < JetContainer > JetContainers;

        class GenPartContainer {
          private:
          public:
            pythia_nodes store;
            vector4s taus;
            vector4s bots;
            float weight;
            void clear_all () {
                store.clear ();
                taus.clear ();
                bots.clear ();
                weight = 0;
            }
            size_t findparticle (long PID) {
                size_t ret = 0;
                long MPID = CPPFileIO::mymod (PID);

                for (size_t i = 0; i < store.size (); i++) {
                    long ppid = store[i].id ();
                    long mpid = CPPFileIO::mymod (ppid);

                    if (ppid == PID) {
                        return i;
                    } else if (mpid == MPID) {
                        ret = i;
                    }
                }
                return ret;
            }
            size_t recurse (size_t idx) {
                if (idx > 0) {
                    int PID = store[idx].id ();
                    size_t d1 = store[idx].daughter1 ();
                    size_t d2 = store[idx].daughter2 ();

                    if ((d1 > idx) && (store[d1].id () == PID)) {
                        return recurse (d1);
                    } else if ((d2 > idx) && (store[d2].id () == PID)) {
                        return recurse (d2);
                    }
                }
                return idx;
            }
            size_t getdaughter (size_t idx, long pid) {
                size_t ret = 0;

                if (idx > 0) {
                    idx = recurse (idx);
                    size_t d1 = store[idx].daughter1 ();
                    size_t d2 = store[idx].daughter2 ();

                    if ((d1 > 0) || (d2 > 0)) {
                        if (d2 > d1)
                            for (size_t i = d1; (i >= d1) && (i <= d2) && (i > 0); i++) {
                                long PPID = (int) store[i].id ();
                                long MPID = CPPFileIO::mymod (PPID);

                                if (PPID == pid) {
                                    ret = i;
                                    i = d2 + 1;
                                } else if (MPID == pid) {
                                    ret = i;
                                }
                        } else {
/* Check the first daughter: */  {
                                size_t i = d1;
                                long PPID = (int) store[i].id ();
                                long MPID = CPPFileIO::mymod (PPID);

                                if (PPID == pid) {
                                    ret = i;
                                } else if (MPID == pid) {
                                    ret = i;
                                }
                            }
/* Check the second daughter: */  {
                                size_t i = d2;
                                long PPID = (int) store[i].id ();
                                long MPID = CPPFileIO::mymod (PPID);

                                if (PPID == pid) {
                                    ret = i;
                                    i = d2 + 1;
                                } else if (MPID == pid) {
                                    ret = i;
                                }
                            }
                        }
                    }
                }
                return ret;
            }
            void GetTaus () {
                taus.clear ();
                std::vector < size_t > tau_indices;
/* Get genuine taus: */  {
                    size_t Higgs_Index = 0;

                    for (size_t i = 0; (i < store.size ()) && (Higgs_Index == 0); i++) {
                        long ppid = store[i].id ();

                        if ((ppid == NewHEPHeaders::PID::H0)||(ppid == NewHEPHeaders::PID::Z)) {
                            Higgs_Index = i;
                        }
                    }
                    if (Higgs_Index > 0) {
                        Higgs_Index = recurse (Higgs_Index);
                    } else {
                        printf ("SCREWEDDDDDDD!!!! higgs not found\n");
                    }
                    size_t d1 = store[Higgs_Index].daughter1 ();
                    size_t d2 = store[Higgs_Index].daughter2 ();

                    tau_indices.push_back (recurse (d1));
                    tau_indices.push_back (recurse (d2));
                }
                for (size_t i = 0; i < tau_indices.size (); i++) {
                    vector4 tmptauvect = store[tau_indices[i]].vec;

                    if (CPPFileIO::mymod (tmptauvect.eta ()) < 4.0) {
                        taus.push_back (tmptauvect);
                    }
                }
            }
            void GetBots () {
                bots.clear ();
                std::vector < size_t > tau_indices;
                for (size_t i = 0; i < store.size (); i++)
                    if (CPPFileIO::mymod (store[i].pid) == PID::BOTTOM) {
                        size_t tmpindex = recurse (i);

                        tau_indices.push_back (tmpindex);
                    }
                CPPFileIO::deduplicate (tau_indices);
                for (size_t i = 0; i < tau_indices.size (); i++) {
                    bots.push_back (store[tau_indices[i]].vec);
                }
            }
            template < typename T > void ReadFromDelphes (T & delin) {
                clear_all ();
                weight = delin.Event_Weight[0];
                for (int i = 0; i < delin.Particle_; i++) {
                    float _x = delin.Particle_Px[i];
                    float _y = delin.Particle_Py[i];
                    float _z = delin.Particle_Pz[i];
                    float _t = delin.Particle_E[i];
                    int _d1 = delin.Particle_D1[i];
                    int _d2 = delin.Particle_D2[i];
                    int _pid = delin.Particle_PID[i];
                    pythia_node tmp (_x, _y, _z, _t, _d1, _d2, _pid);

                    store.push_back (tmp);
                }
                GetTaus ();
                GetBots ();
            }
            GenPartContainer () {
            }
            ~GenPartContainer () {
            }
        };
        class DelphesContainer {
          private:
          public:
            DetVectors AllParticles;
            vector4s Muons, Electrons;
            JetContainers jets;
            DetVectors ToJets;
            float sigma, weight;
              fastjet::JetDefinition jet_def;
              fastjet::ClusterSequence * clust_seq;
              std::vector < fastjet::PseudoJet > jetvectors;
              std::vector < fastjet::PseudoJet > inclusive_jets;
            double calciso (size_t index, double DeltaR) {
                double DeltaR2 = DeltaR * DeltaR;
                double ret = 0;
                if (index < AllParticles.size ()) {
                    for (size_t i = 0; i < index; i++) {
                        if (AllParticles[index].pcone2 (AllParticles[i]) < DeltaR2) {
                            ret = ret + AllParticles[i].pt ();
                        }
                    }
                    for (size_t i = index + 1; i < AllParticles.size (); i++) {
                        if (AllParticles[index].pcone2 (AllParticles[i]) < DeltaR2) {
                            ret = ret + AllParticles[i].pt ();
                        }
                    }
                }
                return ret;
            }
            void DecideLeptJet () {
                for (size_t i = 0; i < AllParticles.size (); i++) {
                    if ((AllParticles[i].pt () > 15)
                        && (AllParticles[i].charge != 0)) {
                        if (AllParticles[i].M_fraction > 0.9) {
                            if (calciso (i, 0.3) < (0.3 * AllParticles[i].pt ())) {
                                Muons.push_back (NewHEPHeaders::vector4 (AllParticles[i][0], AllParticles[i][1], AllParticles[i][2], AllParticles[i][3]));
                            } else {
                                ToJets.push_back (AllParticles[i]);
                            }
                        } else if (AllParticles[i].E_fraction > 0.9) {
                            if (calciso (i, 0.3) < (0.3 * AllParticles[i].pt ())) {
                                Electrons.push_back (NewHEPHeaders::vector4 (AllParticles[i][0], AllParticles[i][1], AllParticles[i][2], AllParticles[i][3]));
                            } else {
                                ToJets.push_back (AllParticles[i]);
                            }
                        }
                    } else {
                        ToJets.push_back (AllParticles[i]);
                    }
                }
            }
            void ClusterJets () {
                jetvectors.clear (); /* Get the vectors: */  {
                    for (size_t i = 0; i < ToJets.size (); i++) {
                        fastjet::PseudoJet tmp (ToJets[i][0], ToJets[i][1], ToJets[i][2], ToJets[i][3]);
                        tmp.set_user_index (i);
                        jetvectors.push_back (tmp);
                    }
                }
                if (jetvectors.size () > 0) {
                    clust_seq = new fastjet::ClusterSequence (jetvectors, jet_def);
                    inclusive_jets = sorted_by_pt (clust_seq->inclusive_jets (20.0));
                    for (size_t i = 0; i < inclusive_jets.size (); i++) {
                        JetContainer tmp (ToJets, inclusive_jets[i]);

                        tmp.weight = weight;
                        jets.push_back (tmp);
                    }
                }
            }
          DelphesContainer ():jet_def (fastjet::antikt_algorithm, 0.4) {
                AllParticles.clear ();
                Electrons.clear ();
                Muons.clear ();
                inclusive_jets.clear ();
                clust_seq = (fastjet::ClusterSequence *) (&JunkAddress);
            }
            ~DelphesContainer () {
                if (clust_seq != ((fastjet::ClusterSequence *) (&JunkAddress))) {
                    delete clust_seq;
                }
            }
            template < typename T > void ReadFromDelphes (T & indata) {
                weight = indata.Event_Weight[0];
                for (size_t i = 0; i < indata.EFlowTrack_; i++) {
                    DetVector tmpvector; /* Assign the vectors: */  {
/* Setting momentum components: */  {
                            TLorentzVector tmp;

                            tmp.SetPtEtaPhiM (indata.EFlowTrack_PT[i], indata.EFlowTrack_Eta[i], indata.EFlowTrack_Phi[i], 0);
                            tmpvector[0] = tmp.Px ();
                            tmpvector[1] = tmp.Py ();
                            tmpvector[2] = tmp.Pz ();
                            tmpvector[3] = tmp.E ();
                        }
/* Set charge and type: */  {
                            tmpvector.charge = indata.EFlowTrack_Charge[i];
                            if (CPPFileIO::mymod (indata.EFlowTrack_PID[i]) == PID::MUON) {
                                tmpvector.M_fraction = 1.0;
                            } else if (CPPFileIO::mymod (indata.EFlowTrack_PID[i]) == PID::ELECTRON) {
                                tmpvector.E_fraction = 1.0;
                            } else {
                                tmpvector.H_fraction = 1.0;
                            }
                        }
                    }
                    AllParticles.push_back (tmpvector);
                }
                for (size_t i = 0; i < indata.EFlowPhoton_; i++) {
                    DetVector tmpvector; /* Assign the vectors: */  {
/* Setting momentum components: */  {
                            TLorentzVector tmp;

                            tmp.SetPtEtaPhiM (indata.EFlowPhoton_ET[i], indata.EFlowPhoton_Eta[i], indata.EFlowPhoton_Phi[i], 0);
                            tmpvector[0] = tmp.Px ();
                            tmpvector[1] = tmp.Py ();
                            tmpvector[2] = tmp.Pz ();
                            tmpvector[3] = tmp.E ();
                        }
/* Set charge and type: */  {
                            tmpvector.charge = 0;
                            tmpvector.E_fraction = indata.EFlowPhoton_Eem[i];
                            tmpvector.H_fraction = indata.EFlowPhoton_Ehad[i];
                        }
                    }
                    AllParticles.push_back (tmpvector);
                }
                for (size_t i = 0; i < indata.EFlowNeutralHadron_; i++) {
                    DetVector tmpvector; /* Assign the vectors: */  {
/* Setting momentum components: */  {
                            TLorentzVector tmp;

                            tmp.SetPtEtaPhiM (indata.EFlowNeutralHadron_ET[i], indata.EFlowNeutralHadron_Eta[i], indata.EFlowNeutralHadron_Phi[i], 0);
                            tmpvector[0] = tmp.Px ();
                            tmpvector[1] = tmp.Py ();
                            tmpvector[2] = tmp.Pz ();
                            tmpvector[3] = tmp.E ();
                        }
/* Set charge and type: */  {
                            tmpvector.charge = 0;
                            tmpvector.E_fraction = indata.EFlowNeutralHadron_Eem[i];
                            tmpvector.H_fraction = indata.EFlowNeutralHadron_Ehad[i];
                        }
                    }
                    AllParticles.push_back (tmpvector);
                }
                DecideLeptJet ();
                ClusterJets ();
            }
        };
        class FullDelphesContainer {
          private:
          public:
            DelphesContainer detinfo;
            GenPartContainer geninfo;
              std::vector < JetContainer * >taujets;
              std::vector < JetContainer * >botjets;
              FullDelphesContainer () {
            }
             ~FullDelphesContainer () {
            }
            template < typename T > inline void ReadFromDelphes (T & indata) {
                taujets.clear ();
                botjets.clear ();
                geninfo.ReadFromDelphes (indata);
                detinfo.ReadFromDelphes (indata);
                for (size_t i = 0; i < geninfo.taus.size (); i++) {
                    double smallest = 10000;
                    long good_index = -10000;

                    for (size_t j = 0; j < detinfo.jets.size (); j++) {
                        double tmpdist = detinfo.jets[j].cone (geninfo.taus[i]);

                        if (tmpdist < smallest) {
                            smallest = tmpdist;
                            good_index = j;
                        }
                    }
                    if (smallest < 0.3) {
                        taujets.push_back (&(detinfo.jets[good_index]));
                        detinfo.jets[good_index].TAG = (detinfo.jets[good_index].TAG | TAUTAG);
                    }
                }
                for (size_t i = 0; i < geninfo.bots.size (); i++) {
                    double smallest = 10000;
                    long good_index = -10000;

                    for (size_t j = 0; j < detinfo.jets.size (); j++) {
                        double tmpdist = detinfo.jets[j].cone (geninfo.bots[i]);

                        if (tmpdist < smallest) {
                            smallest = tmpdist;
                            good_index = j;
                        }
                    }
                    if (smallest < 0.3) {
                        botjets.push_back (&(detinfo.jets[good_index]));
                        detinfo.jets[good_index].TAG = (detinfo.jets[good_index].TAG | BTAG);
                    }
                }
            }
        };
    }
    class reco_top_had {
      private:
        vector4 * njet[2];
        vector4 *bjet;
      public:
          vector4 top, w;
        bool pass;
          reco_top_had () {
            clearthis ();
        } reco_top_had (vector4 & _njet0, vector4 & _njet1, vector4 & _bjet) {
            pass = false;
            top.clearthis ();
            w.clearthis ();
            njet[0] = &_njet0;
            njet[1] = &_njet1;
            bjet = &_bjet;
            if ((!njet[0]->cleared ()) && (!njet[1]->cleared ())
                && (!bjet->cleared ())) {
                w = (*njet[0]) + (*njet[1]);
                top = w + (*bjet);
                pass = true;
            }
        }
        inline double error_w () {
            if (cleared ()) {
                return 1000000000;
            } else {
                const double sigma_w = 10;
                double ret = (w.m () - MASS::W) / sigma_w;

                ret = ret * ret;
                return ret;
            }
        }
        inline double error_top () {
            if (cleared ()) {
                return 1000000000;
            } else {
                const double sigma_top = 25;
                double ret = (top.m () - MASS::TOP) / sigma_top;

                ret = ret * ret;
                return ret;
            }
        }
        inline double error () {
            if (cleared ()) {
                return 1000000000;
            } else {
                return sqrt (error_top () + error_w ());
            }
        }
        inline bool operator > (reco_top_had other) {
            return error () > other.error ();
        }
        inline bool operator < (reco_top_had other) {
            return error () < other.error ();
        }
        inline bool operator > (double other) {
            return error () > other;
        }
        inline bool operator < (double other) {
            return error () < other;
        }
        inline void clearthis () {
            top.clearthis ();
            w.clearthis ();
            pass = false;
        }
        inline bool cleared () {
            bool ret = top.cleared () || w.cleared () || (!pass);

            return ret;
        }
        inline void clear_parts () {
            njet[0]->clearthis ();
            njet[1]->clearthis ();
            bjet->clearthis ();
        }
    };
    class reco_top_lept {
      private:
        vector4 * bjet, *met, *lept;
      public:
        vector4 top, pnu[2], w;
        bool pass;
          reco_top_lept () {
            clearthis ();
        } reco_top_lept (vector4 & _lept, vector4 & _met, vector4 & _bjet) {
            lept = &_lept;
            met = &_met;
            bjet = &_bjet;
            const double WMASS2 = MASS::W * MASS::W;

            pnu[0] = *met;
            pnu[1] = *met;
            double k = (WMASS2 / 2.0) + ((*lept)[0] * (*met)[0]) + ((*lept)[1] * (*met)[1]);

            k = k / (*lept)[3];
            double a = (*lept)[2] / (*lept)[3];

            a = a * a;
            a = a - 1.0;
            double b = 2.0 * k * (*lept)[2] / (*lept)[3];
            double c = (k * k) - ((*met)[0] * (*met)[0]) - ((*met)[1] * (*met)[1]);
            double dis = (b * b) - (4.0 * a * c);

            if (dis > 0) {
                pnu[0][2] = (-b + sqrt (dis)) / (2.0 * a);
                pnu[0][3] = pnu[0].p ();
                pnu[1][2] = (-b - sqrt (dis)) / (2.0 * a);
                pnu[1][3] = pnu[1].p ();
                double diff0 = (((*bjet) + pnu[0] + (*lept)).m ()) - MASS::TOP;

                diff0 = CPPFileIO::mymod (diff0);
                double diff1 = (((*bjet) + pnu[1] + (*lept)).m ()) - MASS::TOP;

                diff1 = CPPFileIO::mymod (diff1);
                if (diff0 > diff1) {
                    CPPFileIO::myswap (pnu[0], pnu[1]);
                }
                w = (pnu[0]) + (*lept);
                top = w + (*bjet);
                pass = true;
                if (CPPFileIO::mymod (w.m () - MASS::W) > 1.0) {
                    printf ("ERROR: Something has gone blatently wrong... W Mass error \n");
                }
            } else {
                clearthis ();
            }
        }
        void clearthis () {
            top.clearthis ();
            pnu[0].clearthis ();
            pnu[1].clearthis ();
            w.clearthis ();
            pass = false;
        }
        bool cleared () {
            bool ret = top.cleared () || w.cleared () || pnu[0].cleared ()
                || (!pass);

            return ret;
        }
        inline double error () {
            if (cleared ()) {
                return 1000000000;
            } else {
                const double sigma_top = 15;
                double ret = (top.m () - MASS::TOP) / sigma_top;

                return CPPFileIO::mymod (ret);
            }
        }
        inline void clear_parts () {
            bjet->clearthis ();
            met->clearthis ();
            lept->clearthis ();
        }
        inline bool operator > (reco_top_had other) {
            return error () > other.error ();
        }
        inline bool operator < (reco_top_had other) {
            return error () < other.error ();
        }
        inline bool operator > (reco_top_lept other) {
            return error () > other.error ();
        }
        inline bool operator < (reco_top_lept other) {
            return error () < other.error ();
        }
        inline bool operator > (double other) {
            return error () > other;
        }
        inline bool operator < (double other) {
            return error () < other;
        }
    };
    class WriteHepmc2Fifo {
      private:
        std::string name;
        HepMC::Pythia8ToHepMC ToHepMC;
        HepMC::IO_GenEvent ascii_io;
      public:
        WriteHepmc2Fifo (std::string _name):ascii_io (&(_name[0]), std::ios::out) {
            name = _name;
        }
         ~WriteHepmc2Fifo () {
        }
        inline void operator  () (Pythia8::Pythia & pythia) {
            HepMC::GenEvent * hepmcevt = new HepMC::GenEvent ();
            ToHepMC.fill_next_event (pythia, hepmcevt);
            ascii_io << hepmcevt;
            delete hepmcevt;
        }
    };
    void LHA2HEPMC (std::string lhafile, std::string hepmcfile) {
        WriteHepmc2Fifo outfile (hepmcfile);

        Pythia8::Pythia pythia;
        pythia.readString ("Beams:frameType = 4");
        std::string tmp ("Beams:LHEF = ");
        tmp = tmp + lhafile;
        pythia.readString (&(tmp[0]));
        pythia.init ();
        for (size_t iEvent = 0; (!pythia.info.atEndOfFile ()); iEvent++)
            if (pythia.next ()) {
                outfile (pythia);
            }
        pythia.stat ();
    }
    class myhist {
      private:
        TH1F * thehist;
        std::vector < float >vals;
          std::vector < float >weights;
          std::string name;
        bool converted;
        bool convert_hist () {
            size_t nums = vals.size ();
            if ((nums > 1) && (!converted)) {
                std::sort (vals.begin (), vals.end ());
                float hist_begin = vals[0];
                float hist_end = vals[nums - 1];
                float diff = hist_end - hist_begin;
                  hist_begin = hist_begin - (diff / 10.0);
                  hist_end = hist_end + (diff / 10.0);
                size_t nbins = CPPFileIO::mymin ((size_t) nums / 4, N_Bins_Max);
                  nbins = CPPFileIO::mymax ((size_t) 8, nbins);
                  thehist = new TH1F (&(name[0]), &(name[0]), nbins, hist_begin, hist_end);
                for (size_t ii = 0; ii < nums; ii++) {
                    thehist->Fill (vals[ii], weights[ii]);
                } vals.clear ();

                weights.clear ();
                converted = true;
            }
            return converted;
        }
        void write_histogram () {
            if (!converted) {
                convert_hist ();
            }
            if (converted) {
                thehist->Write ();
                delete thehist;
            } else {
                printf ("ERROR: Something went horribly wrong... %s\n", &(name[0]));
                vals.clear ();
            }
        }
        void fill (float val, float weight = 1.0) {
            if (converted) {
                thehist->Fill (val, weight);
            } else {
                vals.push_back (val);
                weights.push_back (weight);
                if (vals.size () > 10000) {
                    convert_hist ();
                }
            }
        }
      public:
        size_t N_Bins_Max;
      myhist (std::string _name):name (_name), converted (false) {
            vals.clear ();
            N_Bins_Max = 100;
        }
        ~myhist () {
            write_histogram ();
        }
        inline void Fill (float val, float weight = 1.0) {
            fill (val, weight);
        }
        inline std::string get_name () {
            return name;
        }
        bool force_convert_hist (size_t nbins, float hist_begin, float hist_end) {
            if (!converted) {
                std::sort (vals.begin (), vals.end ());
                thehist = new TH1F (&(name[0]), &(name[0]), nbins, hist_begin, hist_end);
                for (size_t ii = 0; ii < vals.size (); ii++) {
                    thehist->Fill (vals[ii]);
                }
                vals.clear ();
                converted = true;
            }
            return converted;
        }
    };
    class VectorHist {
      private:
        std::string vectorname;
      public:
        myhist pt, eta, phi, m;
        VectorHist (std::string name):pt (name + std::string (":pt")), eta (name + std::string (":eta")), phi (name + std::string (":phi")), m (name + std::string (":mass")), vectorname (name) {
        } ~VectorHist () {
        }
        void Fill (NewHEPHeaders::vector4 invector) {
            if (invector.pass ()) {
                pt.Fill (invector.pt ());
                eta.Fill (invector.eta ());
                phi.Fill (invector.phi ());
                m.Fill (invector.m ());
            }
        }
        void force_convert (double In_pt = 1400, double In_eta = 6.0, double In_phi = 6.3, double In_m = 1200) {
            pt.force_convert_hist (150, -0.1, In_pt);
            eta.force_convert_hist (120, -In_eta, In_eta);
            phi.force_convert_hist (80, -0.1, In_phi);
            m.force_convert_hist (80, -0.1, In_m);
        }
    };
    typedef std::vector < VectorHist > VectorHists;
    class MultiPlot {
      private:
        TFile rootfile;
        std::vector < TH1F * >hists;
        double maxy;
          std::string title;
      public:
          bool normalized;
          MultiPlot (std::string rootfilename, std::string _title):rootfile (&(rootfilename[0])), title (_title) {
            normalized = false;
            maxy = 0;
        } ~MultiPlot () {
            for (size_t i = 0; i < hists.size (); i++) {
                delete hists[i];
            }
        }
        void operator  () (std::string histname) {
            TH1F *tmphist = (TH1F *) rootfile.Get (&(histname[0]));

            if (tmphist != NULL) {
                if (normalized) {
                    double factor = tmphist->Integral ();

                    tmphist->Scale (1.0 / factor);
                }
                int binmax = tmphist->GetMaximumBin ();
                double x = tmphist->GetBinContent (binmax);

                maxy = CPPFileIO::mymax (x, maxy);
                tmphist->SetTitle (&(title[0]));
                hists.push_back (tmphist);
            }
        }
        void Save (std::string dirname = "./") {
            std::vector < int >goodcolors; /* Decide on good colours: */  {
                goodcolors.push_back (kBlack);
                goodcolors.push_back (kRed);
                goodcolors.push_back (kBlue);
                goodcolors.push_back (kGreen + 3);
                goodcolors.push_back (kMagenta + 2);
            }
/* The plotting part: */  {
                TCanvas C;

                for (size_t i = 0; i < hists.size (); i++) {
                    TH1F *thehist = hists[i];

                    thehist->SetLineColor (goodcolors[i]);
                    thehist->SetMaximum (maxy);
                    thehist->Draw ("same");
                }
/* Save the canvas: */  {
                    mkdir (&(dirname[0]), 0755);
                    std::string outname = dirname + "/" + title + ".pdf";
                    C.SaveAs (&(outname[0]));
                }
            }
        }
    };
    class flowtable {
      private:
        size_t current;
        inline void copyfrom (const flowtable & other) {
            L.resize (other.L.size ());
            while (names.size () < L.size () + 1) {
                names.push_back (std::string ("nothing"));
            } memcpy ((void *) &(L[0]), (const void *) &(other.L[0]), (size_t) sizeof (size_t) * L.size ());

            current = other.current;
        }
      public:
        std::vector < size_t > L;
        std::vector < std::string > names;
        void startover () {
            current = 0;
        }
        void fill (bool val = true, std::string name = "noothing") {
            while (L.size () < current + 1) {
                L.push_back (0);
            }
            while (names.size () < current + 1) {
                names.push_back (name);
            }
            if (val) {
                L[current]++;
            }
            names[current] = name;
            current++;
        }
        flowtable () {
            L.clear ();
            current = 0;
        }
        flowtable (const flowtable & other) {
            copyfrom (other);
            current = 0;
        }
        ~flowtable () {
        }
        inline size_t operator >> (CPPFileIO::FileFD & outfile) {
            outfile << L;
        }
        inline size_t operator << (CPPFileIO::FileFD & outfile) {
            outfile >> L;
        }
        inline void operator = (const flowtable & other) {
            copyfrom (other);
        }
        void filewrite (std::string filename) {
            FILE *f = fopen (&(filename[0]), "w");
            size_t min_v = CPPFileIO::mymin (L.size (), names.size ());

            for (size_t i = 0; i < min_v; i++) {
                fprintf (f, " L [%ld] = %ld \t : \t %s \n", i, L[i], &(names[i][0]));
            }
            fclose (f);
        }
    };
    class flowtables {
      private:
        std::vector < flowtable > thetables;
        std::vector < std::string > names;
        std::vector < std::string > titles;
        std::vector < double >sigmas;
        void attachtable (const flowtable & other, double _sigma = 1.0) {
            thetables.push_back (flowtable (other));
            sigmas.push_back (_sigma);
        } void showtable (FILE * fout) {
            size_t max_v = 0;

            for (size_t i = 0; i < thetables.size (); i++) {
                max_v = CPPFileIO::mymax (max_v, thetables[i].L.size ());
            }
            for (size_t i = 0; i < thetables.size (); i++)
                while (thetables[i].L.size () < max_v) {
                    thetables[i].L.push_back (0);
                }
            size_t min_h = CPPFileIO::mymin (thetables.size (), titles.size ());

/* The first declaration line: */  {
                fprintf (fout, "\\begin{tabular}{");
                for (size_t i = 0; i < min_h + 1; i++) {
                    fprintf (fout, "|c");
                }
                fprintf (fout, "|}\n");
            }
/* The second titles line: */  {
                fprintf (fout, "\\hline Event type:");
                for (size_t i = 0; i < min_h; i++) {
                    fprintf (fout, " & %s ", &(titles[i][0]));
                }
                fprintf (fout, "\\\\\n");
            }
/* The third sigmas line: */  {
                fprintf (fout, "\\hline $\\sigma$ :");
                for (size_t i = 0; i < min_h; i++) {
                    fprintf (fout, " & %e ", sigmas[i]);
                }
                fprintf (fout, "\\\\\n");
            }
            for (size_t j = 0; j < max_v; j++) {
                fprintf (fout, "\\hline %s", &(names[j][0]));
                for (size_t i = 0; i < min_h; i++) {
                    fprintf (fout, " & %ld (%.3e) ", thetables[i].L[j], (double) ((double) thetables[i].L[j] * sigmas[i]) / thetables[i].L[0]
                        );
                }
                fprintf (fout, "\\\\\n");
            }
            fprintf (fout, "\\hline\n");
            fprintf (fout, "\\end{tabular}\n");
        }
      public:
        flowtables (std::string namefile, std::string titlefile) {
            thetables.clear ();
            CPPFileIO::getnames (names, namefile);
            CPPFileIO::getnames (titles, titlefile);
        }
        ~flowtables () {
        }
        inline void operator  () (std::string filename) {
            FILE *fout = fopen (&(filename[0]), "w");

            showtable (fout);
            fclose (fout);
        }
        inline void operator  () (const flowtable & other, double _sigma = 1.0) {
            attachtable (other, _sigma);
        }
    };
}
#endif
