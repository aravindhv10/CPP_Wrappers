#ifndef _HEADER_GUARD_CPPFileIO_LineReaderSequentialThreaded_
#define _HEADER_GUARD_CPPFileIO_LineReaderSequentialThreaded_

#include "./FileArray.hh"

#define _MACRO_CLASS_NAME_ LineReaderSequentialThreaded
template <char seperator, char newline> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_SELF       = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_BUFFER     = char const;
    using TYPE_MAINREADER = FileArray<TYPE_BUFFER>;
    using TYPE_LINE       = std::vector<char>;

  private:
    std::string const FILENAME;
    TYPE_MAINREADER   MAINREADER;
    size_t            FILE_LOC;
    size_t const      FILE_END;
    size_t            MEM_LOC;
    size_t const      MEM_SIZE;
    TYPE_BUFFER *     BUFFER;

  private:
    inline size_t LENGTH() const {
        return mymin(FILE_END - FILE_LOC, MEM_SIZE);
    }

    inline void ALLOC() {
        FILE_LOC = FILE_LOC + MEM_LOC;
        MEM_LOC  = 0;
        BUFFER   = &(MAINREADER(FILE_LOC, LENGTH()));
    }


  public:
    _MACRO_CLASS_NAME_(std::string const filename, size_t const mem_size = 28)
      : FILENAME(filename), MAINREADER(FILENAME), FILE_LOC(0),
        FILE_END(MAINREADER()), MEM_LOC(0), MEM_SIZE(shifter(mem_size)) {
        ALLOC();
    }

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
