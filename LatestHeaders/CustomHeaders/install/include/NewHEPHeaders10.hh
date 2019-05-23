#ifndef NewHEPHeaders2_HH
#define NewHEPHeaders2_HH

#ifdef USE_ALL
#define USE_PYTHIA
#define USE_FASTJET
#define USE_DELPHES
#define USE_ROOT
#endif

#include "CPPFileIO9.hh"

#ifdef USE_PYTHIA
#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC2.h>
#endif

#ifdef USE_FASTJET
#include <fastjet/Selector.hh>
#include <fastjet/tools/JHTopTagger.hh>
#endif

namespace NewHEPHeaders /* The preliminary standards: */ {
    constexpr double SMALL_EPSILON = 0.000001 ;
    constexpr bool DEBUG = false;
    inline size_t shifter  (size_t const in) {return (1<<in);}
    inline bool   checkbit (size_t const inbits, size_t const checkbits) {return ((inbits&checkbits)==checkbits);}
    namespace CONSTANTS   {
        constexpr double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
        constexpr double PI2 = PI * 2.0;
    }
    namespace PID         {
        constexpr long DOWN = 1;
        constexpr long UP = 2;
        constexpr long STRANGE = 3;
        constexpr long CHARM = 4;
        constexpr long BOTTOM = 5;
        constexpr long TOP = 6;
        constexpr long GAMMA = 22;
        constexpr long Z = 23;
        constexpr long W = 24;
        constexpr long H0 = 25;
        constexpr long H1 = 35;
        constexpr long A0 = 36;
        constexpr long CHARGED_HIGGS = 37;
        constexpr long GLUON = 21;
        constexpr long ELECTRON_NU = 12;
        constexpr long MUON_NU = 14;
        constexpr long TAU_NU = 16;
        constexpr long ELECTRON = 11;
        constexpr long MUON = 13;
        constexpr long TAU = 15;
        constexpr long CHI10 = 1000022;
        constexpr long CHI20 = 1000023;
    }
    namespace MASS        {
        constexpr double TOP = 173.340;
        constexpr double W = 80.385;
        constexpr double Z = 91.190;
        constexpr double H0 = 125.000;
    }
    namespace DECAY_WIDTH {
        constexpr double W = 2.085 ;
    }
    inline bool detectable (long pid) {
        pid = CPPFileIO::mymod (pid);
        if (pid == PID::ELECTRON_NU) {return false;}
        if (pid == PID::MUON_NU) {return false;}
        if (pid == PID::TAU_NU) {return false;}
        if (pid == PID::CHI10) {return false;}
        if (pid == PID::CHI20) {return false;}
        return true;
    }
    inline bool islepton (long pid) {
        pid = CPPFileIO::mymod (pid);
        if (pid == PID::ELECTRON) {return true;}
        if (pid == PID::MUON) {return true;}
        if (pid == PID::TAU) {return true;}
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
}

namespace NewHEPHeaders /* The main lorentz vectors part: */ {
    namespace VECTORS {
        const size_t NOTHING = 0;
        const size_t BTAG = shifter(0);
        const size_t TAUTAG = shifter(1);
        const size_t BMESONTAG = shifter(2);
        double VECTOR_EQUALITY_LIMIT = 50.0;
        template < typename TR=double > class plane2vector   {
        public:
            typedef TR TYPE_Data ;
            typedef plane2vector <TYPE_Data> TYPE_Self ;
        private:
            void construct (TR _x = 0, TR _y = 0) {x[0] = _x;x[1] = _y;}
        public:
            TYPE_Data x[2];
            inline void SetPtPhi (const TYPE_Data _pt=0,const TYPE_Data _phi=0) {x[0]=_pt*cos(_phi);x[1]=_pt*sin(_phi);}
            inline TYPE_Data pt2 () const { return (x[0] * x[0]) + (x[1] * x[1]); }
            inline TYPE_Data pt  () const { return (sqrt(pt2())) ; }
            inline TYPE_Data R   () const { return pt()          ; }
            inline TYPE_Data safenorm2 () const {
                TYPE_Data mag = pt2 ();
                if (CPPFileIO::mymod (mag) < 0.0000000001) {mag = CPPFileIO::mysign (mag) * 0.0000000001;}
                return mag;
            }
            inline TYPE_Data phi () const {
                TYPE_Data ret = acos (x[0] / sqrt (safenorm2 ()));
                if (x[1] < 0) {ret = CONSTANTS::PI2 - ret;}
                return ret;
            }
            inline TYPE_Data dphi (const TYPE_Self b) const {
                TYPE_Data ret = CPPFileIO::mymod (b.phi () - phi ());
                if (ret > CONSTANTS::PI) { ret = CONSTANTS::PI2 - ret; }
                return ret;
            }
            inline TYPE_Self operator + (const TYPE_Self b) const
            { return TYPE_Self (x[0] + b.x[0], x[1] + b.x[1]); }

            inline TYPE_Self operator - (const TYPE_Self b) const
            { return TYPE_Self (x[0] - b.x[0], x[1] - b.x[1]); }

            inline TYPE_Data operator * (const TYPE_Self b) const
            { return (x[0] * b.x[0]) + (x[1] * b.x[1]); }

            inline TYPE_Self operator + (const TYPE_Data b) const
            { return TYPE_Self (x[0] + b, x[1] + b); }

            inline TYPE_Self operator - (const TYPE_Data b) const
            { return TYPE_Self (x[0] - b, x[1] - b); }

            inline TYPE_Self operator * (const TYPE_Data b) const
            { return TYPE_Self (x[0] * b, x[1] * b); }

            inline TYPE_Self operator / (const TYPE_Data b) const
            { return TYPE_Self (x[0] / b, x[1] / b); }

            inline TYPE_Data operator  () (const TYPE_Self b) const { return dphi (b); }

            inline TYPE_Self flip () const
            { return TYPE_Self (-x[0], -x[1]); }

            inline TYPE_Self dir () const {
                TYPE_Self ret (x[0], x[1]);
                TYPE_Data mag = sqrt (ret.safenorm2 ());
                ret = ret / mag;
                return ret;
            }

            inline bool operator > (const plane2vector < TR > b) const { return pt2 () > b.pt2 (); }
            inline bool operator < (const plane2vector < TR > b) const { return pt2 () < b.pt2 (); }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) const { return f.multiwrite2file (*this); }
            inline ssize_t operator << (CPPFileIO::FileFD & f) const { return f.multiread2file (*this); }

            inline void operator += ( const plane2vector < TR > b ) { x[0] += b.x[0] ; x[1] += b.x[1] ; }
            inline void operator -= ( const plane2vector < TR > b ) { x[0] -= b.x[0] ; x[1] -= b.x[1] ; }
            inline void operator *= ( const plane2vector < TR > b ) { x[0] *= b.x[0] ; x[1] *= b.x[1] ; }
            inline void operator /= ( const plane2vector < TR > b ) { x[0] /= b.x[0] ; x[1] /= b.x[1] ; }

            inline void operator += ( TYPE_Data const b ) { x[0] += b ; x[1] += b ; }
            inline void operator -= ( TYPE_Data const b ) { x[0] -= b ; x[1] -= b ; }
            inline void operator *= ( TYPE_Data const b ) { x[0] *= b ; x[1] *= b ; }
            inline void operator /= ( TYPE_Data const b ) { x[0] /= b ; x[1] /= b ; }

            inline TR & operator [] (size_t i)       { return x[i]; }
            inline TR   operator [] (size_t i) const { return x[i]; }

            inline void clearthis () { x[0] = 0; x[1] = 0; }
            inline bool operator == (const plane2vector < TR > b) const {
                plane2vector < TR > tmp (x[0], x[1]);
                tmp = tmp - b;
                TR diff = tmp.pt2 ();
                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }

            plane2vector (const TR _x = 0, const TR _y = 0) {construct (_x, _y);}
            plane2vector (const plane2vector < TR > &c) {construct(c.x[0], c.x[1]);}
            ~plane2vector () {}
        };
        template < typename TR=double > class euclid3vector  {
        public:
            typedef TR TYPE_Data ;
            typedef euclid3vector <TYPE_Data> TYPE_Self ;
            typedef plane2vector <TYPE_Data> TYPE_Vector2 ;
        public:
            TYPE_Vector2 xy; TYPE_Data z;
            inline void SetPtEtaPhi(const TR _pt=0, const TR _eta=0, const TR _phi=0){
                xy.SetPtPhi (_pt,_phi) ;
                TR K = 2.0 * _eta ;
                K = exp (K) ;
                K = (K-1.0) / (K+1.0) ;
                K = K * K ;
                K = K / ( 1.0 - K ) ;
                z = sqrt(_pt*_pt*K) * CPPFileIO::mysign(_eta) ;
            }
            inline TR phi () const {return xy.phi ();}
            inline TR pt2 () const {return xy.pt2 ();}
            inline TR pt () const {return xy.pt ();}
            inline euclid3vector < TR > operator + (const euclid3vector < TR > a) const
            { return euclid3vector < TR > (xy + a.xy, z + a.z); }

            inline euclid3vector < TR > operator - (const euclid3vector < TR > a) const
            { return euclid3vector < TR > (xy - a.xy, z - a.z); }

            inline TR operator * (const euclid3vector <TR> a) const
            { return (xy * a.xy) + (z * a.z); }

            inline euclid3vector <TR> operator * (const TR a) const
            { return euclid3vector < TR > (xy * a, z * a); }

            inline euclid3vector <TR> operator / (const TR a) const
            { return euclid3vector < TR > (xy / a, z / a); }

            inline TR p2 () const { return xy.pt2 () + (z * z); }
            inline TR p () const { return sqrt (p2 ()); }

            inline TR & operator [] (size_t ret) {
                if (ret > 1) { return z; }
                else { return xy[ret]; }
            }
            inline TR operator [] (size_t ret) const {
                if (ret > 1) { return z; }
                else { return xy[ret]; }
            }

            inline TR eta () const
            { TR tmp_p = p (); return 0.5 * log ((tmp_p + z) / (tmp_p - z)); }

            inline TR meta () const { return CPPFileIO::mymod (eta ()); }

            inline TR cone2 (const euclid3vector < TR > b) const {
                TR tphi = xy.dphi (b.xy);
                tphi = tphi * tphi;
                TR teta = eta () - b.eta ();
                teta = teta * teta;
                TR ret = teta + tphi;
                return ret;
            }
            inline TR cone (const euclid3vector < TR > b) const
            { return sqrt (cone2 (b)); }

            inline TR dphi (const euclid3vector < TR > b) const
            { TR tphi = xy.dphi (b.xy); return tphi; }

            inline TR operator  () (const euclid3vector <TR> b) const { return cone (b); }
            inline TR safenorm2 () const {
                TR mag = xy.pt2 () + (z * z);
                if (CPPFileIO::mymod (mag) < 0.0000000001)
                { mag = CPPFileIO::mysign (mag) * 0.0000000001; }
                return mag;
            }
            inline euclid3vector < TR > flip () const { return euclid3vector < TR > (xy.flip (), -z); }
            inline euclid3vector < TR > trans () const { return euclid3vector < TR > (xy, 0); }
            inline euclid3vector < TR > dir () const {
                euclid3vector < TR > ret (*this);
                TR mag = sqrt (ret.safenorm2 ());
                ret = ret / mag;
                return ret;
            }

            inline void operator += (const euclid3vector<TR>b) {xy+=b.xy;z+=b.z;}
            inline void operator -= (const euclid3vector<TR>b) {xy-=b.xy;z-=b.z;}
            inline void operator *= (const euclid3vector<TR>b) {xy*=b.xy;z*=b.z;}
            inline void operator /= (const euclid3vector<TR>b) {xy/=b.xy;z/=b.z;}

            inline bool operator > (const euclid3vector < TR > b) const { return pt2 () > b.pt2 (); }
            inline bool operator < (const euclid3vector < TR > b) const { return pt2 () < b.pt2 (); }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) const { return f.multiwrite2file (*this); }
            inline ssize_t operator << (CPPFileIO::FileFD & f) const { return f.multiread2file (*this); }
            inline void clearthis () { xy = plane2vector < TR > (0, 0); z = 0; }
            inline bool operator == (const euclid3vector < TR > b) const {
                euclid3vector < TR > tmp = (*this) - b;
                TR diff = tmp.p2 ();
                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }
            euclid3vector (const TR _x = 0, const TR _y = 0, const TR _z = 0):xy (_x, _y) {z = _z;}
            euclid3vector (const plane2vector < TR > a, const TR _z = 0):xy (a) {z = _z;}
            euclid3vector (const euclid3vector < TR > &a):xy (a.xy) {z = a.z;}
        };
        template < typename TR=double > class lorentz4vector {
        public:
            typedef TR TYPE_Data ;
            typedef lorentz4vector <TYPE_Data> TYPE_Self    ;
            typedef euclid3vector  <TYPE_Data> TYPE_Vector3 ;
        public:
            TYPE_Vector3 xyz; TYPE_Data t;
            inline void SetPtEtaPhiM(const TR _pt, const TR _eta, const TR _phi, const TR _m){
                xyz.xy.SetPtPhi(_pt,_phi);
                TR K = exp(2.0*_eta) ;
                K=(K+1.0)/(K-1.0);
                TR _pt2 = _pt * _pt ;
                TR _m2 = _m*_m ;
                TR _z2 = (_pt2+_m2)/((K*K)-1.0) ;
                TR _E2 = _pt2 + _z2 + _m2 ;
                t = sqrt(_E2);
                xyz[2]=sqrt(_z2)*CPPFileIO::mysign(_eta);
            }

            inline TR & operator [] (const size_t ref)
            { if (ref > 2) { return t; } else { return xyz[ref]; } }

            inline TR operator [] (const size_t ref) const
            { if (ref > 2) { return t; } else { return xyz[ref]; } }

            inline TR pt2 () const { return xyz.pt2 (); }
            inline TR pt () const { return xyz.pt (); }
            inline TR p2 () const { return xyz.p2 (); }
            inline TR p () const { return xyz.p (); }
            inline TR phi () const { return xyz.phi (); }
            inline TR m2 () const { return CPPFileIO::mymod ((t * t) - p2 ()); }
            inline TR n2 () const { return CPPFileIO::mymod ((t * t) + p2 ()); }
            inline TR eta () const { return (0.5 * log ((t + xyz.z) / (t - xyz.z))); }
            inline TR meta () const { return CPPFileIO::mymod (eta ()); }
            inline TR peta () const { return xyz.eta (); }
            inline TR pmeta () const { return xyz.meta (); }
            inline TR m () const { return sqrt (m2 ()); }
            inline TR n () const { return sqrt (n2 ()); }
            inline TR dphi (const lorentz4vector < TR > b) const { return xyz.dphi (b.xyz); }
            inline lorentz4vector <TR> operator + (const lorentz4vector < TR > b) const
            { return lorentz4vector <TR> (xyz + b.xyz, t + b.t); }

            inline lorentz4vector <TR> operator - (const lorentz4vector < TR > b) const
            { return lorentz4vector <TR> (xyz - b.xyz, t - b.t); }

            inline TR operator * (const lorentz4vector <TR> b) const
            { return (t * b.t) - (xyz * b.xyz); }

            inline lorentz4vector < TR > operator * (const TR b) const
            { return lorentz4vector < TR > (xyz * b, t * b); }

            inline lorentz4vector < TR > operator / (const TR b) const
            { return lorentz4vector < TR > (xyz / b, t / b); }

            inline void operator = ( const euclid3vector < TR > other ) {
                xyz = other     ;
                t   = other.p() ;
            }
            inline void operator = ( const lorentz4vector < TR > other ) {
                xyz = other.xyz ;
                t   = other.t ;
            }
            inline TR pcone2 (const lorentz4vector < TR > b) const {
                TR tphi = xyz.dphi (b.xyz);
                tphi = tphi * tphi;
                TR teta = peta () - b.peta ();
                teta = teta * teta;
                TR ret = teta + tphi;
                return ret;
            }
            inline TR pcone (const lorentz4vector < TR > b) const { return sqrt (pcone2 (b)); }
            inline TR cone2 (const lorentz4vector < TR > b) const {
                TR tphi = xyz.dphi (b.xyz);
                tphi = tphi * tphi;
                TR teta = eta () - b.eta ();
                teta = teta * teta;
                TR ret = teta + tphi;
                return ret;
            }
            inline TR cone (const lorentz4vector < TR > b) const { return sqrt (cone2 (b)); }
            inline TR operator  () (const lorentz4vector < TR > b) const { return cone (b); }
            inline lorentz4vector < TR > flip () const
            { return lorentz4vector < TR > (xyz.flip (), t); }

            inline lorentz4vector < TR > trans () const
            { return lorentz4vector < TR > (xyz.trans (), t); }

            inline lorentz4vector < TR > dir () const
            { return lorentz4vector < TR > (xyz.dir (), t); }

            inline void operator += (const lorentz4vector<TR>b) {xyz+=b.xyz;t+=b.t;}
            inline void operator -= (const lorentz4vector<TR>b) {xyz-=b.xyz;t-=b.t;}
            inline void operator *= (const lorentz4vector<TR>b) {xyz*=b.xyz;t*=b.t;}
            inline void operator /= (const lorentz4vector<TR>b) {xyz/=b.xyz;t/=b.t;}
            inline bool operator > (const lorentz4vector < TR > b) const { return t < b.t ; }
            inline bool operator < (const lorentz4vector < TR > b) const { return t > b.t ; }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) const { return f.multiwrite2file (*this); }
            inline ssize_t operator << (CPPFileIO::FileFD & f) const { return f.multiread2file (*this); }
            inline bool cleared () const { return (t < 0); }
            inline bool pass () const { return (t > 0); }
            inline void clearthis () { t = -1; xyz = euclid3vector < TR > (0, 0, 0); }
            inline bool operator == (const lorentz4vector < TR > b) const {
                lorentz4vector < TR > tmp = (*this) - b;
                TR diff = tmp.n2 ();
                diff = CPPFileIO::mymod (diff);
                return diff < VECTOR_EQUALITY_LIMIT;
            }
            inline TR gamma () const { return (TR) t / m (); }
            inline TR gamma2 () const { return t * t / m2 (); }
            inline TR beta () const {
                TR g = (TR) gamma2 ();
                g = 1.0 / g;
                g = 1.0 - g;
                return sqrt (g);
            }
            inline euclid3vector <TR> Velocity () const { return xyz.dir () * beta (); }
            inline lorentz4vector <TR> LorentzBoost (const euclid3vector < TR > booster) const {
                euclid3vector < TR > parallel = booster * ((xyz * booster) / booster.p2 ());
                euclid3vector < TR > perpendicular = xyz - parallel;
                TR gm = booster.p2 ();
                gm = 1.0 - gm;
                gm = (1.0 / gm);
                gm = sqrt (gm);
                lorentz4vector < TR > ret;
                ret.t = gm * (t - (parallel * booster));
                parallel = (parallel - (booster * t)) * (TR) gm;
                ret.xyz = parallel + perpendicular;
                return ret;
            }
            inline lorentz4vector < TR > LorentzBoostGamma (const euclid3vector <TR> booster) const {
                TR gm2  = (TR) booster.p2 ()        ;
                if (gm2<1.0+SMALL_EPSILON) { return *this ; }
                else {
                    TR gm    = (TR) sqrt       (gm2)     ;
                    TR beta2 = (TR) 1.0    -   (1.0/gm2) ;
                    TR beta  = (TR) sqrt       (beta2)   ;
                    euclid3vector < TR > dir = booster / (TR)gm   ;
                    euclid3vector < TR > vel = dir     * (TR)beta ;
                    euclid3vector < TR > parallel      = dir * (dir*xyz) ;
                    euclid3vector < TR > perpendicular = xyz - parallel  ;
                    lorentz4vector < TR > ret ; /* Evaluate the vector: */ {
                        ret.t = ( t - (parallel*vel) ) * (TR)gm ;
                        parallel = ( parallel - (vel*t) ) * (TR)gm;
                        ret.xyz = parallel + perpendicular;
                    }
                    return ret;
                }
            }
            inline lorentz4vector < TR > BoostToRestFrame (const lorentz4vector < TR > booster) const
            { return LorentzBoostGamma(booster.xyz.dir()*booster[3]/booster.m()); }

            #ifdef __FASTJET_PSEUDOJET_HH__
            inline fastjet::PseudoJet getpseudojet () const { return fastjet::PseudoJet (xyz[0], xyz[1], xyz[2], t); }
            inline void operator = (const fastjet::PseudoJet & injet) {this[0]=lorentz4vector<TR>(injet);}
            lorentz4vector (const fastjet::PseudoJet & injet) {
                t      = injet.e  () ;
                xyz[2] = injet.pz () ;
                xyz[1] = injet.py () ;
                xyz[0] = injet.px () ;
            }
            #endif

            lorentz4vector (const TR _x = 0, const TR _y = 0, const TR _z = 0, const TR _t = 0):xyz (_x, _y, _z) { t = _t; }
            lorentz4vector (const euclid3vector < TR > a, const TR _t = -1) :xyz (a) { if(_t<0) {t=a.p();} else {t=_t;} }
            lorentz4vector (const plane2vector  < TR > a, const TR _z = 0 , const TR _t = -1) :xyz (a)
            { if (_t<0) {t=a.pt();} else {t=_t;} xyz.z = _z ; }
            lorentz4vector (const lorentz4vector < TR > &a):xyz (a.xyz) { t = a.t; }
            ~lorentz4vector(){}
        };
        template < typename TR=double, typename TI=int > class DelphesVectors {
        public:
            lorentz4vector <TR> momentum      ;
            TI                  Charge        ;
            TR                  Eem, Ehad, Emu;
            inline bool IsElectron () {
                bool ret = (CPPFileIO::mymod(Charge)==1) && (Eem/momentum[3]>0.9) ;
                return ret ;
            }
            inline bool IsMuon () {
                bool ret = (CPPFileIO::mymod(Charge)==1) && (Emu/momentum[3]>0.9) ;
                return ret ;
            }
            inline bool IsLepton () { return IsMuon() || IsElectron () ; }
            inline void Set_Ehad_Fraction (const TR InFrac) { Ehad = momentum[3] * InFrac ; }
            inline void Set_Eem_Fraction  (const TR InFrac) { Eem  = momentum[3] * InFrac ; }
            inline void Set_Emu_Fraction  (const TR InFrac) { Emu  = momentum[3] * InFrac ; }
            inline void SetPtEtaPhiM      (const TR _pt, const TR _eta, const TR _phi, const TR _m)
            { momentum.SetPtEtaPhiM (_pt,_eta,_phi,_m) ; }
            inline TR & operator [] (const size_t ref) {
                if      ( ref <= 3 ) { return momentum[ref] ; }
                else if ( ref == 4 ) { return Eem           ; }
                else if ( ref == 5 ) { return Ehad          ; }
                else if ( ref == 6 ) { return Emu           ; }
            }
            inline TR operator [] (const size_t ref) const {
                if      ( ref <= 3 ) { return momentum[ref] ; }
                else if ( ref == 4 ) { return Eem           ; }
                else if ( ref == 5 ) { return Ehad          ; }
                else if ( ref == 6 ) { return Emu           ; }
            }
            inline TR pt2   () const { return momentum.pt2  () ; }
            inline TR pt    () const { return momentum.pt   () ; }
            inline TR p2    () const { return momentum.p2   () ; }
            inline TR p     () const { return momentum.p    () ; }
            inline TR phi   () const { return momentum.phi  () ; }
            inline TR m2    () const { return momentum.m2   () ; }
            inline TR n2    () const { return momentum.n2   () ; }
            inline TR eta   () const { return momentum.eta  () ; }
            inline TR meta  () const { return momentum.meta () ; }
            inline TR peta  () const { return momentum.peta () ; }
            inline TR pmeta () const { return momentum.meta () ; }
            inline TR m     () const { return momentum.m    () ; }
            inline TR n     () const { return momentum.n    () ; }
            inline TR dphi       (const DelphesVectors <TR,TI> b) const { return momentum.dphi (b.momentum) ; }
            inline TR operator * (const DelphesVectors <TR,TI> b) const { return momentum * b.momentum      ; }
            inline DelphesVectors <TR,TI> operator * (const TR b) const
            { return DelphesVectors <TR,TI> (momentum*b,Eem*b,Ehad*b,Emu*b,Charge); }
            inline DelphesVectors < TR > operator / (const TR b) const
            { return DelphesVectors <TR,TI> (momentum/b,Eem/b,Ehad/b,Emu/b,Charge); }
            inline void operator = ( const DelphesVectors <TR,TI> other )
            { momentum=other.momentum; Eem=other.Eem; Ehad=other.Ehad; Emu=other.Emu; Charge=other.Charge;}
            inline DelphesVectors <TR,TI> operator + (const DelphesVectors <TR,TI> b) const {
                return DelphesVectors <TR,TI> (
                    momentum+b.momentum,
                    Eem+b.Eem,
                    Ehad+b.Ehad,
                    Emu+b.Emu,
                    Charge+b.Charge
                );
            }
            inline DelphesVectors <TR,TI> operator - (const DelphesVectors <TR,TI> b) const {
                return DelphesVectors <TR,TI> (
                    momentum-b.momentum,
                    Eem-b.Eem,
                    Ehad-b.Ehad,
                    Emu-b.Emu,
                    Charge-b.Charge
                );
            }
            inline TR cone (const DelphesVectors <TR,TI> b) const { return momentum.cone(b.momentum); }
            inline TR operator () (const DelphesVectors <TR,TI> b) const { return momentum(b.momentum); }
            inline bool operator > (const DelphesVectors <TR,TI> b) const { return momentum.pt2 () > b.momentum.pt2 (); }
            inline bool operator < (const DelphesVectors <TR,TI> b) const { return momentum.pt2 () < b.momentum.pt2 (); }
            inline ssize_t operator >> (CPPFileIO::FileFD & f) const { return f.multiwrite2file (*this); }
            inline ssize_t operator << (CPPFileIO::FileFD & f) const { return f.multiread2file (*this); }
            inline bool cleared () const { return (momentum[3] < 0); }
            inline bool pass () const { return (momentum[3] > 0); }
            inline void clearthis () { momentum.clearthis(); Eem=-10000; Ehad=-10000; Emu=-10000; Charge=0; }
            inline TR gamma () const { return (TR) momentum[3] / momentum.m(); }
            inline TR gamma2 () const { return (TR) momentum[3] * momentum[3] / momentum.m2(); }
            inline TR beta () const {return momentum.beta();}
            inline euclid3vector < TR > Velocity () const { return momentum.Velocity(); }
            inline DelphesVectors <TR,TI> LorentzBoost (const euclid3vector < TR > booster) const {
                lorentz4vector<TR>ret=momentum.LorentzBoost(booster);
                TR ratio = ret[3] / momentum[3] ;
                TR _Eem=Eem*ratio, _Ehad=Ehad*ratio, _Emu=Emu*ratio;
                return DelphesVectors <TR,TI> (ret,_Eem,_Ehad,_Emu,Charge) ;
            }
            inline DelphesVectors <TR,TI> LorentzBoostGamma (const euclid3vector < TR > booster) const {
                lorentz4vector<TR>ret=momentum.LorentzBoostGamma(booster);
                TR ratio=ret[3]/momentum[3];
                TR _Eem=Eem*ratio, _Ehad=Ehad*ratio, _Emu=Emu*ratio;
                return DelphesVectors <TR,TI> (ret,_Eem,_Ehad,_Emu,Charge) ;
            }
            #ifdef __FASTJET_PSEUDOJET_HH__
            inline fastjet::PseudoJet getpseudojet () const
            { return fastjet::PseudoJet (momentum[0],momentum[1],momentum[2],momentum[3]) ; }
            #endif
            DelphesVectors
            (const lorentz4vector<TR>_momentum, const TR _Eem=0, const TR _Ehad=0, const TR _Emu=0, const TI _Charge=0)
            { momentum=_momentum; Eem=_Eem; Ehad=_Ehad; Emu=_Emu; Charge=_Charge; }
            DelphesVectors
            (const TR _x=0, const TR _y=0, const TR _z=0, const TR _t=0, const TR _Eem=0, const TR _Ehad=0, const TR _Emu=0, const TI _Charge=0)
            { momentum=lorentz4vector<TR>(_x,_y,_z,_t); Eem=_Eem; Ehad=_Ehad; Emu=_Emu; Charge=_Charge; }
            ~DelphesVectors () {}
        } ;
        template < typename TRF=double, typename TRI=long > class ParticleNode   {
        private:
            lorentz4vector <TRF> momentum;
            TRI d1, d2, pid;
        public:
            inline TRI const id () const { return pid; }
            inline bool isFinal () const { return (d1 == 0) && (d2 == 0); }
            inline TRI daughter1 () const { return d1; }
            inline TRI daughter2 () const { return d2; }
            inline TRF px () const { return momentum[0]; }
            inline TRF py () const { return momentum[1]; }
            inline TRF pz () const { return momentum[2]; }
            inline TRF e () const { return momentum[3]; }
            inline TRF pt () const { return momentum.pt (); }
            inline TRF eta () const { return momentum.eta (); }
            inline TRF modeta () const { return CPPFileIO::mymod (eta()); }
            inline bool isDetectable () const {
                bool ret = (pt () > 0.5) && (modeta () < 6.0);
                ret = ret && detectable (pid);
                return ret;
            }
            inline bool IsGood () const { return isFinal () && isDetectable (); }
            inline bool IsLepton () const { return islepton (pid); }
            inline bool IsBLike () const { return isblike(pid); }
            inline bool IsBMeson () const {
                TRI tmppid = CPPFileIO::mymod(pid) ;
                return ((tmppid>100)&&isblike(tmppid));
            }
            inline bool IsBQuakr () const {
                TRI tmppid = CPPFileIO::mymod(pid) ;
                return (tmppid==PID::BOTTOM);
            }
            inline TRF operator [] (size_t i) { return momentum[i] ; }
            inline lorentz4vector <TRF> & getvec () { return momentum; }
            inline lorentz4vector <TRF> const getvec () const { return momentum; }
            inline lorentz4vector <TRF> & operator () () { return momentum; }
            inline lorentz4vector <TRF> const operator () () const { return momentum; }
            inline TRF operator  () (ParticleNode b) { return momentum (b.momentum); }
            inline TRF operator  () (lorentz4vector <TRF> b) { return momentum (b); }
            inline TRF pcone (ParticleNode b) { return momentum.pcone (b.momentum); }
            inline TRF pcone (lorentz4vector <TRF> b) { return momentum.pcone (b); }
            ParticleNode () {d1=-1;d2= -1;pid = 0;momentum.clearthis ();}
            ParticleNode (TRF _x, TRF _y, TRF _z, TRF _t, TRI _d1, TRI _d2, TRI _pid) :
            momentum (_x, _y, _z, _t) { d1 = _d1;d2 = _d2;pid = _pid; }
            #ifdef Pythia8_Pythia_H
            inline fastjet::PseudoJet getpseudojet () { return momentum.getpseudojet (); }
            ParticleNode (const Pythia8::Particle & part) {
                momentum = lorentz4vector <TRF> (part.px (), part.py (), part.pz (), part.e ());
                pid = part.id ();
                if (part.isFinal ()) { d1 = -1; d2 = -1; }
                else { d1 = part.daughter1 (); d2 = part.daughter2 (); }
            }
            #endif
            ~ParticleNode () {}
        };
        #ifdef __FASTJET_PSEUDOJET_HH__
        template < typename TR=double > class JetContainer : public lorentz4vector <TR> {
        private:
            inline lorentz4vector <TR> & CstGet (size_t i) {return vectors[0][constituents[i]];}
            std::vector <int> constituents;
            std::vector <lorentz4vector <TR>> *vectors;
        public:
            size_t TAG;
            fastjet::PseudoJet * injet;
            bool tau_tag () {
                if (checkbit(TAG,TAUTAG)) {return true;}
                else {return false;}
            }
            bool bot_tag () {
                if (checkbit(TAG,BTAG)) {return true;}
                else {return false;}
            }
            bool bms_tag () {
                if (checkbit(TAG,BMESONTAG)) {return true;}
                else {return false;}
            }
            JetContainer (std::vector <lorentz4vector<TR>> & _vectors, fastjet::PseudoJet & _injet) {
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
                    for (size_t j = 0; j < _constituents.size (); j++)
                    { constituents.push_back (_constituents[j].user_index ()); }
                }
            }
            ~JetContainer () {}
            inline lorentz4vector <TR> & operator  () (size_t i) {return CstGet (i);}
            inline size_t operator  () () {return constituents.size ();}
        };
        #endif
        class sortorder {
        public:
            bool ret;
            sortorder (bool _ret = true) {ret = _ret;}
            ~sortorder () {}
            template <typename T> inline bool operator  () (VECTORS::lorentz4vector <T> & a, VECTORS::lorentz4vector <T> & b) {
                if (a>b) {return ret;}
                else {return !ret;}
            }
            template <typename T> inline bool operator  () (VECTORS::lorentz4vector <T> * a, VECTORS::lorentz4vector <T> * b) {
                if ((*a)>(*b)) {return ret;}
                else {return !ret;}
            }
        };
        template <typename T> void cleanup_vectors ( std::vector <lorentz4vector <T>> & thevectors) {
            std::vector <VECTORS::lorentz4vector <T>> tmp_thevectors;
            for (size_t i = 0; i < thevectors.size (); i++)
                if (!thevectors[i].cleared ()) {
                    tmp_thevectors.push_back (thevectors[i]);
                }
                if (tmp_thevectors.size () > 0) {
                    thevectors.resize (tmp_thevectors.size ());
                    size_t n = thevectors.size () * sizeof (VECTORS::lorentz4vector <T>);
                    memcpy ((void *) &(thevectors[0]), (const void *) &(tmp_thevectors[0]), (size_t) n);
                } else { thevectors.clear (); }
        }
        template <typename TR> class RestTopDecays {
        private:
            TR          ST2 , CT2 , SP2 , CP2 ;
            TR          ST3 , CT3 , SP3 , CP3 ;
            euclid3vector  <TR> Velocity      ;
            lorentz4vector <TR> Daughters [3] ;
            lorentz4vector <TR> InVectors [3] ;
            inline bool ClearAll        () {
                Angle            = -10000.0 ;
                SpinAngle        = -10000.0 ;
                Matrix           =      0.0 ;
                MatrixSpin       =      0.0 ;
                Weight           =      0.0 ;
                WeightSpin       =      0.0 ;
                PhaseSpaceWeight =      0.0 ;
                passed           =    false ;
                return               passed ;
            }
            inline void SetPreliminary  () {
                MT  = MASS::TOP                      ;
                TOP = lorentz4vector <TR> (0,0,0,MT) ;
            }
            inline void SetDirections   () {
                ST2 = sin (T2) ; CT2 = cos (T2) ;
                ST3 = sin (T3) ; CT3 = cos (T3) ;
                SP2 = sin (P2) ; CP2 = cos (P2) ;
                SP3 = sin (P3) ; CP3 = cos (P3) ;
                dir[0] = euclid3vector <TR> (ST2*CP2,ST2*SP2,CT2) ;
                dir[1] = euclid3vector <TR> (ST3*CP3,ST3*SP3,CT3) ;
                Angle  = dir[0] * dir[1]  ;
            }
            inline bool EvalVectorSlave () {
                E3     = ((2.0*E2)-MT) / ( 2.0 * (((E2/MT)*(1.0-Angle))-1.0) ) ;
                passed = ( E3 >= 0 ) ;
                if (!passed) { return ClearAll () ; }
                Daughters [0] = dir[0] * E2                       ;
                Daughters [1] = dir[1] * E3                       ;
                Daughters [2] = TOP - Daughters[0] - Daughters[1] ;
                return passed ;
            }
            inline bool EvalVectors     () {
                SetPreliminary () ;
                passed = ( E2 <= (MT/2.0) ) && ( E2 >= 0 ) ;
                if (!passed) { return ClearAll () ; }
                SetDirections () ;
                return EvalVectorSlave () ;
            }
            inline bool PhaseSpace      () {
                if (!passed) { return ClearAll () ; }
                PhaseSpaceWeight = ( ((1.0-Angle)*E2) - MT ) ;
                PhaseSpaceWeight = 8 * PhaseSpaceWeight * PhaseSpaceWeight ;
                PhaseSpaceWeight = PhaseSpaceWeight / E2*MT*((2*E2)-MT)*ST2*ST3 ;
                return passed ;
            }
            inline bool EvalMatrix      () {
                if (!passed) { return ClearAll () ; }
                double tmp     = MASS::W              / MT ;
                double complex = DECAY_WIDTH::W * tmp / MT ;
                double real    = 1 - (tmp*tmp) - (2*E2/MT) ;
                Matrix         = (E3*MT*MT) * (MT-(2*E3)) / ( (real*real) + (complex*complex) ) ;
                return passed;
            }
            inline bool FinalWeight     () {
                if (!passed) { return ClearAll () ; }
                Weight = Matrix * PhaseSpaceWeight ;
                return passed ;
            }
            inline bool EvalChain       () {
                if (PhaseSpace()) if (EvalMatrix()) if (FinalWeight()) {return true;}
                return ClearAll () ;
            }
            inline bool EvalSpinMatrix ( euclid3vector  <TR> SpinDir ) {
                if (!passed) { return ClearAll () ; }
                MatrixSpin = ( (dir[1]*SpinDir) + 1 ) * Matrix ;
                WeightSpin = MatrixSpin * PhaseSpaceWeight ;
                return passed ;
            }
            inline bool permute        (size_t i, size_t j) {
                size_t k     = 3 - i - j    ;
                Daughters[0] = InVectors[i] ;
                Daughters[1] = InVectors[j] ;
                Daughters[2] = InVectors[k] ;
                dir[0] = Daughters[0].xyz.dir() ;
                dir[1] = Daughters[1].xyz.dir() ;
                Angle  = dir[0] * dir[1] ;
                ST2 = dir[0].pt() ;
                ST3 = dir[1].pt() ;
                E2  = Daughters[0][3] ;
                E3  = Daughters[1][3] ;
                passed = true  ;
                return EvalChain () ;
            }
            inline bool ChangeE2       (double _E2) {
                E2 = _E2 ;
                passed = ( E2 <= (MT/2.0) ) && ( E2 >= 0 ) ;
                if (!passed) { return ClearAll () ; }
                if (EvalVectorSlave()) {return EvalChain ();}
                return ClearAll () ;
            }

            inline bool Init (lorentz4vector <TR> a, lorentz4vector <TR> b, lorentz4vector <TR> c) {
                InVectors[0] = a     ;
                InVectors[1] = b     ;
                InVectors[2] = c     ;
                TOP          = InVectors[2] + InVectors[1] + InVectors[0] ;
                Velocity     = TOP.xyz.dir() ;
                Velocity     = Velocity*(TOP[3]/TOP.m()) ;
                TOP          = TOP.LorentzBoostGamma          (Velocity) ;
                InVectors[0] = InVectors[0].LorentzBoostGamma (Velocity) ;
                InVectors[1] = InVectors[1].LorentzBoostGamma (Velocity) ;
                InVectors[2] = InVectors[2].LorentzBoostGamma (Velocity) ;
                return         permute (0,1)                             ;
            }
            inline void Init (lorentz4vector <TR> *_Daughters) { Init (_Daughters[0],_Daughters[1],_Daughters[2]) ; }
            inline bool Init (TR _E2, TR _T2, TR _P2, TR _T3, TR _P3 ) {
                E2=_E2; T2=_T2; P2=_P2; T3=_T3; P3=_P3;
                if (EvalVectors()) if (PhaseSpace()) if (EvalMatrix()) if (FinalWeight()) {return true;}
                return false ;
            }
        public:

            euclid3vector  <TR> dir        [3]  ;
            euclid3vector  <TR> SpinVector      ;
            lorentz4vector <TR> TOP             ;

            TR E2 , T2 , P2 , T3 , P3 , E3 , MT ;
            TR Angle  , SpinAngle  ;
            TR Matrix , MatrixSpin ;
            TR Weight , WeightSpin ;
            TR PhaseSpaceWeight    ;
            bool passed            ;

            inline TR                  operator () ( TR _E2                                 ) {
                if (!passed) {return 0;}
                ChangeE2(_E2); return Weight;
            }
            inline TR                  operator () ( size_t i , size_t j                    ) {
                if (permute(i,j)) {return Weight ;}
                else {return 0;}
            }
            inline bool                operator () ( lorentz4vector <TR> *_Daughters        ) { return Init ( _Daughters          ) ; }
            inline bool                operator () ( TR _E2, TR _T2, TR _P2, TR _T3, TR _P3 ) { return Init ( _E2,_T2,_P2,_T3,_P3 ) ; }
            inline TR                  operator () ( euclid3vector  <TR> SpinDir            ) {
                if (!passed) { return 0 ; }
                return EvalSpinMatrix (SpinDir) ;
            }
            inline TR                  operator () ( TR TS , TR PS                          ) {
                if (!passed) { return 0 ; }
                TR STS = sin (TS) ; TR CTS = cos (TS) ;
                TR SPS = sin (PS) ; TR CPS = cos (PS) ;
                euclid3vector <TR>     SpinDir  (STS*CPS,STS*SPS,CTS) ;
                return EvalSpinMatrix (SpinDir)                       ;
            }
            inline lorentz4vector <TR> operator [] ( size_t i                               ) { return Daughters [i] ; }

            RestTopDecays(){}
            RestTopDecays ( lorentz4vector <TR> *_Daughters        ) { Init ( _Daughters          ) ; }
            RestTopDecays ( TR _E2, TR _T2, TR _P2, TR _T3, TR _P3 ) { Init ( _E2,_T2,_P2,_T3,_P3 ) ; }
            ~RestTopDecays(){}
        };
        template < typename TR=double, typename TI=long >
        class GenParticles : public std::vector <ParticleNode<TR,TI>> {
        public:
            typedef ParticleNode <TR,TI> TYPE_Element ;
            typedef GenParticles <TR,TI> TYPE_Self    ;
        public:
            inline TR CalcISO (size_t IDX, double DeltaR) {
                TR ret = 0 ;
                for (size_t i=0;i<this->size();i++)
                if (
                   (i!=IDX) &&
                   (this[0][i].IsGood()) &&
                   (this[0][i].getvec()(this[0][IDX].getvec())<DeltaR)
                ) { ret += this[0][i].pt() ; }
                return ret / this[0][IDX].pt() ;
            }
            inline size_t FindPID
            ( long   const PID) const {
                TYPE_Self const & SELF = this[0] ;
                size_t ret = 0 ;
                for(size_t i=0;(i<SELF.size())&&(ret==0);i++)if(SELF[i].id()==PID){ret=i;}
                return ret;
            }
            inline size_t FindPIDMod
            ( long const PID) const {
                TYPE_Self const & SELF = this[0] ;
                size_t ret = 0 ;
                for(size_t i=0;(i<SELF.size())&&(ret==0);i++){
                    if (
                        ( SELF[i].id() ==  PID ) ||
                        ( SELF[i].id() == -PID )
                    ) {ret=i;}
                }
                return ret;
            }
            inline size_t Recurse
            ( size_t const idx) const {
                if (idx > 0) {
                    TYPE_Self const & SELF = this[0] ;
                    int PID = SELF[idx].id ();
                    size_t d1 = SELF[idx].daughter1 ();
                    size_t d2 = SELF[idx].daughter2 ();
                    if ((d1 > idx) && (SELF[d1].id () == PID)) {
                        return Recurse(d1);
                    } else if ((d2 > idx) && (SELF[d2].id () == PID)) {
                        return Recurse(d2);
                    }
                }
                return idx;
            }
            inline TI FindDaughter (
                long ParentID,
                long DPID
            ) {
                ParentID =
                    Recurse (ParentID) ;
                //
                long dt [2] = {
                    this[0][ParentID]
                        .daughter1 () ,
                    this[0][ParentID]
                        .daughter2 ()
                } ;
                long dpid [2] = {
                    this[0][dt[0]].id () ,
                    this[0][dt[1]].id ()
                } ;
                //
                if (dpid[0]==DPID)
                    { return dt[0] ; }
                //
                else if (dpid[1]==DPID)
                    { return dt[1] ; }
                //
                else { return 0 ; }
                //
            }
            template <typename T1>
            inline void ReadFromDelphes
            (T1 & inref) {
                TYPE_Self & SELF = this[0] ;
                size_t limit = inref.Particle_ ;
                SELF.resize(limit);
                for(size_t i=0;i<limit;i++){
                    TR x   = inref.Particle_Px  [i] ;
                    TR y   = inref.Particle_Py  [i] ;
                    TR z   = inref.Particle_Pz  [i] ;
                    TR t   = inref.Particle_E   [i] ;
                    TI D1  = inref.Particle_D1  [i] ;
                    TI D2  = inref.Particle_D2  [i] ;
                    TI PID = inref.Particle_PID [i] ;
                    if(inref.Particle_Status[i]==1)
                        {D1=0;D2=0;}
                    //
                    SELF[i]=TYPE_Element(x,y,z,t,D1,D2,PID);
                }
            }
            #ifdef Pythia8_Pythia_H
            inline void ReadFromPythia
            (Pythia8::Pythia const & pythia) {
                for(size_t i=0;i<pythia.event.size();i++){
                    TYPE_Element tmp
                        (pythia.event[i]) ;
                    //
                    this->push_back(tmp);
                }
            }
            #endif
        };
    }
    typedef VECTORS::plane2vector   < float       > vector2         ;
    typedef VECTORS::euclid3vector  < float       > vector3         ;
    typedef VECTORS::lorentz4vector < float       > vector4         ;
    typedef VECTORS::RestTopDecays  < float       > topdecayelement ;
    typedef VECTORS::ParticleNode   < float , int > pythia_node     ;
    typedef std::vector < vector2 > vector2s ;
    typedef std::vector < vector3 > vector3s ;
    typedef std::vector < vector4 > vector4s ;
    typedef std::vector < pythia_node  > pythia_nodes  ;
    #ifdef __FASTJET_PSEUDOJET_HH__
    typedef VECTORS::JetContainer <              float > JetContainer  ;
    typedef std::vector           <       JetContainer > JetContainers ;
    typedef std::vector           < fastjet::PseudoJet > pseudojets    ;
    #endif
}

namespace NewHEPHeaders /* The top reconstruction parts: */ {
    namespace TopReconstruction {
        class reco_top_had  {
        private:
            vector4 *njet[2];
            vector4 *bjet;
        public:
            vector4 top, w;
            bool pass;
            inline void clearthis () {
                top.clearthis ();
                w.clearthis ();
                pass = false;
            }
            inline bool cleared () {
                bool ret = top.cleared () || w.cleared () || (!pass);
                return ret;
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
                if (cleared ()) { return 1000000000; }
                else { return sqrt (error_top () + error_w ()); }
            }
            inline bool operator > (reco_top_had other) { return error () > other.error () ; }
            inline bool operator < (reco_top_had other) { return error () < other.error () ; }
            inline bool operator > (double other)       { return error () > other          ; }
            inline bool operator < (double other)       { return error () < other          ; }
            inline void clear_parts () {
                njet[0]->clearthis () ;
                njet[1]->clearthis () ;
                bjet->clearthis    () ;
            }
            reco_top_had () {clearthis ();}
            reco_top_had (vector4 & _bjet, vector4 & _njet1, vector4 & _njet0) {
                pass = false;
                top.clearthis ();
                w.clearthis ();
                njet[0] = &_njet0;
                njet[1] = &_njet1;
                bjet = &_bjet;
                if (
                    (!njet[0]->cleared ()) &&
                    (!njet[1]->cleared ()) &&
                    (!bjet->cleared ())
                ) {
                    w = (*njet[0]) + (*njet[1]);
                    top = w + (*bjet);
                    pass = true;
                }
            }
        };
        class reco_top_lept {
        private:
            vector4 * bjet, *lept;
            vector2 * met ;
            void construct ( vector4 & _bjet , vector4 & _lept , vector2 & _met ) {
                clearthis ();
                lept = &_lept;
                met = & _met ;
                bjet = &_bjet;
                const double WMASS2 = MASS::W * MASS::W;
                pnu[0] = NewHEPHeaders::vector4(*met) ;
                pnu[1] = pnu[0];
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
                } else {clearthis ();}

            }
        public:
            vector4 top, pnu[2], w;
            bool pass;

            inline void clearthis () {
                top.clearthis ();
                pnu[0].clearthis ();
                pnu[1].clearthis ();
                w.clearthis ();
                pass = false;
            }
            inline bool cleared () {
                bool ret = top.cleared () || w.cleared () || pnu[0].cleared () || (!pass);
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
                met->clearthis  ();
                lept->clearthis ();
            }
            inline bool operator > (reco_top_had other) { return error () > other.error (); }
            inline bool operator < (reco_top_had other) { return error () < other.error (); }
            inline bool operator > (reco_top_lept other) { return error () > other.error (); }
            inline bool operator < (reco_top_lept other) { return error () < other.error (); }
            inline bool operator > (double other) { return error () > other; }
            inline bool operator < (double other) { return error () < other; }
            reco_top_lept () { clearthis (); }
            reco_top_lept ( vector4 & _bjet , vector4 & _lept , vector2 & _met )
            { construct (_bjet,_lept,_met) ; }

            reco_top_lept ( vector4 & _bjet , vector4 & _lept , vector4 & _met )
            { construct (_bjet,_lept,_met.xyz.xy) ; }
        };
    }
    typedef TopReconstruction::reco_top_had  reco_top_had  ;
    typedef TopReconstruction::reco_top_lept reco_top_lept ;
}

#ifdef Pythia8_Pythia_H
namespace NewHEPHeaders /* Pythia to HEPMc part: */ {
    class WriteHepmc2Fifo {
    private:
        std::string name;
        HepMC::Pythia8ToHepMC ToHepMC;
        HepMC::IO_GenEvent ascii_io;
    public:
        WriteHepmc2Fifo (std::string _name):ascii_io (&(_name[0]), std::ios::out) { name = _name; }
        ~WriteHepmc2Fifo () {}
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
        for (size_t iEvent = 0; (!pythia.info.atEndOfFile ()); iEvent++) if (pythia.next ())
        {outfile (pythia);}
        pythia.stat ();
    }
    namespace PythiaConfig {
        inline void PrepareDefault (Pythia8::Pythia & pythia) {
            pythia.readString ( "Beams:eCM = 13000"         ) ;
            pythia.readString ( "Random:setSeed = on"       ) ;
        }
    }
    template <typename T> inline void Generate (size_t const index, size_t const count) {
        CPPFileIO::SetCPUAffinity((int)index);
        char tmp[128] ; sprintf(tmp,"hepmc.%ld.fifo",index);
        mkfifo (tmp,0755) ; CPPFileIO::ForkMe forker ;
        if(forker.InKid()){
            std::vector <std::string> progs ;
            progs.push_back("./RunDelphes");
            progs.push_back(tmp);
            CPPFileIO::starter_self(progs);
        } else {
            NewHEPHeaders::WriteHepmc2Fifo outwriter(tmp);
            Pythia8::Pythia pythia ;
            {PythiaConfig::PrepareDefault(pythia);T tmpslave(pythia);}
            /* Initialize the random number generator: */ {
                sprintf ( tmp , "Random:seed = %ld" , index + 1 ) ;
                pythia.readString ( tmp ) ;
            }
            pythia.init();
            for (size_t i=0;i<count;i++) if (pythia.next()) {outwriter(pythia);}
        }
    }
}
#endif

#ifdef __FASTJET_JETDEFINITION_HH__
namespace NewHEPHeaders /* Fastjet easy jet cluster: */ {
    namespace JET_ALGO {
        long KT  =  1 ;
        long CA  =  0 ;
        long AKT = -1 ;
    }
    class JetClusterInclusive : public pseudojets {
    public:
        typedef JetClusterInclusive TYPE_Self   ;
        typedef pseudojets          TYPE_Parent ;
    private:
        fastjet::JetAlgorithm      algo      ;
        pseudojets               & invectors ;
        fastjet::JetDefinition   * jet_def   ;
        fastjet::ClusterSequence * clust_seq ;
    public:
        JetClusterInclusive
        (pseudojets & _invectors, long _algo=-1, double const _Pt=20, double const _R=0.4) :
        invectors(_invectors) {
            TYPE_Parent & SELF = this[0] ;
            if      ( _algo == -1 ) { algo = fastjet::antikt_algorithm    ; }
            else if ( _algo ==  0 ) { algo = fastjet::cambridge_algorithm ; }
            else if ( _algo ==  1 ) { algo = fastjet::kt_algorithm        ; }
            jet_def   = new fastjet::JetDefinition   (      algo ,         _R ) ;
            clust_seq = new fastjet::ClusterSequence ( invectors , jet_def[0] ) ;
            SELF      = sorted_by_pt ( clust_seq->inclusive_jets (_Pt) ) ;
        }
        ~JetClusterInclusive () { delete jet_def ; delete clust_seq ; }
    } ;
    class JetClusterExclusive : public pseudojets {
    public:
        typedef JetClusterInclusive TYPE_Self   ;
        typedef pseudojets          TYPE_Parent ;
    private:
        fastjet::JetAlgorithm      algo      ;
        pseudojets const         & invectors ;
        fastjet::JetDefinition   * jet_def   ;
        fastjet::ClusterSequence * clust_seq ;
    public:
        JetClusterExclusive
        (pseudojets const & _invectors, long _algo=-1, double _Pt=20, int const _N=1) :
        invectors(_invectors) {
            TYPE_Parent & SELF = this[0] ;
            if      ( _algo == -1 ) { algo = fastjet::antikt_algorithm    ; }
            else if ( _algo ==  0 ) { algo = fastjet::cambridge_algorithm ; }
            else if ( _algo ==  1 ) { algo = fastjet::kt_algorithm        ; }
            jet_def   = new fastjet::JetDefinition   (      algo ,      100.0 ) ;
            clust_seq = new fastjet::ClusterSequence ( invectors , jet_def[0] ) ;
            SELF      = fastjet::sorted_by_pt ( clust_seq->exclusive_jets (_N) ) ;
        }
        ~JetClusterExclusive () { delete jet_def ; delete clust_seq ; }
    } ;

    template <size_t N, typename T>
    class MyJetImageGen {
    public:

        using TYPE_DATA = T ;

        using vector4 =
            VECTORS::lorentz4vector
                <TYPE_DATA>
        ; //

        using vector3 =
            VECTORS::euclid3vector
                <TYPE_DATA>
        ; //

        using vector4s =
            std::vector
                <vector4>
        ; //

        using InputArrayType =
            Tensors::NN::N2D_ARRAY
                <N,N,TYPE_DATA>
        ; //

        using FlatArrayType =
            Tensors::NN::ND_ARRAY <
                InputArrayType::SIZE(),
                TYPE_DATA
            >
        ; //

        InputArrayType MainStore  ;
        vector4        FullJet    ;
        vector4        BoostedJet ;
        bool           eventpass  ;

    private:
        inline InputArrayType
        Process (
            fastjet::PseudoJet const &
                injet
        ) {

            MainStore = 0.0   ;
            FullJet   = injet ;
            eventpass = false ;

            TYPE_DATA constexpr E_0 = 1.0 ;
            TYPE_DATA constexpr M_0 = 0.5 ;

            TYPE_DATA constexpr P_0 =
                sqrt((E_0*E_0)-(M_0*M_0))
            ; //

            TYPE_DATA const
                E_J = injet.e()
            ; //
            TYPE_DATA const
                M_J = injet.m()
            ; //
            TYPE_DATA const
                P_J = FullJet.p ()
            ; //

            TYPE_DATA const
                GAMMA =
                    ((E_J*E_0)-(P_J*P_0)) /
                    (M_J*M_0)
            ; //

            vector3 dir =
                FullJet.xyz.dir() * GAMMA
            ; //
            if(E_J<E_0){
                dir = dir.flip () ;
            }

            pseudojets jetvectors =
                injet.constituents ()
            ; //

            vector4s JetVectors ;
            JetVectors.resize
                (jetvectors.size())
            ; //

            for(size_t i=0;i<jetvectors.size();i++){
                vector4 tmp =
                    jetvectors[i]
                ; //
                tmp = tmp * (M_0/M_J) ;
                JetVectors[i] =
                    tmp
                    .LorentzBoostGamma(dir)
                ; //
            }

            eventpass =
                (JetVectors.size()>2)
            ; //

            if(eventpass){
                JetClusterExclusive
                    jets (
                        jetvectors,
                        1,20,3
                    )
                ; //
                vector4 base [3] ;
                vector3 e    [3] ;
                base[0] = injet   ;
                base[1] = jets[0] ;
                base[2] = jets[1] ;
                for(size_t i=0;i<3;i++){
                    base[i] =
                        base[i] * (M_0/M_J)
                    ; //
                    base[i] =
                        base[i]
                        .LorentzBoostGamma(dir)
                    ; //
                    e[i] =
                        base[i].xyz.dir()
                    ; //
                }
                e[1] = ( e[1] - (e[0]*(e[1]*e[0])) ).dir() ;
                e[2] = ( e[2] - (e[0]*(e[2]*e[0])) ).dir() ;
                e[2] = ( e[2] - (e[1]*(e[2]*e[1])) ).dir() ;
                for(size_t i=0;i<JetVectors.size();i++){
                    TYPE_DATA X =
                        JetVectors[i].xyz * e[1] /
                        JetVectors[i][3]
                    ; //
                    if ( X < -1.0 ) { X = -1.0 ; }
                    if ( X >  1.0 ) { X =  1.0 ; }
                    long IX =
                        (long) N * ((X+1.0)/2.0)
                    ; //
                    if ( IX <     0 ) { IX =     0 ; }
                    if ( IX > N - 1 ) { IX = N - 1 ; }

                    TYPE_DATA Y =
                        JetVectors[i].xyz * e[2] /
                        JetVectors[i][3]
                    ; //
                    if ( Y < -1.0 ) { Y = -1.0 ; }
                    if ( Y >  1.0 ) { Y =  1.0 ; }
                    long IY =
                        (long) N * ((Y+1.0)/2.0)
                    ; //
                    if ( IY <     0 ) { IY =     0 ; }
                    if ( IY > N - 1 ) { IY = N - 1 ; }

                    MainStore[IY][IX] +=
                        JetVectors[i][3]
                    ; //
                }
            }
            return MainStore ;
        }
    public:

        inline InputArrayType operator ()
        (fastjet::PseudoJet const & injet) {
            return Process(injet);
        }

        MyJetImageGen(){}
        ~MyJetImageGen(){}

    } ;


}
#endif




#ifdef USE_ROOT
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
#endif

#ifdef USE_DELPHES
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
        typedef DelphesReader<T> TYPE_Self ;
        template <typename T2> inline void operator () (T2&analyzer) {
            if (this->fChain != 0) {
                long nentries = this->fChain->GetEntriesFast();
                long nbytes = 0, nb = 0;
                for (long jentry=0; jentry<nentries;jentry++) {
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
        template <typename T1> inline void ReadFromDelphes (T1 & inref) {
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
#endif
#endif
