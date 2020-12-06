#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_

#include "../Read_Show_Functions.hh"
#include "./PolarCoordinates.hh"

/////////////////////////////
// Storage Element BEGIN:{ //
/////////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Element

template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT = TF;
    using TYPE_INT   = TI;
    using TYPE_SELF  = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ULONG = unsigned long;

  public:
    TYPE_POINT POINT;
    TYPE_ULONG Z_CURVE;
    TYPE_INT   INDEX;

  public:
    inline char Compare(TYPE_SELF const &other) const {
        return Read_Show_Functions::Compare(Z_CURVE, other.Z_CURVE);
    }

    inline bool operator>(TYPE_SELF const &other) const {
        return (Compare(other) > 0);
    }

    inline bool operator<(TYPE_SELF const &other) const {
        return (Compare(other) < 0);
    }

    inline bool operator==(TYPE_SELF const &other) const {
        return (Compare(other) == 0);
    }

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_
///////////////////////////
// Storage Element END.} //
///////////////////////////

//////////////////////////
// Storage node BEGIN:{ //
//////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Node

template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT      = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;

    static TYPE_INT constexpr LENGTH_LIMIT() { return 128; }

  public:
    TYPE_BOX      BBOX;
    TYPE_PAIR_INT RANGE;

  public:
    inline TYPE_INT LENGTH() const { return (RANGE[1] - RANGE[0] + 1); }
    inline bool     IS_LEAF() const { return (LENGTH() < LENGTH_LIMIT()); }

  public:
    _MACRO_CLASS_NAME_(TYPE_PAIR_INT const in) {
        RANGE[0] = in[0];
        RANGE[1] = in[1];
    }

    _MACRO_CLASS_NAME_(TYPE_INT const start, TYPE_INT const end) {
        RANGE[0] = start;
        RANGE[1] = end;
    }

    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_
////////////////////////
// Storage node END.} //
////////////////////////

//////////////////////////////////
// The main store class BEGIN:{ //
//////////////////////////////////
#define _MACRO_CLASS_NAME_ D2GPS_Store

template <typename TF, typename TI> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_SELF       = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_POINT      = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;
    using TYPE_NODE       = D2GPS_Node<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENT    = D2GPS_Element<TYPE_FLOAT, TYPE_INT>;

    static inline TYPE_INT INDEX_PARENT(TYPE_INT const i) {
        return (i - 1) / 2;
    }

    static inline TYPE_INT INDEX_LEFT_CHILD(TYPE_INT const i) {
        return (2 * i) + 1;
    }

    static inline TYPE_INT INDEX_RIGHT_CHILD(TYPE_INT const i) {
        return (2 * i) + 2;
    }

  public:
    std::string const                  DIRNAME;
    CPPFileIO::FileArray<TYPE_ELEMENT> STORE;
    CPPFileIO::FileArray<TYPE_NODE>    HEAP;
    TYPE_INT                           MAX_HEAP_SIZE;

  private:
    inline std::string const NAME_DIR() const {
        std::string const ret = DIRNAME + "/";
        mkdir(DIRNAME.c_str(), 0755);
        return ret;
    }

    inline std::string const NAME_STORE() const {
        std::string const ret = NAME_DIR() + "out.store.bin";
        return ret;
    }

    inline std::string const NAME_HEAP() const {
        std::string const ret = NAME_DIR() + "out.heap.bin";
        return ret;
    }

  private:
    inline void MAKE_HEAP(TYPE_NODE &parent_node, TYPE_INT const index_node) {
        if (parent_node.IS_LEAF()) {
            auto const    start  = parent_node.RANGE[0];
            auto const    end    = parent_node.RANGE[1];
            auto const    length = parent_node.LENGTH();
            TYPE_ELEMENT *buffer = &(STORE(start, length));
            parent_node.BBOX     = buffer[0].POINT;
            for (TYPE_INT i = 1; i < length; i++) {
                parent_node.BBOX += buffer[i].POINT;
            }
        } else {
            TYPE_INT const index_left  = INDEX_LEFT_CHILD(index_node);
            TYPE_INT const index_right = INDEX_RIGHT_CHILD(index_node);
            TYPE_INT const mid =
              (parent_node.RANGE[0] + parent_node.RANGE[1]) / 2;
            TYPE_NODE left, right;
            left.RANGE[0]  = parent_node.RANGE[0];
            left.RANGE[1]  = mid;
            right.RANGE[0] = mid + 1;
            right.RANGE[1] = parent_node.RANGE[1];
            MAKE_HEAP(left, index_left);
            MAKE_HEAP(right, index_right);
            parent_node.BBOX = left.BBOX + right.BBOX;
        }
        HEAP(index_node) = parent_node;
        MAX_HEAP_SIZE    = CPPFileIO::mymax(MAX_HEAP_SIZE, index_node);
    }

    inline void MAKE_HEAP() {
        MAX_HEAP_SIZE = 0;
        HEAP.writeable(true);
        TYPE_NODE node;
        node.RANGE[0] = 0;
        node.RANGE[1] = STORE() - 1;
        MAKE_HEAP(node, 0);
        HEAP.writeable(false);
        HEAP.size(MAX_HEAP_SIZE);
    }

  public:
    template <typename Reader> inline void MAKE_STORE(Reader &reader) {
        TYPE_INT const limit = reader();
        STORE.writeable(true);
        for (TYPE_INT i = 0; i < limit; i++) {
            TYPE_POINT const place = reader(i);
            TYPE_ELEMENT     tmp;
            tmp.POINT   = place;
            tmp.Z_CURVE = tmp.POINT.z_curve();
            tmp.INDEX   = i;
            STORE(i)    = tmp;
        }
        STORE.writeable(false);
        STORE.size(limit);
        MAKE_HEAP();
    }

  public:
    _MACRO_CLASS_NAME_(std::string const dirname)
      : DIRNAME(dirname), STORE(NAME_STORE()), HEAP(NAME_HEAP()) {}
    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_
////////////////////////////////
// The main store class END.} //
////////////////////////////////

#endif
