#ifndef F2C_TEMPLATED_ARPACK_INCLUDED
#define F2C_TEMPLATED_ARPACK_INCLUDED

typedef long int integer;
typedef long int ftnlen;
typedef long int logical;
void s_copy(char *a, const char *b, ftnlen la, ftnlen lb);
integer s_cmp(const char *a0, const char *b0, ftnlen la, ftnlen lb);
void xerbla_(const char* SRNAME, integer* info, ftnlen srnameLen);

#include <math.h> // to make all the math functions with all their overloads appear in global namespace (g++ doesn't put some overloads from <cmath> into global namespace)
#include <cmath> // for std::abs, because ::abs doesn't appear to be defined in math.h when compiling with clang
#include <limits>

template<typename T>
T dlamch_(const char* cmach, ftnlen)
{
    typedef std::numeric_limits<T> Lim;

    const bool RND=true;
    const T EPS=Lim::epsilon()*(RND ? T(0.5) : T(1.));
    switch(*cmach)
    {
    case 'E': return EPS;
    case 'S':
    {
         auto SFMIN = Lim::min();
         const auto SMALL = T(1.) / Lim::max();
         if( SMALL >= SFMIN )
         {
/*
*           Use SMALL plus a bit, to avoid the possibility of rounding
*           causing overflow when computing  1/sfmin.
*/
            SFMIN = SMALL*( 1.+EPS );
         }
         return SFMIN;
    }
    case 'B': return Lim::radix;
    case 'P': return EPS*Lim::radix;
    case 'N': return Lim::digits;
    case 'R': return RND;
    case 'M': return Lim::min_exponent;
    case 'U': return Lim::min();
    case 'L': return Lim::max_exponent;
    case 'O': return Lim::max();
    default:  return 0;
    }
}

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
template<typename A, typename B> constexpr auto min(A const& a, B const& b) { return a<=b ? a : b; }
template<typename A, typename B> constexpr auto max(A const& a, B const& b) { return a>=b ? a : b; }
using std::abs;

#endif
