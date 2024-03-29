//#define _MACRO_STRING_(sz) Tensors::Array::ND_ARRAY <sz,char>

	static inline void
	Show_Label(char const * c, FILE * f = stdout) {
		fprintf(f,"%s",c);
	}

	template <size_t n>
	static inline void
	Show_All (Tensors::Array::ND_ARRAY <n,char> const & in, FILE *f = stdout) {
		fprintf(f,"%s",in.GET_DATA());
	}

	static inline void
	Show_Next(FILE *f = stdout) {
		fprintf(f,"\n");
	}

	static inline void
	Show_All(FILE *f = stdout) {
		fprintf(f,"\t");
	}

	static inline void
	Show_All(char const in, FILE *f = stdout) {
		fprintf(f,"%c",in);
	}

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
