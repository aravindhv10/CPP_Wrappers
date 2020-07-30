namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ FasterLineReader

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
		size_t const linesize ;
		size_t const memsize ;
		size_t const numlines ;
		size_t count ;

	public:

		inline TYPE_LINES &
		next () {

			lines.clear();

			if(current_loc>=limit) {return lines;}

			size_t readsize = linesize ;

			if((count&(numlines-1))==0){readsize=memsize;}

			size_t const max_i =
				CPPFileIO::mymin (
					limit -	current_loc ,
					readsize
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
				count++;
				return lines;
			}

		}

		_MACRO_CLASS_NAME_(
			std::string const _filename
			, size_t const _linesize = 12
			, size_t const _memsize = 29
		) :
			filename(_filename)
			, filereader(filename)
			, current_loc(0)
			, limit(filereader.size())
			, linesize(shifter(_linesize))
			, memsize(shifter(_memsize))
			, numlines(shifter(_memsize-_linesize))
			, count(0)
		   	{lines.clear();}

		~_MACRO_CLASS_NAME_(){}

	} ;

#undef _MACRO_CLASS_NAME_

}
