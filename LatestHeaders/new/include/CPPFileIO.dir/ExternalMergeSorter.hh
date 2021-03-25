#ifndef _HEADER_GUARD_CPPFileIO_ExternalMergeSorter_
#define _HEADER_GUARD_CPPFileIO_ExternalMergeSorter_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Basic.hh"
#include "./ExternalPrograms.hh"
#include "./FileArray.hh"
#include "./FileFD.hh"
#include "./FileSplitter.hh"
#include "./FileWriter.hh"
#include "./Sorter.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ ExternalMergeSorter

template <typename T, bool cleanup = false> class _MACRO_CLASS_NAME_ {

    ////////////////////////
    // Definitions BEGIN: //
    ////////////////////////
  private:
    using TYPE_ELEMENT = T;
    using TYPE_SELF    = _MACRO_CLASS_NAME_<TYPE_ELEMENT, cleanup>;
    //////////////////////
    // Definitions END. //
    //////////////////////

    //////////////////////////
    // Data Elements BEGIN: //
    //////////////////////////
  private:
    std::string const infilename;
    std::string const outfilename;
    std::string const dirname;
    size_t const      N_SPLITS;
    size_t const      NTH;
    ////////////////////////
    // Data Elements END. //
    ////////////////////////

    ///////////////////////////////////
    // Main Working Functions BEGIN: //
    ///////////////////////////////////
    static inline void RM(std::string const filename) {
        ExternalStarter<true>::GET("/bin/rm")("-vf")("--")(filename);
    }

    static inline void RMDIR(std::string const filename) {
        ExternalStarter<true>::GET("/bin/rmdir")("-v")("--")(filename);
    }

    inline std::string GetFileName(size_t const i) const {
        mkdir(&(dirname[0]), 0755);
        char tmp[128];
        sprintf(tmp, "/bin.%zu.piece", i);
        return dirname + std::string(tmp);
    }

    inline std::string GetFileName(size_t const i1, size_t const i2) const {
        if (i1 == i2) {
            return GetFileName(i1);
        } else {
            mkdir(&(dirname[0]), 0755);
            char tmp[128];
            sprintf(tmp, "/bin.%zu-%zu.sorted", i1, i2);
            std::string ret = dirname + std::string(tmp);
            return ret;
        }
    }

    inline void MergeRange(size_t const i1, size_t const i2,
                           size_t const dp = 0) {
        if (i1 == i2) {
            return;
        } else if (i2 == (i1 + 1)) {
            MergeFile<TYPE_ELEMENT>(GetFileName(i1), GetFileName(i2),
                                    GetFileName(i1, i2));
            /* Remove the temporary file: */ {
                ExternalStarter<true>::GET("/bin/rm")("-v")("--")(
                  GetFileName(i1))(GetFileName(i2));
            }
            return;
        } else {
            size_t const ath = shifter(dp);
            size_t const mid = (i1 + i2) / 2;
            if (ath <= NTH) {
                ForkMe forker;
                if (forker.InKid()) {
                    MergeRange(i1, mid, dp + 1);
                } else {
                    MergeRange(mid + 1, i2, dp + 1);
                }
            } else {
                MergeRange(i1, mid, dp + 1);
                MergeRange(mid + 1, i2, dp + 1);
            }
            MergeFile<TYPE_ELEMENT>(GetFileName(i1, mid),
                                    GetFileName(mid + 1, i2),
                                    GetFileName(i1, i2)); //
            /* Remove the temporary file: */ {
                ExternalStarter<true>::GET("/bin/rm")("-v")("--")(
                  GetFileName(i1, mid))(GetFileName(mid + 1, i2));
            }
            return;
        }
    }

    inline void MergeRange() {
        MergeRange(0, N_SPLITS - 1);
        std::string filename = GetFileName(0, N_SPLITS - 1);
        /* Move the temporary file: */ {
            ExternalStarter<true>::GET("/bin/mv")("-v")("--")(filename)(
              outfilename);
        }
        RMDIR(dirname);
    }

    inline void SortFileSlave(size_t const index) {
        std::string const filename = GetFileName(index);
        SortFile<TYPE_ELEMENT>(filename);
    }

    inline void SortAllFiles() {
#pragma omp parallel for
        for (size_t j = 0; j < NTH; j++)
            for (size_t i = j; i < N_SPLITS; i += NTH) { SortFileSlave(i); }
    }

    inline void DoSplit() {
        std::vector<std::string> names;
        for (size_t i = 0; i < N_SPLITS; i++) {
            names.push_back(GetFileName(i));
        }
        SplitFile<TYPE_ELEMENT>(infilename, names, NTH);
        if (cleanup) { RM(infilename); }
    }
    /////////////////////////////////
    // Main Working Functions END. //
    /////////////////////////////////

    /////////////////////////////////////
    // Constructor & Destructor BEGIN: //
    /////////////////////////////////////
  private:
    _MACRO_CLASS_NAME_(std::string const _infilename,
                       std::string const _outfilename, size_t const n_splits,
                       size_t const nth)
      : infilename(_infilename), outfilename(_outfilename),
        dirname(outfilename + ".dir"), N_SPLITS(n_splits), NTH(nth) {}

    ~_MACRO_CLASS_NAME_() {}
    ///////////////////////////////////
    // Constructor & Destructor END. //
    ///////////////////////////////////

    ///////////////////////////
    // Main Interface BEGIN: //
    ///////////////////////////
  public:
    static inline void DoAllSteps(std::string const binfilename = "./infile",
                                  std::string const tmpdirname  = "./tmps",
                                  size_t const      n_splits    = 16,
                                  size_t const      nth         = 4) {
        TYPE_SELF
        slave(binfilename, tmpdirname, n_splits, nth);
        slave.DoSplit();
        slave.SortAllFiles();
        slave.MergeRange();
    }
    /////////////////////////
    // Main Interface END. //
    /////////////////////////
};

#undef _MACRO_CLASS_NAME_

#endif
