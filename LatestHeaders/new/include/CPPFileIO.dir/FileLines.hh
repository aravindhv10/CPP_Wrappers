﻿#ifndef _HEADER_GUARD_CPPFileIO_FileLines_
#define _HEADER_GUARD_CPPFileIO_FileLines_

/////////////////////
// Includes BEGIN: //
/////////////////////
#include "./FileFD.hh"
///////////////////
// Includes END. //
///////////////////

	void getnames (
		std::vector <std::string> & lines ,
		std::string const filename
	) {
		lines.clear();
		char *line=NULL;
		size_t len=0;
		ssize_t read;
		FILE *stream = fopen (&(filename[0]),"r") ;
		while (
			(read = getline(&line, &len, stream)) != -1
		) {
			std::string linename(line);
			linename[linename.size()-1]=0;
			lines.push_back(linename);
		}
		if (len>0) {free(line);}
		fclose(stream);
	}

	class FileLines {
	private:
		std::vector <std::string> lines    ;
		std::string               filename ;
	public:
		FileLines(std::string _filename):
			filename(_filename) {
			getnames(lines,filename);
		}
		~FileLines () {}
		inline std::string & operator [] (size_t i)       { return lines[i]      ; }
		inline size_t        size        ()         const { return lines.size () ; }
		inline std::string & operator () (size_t i)       { return lines[i]      ; }
		inline size_t        operator () ()         const { return lines.size () ; }
		inline void operator () (FILE *f) { for (size_t i=0;i<lines.size();i++) {fprintf(f,"%s\n",&(lines[i][0]));} }
		void debug_show () { for (size_t i=0;i<lines.size();i++) {printf("%ld => [%s]\n",i,&(lines[i][0]));} }
	};

    class StringSplit {
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The Main Data Elements: /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::string               line    ; ////////////////////////////////////////////////////////////////////////////////////////
        std::vector <std::string> words   ; ////////////////////////////////////////////////////////////////////////////////////////
        char                      delimit ; ////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Important functions: ////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        StringSplit  ( std::string _line = "" , char _delimit = ' ' ) : line(_line) , delimit(_delimit) { //////////////////////////
            std::vector <char> buf ; buf.reserve(line.size()) ; ////////////////////////////////////////////////////////////////////
            for (size_t i=0;i<line.size();i++) { ///////////////////////////////////////////////////////////////////////////////////
                if (line[i]==delimit) { ////////////////////////////////////////////////////////////////////////////////////////////
                    buf.push_back((char)0); ////////////////////////////////////////////////////////////////////////////////////////
                    words.push_back(std::string(&(buf[0]))); ///////////////////////////////////////////////////////////////////////
                    buf.resize(0) ; ////////////////////////////////////////////////////////////////////////////////////////////////
                    buf.push_back(line[i]); ////////////////////////////////////////////////////////////////////////////////////////
                    buf.push_back((char)0); ////////////////////////////////////////////////////////////////////////////////////////
                    words.push_back(std::string(&(buf[0]))); ///////////////////////////////////////////////////////////////////////
                    buf.resize(0) ; ////////////////////////////////////////////////////////////////////////////////////////////////
                } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                else { buf.push_back(line[i]); } ///////////////////////////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (buf.size()>0) { ////////////////////////////////////////////////////////////////////////////////////////////////////
                buf.push_back((char)0); ////////////////////////////////////////////////////////////////////////////////////////////
                words.push_back(std::string(&(buf[0]))); ///////////////////////////////////////////////////////////////////////////
                buf.resize(0) ; ////////////////////////////////////////////////////////////////////////////////////////////////////
            } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~StringSplit () {} /////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Some convinent functions: ///////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline std::string & operator [] (size_t i) {return words[i]     ; } ///////////////////////////////////////////////////////
        inline size_t        size        ()         {return words.size() ; } ///////////////////////////////////////////////////////
        inline std::string & operator () (size_t i) {return words[i]     ; } ///////////////////////////////////////////////////////
        inline size_t        operator () ()         {return words.size() ; } ///////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Print the string: ///////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void show (std::string & in_line) { ////////////////////////////////////////////////////////////////////////////////////////
            std::vector <char> outline ; ///////////////////////////////////////////////////////////////////////////////////////////
            for(size_t i=0;i<words.size();i++) for(size_t j=0;j<words[i].size();j++) if(words[i][j]!=(char)0) {outline.push_back(words[i][j]);}
            outline.push_back((char)0); ////////////////////////////////////////////////////////////////////////////////////////////
            in_line = std::string( &(outline[0]) ); ////////////////////////////////////////////////////////////////////////////////
        } //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        inline void operator () (std::string & in_line) {show(in_line);} ///////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void debug_show () { for (size_t i=0;i<words.size();i++) {printf("(%ld:%s)",i,&(words[i][0]));} } //////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    } ; ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
