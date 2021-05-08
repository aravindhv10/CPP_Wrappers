#ifndef _HEADER_GUARD_all_
#define _HEADER_GUARD_all_

#include "./include/Read_Show_Functions.hh"

#define _MACRO_CLASS_NAME_ element
class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_SELF = _MACRO_CLASS_NAME_;
    using str       = std::string;

    str filename, sha256;

    inline bool operator>(TYPE_SELF const &other) const {
        return sha256 > other.sha256;
    }

    inline bool operator<(TYPE_SELF const &other) const {
        return sha256 < other.sha256;
    }

    inline bool operator==(TYPE_SELF const &other) const {
        return (sha256 == other.sha256);
    }

    inline bool operator!=(TYPE_SELF const &other) const {
        return (sha256 != other.sha256);
    }

    _MACRO_CLASS_NAME_(str FILENAME, str SHA256)
      : filename(FILENAME), sha256(SHA256) {}

    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};
using elements = std::vector<element>;
#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ config
class _MACRO_CLASS_NAME_ {
  public:
    using str  = std::string;
    using strs = std::vector<str>;

  public:
    strs base_urls;
    strs versions;
    strs components;
    strs final_urls;

  private:
    inline void generate_names() {
        final_urls.clear();
        strs final_files;

        final_files.push_back("Contents-amd64.gz");
        final_files.push_back("Contents-i386.gz");
        final_files.push_back("binary-amd64/Packages.gz");
        final_files.push_back("binary-i386/Packages.gz");
        final_files.push_back("binary-all/Packages.gz");
        final_files.push_back("binary-amd64/Packages.xz");
        final_files.push_back("binary-i386/Packages.xz");
        final_files.push_back("binary-all/Packages.xz");
        final_files.push_back("binary-amd64/Packages");
        final_files.push_back("binary-i386/Packages");
        final_files.push_back("binary-all/Packages");

        for (size_t i = 0; i < base_urls.size(); i++) {
            for (size_t j = 0; j < versions.size(); j++) {
                for (size_t k = 0; k < components.size(); k++) {
                    for (size_t l = 0; l < final_files.size(); l++) {
                        str tmp = base_urls[i] + "/dists/" + versions[j] + "/" +
                                  components[k] + "/" + final_files[l];
                        final_urls.push_back(tmp);
                    }
                }
            }
        }
    }

  public:
    inline strs &operator()() {
        generate_names();
        return final_urls;
    }

    _MACRO_CLASS_NAME_() {
        base_urls.push_back("http://ftp.debian.org/debian");

        versions.push_back("stable-backports");

        components.push_back("main");
        components.push_back("contrib");
        components.push_back("non-free");
    }
    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ MainWorker
class _MACRO_CLASS_NAME_ {
  public:
    using string      = std::string;
    using strings     = std::vector<string>;
    using file_buffer = CPPFileIO::Dynamic1DArray<char, long>;

  private:
    static inline std::string const base_mirror_url() {
        string const base_url("http://ftp.debian.org/debian/");
        return base_url;
    }

    static inline std::string const get_prefix() {
        string const prefix("./MIRROR/ftp.debian.org/debian/");
        return prefix;
    }

    static inline void get_link_dists(strings &list) {
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/Contents-amd64.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/Contents-i386.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/Contents-source.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/Contents-amd64.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/Contents-i386.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/Contents-source.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/Contents-amd64.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/Contents-i386.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/Contents-source.gz");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/binary-amd64/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/binary-i386/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "main/binary-all/");
        list.push_back(
          "http://ftp.debian.org/debian/dists/stable-backports/main/source/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/binary-amd64/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/binary-i386/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/binary-all/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "contrib/source/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/binary-amd64/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/binary-i386/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/binary-all/");
        list.push_back("http://ftp.debian.org/debian/dists/stable-backports/"
                       "non-free/source/");
    }

  private:
    elements store;

  private:
    static inline file_buffer get_file(string const in) {
        CPPFileIO::FileArray<char const> reader(in);
        size_t const                     limit = reader();
        file_buffer                      ret(limit + 1);
        memcpy(
          /* void *dest = */ static_cast<void *>(&(ret(0))),
          /* const void *src = */ static_cast<const void *>(&(reader(0))),
          /* size_t n = */ static_cast<size_t>(limit));
        ret(limit) = 0;
        return ret;
    }

    static inline void get_lines(strings &list, string const filename) {
        auto                   buf = get_file(filename);
        CPPFileIO::MyStr<long> cutter(&(buf(0)));
        auto                   lines = cutter('\n');
        for (size_t i = 0; i < lines.size(); i++) {
            list.push_back(std::string(lines[i].start()));
        }
    }

    static inline void WGET_MIRROR(string const in) {
        CPPFileIO::ExternalStarter<true>::GET("/usr/bin/wget")("-c")("-m")(
          "--timeout=10")("-np")(in);
    }

    static inline void WGET(string const in, string const out) {
        bool constexpr use_wget = false;
        if (use_wget) {
            CPPFileIO::ExternalStarter<true>::GET("/usr/bin/wget")("-c")(in)(
              "-O")(out);
            CPPFileIO::ExternalStarter<true>::GET("/usr/bin/aria2c")("-x16")(
              "-c")(in);
        } else {
            CPPFileIO::ExternalStarter<true>::GET("./download")(in)(out);
        }
    }

    static inline void get_package_list(strings &list) {
        get_lines(list, "./list_Packages");
    }

    static inline bool shamatch(string const &in) {
        // char const buf[9] = "SHA256: " ;
        // size_t const * buff = reinterpret_cast <size_t const *>(buf) ;
        if (in.size() < 9) {
            return false;
        } else {
            size_t const  buff = 2322228159967217747; // Deadly vodoo !
            size_t const *inn  = reinterpret_cast<size_t const *>(&(in[0]));
            return !(inn[0] ^ buff);
        }
    }

    static inline bool filenamematch(string const &in) {
        if (in.size() < 11) {
            return false;
        } else {
            // char const                buf[11] = "Filename: ";
            size_t const              pos0 = 7308604897068083526;
            CPPFileIO::TYPE_U16 const pos1 = 8250;
            size_t const *part0 = reinterpret_cast<size_t const *>(&(in[0]));
            CPPFileIO::TYPE_U16 const *part1 =
              reinterpret_cast<CPPFileIO::TYPE_U16 const *>(&(in[8]));
            return (!(part0[0] ^ pos0)) & (!(part1[0] ^ pos1));
        }
    }

    static inline bool is_dbg(string const in) {
        char constexpr pattern[6] = "-dbg_";
        for (size_t i = 0; i < in.size(); i++) {
            size_t j = 0;
            while (in[i + j] == pattern[j]) { j++; }
            if (j > 4) { return true; }
        }
        return false;
    }

    inline bool is_dbg(size_t const i) const {
        return is_dbg(store[i].filename);
    }

    inline void get_list() {
        store.clear();

        elements tmpbuf; /* populate the tmp buffer: */
        {
            strings in;
            get_package_list(in);
            for (size_t id_in = 0; id_in < in.size(); id_in++) {

                strings lines;
                get_lines(lines, in[id_in]);

                strings list_sha, list_filename;
                for (size_t i = 0; i < lines.size(); i++) {
                    if (shamatch(lines[i])) {
                        list_sha.push_back(&(lines[i][8]));
                    }

                    if (filenamematch(lines[i])) {
                        list_filename.push_back(&(lines[i][10]));
                    }
                }

                size_t const min =
                  CPPFileIO::mymin(list_sha.size(), list_filename.size());

                for (size_t i = 0; i < min; i++) {
                    element tmp;
                    tmp.filename = list_filename[i];
                    tmp.sha256   = list_sha[i];
                    tmpbuf.push_back(tmp);
                }
            }
        }

        if (tmpbuf.size() > 1) { std::sort(tmpbuf.begin(), tmpbuf.end()); }

        if (tmpbuf.size() > 0) {
            store.push_back(tmpbuf[0]);
            for (size_t i = 1; i < tmpbuf.size(); i++) {
                if (tmpbuf[i] != tmpbuf[i - 1]) { store.push_back(tmpbuf[i]); }
            }
        }
    }

  public:
    static inline void download_dists() {
        strings link_dists;
        get_link_dists(link_dists);
        mkdir("./MIRROR", 0755);
        chdir("./MIRROR");

        /*Download the mirror in parallel:*/ {
            CPPFileIO::ForkMe forker;
            for (size_t i = 0; i < link_dists.size(); i++) {
                if (forker.InKid()) { WGET_MIRROR(link_dists[i]); }
            }
        }
        chdir("../");

        { CPPFileIO::ExternalStarter<true>::GET("./extract_Packages"); }
    }

    inline void clean_sha() {
        get_list();
        {
            CPPFileIO::ExternalStarter<true>::GET("/bin/mv")("-vf")("--")(
              "./SHA256")("WASTE");
        }
        mkdir("./SHA256", 0755);
        for (size_t i = 0; i < store.size(); i++) {
            if (!is_dbg(i)) {
                string const input = "./WASTE/" + store[i].sha256;
                CPPFileIO::ExternalStarter<true>::GET("/bin/mv")("-vf")("--")(
                  input)("./SHA256");
            }
        }
        {
            CPPFileIO::ExternalStarter<true>::GET("/bin/rmdir")("-v")("--")(
              "WASTE");
        }
    }

    inline void show_dbg() {
        get_list();
        for (size_t i = 0; i < store.size(); i++) {
            if (is_dbg(i)) { printf("DEBUG: %s\n", store[i].filename.c_str()); }
        }
    }

    inline void download_list() {
        get_list();
        string const base_url = base_mirror_url();
        string const out_dir("./SHA256/");
        mkdir(out_dir.c_str(), 0755);

        CPPFileIO::Atomic_Counter<long> counter;

#pragma omp parallel for
        for (int nth = 0; nth < 16; nth++) {
            size_t index;
        MainLoop:
            index = counter++;
            if (index < store.size()) {
                if (!is_dbg(index)) {
                    WGET(base_url + store[index].filename,
                         out_dir + store[index].sha256);
                }
                goto MainLoop;
            }
        }
    }

    inline void setup_files() {
        get_list();
        string const prefix = get_prefix();
#pragma omp parallel for
        for (size_t i = 0; i < store.size(); i++) {
            if (!is_dbg(i)) {
                string const outfile = prefix + store[i].filename;
                size_t       counts  = 0;
                for (char const &i : outfile) {
                    if (i == '/') { counts++; }
                }
                if (counts > 0) { counts--; }
                string infile = "./SHA256/" + store[i].sha256;
                while (counts > 0) {
                    infile = "../" + infile;
                    counts--;
                }
                {
                    CPPFileIO::ExternalStarter<true>::GET("./do_link")(outfile)(
                      infile);
                }
            }
        }
    }

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
