class Expr {
public:
    std::string expression ;
	//
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

