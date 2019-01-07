#pragma once
//============================================================================
// public domain 2003
//============================================================================
#include <CoreLib/config_corelib.h>
#include <math.h>
#include <stdlib.h>
#ifndef _MSC_VER
    // gcc does not have msvc style inlines
    #define _inline GOTV_INLINE
    #define VX_INLINE GOTV_INLINE
    #define VXMATH_INLINE GOTV_INLINE
#else
    // MSVC
    #ifndef VX_INLINE
        #define VX_INLINE __forceinline
    #endif

    #ifndef VXMATH_INLINE
        #define VXMATH_INLINE __forceinline
    #endif
#endif

//=== platform variable defines ===//
#define MAX_S16 32767
#define MIN_S16 -32768
#define MAX_S32 2147483647
#define MIN_S32 -2147483648
#define MAX_F32 3.402823466e+38F			// maximum possible positive value for 32 bit float
#define MIN_F32 -3.402823466e+38F			// minimum possible value for 32 bit float
#define MAX_F64 1.7976931348623158e+308		// maximum possible positive value for 64 bit float
#define MIN_F64 -1.7976931348623158e+308	// minimum possible value for 64 bit double

#define SMALL_F64	 0.001
#define SMALL_F32  0.001f

#define TINY_F64     (1.7976931348623158e+300)
#define TINY_F32   (3.402823466e+30F)

#define	ANGLE2SHORT(a)	( (short) ((a)*65536/360) & 65535)
#define	SHORT2ANGLE(a)	( (a) * (360.0/65536) )

#define ROUND_TO_4BYTE_BOUNDRY( a ) (( a + 3 ) & ~3 )		//round upto even 4 byte boundry
#define ROUND_TO_8BYTE_BOUNDRY( a ) (( a + 7 ) & ~7 )		//round upto even 8 byte boundry
#define ROUND_TO_16BYTE_BOUNDRY( a ) (( a + 15 ) & ~15 )	//round upto even 16 byte boundry

//=== math defines  ===//
#define CONVERT_UM_TO_MM(um)	 (double)(um) / 1000.0              //Convert micrometers to millimeters
#define CONVERT_MM_TO_UM(mm)	 (double)(mm) * 1000.0	            //Convert millimeters to micrometers
#define CONVERT_MM_TO_INCH(mm)	 (double)(mm) / 25.4	            //25.4 millimeters per inch
#define CONVERT_UM_TO_INCH(um)	 (double)(um) / 25400.0	            //25400 micrometers per inch
#define CONVERT_INCH_TO_MM(inch) (double)(inch) * 25.4	            //25.4 millimeters per inch
#define CONVERT_INCH_TO_UM(inch) (double)(inch) * 25400.0           //25400 micrometers per inch
#define CONVERT_FREQ_TO_CWORD(freq) (unsigned)(218.462*freq)           //Convert freq. to steering control word
#define CONVERT_CWORD_TO_FREQ(cword)(double)(cword/(double)218.462)    //Convert control word to freq.

// constants (double)
#define EPSILON		0.0001 //small fudge for double

#define VX_PI            (3.1415926535897932384626433832795) //pi
#define VX_PI_X2         (6.28318530717958647692528676655901) // 2  * pi
#define VX_PI_DIV2       (1.57079632679489661923132169163975)//pi/2
#define VX_PI_DIV2_X3    (4.71238898038470305769396507491925) //3 * pi/2
#define VX_Ln10          (2.30258509299404568401799145468436)  // Ln( 10 )  =  2.3025851
#define VX_ONE_OVER_LN10 (0.434294481903251827651128918916605) // 1/ln10
#define VX_PI_OVER_180   (1.74532925199432957692369076848861E-002)  // Pi  / 180 =  0.0174533
#define VX_PI_UNDER_180  (57.2957795130823208767981548141052)  // 180 / Pi  = 57.2957795
#define VX_RADTODEG		VX_PI_UNDER_180 //radians to degrees
#define VX_DEGTORAD		VX_PI_OVER_180 //degrees to radians

//
#define VX_EPSILON_F	0.0001 //small fudge for float
#define VX_PI_F          (3.1415926535897932384626433832795f) //pi
#define VX_PI_X2_F       (6.28318530717958647692528676655901f) // 2  * pi
#define VX_PI_DIV2_F     (1.57079632679489661923132169163975f)//pi/2
#define VX_PI_DIV2_X3_F  (4.71238898038470305769396507491925f) //3 * pi/2
#define VX_Ln10_F        (2.30258509299404568401799145468436f)  // Ln( 10 )  =  2.3025851
#define VX_ONE_OVER_LN10_F (0.434294481903251827651128918916605f) // 1/ln10
#define VX_PI_OVER_180_F  (1.74532925199432957692369076848861E-002f)  // Pi  / 180 =  0.0174533
#define VX_PI_UNDER_180_F (57.2957795130823208767981548141052f)   //180 / Pi  = 57.2957795
#define VX_RADTODEG_F		VX_PI_UNDER_180_F //radians to degrees
#define VX_DEGTORAD_F		VX_PI_OVER_180_F //degrees to radians

#ifndef M_E 
# define M_E        2.71828182845904523536   // e
#endif // M_E
#ifndef M_LOG2E 
# define M_LOG2E    1.44269504088896340736   // log2(e)
#endif // M_LOG2E
#ifndef M_LOG10E 
# define M_LOG10E   0.434294481903251827651  // log10(e)
#endif // M_LOG10E
#ifndef M_LN2 
# define M_LN2      0.693147180559945309417  // ln(2)
#endif // M_LN2
#ifndef M_LN10 
# define M_LN10     2.30258509299404568402   // ln(10)
#endif // M_LN10
#ifndef M_PI		
# define M_PI       (3.1415926535897932384626433832795)  // pi definition used by most gnu etc programs
#endif // M_PI
#ifndef M_PI_2 
# define M_PI_2     1.57079632679489661923   // pi/2
#endif // M_PI_2
#ifndef M_PI_4 
# define M_PI_4     0.785398163397448309616  // pi/4
#endif // M_PI_4
#ifndef M_1_PI 
# define M_1_PI     0.318309886183790671538  // 1/pi
#endif // M_1_PI
#ifndef M_2_PI 
# define M_2_PI     0.636619772367581343076  // 2/pi
#endif // M_2_PI
#ifndef M_2_SQRTPI 
# define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#endif // M_2_SQRTPI
#ifndef M_SQRT2 
# define M_SQRT2    1.41421356237309504880   // sqrt(2)
#endif // M_SQRT2
#ifndef M_SQRT1_2 
# define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif // M_SQRT1_2

//=== macro functions ===//

#define DegToRad(Angle) ((Angle) * VX_PI_OVER_180)
#define RadToDeg(Angle) ((Angle) * VX_PI_UNDER_180)        //    { Angle * 180 / Pi }
#define DegToRadf(Angle) ((Angle) * VX_PI_OVER_180_F)
#define RadToDegf(Angle) ((Angle) * VX_PI_UNDER_180_F)        //    { Angle * 180 / Pi }

#define CosDeg(Angle) (cos((Angle) * VX_PI_OVER_180 ))
#define SinDeg(Angle) (sin((Angle) * VX_PI_OVER_180 ))
#define TanDeg(Angle) (tan((Angle) * VX_PI_OVER_180 ))
#define CosDegf(Angle) (cosf((Angle) * VX_PI_OVER_180_F ))
#define SinDegf(Angle) (sinf((Angle) * VX_PI_OVER_180_F ))
#define TanDegf(Angle) (tanf((Angle) * VX_PI_OVER_180_F ))

#define ClampAngle(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x

#define Trunc(x)	((long)x)
#define Sqr(x)		( x * x )
#define Sign(x) ((x)> 0 ? 1: ((x)==0 ? 0: (-1)))
#ifndef ABS
# define ABS(x) ((x)<0 ? -(x) : (x))
#endif // ABS
#define VxAbs(x) ((x)<0 ? -(x) : (x))
#define Floor(a)     ((a)>0?(long)a:-(long)(a))
#define Cieling(a)   ((a)==(long)(a)?(a):(a>0?1 +(long)a):-(1+(long)(-a))
#define Round(a)     ((a)>0?(long)(a)+0.5:-(long)(0.5-a))
#define MAX3(x,y,z)  (x>y && x>z ? x : (y>z ? y : z))
#define MIN3(x,y,z)  (x<y && x<z ? x : (y<z ? y : z))

#ifndef MIN
# define MIN(x,y)     ((x)<(y) ? (x) : (y))
# define MAX(x,y)     ((x)>(y) ? (x) : (y))
#endif // MIN


//============================================================================
//! return true if value is a power of 2 value
VXMATH_INLINE int IsAPowerOf2( int iVal )
{
    int iBitCount = 0;
	int iBitsPerInt = sizeof(int) * 8;
    for( int i = 0; i < iBitsPerInt; i++ )
	if( iVal & ( 1 << i ) )
	{
	    iBitCount++;
	}
	return ( 1 == iBitCount )?1:0;
}


#define	PITCH	0 //sumersault facing view origin
#define	YAW		1 //rotate clockwise/counterclockwise
#define	ROLL	2 //sumersault facing left

enum AXIS
{
	XAXIS=0,
	YAXIS,
	ZAXIS,
	WAXIS
};

//fast rand functions
//! random number 0.0f to 1.0f
#define Rand0To1() (( rand() & 32767 ) * ( 1.0f / 32767.0f ))
//! random number 0.0f to n
#define Rand0ToNum(n) ((Rand0To1())*(n))
//! random number 1.0f to n
#define Rand1ToNum(n) ((Rand0To1())*((n)-1)+1)
//! random number -1.0f to 1.0f
#define RandNeg1To1() (( rand() & 32767 ) * ( 2.0 / 32767 ) - 1 )
//! random number -0.5f to 0.5f
#define RandNegPoint5ToPoint5() ((((float)rand())/RAND_MAX) - 0.05f) //Returns -0.5 to +0.5
//! random number 0.0f to 1.0f
#define Random       (((double)rand())/RAND_MAX)//Returns 0.0 - 1.0
//! random number -n to n
#define RandPlusMinus(range)    (range*(((float)rand())/RAND_MAX - 0.05f))//Returns -amount/2 to +amount/2

//============================================================================
//=== standard math replacement functions ( for speed reasons ) ===//
static VXMATH_INLINE long FastLog( long x )
{
	long a=0;
	while( x>>=1 )
		a++;
	return a;
}

//============================================================================
static VXMATH_INLINE float FastFabs( float f )
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
static VXMATH_INLINE long FastFtol(float f)
{
#ifdef _MSC_VER
    return (long)f;
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
#endif // _MSC_VER
}

//============================================================================
//fast way to make number the next power of 2
static VXMATH_INLINE int FastNextPow2(int Number)
{
#ifdef _MSC_VER
    return (int)pow(2, ceil(log(Number)/log(2)));
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
#endif // _MSC_VER
}
//nVidia FastSqrt
//table for fast sqrt
extern unsigned long g_au32FastSqrtTable[ 0x10000 ];
//floating point bits
#define F32Bits(fp) (*(unsigned *)&(fp))

//============================================================================
static VXMATH_INLINE float FastSqrtf( float x )
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
static VXMATH_INLINE float AngleModulus( float fAngle )
{
	return (float)(( 360.0/65536 ) * ((long)( fAngle * ( 65536/360.0 )) & 65535));
}

//============================================================================
static VXMATH_INLINE float LerpAngle( float fAngleA, float fAngleB, float fFraction )
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
static VXMATH_INLINE float ClampedAcosf(float x)
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
static VXMATH_INLINE float ClampedAsinf(float x)
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
static VXMATH_INLINE float vx_acos(float x)
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
static VXMATH_INLINE float vx_asin(float x)
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
static VXMATH_INLINE float vx_sqrt(float x)
{
    if (x<0.0f)
	{
		x = 0.0f;
	}

    return sqrtf(x);
}

//============================================================================
static VXMATH_INLINE bool vx_fequal(float f0, float f1, float tol)
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
static VXMATH_INLINE bool vx_fless(float f0, float f1, float tol)
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
static VXMATH_INLINE bool vx_fgreater(float f0, float f1, float tol)
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
VXMATH_INLINE int CountBitsSetInVar( unsigned long u32Var )
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
static VXMATH_INLINE unsigned long VxMakePowerOf2( unsigned long u32Val )
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
static VXMATH_INLINE unsigned long VxIncrementPowerOf2( unsigned long u32Val )
{
	// first to make sure starting value is a power of 2
	u32Val = VxMakePowerOf2( u32Val );
	u32Val <<= 1;
	return u32Val;
}

//============================================================================
//! increments Value to the next lowest power of 2
static VXMATH_INLINE unsigned long VxDecrementPowerOf2( unsigned long u32Val )
{
	u32Val = VxMakePowerOf2( u32Val );
	u32Val >>= 1;
	if( 0 == u32Val )
	{
		return 1;
	}

	return u32Val;
}

//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
extern const float vx_fast_atof_table[16];



#ifdef __cplusplus
#ifndef sincos
# define sincos( a, b, c ) (b=sin(a),c=cos(a))
# define sincosf( a, b, c ) (b=sinf(a),c=cosf(a))
#endif // sincos

//-------------------------------------------------------------------
//  random floating point with precision
//-------------------------------------------------------------------
VXMATH_INLINE float frand( int precis = 4 )
{
	int val=(int)pow(10.0f,precis);
	return (rand()%val)/(float)val;
}
//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
static VXMATH_INLINE void LimitTo360Degrees( float & f32Degrees )
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
static VXMATH_INLINE void LimitTo360Degrees( double & f64Degrees )
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

//-------------------------------------------------------------------
//! lerp function template
template<class T>
VXMATH_INLINE T vx_lerp( const T& a, const T&b, const T& t )
{
	return a - ( a*t ) + ( b*t );
}

//============================================================================
//! plagiarism of code by Nikolaus Gebhardt.. see irrlicht copywrite for details
//! Provides a fast function for converting a string into a float,
//! about 6 times faster than atof in win32.
// If you find any bugs, please send them to me, niko (at) irrlicht3d.org.
VXMATH_INLINE const char* vx_fast_atof_func( const char* c, float& out )
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
VXMATH_INLINE float vx_fast_atof( const char * c )
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

VXMATH_INLINE float frand( int precis )
{
	int val = ( int )pow( 10.0f, precis );
	return ( rand() % val ) / ( float )val;
}
//-------------------------------------------------------------------
//  limit value to + or - 360 degrees
//-------------------------------------------------------------------
static VXMATH_INLINE void LimitTo360DegreesF32( float * f32Degrees )
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
static VXMATH_INLINE void LimitTo360DegreesF64( double * f64Degrees )
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
VXMATH_INLINE const char * vx_fast_atof_func( const char * c, float * out )
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
VXMATH_INLINE float vx_fast_atof( const char * c )
{
	float ret;
	vx_fast_atof_func( c, &ret );
	return ret;
}


#endif // __cplusplus

