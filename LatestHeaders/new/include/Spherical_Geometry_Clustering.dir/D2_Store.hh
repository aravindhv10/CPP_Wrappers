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

template <typename TF, typename TI, TI LEAF_LENGTH> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_POINT      = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;

    static TYPE_INT constexpr LENGTH_LIMIT() { return LEAF_LENGTH; }
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT, LENGTH_LIMIT()>;

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

template <typename TF, typename TI, TI LEAF_LENGTH = 16>
class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT      = TF;
    using TYPE_INT        = TI;
    using TYPE_INTS       = std::vector<TYPE_INT>;
    using TYPE_POINT      = D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;
    using TYPE_ELEMENT    = D2GPS_Element<TYPE_FLOAT, TYPE_INT>;

    static TYPE_INT constexpr LENGTH_LIMIT() { return LEAF_LENGTH; }
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT, LENGTH_LIMIT()>;
    using TYPE_NODE = D2GPS_Node<TYPE_FLOAT, TYPE_INT, LENGTH_LIMIT()>;

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

  public:
    inline void DEBUG() {
        size_t const limit_HEAP = HEAP();
        for (size_t i = 0; i < limit_HEAP; i++) {
            printf("HEAP (%lf, %lf) (%lf, %lf) : %ld, %ld\n",
                   HEAP(i).BBOX.MIN.latitude, HEAP(i).BBOX.MIN.longitude,
                   HEAP(i).BBOX.MAX.latitude, HEAP(i).BBOX.MAX.longitude,
                   HEAP(i).RANGE[0], HEAP(i).RANGE[1]);
        }
        size_t const limit_STORE = STORE();
        for (size_t i = 0; i < limit_STORE; i++) {
            printf("STORE: %ld %lu (%lf, %lf)\n", STORE(i).INDEX,
                   STORE(i).Z_CURVE, STORE(i).POINT.latitude,
                   STORE(i).POINT.longitude);
        }
    }

  private:
    inline void MAKE_HEAP_OLD(TYPE_NODE &    parent_node,
                              TYPE_INT const index_node) {
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
            MAKE_HEAP_OLD(left, index_left);
            MAKE_HEAP_OLD(right, index_right);
            parent_node.BBOX = left.BBOX + right.BBOX;
        }
        HEAP(index_node) = parent_node;
        MAX_HEAP_SIZE    = CPPFileIO::mymax(MAX_HEAP_SIZE, index_node);
    }

    inline void MAKE_HEAP_OLD() {
        HEAP.size(0);
        MAX_HEAP_SIZE = 0;
        HEAP.writeable(true);
        TYPE_NODE node;
        node.RANGE[0] = 0;
        node.RANGE[1] = STORE() - 1;
        MAKE_HEAP_OLD(node, 0);
        HEAP.size(MAX_HEAP_SIZE);
        HEAP.writeable(false);
    }

    inline void MAKE_HEAP() {
        HEAP.size(0);
        MAX_HEAP_SIZE = 0;
        HEAP.writeable(true);

        std::stack<TYPE_INT> stack;
        /* Prepare the 1st element of the HEAP: */ {
            TYPE_NODE &element = HEAP(0);
            element.RANGE[0]   = 0;
            element.RANGE[1]   = STORE() - 1;
            stack.push(0);
        }

        while (!stack.empty()) {
            TYPE_INT const the_index = stack.top();
            stack.pop();
            MAX_HEAP_SIZE      = CPPFileIO::mymax(MAX_HEAP_SIZE, the_index);
            TYPE_NODE &element = HEAP(the_index);
            if (element.IS_LEAF()) {
                TYPE_INT const      start  = element.RANGE[0];
                TYPE_INT const      end    = element.RANGE[1];
                TYPE_INT const      length = end - start + 1;
                TYPE_ELEMENT const *buffer = &(STORE(start, length));
                element.BBOX               = buffer[0].POINT;
                for (TYPE_INT i = 1; i < length; i++) {
                    element.BBOX += buffer[i].POINT;
                }
            } else {
                TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(the_index);
                TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(the_index);
                stack.push(index_left_child);
                stack.push(index_right_child);

                TYPE_INT const mid = (element.RANGE[0] + element.RANGE[1]) / 2;

                TYPE_NODE left_child, right_child;
                /* Assign the 2 children nodes: */ {
                    left_child.RANGE[0] = element.RANGE[0];
                    left_child.RANGE[1] = mid;

                    right_child.RANGE[0] = mid + 1;
                    right_child.RANGE[1] = element.RANGE[1];
                }

                HEAP(index_left_child)  = left_child;
                HEAP(index_right_child) = right_child;
            }
        }

        for (TYPE_INT i = MAX_HEAP_SIZE; i >= 0; i--) {
            TYPE_NODE element = HEAP(i);
            if (!element.IS_LEAF()) {
                TYPE_NODE const *children = &(HEAP(INDEX_LEFT_CHILD(i), 2));
                element.BBOX              = children[0].BBOX + children[1].BBOX;
                HEAP(i) = element;
            }
        }

        HEAP.size(MAX_HEAP_SIZE + 1);
        HEAP.writeable(false);
    }

    inline void RETRIEVE_ELEMENTS(TYPE_INTS &indices, TYPE_BOX const &in) {
        std::stack<TYPE_INT> stack;
        stack.push(0);
        while (!stack.empty()) {
            TYPE_INT const index = stack.top();
            stack.pop();
            TYPE_NODE const element    = HEAP(index);
            bool const      intersects = element.BBOX(in);
            if (intersects) {
                if (element.IS_LEAF()) {
                    TYPE_INT const      start  = element.RANGE[0];
                    TYPE_INT const      end    = element.RANGE[1];
                    TYPE_INT const      length = end - start + 1;
                    TYPE_ELEMENT const *buffer = &(STORE(start, length));
                    for (TYPE_INT i = 0; i < length; i++) {
                        bool const toinclude = in(buffer[i].POINT);
                        if (toinclude) { indices.push_back(buffer[i].INDEX); }
                    }
                } else {
                    TYPE_INT const leftchild = INDEX_LEFT_CHILD(index);
                    stack.push(leftchild);
                    stack.push(leftchild + 1);
                }
            }
        }
        /* DEBUGGING */ if (false) {
            TYPE_INT const limit_store = STORE();
            TYPE_INTS      checking;
            for (TYPE_INT i = 0; i < limit_store; i++) {
                bool const intersects = in(STORE(i).POINT);
                if (intersects) { checking.push_back(STORE(i).INDEX); }
            }
            if (indices.size() > 0) {
                std::sort(indices.begin(), indices.end());
            }
            if (checking.size() > 0) {
                std::sort(checking.begin(), checking.end());
            }
            if (checking.size() == indices.size()) {
                for (size_t i = 0; i < indices.size(); i++) {
                    printf("CHECK INTERNAL...: %ld %ld\n", indices[i],
                           checking[i]);
                }
            } else {
                printf("RESULTS ARE WRONG!!!\n");
            }
        }
    }

    inline void RETRIEVE_ELEMENTS_OLD(TYPE_INTS &indices, TYPE_BOX const &in,
                                      TYPE_INT const i) {

        auto const &heap = HEAP(i);
        bool const  ret  = heap.BBOX(in);

        printf("1\n");
        if (ret) {
            if (HEAP(i).IS_LEAF()) {
                printf("2\n");
                TYPE_INT const start  = heap.RANGE[0];
                TYPE_INT const end    = heap.RANGE[1];
                TYPE_INT const length = end - start + 1;
                printf("DEBUG: %ld %ld %ld %ld\n", start, end, length, STORE());
                TYPE_ELEMENT const *buffer = &(STORE(start, length));
                printf("2.1\n");
                for (TYPE_INT j = 0; j < length; j++) {
                    printf("DEBUG j = %ld\n", j);
                    bool const res = in(buffer[j].POINT);
                    printf("2.2\n");
                    if (res) {
                        printf("2.3\n");
                        printf("adding %ld (%lf, %lf)\n", buffer[j].INDEX,
                               buffer[j].POINT.latitude,
                               buffer[i].POINT.longitude);
                        printf("2.4\n");
                        indices.push_back(buffer[j].INDEX);
                    }
                    printf("2.4\n");
                }
            } else {
                printf("3\n");
                RETRIEVE_ELEMENTS_OLD(indices, in, INDEX_LEFT_CHILD(i));
                RETRIEVE_ELEMENTS_OLD(indices, in, INDEX_RIGHT_CHILD(i));
                printf("4\n");
            }
        }
    }

  public:
    template <typename Reader> inline void MAKE_STORE(Reader &reader) {
        TYPE_INT const limit = reader();

        STORE.size(0);
        STORE.writeable(true);
        TYPE_ELEMENT *buffer = &(STORE(0, limit));

        for (TYPE_INT i = 0; i < limit; i++) {
            TYPE_POINT const place = reader(i);
            TYPE_ELEMENT     tmp;
            tmp.POINT   = place;
            tmp.Z_CURVE = tmp.POINT.z_curve();
            tmp.INDEX   = i;
            buffer[i]   = tmp;
        }
        std::sort(&(buffer[0]), &(buffer[limit]));
        STORE.size(limit);
        STORE.writeable(false);
        MAKE_HEAP();
    }

    inline void operator()(TYPE_INTS &indices, TYPE_BOX const &in) {
        indices.clear();
        RETRIEVE_ELEMENTS(indices, in);
    }

    inline void operator()(TYPE_INTS &indices, TYPE_FLOAT const lat1,
                           TYPE_FLOAT const lon1, TYPE_FLOAT const lat2,
                           TYPE_FLOAT const lon2) {

        TYPE_BOX inbox;
        inbox.MIN.latitude  = CPPFileIO::mymin(lat1, lat2);
        inbox.MIN.longitude = CPPFileIO::mymin(lon1, lon2);
        inbox.MAX.latitude  = CPPFileIO::mymax(lat1, lat2);
        inbox.MAX.longitude = CPPFileIO::mymax(lon1, lon2);
        RETRIEVE_ELEMENTS(indices, inbox);
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
