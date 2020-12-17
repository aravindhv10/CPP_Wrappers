#ifndef _HEADER_GUARD_CPPFileIO_BinarySearch_
#define _HEADER_GUARD_CPPFileIO_BinarySearch_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
//////////////////
// Headers END. //
//////////////////

//////////////////////////////////
// Main searching class BEGIN:{ //
//////////////////////////////////
#define _MACRO_CLASS_NAME_ BinarySearch
template <typename rd = char> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // DEFINITIONS BEGIN: //
    ////////////////////////
  public:
    using TYPE_READER = rd;
    using TYPE_SELF   = _MACRO_CLASS_NAME_<rd>;

    using TYPE_RETURN = struct {
        size_t begin;
        size_t end;
        char   status;
    }; //
       //////////////////////
       // DEFINITIONS END. //
       //////////////////////

    /////////////////
    // DATA BEGIN: //
    /////////////////
  private:
    TYPE_READER &reader;
    size_t const limit;
    ///////////////
    // DATA END. //
    ///////////////

    //////////////////////////
    // RETURN STATUS BEGIN: //
    //////////////////////////
  private:
    inline TYPE_RETURN FAILED() const {
        TYPE_RETURN ret;
        ret.begin  = 0;
        ret.end    = limit - 1;
        ret.status = -1;
        return ret;
    }

    inline TYPE_RETURN FOUND() const { return FAILED(); }

    inline TYPE_RETURN FOUND(size_t const val) const {
        TYPE_RETURN ret;
        ret.begin  = val;
        ret.end    = val;
        ret.status = 0;
        return ret;
    }

    inline TYPE_RETURN FOUND(size_t const begin, size_t const end) const {
        TYPE_RETURN ret;
        ret.begin  = begin;
        ret.end    = end;
        ret.status = 1;
        return ret;
    }
    ////////////////////////
    // RETURN STATUS END. //
    ////////////////////////

    //////////////////
    // RANGE BEGIN: //
    //////////////////
  public:
    template <typename F> inline size_t find_start(size_t end, F &in) {

        size_t start = 0;
        if (start >= end) { return start; }
        auto const val = reader(end);
        char       cmp = in(reader(start), val);
        if (cmp >= 0) { return start; }

    MainLoop:
        /* The Main Loop Body: */ {

            size_t const mid = (start + end) / 2;

            char const stat = (10 * (start >= mid)) + in(reader(mid), val);

            switch (stat) {
                case 10: return start;

                case -1: start = mid; goto MainLoop;

                case 0: end = mid; goto MainLoop;

                case 9:
                case 11:
                default: return end;
            }
        }

        return end;
    }

    inline size_t find_start(size_t end) {

        size_t start = 0;
        if (start >= end) { return start; }
        auto const val = reader(end);
        if (reader(start) >= val) { return start; }

    MainLoop:
        /* The Main Loop Body */ {

            size_t const mid = (start + end) / 2;

            char const stat =
              (10 * (start >= mid)) + (1 * (reader(mid) >= val));

            switch (stat) {

                case 10: return end;

                case 11: return start;

                case 1: end = mid; goto MainLoop;

                default: start = mid; goto MainLoop;
            }
        }

        return end;
    }

    template <typename F> inline size_t find_end(size_t start, F &in) {
        size_t end = reader() - 1;
        if (start >= end) { return end; }
        auto const val = reader(start);
        char       cmp = in(reader(end), val);
        if (cmp <= 0) { return end; }

    MainLoop:
        /* The main loop body: */ {

            size_t const mid = (start + end) / 2;

            char const stat = (10 * (start >= mid)) + in(reader(mid), val);

            switch (stat) {

                case 9:
                case 11:
                case 10:
                case -1: return start;

                case 0: start = mid; goto MainLoop;

                case 1: end = mid; goto MainLoop;
            }
        }

        return start;
    }

    inline size_t find_end(size_t start) {

        size_t end = reader() - 1;
        if (start >= end) { return end; }
        auto const val = reader(start);
        if (reader(end) <= val) { return end; }

    MainLoop:
        /* The main loop body */ {

            size_t const mid = (start + end) / 2;

            char const stat = (10 * (start >= mid)) + (1 * (reader(mid) <= val));

            switch (stat) {

                case 10:
                case 11: return start;

                case 1: start = mid; goto MainLoop;

                default: end = mid; goto MainLoop;
            }
        }

        return end;
    }

    inline TYPE_RETURN find_range(size_t const index) {
        return FOUND(find_start(index), find_end(index));
    }

    template <typename T>
    inline TYPE_RETURN find_range(size_t const index, T &in) {
        return FOUND(find_start(index, in), find_end(index, in));
    }
    ////////////////
    // RANGE END. //
    ////////////////

    ///////////////////////////////
    // FIND WITH COMPARER BEGIN: //
    ///////////////////////////////
  private:
    template <typename T, typename F>
    inline TYPE_RETURN find_new(T const &in, F &compare) {

        size_t begin = 0;
        size_t end   = limit - 1;
        size_t mid;
        char   status;

        /* evaluate status: */ {
            // status = (1 * (end < begin)) + (2 * (end == begin)); //
            bool const res[2] = {(end < begin), (end == begin)};
            status            = res[0] + (2 * res[1]);
        }

        switch (status) {

            case 1: return FOUND();

            case 2: {
                char const res = compare(in, reader(begin));
                if (res == 0) {
                    return FOUND(begin);
                } else {
                    return FOUND();
                }
            }

            default: break;
        }

    MainLoopBegin:
        /* The main loop body: */ {

            mid = (begin + end) / 2;

            status = compare(in, reader(mid)) + (10 * (begin == mid)); //

            switch (status) {

                case -1: end = mid; goto MainLoopBegin;

                case 1: begin = mid; goto MainLoopBegin;

                case 0:
                case 10: return FOUND(mid);

                case 11: return FOUND(mid, end);

                default: return FOUND();
            }
        }
    }
    /////////////////////////////
    // FIND WITH COMPARER END. //
    /////////////////////////////

    ///////////////////////
    // Plain find BEGIN: //
    ///////////////////////
  private:
    template <typename T> inline TYPE_RETURN find_new(T const &in) {
        size_t begin = 0;
        size_t end   = limit - 1;
        size_t mid;
        char   status;

        /* assign the status: */ {
            // status = (1 * (end < begin)) + (2 * (end == begin)); //
            bool const res[2] = {(end < begin), (end == begin)};
            status            = res[0] + (res[1] * 2);
        }

        switch (status) {

            case 1: return FOUND();

            case 2: {
                if (in == reader(begin)) {
                    return FOUND(begin);
                } else {
                    return FOUND();
                }
            }

            default: break;
        }

    MainLoopBegin:
        /* The main loop body: */ {
            mid = (begin + end) / 2;

            // status = (-1 * (in < reader(mid))) + (1 * (in > reader(mid))) +
            //         (10 * (begin == mid));

            bool const res[3] = {(in < reader(mid)), (in > reader(mid)),
                                 (begin == mid)};

            status = (res[0] * -1) + (res[1]) + (res[2] * 10);

            switch (status) {
                case -1: end = mid; goto MainLoopBegin;

                case 1: begin = mid; goto MainLoopBegin;

                case 0:
                case 10: return FOUND(mid);

                case 11: return FOUND(mid, end);

                default: return FOUND();
            }
        }
    }
    /////////////////////
    // Plain find END. //
    /////////////////////

  public:
    template <typename T> inline TYPE_RETURN operator()(T const &in) {
        return find_new(in); //
    }

    template <typename T, typename F>
    inline TYPE_RETURN operator()(T const &in, F &compare) {
        return find_new(in, compare); //
    }

    _MACRO_CLASS_NAME_(TYPE_READER &_reader)
      : reader(_reader), limit(reader()) {}

    ~_MACRO_CLASS_NAME_() {}

    template <typename T> static inline _MACRO_CLASS_NAME_<T> GET(T &in) {
        _MACRO_CLASS_NAME_<T> ret(in);
        return ret;
    }
};
#undef _MACRO_CLASS_NAME_
////////////////////////////////
// Main searching class END.} //
////////////////////////////////

#endif
