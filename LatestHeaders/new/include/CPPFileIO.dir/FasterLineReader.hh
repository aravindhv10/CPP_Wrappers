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

template <char seperator = '\t', char newline = '\n'> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF  = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_LINES = std::vector<std::string>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const filename;
    FileArray<char>   filereader;
    char const *      buffer;
    size_t            current_loc;
    size_t            memloc;
    size_t const      limit;
    TYPE_LINES        lines;
    size_t const      memsize;
    std::string       data;
    size_t            max_i;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void Alloc() {
        current_loc += memloc;
        memloc = 0;
        max_i  = mymin(limit - current_loc, memsize);
        buffer = &(filereader(current_loc, max_i));
    }

    inline void got_separator() {
        lines.push_back(data);
        data.clear();
    }

    inline void got_normal_char() { data.push_back(buffer[memloc]); }

    inline TYPE_LINES const &next() {

        lines.clear();

        if ((current_loc + memloc) >= limit) { return lines; }

        data.clear();

        /* The main reading loop: */ {
        start:
            while (memloc < max_i) {
                switch (buffer[memloc]) {
                    case seperator: got_separator(); break;
                    case newline: goto end_of_line;
                    default: got_normal_char(); break;
                }
                memloc++;
            }
            if ((current_loc + memloc) < limit) {
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
    inline TYPE_LINES const &operator()() { return next(); }
    /////////////////////////
    // Main Interface END. //
    /////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  private:
    inline void init() {
        lines.clear();
        Alloc();
    }

  public:
    _MACRO_CLASS_NAME_(std::string const _filename, size_t const _memsize = 25)
      : filename(_filename), filereader(filename), current_loc(0), memloc(0),
        limit(filereader.size()), memsize(shifter(_memsize)) {
        init();
    }

    _MACRO_CLASS_NAME_(size_t const _start, size_t const _end,
                       std::string const _filename, size_t const _memsize = 25)
      : filename(_filename), filereader(filename), current_loc(_start),
        memloc(0), limit(_end), memsize(shifter(_memsize)) {
        init();
    }

    ~_MACRO_CLASS_NAME_() {}
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ FileDivider

template <char endline> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF       = _MACRO_CLASS_NAME_<endline>;
    using TYPE_BOUNDARIES = std::vector<size_t>;
    using TYPE_READER     = FileArray<char const>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const FILENAME;
    TYPE_READER       READER;
    TYPE_BOUNDARIES   BOUNDARIES;
    size_t const      LIMIT;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void CRAWL(size_t const i) {
        if (i < (BOUNDARIES.size() - 1)) {
            size_t const start = BOUNDARIES[i];
            size_t const end   = BOUNDARIES[i + 1];
            for (size_t j = start; j < end; j++) {
                if (READER(j) == endline) {
                    BOUNDARIES[i] = j + 1;
                    return;
                }
            }
        }
    }

    inline TYPE_BOUNDARIES const &GET_BOUNDARIES(size_t const nums = 1) {
        BOUNDARIES.resize(nums + 1);
        for (size_t i = 0; i <= nums; i++) {
            BOUNDARIES[i] = (LIMIT * i) / nums;
        }
        for (size_t i = 1; i < nums; i++) { CRAWL(i); }
        return BOUNDARIES;
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    ///////////////////////
    // Interfaces BEGIN: //
    ///////////////////////
  public:
    inline TYPE_BOUNDARIES const &operator()(size_t const nums = 1) {
        return GET_BOUNDARIES(nums);
    }

    _MACRO_CLASS_NAME_(std::string const filename)
      : FILENAME(filename), READER(FILENAME), LIMIT(READER()) {}

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////
    // Interfaces END. //
    /////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ ThreadedLineReader

template <char seperator = '\t', char newline = '\n'> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF        = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_LINE_READER = FasterLineReader<seperator, newline>;
    using TYPE_LINES       = typename TYPE_LINE_READER::TYPE_LINES;
    using TYPE_DIVIDER     = FileDivider<newline>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const FILENAME;
    size_t const      NTH;
    TYPE_LINE_READER *LINE_READER;
    TYPE_DIVIDER      DIVIDER;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  private:
    inline void CONSTRUCT() {
        auto const &boundaries = DIVIDER(NTH);
        LINE_READER            = static_cast<TYPE_LINE_READER *>(
          malloc(NTH * sizeof(TYPE_LINE_READER)));
        for (size_t i = 0; i < NTH; i++) {
            new (&(LINE_READER[i]))
              TYPE_LINE_READER(boundaries[i], boundaries[i + 1], FILENAME);
        }
    }

    inline void DESTROY() {
        for (size_t i = 0; i < NTH; i++) { LINE_READER[i].~FasterLineReader(); }
        free(LINE_READER);
    }

  public:
    _MACRO_CLASS_NAME_(std::string const filename, size_t const nth)
      : FILENAME(filename), NTH(nth), DIVIDER(FILENAME) {
        CONSTRUCT();
    }

    ~_MACRO_CLASS_NAME_() { DESTROY(); }
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////

    ////////////////////////////
    // Main Interfaces BEGIN: //
    ////////////////////////////
  public:
    inline size_t            operator()() const { return NTH; }
    inline TYPE_LINES const &operator()(size_t const i) {
        return LINE_READER[i]();
    }
    //////////////////////////
    // Main Interfaces END. //
    //////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
