#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <limits>

template<typename T>
T lamch(char cmach)
{
    typedef std::numeric_limits<T> Lim;

    const bool RND=true;
    const T EPS=Lim::epsilon()*(RND ? T(0.5) : T(1.));
    switch(cmach)
    {
    case 'E': return EPS;
    case 'S':
    {
         double SFMIN = Lim::min();
         const double SMALL = T(1.) / Lim::max();
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

extern "C"
{

int disnan_(double x) { return isnan(x); }

double dlamch_(const char* cmach)
{
    return lamch<double>(*cmach);
}

void xerbla_(const char* SRNAME, const int* info, unsigned srnameLen)
{
    std::cerr << " ** On entry to " << std::string(SRNAME,srnameLen) << " parameter number " << info << " had an illegal value\n";
    exit(255);
}

// Some functions from libf2c (adapted to C++ and reformatted), to avoid linking to it
#include "f2c.h"
integer s_cmp(const char *a0, const char *b0, ftnlen la, ftnlen lb)
{
    const unsigned char *a, *aend, *b, *bend;
    a = (const unsigned char *)a0;
    b = (const unsigned char *)b0;
    aend = a + la;
    bend = b + lb;

    if(la <= lb)
    {
        while(a < aend)
            if(*a != *b)
                return( *a - *b );
            else
            { ++a; ++b; }

        while(b < bend)
            if(*b != ' ')
                return( ' ' - *b );
            else
                ++b;
    }

    else
    {
        while(b < bend)
            if(*a == *b)
            { ++a; ++b; }
            else
                return( *a - *b );
        while(a < aend)
            if(*a != ' ')
                return(*a - ' ');
            else
                ++a;
    }
    return(0);
}

void s_copy(char *a, const char *b, ftnlen la, ftnlen lb)
{
    char *aend;
    const char *bend;

    aend = a + la;

    if(la <= lb)
#ifndef NO_OVERWRITE
        if (a <= b || a >= b + la)
#endif
            while(a < aend)
                *a++ = *b++;
#ifndef NO_OVERWRITE
        else
            for(b += la; a < aend; )
                *--aend = *--b;
#endif

    else
    {
        bend = b + lb;
#ifndef NO_OVERWRITE
        if (a <= b || a >= bend)
#endif
            while(b < bend)
                *a++ = *b++;
#ifndef NO_OVERWRITE
        else
        {
            a += lb;
            while(b < bend)
                *--a = *--bend;
            a += lb;
        }
#endif
        while(a < aend)
            *a++ = ' ';
    }
}

double d_sign(const doublereal *a, const doublereal *b)
{
    double x;
    x = (*a >= 0 ? *a : - *a);
    return( *b >= 0 ? x : -x);
}

double pow_di(const doublereal *ap, const integer *bp)
{
    double pow, x;
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

double pow_dd(const doublereal *ap, const doublereal *bp)
{
    return(pow(*ap, *bp) );
}

}
