#ifndef _HEADER_GUARD_CPPFileIO_Z_Curve_Digits_
#define _HEADER_GUARD_CPPFileIO_Z_Curve_Digits_

#include "./Basic.hh"

///////////////////////////
// 8-bit intiger BEGIN:{ //
///////////////////////////
inline TYPE_U16 constexpr get_digit_slow_8(TYPE_U8 const in) {
    TYPE_U16 ret = (in & 1);
    ret |= (static_cast<TYPE_U16>(in & 2) << 1);
    ret |= (static_cast<TYPE_U16>(in & 4) << 2);
    ret |= (static_cast<TYPE_U16>(in & 8) << 3);
    ret |= (static_cast<TYPE_U16>(in & 16) << 4);
    ret |= (static_cast<TYPE_U16>(in & 32) << 5);
    ret |= (static_cast<TYPE_U16>(in & 64) << 6);
    ret |= (static_cast<TYPE_U16>(in & 128) << 7);
    return ret;
}

inline void print_data_8() {
    FILE *f = fopen("./data_8.hh", "w");
    for (TYPE_U16 i = 0; i <= 0xFF; i++) {
        fprintf(f, "case %d : return %d;\n", i, get_digit_slow_8(i));
    }
    fclose(f);
}

inline TYPE_U16 constexpr get_digit_fast_8(TYPE_U8 const in) {
    switch (in) {
#include "./data_8.hh"
    }
    return 0;
}

inline TYPE_U16 constexpr get_z_curve_8(TYPE_U8 const a1, TYPE_U8 const a2) {
    TYPE_U16 const b1 = get_digit_fast_8(a1);
    TYPE_U16 const b2 = get_digit_fast_8(a2);
    return b1 | (b2 << 1);
}
/////////////////////////
// 8-bit intiger END.} //
/////////////////////////

////////////////////////////
// 16-big intiger BEGIN:{ //
////////////////////////////
inline TYPE_U32 constexpr get_digit_slow_16(TYPE_U16 const in) {
    TYPE_U8 const  a1 = in & 0xFF;
    TYPE_U8 const  a2 = (in >> 8);
    TYPE_U32 const b1 = get_digit_fast_8(a1);
    TYPE_U32 const b2 = get_digit_fast_8(a2);
    return b1 | (b2 << 16);
}

inline void print_data_16() {
    FILE *f = fopen("./data_16.hh", "w");
    for (TYPE_U32 i = 0; i <= 0xFFFF; i++) {
        fprintf(f, "case %d : return %d;\n", i, get_digit_slow_16(i));
    }
    fclose(f);
}

inline TYPE_U32 constexpr get_digit_fast_16(TYPE_U16 const in) {
    switch (in) {
#include "./data_16.hh"
    }
    return 0;
}

inline TYPE_U32 constexpr get_z_curve_16(TYPE_U16 const a1, TYPE_U16 const a2) {
    TYPE_U32 const b1 = get_digit_fast_16(a1);
    TYPE_U32 const b2 = get_digit_fast_16(a2);
    return b1 | (b2 << 1);
}
//////////////////////////
// 16-big intiger END.} //
//////////////////////////

////////////////////////////
// 32-bit intiger BEGIN:{ //
////////////////////////////
inline TYPE_U64 constexpr get_digit_slow_32(TYPE_U32 const in) {
    TYPE_U16 const a1 = in & 0xFFFF;
    TYPE_U16 const a2 = in >> 16;
    TYPE_U64 const b1 = get_digit_fast_16(a1);
    TYPE_U64 const b2 = get_digit_fast_16(a2);
    return b1 | (b2 << 32);
}

inline TYPE_U64 constexpr get_z_curve_32(TYPE_U32 const a1, TYPE_U32 const a2) {
    TYPE_U64 const b1 = get_digit_slow_32(a1);
    TYPE_U64 const b2 = get_digit_slow_32(a2);
    return b1 | (b2 << 1);
}
//////////////////////////
// 32-bit intiger END.} //
//////////////////////////

/////////////////////////////
// Wrapper for all BEGIN:{ //
/////////////////////////////
inline TYPE_U16 constexpr get_z_curve(TYPE_U8 const a1, TYPE_U8 const a2) {
    return get_z_curve_8(a1, a2);
}

inline TYPE_U32 constexpr get_z_curve(TYPE_U16 const a1, TYPE_U16 const a2) {
    return get_z_curve_16(a1, a2);
}

inline TYPE_U64 constexpr get_z_curve(TYPE_U32 const a1, TYPE_U32 const a2) {
    return get_z_curve_32(a1, a2);
}
///////////////////////////
// Wrapper for all END.} //
///////////////////////////

#endif
