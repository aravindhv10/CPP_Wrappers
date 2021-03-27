#ifndef _HEADER_GUARD_StaticArray_RadixSort_
#define _HEADER_GUARD_StaticArray_RadixSort_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../CPPFileIO.hh"
#include "./D1.hh"
///////////////////
// Headers END.} //
///////////////////

////////////////////////////
// Byte extractor BEGIN:{ //
////////////////////////////
#define _MACRO_CLASS_NAME_ ByteExtract
template <typename T, typename TI = CPPFileIO::TYPE_U32>
class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_CHILD = T;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_CHILD, TYPE_INT>;
    using TYPE_BYTE  = CPPFileIO::TYPE_BYTE;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ////////////////////////////////
    // Interpret as child BEGIN:{ //
    ////////////////////////////////
  public:
    inline TYPE_CHILD &AS_CHILD() { return static_cast<TYPE_CHILD &>(this[0]); }

    inline TYPE_CHILD const &AS_CHILD() const {
        return static_cast<TYPE_CHILD const &>(this[0]);
    }
    //////////////////////////////
    // Interpret as child END.} //
    //////////////////////////////

    /////////////////////////////////////
    // Main index to interpret BEGIN:{ //
    /////////////////////////////////////
  public:
    TYPE_INT INDEX;
    ///////////////////////////////////
    // Main index to interpret END.} //
    ///////////////////////////////////

    ///////////////////////////////////
    // Function to set index BEGIN:{ //
    ///////////////////////////////////
  public:
    inline bool operator()(TYPE_INT const i) {
        if (i < AS_CHILD()()) {
            INDEX = i;
            return true;
        } else {
            return false;
        }
    }
    /////////////////////////////////
    // Function to set index END.} //
    /////////////////////////////////

    //////////////////////////////////////////
    // Functions forwarded to child BEGIN:{ //
    //////////////////////////////////////////
  public:
    inline size_t operator()() {
        // Forward to child:
        // Return the number of indices in the element.
        return AS_CHILD()();
    }

    template <typename TYPE_ELEMENT>
    inline TYPE_BYTE operator[](TYPE_ELEMENT const &in) {
        return AS_CHILD()[in];
    }
    ////////////////////////////////////////
    // Functions forwarded to child END.} //
    ////////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_
//////////////////////////
// Byte extractor END.} //
//////////////////////////

/////////////////////////////
// Main Radix Sort BEGIN:{ //
/////////////////////////////
#define _MACRO_CLASS_NAME_ RadixSorter
template <typename T_BUF, typename T_BE, typename TI = CPPFileIO::TYPE_I64>
class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_BUFFER       = T_BUF;
    using TYPE_BYTE_EXTRACT = T_BE;
    using TYPE_INT          = TI;
    using TYPE_BYTE         = CPPFileIO::TYPE_BYTE;
    using TYPE_COUNTS       = CPPFileIO::Dynamic1DArray<TYPE_INT, TYPE_INT>;
    using TYPE_BOUNDARIES = CPPFileIO::Dynamic1DArray<TYPE_INT const, TYPE_INT>;

    using TYPE_SELF =
      _MACRO_CLASS_NAME_<TYPE_BUFFER, TYPE_BYTE_EXTRACT, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_BUFFER &           BUFFER1; // Input
    TYPE_BUFFER &           BUFFER2; // Output
    TYPE_INT const          START;
    TYPE_INT const          END;
    ND_ARRAY<256, TYPE_INT> COUNTS;
    ND_ARRAY<257, TYPE_INT> BOUNDARIES;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////////////
    // MAin Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline void ZERO_COUNTS() {
        COUNTS = 0;
        // for (TYPE_INT i = 0; i < 256; i++) { COUNTS[i] = 0; }
    }

    inline void EVALUATE_PREFIX_SUM(TYPE_BYTE_EXTRACT &in) {
        for (TYPE_INT i = START; i <= END; i++) {
            auto const &    element = BUFFER1(i);
            TYPE_BYTE const val     = in[element];
            COUNTS[val]++;
        }
    }

    inline void FIND_BOUNDARIES() {
        BOUNDARIES[0] = 0;
        for (TYPE_INT i = 1; i < BOUNDARIES.SIZE(); i++) {
            BOUNDARIES[i] = BOUNDARIES[i - 1] + COUNTS[i - 1];
        }
        for (TYPE_INT i = 0; i < COUNTS.SIZE(); i++) {
            COUNTS[i] = BOUNDARIES[i];
        }
    }

    inline void PERFORM_COPY(TYPE_BYTE_EXTRACT &in) {
        for (TYPE_INT i = START; i <= END; i++) {
            auto const &    element = BUFFER1(i);
            TYPE_BYTE const val     = in[element];
            BUFFER2(COUNTS[val])    = element;
            COUNTS[val]++;
        }
    }

    inline void DO_SCAN(TYPE_BYTE_EXTRACT in) {
        ZERO_COUNTS();
        EVALUATE_PREFIX_SUM(in);
        FIND_BOUNDARIES();
        PERFORM_COPY(in);
    }

  public:
    inline TYPE_BOUNDARIES const get_boundaries() const {
        TYPE_BOUNDARIES ret(BOUNDARIES.GET_DATA(), 257);
        return ret;
    }

    inline TYPE_BOUNDARIES operator()(TYPE_BYTE_EXTRACT &in) {
        DO_SCAN(in);
        return get_boundaries();
    }
    //////////////////////////////////
    // MAin Working Functions END.} //
    //////////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(TYPE_BUFFER &buffer1, TYPE_BUFFER &buffer2,
                       TYPE_INT const start, TYPE_INT const end)
      : BUFFER1(buffer1), BUFFER2(buffer2), START(start), END(end) {}

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};
#undef _MACRO_CLASS_NAME_
///////////////////////////
// Main Radix Sort END.} //
///////////////////////////

#endif
