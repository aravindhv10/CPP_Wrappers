namespace CPPFileIO {
	template <typename T, typename R=pcg64_fast>
	inline void
	shuffle (
		std::string const
			filename
	) {
		FileArray <T> filereaderS (filename) ;
		size_t const limit = filereaderS.size() ;
		std::random_device randinit ;
		R engine(randinit()) ;
		filereaderS.writeable(true) ;
		/* Shuffel the file */ {
			FileArray <T> filereaderD (filename) ;
			filereaderD.writeable(true);
			for(size_t i=0;i<(limit-1);i++){
				std::uniform_int_distribution <size_t> dis(i,limit-1) ;
				size_t const j = dis(engine) ;
				if(i!=j){
					T const tmp = filereaderS(i) ;
					filereaderS(i) = filereaderD(j) ;
					filereaderD(j) = tmp ;
				}
			}
		}
		filereaderS.size(limit);
	}
}
