namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ LineReader

	class _MACRO_CLASS_NAME_ {

	public:

		using TYPE_SELF = _MACRO_CLASS_NAME_ ;
		using TYPE_LINES = std::vector <std::string> ;

	private:

		std::string const filename ;
		FileArray <char> filereader ;
		size_t current_loc ;
		size_t const limit ;
		char const seperator ;
		TYPE_LINES lines ;

	public:

		inline TYPE_LINES &
		next (
			size_t const linesize = 4096
		) {

			lines.clear();

			if(current_loc>=limit) {return lines;}

			char const * buffer = &(filereader(current_loc,linesize)) ;
			size_t i = 0 ;
			std::string data ;

			begin_field: {
				data.clear();
				goto checker;
			}

			checker: {
				if (
					( (current_loc+i) >= limit )
					|| ( buffer[i] == '\n' )
				) {
					goto end_of_line ;
				} else if ( buffer[i] == seperator ) {
					goto found_seperator ;
				} else {
					goto normal_char ;
				}
			}

			found_seperator: {
				lines.push_back(data);
				i++;
				goto begin_field;
			}

			normal_char: {
				data.push_back(buffer[i]);
				i++;
				goto checker;
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
			, char const _seperator
		) :
			filename(_filename)
			, filereader(filename)
			, current_loc(0)
			, limit(filereader.size())
			, seperator(_seperator) {
			lines.clear();
		}

		~_MACRO_CLASS_NAME_(){}

	} ;

#undef _MACRO_CLASS_NAME_

}
