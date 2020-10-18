#ifndef _HEADER_GUARD_CPPFileIO_FileFD_
#define _HEADER_GUARD_CPPFileIO_FileFD_

/////////////////////
// Includes BEGIN: //
/////////////////////
#include "../Headers.hh"
#include "./Basic.hh"
///////////////////
// Includes END. //
///////////////////

#define _MACRO_CLASS_NAME_ FileFD

class _MACRO_CLASS_NAME_ {

  public:
    using TYPE_SELF = _MACRO_CLASS_NAME_;

    //////////////////////
    // Data part BEGIN: //
    //////////////////////

  private:
    std::string filename;
    int         fd;
    struct stat abtme;
    int         filemode;
    void *      mapped;
    size_t      maplength;
    int         mmapprot;

    ////////////////////
    // Data part END. //
    ////////////////////

    //////////////////////
    // File Mode BEGIN: //
    //////////////////////

  private:
    inline void construct(std::string const Afile = std::string("outfile")) {
        filename  = Afile;
        fd        = -1;
        filemode  = -1;
        mapped    = MAP_FAILED;
        mmapprot  = PROT_NONE;
        maplength = 0;
    }

  public:
    inline int closefile() {
        unmapfile();
        filemode = -1;
        mmapprot = PROT_NONE;
        if (fd < 0) {
            return 0;
        } else {
            int const ret = close(fd);
            fd            = -1;
            return ret;
        }
    }

    inline void check_file_mode(int const newfilemode = -1) {
        if (filemode != newfilemode) { closefile(); }
        if (fd < 0) {
            fd = open(static_cast<const char *>(&(filename[0])), newfilemode,
                      static_cast<mode_t>(0755)); //
            filemode = newfilemode;
        }
    }

    inline void readfile() {
        check_file_mode(static_cast<int>(O_RDONLY));
        mmapprot = static_cast<int>(PROT_READ);
    }

    inline void writefile() {
        check_file_mode(static_cast<int>(O_WRONLY | O_CREAT | O_TRUNC)); //
        mmapprot = static_cast<int>(PROT_WRITE);                         //
    }

    inline void appendfile() {
        check_file_mode(static_cast<int>(O_RDWR | O_CREAT)); //
        mmapprot = static_cast<int>(PROT_READ | PROT_WRITE); //
    }

    inline void destroy() { closefile(); }

    inline void reconstruct(std::string const Afile = std::string("outfile")) {
        destroy();
        construct(Afile);
    }

    inline void reconstruct(char const *Afile) {
        reconstruct(std::string(Afile));
    }

    inline FileFD &operator()(std::string const name) {
        reconstruct(name);
        return (*this);
    }

    inline int truncatefile(off_t const length = 0) {
        return static_cast<int>(ftruncate(fd, length));
    }

    ////////////////////
    // File Mode END. //
    ////////////////////

    /////////////////
    // Seek BEGIN: //
    /////////////////

  public:
    inline off_t seekfile(off_t const offset = 0, int const whence = SEEK_CUR) {
        return static_cast<off_t>(lseek(fd, offset, whence)); //
    }

    inline off_t operator()(off_t const offset = 0) { return seekfile(offset); }

    inline FileFD &operator[](off_t const pos) {
        seekfile(static_cast<off_t>(pos), SEEK_SET);
        return (*this);
    }

    ///////////////
    // Seek END. //
    ///////////////

    /////////////////
    // Info BEGIN: //
    /////////////////

  public:
    inline int info() {
        return static_cast<int>(fstat(fd,
                                      static_cast<struct stat *>(&abtme))); //
    }

    inline off_t sizefile() {
        info();
        return static_cast<off_t>(abtme.st_size);
    }

    inline std::string const &getfilename() const { return filename; }

    inline int getfd() const { return fd; }

    ///////////////
    // Info END. //
    ///////////////

    ///////////////////////////
    // Memory Mapping BEGIN: //
    ///////////////////////////

  public:
    inline void *mapfile(size_t const length, off_t const offset = 0) {
        if ((maplength == 0) && (mmapprot != PROT_NONE) &&
            (mapped == MAP_FAILED)) {
            off_t const total_len = length + offset;
            if (sizefile() < total_len) { truncatefile(total_len); }
            mapped = reinterpret_cast<void *>(
              mmap(reinterpret_cast<void *>(NULL), length, mmapprot,
                   static_cast<int>(MAP_SHARED), fd, offset)); //
            if (mapped != MAP_FAILED) {
                maplength = length;
            } else {
                printf("MMAP FAILED 1 !!! %s\n", &(filename[0]));
            }
        } else {
            printf("MMAP FAILED 2 !!!\n");
        }
        return mapped;
    }

    inline int unmapfile() {
        int ret = 0;
        if ((mapped != MAP_FAILED) && (maplength > 0) &&
            (mmapprot != PROT_NONE)) {
            ret       = munmap(reinterpret_cast<void *>(mapped),
                         reinterpret_cast<size_t>(maplength)); //
            maplength = 0;
            mapped    = MAP_FAILED;
        }
        return ret;
    }

    inline size_t getmaplength() const { return maplength; }

    /////////////////////////
    // Memory Mapping END. //
    /////////////////////////

    /////////////////////////
    // Read & Write BEGIN: //
    /////////////////////////

  public:
    inline ssize_t read2file(void *buf = NULL, size_t const count = 0) {
        return static_cast<ssize_t>(read(fd, buf, count)); //
    }

    inline ssize_t write2file(const void *buf = NULL, size_t const count = 0) {
        return static_cast<ssize_t>(write(fd, buf, count)); //
    }
    template <typename T>
    inline ssize_t multiread2file(T &buf, size_t const count = 1) {
        return read2file(reinterpret_cast<void *>(&buf),
                         static_cast<size_t>(sizeof(T) * count));
    }

    template <typename T>
    inline ssize_t multiwrite2file(const T &buf, size_t count = 1) {
        return write2file(reinterpret_cast<const void *>(&buf),
                          static_cast<size_t>(sizeof(T) * count)); //
    }

    template <typename T>
    inline ssize_t WriteVector(std::vector<T> const &out) {
        size_t const count       = out.size();
        ssize_t      writtensize = multiwrite2file(count);
        writtensize              = writtensize + multiwrite2file(out[0], count);
        return writtensize;
    }

    template <typename T> inline ssize_t ReadVector(std::vector<T> &out) {
        size_t       count       = 0;
        size_t const oldsize     = out.size();
        ssize_t      writtensize = multiread2file(count);
        out.resize(oldsize + count);
        writtensize = multiread2file(out[oldsize], count);
        return writtensize;
    }

    inline ssize_t WriteString(std::string const &out) {
        size_t const count       = out.size();
        ssize_t      writtensize = multiwrite2file(count);
        writtensize              = writtensize + multiwrite2file(out[0], count);
        return writtensize;
    }

    inline ssize_t ReadString(std::string &out) {
        size_t       count       = 0;
        size_t const oldsize     = out.size();
        ssize_t      writtensize = multiread2file(count);
        out.resize(oldsize + count);
        writtensize = multiread2file(out[oldsize], count);
        return writtensize;
    }

    inline ssize_t WriteStrings(std::vector<std::string> const &out) {
        size_t const count       = out.size();
        ssize_t      writtensize = multiwrite2file(count);
        for (size_t i = 0; i < count; i++) {
            writtensize += WriteString(out[i]);
        }
        return writtensize;
    }

    inline ssize_t ReadStrings(std::vector<std::string> &out) {
        out.clear();
        size_t  count       = 0;
        ssize_t writtensize = multiread2file(count);
        out.resize(count);
        for (size_t i = 0; i < count; i++) {
            writtensize += ReadString(out[i]);
        }
        return writtensize;
    }

    ///////////////////////
    // Read & Write END. //
    ///////////////////////

    ////////////////////////////////
    // Convinence Wrappers BEGIN: //
    ////////////////////////////////

  public:
    inline ssize_t operator>>(char &out) { return multiread2file(out); }
    inline ssize_t operator<<(char const out) { return multiwrite2file(out); }
    inline ssize_t operator>>(int &out) { return multiread2file(out); }
    inline ssize_t operator<<(int const out) { return multiwrite2file(out); }
    inline ssize_t operator>>(float &out) { return multiread2file(out); }
    inline ssize_t operator<<(float const out) { return multiwrite2file(out); }
    inline ssize_t operator>>(double &out) { return multiread2file(out); }
    inline ssize_t operator<<(double const out) { return multiwrite2file(out); }
    inline ssize_t operator>>(long &out) { return multiread2file(out); }
    inline ssize_t operator<<(long const out) { return multiwrite2file(out); }
    inline ssize_t operator>>(size_t &out) { return multiread2file(out); }
    inline ssize_t operator<<(size_t const out) { return multiwrite2file(out); }

    inline ssize_t operator<<(std::string const out) {
        return WriteString(out);
    }
    inline ssize_t operator>>(std::string &out) { return ReadString(out); }

    template <typename T> inline ssize_t operator<<(std::vector<T> const &out) {
        return WriteVector(out);
    }
    template <typename T> inline ssize_t operator>>(std::vector<T> &out) {
        return ReadVector(out);
    }

    //////////////////////////////
    // Convinence Wrappers END. //
    //////////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////

  public:
    _MACRO_CLASS_NAME_(const char *Afile) { construct(Afile); }
    _MACRO_CLASS_NAME_() { construct(); }
    _MACRO_CLASS_NAME_(std::string Afile) { construct(Afile); }
    ~_MACRO_CLASS_NAME_() { destroy(); }

    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
