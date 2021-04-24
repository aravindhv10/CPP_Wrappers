#ifndef _HEADER_GUARD_all_
#define _HEADER_GUARD_all_

#include "./LatestHeaders/new/include/Read_Show_Functions.hh"

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
    strings list_sha, list_filename;

  private:
    static inline file_buffer get_file(string const in) {
        CPPFileIO::FileArray<char> reader(in);
        file_buffer                ret(reader() + 1);
        reader(0, ret);
        ret(ret() - 1) = 0;
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
        CPPFileIO::ExternalStarter<true>::GET("/usr/bin/wget")("-c")(
          "-m")("-np")(in);
    }

    static inline void WGET(string const in, string const out) {
        bool constexpr use_wget = false;
        if (use_wget) {
            CPPFileIO::ExternalStarter<true>::GET("/usr/bin/wget")("-c")(
              in)("-O")(out);
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
        return is_dbg(list_filename[i]);
    }

    inline void get_list() {
        list_sha.clear();
        list_filename.clear();

        strings in;
        get_package_list(in);

        for (size_t id_in = 0; id_in < in.size(); id_in++) {
            strings lines;
            get_lines(lines, in[id_in]);
            for (size_t i = 0; i < lines.size(); i++) {
                if (shamatch(lines[i])) { list_sha.push_back(&(lines[i][8])); }

                if (filenamematch(lines[i])) {
                    list_filename.push_back(&(lines[i][10]));
                }
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
            CPPFileIO::ExternalStarter<true>::GET("/bin/mv")("-vf")(
              "--")("./SHA256")("WASTE");
        }
        mkdir("./SHA256", 0755);
        for (size_t i = 0; i < list_sha.size(); i++) {
            if (!is_dbg(i)) {
                string const input = "./WASTE/" + list_sha[i];
                CPPFileIO::ExternalStarter<true>::GET("/bin/mv")("-vf")(
                  "--")(input)("./SHA256");
            }
        }
        {
            CPPFileIO::ExternalStarter<true>::GET("/bin/rmdir")("-v")(
              "--")("WASTE");
        }
    }

    inline void show_dbg() {
        get_list();
        for (size_t i = 0; i < list_filename.size(); i++) {
            if (is_dbg(i)) { printf("DEBUG: %s\n", list_filename[i].c_str()); }
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
            index = counter();
            if (index < list_sha.size()) {
                if (!is_dbg(index)) {
                    WGET(base_url + list_filename[index],
                         out_dir + list_sha[index]);
                }
                goto MainLoop;
            }
        }
    }

    inline void setup_files () {
        get_list();
        string const prefix = get_prefix();
#pragma omp parallel for
        for(size_t i=0;i< list_filename.size();i++ ){
            if(!is_dbg(i)){
                string const outfile = prefix + list_filename[i];
                size_t counts = 0;
                for(char const & i : outfile) {
                    if(i=='/'){counts++;}
                }
                if(counts>0){counts--;}
                string infile = "./SHA256/" + list_sha[i];
                while(counts>0){
                  infile = "../" + infile ;
                  counts--;
                }
                {CPPFileIO::ExternalStarter<true>::GET("./do_link")(outfile)(infile);}
            }
        }
    }

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_

#endif
