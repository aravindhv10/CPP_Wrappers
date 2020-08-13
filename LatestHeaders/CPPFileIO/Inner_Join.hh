namespace CPPFileIO {

	template <typename T1, typename T2, typename O>
	inline void
	Inner_Join (
		std::string const filename1
		, std::string const filename2
		, std::string const outfilename
	) {

		CPPFileIO::FileArray <T1> R1 (filename1) ;
		CPPFileIO::FileArray <T2> R2 (filename2) ;
		CPPFileIO::FileWriter <O> W (outfilename) ;

		size_t L1 = R1() ;
		size_t L2 = R2() ;
		size_t I1 = 0 ;
		size_t I2 = 0 ;

		while (
			(I1<L1) &&
			(I2<L2)
		) {

			O tmp ;
			int const val = tmp(R1(I1),R2(I2));

			switch (val) {

				case 1:
					I2++ ;
					break ;

				case 0:
					W(tmp);

				case -1:
					I1++ ;
					break ;

			}

		}

	}

}
