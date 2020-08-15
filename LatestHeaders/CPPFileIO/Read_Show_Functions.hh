namespace Read_Show_Functions {

#define COMPARE_INT(intype)                  \
	inline char                          \
	Compare (                            \
		intype const a               \
		, intype const b             \
	) {                                  \
		return                       \
			  ( -1 * (a  < b ) ) \
			+ (  0 * (a == b ) ) \
			+ (  1 * (a  > b ) ) \
		;                            \
	}                                    //
	COMPARE_INT(char)
	COMPARE_INT(unsigned char)
	COMPARE_INT(int)
	COMPARE_INT(unsigned int)
	COMPARE_INT(long)
	COMPARE_INT(size_t)
	COMPARE_INT(float)
	COMPARE_INT(double)
#undef COMPARE_INT

	inline int
	Compare (
		char const * S1
		, char const * S2
	) {
		int const val = strcmp(S1,S2);
		return Compare(val,0);
	}

	template <size_t n>
	inline int
	Compare(
		Tensors::Array::ND_ARRAY <n,char> const & S1
		, Tensors::Array::ND_ARRAY <n,char> const & S2
	) {
		return
			Compare(
				S1.GET_DATA()
				, S2.GET_DATA()
			)
		; //
	}

	template <size_t m, size_t n>
	inline int
	Compare(
		Tensors::Array::ND_ARRAY <m,char> const & S1
		, Tensors::Array::ND_ARRAY <n,char> const & S2
	) {
		return
			Compare(
				S1.GET_DATA()
				, S2.GET_DATA()
			)
		; //
	}

	template <size_t n>
	inline int
	Compare(
		Tensors::Array::ND_ARRAY <n,char> const & S1
		, std::string const & S2
	) {
		return Compare(S1.GET_DATA(),S2.c_str());
	}
	
	template <size_t n>
	inline int
	Compare(
		std::string const & S1
		, Tensors::Array::ND_ARRAY <n,char> const & S2
	) {
		return Compare(S2,S1) * -1 ;
	}

	inline int
	Compare(
		std::string const & S1
		, std::string const & S2
	) {
		return Compare(S1.c_str(),S2.c_str()) ;
	}

	static inline void
	Show_Label
	(	char const * c
	,	FILE * f = stdout
	) {	fprintf(f,"%s",c);
	}

	template <size_t n>
	static inline void
	Show_All
	(	Tensors::Array::ND_ARRAY <n,char> const & in
	,	FILE *f = stdout
	) {	fprintf(f,"%s",in.GET_DATA());
	}

	static inline void
	Show_Next
	(	FILE *f = stdout
	) {	fprintf(f,"\n");
	}

	static inline void
	Show_All (FILE *f = stdout)
	{ fprintf(f,"\t"); }

	static inline void
	Show_All
		( char const in
		, FILE *f = stdout
	) { fprintf(f,"%c",in); }

	static inline void
	Show_All(unsigned char const in, FILE *f = stdout) {
		fprintf(f,"%d", static_cast<int>(in));
	}

	static inline void
	Show_All(int const in, FILE *f = stdout) {
		fprintf(f,"%d",in);
	}

	static inline void
	Show_All(long const in, FILE *f = stdout) {
		fprintf(f,"%ld",in);
	}

	static inline void
	Show_All(size_t const in, FILE *f = stdout) {
		fprintf(f,"%zu",in);
	}

	static inline void
	Show_All(float const in, FILE *f = stdout) {
		fprintf(f,"%f",in);
	}

	static inline void
	Show_All(double const in, FILE *f = stdout) {
		fprintf(f,"%lf",in);
	}

	template <size_t n>
	inline void
	Read_All(
		Tensors::Array::ND_ARRAY <n,char> & dest
		, std::string const & src
	) {
		std::memset(
			/* void *s = */ reinterpret_cast<void*>(dest.GET_DATA()) ,
			/* int c = */ 0 ,
			/* size_t n = */ dest.SIZE()
		) ;
		size_t const limit =
			CPPFileIO::mymin(
				static_cast<size_t>(src.size()) ,
				static_cast<size_t>(dest.SIZE()-1)
			)
		; //
		memcpy(
			/* void *dest = */ reinterpret_cast<void*>(dest.GET_DATA()) ,
			/* const void *src = */ reinterpret_cast<void const*>(src.c_str()) ,
			/* size_t n = */ limit
		);
	}

	static inline void
	Read_All (
		float & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=-999.99;}
		else{dest = std::stof(src);}
	}

	static inline void
	Read_All (
		double & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=-999.99;}
		else{dest = std::stod(src);}
	}

	static inline void
	Read_All (
		char & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=0;}
		else{dest = src[0];}
	}

	static inline void
	Read_All (
		unsigned char & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=0;}
		else{dest = static_cast<unsigned char>(std::stoi(src));}
	}

	static inline void
	Read_All (
		int & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=-999;}
		else{dest = std::stoi(src);}
	}

	static inline void
	Read_All (
		long & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=-999;}
		else{dest = std::stol(src);}
	}

	static inline void
	Read_All (
		size_t & dest
		, std::string const & src
	) {
		if(src.size()<1){dest=0;}
		else{
			sscanf(src.c_str(),"%zu",&dest);
		}
	}

}
