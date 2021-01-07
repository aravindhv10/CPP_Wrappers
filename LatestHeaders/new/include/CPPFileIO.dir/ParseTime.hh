#ifndef _HEADER_GUARD_CPPFileIO_ParseTime_
#define _HEADER_GUARD_CPPFileIO_ParseTime_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../Headers.hh"
#include "./Basic.hh"
//////////////////
// Headers END. //
//////////////////

inline time_t Parse_Time_Slow(char const *intime) {
    std::tm mytime;
    strptime(intime, "%Y-%m-%d %H:%M:%S", &mytime);
    std::time_t ret = std::mktime(&mytime);
    return ret;
}

#define _MACRO_CLASS_NAME_ FastTimeParser
class _MACRO_CLASS_NAME_ {
  public:
    static inline TYPE_U32 get_year(char const *in) {
        return (static_cast<TYPE_U32>(in[0] - '0') * 1000) +
               (static_cast<TYPE_U32>(in[1] - '0') * 100) +
               (static_cast<TYPE_U32>(in[2] - '0') * 10) +
               (static_cast<TYPE_U32>(in[3] - '0'));
    }

    static inline TYPE_U8 get_month(char const *in) {
        return (static_cast<TYPE_U8>(in[5] - '0') * 10) +
               (static_cast<TYPE_U8>(in[6] - '0'));
    }

    static inline TYPE_U8 get_day(char const *in) {
        return (static_cast<TYPE_U8>(in[8] - '0') * 10) +
               (static_cast<TYPE_U8>(in[9] - '0'));
    }

    static inline TYPE_U8 get_hour(char const *in) {
        return (static_cast<TYPE_U8>(in[11] - '0') * 10) +
               (static_cast<TYPE_U8>(in[12] - '0'));
    }

    static inline TYPE_U8 get_minute(char const *in) {
        return (static_cast<TYPE_U8>(in[14] - '0') * 10) +
               (static_cast<TYPE_U8>(in[15] - '0'));
    }

    static inline TYPE_U8 get_second(char const *in) {
        return (static_cast<TYPE_U8>(in[17] - '0') * 10) +
               (static_cast<TYPE_U8>(in[18] - '0'));
    }

    static inline time_t to_epoch(char const *in) {
        std::tm ret;
        ret.tm_year  = get_year(in) - 1900;
        ret.tm_mon   = get_month(in) - 1;
        ret.tm_mday  = get_day(in);
        ret.tm_hour  = get_hour(in);
        ret.tm_min   = get_minute(in);
        ret.tm_sec   = get_second(in);
        ret.tm_isdst = 0;
        // return std::mktime(&ret);
        return timegm(&ret);
    }
};
#undef _MACRO_CLASS_NAME_

inline time_t Parse_Time(char const *intime) {
    if (true) {
        return FastTimeParser::to_epoch(intime);
    } else {
        std::tm mytime;
        strptime(intime, "%Y-%m-%d %H:%M:%S", &mytime);
        std::time_t ret = std::mktime(&mytime);
        return ret;
    }
}

inline time_t Parse_Time(std::string const intime) {
    return Parse_Time(intime.c_str());
}

#endif
