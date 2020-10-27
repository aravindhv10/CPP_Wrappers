#ifndef _HEADER_GUARD_CPPFileIO_FileWriter_
#define _HEADER_GUARD_CPPFileIO_FileWriter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./D1.hh"
#include "./FileArray.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ FileWriter

template <typename TD, typename TI = long> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_INT      = TI;
    using TYPE_ELEMENT  = TD;
    using TYPE_ELEMENTS = std::vector<TYPE_ELEMENT>;
    using TYPE_SELF     = _MACRO_CLASS_NAME_<TYPE_ELEMENT, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    //////////////////
    // Data BEGIN:{ //
    //////////////////
  private:
    FileArray<TYPE_ELEMENT> infile;
    TYPE_INT                count;
    TYPE_INT const          bufsize;
    TYPE_INT const          mask;
    TYPE_ELEMENT *          buf;
    ////////////////
    // Data END.} //
    ////////////////

    ////////////////////////////////////
    // Main Working Functions BEGIN:{ //
    ////////////////////////////////////
  private:
    inline TYPE_INT size() const { return count; }

    inline TYPE_INT push_back(TYPE_ELEMENT const &indata) {
        TYPE_INT const mod = count & mask;
        if (mod == 0) { buf = &(infile(count, bufsize)); }
        buf[mod] = indata;
        count++;
        return size();
    }

    inline TYPE_INT push_back(TYPE_ELEMENTS const &indata) {
        for (TYPE_INT i = 0; i < indata.size(); i++) { push_back(indata[i]); }
        return size();
    }

    template <typename TPI>
    inline TYPE_INT push_back(Dynamic1DArray<TYPE_ELEMENT, TPI> const &in) {
        for (TYPE_INT i = 0; i < in(); i++) { push_back(in(i)); }
        return size();
    }
    //////////////////////////////////
    // Main Working Functions END.} //
    //////////////////////////////////

    ////////////////////////////////
    // Exposed Interfaces BEGIN:{ //
    ////////////////////////////////
  public:
    inline TYPE_INT operator()() const { return size(); }

    inline TYPE_INT operator()(TYPE_ELEMENT const &indata) {
        return push_back(indata);
    }

    inline TYPE_INT operator()(TYPE_ELEMENTS const &indata) {
        return push_back(indata);
    }

    template <typename TPI>
    inline TYPE_INT operator()(Dynamic1DArray<TYPE_ELEMENT, TPI> const &in) {
        return push_back(in);
    }
    //////////////////////////////
    // Exposed Interfaces END.} //
    //////////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(std::string const name, TYPE_INT const _bufsize = 20)
      : infile(name), bufsize(shifter(_bufsize)), mask(bufsize - 1) {
        infile.writeable();
        count = 0;
        infile.size(count);
    }

    ~_MACRO_CLASS_NAME_() { infile.size(count); }
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ FileWriterThreaded

template <typename TD, typename TI = long> class _MACRO_CLASS_NAME_ {
    /////////////////////////
    // Definitions BEGIN:{ //
    /////////////////////////
  public:
    using TYPE_INT      = TI;
    using TYPE_ELEMENT  = TD;
    using TYPE_ELEMENTS = std::vector<TYPE_ELEMENT>;
    using TYPE_MAIN     = FileWriter<TYPE_ELEMENT>;
    using TYPE_SELF     = _MACRO_CLASS_NAME_<TYPE_ELEMENT, TYPE_INT>;
    ///////////////////////
    // Definitions END.} //
    ///////////////////////

    ///////////////////////////
    // Data Elements BEGIN:{ //
    ///////////////////////////
  private:
    TYPE_MAIN  mainwriter;
    std::mutex locker;
    /////////////////////////
    // Data Elements END.} //
    /////////////////////////

    ////////////////////////////
    // Main interface BEGIN:{ //
    ////////////////////////////
  public:
    inline TYPE_INT operator()() const { return mainwriter(); }

    inline TYPE_INT operator()(TYPE_ELEMENT const &indata) {
        locker.lock();
        TYPE_INT ret = mainwriter(indata);
        locker.unlock();
        return ret;
    }

    inline TYPE_INT operator()(TYPE_ELEMENTS const &indata) {
        locker.lock();
        TYPE_INT ret = mainwriter(indata);
        locker.unlock();
        return ret;
    }

    template <typename TPI>
    inline TYPE_INT operator()(Dynamic1DArray<TYPE_ELEMENT, TPI> const &in) {
        locker.lock();
        TYPE_INT ret = mainwriter(in);
        locker.unlock();
        return ret;
    }
    //////////////////////////
    // Main interface END.} //
    //////////////////////////

    //////////////////////////////////////
    // Constructor & Destructor BEGIN:{ //
    //////////////////////////////////////
  public:
    _MACRO_CLASS_NAME_(std::string const filename, TYPE_INT const bufsize = 20)
      : mainwriter(filename, bufsize) {}

    ~_MACRO_CLASS_NAME_() {}
    ////////////////////////////////////
    // Constructor & Destructor END.} //
    ////////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
