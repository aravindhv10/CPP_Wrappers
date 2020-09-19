namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ FastLineReader

	template <char seperator='\t', char newline='\n'>
	class _MACRO_CLASS_NAME_ {

	public:

		using TYPE_SELF = _MACRO_CLASS_NAME_<seperator,newline> ;
		using TYPE_LINES = std::vector <std::string> ;

	private:

		std::string const filename ;
		FileArray <char> filereader ;
		size_t current_loc ;
		size_t const limit ;
		TYPE_LINES lines ;

	public:

		inline TYPE_LINES &
		next (
			size_t const linesize = 4096
		) {
			lines.clear();

			if(current_loc>=limit) {return lines;}

			size_t const max_i =
				CPPFileIO::mymin (
					limit -	current_loc ,
					linesize
				)
			; //

			char const * buffer =
				&(filereader(current_loc,max_i))
			; //

			std::string data ;
			size_t i = 0 ;
			start: {
				if(i<max_i){
					switch (buffer[i]) {
						case seperator:
							lines.push_back(data);
							data.clear();
							i++;
							goto start;

						case newline:
							goto end_of_line;

						default:
							data.push_back(buffer[i]);
							i++;
							goto start;
					}
				} else {
					goto end_of_line;
				}
			}

			end_of_line:{
				lines.push_back(data);
				i++;
				current_loc += i ;
				return lines;
			}

		}

		_MACRO_CLASS_NAME_(
			std::string const _filename
		) :
			filename(_filename)
			, filereader(filename)
			, current_loc(0)
			, limit(filereader.size())
		   	{lines.clear();}

		~_MACRO_CLASS_NAME_(){}

	} ;

#undef _MACRO_CLASS_NAME_

}
