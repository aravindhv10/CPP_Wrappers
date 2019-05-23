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
