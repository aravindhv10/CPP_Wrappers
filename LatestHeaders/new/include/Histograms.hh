#ifndef _HEADER_GUARD_Histograms_
#define _HEADER_GUARD_Histograms_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./StaticArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ MyHist

template <size_t Num = 1, typename T = float> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    static inline size_t constexpr SIZE() { return Num; } //
    using TYPE_DATA = T;                                  //
    using TYPE_HIST = StaticArray::ND_ARRAY<SIZE(), TYPE_DATA>;
    using TYPE_SELF = _MACRO_CLASS_NAME_<SIZE(), TYPE_DATA>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    ///////////////////////////////
    // Main Data Elements BEGIN: //
    ///////////////////////////////
  private:
    TYPE_DATA const Begin, End;
    TYPE_HIST       DATA;
    /////////////////////////////
    // Main Data Elements END. //
    /////////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    static inline size_t constexpr N1() { return 0; }

    static inline size_t constexpr N2() { return SIZE(); }

    static inline TYPE_DATA constexpr N2_Minux_M1() {
        return static_cast<TYPE_DATA>(N2() - N1());
    }

    inline TYPE_DATA const N_to_X(size_t const N) const {
        return End - (((End - Begin) * (N2() - N)) / N2_Minux_M1());
    }

    inline size_t X_to_N(TYPE_DATA const X) const {
        size_t const ret =
          N2() -
          (static_cast<size_t>((End - X) * N2_Minux_M1() / (End - Begin)));

        return CPPFileIO::mymin(ret, N2() - 1);
    }

    inline void Fill(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        size_t const index = X_to_N(X);
        DATA[index] += W;
    }

    inline void Show(FILE *f = stdout) const {
        for (size_t i = N1(); i < N2(); i++) {
            TYPE_DATA const x1 = N_to_X(i);
            TYPE_DATA const x2 = N_to_X(i + 1);
            fprintf(f, "%e %e\n%e %e\n", x1, DATA[i], x2, DATA[i]);
        }
    }

    inline void Show(std::string const Filename) const {
        FILE *f = fopen(&(Filename[0]), "w");
        Show(f);
        fclose(f);
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ///////////////////////////
    // Main Interface BEGIN: //
    ///////////////////////////
  public:
    inline void operator()(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        Fill(X, W);
    }

    inline void operator>>(FILE *f) const { Show(f); }

    inline void operator>>(std::string const Filename) const { Show(Filename); }

    _MACRO_CLASS_NAME_(TYPE_DATA const _Begin, TYPE_DATA const _End)
      : Begin(_Begin), End(_End) {
        DATA = 0;
    }

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////////
    // Main Interface END. //
    /////////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ DynamicHist

template <typename T = float> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_DATA = T;
    using TYPE_HIST = CPPFileIO::Dynamic1DArray<TYPE_DATA>;
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_DATA>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    ///////////////////////////////
    // Main Data Elements BEGIN: //
    ///////////////////////////////
  private:
    TYPE_DATA const Begin, End;
    TYPE_HIST       DATA;
    /////////////////////////////
    // Main Data Elements END. //
    /////////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    static inline size_t constexpr N1() { return 0; }

    inline size_t N2() { return DATA(); }

    inline TYPE_DATA N2_Minux_M1() {
        return static_cast<TYPE_DATA>(N2() - N1());
    }

    inline TYPE_DATA const N_to_X(size_t const N) const {
        return End - (((End - Begin) * (N2() - N)) / N2_Minux_M1());
    }

    inline size_t X_to_N(TYPE_DATA const X) const {
        size_t const ret =
          N2() -
          (static_cast<size_t>((End - X) * N2_Minux_M1() / (End - Begin)));

        return CPPFileIO::mymin(ret, N2() - 1);
    }

    inline void Fill(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        size_t const index = X_to_N(X);
        DATA(index) += W;
    }

    inline void Show(FILE *f = stdout) const {
        for (size_t i = N1(); i < N2(); i++) {
            TYPE_DATA const x1 = N_to_X(i);
            TYPE_DATA const x2 = N_to_X(i + 1);
            fprintf(f, "%e %e\n%e %e\n", x1, DATA(i), x2, DATA(i));
        }
    }

    inline void Show(std::string const Filename) const {
        FILE *f = fopen(&(Filename[0]), "w");
        Show(f);
        fclose(f);
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ///////////////////////////
    // Main Interface BEGIN: //
    ///////////////////////////
  public:
    inline void operator()(TYPE_DATA const X, TYPE_DATA const W = 1.0) {
        Fill(X, W);
    }

    inline void operator>>(FILE *f) const { Show(f); }

    inline void operator>>(std::string const Filename) const { Show(Filename); }

    _MACRO_CLASS_NAME_(TYPE_DATA const _Begin, TYPE_DATA const _End,
                       size_t const _N)
      : Begin(_Begin), End(_End), DATA(_N) {
        DATA = 0;
    }

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////////
    // Main Interface END. //
    /////////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ FullHist

template <typename T>
class _MACRO_CLASS_NAME_ {

public:
	using TYPE_DATA = T ;
	using TYPE_SELF = _MACRO_CLASS_NAME_ <TYPE_DATA> ;

private:
	std::string const source_name ;

private:
	inline std::string dirname () const {
		std::string ret = source_name + ".dir/" ;
		mkdir(ret.c_str(),0755);
		return ret ;
	}

	inline std::string histname() const {
		return dirname() + "out.hist.txt" ;
	}
	inline std::string dataname() const {
		return dirname() + "out.data.bin" ;
	}
	inline std::string statsname() const {
		return dirname() + "out.stats.txt" ;
	}

private:
	inline void copy () {
		CPPFileIO::FullFileReader <TYPE_DATA> reader(source_name) ;
		CPPFileIO::FileWriter <TYPE_DATA> writer(dataname()) ;
		for(size_t i=0;i<reader();i++){
			writer(reader(i));
		}
	}

	inline void sort () {
		CPPFileIO::SortFile <TYPE_DATA> (dataname()) ;
	}

public:
	inline void MakeHist (TYPE_DATA const begin, TYPE_DATA const end, size_t const n_bins) {
		copy();
		sort();
		DynamicHist <TYPE_DATA> hist (begin,end,n_bins) ;
		CPPFileIO::FullFileReader <TYPE_DATA> reader(dataname()) ;
		for(size_t i=0;i<reader();i++){
			hist(reader(i));
		}
		hist >> histname() ;
	}

	inline void MakeHist (size_t const n_bins) {
		copy();
		sort();
		CPPFileIO::FullFileReader <TYPE_DATA> reader(dataname()) ;
		DynamicHist <TYPE_DATA> hist (reader(0),reader(reader()-1),n_bins) ;
		for(size_t i=0;i<reader();i++){
			hist(reader(i));
		}
		hist >> histname() ;
	}

	inline void MakeStats (FILE * f, TYPE_DATA const fraction) {
		CPPFileIO::FullFileReader <TYPE_DATA> reader(dataname()) ;
		size_t const limit = reader() - 1 ;
		size_t const index = static_cast <size_t> ( fraction * static_cast<TYPE_DATA>(limit) );
		fprintf(f,"%e : %e\n",fraction,reader(index));
	}

	inline void MakeStats (std::vector <TYPE_DATA> const & in) {
		FILE * f = fopen(statsname().c_str(),"w");
		for(size_t i=0;i<in.size();i++){
			MakeStats(f,in[i]);
		}
		fclose(f);
	}

public:
	_MACRO_CLASS_NAME_(std::string const _source_name) : source_name(_source_name){
		copy();
		sort();
	}
	~_MACRO_CLASS_NAME_(){}

} ;

#undef _MACRO_CLASS_NAME_


#endif
