#ifndef _HEADER_GUARD_StaticArray_D1_
#define _HEADER_GUARD_StaticArray_D1_

////////////////////
// Headers BEGIN: //
////////////////////
#include "../CPPFileIO.hh"
#include "./ImportantHeads.hh"
#include "./Nothing.hh"
//////////////////
// Headers END. //
//////////////////

/////////////////////////
// D1_OPERATORS BEGIN: //
/////////////////////////

namespace D1_OPERATORS {

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_DECLARE_HEADER_                                                 \
    template <typename T1 = Nothing, typename T2 = Nothing> class
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_INHERIT_OPERATOR_                                               \
    using TYPE_DATA = typename T1::TYPE_DATA;                                  \
    T1 const &t1;                                                              \
    T2 const &t2;
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_DEFINE_OPERATION_                                               \
    inline TYPE_DATA const operator[](size_t const i) const
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_DEFINE_CONSTRUCTOR_(ClassName)                                  \
                                                                               \
    ClassName(T1 const &_t1, T2 const &_t2) : t1(_t1), t2(_t2) {}              \
                                                                               \
    ~ClassName() {}                                                            \
                                                                               \
    template <typename S1, typename S2>                                        \
    static inline ClassName<S1, S2> GET(S1 const &s1, S2 const &s2) {          \
        return ClassName<S1, S2>(s1, s2);                                      \
    }
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_DECLARE_A_A_(ClassName, Sign)                                   \
                                                                               \
    _MACRO_DECLARE_HEADER_                                                     \
    ClassName{public : _MACRO_INHERIT_OPERATOR_ _MACRO_DEFINE_OPERATION_{      \
      return t1[i] Sign t2[i];                                                 \
    }                                                                          \
                                                                               \
    _MACRO_DEFINE_CONSTRUCTOR_(ClassName)                                      \
    }                                                                          \
    ;
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_DECLARE_A_S_(ClassName, Sign)                                   \
                                                                               \
    _MACRO_DECLARE_HEADER_                                                     \
    ClassName{public : _MACRO_INHERIT_OPERATOR_ _MACRO_DEFINE_OPERATION_{      \
      return t1[i] Sign t2;                                                    \
    }                                                                          \
                                                                               \
    _MACRO_DEFINE_CONSTRUCTOR_(ClassName)                                      \
    }                                                                          \
    ;
/////////////////////////////////////////////////////////////////////////////////

_MACRO_DECLARE_A_A_(ADD_A_A, +)
_MACRO_DECLARE_A_A_(SUBTRACT_A_A, -)
_MACRO_DECLARE_A_A_(MULTIPLY_A_A, *)
_MACRO_DECLARE_A_A_(DIVIDE_A_A, /)
_MACRO_DECLARE_A_S_(MULTIPLY_A_S, *)
_MACRO_DECLARE_A_S_(DIVIDE_A_S, /)

#undef _MACRO_DECLARE_A_S_
#undef _MACRO_DECLARE_A_A_
#undef _MACRO_DEFINE_CONSTRUCTOR_
#undef _MACRO_INHERIT_OPERATOR_
#undef _MACRO_DEFINE_OPERATION_
#undef _MACRO_DECLARE_HEADER_

} // namespace D1_OPERATORS

///////////////////////
// D1_OPERATORS END. //
///////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_ND_LOOP_(I) for (size_t I = 0; I < SIZE(); I++)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name, Sign)                  \
    inline void Name(const TYPE_DATA &other) {                                 \
        _MACRO_ND_LOOP_(x) { DATA[x] Sign other; }                             \
    }                                                                          \
    inline void operator Sign(const TYPE_DATA other) { Name(other); }
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Name, Sign)                  \
    inline void Name(const TYPE_SELF &other) {                                 \
        _MACRO_ND_LOOP_(x) { DATA[x] Sign other[x]; }                          \
    }                                                                          \
    inline void operator Sign(const TYPE_SELF &other) { Name(other); }         \
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name, Sign)
/////////////////////////////////////////////////////////////////////////////////

//////////////////////
// Main Part BEGIN: //
//////////////////////

#define _MACRO_CLASS_NAME_ ND_ARRAY

template <size_t X = 1, typename T = float> class _MACRO_CLASS_NAME_ {

  public:
    using TYPE_DATA = T;

    inline static size_t constexpr SIZE() {
        return X; //
    }

    inline static size_t constexpr SIZE_1() {
        return SIZE(); //
    }

    using TYPE_SELF = _MACRO_CLASS_NAME_<SIZE(), TYPE_DATA>; //

  public:
    TYPE_DATA DATA[SIZE()];

  public:
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Add, +=)
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Sub, -=)
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Mul, *=)
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Div, /=)
    _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Eqt, =)

    inline TYPE_SELF const operator*(TYPE_DATA const other) const {
        TYPE_SELF ret;
        /* Prepare the return value : */ {
            _MACRO_ND_LOOP_(x) { ret[x] = DATA[x] * other; }
        }
        return ret;
    }

    inline TYPE_SELF const operator/(TYPE_DATA const other) const {
        TYPE_SELF ret;
        /* Prepare the return value : */ {
            _MACRO_ND_LOOP_(x) {
                ret[x] = DATA[x] / other; //
            }
        }
        return ret;
    }

    inline TYPE_DATA Dot(const TYPE_SELF &other) const {
        TYPE_DATA ret = 0;
        /* Prepare the matrix: */ {
            _MACRO_ND_LOOP_(i) { ret += DATA[i] * other[i]; }
        }
        return ret; //
    }

    inline void *CopyFrom(const void *__restrict__ other) {
        return (void *)memcpy((void *)/* dest = */ DATA,
                              (const void *)/* src  = */ other,
                              (size_t) /* n    = */ sizeof(TYPE_SELF));
    }

    inline void *CopyFrom(const TYPE_DATA *__restrict__ other) {
        return CopyFrom((const void *)other);
    }

    inline void *CopyFrom(const TYPE_SELF other) {
        return CopyFrom((const TYPE_DATA *)other.DATA);
    }

    inline TYPE_SELF &FLATTEN() { return this[0]; }

    inline TYPE_SELF const &FLATTEN() const { return this[0]; }

    inline void GET_SQUARED(const TYPE_SELF &other) {
        _MACRO_ND_LOOP_(x) {
            DATA[x] = other[x] * other[x]; //
        }
    }

    inline void ADD_SQUARED(const TYPE_SELF &other) {
        _MACRO_ND_LOOP_(x) { DATA[x] += other[x] * other[x]; }
    }

    inline TYPE_DATA operator*(const TYPE_SELF &other) const {
        return Dot(other);
    }

    _MACRO_CLASS_NAME_() {}

    inline TYPE_DATA &operator[](size_t x) { return DATA[x]; }

    inline TYPE_DATA const &operator[](size_t x) const { return DATA[x]; }

    inline TYPE_DATA *GET_DATA() { return reinterpret_cast<TYPE_DATA *>(DATA); }

    inline TYPE_DATA const *GET_DATA() const {
        return reinterpret_cast<TYPE_DATA const *>(DATA);
    }

    inline TYPE_DATA L1_NORM() const {
        TYPE_DATA Ret = 0;
        /* Prepare the output: */ {
            _MACRO_ND_LOOP_(i) { Ret += CPPFileIO::mymod(DATA[i]); }
        }
        return Ret;
    }

    inline TYPE_DATA L2_NORM() const {
        TYPE_DATA Ret = 0;
        _MACRO_ND_LOOP_(i) {
            Ret += DATA[i] * DATA[i];
        }
        return Ret;
    }

    inline TYPE_SELF operator+(TYPE_SELF const &other) const {
        TYPE_SELF ret;
        _MACRO_ND_LOOP_(i) { ret[i] = DATA[i] + other[i]; }
        return ret;
    }

    inline TYPE_SELF operator-(TYPE_SELF const &other) const {
        TYPE_SELF ret;
        _MACRO_ND_LOOP_(i) { ret[i] = DATA[i] - other[i]; }
        return ret;
    }

};

////////////////////
// Main Part END. //
////////////////////

///////////////////////////////////////////////////
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_ //
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_ //
#undef _MACRO_ND_LOOP_                           //
///////////////////////////////////////////////////

#undef _MACRO_CLASS_NAME_

#endif
