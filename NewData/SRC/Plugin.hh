#include <vector>
#include <fastjet/Selector.hh>
#include <fastjet/ClusterSequence.hh>

template <
    size_t N ,
    typename Tf = float
> class ImageGen {
public:

    using vector4  =
        fastjet::PseudoJet
    ; //

    using vector4s =
        std::vector
            <fastjet::PseudoJet>
    ; //

    Tf Image[N][N] ;

    static inline double
    norm4 (
        vector4 const & a
    ) {
        double ret =
            (a[3]*a[3]) -
            (a[2]*a[2]) -
            (a[1]*a[1]) -
            (a[0]*a[0])
        ;
        return
            sqrt(ret)
        ; //
    }

    static inline double
    norm3 (
        vector4 const & a
    ) {
        double ret =
            (a[2]*a[2]) +
            (a[1]*a[1]) +
            (a[0]*a[0])
        ;
        return
            sqrt(ret)
        ; //
    }

    static inline double
    dot3 (
        vector4 const & a ,
        vector4 const & b
    ) {
        return
            (a[0]*b[0]) +
            (a[1]*b[1]) +
            (a[2]*b[2])
        ; //
    }

    static inline double
    dot4 (
        vector4 const & a ,
        vector4 const & b
    ) {
        return
            (a[3]*b[3]) -
            dot3(a,b)
        ; //
    }

    inline void Eval (
        vector4s const &
            invectors  ,
        double const
            M0 = 0.5
    ) {
        for(size_t x=0;x<N;x++){
            for(size_t y=0;y<N;y++){
                Image[x][y] = 0.0 ;
            }
        }
        if(invectors.size()>2){
            const double E0 =
                1.0
            ; //
            const double P0 =
                sqrt(
                    (E0*E0) -
                    (M0*M0)
                )
            ; //

            fastjet::JetAlgorithm algo =
                fastjet::kt_algorithm
            ; //

            fastjet::JetDefinition
                jet_def (
                    algo ,
                    100.0
                )
            ; //

            fastjet::ClusterSequence
                clust_seq (
                    invectors ,
                    jet_def
                )
            ; //

            vector4s basis =
                fastjet::sorted_by_pt (
                    clust_seq.exclusive_jets (3)
                )
            ; //

            if(basis.size()>2){
                vector4 eZ, eY, eX ;

                eZ =
                    basis[0] +
                    basis[1] +
                    basis[2]
                ; //

                const double EJ =
                    eZ[3]
                ; //
                const double PJ =
                    norm3 (eZ)
                ; //
                const double MJ =
                    norm4 (eZ)
                ; //
                const double Factor =
                    (M0/MJ)
                ; //
                double GM =
                    ( (EJ*E0) - (PJ*P0) ) /
                    (MJ*M0)
                ; //
                if(GM<1.0000000001){
                    GM=1.0000000001;
                }
                const double BT =
                    sqrt(
                        1.0 - (1.0/(GM*GM))
                    )
                ; //

                eZ /=
                    norm3 (eZ)
                ; //

                eX =
                    basis[0] -
                    (eZ*dot3(basis[0],eZ))
                ; //
                eX /=
                    norm3 (eX)
                ; //

                eY =
                    basis[1] -
                    (eZ*dot3(basis[1],eZ)) -
                    (eX*dot3(basis[1],eX))
                ; //
                eY /=
                    norm3 (eY)
                ; //

                for(
                    size_t i=0;
                    i<invectors.size();
                    i++
                ) {

                    vector4 tmp =
                        invectors[i] *
                        Factor
                    ; //

                    double eI = tmp[3]        ;
                    double pX = dot3 (tmp,eX) ;
                    double pY = dot3 (tmp,eY) ;
                    double pZ = dot3 (tmp,eZ) ;

                    if (EJ>E0) {
                        eI = GM * ( eI - (BT*pZ) ) ;
                    } else {
                        eI = GM * ( eI + (BT*pZ) ) ;
                    }

                    double cX = pX / eI ;
                    double cY = pY / eI ;

                    size_t N_X =
                        ( (size_t) (((double)N) * (cX+1.0) / 2.0) )
                    ;
                    if(N_X>=N){N_X=N-1;}

                    size_t N_Y =
                        ( (size_t) (((double)N) * (cY+1.0) / 2.0) )
                    ;
                    if(N_Y>=N){N_Y=N-1;}

                    Image[N_Y][N_X] += eI ;
                }
            }
        }
    }
    //
    inline void operator () (
        vector4s const &
            invectors  ,
        double const
            M0 = 0.5
    ) {
        Eval(invectors,M0);
    }
    //
    ImageGen (
        vector4s const &
            invectors  ,
        double const
            M0 = 0.5
    ) {
        //printf("%ld\n",sizeof(ImageGen));
        Eval(invectors,M0);
    }
    //
    ImageGen(){
        //printf("%ld\n",sizeof(ImageGen));
    }
    //
    ~ImageGen(){}
    //
} ;
