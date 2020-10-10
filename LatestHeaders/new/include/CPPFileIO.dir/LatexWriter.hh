﻿#ifndef _HEADER_GUARD_CPPFileIO_LatexWriter_
#define _HEADER_GUARD_CPPFileIO_LatexWriter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileFD.hh"
#include "./FileLines.hh"
//////////////////
// Headers END. //
//////////////////

    template < typename T > class latexshow {
        ///////////////////////////////////////////////////////////////
    private:
        ///////////////////////////////////////////////////////////////
        long power; ///////////////////////////////////////////////////
        T num; ////////////////////////////////////////////////////////
        T input; //////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        inline void reduce () { ///////////////////////////////////////
            ///////////////////////////////////////////////////////////
            while ((-1<num) && (num<1) && (num!=0)) { /////////////////
                num = num * 10.0; /////////////////////////////////////
                power--; //////////////////////////////////////////////
            } /////////////////////////////////////////////////////////
            while ((num > 10) || (-10 > num)) { ///////////////////////
                num = num / 10.0; /////////////////////////////////////
                power++; //////////////////////////////////////////////
            } /////////////////////////////////////////////////////////
        } /////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        inline char * show (char * b) { ///////////////////////////////
            ///////////////////////////////////////////////////////////
            if ( mymod(power) > limit ) { /////////////////////////////
                sprintf (b, "{$%.3lf \\times 10^{%ld}$}", num, power);
            } else { //////////////////////////////////////////////////
                sprintf (b, "{$%.3lf$}", input); //////////////////////
            } /////////////////////////////////////////////////////////
            return b ; ////////////////////////////////////////////////
        } /////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
    public:
        ///////////////////////////////////////////////////////////////
        size_t limit; /////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        latexshow (T a) { /////////////////////////////////////////////
            ///////////////////////////////////////////////////////////
            input = a; ////////////////////////////////////////////////
            num = a; //////////////////////////////////////////////////
            power = 0; ////////////////////////////////////////////////
            limit = 2; ////////////////////////////////////////////////
            reduce (); ////////////////////////////////////////////////
        } /////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        ~latexshow () { ///////////////////////////////////////////////
        } /////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////
        inline char * operator  () (char * b) { ///////////////////////
            return show (b); //////////////////////////////////////////
        } /////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////

    }; ////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    template <typename T>
    std::string showlatex (
        T a
    ) {
        latexshow <T> tmp(a) ;
        char buf[128] ;
        tmp(buf);
        std::string ret(buf);
        return buf;
    }

    template <typename T> class flowtable {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        size_t current ; ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::vector <T>           L     ; //////////////////////////////////////////////////////////////////////////////////////////
        std::vector <std::string> names ; //////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void safefill ( size_t CLimit ) { //////////////////////////////////////////////////////////////////////////////////////////
            while ( L.size     () <= CLimit    ) { L.push_back     (0)                      ; } /////////////////////////////////////
            while ( names.size () <  L.size () ) { names.push_back (std::string("nothing")) ; } /////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void safefill ()    { safefill (current) ; } ////////////////////////////////////////////////////////////////////////
        inline void namefill ()    { if (L.size()>0) { safefill (L.size()-1) ; } } /////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void copyfrom ( const flowtable <T> & other ) { /////////////////////////////////////////////////////////////////////
            safefill (other.L.size()) ; ////////////////////////////////////////////////////////////////////////////////////////////
            memcpy ( (void*) &(L[0]) , (const void*) &(other.L[0]) , (size_t) sizeof(T) * L.size() ) ; /////////////////////////////
            current = other.current ; //////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void startover () { current = 0 ; } /////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void fill ( T val = 1 , std::string name = "noothing" ) { ///////////////////////////////////////////////////////////
            safefill () ; //////////////////////////////////////////////////////////////////////////////////////////////////////////
            L[current]     = L[current] + val ; ////////////////////////////////////////////////////////////////////////////////////
            names[current] = name             ; ////////////////////////////////////////////////////////////////////////////////////
            current++                         ; ////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void fill ( bool val , std::string name = "nothing" ) { /////////////////////////////////////////////////////////////
            if (val) {fill(1,name);} ///////////////////////////////////////////////////////////////////////////////////////////////
            else     {fill(0,name);} ///////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char * showhorizontal (std::string & linetitle) { //////////////////////////////////////////////////////////////////////////
            std::vector <std::string> parts ; /**/ { ///////////////////////////////////////////////////////////////////////////////
                std::string tmp = "\\hline " + linetitle + " &" ; //////////////////////////////////////////////////////////////////
                parts.push_back (tmp) ; ////////////////////////////////////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            for(size_t i=0;i<L.size();i++){ ////////////////////////////////////////////////////////////////////////////////////////
                latexshow <T> tmplatex (L[i]) ; ////////////////////////////////////////////////////////////////////////////////////
                char tmpbuf [512] ; ////////////////////////////////////////////////////////////////////////////////////////////////
                tmplatex (tmpbuf) ; ////////////////////////////////////////////////////////////////////////////////////////////////
                if (i<L.size()-1) {parts.push_back(std::string(tmpbuf)+" &");} /////////////////////////////////////////////////////
                else {parts.push_back(std::string(tmpbuf)+" \\\\");} ///////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            linetitle = parts[0] ; /////////////////////////////////////////////////////////////////////////////////////////////////
            for(size_t i=1;i<parts.size();i++){ ////////////////////////////////////////////////////////////////////////////////////
                linetitle = linetitle + parts[i] ; /////////////////////////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            return &(linetitle[0]) ; ///////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void showvals (std::vector <std::string> & outvals) { //////////////////////////////////////////////////////////////////////
            for(size_t i=0;i<L.size();i++) { ///////////////////////////////////////////////////////////////////////////////////////
                latexshow <double> tmpshow (L[i]) ; ////////////////////////////////////////////////////////////////////////////////
                char tmp[128] ; tmpshow(tmp) ; /////////////////////////////////////////////////////////////////////////////////////
                outvals.push_back(std::string(tmp)); ///////////////////////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void operator () () {startover();} //////////////////////////////////////////////////////////////////////////////////
        inline void operator () ( T    val , std::string name = "nothing" , bool inflag = true ) { /////////////////////////////////
            if   (inflag) { fill ( val  , name ) ; } ///////////////////////////////////////////////////////////////////////////////
            else          { fill ( (T)0 , name ) ; } ///////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void operator () ( bool val , std::string name = "nothing" ) {fill(val,name);} //////////////////////////////////////
        inline char * operator () (std::string & linetitle) {return showhorizontal(linetitle);} ////////////////////////////////////
        inline size_t size() {return L.size();} ////////////////////////////////////////////////////////////////////////////////////
        inline T & operator [] (size_t i) {return L[i];} ///////////////////////////////////////////////////////////////////////////
        inline std::vector <std::string> & operator () (std::vector <std::string> & outvals) { /////////////////////////////////////
            showvals(outvals); /////////////////////////////////////////////////////////////////////////////////////////////////////
            return outvals; ////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline size_t operator >> (FileFD & outfile) {return (outfile<<L);} /////////////////////////////////////////////
        inline size_t operator << (FileFD & outfile) {return (outfile>>L);} /////////////////////////////////////////////
        inline void operator = (const flowtable <T> & other) {copyfrom(other);} ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void filewrite (std::string filename) { ////////////////////////////////////////////////////////////////////////////////////
            FILE *f = fopen (&(filename[0]),"w") ; /////////////////////////////////////////////////////////////////////////////////
            namefill(); ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            size_t min_v = L.size(); ///////////////////////////////////////////////////////////////////////////////////////////////
            for (size_t i=0;i<min_v;i++) {fprintf(f," L [%ld] = %e \t : \t %s \n",i,(double)L[i],&(names[i][0]));} /////////////////
            fclose(f); /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        flowtable() { //////////////////////////////////////////////////////////////////////////////////////////////////////////////
            L.clear(); /////////////////////////////////////////////////////////////////////////////////////////////////////////////
            current=0; /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        flowtable (const flowtable & other) { //////////////////////////////////////////////////////////////////////////////////////
            copyfrom(other); ///////////////////////////////////////////////////////////////////////////////////////////////////////
            current=0; /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~flowtable(){} /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }; /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class flowtables {
    private:
        FILE *f ;
    public:
        void operator () (std::string infile) {
            FileLines reader(infile);
            fprintf(f,"{|");
            for (size_t i=0;i<reader.size();i++) {fprintf(f,"c|");}
            fprintf(f,"}\n");
            for (size_t i=0;i<reader();i++) {
                if(i==0) {fprintf(f,"\\hline ");}
                else {fprintf(f,"& ");}
                fprintf(f,"%s",&(reader(i)[0]));
            }
            fprintf(f," \\\\ \n");
        }
        template <typename T> void operator () (flowtable <T> & intable, std::string eventtype) {
            fprintf(f,"%s\n",intable(eventtype));
        }
        flowtables  (FILE * _f) {f=_f;fprintf(f,"\\begin{tabular}");}
        ~flowtables ()          {fprintf(f,"\\hline \\end{tabular}\n");}
    };

    class LatexPrintMatrix {
    private:
        std::vector <std::string> elements ;
        size_t X, Y ;
        inline std::string & GetElement ( size_t x , size_t y ) {
            if(x>=X) {printf("BUG!!! out of bounds in x !!!\n");}
            if(y>=Y) {printf("BUG!!! out of bounds in y !!!\n");}
            return elements [x+(X*y)] ;
        }
        void Write2FILE ( FILE * f ) {
            fprintf(f,"\\begin{tabular}");
            for(size_t x=0;x<X;x++) {
                if(x==0){fprintf(f,"{|");}
                fprintf(f,"c|");
                if(x==(X-1)){fprintf(f,"}\n");}
            }
            for (size_t y=0;y<Y;y++) {
                for (size_t x=0;x<X;x++) {
                    if(x==0) {fprintf(f," \\hline %s ",&(GetElement(x,y)[0]));}
                    else     {fprintf(f," & %s ",&(GetElement(x,y)[0]));}
                }
                fprintf(f," \\\\ \n");
            }
            fprintf(f,"\\hline \\end{tabular}\n");
        }
        inline void Write2FILE ( std::string filename ) {
            FILE *f = fopen(&(filename[0]),"w");
            Write2FILE(f);
            fclose(f);
        }
    public:
        inline void operator () (std::vector <std::string> & x, size_t y) {
            size_t limit = mymin (x.size(),X) ;
            for(size_t i=0;i<limit;i++) {GetElement(i,y)=x[i];}
        }
        inline void operator () (size_t x, std::vector <std::string> & y) {
            size_t limit = mymin (y.size(),Y) ;
            for(size_t i=0;i<limit;i++) {GetElement(x,i)=y[i];}
        }
        inline void operator () (FileLines & x, size_t y) {
            size_t limit = mymin(x(),X);
            for(size_t i=0;i<limit;i++) {GetElement(i,y)=x(i);}
        }
        inline void operator () (size_t x, FileLines & y) {
            size_t limit = mymin(y(),Y);
            for(size_t i=0;i<limit;i++) {GetElement(x,i)=y(i);}
        }
        inline void operator () (FILE *f)              {Write2FILE(f);}
        inline void operator () (std::string filename) {Write2FILE(filename);}
        LatexPrintMatrix (size_t _X, size_t _Y) {
            X=_X;Y=_Y;
            elements.resize(X*Y);
        }
        ~LatexPrintMatrix(){}
    };

#endif