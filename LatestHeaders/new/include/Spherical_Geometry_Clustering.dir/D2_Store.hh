#ifndef _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_
#define _HEADER_GUARD_Spherical_Geometry_Clustering_D2_Store_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "../Read_Show_Functions.hh"
#include "./Box.hh"
#include "./PolarCoordinates.hh"
///////////////////
// Headers END.} //
///////////////////

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
    inline void Eval_Z_Curve() { Z_CURVE = POINT.z_curve(); }

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
    using TYPE_POINTS     = std::vector<TYPE_POINT>;
    using TYPE_BOX        = D2GPS_Box<TYPE_FLOAT, TYPE_INT>;
    using TYPE_PAIR_INT   = StaticArray::ND_ARRAY<2, TYPE_INT>;
    using TYPE_PAIR_FLOAT = StaticArray::ND_ARRAY<2, TYPE_FLOAT>;
    using TYPE_ELEMENT    = D2GPS_Element<TYPE_FLOAT, TYPE_INT>;
    using TYPE_ELEMENTS   = std::vector<TYPE_ELEMENT>;

    static TYPE_INT constexpr LENGTH_LIMIT() { return LEAF_LENGTH; }
    using TYPE_SELF = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT, LENGTH_LIMIT()>;
    using TYPE_NODE = D2GPS_Node<TYPE_FLOAT, TYPE_INT, LENGTH_LIMIT()>;

    static inline TYPE_INT constexpr INDEX_PARENT(TYPE_INT const i) {
        return (i - 1) / 2;
    }

    static inline TYPE_INT constexpr INDEX_LEFT_CHILD(TYPE_INT const i) {
        return (2 * i) + 1;
    }

    static inline TYPE_INT constexpr INDEX_RIGHT_CHILD(TYPE_INT const i) {
        return (2 * i) + 2;
    }

  private:
    std::string const                  DIRNAME;
    CPPFileIO::FileArray<TYPE_ELEMENT> STORE;
    CPPFileIO::FileArray<TYPE_NODE>    HEAP;
    TYPE_INT                           MAX_HEAP_SIZE;

  private:
    bool                READ_STORE;
    TYPE_INT            LIMIT_STORE;
    TYPE_ELEMENT const *PTR_STORE;

  public:
    inline void read_all_store() {
        READ_STORE  = true;
        LIMIT_STORE = STORE();
        PTR_STORE   = &(STORE(0, LIMIT_STORE));
    }

  private:
    bool             READ_HEAP;
    TYPE_INT         LIMIT_HEAP;
    TYPE_NODE const *PTR_HEAP;

  public:
    inline void read_all_heap() {
        READ_HEAP  = true;
        LIMIT_HEAP = HEAP();
        PTR_HEAP   = &(HEAP(0, LIMIT_HEAP));
    }

    inline TYPE_NODE *read_small_heap(TYPE_INT const in) {
        READ_HEAP = false;
        if (in > 0) {
            LIMIT_HEAP     = in;
            TYPE_NODE *ret = &(HEAP(0, in));
            PTR_HEAP       = ret;
            return ret;
        } else {
            LIMIT_HEAP = 0;
            HEAP.size(0);
            return NULL;
        }
    }

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
    inline TYPE_BOX GET_B_BOX(TYPE_INT const start, TYPE_INT const end) {
        TYPE_BOX ret;
        if (READ_STORE) {
            ret = PTR_STORE[start].POINT;
            for (TYPE_INT i = start + 1; i <= end; i++) {
                ret += PTR_STORE[i].POINT;
            }
            return ret;
        } else {
            TYPE_INT const length = end - start + 1;
            CPPFileIO::Dynamic1DArray<TYPE_ELEMENT, TYPE_INT> store(length);
            STORE(start, store);
            ret = store(0).POINT;
            for (TYPE_INT i = 1; i < length; i++) { ret += store(i).POINT; }
            return ret;
        }
    }

    inline TYPE_INT PREFERRED_HEAP_SIZE() {
        TYPE_INT calc =
          static_cast<TYPE_INT>(std::log2(STORE() / (LEAF_LENGTH / 2)));
        calc = 2 << (calc);
        return calc;
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

    inline void DEBUG(TYPE_BOX const &in) {
        size_t const limit_STORE = STORE();
        for (size_t i = 0; i < limit_STORE; i++) {
            auto const &element = STORE(i);
            bool const  ret     = in(element.POINT);
            if (ret) {
                printf("DEBUG: (%lf, %lf)\n", element.POINT.latitude,
                       element.POINT.longitude);
            }
        }
    }

  private:
    inline void MAKE_HEAP_STRUCTURE_DFS() {
        HEAP.writeable(true);
        read_small_heap(0);
        MAX_HEAP_SIZE = 0;
        HEAP.writeable(true);

        std::stack<TYPE_INT> stack;

        TYPE_NODE *buffer = read_small_heap(
          CPPFileIO::mymax(PREFERRED_HEAP_SIZE(), static_cast<TYPE_INT>(1024)));

        /* for 0: */ {
            TYPE_NODE node;
            node.RANGE[0] = 0;
            node.RANGE[1] = STORE() - 1;
            buffer[0]     = node;

            if (!node.IS_LEAF()) {
                TYPE_INT constexpr index_left_child  = INDEX_LEFT_CHILD(0);
                TYPE_INT constexpr index_right_child = INDEX_RIGHT_CHILD(0);

                stack.push(index_left_child);
                stack.push(index_right_child);

                TYPE_INT const mid = (node.RANGE[0] + node.RANGE[1]) / 2;

                TYPE_NODE left_child, right_child;
                /* Assign the 2 children nodes: */ {
                    left_child.RANGE[0] = node.RANGE[0];
                    left_child.RANGE[1] = mid;

                    right_child.RANGE[0] = mid + 1;
                    right_child.RANGE[1] = node.RANGE[1];
                }

                buffer[index_left_child]  = left_child;
                buffer[index_right_child] = right_child;
            }
        }

        while (!stack.empty()) {
            TYPE_INT const index = stack.top();
            stack.pop();
            MAX_HEAP_SIZE = CPPFileIO::mymax(MAX_HEAP_SIZE, index);

            TYPE_NODE const node = buffer[index];

            if (!node.IS_LEAF()) {
                TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(index);
                TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(index);

                stack.push(index_left_child);
                stack.push(index_right_child);

                TYPE_INT const mid = (node.RANGE[0] + node.RANGE[1]) / 2;

                TYPE_NODE left_child, right_child;
                /* Assign the 2 children nodes: */ {
                    left_child.RANGE[0] = node.RANGE[0];
                    left_child.RANGE[1] = mid;

                    right_child.RANGE[0] = mid + 1;
                    right_child.RANGE[1] = node.RANGE[1];
                }

                auto const tmpmax =
                  CPPFileIO::mymax(index_left_child, index_right_child);

                while (LIMIT_HEAP <= tmpmax) {
                    buffer = read_small_heap(2 * LIMIT_HEAP);
                }

                buffer[index_left_child]  = left_child;
                buffer[index_right_child] = right_child;
            }
        }

        HEAP.size(MAX_HEAP_SIZE + 1);
        HEAP.writeable(false);
        READ_HEAP  = false;
        LIMIT_HEAP = 0;
    }

    inline void MAKE_HEAP_STRUCTURE_BFS() {
        HEAP.writeable(true);
        read_small_heap(0);
        MAX_HEAP_SIZE = 0;
        HEAP.writeable(true);

        std::queue<TYPE_INT> stack;

        TYPE_NODE *buffer = read_small_heap(
          CPPFileIO::mymax(PREFERRED_HEAP_SIZE(), static_cast<TYPE_INT>(1024)));

        /* for 0: */ {
            TYPE_NODE node;
            node.RANGE[0] = 0;
            node.RANGE[1] = STORE() - 1;
            buffer[0]     = node;

            if (!node.IS_LEAF()) {
                TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(0);
                TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(0);

                stack.push(index_left_child);
                stack.push(index_right_child);

                TYPE_INT const mid = (node.RANGE[0] + node.RANGE[1]) / 2;

                TYPE_NODE left_child, right_child;
                /* Assign the 2 children nodes: */ {
                    left_child.RANGE[0] = node.RANGE[0];
                    left_child.RANGE[1] = mid;

                    right_child.RANGE[0] = mid + 1;
                    right_child.RANGE[1] = node.RANGE[1];
                }

                buffer[index_left_child]  = left_child;
                buffer[index_right_child] = right_child;
            }
        }

        while (!stack.empty()) {
            TYPE_INT const index = stack.front();
            stack.pop();
            MAX_HEAP_SIZE = CPPFileIO::mymax(MAX_HEAP_SIZE, index);

            TYPE_NODE const node = buffer[index];

            if (!node.IS_LEAF()) {
                TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(index);
                TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(index);

                stack.push(index_left_child);
                stack.push(index_right_child);

                TYPE_INT const mid = (node.RANGE[0] + node.RANGE[1]) / 2;

                TYPE_NODE left_child, right_child;
                /* Assign the 2 children nodes: */ {
                    left_child.RANGE[0] = node.RANGE[0];
                    left_child.RANGE[1] = mid;

                    right_child.RANGE[0] = mid + 1;
                    right_child.RANGE[1] = node.RANGE[1];
                }

                auto const tmpmax =
                  CPPFileIO::mymax(index_left_child, index_right_child);

                while (LIMIT_HEAP <= tmpmax) {
                    buffer = read_small_heap(2 * LIMIT_HEAP);
                }

                buffer[index_left_child]  = left_child;
                buffer[index_right_child] = right_child;
            }
        }

        HEAP.size(MAX_HEAP_SIZE + 1);
        HEAP.writeable(false);
        READ_HEAP  = false;
        LIMIT_HEAP = 0;
    }

    inline void MAKE_HEAP_STRUCTURE() { MAKE_HEAP_STRUCTURE_DFS(); }

    inline void EVAL_BBOX() {
        TYPE_INT const limit = HEAP();
        HEAP.writeable(true);
        TYPE_NODE *buffer = &(HEAP(0, limit));

        read_all_store();

#pragma omp parallel for
        for (TYPE_INT i = 0; i < limit; i++) {
            if (buffer[i].IS_LEAF()) {
                buffer[i].BBOX =
                  GET_B_BOX(buffer[i].RANGE[0], buffer[i].RANGE[1]);
            }
        }

        for (TYPE_INT i = limit - 1; i >= 0; i--) {
            if (!buffer[i].IS_LEAF()) {
                TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(i);
                TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(i);
                buffer[i].BBOX = buffer[index_left_child].BBOX +
                                 buffer[index_right_child].BBOX;
            }
        }

        HEAP.writeable(false);
        HEAP.size(limit);
        READ_HEAP = false;
    }

    inline TYPE_INT MAKE_HEAP_STACK(TYPE_INT const index) {

        CPPFileIO::FileArray<TYPE_ELEMENT> store(NAME_STORE());

        CPPFileIO::FileArray<TYPE_NODE> heap(NAME_HEAP());
        if (index == 0) { heap.size(0); }
        heap.writeable(true);

        TYPE_INT max_heap_size = index;

        std::stack<TYPE_INT> stack;
        stack.push(index);

        TYPE_INTS toprocess;

        while (!stack.empty()) {

            TYPE_INT const the_index = stack.top();
            stack.pop();
            max_heap_size = CPPFileIO::mymax(max_heap_size, the_index);

            TYPE_NODE element;
            if (the_index == 0) {
                element.RANGE[0] = 0;
                element.RANGE[1] = store() - 1;
                heap(0)          = element;
            } else {
                element = heap(the_index);
            }

            if (element.IS_LEAF()) {

                TYPE_INT const      start  = element.RANGE[0];
                TYPE_INT const      end    = element.RANGE[1];
                TYPE_INT const      length = end - start + 1;
                TYPE_ELEMENT const *buffer = &(store(start, length));

                element.BBOX = buffer[0].POINT;
                for (TYPE_INT i = 1; i < length; i++) {
                    element.BBOX += buffer[i].POINT;
                }

                heap(the_index) = element;

            } else {

                toprocess.push_back(the_index);

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

                heap(index_left_child)  = left_child;
                heap(index_right_child) = right_child;
            }
        }

        // if(toprocess.size()>1){
        //     std::sort(toprocess.begin(), toprocess.end());
        // }

        for (TYPE_INT i = toprocess.size() - 1; i >= 0; i--) {
            TYPE_INT const  the_index         = toprocess[i];
            TYPE_INT const  index_left_child  = INDEX_LEFT_CHILD(the_index);
            TYPE_INT const  index_right_child = INDEX_RIGHT_CHILD(the_index);
            TYPE_NODE const left_child        = heap(index_left_child);
            TYPE_NODE const right_child       = heap(index_right_child);
            heap(the_index).BBOX = left_child.BBOX + right_child.BBOX;
        }

        if (index == 0) {
            heap.size(max_heap_size + 1);
            heap.writeable(false);
        }

        return max_heap_size;
    }

    inline TYPE_INT MAKE_HEAP_RECURSIVE(TYPE_INT const index,
                                        TYPE_INT const nthreads) {

        CPPFileIO::FileArray<TYPE_NODE> heap(NAME_HEAP());
        if (index == 0) { heap.size(0); }

        TYPE_NODE element;
        if (index == 0) {
            element.RANGE[0] = 0;
            element.RANGE[1] = STORE() - 1;
        } else {
            element = heap(index);
        }

        heap.writeable(true);

        if (element.IS_LEAF()) {

            CPPFileIO::FileArray<TYPE_ELEMENT> store(NAME_STORE());

            TYPE_INT const      start  = element.RANGE[0];
            TYPE_INT const      end    = element.RANGE[1];
            TYPE_INT const      length = end - start + 1;
            TYPE_ELEMENT const *buffer = &(store(start, length));

            element.BBOX = buffer[0].POINT;
            for (TYPE_INT i = 1; i < length; i++) {
                element.BBOX += buffer[i].POINT;
            }

            heap(index) = element;
            if (index == 0) { heap.size(1); }
            heap.writeable(false);

            return index;
        }

        TYPE_INT const index_left_child  = INDEX_LEFT_CHILD(index);
        TYPE_INT const index_right_child = INDEX_RIGHT_CHILD(index);

        /* Assign the 2 children nodes: */ {
            TYPE_INT const mid = (element.RANGE[0] + element.RANGE[1]) / 2;
            TYPE_NODE      left_child, right_child;

            left_child.RANGE[0]     = element.RANGE[0];
            left_child.RANGE[1]     = mid;
            right_child.RANGE[0]    = mid + 1;
            right_child.RANGE[1]    = element.RANGE[1];
            heap(index_left_child)  = left_child;
            heap(index_right_child) = right_child;
        }

        TYPE_INT sizes[2];
        if (nthreads > 1) {
            TYPE_INT const newthreads = nthreads >> 1;
#pragma omp parallel for
            for (size_t i = 0; i < 2; i++) {
                if (i == 0) {
                    sizes[0] =
                      MAKE_HEAP_RECURSIVE(index_left_child, newthreads);
                } else {
                    sizes[1] =
                      MAKE_HEAP_RECURSIVE(index_right_child, newthreads);
                }
            }
        } else {
            for (size_t i = 0; i < 2; i++) {
                if (i == 0) {
                    sizes[0] = MAKE_HEAP_STACK(index_left_child);
                } else {
                    sizes[1] = MAKE_HEAP_STACK(index_right_child);
                }
            }
        }
        auto const max_heap_size = CPPFileIO::mymax(sizes[0], sizes[1]);

        /* Assign the 2 children nodes: */ {
            TYPE_NODE const left_child  = heap(index_left_child);
            TYPE_NODE const right_child = heap(index_right_child);
            heap(index).BBOX            = left_child.BBOX + right_child.BBOX;
        }

        if (index == 0) { heap.size(max_heap_size + 1); }
        heap.writeable(false);

        return max_heap_size;
    }

    inline void MAKE_HEAP(TYPE_INT nthreads = 4) {
        MAKE_HEAP_STRUCTURE();
        EVAL_BBOX();
        // MAKE_HEAP_RECURSIVE(0, nthreads);
        // MAKE_HEAP_STACK(0);
    }

  private:
    template <typename Type>
    inline void RETRIEVE_ELEMENTS(Type &places, TYPE_BOX const &in) {
        std::stack<TYPE_INT> stack;
        stack.push(0);
        while (!stack.empty()) {
            TYPE_INT const index = stack.top();
            stack.pop();
            TYPE_NODE const *element;
            /* Read in the element */ {
                if (READ_HEAP) {
                    element = &(PTR_HEAP[index]);
                } else {
                    element = &(HEAP(index));
                }
            }
            bool const intersects = element->BBOX(in);
            if (intersects) {
                if (element->IS_LEAF()) {
                    TYPE_INT const      start  = element->RANGE[0];
                    TYPE_INT const      end    = element->RANGE[1];
                    TYPE_INT const      length = end - start + 1;
                    TYPE_ELEMENT const *buffer;
                    if (READ_STORE) {
                        buffer = &(PTR_STORE[start]);
                    } else {
                        buffer = &(STORE(start, length));
                    }
                    for (TYPE_INT i = 0; i < length; i++) {
                        bool const toinclude = in(buffer[i].POINT);
                        if (toinclude) { places(buffer[i]); }
                    }
                } else {
                    TYPE_INT const leftchild = INDEX_LEFT_CHILD(index);
                    stack.push(leftchild);
                    stack.push(leftchild + 1);
                }
            }
        }
    }

  public:
    template <typename Type>
    inline void operator()(Type &indices, TYPE_BOX const &in) {
        RETRIEVE_ELEMENTS(indices, in);
        // DEBUG(in);
    }

    template <typename Type>
    inline void operator()(Type &indices, TYPE_FLOAT const lat1,
                           TYPE_FLOAT const lon1, TYPE_FLOAT const lat2,
                           TYPE_FLOAT const lon2) {
        TYPE_BOX inbox;
        inbox.MIN.latitude  = CPPFileIO::mymin(lat1, lat2);
        inbox.MIN.longitude = CPPFileIO::mymin(lon1, lon2);
        inbox.MAX.latitude  = CPPFileIO::mymax(lat1, lat2);
        inbox.MAX.longitude = CPPFileIO::mymax(lon1, lon2);
        this[0](indices, inbox);
    }

#define _MACRO_INTERFACE_(Type, PushStatement)                                 \
    inline void operator()(Type &indices, TYPE_BOX const &in) {                \
        indices.clear();                                                       \
        struct reader {                                                        \
            Type *      ref;                                                   \
            inline void operator()(TYPE_ELEMENT const &in) {                   \
                ref->push_back(PushStatement);                                 \
            }                                                                  \
        };                                                                     \
        reader tmp;                                                            \
        tmp.ref = &indices;                                                    \
        this[0](tmp, in);                                                      \
    }                                                                          \
    inline void operator()(Type &indices, TYPE_FLOAT const lat1,               \
                           TYPE_FLOAT const lon1, TYPE_FLOAT const lat2,       \
                           TYPE_FLOAT const lon2) {                            \
        indices.clear();                                                       \
        TYPE_BOX inbox;                                                        \
        inbox.MIN.latitude  = CPPFileIO::mymin(lat1, lat2);                    \
        inbox.MIN.longitude = CPPFileIO::mymin(lon1, lon2);                    \
        inbox.MAX.latitude  = CPPFileIO::mymax(lat1, lat2);                    \
        inbox.MAX.longitude = CPPFileIO::mymax(lon1, lon2);                    \
        this[0](indices, inbox);                                               \
    }

    _MACRO_INTERFACE_(TYPE_INTS, in.INDEX)
    _MACRO_INTERFACE_(TYPE_POINTS, in.POINT)
    _MACRO_INTERFACE_(TYPE_ELEMENTS, in)

#undef _MACRO_INTERFACE_

  public:
    template <typename Reader>
    inline void MAKE_STORE(Reader &reader, TYPE_INT nthreads = 16) {
        TYPE_INT const limit = reader();
        if (true) /* store and sort the elements: */ {
            std::string const tmpfilename = NAME_STORE() + ".tmp";
            std::string const outfilename = NAME_STORE();

            /* prepare the data: */ {
                CPPFileIO::FileWriter<TYPE_ELEMENT> writer(tmpfilename);
                for (TYPE_INT i = 0; i < limit; i++) {
                    TYPE_POINT const place = reader(i);
                    TYPE_ELEMENT     tmp;
                    tmp.POINT = place;
                    // tmp.Z_CURVE = tmp.POINT.z_curve();
                    tmp.INDEX = i;
                    writer(tmp);
                }
            }
            /* Evaluate the z curves: */ {
                CPPFileIO::FileArray<TYPE_ELEMENT> array(tmpfilename);
                size_t const                       limit = array();
                array.writeable(true);
                TYPE_ELEMENT *buffer  = &(array(0, limit));
                size_t constexpr step = (1 << 13);
#pragma omp parallel for
                for (size_t start = 0; start < limit; start += step) {
                    size_t const end = CPPFileIO::mymin(start + step, limit);
                    for (size_t j = start; j < end; j++) {
                        buffer[j].Eval_Z_Curve();
                    }
                }
                array.size(limit);
            }

            CPPFileIO::ExternalMergeSorter<TYPE_ELEMENT, true>::DoAllSteps(
              tmpfilename, outfilename, nthreads, nthreads);

            STORE(NAME_STORE());
            READ_STORE = false;

        } else {
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
        }

        MAKE_HEAP(nthreads);
    }

    inline void read_all() {
        read_all_heap();
        read_all_store();
    }

  public:
    _MACRO_CLASS_NAME_(std::string const dirname)
      : DIRNAME(dirname), STORE(NAME_STORE()), HEAP(NAME_HEAP()),
        READ_STORE(false), READ_HEAP(false) {}

    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_
////////////////////////////////
// The main store class END.} //
////////////////////////////////

#endif
