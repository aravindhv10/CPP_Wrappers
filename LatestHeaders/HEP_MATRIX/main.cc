#include "./pcg.hh"
#include "cblas.h"
#include "cmath"
#include "./HEP_MATRIX.hh"
#include <iomanip>

using TYPE_DATA =
    double
; //
using TYPE_DATAs =
    std::vector
        <TYPE_DATA>
; //
using HEP =
    MainList
        <TYPE_DATA>
; //
using TYPE_COMPLEX_DATA =
    typename
        HEP::TYPE_COMPLEX_DATA
; //
using TYPE_COMPLEX_DATAs =
    std::vector
        <TYPE_COMPLEX_DATA>
;
int main () {
    HEP slave ;
    slave.APPROX_C0(5.0000001,10.0000001);
    /* Testing B0 function: */ {
        auto res =
            slave
                .B0 (
                    5.0, 2.0,
                    1.0,
                    1.7*1.7
                )
        ; //
        printf("B0 eps0 (%e,%e)\n",res[0].real(),res[0].imag());
        printf("B0 eps1 (%e,%e)\n",res[1].real(),res[1].imag());
        printf("B0 eps2 (%e,%e)\n",res[2].real(),res[2].imag());
    }
    /* Testing C0 function: */ {
        auto q1 =
            slave.GET_VECTOR
                (5,0,0,4)
        ;
        auto q2 =
            slave.GET_VECTOR
                (5,0,0,3)
        ;
        auto res =
            slave
                .C0 (
                    5.0     , 2.0 , 2.0 ,
                    1.0     , 2.0 , 2.0 ,
                    1.7*1.7
                )
        ; //
        res =
            slave
            .LoopIntegral (
                5.0 , 2.0 , 2.0 ,
                q1 , q2
            )
        ;
        printf("C0 eps0 (%e,%e)\n",res[0].real(),res[0].imag());
        printf("C0 eps1 (%e,%e)\n",res[1].real(),res[1].imag());
        printf("C0 eps2 (%e,%e)\n",res[2].real(),res[2].imag());
    }

    return 0 ;
}
