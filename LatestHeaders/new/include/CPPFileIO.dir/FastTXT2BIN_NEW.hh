#ifndef _HEADER_GUARD_CPPFileIO_FastTXT2BIN_NEW_
#define _HEADER_GUARD_CPPFileIO_FastTXT2BIN_NEW_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./FasterLineReader.hh"
//////////////////
// Headers END. //
//////////////////

/////////////////////////////////
// BufferLineReaderNew BEGIN:{ //
/////////////////////////////////
#define _MACRO_CLASS_NAME_ BufferLineReaderNew
template <char seperator, char newline> class _MACRO_CLASS_NAME_ {

    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_SELF   = _MACRO_CLASS_NAME_<seperator, newline>;
    using TYPE_BUFFER = char;
    using TYPE_WORDS  = std::vector<TYPE_BUFFER const *>;

    inline size_t constexpr NUM_COMPARE_ELEMENTS () {return 2;}
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    size_t       MEM_LOC;
    size_t const MEM_SIZE;
    TYPE_BUFFER *BUFFER;
    TYPE_WORDS   WORDS;
    size_t const VECTORIZED_LIMIT;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline TYPE_WORDS const &GET_WORDS() {
        WORDS.clear();
        bool allocate = true;

        while (MEM_LOC < MEM_SIZE) {
            if (allocate) {
                WORDS.push_back(&(BUFFER[MEM_LOC]));
                allocate = false;
            }

            switch (BUFFER[MEM_LOC]) {
                case seperator:
                    BUFFER[MEM_LOC] = 0;
                    MEM_LOC++;
                    allocate = true;
                    break;

                case newline:
                    BUFFER[MEM_LOC] = 0;
                    MEM_LOC++;
                    return WORDS;

                default: MEM_LOC++; break;
            }
        }
        BUFFER[MEM_SIZE - 1] = 0;
        return WORDS;
    }

    inline TYPE_WORDS const &GET_WORDS_NEW() {
        size_t constexpr LOC_SEP[4] = {1, 2, 4, 8};
        size_t constexpr LOC_NWL[4] = {16, 32, 64, 128};

        WORDS.clear();
        if (MEM_LOC >= MEM_SIZE) { return WORDS; }

        WORDS.push_back(BUFFER[MEM_LOC]);

        size_t const status = (LOC_SEP[0] * (WORDS[MEM_LOC] == seperator)) +
                              (LOC_SEP[1] * (WORDS[MEM_LOC + 1] == seperator)) +
                              (LOC_NWL[0] * (WORDS[MEM_LOC] == newline)) +
                              (LOC_NWL[1] * (WORDS[MEM_LOC + 1] == newline));
        switch(status){
          case LOC_NWL[0]:
            BUFFER[MEM_LOC] = 0;

        }
        return WORDS;
    }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    /////////////////////////////////
    // Convinent interface BEGIN{: //
    /////////////////////////////////
  public:
    inline TYPE_WORDS const &operator()() { return GET_WORDS(); }
    ///////////////////////////////
    // Convinent interface END}. //
    ///////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
    _MACRO_CLASS_NAME_(TYPE_BUFFER *buffer, size_t const mem_size)
      : BUFFER(buffer), MEM_SIZE(mem_size), MEM_LOC(0), VECTORIZED_LIMIT(MEM_SIZE-NUM_COMPARE_ELEMENTS()) {}

    _MACRO_CLASS_NAME_(Dynamic1DArray<TYPE_BUFFER> &in)
      : BUFFER(in.GET_DATA()), MEM_SIZE(in()), MEM_LOC(0), VECTORIZED_LIMIT(MEM_SIZE-NUM_COMPARE_ELEMENTS()) {}

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_
///////////////////////////////
// BufferLineReaderNew END.} //
///////////////////////////////

////////////////////////
// Main Class BEGIN:{ //
////////////////////////
#define _MACRO_CLASS_NAME_ FastTXT2BIN_NEW
template <typename T, char seperator, char newline> class _MACRO_CLASS_NAME_ {
    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  public:
    using TYPE_SLAVE      = T;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_SLAVE, seperator, newline>;
    using TYPE_DIVIDER    = FileDivider<newline>;
    using TYPE_BOUNDARIES = typename TYPE_DIVIDER::TYPE_BOUNDARIES;
    using TYPE_BUFFER     = typename TYPE_DIVIDER::TYPE_BUFFER;
    using TYPE_READER     = BufferLineReaderNew<seperator, newline>;
    using TYPE_BIN_WRITER = FileWriter<TYPE_SLAVE>;
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
    static inline void RM(std::string const filename) {
        ExternalStarter<true>::GET("/bin/rm")("-vf")("--")(filename);
    }

    static inline void RMDIR(std::string const filename) {
        ExternalStarter<true>::GET("/bin/rmdir")("-v")("--")(filename);
    }

    static inline void MV(std::string const filename,
                          std::string const outfilename) {
        ExternalStarter<true>::GET("/bin/mv")("-vf")("--")(filename)(
          outfilename);
    }

    inline std::string const DIRNAME() const {
        std::string const ret = OUTPUTNAME + ".dir/";
        mkdir(ret.c_str(), 0755);
        return ret;
    }

    inline std::string const GET_OUT_FILENAME(size_t const i) {
        std::string const ret = DIRNAME() + "out." + std::to_string(i) + ".bin";
        return ret;
    }

    inline std::string const GET_OUT_FILENAME(size_t const i, size_t const j) {
        if (i == j) {
            return GET_OUT_FILENAME(i);
        } else {
            std::string const ret = DIRNAME() + "out." + std::to_string(i) +
                                    "-" + std::to_string(j) + ".bin";

            return ret;
        }
    }
    ////////////////////
    // Filenames END. //
    ////////////////////

    ////////////////////
    // Merging BEGIN: //
    ////////////////////
  private:
    inline std::string const MERGE_FILE(size_t const i1, size_t const i2,
                                        size_t const nth) {

        size_t const status = (1 * (i1 == i2)) + (2 * ((i1 + 1) == i2)) +
                              (4 * ((i1 + 1) < i2)) + (8 * (i2 < i1));

        switch (status) {

            case 1:
                /* Nothing to be done: */ { return GET_OUT_FILENAME(i1); }

            case 2:
                /* Merge only 2 files: */ {
                    std::string const name_i1  = GET_OUT_FILENAME(i1);
                    std::string const name_i2  = GET_OUT_FILENAME(i2);
                    std::string const name_out = GET_OUT_FILENAME(i1, i2);
                    TYPE_SLAVE::MERGE(name_i1, name_i2, name_out);
                    RM(name_i1);
                    RM(name_i2);
                    return name_out;
                }

            case 4:
                /* Merge many files: */ {
                    size_t const mid     = (i1 + i2) / 2;
                    size_t const new_nth = nth >> 1;

                    if (nth != 0) {
                        ForkMe forker;
                        if (forker.InKid()) {
                            MERGE_FILE(i1, mid, new_nth);
                        } else {
                            MERGE_FILE(mid + 1, i2, new_nth);
                        }
                    } else {
                        MERGE_FILE(i1, mid, 0);
                        MERGE_FILE(mid + 1, i2, 0);
                    }

                    std::string const name_i1  = GET_OUT_FILENAME(i1, mid);
                    std::string const name_i2  = GET_OUT_FILENAME(mid + 1, i2);
                    std::string const name_out = GET_OUT_FILENAME(i1, i2);

                    TYPE_SLAVE::MERGE(name_i1, name_i2, name_out);
                    RM(name_i1);
                    RM(name_i2);

                    return name_out;
                }

            case 8:
                /* Wrong Invocation: */ { return MERGE_FILE(i2, i1, nth); }
        }

        return GET_OUT_FILENAME(i2, i1);
    }
    //////////////////
    // Merging END. //
    //////////////////

    ////////////////////
    // Sorting BEGIN: //
    ////////////////////
  private:
    inline void SORT(size_t const index) {
        std::string const name = GET_OUT_FILENAME(index);
        TYPE_SLAVE::SORT(name);
    }

  public:
    inline void sort_all(size_t const nth) {

#pragma omp parallel for
        for (size_t i = 0; i < nth; i++) {
            for (size_t j = i; j < NTH; j += nth) { SORT(j); }
        }

        std::string const outfilename = MERGE_FILE(0, NTH - 1, nth);
        MV(outfilename, OUTPUTNAME);
        RMDIR(DIRNAME());
    }
    //////////////////
    // Sorting END. //
    //////////////////

    /////////////////////////
    // Main TXT2BIN BEGIN: //
    /////////////////////////
  public:
    inline std::string const work(size_t const index) {

        std::string const OUT_FILENAME = GET_OUT_FILENAME(index);

        TYPE_BUFFER buffer(0);
        DIVIDER(index, buffer);
        TYPE_READER reader(buffer.GET_DATA(), buffer());

        TYPE_SLAVE tmpbuf(OUT_FILENAME, index);
    MainLoop:
        /* The main working loop: */ {
            auto const &lines = reader();
            if (lines.size() > 0) {
                tmpbuf(lines);
                goto MainLoop;
            }
        }

        return GET_OUT_FILENAME(index);
    }
    ///////////////////////
    // Main TXT2BIN END. //
    ///////////////////////

  public:
    inline void do_all(size_t const nth = 8) {
#pragma omp parallel for
        for (size_t i = 0; i < nth; i++) {
            for (size_t j = i; j < NTH; j += nth) { work(j); }
        }

        sort_all(nth);
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

  public:
    _MACRO_CLASS_NAME_(size_t const nth, std::string const filename,
                       std::string const outputname)
      : NTH(nth), FILENAME(filename), OUTPUTNAME(outputname),
        DIVIDER(FILENAME) {
        BOUNDARIES = DIVIDER(NTH);
    }

    ~_MACRO_CLASS_NAME_() {}

    static inline void Do_All(std::string const inputname,
                              std::string const outputname,
                              size_t const n_splits, size_t const n_threads) {

        TYPE_SELF slave(n_splits, inputname, outputname);
        slave.do_all(n_threads);
    }
};
#undef _MACRO_CLASS_NAME_
//////////////////////
// Main Class END.} //
//////////////////////

#endif
