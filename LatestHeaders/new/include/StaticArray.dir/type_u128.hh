#ifndef _HEADER_GUARD_StaticArray_type_u128_
#define _HEADER_GUARD_StaticArray_type_u128_

#include "./D1.hh"

using CPPFileIO::TYPE_U8;
using CPPFileIO::TYPE_U16;
using CPPFileIO::TYPE_U32;
using CPPFileIO::TYPE_U64;
using CPPFileIO::TYPE_BYTE;

#define _MACRO_CLASS_NAME_ TYPE_U128
class _MACRO_CLASS_NAME_ : public ND_ARRAY<2, TYPE_U64> {
  public:
    using TYPE_SELF = _MACRO_CLASS_NAME_;

    inline bool operator > (TYPE_SELF const & in) const {
        if (this[0][1]!=in[1]) { return (this[0][1]>in[1]) ; }
        else { return (this[0][0]>in[0]) ; }
    }

    inline bool operator < (TYPE_SELF const & in) const {
        if (this[0][1]!=in[1]) { return (this[0][1]<in[1]) ; }
        else { return (this[0][0]<in[0]) ; }
    }

    inline bool operator == (TYPE_SELF const & in) const {
        return (this[0][0]==in[0]) && (this[0][1]==in[1]) ;
    }
};
#undef _MACRO_CLASS_NAME_

#endif
