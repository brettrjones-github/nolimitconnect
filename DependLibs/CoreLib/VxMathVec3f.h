#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/config_corelib.h>
#include <math.h>
#include "VxMathDef.h"

//NOTE: if you use the class with .x etc instead of [0] etc the compiler generates faster smaller code
//TODO: All Vector classes should be made structures instead of classes so
//      Time isnt wasted on empty constructor ( call + 14 instructions )



//=== forward declarations ===//
//class Vec2s;
//class Vec3s;
//class Vec2i;
//class Vec3i;
//class Vec2f;

class Vec3f;
class Vec4f;

class Vec3f
{

public:
    union
	{
        struct  //stucture for cartesian format
		{
            float x;
            float y;
            float z;
        };
        struct //stucture for spherical format
		{
            float fAz;
            float fEl;
            float fRadius;
        };
        float v[3];
    };
	//=== constructors/destructors ===//
	//for speed sake dont clear in default constructor
	Vec3f(){}
    Vec3f( const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {};
    Vec3f( const Vec3f& vec) : x(vec.x), y(vec.y), z(vec.z) {};
    Vec3f( const float *p) : x(p[0]), y(p[1]), z(p[2]) {};

//    Vec3f( Vec3f& v) : x(v.x), y(v.y), z(v.z) {};


    //=== Access grants ===//
	//BUGBUG dont do this operator this way
	//compiler bug happens when times vector * matrix
	//float operator[](int index){ return v[index]; }
	VXMATH_INLINE float operator[](int index){ return (&x)[index]; }

    //=== operators ===//
    VXMATH_INLINE Vec3f& operator +=( const Vec3f& v0 )
	{
        x += v0.x;
        y += v0.y;
        z += v0.z;
        return *this;
    };
    VXMATH_INLINE Vec3f& operator -=(const Vec3f& v0)
	{
        x -= v0.x;
        y -= v0.y;
        z -= v0.z;
        return *this;
    };
    VXMATH_INLINE Vec3f& operator *=(float s)
	{
        x *= s;
        y *= s;
        z *= s;
        return *this;
    };
    VXMATH_INLINE Vec3f& operator *=( const Vec3f& v0 )
	{
        x *= v0.x;
        y *= v0.y;
        z *= v0.z;
        return *this;
    };
    VXMATH_INLINE Vec3f& operator /=(float s)
	{
        float one_over_s = 1.0f/s;
        x *= one_over_s;
        y *= one_over_s;
        z *= one_over_s;
        return *this;
    };
	VXMATH_INLINE bool operator==( const Vec3f& v0 )
	{
		if (x == v0.x && y == v0.y && z == v0.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
    //=== methods ===//
	bool IsZero( void );
	bool IsZero( float fTol );
	void Set( const float x1, const float y1, const float z1 );
	void Set( const Vec3f& v0 );
	void SetToOrigin( void );

    void Normalize(void);
    float Len(void);
    float LenSquared(void);
    bool IsEqualTo(const Vec3f& v0 );
    bool IsEqualTo(const Vec3f& v0, const float& fTolerance);
	void n_vec_rot_round_vec( Vec3f *vec, float *m );
	void Rotate( Vec3f *axis, float angle );
	void RotateX( float angle );
	void RotateY( float angle );
	void RotateZ( float angle );
	void Reset( void );


	void SetOrbitPosDeg( float fAzDeg, float fElDeg, float fDist );
	void SetOrbitPosRad( float fAzRad, float fElRad, float fDist );

	//=== spherical only functions ===//
	float GetAzRad( void );
	float GetAzDeg( void );
	float GetElRad( void );
	float GetElDeg( void );
	float GetRadius( void );

	//=== Director format functions ===//
	float GetPanRad( void );
	float GetPanDeg( void );
	float GetLiftRad( void );
	float GetLiftDeg( void );
	float GetZoom( void );


	Vec3f Min(Vec3f &);
	Vec3f Max(Vec3f &);
	Vec3f Rotate(float cos1, float sin1, float cos2, float sin2);
	Vec3f RevRotate(float cos1, float sin1, float cos2, float sin2);
    void CartesianToOrbital(void); //convert vector to spherical
    Vec3f OrbitalToCartesian(void);//  convert vector to rectangular from spherical
	Vec3f DirectorToCartesian( void );

	Vec3f ChangeLengthRectVect(float dLength);//change the length of a rect vector
		//by changing it to spherical, changing the length and changing it back to rect
	Vec3f RoundToResolution(float dResolution); //rounds x y and z down to the specified resolution
/*
	float Normalize( void )// Set this vector to a normalized version of itself
			{Vec3f o;float l,f;l=(float)sqrt(x*x+y*y+z*z);if(l){f=1/l;x=x*f;y=y*f;z=z*f;}return l;}
	Vec3f Normal( void ) //returns normalized version of this vector
			{Vec3f o;float l,f;l=(float)sqrt(x*x+y*y+z*z);if(l){f=1/l;o.x=x*f;o.y=y*f;o.z=z*f;}return o;}
*/
};
//=====================================================
// Vec3f macros
//=====================================================
//
// these macros in general generate smaller code and much faster than member functions
//
#define Vec3fSetToMax(v)		(v.x = MAX_F32, v.y = MAX_F32, v.z = MAX_F32 )
#define Vec3fSetToMin(v)		(v.x = MIN_F32, v.y = MIN_F32, v.z = MIN_F32 )
#define Vec3fAbs(a, b)			(b.x=(a.x>=0.0f)?a.x:-a.x,b.y=(a.y>=0.0f)?a.y:-a.y,b.z=(a.z>=0.0f)?a.z:-a.z )
#define Vec3fSet(f, g, h, v)	(v.x =(f),v.y=(g),v.z=(h));
#define Vec3fNegate(a,b)		(b.x=-a.x,b.y=-a.y,b.z=-a.z)
#define Vec3fDot(a,b)			(a.x*b.x+a.y*b.y+a.z*b.z)
#define Vec3fCopy(a,b)			((b).x=(a).x,(b).y=(a).y,(b).z=(a).z)
#define Vec3fSub(c,a,b)			(c.x=a.x-b.x,c.y=a.y-b.y,c.z=a.z-b.z)
#define Vec3fAdd(c,a,b)			(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z)
#define Vec3fMult(c,a,b)		(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z)
#define Vec3fComb(c,A,a,B,b)	(c.x=(A)*a.x+(B)*b.x,c.y=(A)*a.y+(B)*b.y,c.z=(A)*a.z+(B)*b.z)
#define Vec3fScale(A,a,b)		(b.x=(A)*a.x,b.y=(A)*a.y,b.z=(A)*a.z)
#define Vec3fScaleAdd(c,A,a,b)	(c.x=(A)*a.x+b.x,c.y=(A)*a.y+b.y,c.z=(A)*a.z+b.z)
#define Vec3fCross(c,a,b)		(c.x=a.y*b.z-a.z*b.y,c.y=a.z*b.x-a.x*b.z,c.z=a.x*b.y-a.y*b.x)
#define Vec3fClear(v)			(v.x=0.0f,v.y=0.0f,v.z=0.0f)
#define Vec3fLen(a)				(sqrtf(Vec3fDot(a,a)))
#define Vec3fLenSquared(a)		(a.x*a.x+a.y*a.y+a.z*a.z)
#define Vec3fEqual(a,b)			((a.x==b.x && a.y==b.y && a.z==b.z)?1:0)
#define Vec3fIsZero(v)			((v.x==0.0f && v.y==0.0f && v.z==0.0f)?1:0)
#define Vec3fMin(c,a,b)		(c.x = MIN(a.x,b.x),c.y=MIN(a.y,b.y),c.z=MIN(a.z,b.z))
#define Vec3fMax(c,a,b)		(c.x = MAX(a.x,b.x),c.y=MAX(a.y,b.y),c.z=MAX(a.z,b.z))
#define Vec3fClampAngle( v )	(ClampAngle(v.x), ClampAngle(v.y), ClampAngle(v.z))
//=====================================================
// Vec3f functions not part of Vec3f class
//=====================================================
//VXMATH_INLINE float Vec3fNormal(Vec3f a,Vec3f o)
//	{float l,f;l=(float)Vec3fLen(a);if(l){f=1/l;o.x=a.x*f;o.y=a.y*f;o.z=a.z*f;}return f;}
VXMATH_INLINE float Vec3fNormal( Vec3f & r, Vec3f & v )
{
    float l;
    l = Vec3fLen( v );
    if( l > TINY )
	{
		//slight speed improvement use 1/ instead of 3 divide ops
//        x /= l;
//        y /= l;
 //       z /= l;
        l = 1 / l;
        r.x = v.x * l;
        r.y = v.y * l;
        r.z = v.z * l;
		return l;
    }
    else
	{
        r.x = 0.0f;
        r.y = 1.0f;
        r.z = 0.0f;
		return 1.0f;
    }
}

void Vec3fSetOrbitPosRad( float fRadianAz, float fRadianEl, float fDist, Vec3f &vRetCartesian );
void Vec3fOrbitalToCartesian( Vec3f &vOrbital, Vec3f &vRetCartesian );
void Vec3fCartesianToOrbital( Vec3f &vCaresian, Vec3f &vRetOrbital );
void Vec3fDirectorToCartesian( Vec3f &vDirector, Vec3f &vRetCartesian );
void Vec3fCartesianToDirector( Vec3f &vCartesian, Vec3f &vRetDirector );


//=====================================================
// Vec3f operators
//=====================================================
//add operator
VXMATH_INLINE Vec3f operator +(const Vec3f& v0, const Vec3f& v1)
{
    return Vec3f(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
};
//subtract operator
VXMATH_INLINE Vec3f operator -(const Vec3f& v0, const Vec3f& v1)
{
    return Vec3f(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
};
//multiply operator
VXMATH_INLINE Vec3f operator *(const Vec3f& v0, const float s)
{
    return Vec3f(v0.x*s, v0.y*s, v0.z*s);
};
//multiply operator
VXMATH_INLINE Vec3f operator* ( const float s, const Vec3f& v0 )
{
    return Vec3f(v0.x*s, v0.y*s, v0.z*s);
}
//divide operator
VXMATH_INLINE Vec3f operator /(const Vec3f& v0, const float s)
{
    float fOneOver = 1.0f/s;
    return Vec3f(v0.x*fOneOver, v0.y*fOneOver, v0.z*fOneOver);
};
//negate operator
VXMATH_INLINE Vec3f operator-(const Vec3f& v)
{
    return Vec3f(-v.x,-v.y,-v.z);
};
//dot product
VXMATH_INLINE float operator %(const Vec3f& v0, const Vec3f& v1)
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
};
//cross product
VXMATH_INLINE Vec3f operator ^(const Vec3f& v0, const Vec3f& v1)
{
    return Vec3f(v0.y*v1.z - v0.z*v1.y,
                   v0.z*v1.x - v0.x*v1.z,
                   v0.x*v1.y - v0.y*v1.x);
};







//=====================================================
// Vec3f methods
//=====================================================
VXMATH_INLINE bool Vec3f::IsZero( void )
{
	return((x==0.0f && y==0.0f && y==0.0f )?1:0);
} // return true if x, y, and z is zero
VXMATH_INLINE bool Vec3f::IsZero( float fTol )
{
    if( FastFabs( x ) > fTol )
	{
		return false;
	}
    else if ( FastFabs( y ) > fTol )
	{
		return false;
	}
    else if ( FastFabs( z ) > fTol )
	{
		return false;
	}
    return true;
} // return true if x, y, and z is zero

VXMATH_INLINE void Vec3f::Set(float _x, float _y, float _z)
{
    x=_x; y=_y; z=_z;
};
VXMATH_INLINE void Vec3f::Set( const Vec3f& v)
{
    x=v.x; y=v.y; z=v.z;
};
VXMATH_INLINE void Vec3f::SetToOrigin( void )
{
    x=0.0f; y=0.0f; z=0.0f;
};
VXMATH_INLINE float Vec3f::Len(void)
{
    return (float) sqrt(x*x+y*y+z*z);
};
VXMATH_INLINE float Vec3f::LenSquared(void)
{
    return x*x+y*y+z*z;
};


VXMATH_INLINE void Vec3f::Normalize()
{
    float l;
    l = Len();
    if( l > TINY )
	{
		//slight speed improvement use 1/ instead of 3 divide ops
//        x /= l;
//        y /= l;
 //       z /= l;
        l = 1 / l;
        x = x * l;
        y = y * l;
        z = z * l;
    }
    else
	{
        x = 0.0f;
        y = 1.0f;
        z = 0.0f;
    }
}
VXMATH_INLINE bool Vec3f::IsEqualTo(const Vec3f& v0 )
{
	return((x==v0.x && y==v0.x && z==v0.z )?1:0);
}
//which is faster ?
VXMATH_INLINE bool Vec3f::IsEqualTo(const Vec3f& v0, const float& fTol)
{
    if( FastFabs(v0.x-x) > fTol )
	{
		return false;
	}
    else if ( FastFabs( v0.y-y ) > fTol )
	{
		return false;
	}
    else if ( FastFabs(v0.z-z) > fTol )
	{
		return false;
	}
    return true;
};
/*
VXMATH_INLINE bool Vec2f::IsEqualTo(const Vec2f& v0, const float& fTol )
{
    if (x < v0.x + fTol &&
        x > v0.x - fTol &&
        y < v0.y + fTol &&
        y > v0.y - fTol &&
        z < v0.z + fTol &&
        z > v0.z - fTol )
	{
        return true;
	}
    else
	{
        return false;
	}
};*/

VXMATH_INLINE  void Vec3f::n_vec_rot_round_vec( Vec3f *vec, float *m )
{
	Vec3f v;
	v = (*vec);

	vec->x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
	vec->y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
	vec->z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
}

VXMATH_INLINE void Vec3f::Rotate( Vec3f *axis, float angle )
{
	float rot_m[9];
	Vec3f axis2;
	float sa, ca;

	sa = (float) sin( angle );
	ca = (float) cos( angle );

	axis2 = (*axis);
	axis2.Normalize();

	rot_m[0] = ca + (1 - ca) * axis2.x * axis2.x;
	rot_m[1] = (1 - ca) * axis2.x * axis2.y - sa * axis2.z;
	rot_m[2] = (1 - ca) * axis2.z * axis2.x + sa * axis2.y;
	rot_m[3] = (1 - ca) * axis2.x * axis2.y + sa * axis2.z;
	rot_m[4] = ca + (1 - ca) * axis2.y * axis2.y;
	rot_m[5] = (1 - ca) * axis2.y * axis2.z - sa * axis2.x;
	rot_m[6] = (1 - ca) * axis2.z * axis2.x - sa * axis2.y;
	rot_m[7] = (1 - ca) * axis2.y * axis2.z + sa * axis2.x;
	rot_m[8] = ca + (1 - ca) * axis2.z * axis2.z;

	n_vec_rot_round_vec( this, rot_m );
}


VXMATH_INLINE void Vec3f::RotateX( float a )
{
	float rot_m[9];

	rot_m[0] = 1.0f; rot_m[1] = 0.0f;            rot_m[2] =  0.0f;
	rot_m[3] = 0.0f; rot_m[4] = (float) cos(a); rot_m[5] = -(float) sin(a);
	rot_m[6] = 0.0f; rot_m[7] = (float) sin(a); rot_m[8] =  (float) cos(a);

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec3f::RotateY( float a )
{
	float rot_m[9];

	rot_m[0] = (float) cos(a);  rot_m[1] = 0.0f; rot_m[2] = (float) sin(a);
	rot_m[3] = 0.0f;            rot_m[4] = 1.0f; rot_m[5] = 0.0f;
	rot_m[6] = -(float) sin(a); rot_m[7] = 0.0f; rot_m[8] = (float) cos(a);

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec3f::RotateZ( float a )
{
	float rot_m[9];

	rot_m[0] = (float) cos(a); rot_m[1] = -(float) sin(a); rot_m[2] = 0.0f;
	rot_m[3] = (float) sin(a); rot_m[4] =  (float) cos(a); rot_m[5] = 0.0f;
	rot_m[6] = 0.0f;           rot_m[7] = 0.0f;            rot_m[8] = 1.0f;

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec3f::Reset( void )
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}





VXMATH_INLINE void Vec3f::SetOrbitPosRad( float fRadAz, float fRadEl, float fDist )
{ Vec3fSetOrbitPosRad( fRadAz , fRadEl , fDist, *this );}

VXMATH_INLINE void Vec3f::SetOrbitPosDeg( float fDegAz, float fDegEl, float fDist )
	{ Vec3fSetOrbitPosRad( DegToRadf( fDegAz ), DegToRadf( fDegEl ), fDist, *this );}

VXMATH_INLINE void Vec3f::CartesianToOrbital( void )
	{ Vec3fCartesianToOrbital( *this, *this ); }

VXMATH_INLINE Vec3f Vec3f::OrbitalToCartesian( void )
	{ Vec3f vResult; Vec3fOrbitalToCartesian( *this, vResult ); return vResult; }

VXMATH_INLINE Vec3f Vec3f::DirectorToCartesian( void )
	{ Vec3f vResult; Vec3fDirectorToCartesian( *this, vResult ); return vResult; }

//=== Orbital ( Obital ) format ===//
VXMATH_INLINE float Vec3f::GetAzRad( void )
	{ return x;}

VXMATH_INLINE float Vec3f::GetAzDeg( void )
	{ return RadToDegf( x );}

VXMATH_INLINE float Vec3f::GetElRad( void )
	{ return y;}

VXMATH_INLINE float Vec3f::GetElDeg( void )
	{ return RadToDegf( y );}

VXMATH_INLINE float Vec3f::GetRadius( void )
	{ return z;}

//=== Director format functions ===//
VXMATH_INLINE float Vec3f::GetPanRad( void )
	{ return x;}

VXMATH_INLINE float Vec3f::GetPanDeg( void )
	{ return RadToDegf( x );}

VXMATH_INLINE float Vec3f::GetLiftRad( void )
	{ return y;}

VXMATH_INLINE float Vec3f::GetLiftDeg( void )
	{ return RadToDegf( y );}

VXMATH_INLINE float Vec3f::GetZoom( void )
	{ return z;}

//=== NLC_INLINE functions ===//
VXMATH_INLINE float Vec3fNormalize (Vec3f v)
	{float l,f;l=(float)Vec3fLen(v);if(l){f=1/l;v.x*=f;v.y*=f;v.z*=f;}return l;}


VXMATH_INLINE void Vec3fOrbitalToCartesianRad( Vec3f &vIn, Vec3f &vo )
{	double r = vIn.fRadius*cos(vIn.fEl); //get the xy radius
	vo.z = -(float)(vIn.fRadius*sin(vIn.fEl));
	vo.y = (float)(r*cos(vIn.fAz));
	vo.x = (float)(r*sin(vIn.fAz));
}
VXMATH_INLINE void Vec3fOrbitalToCartesianDeg( Vec3f &vIn, Vec3f &vo )
{	float fElRad = DegToRadf( vIn.fEl );
	float fAzRad = DegToRadf( vIn.fAz );
	double r = vIn.fRadius*cos( fElRad ); //get the xy radius
	vo.z = -(float)(vIn.fRadius*sin( fElRad ));
	vo.y = (float)(r*cos( fAzRad ));
	vo.x = (float)(r*sin( fAzRad ));
}

VXMATH_INLINE void Vec3fDirectorToCartesian( Vec3f &vIn, Vec3f &vRetVec )
{
    vRetVec.x = (float)(vIn.fRadius * cos( vIn.fAz ))	;
    vRetVec.y = (float)(vIn.fRadius * sin( vIn.fAz ));
    vRetVec.z = (float)(vIn.fRadius * sin( vIn.fEl ));
}

VXMATH_INLINE void Vec3fRotateRad( Vec3f &vIn, Vec3f &vAngles, Vec3f &vOut )
{
	float fX = vIn.x;
	float fY = vIn.y;
	float fZ = vIn.z;

	float fCosHeading = (float) cos( vAngles.x );
	float fSinHeading = (float) sin( vAngles.x );
	float fCosPitch = (float) cos( vAngles.y );
	float fSinPitch = (float) sin( vAngles.y);
	float fCosRoll = (float) cos( vAngles.z );
	float fSinRoll = (float) sin( vAngles.z );

	fX = -1 * fX;
	float fXa = fCosPitch * fX - fSinPitch * fZ;
	float fZa = fSinPitch * fX + fCosPitch * fZ;
	vOut.x =fCosRoll * fXa + fSinRoll * fY;
	float fYa = fCosRoll * fY - fSinRoll * fXa;
	vOut.z = fCosHeading * fZa - fSinHeading * fYa;
	vOut.y = fSinHeading * fZa + fCosHeading * fYa;
}
VXMATH_INLINE void Vec3fRotateDeg( Vec3f &vIn, Vec3f &vAngles, Vec3f &vOut )
{
	float fX = DegToRadf( vIn.x );
	float fY = DegToRadf( vIn.y );
	float fZ = DegToRadf( vIn.z );

	float fCosHeading = (float) cos( vAngles.x );
	float fSinHeading = (float) sin( vAngles.x );
	float fCosPitch = (float) cos( vAngles.y );
	float fSinPitch = (float) sin( vAngles.y);
	float fCosRoll = (float) cos( vAngles.z );
	float fSinRoll = (float) sin( vAngles.z );

	fX = -1 * fX;
	float fXa = fCosPitch * fX - fSinPitch * fZ;
	float fZa = fSinPitch * fX + fCosPitch * fZ;
	vOut.x =fCosRoll * fXa + fSinRoll * fY;
	float fYa = fCosRoll * fY - fSinRoll * fXa;
	vOut.z = fCosHeading * fZa - fSinHeading * fYa;
	vOut.y = fSinHeading * fZa + fCosHeading * fYa;
}
VXMATH_INLINE void Vec3fOrbitPointDeg( Vec3f &vPlanet, Vec3f &vAnglesDeg, Vec3f &vSun, Vec3f &vOut )
{
	//translate to origin
	Vec3fSub( vPlanet, vSun, vOut);
	//rotate to position
	Vec3fRotateDeg( vOut, vAnglesDeg, vOut );
	//transltate back
	Vec3fAdd( vOut, vSun, vOut);
}


VXMATH_INLINE void Vec3fAbsOrbitXAxisRad( Vec3f &vVulture, float fAngleRad, Vec3f &vOut )
{
	vOut.x = vVulture.x; //x distance remains the same
	float fRadius = (float) sqrt( vVulture.y * vVulture.y + vVulture.z * vVulture.z );
	float fOriginalAngleRad = (float)( atan2( vVulture.y, vVulture.z ) );
	fAngleRad += fOriginalAngleRad;
	//rotate to position
	vOut.y = (float) sin( fAngleRad ) * fRadius;
	vOut.z = (float) cos( fAngleRad ) * fRadius;
}

VXMATH_INLINE void Vec3fAbsOrbitYAxisRad( Vec3f &vVulture, float fAngleRad, Vec3f &vOut )
{
	vOut.y = vVulture.y; //y distance remains the same
	float fRadius = (float) sqrt( vVulture.x * vVulture.x + vVulture.z * vVulture.z );
	float fOriginalAngleRad = (float)( atan2( vVulture.x, vVulture.z ) );
	fAngleRad += fOriginalAngleRad;
	//rotate to position
	vOut.z = (float) sin( fAngleRad ) * fRadius;
	vOut.x = (float) cos( fAngleRad ) * fRadius;
}

VXMATH_INLINE void Vec3fAbsOrbitZAxisRad( Vec3f &vVulture, float fAngleRad, Vec3f &vOut)
{
	vOut.z= vVulture.z; //z distance remains the same
	float fRadius = (float) sqrt( vVulture.x * vVulture.x + vVulture.y * vVulture.y );
	float fOriginalAngleRad = (float)( atan2( vVulture.x, vVulture.y ) );
	fAngleRad += fOriginalAngleRad;
	//rotate to position
	vOut.x = (float) sin( fAngleRad ) * fRadius;
	vOut.y = (float) cos( fAngleRad ) * fRadius;
}


VXMATH_INLINE void Vec3fOrbitPointsXAxis( Vec3f &vVulture, float fAngle, Vec3f &vMeat, Vec3f &vOut )
{
	//translate y,z to origin
	vOut.x = vVulture.x; //x distance remains the same
	vOut.y = vVulture.y - vMeat.y;
	vOut.z = vVulture.z - vMeat.z;
	float fRadius = (float) sqrt( vOut.y * vOut.y + vOut.z * vOut.z );
	float fOriginalAngleRad = (float)( atan2( vOut.y, vOut.z ) );
	//rotate to position
	vOut.y = (float) sin( fAngle + fOriginalAngleRad ) * fRadius;
	vOut.z = (float) cos( fAngle + fOriginalAngleRad ) * fRadius;
	//translate back out
	vOut.y = vOut.y + vMeat.y;
	vOut.z = vOut.z + vMeat.z;

}

VXMATH_INLINE void Vec3fOrbitPointsYAxis( Vec3f &vVulture, float fAngle, Vec3f &vMeat, Vec3f &vOut )
{
	//translate z,x to origin
	vOut.x = vVulture.x - vMeat.x;
	vOut.y = vVulture.y; //y distance remains the same
	vOut.z = vVulture.z - vMeat.z;
	float fRadius = (float) sqrt( vOut.x * vOut.x + vOut.z * vOut.z );
	float fOriginalAngleRad = (float)( atan2( vOut.z, vOut.x ) );
	//rotate to position
	vOut.z = (float) sin( fAngle + fOriginalAngleRad  ) * fRadius;
	vOut.x = (float) cos( fAngle + fOriginalAngleRad  ) * fRadius;
	//translate back out
	vOut.z = vOut.z + vMeat.z;
	vOut.x = vOut.x + vMeat.x;
}

VXMATH_INLINE void Vec3fOrbitPointsZAxis( Vec3f &vVulture, float fAngleRad, Vec3f &vMeat, Vec3f &vOut )
{
	//translate x y to origin
	vOut.x = vVulture.x - vMeat.x;
	vOut.y = vVulture.y - vMeat.y;
	vOut.z= vVulture.z; //z distance remains the same
	float fRadius = (float) sqrt( vOut.x * vOut.x + vOut.y * vOut.y );
	float fOriginalAngleRad = (float)( atan2( vOut.x, vOut.y ) );
	//rotate to position
	vOut.x = (float) sin( fAngleRad + fOriginalAngleRad ) * fRadius;
	vOut.y = (float) cos( fAngleRad + fOriginalAngleRad ) * fRadius;
	//translate back out
	vOut.x = vOut.x + vMeat.x;
	vOut.y = vOut.y + vMeat.y;
}

//calculate heading needed to arive at destination cartesian pos from origin cartesian position
VXMATH_INLINE float Vec3fCalcHeadingDeg( Vec3f &vOrigin, Vec3f &vDest )
{
	float fHeading;
	Vec3f vDif;
	Vec3fSub( vOrigin, vDest, vDif );
	if( vDif.x == 0 && vDif.y == 0)
	{
		fHeading = 0;
	}
	else
	{
		fHeading = RadToDegf( (float)( atan2( vDif.x, vDif.y ) ));
		if( fHeading < 0 )
		{
			fHeading += 360;
		}
	}
	return fHeading;
}
//same as above but the difference has already been calculated
//calculate heading needed to arive at destination give a difference vector of origin - dest
VXMATH_INLINE float Vec3fCalcHeadingDeg( Vec3f &vDif )
{
	float fHeading;
	if( vDif.x == 0 && vDif.y == 0)
	{
		fHeading = 0;
	}
	else
	{
		fHeading = RadToDegf( (float)( atan2( vDif.x, vDif.y ) ));
		if( fHeading < 0 )
		{
			fHeading += 360;
		}
	}
	return fHeading;
}



