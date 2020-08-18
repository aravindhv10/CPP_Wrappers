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
		char const * buffer ;
		size_t current_loc ;
		size_t memloc ;
		size_t const limit ;
		TYPE_LINES lines ;
		size_t const memsize ;
		std::string data ;
		size_t max_i ;

		inline void
		Alloc () {
			current_loc += memloc ;
			memloc = 0 ;
			max_i =
				CPPFileIO::mymin (
					limit -	current_loc ,
					memsize
				)
			; //
			buffer =
				& ( filereader(current_loc,max_i) )
			; //
		}

		inline void
		got_separator () {
			lines.push_back(data);
			data.clear();
		}

		inline void
		got_normal_char () {
			data.push_back(buffer[memloc]);
		}


		inline TYPE_LINES const &
		next () {

			lines.clear();

			if ( (current_loc+memloc) >= limit ) {return lines;}

			data.clear();

			int status ;

			/* The main reading loop: */ {
				start:
				while(memloc<max_i) {
					switch (buffer[memloc]) {
						case seperator:
							got_separator();
							break ;
						case newline:
							goto end_of_line;
						default:
							got_normal_char();
							break ;
					}
					memloc++;
				}
				if( (current_loc+memloc) < limit ) {
					Alloc();
					goto start;
				}
			}

			/* Found end of line / file: */ {
				end_of_line:
				got_separator();
				memloc++;
				return lines;
			}

		}

	public:

		inline TYPE_LINES const &
		operator () () {
			return next() ;
		}

		_MACRO_CLASS_NAME_
		(	std::string const _filename
		,	size_t const _memsize = 25
		) :	filename(_filename)
		,	filereader(filename)
		,	current_loc(0)
		,	memloc(0)
		,	limit(filereader.size())
		,	memsize(shifter(_memsize))
		{	lines.clear()
		;	Alloc()
		; }

		~_MACRO_CLASS_NAME_(){}

	} ;

#undef _MACRO_CLASS_NAME_

}
