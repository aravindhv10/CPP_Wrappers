    namespace Array /* 1 D Array */ {

////////////////////////////////////////////////////////////////
#define _MACRO_ND_LOOP_(I) for(size_t I=0;I<SIZE();I++) ////////
////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name,Sign)   \
        inline void Name ( const TYPE_DATA & other ) {         \
            _MACRO_ND_LOOP_(x)                                 \
            { DATA[x] Sign other ; }                           \
        }                                                      \
        inline void operator Sign ( const TYPE_DATA other )    \
        { Name (other) ; } /////////////////////////////////////
////////////////////////////////////////////////////////////////
#define _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Name,Sign)   \
        inline void Name ( const TYPE_SELF & other ) {         \
            _MACRO_ND_LOOP_(x)                                 \
            { DATA[x] Sign other[x] ; }                        \
        }                                                      \
        inline void operator Sign ( const TYPE_SELF & other )  \
        { Name (other) ; }                                     \
        _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Name,Sign) ///
////////////////////////////////////////////////////////////////

        template <size_t X, typename T=float>
        class ND_ARRAY {
        public:
            using TYPE_DATA = T ;
            //
            inline static size_t constexpr
            SIZE() {
                return
                    X
                ; //
            }
            //
            inline static
            size_t constexpr
            SIZE_1() {
                return
                    SIZE()
                ; //
            }
            //
            using TYPE_SELF =
                ND_ARRAY <
                    SIZE(),
                    TYPE_DATA
                >
            ; //
        private:
            TYPE_DATA
                DATA[SIZE()]
            ; //
        public:
            //
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Add,+=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Sub,-=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Mul,*=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_(Div,/=)
            _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_(Eqt,=)
            //
            inline TYPE_SELF const
            operator * (
                TYPE_DATA const
                    other
            ) const {
                TYPE_SELF
                    ret
                ; /* Prepare the return value : */ {
                    _MACRO_ND_LOOP_(x) {
                        ret[x] =
                            DATA[x] *
                            other
                        ; //
                    }
                }
                return
                    ret
                ;
            } //
            inline TYPE_SELF const
            operator / (
                TYPE_DATA const
                    other
            ) const {
                TYPE_SELF
                    ret
                ; /* Prepare the return value : */ {
                    _MACRO_ND_LOOP_(x) {
                        ret[x] =
                            DATA[x] /
                            other
                        ; //
                    }
                }
                return
                    ret
                ;
            } //
            inline TYPE_DATA
            Dot (
                const TYPE_SELF &
                    other
            ) const {
                TYPE_DATA ret =
                    0
                ; /* Prepare the matrix: */ {
                    _MACRO_ND_LOOP_(i) {
                        ret +=
                            DATA[i] *
                            other[i]
                        ;
                    }
                }
                return
                    ret
                ; //
            } //
            inline void * CopyFrom
            ( const void * __restrict__ other ) {
                return (void *) memcpy (
                    (       void * ) /* dest = */ DATA  ,
                    ( const void * ) /* src  = */ other ,
                    (     size_t   ) /* n    = */ sizeof (TYPE_SELF)
                ) ;
            } //
            inline void *
            CopyFrom (
                const TYPE_DATA *
                    __restrict__ other
            ) {
                return CopyFrom (
                    ( const void * )
                        /* other = */ other
                ) ;
            } //
            inline void * CopyFrom
            ( const TYPE_SELF other ) {
                return CopyFrom (
                    ( const TYPE_DATA * )
                        /* other = */ other.DATA
                ) ;
            } //
            //
            inline TYPE_SELF &
            FLATTEN () {
                return
                    this[0]
                ; //
            } //
            inline TYPE_SELF const &
            FLATTEN () const {
                return
                    this[0]
                ; //
            } //
            inline void
            GET_SQUARED (
                const TYPE_SELF &
                    other
            ) {
                _MACRO_ND_LOOP_(x) {
                    DATA[x] =
                        other[x] *
                        other[x]
                    ; //
                }
            } //
            inline void
            ADD_SQUARED (
                const TYPE_SELF &
                    other
            ) {
                _MACRO_ND_LOOP_(x) {
                    DATA[x] +=
                        other[x] *
                        other[x]
                    ; //
                }
            } //
            inline TYPE_DATA
            operator * (
                const TYPE_SELF &
                    other
            ) const {
                return
                    Dot (other)
                ; //
            }
            //
            ND_ARRAY(){}
            //
            inline TYPE_DATA &
            operator [] (size_t x)
            { return DATA[x] ; }
            //
            inline TYPE_DATA const &
            operator [] (size_t x) const
            { return DATA[x] ; }
            //
            inline TYPE_DATA *
            GET_DATA () {
                return
                    reinterpret_cast
                        < TYPE_DATA * >
                        ( DATA )
                ; //
            } //
            inline TYPE_DATA const *
            GET_DATA () const {
                return
                    reinterpret_cast
                        < TYPE_DATA const * >
                        ( DATA )
                ; //
            } //
            inline TYPE_DATA
            L1_NORM () const {
                TYPE_DATA Ret =
                    0
                ; /* Prepare the output: */ {
                    _MACRO_ND_LOOP_(i) {
                        Ret +=
                            CPPFileIO::mymod
                                (DATA[i])
                        ; //
                    }
                }
                return
                    Ret
                ; //
            } //
            inline TYPE_DATA
            L2_NORM () const {
                TYPE_DATA Ret = 0 ;
                _MACRO_ND_LOOP_(i) {
                    Ret +=
                        DATA[i] *
                        DATA[i]
                    ; //
                }
                return Ret ;
            } //
            inline TYPE_SELF
            operator + (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                _MACRO_ND_LOOP_(i) {
                    ret[i] =
                        DATA[i] +
                        other[i]
                    ;
                }
                return ret ;
            } //
            inline TYPE_SELF
            operator - (
                TYPE_SELF const &
                    other
            ) const {
                TYPE_SELF ret ;
                _MACRO_ND_LOOP_(i) {
                    ret[i] =
                        DATA[i] -
                        other[i]
                    ;
                }
                return ret ;
            } //
        } ;

//////////////////////////////////////////////////////
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_SELF_	//
#undef _MACRO_ND_REGISTER_OPERATOR_ON_TYPE_DATA_	//
#undef _MACRO_ND_LOOP_								//
//////////////////////////////////////////////////////

    }
