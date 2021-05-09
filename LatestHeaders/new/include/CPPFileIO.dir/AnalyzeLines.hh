#ifndef _HEADER_GUARD_CPPFileIO_AnalyzeLines_
#define _HEADER_GUARD_CPPFileIO_AnalyzeLines_

/////////////////////
// Includes BEGIN: //
/////////////////////
#include "./Basic.hh"
#include "./FastTXT2BIN_NEW.hh"
#include "./FileFD.hh"
///////////////////
// Includes END. //
///////////////////

///////////////////////////
// Main Analyzer BEGIN:{ //
///////////////////////////
#define _MACRO_CLASS_NAME_ AnalyzeLines
class _MACRO_CLASS_NAME_ {

    /////////////////////////////
    // Main Definitions BEGIN: //
    /////////////////////////////
  public:
    using TYPE_SELF = _MACRO_CLASS_NAME_; //
    static inline size_t constexpr CODE_NOT_NUMBER() { return 1; }
    static inline size_t constexpr CODE_HAS_DECIMAL() { return 2; }
    static inline size_t constexpr CODE_HAS_NEGATIVE() { return 4; }
    static inline size_t constexpr CODE_BIGGER_THAN_CHAR() { return 8; }
    ///////////////////////////
    // Main Definitions END. //
    ///////////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::vector<size_t>      sizes;
    std::vector<size_t>      status_codes;
    std::vector<std::string> labels;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////
    // Merging BEGIN: //
    ////////////////////
  private:
    inline void Merge(TYPE_SELF const &other) {

        size_t const max_sizes = mymax(sizes.size(), other.sizes.size());

        size_t const max_status_codes =
          mymax(status_codes.size(), other.status_codes.size());

        sizes.resize(max_sizes);
        status_codes.resize(max_status_codes);

        for (size_t i = 0; i < other.sizes.size(); i++) {
            sizes[i] = mymax(sizes[i], other.sizes[i]);
        }

        for (size_t i = 0; i < other.status_codes.size(); i++) {
            status_codes[i] = status_codes[i] | other.status_codes[i];
        }
    }

  public:
    inline void operator()(TYPE_SELF const &other) { Merge(other); }
    //////////////////
    // Merging END. //
    //////////////////

    ////////////////////
    // File IO BEGIN: //
    ////////////////////
  private:
    inline ssize_t WriteOut(FileFD &in) const {
        ssize_t ret = (in << sizes);
        ret += (in << status_codes);
        ret += in.WriteStrings(labels);
        return ret;
    }

    inline ssize_t WriteOut(std::string const outfilename) const {
        FileFD file(outfilename);
        file.writefile();
        return WriteOut(file);
    }

    inline ssize_t ReadIn(FileFD &in) {
        ssize_t ret = (in >> sizes);
        ret += (in >> status_codes);
        ret += in.ReadStrings(labels);
        return ret;
    }

    inline ssize_t ReadIn(std::string const infilename) {
        FileFD file(infilename);
        file.readfile();
        return ReadIn(file);
    }

  public:
    inline ssize_t operator>>(std::string const filename) const {
        return WriteOut(filename);
    }

    inline ssize_t operator<<(std::string const filename) {
        return ReadIn(filename);
    }
    //////////////////
    // File IO END. //
    //////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    static inline bool Check(size_t const in1, size_t const in2) {
        return (in1 & in2) == in2;
    }

    static inline std::string InferCodes(size_t const in, size_t const sz) {
        if (Check(in, CODE_NOT_NUMBER())) {
            char tmp[128];
            sprintf(tmp, "StaticArray::ND_ARRAY <%ld,char>", (sz + 1));
            return std::string(tmp);
        } else if (Check(in, CODE_HAS_DECIMAL())) {
            return std::string("double");
        } else if (Check(in, CODE_HAS_NEGATIVE())) {
            return std::string("long");
        } else if (Check(in, CODE_BIGGER_THAN_CHAR())) {
            return std::string("size_t");
        } else {
            return std::string("unsigned char");
        }
    }

    inline std::string InferCodes(size_t const i) const {
        return InferCodes(status_codes[i], sizes[i]);
    }

    template <typename T> inline void Read_All(T &dst, std::string const &src) {
        std::stringstream ss(src);
        ss >> dst;
    }

    inline size_t AnalyzeStatus(std::string const &in) {

        size_t num_dot  = 0;
        size_t num_dash = 0;
        size_t ret      = 0;

        if (in.size() == 0) { return ret; }

        for (size_t i = 0; i < in.size(); i++) {

            switch (in[i]) {

                case '.':
                    ret = ret | CODE_HAS_DECIMAL();
                    num_dot++;
                    break;

                case '-':
                    ret      = ret | CODE_HAS_NEGATIVE();
                    num_dash = num_dash + 1 + ((i > 0) * 10);
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                case ' ':
                case '\t': break;

                default: ret = ret | CODE_NOT_NUMBER(); break;
            }
        }

        if ((num_dot > 1) || (num_dash > 1)) { ret = ret | CODE_NOT_NUMBER(); }

        if (ret == 0) {
            size_t tmpbuf;
            Read_All(tmpbuf, in);
            if (tmpbuf > 255) { ret = ret | CODE_BIGGER_THAN_CHAR(); }
        }

        return ret;
    }

    inline size_t AnalyzeStatus(char const *in) {

        size_t num_dot  = 0;
        size_t num_dash = 0;
        size_t ret      = 0;
        size_t const length = strlen(in);

        if (length == 0) { return ret; }

        for (size_t i = 0; i < length; i++) {

            switch (in[i]) {

                case '.':
                    ret = ret | CODE_HAS_DECIMAL();
                    num_dot++;
                    break;

                case '-':
                    ret      = ret | CODE_HAS_NEGATIVE();
                    num_dash = num_dash + 1 + ((i > 0) * 10);
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                case ' ':
                case '\t': break;

                default: ret = ret | CODE_NOT_NUMBER(); break;
            }
        }

        if ((num_dot > 1) || (num_dash > 1)) { ret = ret | CODE_NOT_NUMBER(); }

        if (ret == 0) {
            size_t tmpbuf;
            Read_All(tmpbuf, in);
            if (tmpbuf > 255) { ret = ret | CODE_BIGGER_THAN_CHAR(); }
        }

        return ret;
    }

    inline void AnalyzeStatus(std::vector<std::string> const &in) {
        status_codes.reserve(in.size());
        while (status_codes.size() < in.size()) { status_codes.push_back(0); }
        for (size_t i = 0; i < in.size(); i++) {
            size_t const rettype = AnalyzeStatus(in[i]);
            status_codes[i]      = status_codes[i] | rettype;
        }
    }

    inline void AnalyzeStatus(std::vector<char const *> const &in) {
        status_codes.reserve(in.size());
        while (status_codes.size() < in.size()) { status_codes.push_back(0); }
        for (size_t i = 0; i < in.size(); i++) {
            size_t const rettype = AnalyzeStatus(in[i]);
            status_codes[i]      = status_codes[i] | rettype;
        }
    }

    inline void ReadLine(std::vector<std::string> const &in) {
        sizes.reserve(in.size());
        while (in.size() > sizes.size()) { sizes.push_back(0); }
        for (size_t i = 0; i < in.size(); i++) {
            sizes[i] = mymax(sizes[i], in[i].size());
        }
    }

    inline void ReadLine(std::vector<char const *> const &in) {
        sizes.reserve(in.size());
        while (in.size() > sizes.size()) { sizes.push_back(0); }
        for (size_t i = 0; i < in.size(); i++) {
            size_t const length = strlen(in[i]);
            sizes[i] = mymax(sizes[i], length);
        }
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ////////////////////////////////////////
    // Header Generation Functions BEGIN: //
    ////////////////////////////////////////
  public:
    inline void show_read_write_wrappers(FILE *f = stdout) const {

        fprintf(f, "\n");

        fprintf(f, "\tstatic inline CPPFileIO::FullFileReader <TYPE_SELF>\n");
        fprintf(f, "\tGet_Reader_Full (std::string const filename) {\n");
        fprintf(f, "\t\treturn\n");
        fprintf(f, "\t\t\tCPPFileIO::FullFileReader <TYPE_SELF>\n");
        fprintf(f, "\t\t\t\t(filename)\n");
        fprintf(f, "\t\t; //\n");
        fprintf(f, "\t}\n");

        fprintf(f, "\n");

        fprintf(f, "\tstatic inline CPPFileIO::FileArray <TYPE_SELF>\n");
        fprintf(f, "\tGet_Reader (std::string const filename) {\n");
        fprintf(f, "\t\treturn\n");
        fprintf(f, "\t\t\tCPPFileIO::FileArray <TYPE_SELF>\n");
        fprintf(f, "\t\t\t\t(filename)\n");
        fprintf(f, "\t\t; //\n");
        fprintf(f, "\t}\n");

        fprintf(f, "\n");

        fprintf(f, "\tstatic inline CPPFileIO::FileWriter <TYPE_SELF>\n");
        fprintf(f, "\tGet_Writer (std::string const filename) {\n");
        fprintf(f, "\t\treturn\n");
        fprintf(f, "\t\t\tCPPFileIO::FileWriter <TYPE_SELF>\n");
        fprintf(f, "\t\t\t\t(filename)\n");
        fprintf(f, "\t\t; //\n");
        fprintf(f, "\t}\n");

        fprintf(f, "\n");
    }

    inline void show_labels(FILE *f = stdout) const {
        printf("\n");
        for (size_t i = 0; i < labels.size(); i++) {
            fprintf(f, "#define L%zu_ %s\n", i, labels[i].c_str());
        }
        printf("\n");
        for (size_t i = 0; i < labels.size(); i++) {
            fprintf(f, "#undef L%zu_\n", i);
        }
        printf("\n");
    }

    inline void show_header(FILE *f = stdout) const {
        fprintf(f, "\n");
        fprintf(f, "#define _MACRO_CLASS_NAME_ StorageElement\n");
        fprintf(f, "\n");
        fprintf(f, "class _MACRO_CLASS_NAME_ {\n");
        fprintf(f, "\n");
        fprintf(f, "private:\n");
        fprintf(f, "\n");
        fprintf(f, "public:\n");
        fprintf(f, "\tusing TYPE_SELF = _MACRO_CLASS_NAME_;\n");
        fprintf(f, "\n");
    }

    inline void show_data(FILE *f = stdout) const {
        for (size_t i = 0; i < sizes.size(); i++) {
            char tmp[8];
            sprintf(tmp, "L%zu_", i);
            fprintf(f, "\t%s %s; // %zu = %zu\n", InferCodes(i).c_str(), tmp, i,
                    sizes[i]);
        }
        fprintf(f, "\n");
    }

    inline void show_read(FILE *f = stdout) const {
        fprintf(f, "\n");
        fprintf(f, "\tinline void\n");
        fprintf(f, "\tRead_All (\n");
        fprintf(f, "\t\tstd::vector <std::string> const &\n");
        fprintf(f, "\t\t\tin\n");
        fprintf(f, "\t) {\n");
        fprintf(f, "\t\tusing namespace Read_Show_Functions;\n");
        fprintf(f, "\t\tsize_t i=0 ;\n");
        fprintf(f, "\t\t#define SA(name) Read(name,in[i]); i++;\n");
        fprintf(f, "\t\t_MACRO_SA_\n");
        fprintf(f, "\t\t#undef SA\n");
        fprintf(f, "\t}\n");

        fprintf(f, "\n");
        fprintf(f, "\tinline void\n");
        fprintf(f, "\tRead_All (\n");
        fprintf(f, "\t\tstd::vector <char const *> const &\n");
        fprintf(f, "\t\t\tin\n");
        fprintf(f, "\t) {\n");
        fprintf(f, "\t\tusing namespace Read_Show_Functions;\n");
        fprintf(f, "\t\tsize_t i=0 ;\n");
        fprintf(f, "\t\t#define SA(name) Read(name,in[i]); i++;\n");
        fprintf(f, "\t\t_MACRO_SA_\n");
        fprintf(f, "\t\t#undef SA\n");
        fprintf(f, "\t}\n");
    }

    inline void show_show(FILE *f = stdout) const {
        fprintf(f, "\n");
        fprintf(f, "\tinline void\n");
        fprintf(f, "\tShow_All (\n");
        fprintf(f, "\t\tFILE *\n");
        fprintf(f, "\t\t\tf = stdout\n");
        fprintf(f, "\t) const {\n");
        fprintf(f, "\t\tusing namespace Read_Show_Functions;\n");
        fprintf(f, "\t\t#define SA(name) Show(name,f); Show(f);\n");
        fprintf(f, "\t\t_MACRO_SA_\n");
        fprintf(f, "\t\t#undef SA\n");
        fprintf(f, "\t\tShow_Next(f);\n");
        fprintf(f, "\t}\n");
    }

    inline void show_macros(FILE *f = stdout) const {
        fprintf(f, "#define _MACRO_SA_ \\\n");
        for (size_t i = 0; i < sizes.size(); i++) {
            fprintf(f, "\tSA(L%ld_)", i);
            if (i != (sizes.size() - 1)) { fprintf(f, " \\"); }
            fprintf(f, "\n");
        }
        show_read(f);
        show_show(f);
        fprintf(f, "\n#undef _MACRO_SA_\n");
        fprintf(f, "\n");
    }

    inline void show_tail(FILE *f = stdout) const {
        fprintf(f, "};\n");
        fprintf(f, "\n");
        fprintf(f, "#undef _MACRO_CLASS_NAME_\n");
        fprintf(f, "\n");
    }

    inline void show(FILE *f = stdout) const {
        show_header(f);
        show_labels(f);
        show_data(f);
        show_macros(f);
        show_read_write_wrappers(f);
        show_tail(f);
    }
    //////////////////////////////////////
    // Header Generation Functions END. //
    //////////////////////////////////////

    ///////////////////////
    // Interfaces BEGIN: //
    ///////////////////////
  public:
    inline void Read_Labels(std::vector<std::string> const &in) {
        labels.resize(in.size());
        for (size_t i = 0; i < in.size(); i++) { labels[i] = in[i]; }
    }

    inline void Read_Labels(std::vector<char const *> const &in) {
        labels.resize(in.size());
        for (size_t i = 0; i < in.size(); i++) { labels[i] = in[i]; }
    }

    inline size_t operator()() const { return sizes.size(); }

    inline size_t operator[](size_t const in) const { return sizes[in]; }

    inline void operator()(std::vector<std::string> const &in) {
        ReadLine(in);
        AnalyzeStatus(in);
    }

    inline void operator()(std::vector<char const *> const &in) {
        ReadLine(in);
        AnalyzeStatus(in);
    }
    /////////////////////
    // Interfaces END. //
    /////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};
#undef _MACRO_CLASS_NAME_
/////////////////////////
// Main Analyzer END.} //
/////////////////////////

///////////////////////////////////////////////////
// Slave for multi threaded analysis new BEGIN:{ //
///////////////////////////////////////////////////
#define _MACRO_CLASS_NAME_ AnalyzeSlave_new
class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF     = _MACRO_CLASS_NAME_;
    using TYPE_WORD     = std::string;
    using TYPE_LINE     = std::vector<char const *>;
    using TYPE_ANALYZER = AnalyzeLines;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    TYPE_WORD const FILENAME;
    TYPE_ANALYZER   ANALYZER;
    size_t          COUNT;
    size_t const    INDEX;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ////////////////////////////////
    // Required interfaces BEGIN: //
    ////////////////////////////////
  public:
    static inline void SORT(TYPE_WORD const) {}

    static inline void MERGE(TYPE_WORD const in1, TYPE_WORD const in2,
                             TYPE_WORD const out) {

        TYPE_ANALYZER A1;
        TYPE_ANALYZER A2;
        A1 << in1;
        A2 << in2;
        A1(A2);
        A1 >> out;
    }

    inline void operator()(TYPE_LINE const &in) {
        bool const outcome = (COUNT == 0) && (INDEX == 0);
        if (outcome) {
            ANALYZER.Read_Labels(in);
        } else {
            ANALYZER(in);
        }
        COUNT++;
    }

    _MACRO_CLASS_NAME_(std::string const filename, size_t const index)
      : FILENAME(filename), INDEX(index) {
        COUNT = 0;
    }

    ~_MACRO_CLASS_NAME_() { ANALYZER >> FILENAME; }
    //////////////////////////////
    // Required interfaces END. //
    //////////////////////////////

    template <char seperator, char newline>
    static inline void PrepareFileSchema(std::string const infilename,
                                         std::string const outfilename,
                                         size_t const      n_splits  = 16,
                                         size_t const      n_threads = 4) {

        FastTXT2BIN_NEW<TYPE_SELF, seperator, newline>::Do_All(
          infilename, outfilename, n_splits, n_threads);

        TYPE_ANALYZER analyze;
        analyze << outfilename;
        std::string const headername = outfilename + ".hh";
        FILE *            f          = fopen(headername.c_str(), "w");
        analyze.show(f);
        fclose(f);
    }
};
#undef _MACRO_CLASS_NAME_
/////////////////////////////////////////////////
// Slave for multi threaded analysis new END.} //
/////////////////////////////////////////////////

#endif
