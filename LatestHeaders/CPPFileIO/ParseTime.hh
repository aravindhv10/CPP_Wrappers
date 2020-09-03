namespace CPPFileIO {

	inline time_t
	Parse_Time(
		char const * intime
	){
		std::tm mytime ;
		strptime(intime, "%Y-%m-%d %H:%M:%S", &mytime);
		std::time_t ret = std::mktime(&mytime);
		return ret ;
	}

	inline time_t
	Parse_Time(
		std::string const intime
	){
		return Parse_Time(intime.c_str()) ;
	}

}
