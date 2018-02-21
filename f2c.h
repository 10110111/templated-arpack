#ifndef F2C_TEMPLATED_ARPACK_INCLUDED
#define F2C_TEMPLATED_ARPACK_INCLUDED

typedef long int integer;
typedef double doublereal;
typedef long int ftnlen;
typedef long int logical;
void s_copy(char *a, const char *b, ftnlen la, ftnlen lb);
integer s_cmp(const char *a0, const char *b0, ftnlen la, ftnlen lb);

#include <math.h>

template<typename T>
int disnan_(const T* x) { return isnan(*x); }

template<typename doublereal>
doublereal d_sign(doublereal *a, doublereal *b)
{
    doublereal x;
    x = (*a >= 0 ? *a : - *a);
    return( *b >= 0 ? x : -x);
}

template<typename doublereal>
doublereal pow_di(doublereal *ap, integer *bp)
{
    doublereal pow, x;
    integer n;
    unsigned long u;

    pow = 1;
    x = *ap;
    n = *bp;

    if(n != 0)
    {
        if(n < 0)
        {
            n = -n;
            x = 1/x;
        }
        for(u = n; ; )
        {
            if(u & 01)
                pow *= x;
            if(u >>= 1)
                x *= x;
            else
                break;
        }
    }
    return(pow);
}

template<typename doublereal>
doublereal pow_dd(const doublereal *ap, const doublereal *bp)
{
    return(pow(*ap, *bp) );
}
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
using std::abs;
#define TRUE_ (1)
#define FALSE_ (0)

#endif
