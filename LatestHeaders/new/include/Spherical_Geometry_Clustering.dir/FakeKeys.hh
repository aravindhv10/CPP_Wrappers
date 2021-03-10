#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_FakeKeys_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_FakeKeys_

#include "../CPPFileIO.hh"

#define _MACRO_CLASS_NAME_ FakeKeys
class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_SELF          = _MACRO_CLASS_NAME_;
    using TYPE_RANDOM_ENGINE = pcg64;
    using TYPE_DISTRIBUTION  = std::uniform_int_distribution<size_t>;
    using TYPE_OUT           = std::vector<std::string>;

  private:
    size_t   MAIN_KEY;
    TYPE_OUT OUT_KEYS;

  public:
    inline TYPE_OUT const &operator()(size_t const i) {
        TYPE_RANDOM_ENGINE engine(MAIN_KEY);
        TYPE_DISTRIBUTION  dist(1, 1 << 15);
        OUT_KEYS.resize(i);
        for (size_t j = 0; j < i; j++) {
            OUT_KEYS[j] = std::to_string(dist(engine)) + "FAKE";
        }
        return OUT_KEYS;
    }

  public:
#define _MACRO_MAKE_CONSTRUCTOR_(classtype)                                    \
    inline TYPE_SELF &operator[](classtype const inkey) {                      \
        std::hash<classtype> hasher;                                           \
        MAIN_KEY = hasher(inkey);                                              \
        return this[0];                                                        \
    }

    _MACRO_MAKE_CONSTRUCTOR_(std::string);
    _MACRO_MAKE_CONSTRUCTOR_(char);
    _MACRO_MAKE_CONSTRUCTOR_(int);
    _MACRO_MAKE_CONSTRUCTOR_(long);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned char);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned int);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned long);
#undef _MACRO_MAKE_CONSTRUCTOR_

  public:
#define _MACRO_MAKE_CONSTRUCTOR_(classtype)                                    \
    _MACRO_CLASS_NAME_(classtype const inkey) {                                \
        std::hash<classtype> hasher;                                           \
        MAIN_KEY = hasher(inkey);                                              \
    }

    _MACRO_MAKE_CONSTRUCTOR_(std::string);
    _MACRO_MAKE_CONSTRUCTOR_(char);
    _MACRO_MAKE_CONSTRUCTOR_(int);
    _MACRO_MAKE_CONSTRUCTOR_(long);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned char);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned int);
    _MACRO_MAKE_CONSTRUCTOR_(unsigned long);
#undef _MACRO_MAKE_CONSTRUCTOR_

    _MACRO_CLASS_NAME_() { MAIN_KEY = 0; }

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
