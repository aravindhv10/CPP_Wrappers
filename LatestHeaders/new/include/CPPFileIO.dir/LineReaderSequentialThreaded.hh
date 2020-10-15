#ifndef _HEADER_GUARD_CPPFileIO_LineReaderSequentialThreaded_
#define _HEADER_GUARD_CPPFileIO_LineReaderSequentialThreaded_

#include "./FileArray.hh"

#define _MACRO_CLASS_NAME_ LineReaderSequentialThreaded
template <char seperator, char newline> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SELF       = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_BUFFER     = char const;
    using TYPE_MAINREADER = FileArray<TYPE_BUFFER>;
    using TYPE_LINE       = std::vector<char>;
    using TYPE_WORDS      = std::vector<std::string>;
    using TYPE_LOCK       = std::mutex;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const FILENAME;
    TYPE_MAINREADER   MAINREADER;
    size_t            FILE_LOC;
    size_t const      FILE_SIZE;
    size_t            MEM_LOC;
    size_t const      MEM_SIZE;
    TYPE_BUFFER *     BUFFER;
    TYPE_LOCK         LOCKER;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
  private:
    inline void LOCK() { LOCKER.lock(); }
    inline void UNLOCK() { LOCKER.unlock(); }

    inline size_t LENGTH() const {
        return mymin(FILE_SIZE - FILE_LOC, MEM_SIZE);
    }
    inline size_t CURRENT_LOC() const { return MEM_LOC + FILE_LOC; }

    inline bool FILE_OVER() { return CURRENT_LOC() >= FILE_SIZE; }

    inline void ALLOC() {
        FILE_LOC = FILE_LOC + MEM_LOC;
        MEM_LOC  = 0;
        BUFFER   = &(MAINREADER(FILE_LOC, LENGTH()));
    }

    inline char const &POP() {
        char const &ret = BUFFER[MEM_LOC];
        MEM_LOC         = MEM_LOC + 1;
        return ret;
    }

  public:
    inline void GET_LINE(TYPE_LINE &in) {
        in.clear();
        LOCK();
    FuncStart:
        if (FILE_OVER()) {
            UNLOCK();
            return;
        }
        while (MEM_LOC < MEM_SIZE) {
            char const &tmp = POP();
            if (tmp == newline) {
                UNLOCK();
                return;
            } else {
                in.push_back(tmp);
            }
        }
        ALLOC();
        goto FuncStart;
    }

    inline void GET_WORDS(TYPE_LINE &line, TYPE_WORDS &words) {
        words.clear();
        GET_LINE(line);
        std::string word;
        word.reserve(line.size());
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == seperator) {
                words.push_back(word);
                word.clear();
            } else {
                word.push_back(line[i]);
            }
        }
        if (word.size() > 0) { words.push_back(word); }
    }

    inline void GET_WORDS(TYPE_WORDS &words) {
        TYPE_LINE line;
        GET_WORDS(line, words);
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    _MACRO_CLASS_NAME_(std::string const filename, size_t const mem_size = 28)
      : FILENAME(filename), MAINREADER(FILENAME), FILE_LOC(0),
        FILE_SIZE(MAINREADER()), MEM_LOC(0), MEM_SIZE(shifter(mem_size)) {
        ALLOC();
    }

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
