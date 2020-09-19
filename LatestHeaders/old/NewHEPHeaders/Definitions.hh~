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
