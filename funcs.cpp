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

}
