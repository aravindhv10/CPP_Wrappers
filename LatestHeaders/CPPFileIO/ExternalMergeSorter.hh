namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ ExternalMergeSorter

	template <typename T>
	class _MACRO_CLASS_NAME_ {
	private:

		using TYPE_ELEMENT = T ;

		using TYPE_SELF = _MACRO_CLASS_NAME_ <TYPE_ELEMENT> ;

		std::string const infilename ;
		std::string const outfilename ;
		std::string const dirname ;
		CPPFileIO::FileArray <TYPE_ELEMENT> MainReader ;
		size_t const limit ;
		size_t const N_SPLITS ;

		inline std::string
		GetFileName (
			size_t const i
		) const {
			mkdir(&(dirname[0]),0755);
			char tmp[128] ;
			sprintf(tmp,"/bin.%zu.piece",i);
			return dirname + std::string(tmp) ;
		}

		inline std::string
		GetFileName (
			size_t const i1,
			size_t const i2
		) const {
			if(i1==i2){
				return GetFileName(i1) ;
			} else {
				mkdir(&(dirname[0]),0755);
				char tmp[128] ;
				sprintf(tmp,"/bin.%zu-%zu.sorted",i1,i2);
				std::string ret = dirname + std::string(tmp) ;
				return ret ;
			}
		}

		inline void
		MergeRange (
			size_t const i1 ,
			size_t const i2
		) {
			if (i1==i2) {
				return;
			} else if (i2==(i1+1)) {
				CPPFileIO::MergeFile <TYPE_ELEMENT> (
					GetFileName(i1) ,
					GetFileName(i2) ,
					GetFileName(i1,i2)
				) ;
				{
					CPPFileIO::ExternalStarter<true>::GET("/bin/rm")
						("-v")("--")
						(GetFileName(i1))
						(GetFileName(i2))
					;
				}
			} else {
				size_t const mid = (i1+i2) / 2 ;
				MergeRange(i1,mid);
				MergeRange(mid+1,i2);
				CPPFileIO::MergeFile <TYPE_ELEMENT> (
					GetFileName(i1,mid) ,
					GetFileName(mid+1,i2) ,
					GetFileName(i1,i2)
				) ;
				{
					CPPFileIO::ExternalStarter<true>::GET("/bin/rm")
						("-v")("--")
						(GetFileName(i1,mid))
						(GetFileName(mid+1,i2))
					;
				}
			}
		}

		inline void
		MergeRange () {
			MergeRange(0,N_SPLITS-1);
			std::string filename = GetFileName(0,N_SPLITS-1);
			{
				CPPFileIO::ExternalStarter<true>::GET("/bin/mv")
					("-v")("--")
					(filename)(outfilename)
				;
			}
			{
				CPPFileIO::ExternalStarter<true>::GET("/bin/rmdir")
					("-v")("--")(dirname)
				;
			}
		}

		inline void
		SortFile (size_t const index) {
			std::string const filename = GetFileName(index) ;
			CPPFileIO::SortFile <TYPE_ELEMENT> (filename) ;
		}

		inline void
		SortAllFiles () {
			for(size_t i=0;i<N_SPLITS;i++){
				SortFile(i);
			}
		}

		inline void
		DoSplit () {
			size_t const I = N_SPLITS ;
			using TYPE_WRITER = CPPFileIO::FileVector<TYPE_ELEMENT> ;
			std::vector <TYPE_WRITER*> writers(N_SPLITS) ;
			for(size_t i=0;i<N_SPLITS;i++){
				writers[i] = new TYPE_WRITER(GetFileName(i)) ;
			}
			for(size_t i=0;i<limit;i++){
				writers[i%N_SPLITS]->push_back(MainReader(i));
			}
			for(size_t i=0;i<writers.size();i++) {
				delete writers[i] ;
			}
		}

		_MACRO_CLASS_NAME_ (
			std::string const _infilename	,
			std::string const _outfilename	,
			size_t const n_splits
		) :	infilename(_infilename)		,
			outfilename(_outfilename)	,
			dirname(outfilename+".dir")	,
			MainReader(infilename)		,
			limit(MainReader.size())	,
			N_SPLITS(n_splits)			{}

		~_MACRO_CLASS_NAME_(){}

	public:

		static inline void
		DoAllSteps (
			std::string const binfilename = "./infile",
			std::string const tmpdirname = "./tmps",
			size_t const n_splits = 16
		) {
			TYPE_SELF
				slave (
					binfilename,
					tmpdirname,
					n_splits
				)
			; //
			slave.DoSplit();
			slave.SortAllFiles();
			slave.MergeRange();
		}

	} ;

#undef _MACRO_CLASS_NAME_

}
