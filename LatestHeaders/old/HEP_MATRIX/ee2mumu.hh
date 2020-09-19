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
