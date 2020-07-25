namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ AnalyzeLines

	class _MACRO_CLASS_NAME_ {

	public:

		using TYPE_SELF = _MACRO_CLASS_NAME_ ;

		static inline size_t constexpr
		CODE_NOT_NUMBER () {
			return 1 ;
		}

		static inline size_t constexpr
		CODE_HAS_DECIMAL () {
			return 2 ;
		}

		static inline size_t constexpr
		CODE_HAS_NEGATIVE () {
			return 4 ;
		}

		static inline size_t constexpr
		CODE_BIGGER_THAN_CHAR () {
			return 8 ;
		}

	private:

		static inline bool
		Check (
			size_t const in1 ,
			size_t const in2
		) {
			return (in1&in2) == in2 ;
		}

		static inline std::string
		InferCodes (
			size_t const in,
			size_t const sz
		) {
			if( Check(in,CODE_NOT_NUMBER()) ){
				char tmp[128] ;
				sprintf(tmp,"Tensors::Array::ND_ARRAY <%ld,char>",sz+1);
				return std::string(tmp);
			}
			if( Check(in,CODE_HAS_DECIMAL()) ){return std::string("double");}
			if( Check(in,CODE_HAS_NEGATIVE()) ){return std::string("long");}
			if( Check(in,CODE_BIGGER_THAN_CHAR()) ){return std::string("size_t");}
			return std::string("unsigned char");
		}

		inline std::string
		InferCodes (size_t const i) const {
			return InferCodes(status_codes[i],sizes[i]);
		}

		template <typename T>
		inline void
		Read_All (
			T & dst,
			std::string const & src
		) {
			std::stringstream ss(src) ;
			ss >> dst ;
		}

		std::vector <size_t> sizes ;
		std::vector <size_t> status_codes ;

		inline size_t
		AnalyzeStatus(
			std::string const & in
		) {
			size_t ret = 0 ;
			if(in.size()==0){return ret;}
			for(size_t i=0;i<in.size();i++){
				switch (in[i]) {
					case '.' :
						ret = ret | CODE_HAS_DECIMAL() ;
						goto endofswitch;
					case '-' :
						ret = ret | CODE_HAS_NEGATIVE();
						goto endofswitch;
					case '1'  :
					case '2'  :
					case '3'  :
					case '4'  :
					case '5'  :
					case '6'  :
					case '7'  :
					case '8'  :
					case '9'  :
					case '0'  :
					case ' '  :
					case '\t' :
						goto endofswitch;
					default   :
						ret = ret | CODE_NOT_NUMBER() ;
						goto endofswitch;
				}
				endofswitch:;
			}
			if(ret==0){
				size_t tmpbuf ;
				Read_All(tmpbuf,in);
				if(tmpbuf>255){
					ret = ret | CODE_BIGGER_THAN_CHAR() ;
				}
			}
			return ret ;
		}

		inline void
		AnalyzeStatus (
			std::vector <std::string> const &
				in
		) {
			status_codes.reserve(in.size());
			while(status_codes.size()<in.size()){
				status_codes.push_back(0);
			}
			for(size_t i=0;i<in.size();i++){
				size_t const rettype = AnalyzeStatus(in[i]);
				status_codes[i] = status_codes[i] | rettype ;
			}
		}

		inline void
		ReadLine (
			std::vector <std::string> const &
				in
		) {
			sizes.reserve(in.size());
			while(in.size()>sizes.size()){
				sizes.push_back(0);
			}
			for(size_t i=0;i<in.size();i++){
				sizes[i]=CPPFileIO::mymax(sizes[i],in[i].size());
			}
		}

	public:

		inline void
		show() const {
			for(size_t i=0;i<sizes.size();i++){
				char tmp[8] ;
				sprintf(tmp,"L%zu_",i);
				printf(
					"%s %s; // %zu = %zu\n"
					, InferCodes(i).c_str()
					, tmp
					, i
					, sizes[i]
				);
			}
		}

		inline size_t
		operator () () const {
			return sizes.size();
		}

		inline size_t 
		operator [] (
			size_t const in
		) const {
			return sizes[in] ;
		}

		inline void
		operator () (
			std::vector <std::string> const &
				in
		) {
			ReadLine(in);
			AnalyzeStatus(in);
		}

		_MACRO_CLASS_NAME_(){}
		~_MACRO_CLASS_NAME_(){}
	
	};

#undef _MACRO_CLASS_NAME_

}
