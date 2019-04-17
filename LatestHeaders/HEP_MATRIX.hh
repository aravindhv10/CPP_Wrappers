#include "./CPPFileIO9.hh"
#include "./qcdloop/main.hh"
////////////////////////////////////////////////////////////////
class Expr {
public:
////////////////////////////////////////////////////////////////
    std::string expression ;
////////////////////////////////////////////////////////////////
    inline void operator =
    (char a) {
        expression =
                a
        ; //
    }
    inline void operator =
    (char const * a) {
        expression =
                std::string(a)
        ; //
    }
    inline void operator =
    (std::string const & name) {
        expression = name ;
    }
    inline void operator =
    ( Expr const & other ) {
        expression = other.expression ;
    }
    inline void operator =
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string(tmp)
        ; //
    }
    inline void operator =
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator +=
    (char a) {
        expression =
                expression +
                std::string("+") +
                a
        ; //
    }
    inline void operator +=
    (char const * a) {
        expression =
                expression +
                std::string("+") +
                std::string(a)
        ; //
    }
    inline void operator +=
    (std::string const & name) {
        expression =
                expression +
                std::string("+") +
                name
        ; //
    }
    inline void operator +=
    ( Expr const & other ) {
        expression =
                expression +
                std::string("+") +
                other.expression
        ; //
    }
    inline void operator +=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
    inline void operator +=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("+") +
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator -=
    (char a) {
        expression =
                expression +
                std::string("-") +
                a
        ; //
    }
    inline void operator -=
    (char const * a) {
        expression =
                expression +
                std::string("-") +
                a
        ; //
    }
    inline void operator -=
    (std::string const & name) {
        expression =
                expression +
                std::string("-") +
                name
        ; //
    }
    inline void operator -=
    ( Expr const & other ) {
        expression =
                expression +
                std::string("-") +
                other.expression
        ; //
    }
    inline void operator -=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
    inline void operator -=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string("-") +
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator *=
    (char a) {
        expression =
                expression +
                std::string(" \\times ")+
                a
        ; //
    }
    inline void operator *=
    (char const * a) {
        expression =
                expression +
                std::string(" \\times ")+
                std::string(a)
        ; //
    }
    inline void operator *=
    (std::string const & name) {
        expression =
                expression +
                std::string(" \\times ")+
                name
        ; //
    }
    inline void operator *=
    ( Expr const & other ) {
        expression =
                expression +
                std::string(" \\times ")+
                other.expression
        ; //
    }
    inline void operator *=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
    inline void operator *=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                expression +
                std::string(" \\times ")+
                std::string(tmp)
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline void operator /=
    (char a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                a+
                std::string("}")
        ; //
    }
    inline void operator /=
    (char const * a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(a)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (std::string const & a) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                a +
                std::string("}")
        ; //
    }
    inline void operator /=
    ( Expr const & other ) {
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                other.expression +
                std::string("}")
        ; //
    }
    inline void operator /=
    (int a) {
        char tmp[12] ;
        sprintf(tmp,"%d",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (long a) {
        char tmp[16] ;
        sprintf(tmp,"%ld",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (float a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
    inline void operator /=
    (double a) {
        char tmp[16] ;
        sprintf(tmp,"(%e)",a);
        expression =
                std::string("\\frac{")+
                expression +
                std::string("}{")+
                std::string(tmp)+
                std::string("}")
        ; //
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    template <typename T>
    inline Expr operator +
    (T const & a) {
        Expr ret(this[0]);
        ret+=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator -
    (T const & a) {
        Expr ret(this[0]);
        ret-=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator *
    (T const & a) {
        Expr ret(this[0]);
        ret*=a;
        return ret ;
    }
    template <typename T>
    inline Expr operator /
    (T const & a) {
        Expr ret(this[0]);
        ret/=a;
        return ret ;
    }
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
    inline Expr operator - () {
        return Expr (
            std::string("-") +
            expression
        ) ; //
    }
////////////////////////////////////////////////////////////////
    inline Expr
    bracketed () {
        return Expr (
                    std::string ("\\left( ")+
                    expression +
                    std::string (" \\right)")
        ) ; //
    }
    inline void show () {
        std::cout << expression ;
    }
////////////////////////////////////////////////////////////////
    Expr(){}
    template <typename T>
    Expr (T const & a) {
        this[0] = a ;
    }
    ~Expr(){}
////////////////////////////////////////////////////////////////
} ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
template <typename T>
class MainList {
public:
    ////////////////////////////////
    // The Definitions /////////////
    ////////////////////////////////
    using TYPE_DATA = T ;
    //
    using TYPE_COMPLEX_DATA =
        std::complex
            <TYPE_DATA>
    ; //
    //
    using TYPE_MAIN_VECTOR =
        Tensors::NN::ND_ARRAY
            <4,TYPE_COMPLEX_DATA>
    ; //
    //
    using TYPE_MAIN_MATRIX =
        Tensors::NN::N2D_ARRAY
            <4,4,TYPE_COMPLEX_DATA>
    ; //
    //
    using TYPE_REAL_MATRIX =
        Tensors::NN::N2D_ARRAY
            <4,4,TYPE_DATA>
    ; //
    //
    using TYPE_FULL_GAMMA_MATRIX =
        Tensors::NN::ND_ARRAY
            <5,TYPE_MAIN_MATRIX>
    ; //
    ////////////////////////////////
    // Important Constants: ////////
    ////////////////////////////////
    static inline TYPE_DATA
    constexpr PI() {
        return
            3.1415926535897932384626433
        ;
    }
    //
    static inline TYPE_DATA
    constexpr CHARGE_E() {
        return
            0.30282212
        ;
    }
    //
    static inline TYPE_DATA
    constexpr EPS() {
        return
            0.000000001
        ;
    }
    ////////////////////////////////
    // Complexified constants: /////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    constexpr C_1 () {
        return
            TYPE_COMPLEX_DATA
                ( 1.0 , 0.0 )
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_I () {
        return
            TYPE_COMPLEX_DATA
                ( 0.0 , 1.0 )
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr IEPS () {
        return
            C_I () * EPS()
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_PI () {
        return
            C_I() * PI()
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    constexpr C_E () {
        return
            C_I() * CHARGE_E()
        ; //
    }
    ////////////////////////////////
    // Function to get metric //////
    ////////////////////////////////
    static inline TYPE_REAL_MATRIX
    constexpr METRIC () {
        TYPE_REAL_MATRIX ret ;
        ret       =  0.0 ;
        ret[0][0] =  1.0 ;
        ret[1][1] = -1.0 ;
        ret[2][2] = -1.0 ;
        ret[3][3] = -1.0 ;
        return ret ;
    }
    ////////////////////////////////
    // Simple Functions: ///////////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    DAGGER (
        TYPE_COMPLEX_DATA const
            in
    ) {
        return
            std::conj (in)
        ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    GET_VECTOR (
        TYPE_DATA const
            En ,
        TYPE_DATA const
            X  ,
        TYPE_DATA const
            Y  ,
        TYPE_DATA const
            Z
    ) {
        TYPE_MAIN_VECTOR
            ret
        ; /* Prepare the vector */ {
            ret[0] =
                TYPE_COMPLEX_DATA
                    ( En , 0 )
            ; //
            ret[1] =
                TYPE_COMPLEX_DATA
                    ( X  , 0 )
            ; //
            ret[2] =
                TYPE_COMPLEX_DATA
                    ( Y  , 0 )
            ; //
            ret[3] =
                TYPE_COMPLEX_DATA
                    ( Z  , 0 )
            ; //
        } //
        return
            ret
        ; //
    }
    //
    static inline TYPE_MAIN_VECTOR
    DAGGER (
        TYPE_MAIN_VECTOR const &
            in
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(size_t i=0;i<in.SIZE();i++){
            ret[i] =
                DAGGER (in[i])
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    DAGGER (
        TYPE_MAIN_MATRIX const &
            in
    ) {
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                ret[y][x] =
                    DAGGER (
                        in[x][y]
                    )
                ; //
            }
        }
        return ret ;
    }
    //
    static inline TYPE_COMPLEX_DATA
    DOT (
        TYPE_MAIN_VECTOR const &
            p1 ,
        TYPE_MAIN_VECTOR const &
            p2
    ) {
        return
            (p1[0]*DAGGER(p2[0])) -
            (p1[1]*DAGGER(p2[1])) -
            (p1[2]*DAGGER(p2[2])) -
            (p1[3]*DAGGER(p2[3]))
        ; //
    }
    //
    static inline TYPE_COMPLEX_DATA
    NORM2 (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        return
            DOT(p,p)
        ; //
    }
    ////////////////////////////////
    // Scalar Propagator: //////////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    PROPAGATOR (
        TYPE_DATA const & p2         ,
        TYPE_DATA const   m  = 0     ,
        TYPE_DATA const   w  = EPS()
    ) {
        return C_I() / (
            p2 +
            TYPE_COMPLEX_DATA
                (-(m*m),w)
        ) ;
        //
    }
    //
    static inline TYPE_COMPLEX_DATA
    PROPAGATOR (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            PROPAGATOR(
                NORM2(p).real(),
                m,w
            )
        ;
    }
    ////////////////////////////////
    // More convinence functions: //
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr ZERO_MATRIX () {
        TYPE_MAIN_MATRIX ret ;
        ret = TYPE_COMPLEX_DATA ( 0 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    constexpr ZERO_VECTOR () {
        TYPE_MAIN_VECTOR ret ;
        ret = TYPE_COMPLEX_DATA ( 0 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr IDENTITY () {
        auto ret = ZERO_MATRIX () ;
        ret[0][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[1][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[2][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[3][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        return ret ;
    }
    ////////////////////////////////
    // The Gamma Matrix Part ///////
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr C_METRIC() {
        TYPE_MAIN_MATRIX ret = ZERO_MATRIX () ;
        ret[0][0] = TYPE_COMPLEX_DATA (+1.0,0.0) ;
        ret[1][1] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        ret[2][2] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        ret[3][3] = TYPE_COMPLEX_DATA (-1.0,0.0) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_0 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA ( 0 , 0 ) ;
        ret[0][2] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[1][3] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[2][0] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        ret[3][1] = TYPE_COMPLEX_DATA ( 1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_1 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[1][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[2][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[3][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_2 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA ( 0 ,  0 ) ;
        ret[0][3] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
        ret[1][2] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
        ret[2][1] = TYPE_COMPLEX_DATA ( 0 ,  1 ) ;
        ret[3][0] = TYPE_COMPLEX_DATA ( 0 , -1 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_3 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[1][3] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[2][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[3][1] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr GAMMA_5 () {
        TYPE_MAIN_MATRIX ret ;
        ret       = TYPE_COMPLEX_DATA (  0 , 0 ) ;
        ret[0][0] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[1][1] = TYPE_COMPLEX_DATA ( -1 , 0 ) ;
        ret[2][2] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        ret[3][3] = TYPE_COMPLEX_DATA (  1 , 0 ) ;
        return ret ;
    }
    ////////////////////////////////
    // Projection Operators: ///////
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    constexpr PR () {
        auto G5 = GAMMA_5 () ;
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                if (y==x) {
                    ret[y][x] = (
                        + TYPE_COMPLEX_DATA (1.0,0.0)
                        + G5[y][x]
                    ) / 2.0 ;
                } else {
                    ret[y][x] = (
                        + G5[y][x]
                    ) / 2.0 ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    constexpr PL () {
        auto G5 = GAMMA_5 () ;
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                if (y==x) {
                    ret[y][x] = (
                        + TYPE_COMPLEX_DATA (1.0,0.0)
                        - G5[y][x]
                    ) / 2.0 ;
                } else {
                    ret[y][x] = (
                        - G5[y][x]
                    ) / 2.0 ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_FULL_GAMMA_MATRIX
    constexpr ALL_GAMMA () {
        TYPE_FULL_GAMMA_MATRIX ret ;
        ret[0] = GAMMA_0 () ;
        ret[1] = GAMMA_1 () ;
        ret[2] = GAMMA_2 () ;
        ret[3] = GAMMA_3 () ;
        ret[4] = GAMMA_5 () ;
        return ret ;
    }
    ////////////////////////////////
    // QED Vertex: /////////////////
    ////////////////////////////////
    static inline TYPE_FULL_GAMMA_MATRIX
    constexpr QED_VERTEX () {
        auto tmp =
            TYPE_COMPLEX_DATA
                (0.0,-1.0*CHARGE_E())
        ;
        //
        TYPE_FULL_GAMMA_MATRIX ret ;
        ret[0] = MULTIPLY(GAMMA_0(),tmp);
        ret[1] = MULTIPLY(GAMMA_1(),tmp);
        ret[2] = MULTIPLY(GAMMA_2(),tmp);
        ret[3] = MULTIPLY(GAMMA_3(),tmp);
        ret[4] = GAMMA_5();
        return ret ;
    }
    ////////////////////////////////
    // Functions on Matrices ///////
    ////////////////////////////////
    static inline TYPE_COMPLEX_DATA
    TRACE (
        TYPE_MAIN_MATRIX const &
            a
    ) {
        return
            a[0][0] + a[1][1] +
            a[2][2] + a[3][3]
        ; //
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    MULTIPLY (
        TYPE_MAIN_MATRIX  const & a ,
        TYPE_COMPLEX_DATA const   b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp = ret.FLATTEN();
        auto & tmp_in = a.FLATTEN();
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ) {
            tmp[i] =
                tmp_in[i] * b
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    MULTIPLY (
        TYPE_MAIN_VECTOR  const & a ,
        TYPE_COMPLEX_DATA const   b
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(
            size_t i=0;
            i<ret.SIZE();
            i++
        ) {
            ret[i] =
                a[i] * b
            ;
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    MULTIPLY (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_MATRIX const & b
    ) {
        TYPE_MAIN_MATRIX ret ;
        ret = TYPE_COMPLEX_DATA(0,0) ;
        for(size_t y=0;y<TYPE_MAIN_MATRIX::SIZE_Y();y++){
            for(size_t x=0;x<TYPE_MAIN_MATRIX::SIZE_X();x++){
                for(size_t w=0;w<TYPE_MAIN_MATRIX::SIZE_X();w++){
                    ret[y][x] +=
                        a[y][w] * b[w][x]
                    ;
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_VECTOR
    MULTIPLY (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_VECTOR const & b
    ) {
        TYPE_MAIN_VECTOR ret ;
        ret = TYPE_COMPLEX_DATA(0) ;
        for(size_t y=0;y<TYPE_MAIN_MATRIX::SIZE_Y();y++){
            for(size_t x=0;x<TYPE_MAIN_MATRIX::SIZE_X();x++){
                ret[y]+=
                    a[y][x] * b[x]
                ;
            }
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_VECTOR
    ADD (
        TYPE_MAIN_VECTOR const & a ,
        TYPE_MAIN_VECTOR const & b
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(size_t i=0;i<ret.SIZE();i++){
            ret[i]=
                a[i]+b[i]
            ; //
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    ADD (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_MATRIX const & b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp =
            ret.FLATTEN() ;
        //
        tmp = TYPE_COMPLEX_DATA(0) ;
        auto tmp_a =
            a.FLATTEN();
        //
        auto tmp_b =
            b.FLATTEN();
        //
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ){
            tmp[i] =
                tmp_a[i] +
                tmp_b[i]
            ; //
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_VECTOR
    SUBTRACT (
        TYPE_MAIN_VECTOR const &
            a ,
        TYPE_MAIN_VECTOR const &
            b
    ) {
        TYPE_MAIN_VECTOR ret ;
        for(size_t i=0;i<TYPE_MAIN_VECTOR::SIZE();i++){
            ret = a[i] - b[i] ;
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    SUBTRACT (
        TYPE_MAIN_MATRIX const & a ,
        TYPE_MAIN_MATRIX const & b
    ) {
        TYPE_MAIN_MATRIX ret ;
        auto & tmp =
            ret.FLATTEN() ;
        //
        tmp = TYPE_COMPLEX_DATA(0) ;
        auto tmp_a =
            a.FLATTEN();
        //
        auto tmp_b =
            b.FLATTEN();
        //
        for(
            size_t i=0;
            i<tmp.SIZE();
            i++
        ) {
            tmp[i] =
                tmp_a[i] -
                tmp_b[i]
            ; //
        }
        return ret ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    GET_SLASH_ACCURATE (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        TYPE_MAIN_MATRIX ret ;
        for(size_t y=0;y<ret.SIZE_Y();y++){
            for(size_t x=0;x<ret.SIZE_X();x++){
                ret[y][x] = 0 ;
                for(size_t i=0;i<4;i++){
                    ret[y][x] +=
                        ALL_GAMMA()[i][y][x] *
                        p[i]
                    ; //
                }
            }
        }
        return ret ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    GET_SLASH_FAST (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        TYPE_MAIN_MATRIX ret =
            ZERO_MATRIX()
        ; //
        ret[0][2] =   p[3] + p[0] ;
        ret[0][3] =   p[1] - (p[2]*C_I()) ;
        //
        ret[1][2] =   p[1] + (p[2]*C_I()) ;
        ret[1][3] = - p[3] + p[0] ;
        //
        ret[2][0] = - p[3] + p[0] ;
        ret[2][1] = - p[1] + (p[2]*C_I()) ;
        //
        ret[3][0] = - p[1] - (p[2]*C_I()) ;
        ret[3][1] =   p[3] + p[0] ;
        //
        return ret ;
        //
    }
    //
    static inline TYPE_MAIN_MATRIX
    GET_SLASH (
        TYPE_MAIN_VECTOR const &
            p
    ) {
        return
            GET_SLASH_FAST
                (p)
        ;
    }
    ////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    FERMION_NUMERATOR_U (
        TYPE_MAIN_VECTOR const & p ,
        TYPE_DATA        const   m
    ) {
        return
            ADD (
                GET_SLASH(p),
                MULTIPLY(IDENTITY(),m)
            )
        ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_NUMERATOR_V (
        TYPE_MAIN_VECTOR const & p ,
        TYPE_DATA        const   m
    ) {
        return
            SUBTRACT (
                GET_SLASH(p),
                MULTIPLY(IDENTITY(),m)
            )
        ; //
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_PROPAGATOR_U (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            MULTIPLY (
                FERMION_NUMERATOR_U(p,m),
                PROPAGATOR(p,m,w)
            )
        ;
    }
    //
    static inline TYPE_MAIN_MATRIX
    FERMION_PROPAGATOR (
        TYPE_MAIN_VECTOR const & p         ,
        TYPE_DATA        const   m = 0     ,
        TYPE_DATA        const   w = EPS()
    ) {
        return
            FERMION_PROPAGATOR_U
                (p,m,w)
        ; //
    }
////////////////////////////////////////////////////////////////
// Vector propagator = -g_mu_nu * PROPAGATOR ///////////////////
////////////////////////////////////////////////////////////////
    static inline TYPE_MAIN_MATRIX
    PHOTON_PROPAGATOR (
        TYPE_MAIN_VECTOR const & p
    ) {
        auto k = PROPAGATOR (p) ;
        TYPE_MAIN_MATRIX ret =
            ZERO_MATRIX ()
        ; //
        ret[0][0] = -1 ;
        ret[1][1] =  1 ;
        ret[2][2] =  1 ;
        ret[3][3] =  1 ;
        return MULTIPLY (ret,k) ;
    }
////////////////////////////////////////////////////////////////
    using LoopType =
        ql::QCDLoop <
            TYPE_COMPLEX_DATA,
            TYPE_DATA,
            TYPE_DATA
        >
    ; //
    using TYPE_LOOP_RESULTS =
        Tensors::NN::ND_ARRAY
            <3,TYPE_COMPLEX_DATA>
    ; //
    LoopType
        LoopEvaluator
    ; //
    inline TYPE_LOOP_RESULTS
    A0 (
        TYPE_DATA const m ,
        TYPE_DATA const mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
        TYPE_LOOP_RESULTS
            ret
        ; /* Record the return values: */ {
            ret[0] = res[0] ;
            ret[1] = res[1] ;
            ret[2] = res[2] ;
        }
        return ret ;
    }
    //
    inline TYPE_LOOP_RESULTS
    LoopIntegral (
        TYPE_DATA const m ,
        TYPE_DATA const mu2 = 1.0
    ) {
        return
            A0 (
                m*m , mu2
            )
        ; //
    }
    //
    inline TYPE_LOOP_RESULTS
    B0 (
        TYPE_DATA const m1 ,
        TYPE_DATA const m2 ,
        TYPE_DATA const p2 ,
        TYPE_DATA const mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m1)
                ; //
                ms.push_back
                    (m2)
                ; //
            } /* Momentum */ {
                ps.push_back
                    (p2)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
        TYPE_LOOP_RESULTS
            ret
        ; /* Record the return values: */ {
            ret[0] = res[0] ;
            ret[1] = res[1] ;
            ret[2] = res[2] ;
        }
        return ret ;
    }
    //
    inline TYPE_LOOP_RESULTS
    LoopIntegral (
        TYPE_DATA        const m1 ,
        TYPE_DATA        const m2 ,
        TYPE_MAIN_VECTOR const p2 ,
        TYPE_DATA        const mu2 = 1.0
    ) {
        TYPE_DATA p =
            sqrt (
                NORM2(p2)
                .real()
            )
        ; //
        return
            B0 (
                m1*m1 , m2*m2 ,
                p     , mu2
            )
        ; //
    }
    //
    inline TYPE_LOOP_RESULTS
    LoopIntegral (
        TYPE_DATA const m1 ,
        TYPE_DATA const m2 ,
        TYPE_DATA const p2 ,
        TYPE_DATA const mu2 = 1.0
    ) {
        return
            B0 (
                m1*m1 , m2*m2 ,
                p2    , mu2
            )
        ; //
    }
    //
    inline TYPE_LOOP_RESULTS
    C0 (
        TYPE_DATA const m1 ,
        TYPE_DATA const m2 ,
        TYPE_DATA const m3 ,
        TYPE_DATA const p1 ,
        TYPE_DATA const p2 ,
        TYPE_DATA const p3 ,
        TYPE_DATA const mu2 = 1.0
    ) {
        std::vector
            <TYPE_COMPLEX_DATA>
                res
        ; //
        std::vector
            <TYPE_DATA>
                ms, ps
        ; /* Prepare */ {
            /* Mass */ {
                ms.push_back
                    (m1)
                ; //
                ms.push_back
                    (m2)
                ; //
                ms.push_back
                    (m3)
                ; //
            } /* Momentum */ {
                ps.push_back
                    (p1)
                ; //
                ps.push_back
                    (p2)
                ; //
                ps.push_back
                    (p3)
                ; //
            }
        }
        LoopEvaluator
            .integral (
                res , mu2 ,
                ms  , ps
            )
        ; //
        TYPE_LOOP_RESULTS
            ret
        ; /* Record the return values: */ {
            ret[0] = res[0] ;
            ret[1] = res[1] ;
            ret[2] = res[2] ;
        }
        return
            ret
        ;
    }
    //
    inline TYPE_LOOP_RESULTS
    LoopIntegral (
        TYPE_DATA        const m1 ,
        TYPE_DATA        const m2 ,
        TYPE_DATA        const m3 ,
        TYPE_MAIN_VECTOR const q1 ,
        TYPE_MAIN_VECTOR const q2 ,
        TYPE_DATA        const mu2 = 1.0
    ) {
        auto p1 =
            NORM2 (
                q1
            ).real()
        ; //
        auto p2 =
            NORM2 (
                SUBTRACT (
                    q2 , q1
                )
            ).real()
        ; //
        auto p3 =
            NORM2 (
                q2
            ).real()
        ; //
        return
            C0 (
                m1*m1 , m2*m2 , m3*m3 ,
                p1    , p2    , p3    ,
                mu2
            )
        ; //
    }
    //
////////////////////////////////////////////////////////////////
} ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
class ee2mumu {
public:
    typedef float TYPE_DATA ;
    typedef MainList <TYPE_DATA>
        MatrixEval ;
    //
    typedef typename
        MatrixEval::TYPE_COMPLEX_DATA
            TYPE_COMPLEX_DATA ;
    //
    typedef typename
        MatrixEval::TYPE_MAIN_VECTOR
            TYPE_MAIN_VECTOR ;
    //
    typedef typename
        MatrixEval::TYPE_MAIN_MATRIX
            TYPE_MAIN_MATRIX ;
    //
    float            SHAT  ;
    MatrixEval       slave ;
    TYPE_MAIN_MATRIX p     ;
    //
    inline TYPE_COMPLEX_DATA term1 (
        size_t mu, size_t sigma
    ) {
        auto a1 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_V(p[1],0),
            slave.QED_VERTEX()[mu]
        );
        auto a2 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_U(p[0],0),
            slave.QED_VERTEX()[sigma]
        );
        auto a3 = slave.MULTIPLY(a1,a2);
        return slave.TRACE(a3);
    }
    //
    inline TYPE_COMPLEX_DATA
    term2 (
        size_t nu, size_t rho
    ) {
        auto a1 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_U(p[3],1.0),
            slave.QED_VERTEX()[nu]
        );
        auto a2 = slave.MULTIPLY(
            slave.FERMION_NUMERATOR_V(p[2],1.0),
            slave.QED_VERTEX()[rho]
        );
        auto a3 = slave.MULTIPLY(a1,a2);
        return slave.TRACE(a3);
    }
    //
    inline TYPE_COMPLEX_DATA
    eval () {
        //
        TYPE_COMPLEX_DATA P = slave.PROPAGATOR
            (slave.ADD(p[0],p[1]),0);
        //
        P = P * slave.DAGGER(P) ;
        //
        TYPE_COMPLEX_DATA ret = 0 ;
        //
        for(size_t mu=0;mu<4;mu++){
            for(size_t rho=0;rho<4;rho++){
                ret+=
                    slave.C_METRIC()[mu][mu]*
                    slave.C_METRIC()[rho][rho]*
                    term1(mu,rho)*
                    term2(mu,rho)*P;
            }
        }
        //
        return ret ;
        //
    }
    //
    inline TYPE_COMPLEX_DATA
    eval (
        TYPE_DATA const s     ,
        TYPE_DATA const theta ,
        TYPE_DATA const phi
    ) {
        TYPE_COMPLEX_DATA ret ;
        ret = std::sqrt(s/4.0);

        p[0] = slave.ZERO_VECTOR();
        p[1] = slave.ZERO_VECTOR();
        p[2] = slave.ZERO_VECTOR();
        p[3] = slave.ZERO_VECTOR();
        p[0][0] = ret ;
        p[1][0] = ret ;
        p[2][0] = ret ;
        p[3][0] = ret ;

        p[0][3] = ret ;
        p[1][3] = ret * TYPE_COMPLEX_DATA
            (-1.0,0) ;
        //
        p[2][3] = ret * TYPE_COMPLEX_DATA
            ( std::cos(theta) , 0 ) ;
        //
        p[2][2] = ret * TYPE_COMPLEX_DATA (
            std::sin(theta) * std::sin(phi) , 0
        ) ;
        //
        p[2][1] = ret * TYPE_COMPLEX_DATA (
            std::sin(theta) * std::cos(phi) , 0
        ) ;
        //
        p[3][1] = p[2][1] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        p[3][2] = p[2][2] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        p[3][3] = p[2][3] * TYPE_COMPLEX_DATA
            ( -1.0 , 0 ) ;
        //
        auto E4 = slave.CHARGE_E()*
            slave.CHARGE_E()*
            slave.CHARGE_E()*
            slave.CHARGE_E();
        //
        TYPE_COMPLEX_DATA Compare =
            TYPE_COMPLEX_DATA(32.0*E4/(s*s),0.0) * (
                (
                    slave.DOT(p[0],p[2]) *
                    slave.DOT(p[1],p[3])
                ) + (
                    slave.DOT(p[0],p[3]) *
                    slave.DOT(p[1],p[2])
                ) + (
                    slave.DOT(p[0],p[1])
                )
            ) ;
        //
        ret = eval () ;
        //printf("DEBUG: %e\n",Compare.real()/ret.real());
        return ret ;
    }
} ;
////////////////////////////////////////////////////////////////
