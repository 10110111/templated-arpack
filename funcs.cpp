// Some functions from libf2c (adapted to C++ and reformatted), to avoid linking to it
static integer s_cmp(const char *a0, const char *b0, ftnlen la, ftnlen lb)
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

static void s_copy(char *a, const char *b, ftnlen la, ftnlen lb)
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

static int disnan_(const Real* x) { return isnan(*x); }

static Real d_sign(Real *a, Real *b)
{
    Real x;
    x = (*a >= 0 ? *a : - *a);
    return( *b >= 0 ? x : -x);
}

static Real pow_di(Real *ap, integer *bp)
{
    Real pow, x;
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

static Real pow_dd(const Real *ap, const Real *bp)
{
    using std::pow;
    return(pow(*ap, *bp) );
}
template<typename A, typename B> constexpr static auto min(A const& a, B const& b) { return a<=b ? a : b; }
template<typename A, typename B> constexpr static auto max(A const& a, B const& b) { return a>=b ? a : b; }
template<typename T> constexpr static auto abs(T x) { using std::abs; return abs(x); }

// Simple C++ reimplementation of LAPACK's xerbla
static void xerbla_(const char* SRNAME, integer* info, ftnlen srnameLen)
{
    std::cerr << " ** On entry to " << std::string(SRNAME,srnameLen) << " parameter number " << info << " had an illegal value\n";
    exit(255);
}

// C++ template-aware reimplementation of LAPACK's dlamch
static Real dlamch_(const char* cmach, ftnlen)
{
    typedef std::numeric_limits<Real> Lim;

    const bool RND=true;
    const Real EPS=Lim::epsilon()*(RND ? Real(0.5) : Real(1.));
    switch(*cmach)
    {
    case 'e': case 'E': return EPS;
    case 's': case 'S':
    {
         auto SFMIN = Lim::min();
         const auto SMALL = Real(1.) / Lim::max();
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
    case 'b': case 'B': return Lim::radix;
    case 'p': case 'P': return EPS*Lim::radix;
    case 'n': case 'N': return Lim::digits;
    case 'r': case 'R': return RND;
    case 'm': case 'M': return Lim::min_exponent;
    case 'u': case 'U': return Lim::min();
    case 'l': case 'L': return Lim::max_exponent;
    case 'o': case 'O': return Lim::max();
    default:  return 0;
    }
}

