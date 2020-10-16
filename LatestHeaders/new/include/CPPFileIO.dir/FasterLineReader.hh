#ifndef _HEADER_GUARD_CPPFileIO_FasterLineReader_
#define _HEADER_GUARD_CPPFileIO_FasterLineReader_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./D1.hh"
#include "./FileArray.hh"
#include "./FileWriter.hh"
#include "./Sorter.hh"
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

#define _MACRO_CLASS_NAME_ BufferLineReader

template <char seperator, char newline> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF   = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_BUFFER = char const;
    using TYPE_WORDS  = std::vector<std::string>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    size_t       MEM_LOC;
    size_t const MEM_SIZE;
    TYPE_BUFFER *BUFFER;
    TYPE_WORDS   WORDS;
    std::string  WORD;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline TYPE_BUFFER &POP() {
        TYPE_BUFFER &ret = BUFFER[MEM_LOC];
        MEM_LOC          = MEM_LOC + 1;
        return ret;
    }

    inline void APPEND_WORD() {
        WORDS.push_back(WORD);
        WORD.clear();
    }

    inline TYPE_WORDS const &GET_WORDS() {
        WORD.clear();
        WORDS.clear();
        while (MEM_LOC < MEM_SIZE) {
            TYPE_BUFFER tmp = POP();
            switch (tmp) {
                case seperator: APPEND_WORD(); break;
                case newline: APPEND_WORD(); return WORDS;
                default: WORD.push_back(tmp); break;
            }
        }
        if (WORD.size() > 0) { APPEND_WORD(); }
        return WORDS;
    }

  public:
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    inline TYPE_WORDS const &operator()() { return GET_WORDS(); }

    _MACRO_CLASS_NAME_(TYPE_BUFFER *buffer, size_t const mem_size)
      : BUFFER(buffer), MEM_SIZE(mem_size), MEM_LOC(0) {}

    _MACRO_CLASS_NAME_(Dynamic1DArray<TYPE_BUFFER> &in)
      : BUFFER(in.GET_DATA()), MEM_SIZE(in()), MEM_LOC(0) {}

    ~_MACRO_CLASS_NAME_() {}
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
    using TYPE_BUFFER     = Dynamic1DArray<char>;
    using TYPE_LOCKER     = std::mutex;
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
    TYPE_LOCKER       LOCKER;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void LOCK() { LOCKER.lock(); }
    inline void UNLOCK() { LOCKER.unlock(); }

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

    inline void GET_BUFFER(size_t const val, std::vector<char> &in) {
        LOCK();
        if (BOUNDARIES.size() > val + 1) {
            size_t const start  = BOUNDARIES[val];
            size_t const length = BOUNDARIES[val + 1] - BOUNDARIES[val];
            in.resize(length);
            char const *buf = &(READER(start, length));
            memcpy(&(in[0]), buf, length);
        }
        UNLOCK();
    }

    inline void GET_BUFFER(size_t const val, TYPE_BUFFER &in) {
        LOCK();
        if (BOUNDARIES.size() > val + 1) {
            size_t const start  = BOUNDARIES[val];
            size_t const length = BOUNDARIES[val + 1] - BOUNDARIES[val];
            char const * buf    = &(READER(start, length));

            in.~Dynamic1DArray();
            new (&in) TYPE_BUFFER(length);

            memcpy(in.GET_DATA(), buf, length);
        }
        UNLOCK();
    }

    inline void operator()(size_t const val, TYPE_BUFFER &in) {
        return GET_BUFFER(val, in);
    }

    _MACRO_CLASS_NAME_(std::string const filename)
      : FILENAME(filename), READER(FILENAME), LIMIT(READER()) {}

    ~_MACRO_CLASS_NAME_() {}
    /////////////////////
    // Interfaces END. //
    /////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ ThreadedTxt2Bin

template <typename T, char seperator, char newline> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_DATA       = T;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_DATA, seperator, newline>;
    using TYPE_DIVIDER    = FileDivider<newline>;
    using TYPE_BOUNDARIES = typename TYPE_DIVIDER::TYPE_BOUNDARIES;
    using TYPE_BUFFER     = typename TYPE_DIVIDER::TYPE_BUFFER;
    using TYPE_READER     = BufferLineReader<seperator, newline>;
    using TYPE_BUFFER_LINE_READER = BufferLineReader<'\t', '\n'>;
    using TYPE_BIN_WRITER         = FileWriter<TYPE_DATA>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    size_t const      NTH;
    std::string const FILENAME;
    std::string const OUTPUTNAME;
    TYPE_DIVIDER      DIVIDER;
    TYPE_BOUNDARIES   BOUNDARIES;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    //////////////////////
    // Filenames BEGIN: //
    //////////////////////
  private:
    inline std::string const DIRNAME() const { return OUTPUTNAME + ".dir"; }

    inline std::string const GET_OUT_FILENAME(size_t const i) {
        mkdir(DIRNAME().c_str(), 0755);
        std::string ret = DIRNAME() + "/out." + std::to_string(i) + ".bin";
        return ret;
    }
    ////////////////////
    // Filenames END. //
    ////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void TRANSLATE(TYPE_BUFFER_LINE_READER &reader,
                          TYPE_BIN_WRITER &        writer) {
        TYPE_DATA tmpbuf;
    MainLoop:
        /* The main working loop: */ {
            auto const &lines = reader();
            if (lines.size() > 0) {
                tmpbuf(lines);
                writer(tmpbuf);
                goto MainLoop;
            }
        }
    }

  public:
    inline std::string const work(size_t const index,
                                  bool const   process_header = true,
                                  int const    bufsize        = 20) {
        TYPE_BUFFER buffer(0);
        DIVIDER(index, buffer);
        TYPE_BUFFER_LINE_READER reader(buffer.GET_DATA(), buffer());
        TYPE_BIN_WRITER         writer(GET_OUT_FILENAME(index), bufsize);
        if ((index == 0) && (process_header)) { reader(); }
        TRANSLATE(reader, writer);
        return GET_OUT_FILENAME(index);
    }

    inline void sort(size_t const index) {
        SortFile<TYPE_DATA>(GET_OUT_FILENAME(index));
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    _MACRO_CLASS_NAME_(size_t const nth, std::string const filename,
                       std::string const outputname)
      : NTH(nth), FILENAME(filename), OUTPUTNAME(outputname),
        DIVIDER(FILENAME) {
        BOUNDARIES = DIVIDER(NTH);
        for (size_t i = 0; i < BOUNDARIES.size(); i++) {
            printf("%zu\n", BOUNDARIES[i]);
        }
    }

    ~_MACRO_CLASS_NAME_() {}
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
    size_t const      MEMSIZE;
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
            new (&(LINE_READER[i])) TYPE_LINE_READER(
              boundaries[i], boundaries[i + 1], FILENAME, MEMSIZE);
        }
    }

    inline void DESTROY() {
        for (size_t i = 0; i < NTH; i++) { LINE_READER[i].~FasterLineReader(); }
        free(LINE_READER);
    }

  public:
    _MACRO_CLASS_NAME_(std::string const filename, size_t const memsize = 30,
                       size_t const nth = 4)
      : FILENAME(filename), NTH(nth), DIVIDER(FILENAME), MEMSIZE(memsize) {
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
    inline size_t operator()() const { return NTH; }

    inline TYPE_LINES const &operator()(size_t const i) {
        return LINE_READER[i]();
    }
    //////////////////////////
    // Main Interfaces END. //
    //////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
