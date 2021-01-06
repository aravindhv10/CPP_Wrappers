#ifndef _HEADER_GUARD_StaticArray_Z_Curves_
#define _HEADER_GUARD_StaticArray_Z_Curves_

#include "./D1.hh"
#include "./type_u128.hh"

#define _MACRO_CLASS_NAME_ Z_Curve_2
class _MACRO_CLASS_NAME_ {
  public:
    static inline TYPE_BYTE constexpr get_digits_2(TYPE_BYTE const i,
                                                   TYPE_BYTE const j) {
        TYPE_BYTE constexpr digits[2][256] = {
          {0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85,
           0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85},
          {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
           1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
           4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
           5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
           16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
           17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
           20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
           21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
           64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
           65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
           68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68,
           69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
           80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
           81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81,
           84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84,
           85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85}};
        return digits[i][j];
    }

    static inline TYPE_U16 constexpr get_digits_2(TYPE_U8 const i) {
        CPPFileIO::TYPE_U16 constexpr digits[256] = {
          0,     1,     4,     5,     16,    17,    20,    21,    64,    65,
          68,    69,    80,    81,    84,    85,    256,   257,   260,   261,
          272,   273,   276,   277,   320,   321,   324,   325,   336,   337,
          340,   341,   1024,  1025,  1028,  1029,  1040,  1041,  1044,  1045,
          1088,  1089,  1092,  1093,  1104,  1105,  1108,  1109,  1280,  1281,
          1284,  1285,  1296,  1297,  1300,  1301,  1344,  1345,  1348,  1349,
          1360,  1361,  1364,  1365,  4096,  4097,  4100,  4101,  4112,  4113,
          4116,  4117,  4160,  4161,  4164,  4165,  4176,  4177,  4180,  4181,
          4352,  4353,  4356,  4357,  4368,  4369,  4372,  4373,  4416,  4417,
          4420,  4421,  4432,  4433,  4436,  4437,  5120,  5121,  5124,  5125,
          5136,  5137,  5140,  5141,  5184,  5185,  5188,  5189,  5200,  5201,
          5204,  5205,  5376,  5377,  5380,  5381,  5392,  5393,  5396,  5397,
          5440,  5441,  5444,  5445,  5456,  5457,  5460,  5461,  16384, 16385,
          16388, 16389, 16400, 16401, 16404, 16405, 16448, 16449, 16452, 16453,
          16464, 16465, 16468, 16469, 16640, 16641, 16644, 16645, 16656, 16657,
          16660, 16661, 16704, 16705, 16708, 16709, 16720, 16721, 16724, 16725,
          17408, 17409, 17412, 17413, 17424, 17425, 17428, 17429, 17472, 17473,
          17476, 17477, 17488, 17489, 17492, 17493, 17664, 17665, 17668, 17669,
          17680, 17681, 17684, 17685, 17728, 17729, 17732, 17733, 17744, 17745,
          17748, 17749, 20480, 20481, 20484, 20485, 20496, 20497, 20500, 20501,
          20544, 20545, 20548, 20549, 20560, 20561, 20564, 20565, 20736, 20737,
          20740, 20741, 20752, 20753, 20756, 20757, 20800, 20801, 20804, 20805,
          20816, 20817, 20820, 20821, 21504, 21505, 21508, 21509, 21520, 21521,
          21524, 21525, 21568, 21569, 21572, 21573, 21584, 21585, 21588, 21589,
          21760, 21761, 21764, 21765, 21776, 21777, 21780, 21781, 21824, 21825,
          21828, 21829, 21840, 21841, 21844, 21845};
        return digits[i];
    }

    static inline TYPE_U32 constexpr get_digits_2(TYPE_U16 const i) {
#include "./Z_Curves_values_16.hh"
        return digits[i];
    }

    static inline TYPE_U64 constexpr get_digits_2(TYPE_U32 const i) {
        TYPE_U16 const a1 = i & 0xFFFF;
        TYPE_U16 const a2 = (i >> 16) & 0xFFFF;
        TYPE_U64 const b1 = get_digits_2(a1);
        TYPE_U64 const b2 = get_digits_2(a2);
        TYPE_U64 const c  = b1 | (b2 << 32);
        return c;
    }

    static inline TYPE_U128 get_digits_2(TYPE_U64 const i) {
        TYPE_U128 ret;
        ret[0] = get_digits_2(static_cast<TYPE_U32>(i & 0xFFFFFFFF));
        ret[1] =
          get_digits_2(static_cast<TYPE_U32>((i & 0xFFFFFFFF00000000) >> 32));
        return ret;
    }

    static inline TYPE_U16 constexpr get_z_curve(TYPE_U8 const a1,
                                                 TYPE_U8 const a2) {
        TYPE_U16 b1 = get_digits_2(a1);
        TYPE_U16 b2 = get_digits_2(a2);
        return b1 | (b2 << 1);
    }

    static inline TYPE_U32 constexpr get_z_curve(TYPE_U16 const a1,
                                                 TYPE_U16 const a2) {
        TYPE_U32 b1 = get_digits_2(a1);
        TYPE_U32 b2 = get_digits_2(a2);
        return b1 | (b2 << 1);
    }

    static inline TYPE_U64 constexpr get_z_curve(TYPE_U32 const a1,
                                                 TYPE_U32 const a2) {
        TYPE_U64 const b1 = get_digits_2(a1);
        TYPE_U64 const b2 = get_digits_2(a2);
        return b1 | (b2 << 1);
    }

    static inline TYPE_U128 get_z_curve(TYPE_U64 const a1, TYPE_U64 const a2) {
        TYPE_U128 const b1 = get_digits_2(a1);
        TYPE_U128 const b2 = get_digits_2(a2);
        TYPE_U128       ret;
        ret[0] = b1[0] | (b2[0] << 1);
        ret[1] = b1[1] | (b2[1] << 1);
        return ret;
    }

  public:
    _MACRO_CLASS_NAME_() {}
    ~_MACRO_CLASS_NAME_() {}
};
#undef _MACRO_CLASS_NAME_

#endif
