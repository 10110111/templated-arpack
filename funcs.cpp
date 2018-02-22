#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "f2c.h"

void xerbla_(const char* SRNAME, integer* info, ftnlen srnameLen)
{
    std::cerr << " ** On entry to " << std::string(SRNAME,srnameLen) << " parameter number " << info << " had an illegal value\n";
    exit(255);
}

// Some functions from libf2c (adapted to C++ and reformatted), to avoid linking to it
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
