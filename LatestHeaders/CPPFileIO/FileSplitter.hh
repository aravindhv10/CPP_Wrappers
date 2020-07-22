namespace CPPFileIO {
	template <typename T>
	inline void
	SplitFile(
		std::string const infilename
		, std::vector <std::string> const & outnames
	){
		FileArray <T const> reader(infilename);
		size_t const limit = reader.size();
//		printf("DEBUG: limit = %zu\n",limit);
		for(size_t i=0;i<outnames.size();i++){
			FileVector <T> writer (outnames[i]) ;
			size_t const begin =
				limit
				- (
					( (outnames.size()-i) * limit )
					/ outnames.size()
				)
			; //
			size_t const end =
				limit
				- (
					( (outnames.size()-(i+1)) * limit )
					/ outnames.size()
				)
			; //
			for(size_t j=begin;j<end;j++){
				writer.push_back(reader(j));
			}
//			printf("DEBUG: %zu %zu\n",begin,end);
		}
	}
}
