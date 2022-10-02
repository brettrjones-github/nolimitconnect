#include "VxMathDef.h"


const float vx_fast_atof_table[ 16 ] =
{
	0.f,
	0.1f,
	0.01f,
	0.001f,
	0.0001f,
	0.00001f,
	0.000001f,
	0.0000001f,
	0.00000001f,
	0.000000001f,
	0.0000000001f,
	0.00000000001f,
	0.000000000001f,
	0.0000000000001f,
	0.00000000000001f,
	0.000000000000001f
};

//============================================================================
//! return true if value is a power of 2 value
int IsAPowerOf2( int iVal )
{
    int iBitCount = 0;
    int iBitsPerInt = sizeof(int) * 8;
    for( int i = 0; i < iBitsPerInt; i++ )
    {
        if( iVal & ( 1 << i ) )
        {
            iBitCount++;
        }
    }

    return ( 1 == iBitCount )?1:0;
}

//============================================================================
//=== standard math replacement functions ( for speed reasons ) ===//
long FastLog( long x )
{
    long a=0;
    while( x>>=1 )
        a++;
    return a;
}

//============================================================================
float FastFabs( float f )
{
    long a = * ( long * ) &f;
    a &= 0x7FFFFFFF;
    return * ( float * ) &a;
}

//============================================================================
//Eero Pajarre Posted this function to mesa bbs that greatly speeds up ftol
//
//The performance difference between this function and the default of ftol
//of VC++ is dramatic (I have seen Opengl using 20-30% of its time in ftol), but
//it should be noted that the function rounds its argument (ftol truncates),so it
//must be used with some care.
long FastFtol(float f)
{
//#ifdef _MSC_VER
    return (long)f;
    /* errors with ambiguous operand size for `fld'
#else
    // gcc compile
   long r = 0;
   asm (
        ".intel_syntax noprefix\n"
        "fld f\n"
        "fistp r\n"
        ".att_syntax \n"
    );

    return r;
#endif // _MSC_VER*/
}

//============================================================================
//fast way to make number the next power of 2
int FastNextPow2(int Number)
{
//#ifdef _MSC_VER
    return (int)pow(2, ceil(log(Number)/log(2)));
    /* errors with ambiguous operand size for `shl'
#else
    // gcc compile
   long r = 0;
   asm (
        ".intel_syntax noprefix\n"
        "xor ecx, ecx\n"
        "bsr ecx, Number\n"
        "inc ecx\n"
        "shl RetVal, cl\n"
        ".att_syntax \n"
    );

    return r;
#endif // _MSC_VER*/
}
//nVidia FastSqrt
//table for fast sqrt
extern unsigned long g_au32FastSqrtTable[ 0x10000 ];
//floating point bits
#define F32Bits(fp) (*(unsigned *)&(fp))

//============================================================================
float FastSqrtf( float x )
{
    if( 0 == F32Bits( x ) )
    {
        return 0.0f;
    }
    F32Bits(x) = g_au32FastSqrtTable[( F32Bits(x) >> 8) & 0xFFFF] |
        ((((F32Bits(x) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);

    return x;
}

//============================================================================
float AngleModulus( float fAngle )
{
    return (float)(( 360.0/65536 ) * ((long)( fAngle * ( 65536/360.0 )) & 65535));
}

//============================================================================
float LerpAngle( float fAngleA, float fAngleB, float fFraction )
{
    if (fAngleA - fAngleB > 180)
    {
        fAngleA -= 360;
    }

    if (fAngleA - fAngleB < -180)
    {
        fAngleA += 360;
    }

    return fAngleB + fFraction * (fAngleA - fAngleB );
}

//============================================================================
float ClampedAcosf(float x)
{
    if(x >  1.0f)
    {
        x =  1.0f;
    }
    if(x < -1.0f)
    {
        x = -1.0f;
    }
    return (float)acos(x);
}

//============================================================================
float ClampedAsinf(float x)
{
    if(x >  1.0f)
    {
        x =  1.0f;
    }
    if(x < -1.0f)
    {
        x = -1.0f;
    }
    return (float)asin(x);
}

//=== Included for nebula device compatability ===//
#ifdef _MSC_VER
# ifndef isnan
#  define isnan _isnan
# endif // isnan
# ifndef isinf
#  define isinf _isinf
# endif // isinf
#endif // _MSC_VER

#ifndef TINY
#define TINY (0.0000001)
#endif

#define vx_max(a,b)      (((a) > (b)) ? (a) : (b))
#define vx_min(a,b)      (((a) < (b)) ? (a) : (b))
#define vx_abs(a)        (((a)<0.0f) ? (-(a)) : (a))
#define vx_sgn(a)        (((a)<0.0f) ? (-1) : (1))

#define vx_deg2rad(d)    (((d)*PI_F)/180.0f)
#define vx_rad2deg(r)    (((r)*180.0f)/PI_F)

#define vx_sin(x) (sinf(x))
#define vx_cos(x) (cosf(x))

//============================================================================
float vx_acos(float x)
{
    if(x >  1.0f)
    {
        x =  1.0f;
    }

    if(x < -1.0f)
    {
        x = -1.0f;
    }

    return acosf(x);
}

//============================================================================
float vx_asin(float x)
{
    if(x >  1.0f)
    {
        x =  1.0f;
    }

    if(x < -1.0f)
    {
        x = -1.0f;
    }

    return asinf(x);
}

//============================================================================
float vx_sqrt(float x)
{
    if (x<0.0f)
    {
        x = 0.0f;
    }

    return sqrtf(x);
}

//============================================================================
bool vx_fequal(float f0, float f1, float tol)
{
    float f = f0-f1;
    if ((f>(-tol)) && (f<tol))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
//-------------------------------------------------------------------
//  Return true if (f0<f1).
//-------------------------------------------------------------------
bool vx_fless(float f0, float f1, float tol)
{
    if ((f0-f1)<tol)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//============================================================================
//-------------------------------------------------------------------
//  Return true if (f0>f1)
//-------------------------------------------------------------------
bool vx_fgreater(float f0, float f1, float tol)
{
    if ((f0-f1)>tol)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------
//! calculate number of bits set.. speed optimized
//! contributed by Johnny Corvin
int CountBitsSetInVar( unsigned long u32Var )
{
    int iCnt = 0;
    while( u32Var )
    {
        iCnt++;
        u32Var &= ( u32Var - 1 );
    }
    return iCnt;
}

//============================================================================
//! turns a value into a power of 2 Value
unsigned long VxMakePowerOf2( unsigned long u32Val )
{
    // find highest bit set
    unsigned long u32Pow2 = 1;
    unsigned long u32Mask = 1;
    for( int i = 0; i < 32; i++ )
    {
        if( u32Mask & u32Val )
        {
            u32Pow2 = u32Mask;
        }
        u32Mask <<= 1;
    }
    return u32Pow2;
}
//============================================================================
//! increments Value to the next highest power of 2
unsigned long VxIncrementPowerOf2( unsigned long u32Val )
{
    // first to make sure starting value is a power of 2
    u32Val = VxMakePowerOf2( u32Val );
    u32Val <<= 1;
    return u32Val;
}

//============================================================================
//! increments Value to the next lowest power of 2
unsigned long VxDecrementPowerOf2( unsigned long u32Val )
{
    u32Val = VxMakePowerOf2( u32Val );
    u32Val >>= 1;
    if( 0 == u32Val )
    {
        return 1;
    }

    return u32Val;
}


#ifdef __cplusplus
#ifndef sincos
# define sincos( a, b, c ) (b=sin(a),c=cos(a))
# define sincosf( a, b, c ) (b=sinf(a),c=cosf(a))
#endif // sincos

//-------------------------------------------------------------------
//  random floating point with precision
//-------------------------------------------------------------------
float frand( int precis )
{
    int val=(int)pow(10.0f,precis);
    return (rand()%val)/(float)val;
}
//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
void LimitTo360Degrees( float & f32Degrees )
{
    while( f32Degrees > 360.0f )
    {
        f32Degrees -= 360.0f;
    }
    while( f32Degrees < -360.0f )
    {
        f32Degrees += 360.0f;
    }
}

//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
void LimitTo360Degrees( double & f64Degrees )
{
    while( f64Degrees > 360.0 )
    {
        f64Degrees -= 360.0;
    }
    while( f64Degrees < -360.0 )
    {
        f64Degrees += 360.0;
    }
}

//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
//! Provides a fast function for converting a string into a float,
//! about 6 times faster than atof in win32.
// If you find any bugs, please send them to me, niko (at) irrlicht3d.org.
const char* vx_fast_atof_func( const char* c, float& out )
{
    bool inv = false;
    char *t;
    float f;
    if( *c == '-' )
    {
        c++;
        inv = true;
    }
    f = ( float )strtol( c, &t, 10 );

    c = t;

    if( *c == '.' )
    {
        c++;

        float pl = ( float )strtol( c, &t, 10 );
        pl *= vx_fast_atof_table[ t - c ];

        f += pl;

        c = t;

        if( *c == 'e' )
        {
            ++c;
            float exp = ( float )strtol( c, &t, 10 );
            f *= ( float )powf( 10.0f, exp );
            c = t;
        }
    }

    if( inv )
        f *= -1.0f;

    out = f;
    return c;
}

//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
VXMATH_INLINE float vx_fast_atof( const char* c )
{
    float ret;
    vx_fast_atof_func( c, ret );
    return ret;
}


#else
// C only
#ifndef sincos
# define sincos( a, b, c ) (*b=sin(a),*c=cos(a))
# define sincosf( a, b, c ) (*b=sinf(a),*c=cosf(a))
#endif // sincos

float frand( int precis )
{
    int val = ( int )pow( 10.0f, precis );
    return ( rand() % val ) / ( float )val;
}
//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
void LimitTo360DegreesF32( float * f32Degrees )
{
    while( *f32Degrees > 360.0f )
    {
        *f32Degrees -= 360.0f;
    }
    while( *f32Degrees < -360.0f )
    {
        *f32Degrees += 360.0f;
    }
}

//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
void LimitTo360DegreesF64( double * f64Degrees )
{
    while( *f64Degrees > 360.0 )
    {
        *f64Degrees -= 360.0;
    }
    while( *f64Degrees < -360.0 )
    {
        *f64Degrees += 360.0;
    }
}
//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
//! Provides a fast function for converting a string into a float,
//! about 6 times faster than atof in win32.
// If you find any bugs, please send them to me, niko (at) irrlicht3d.org.
const char* vx_fast_atof_func( const char* c, float * out )
{
    bool inv = false;
    char *t;
    float f;
    if( *c == '-' )
    {
        c++;
        inv = true;
    }
    f = ( float )strtol( c, &t, 10 );

    c = t;

    if( *c == '.' )
    {
        c++;

        float pl = ( float )strtol( c, &t, 10 );
        pl *= vx_fast_atof_table[ t - c ];

        f += pl;

        c = t;

        if( *c == 'e' )
        {
            ++c;
            float exp = ( float )strtol( c, &t, 10 );
            f *= ( float )powf( 10.0f, exp );
            c = t;
        }
    }

    if( inv )
        f *= -1.0f;

    *out = f;
    return c;
}

//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
float vx_fast_atof( const char* c )
{
    float ret;
    vx_fast_atof_func( c, &ret );
    return ret;
}


#endif // __cplusplus

