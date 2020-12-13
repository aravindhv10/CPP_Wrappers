#ifndef _HEADER_GUARD_Read_Show_Functions_Read
#define _HEADER_GUARD_Read_Show_Functions_Read

///////////////////
// Header BEGIN: //
///////////////////
#include "../CPPFileIO.hh"
#include "../StaticArray.hh"
/////////////////
// Header END. //
/////////////////

////////////////////
// String BEGIN:{ //
////////////////////
template <size_t n>
inline void Read(StaticArray::ND_ARRAY<n, char> &dest, std::string const &src) {
    std::memset(/* void *s = */ reinterpret_cast<void *>(dest.GET_DATA()),
                /* int c = */ 0, /* size_t n = */ dest.SIZE());

    size_t const limit = CPPFileIO::mymin(
      static_cast<size_t>(src.size()), static_cast<size_t>(dest.SIZE() - 1)); //

    memcpy(/* void *dest = */ reinterpret_cast<void *>(dest.GET_DATA()),
           /* const void *src = */ reinterpret_cast<void const *>(src.c_str()),
           /* size_t n = */ limit);
}

template <size_t n>
inline void Read(StaticArray::ND_ARRAY<n, char> &dest, char const *src) {
    std::memset(/* void *s = */ reinterpret_cast<void *>(dest.GET_DATA()),
                /* int c = */ 0, /* size_t n = */ dest.SIZE());

    size_t const limit = CPPFileIO::mymin(static_cast<size_t>(strlen(src)),
                                          static_cast<size_t>(dest.SIZE() - 1));

    memcpy(/* void *dest = */ reinterpret_cast<void *>(dest.GET_DATA()),
           /* const void *src = */ reinterpret_cast<void const *>(src),
           /* size_t n = */ limit);
}
//////////////////
// String END.} //
//////////////////

///////////////////
// Float BEGIN:{ //
///////////////////
static inline void Read(float &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = NAN;
    } else {
        dest = std::stof(src);
    }
}

static inline void Read(float &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = NAN;
    } else {
        dest = std::stof(src);
    }
}
/////////////////
// Float END.} //
/////////////////

////////////////////
// double BEGIN:{ //
////////////////////
static inline void Read(double &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = NAN;
    } else {
        dest = std::stod(src);
    }
}

static inline void Read(double &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = NAN;
    } else {
        dest = std::stod(src);
    }
}
//////////////////
// double END.} //
//////////////////

//////////////////
// char BEGIN:{ //
//////////////////
static inline void Read(char &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = 0;
    } else {
        dest = src[0];
    }
}

static inline void Read(char &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = 0;
    } else {
        dest = src[0];
    }
}
////////////////
// char END.} //
////////////////

///////////////////////////
// unsigned char BEGIN:{ //
///////////////////////////
static inline void Read(unsigned char &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = 0;
    } else {
        dest = static_cast<unsigned char>(std::stoi(src));
    }
}

static inline void Read(unsigned char &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = 0;
    } else {
        dest = static_cast<unsigned char>(std::stoi(src));
    }
}
/////////////////////////
// unsigned char END.} //
/////////////////////////

//////////////////////////
// unsigned int BEGIN:{ //
//////////////////////////
static inline void Read(unsigned int &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = -999999;
    } else {
        sscanf(src.c_str(), "%u", &dest);
    }
}

static inline void Read(unsigned int &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = -999999;
    } else {
        sscanf(src, "%u", &dest);
    }
}
////////////////////////
// unsigned int END.} //
////////////////////////

/////////////////
// int BEGIN:{ //
/////////////////
static inline void Read(int &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = -999999;
    } else {
        dest = std::stoi(src);
    }
}

static inline void Read(int &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = -999999;
    } else {
        dest = std::stoi(src);
    }
}
///////////////
// int END.} //
///////////////

//////////////////
// long BEGIN:{ //
//////////////////
static inline void Read(long &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = -999999;
    } else {
        dest = std::stol(src);
    }
}

static inline void Read(long &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = -999999;
    } else {
        dest = std::stol(src);
    }
}
////////////////
// long END.} //
////////////////

///////////////////////////
// unsigned long BEGIN:{ //
///////////////////////////
static inline void Read(unsigned long &dest, std::string const &src) {
    if (src.size() < 1) {
        dest = 0;
    } else {
        sscanf(src.c_str(), "%lu", &dest);
    }
}

static inline void Read(unsigned long &dest, char const *src) {
    if (strlen(src) < 1) {
        dest = 0;
    } else {
        sscanf(src, "%lu", &dest);
    }
}
/////////////////////////
// unsigned long END.} //
/////////////////////////

#endif
