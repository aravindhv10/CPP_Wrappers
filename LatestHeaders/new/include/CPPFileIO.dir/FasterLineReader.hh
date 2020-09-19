#ifndef _HEADER_GUARD_CPPFileIO_FasterLineReader_
#define _HEADER_GUARD_CPPFileIO_FasterLineReader_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ FasterLineReader

	template
	<	char seperator='\t'
	,	char newline='\n'
	>
	class _MACRO_CLASS_NAME_ {

	////////////////////////
	// Definitions BEGIN: //
	////////////////////////

	public:

		using TYPE_SELF =
			_MACRO_CLASS_NAME_
			<	seperator
			,	newline
			>
		;

		using TYPE_LINES =
			std::vector <std::string>
		;

	//////////////////////
	// Definitions END. //
	//////////////////////

	//////////////////////////
	// Data Elements BEGIN: //
	//////////////////////////

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

	////////////////////////
	// Data Elements END. //
	////////////////////////

	///////////////////////////////////
	// Main Working Functions BEGIN: //
	///////////////////////////////////

	private:
		inline void
		Alloc () {
			current_loc += memloc ;
			memloc = 0 ;
			max_i =
				mymin
				(	limit -	current_loc
				,	memsize
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

			if ( (current_loc+memloc) >= limit )
			{return lines;}

			data.clear();

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

	/////////////////////////////////
	// Main Working Functions END. //
	/////////////////////////////////

	///////////////////////////
	// Main Interface BEGIN: //
	///////////////////////////

	public:

		inline TYPE_LINES const &
		operator () ()
		{	return next() ;
		}

	/////////////////////////
	// Main Interface END. //
	/////////////////////////

	/////////////////////////////////////
	// Constructor & Destructor BEGIN: //
	/////////////////////////////////////

	public:

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

	///////////////////////////////////
	// Constructor & Destructor END. //
	///////////////////////////////////

	} ;

#undef _MACRO_CLASS_NAME_

#endif
