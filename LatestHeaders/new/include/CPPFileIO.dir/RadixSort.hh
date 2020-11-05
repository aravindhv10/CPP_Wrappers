#ifndef _HEADER_GUARD_CPPFileIO_RadixSort_
#define _HEADER_GUARD_CPPFileIO_RadixSort_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "./D1.hh"
#include "./FileArray.hh"
///////////////////
// Headers END.} //
///////////////////

#define _MACRO_CLASS_NAME_ RadixSorter

template <typename T, typename TI = long> class _MACRO_CLASS_NAME_ {

    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_INT        = TI;
    using TYPE_BUFFER     = T;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_BUFFER, TYPE_INT>;
    using TYPE_BYTE       = unsigned char;
    using TYPE_COUNTS     = Dynamic1DArray<TYPE_INT, TYPE_INT>;
    using TYPE_BOUNDARIES = Dynamic1DArray<TYPE_INT const, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_BUFFER const &BUFFER1;
    TYPE_BUFFER &      BUFFER2;
    TYPE_INT const     START;
    TYPE_INT const     END;
    TYPE_INT           COUNTS[256];
    TYPE_INT           BOUNDARIES[257];
    TYPE_INT const     DIGIT;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // MAin Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline void ZERO_COUNTS() {
        for (TYPE_INT i = 0; i < 256; i++) { COUNTS[i] = 0; }
    }

    inline void EVALUATE_PREFIX_SUM() {
        for (TYPE_INT i = START; i <= END; i++) {
            auto const &    element = BUFFER1(i);
            TYPE_BYTE const val     = element[DIGIT];
            COUNTS[val]++;
        }
    }

    inline void FIND_BOUNDARIES() {
        BOUNDARIES[0] = 0;
        for (TYPE_INT i = 1; i < 257; i++) {
            BOUNDARIES[i] = BOUNDARIES[i - 1] + COUNTS[i - 1];
        }
        for (TYPE_INT i = 0; i < 256; i++) { COUNTS[i] = BOUNDARIES[i]; }
    }

    inline void PERFORM_COPY() {
        for (TYPE_INT i = START; i <= END; i++) {
            auto const &    element = BUFFER1(i);
            TYPE_BYTE const val     = element[DIGIT];
            BUFFER2(COUNTS[val])    = element;
            COUNTS[val]++;
        }
    }

    inline void DO_SCAN() {
        ZERO_COUNTS();
        EVALUATE_PREFIX_SUM();
        FIND_BOUNDARIES();
        PERFORM_COPY();
    }

  public:
    inline TYPE_BOUNDARIES const get_boundaries() const {
        TYPE_BOUNDARIES ret(BOUNDARIES, 257);
        return ret;
    }
    //////////////////////////////////
    // MAin Working Functions END.} //
    //////////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_BUFFER const &buffer1, TYPE_BUFFER &buffer2,
                       TYPE_INT const start, TYPE_INT const end, TYPE_INT const digit)
      : BUFFER1(buffer1), BUFFER2(buffer2), START(start), END(end), DIGIT(digit) {DO_SCAN();}

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
