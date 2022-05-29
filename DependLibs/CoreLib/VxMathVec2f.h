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

#ifndef MATHVEC2F_H
#define MATHVEC2F_H
#include <CoreLib/config_corelib.h>

#include <stdlib.h>
#include <math.h>
#include <float.h>


class Vec2f 
{
public:
    union 
	{
        struct  //stucture for cartesian format
		{
            float x;
            float y;
        };
        float v[2];
    };

    //=== Constructors ===//
	//for speed sake the variables are not initialized with default constructor
	Vec2f (){};
	Vec2f(const float _x, const float _y) : x(_x), y(_y) {};  
    Vec2f(const Vec2f& vec) : x(vec.x), y(vec.y) {};
	Vec2f( float f[2] ){ x = f[0]; y = f[1]; }
	Vec2f( float f ){x = f; y = f;}

//  Vec2f( float _x, float _y) : x(_x), y(_y) {};  
//	Vec2f(Vec2f & v){ x = v.x, y = v.y;}
//  Vec2f( float *p ) : x(p[0]), y(p[1]) {};
 
    //=== Access grants ===//
	//BUGBUG dont do this operator this way
	//compiler bug happens when times vector * matrix 
	//float operator[](int index){ return v[index]; }
	VXMATH_INLINE float operator[](int index){ return (&x)[index]; }

    //=== operators ===//
	VXMATH_INLINE Vec2f& operator+=(const Vec2f& v0) 
	{
		x += v0.x;
		y += v0.y;
		return *this;
	};
	VXMATH_INLINE Vec2f& operator-=(const Vec2f& v0) 
	{
		x -= v0.x;
		y -= v0.y;
		return *this;
	};
	VXMATH_INLINE Vec2f& operator*=(const float s) 
	{
		x *= s;
		y *= s;
		return *this;
	};
	VXMATH_INLINE Vec2f& operator/=(const float s) 
	{
		x /= s;
		y /= s;
		return *this;
	};
	VXMATH_INLINE bool operator==( const Vec2f& v0 ) 
	{
		if (x == v0.x && y == v0.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	//=== methods ===//				
	bool IsZero();
	bool IsZero( float fTol );
    void Normalize(void); 
	void Set( const float x1, const float y1 );
	void Set( const Vec2f v0 );
	void SetToOrigin( void );
    float Len(void); 
    bool IsEqualTo(const Vec2f& v0 );
    bool IsEqualTo(const Vec2f& v0, const float& fTolerance);
	void Reset( void );
};
//=====================================================
// Vec2f macros
//=====================================================
//
// these macros in general generate smaller code and much faster than member functions
//
#define Vec2fSetToMax(v)		(v.x = MAX_F32, v.y = MAX_F32 )
#define Vec2fSetToMin(v)		(v.x = MIN_F32, v.y = MIN_F32 )
#define Vec2fAbs(a, b)			(b.x=(a.x>=0.0f)?a.x:-a.x,b.y=(a.y>=0.0f)?a.y:-a.y )
#define Vec2fSet(f, g, v)		(v.x =(f),v.y=(g));
#define Vec2fNegate(a,b)		(b.x=-a.x,b.y=-a.y)
#define Vec2fCopy(a,b)			((b).x=(a).x,(b).y=(a).y)
#define Vec2fSub(a,b,c)			(c.x=a.x-b.x,c.y=a.y-b.y)
#define Vec2fAdd(a,b,c)			(c.x=a.x+b.x,c.y=a.y+b.y)
#define Vec2fMult(a,b,c)		(c.x=a.x*b.x,c.y=a.y*b.y)
#define Vec2fComb(A,a,B,b,c)	(c.x=(A)*a.x+(B)*b.x,c.y=(A)*a.y+(B)*b.y)
#define Vec2fScale(A,a,b)		(b.x=(A)*a.x,b.y=(A)*a.y)
#define Vec2fScaleAdd(A,a,b,c)	(c.x=(A)*a.x+b.x,c.y=(A)*a.y+b.y)
#define Vec2fClear(v)			(v.x=0.0f,v.y=0.0f)
#define Vec2fLen(a)				(sqrt(a.x*a.x+a.y*a.y))
#define Vec2fLenSquared(a)		(a.x*a.x+a.y*a.y)
#define Vec2fEqual(a,b)			((a.x==b.x && a.y==b.y)?1:0)
#define Vec2fIsZero(v)			((v.x==0.0f && v.y==0.0f)?1:0)
#define Vec2fMin( a,b,o)		(o.x = MIN(a.x,b.x),o.y=MIN(a.y,b.y))
#define Vec2fMax( a,b,o)		(o.x = MAX(a.x,b.x),o.y=MAX(a.y,b.y))
#define Vec2fClampAngle( v )	(ClampAngle(v.x), ClampAngle(v.y))

//=====================================================
// Vec2f operators
//=====================================================
//add operator
VXMATH_INLINE Vec2f operator+(const Vec2f& v0, const Vec2f& v1) 
{
    return Vec2f(v0.x+v1.x, v0.y+v1.y); 
};
//subtract operator
VXMATH_INLINE Vec2f operator-(const Vec2f& v0, const Vec2f& v1) 
{
    return Vec2f(v0.x-v1.x, v0.y-v1.y); 
};
//multiply operator
VXMATH_INLINE Vec2f operator*(const Vec2f& v0, const Vec2f& v1) 
{
    return Vec2f(v0.x*v1.x, v0.y*v1.y); 
};
//multiply operator
VXMATH_INLINE Vec2f operator*( const float s, const Vec2f& v0) 
{
    return Vec2f(v0.x*s, v0.y*s);
};
//multiply operator
VXMATH_INLINE Vec2f operator*(const Vec2f& v0, const float s) 
{
    return Vec2f(v0.x*s, v0.y*s);
};
//divide operator
VXMATH_INLINE Vec2f operator/(const Vec2f& v0, const float s) 
{
    return Vec2f(v0.x/s, v0.y/s);
};
//negate operator
VXMATH_INLINE Vec2f operator-(const Vec2f& v) 
{
    return Vec2f(-v.x,-v.y);
};

//=====================================================
// Vec2f methods
//=====================================================

VXMATH_INLINE bool Vec2f::IsZero()
{
	return((x==0.0f && y==0.0f )?1:0);
} // return true if x, y, and z is zero
VXMATH_INLINE bool Vec2f::IsZero( float fTol )
{
    if( FastFabs( x ) > fTol )
	{
		return false;
	}
    else if ( FastFabs( y ) > fTol )
	{
		return false;
	}
    return true;
} // return true if x, y, and z is zero

VXMATH_INLINE void Vec2f::Set( const float x1, const float y1 )
{
	x = x1; 
	y = y1;
};//initialize the vector with new params

VXMATH_INLINE void Vec2f::Set( const Vec2f v0 )
{
	x = v0.x; 
	y = v0.x;
};//initialize the vector with new params
VXMATH_INLINE void Vec2f::SetToOrigin( void )
{
	x = 0.0f; 
	y = 0.0f;
};

VXMATH_INLINE float Vec2f::Len(void) 
{
    return (float) sqrt(x*x+y*y);
};
VXMATH_INLINE void Vec2f::Normalize(void) 
{
    float l = Len();
    if (l > TINY) 
	{
        x /= l;
        y /= l;
    } 
	else 
	{
        x = 0.0f;
        y = 1.0f;
    }
};
VXMATH_INLINE bool Vec2f::IsEqualTo(const Vec2f& v0 )
{
	return((x==v0.x && y==v0.x )?1:0);
}
//which is faster ?
VXMATH_INLINE bool Vec2f::IsEqualTo(const Vec2f& v0, const float& fTol)
{
    if( FastFabs(v0.x-x) > fTol )
	{
		return false;
	}
    else if ( FastFabs( v0.y-y ) > fTol )
	{
		return false;
	}
    return true;
};
/*
VXMATH_INLINE bool Vec2f::IsEqualTo(const Vec2f& v0, const float& fTol)
{
    if (x < v0.x + fTol &&
        x > v0.x - fTol &&
        y < v0.y + fTol &&
        y > v0.y - fTol)
	{
        return true;
	}
    else
	{
        return false;
	}
};*/

/*
VXMATH_INLINE bool Vec2f::IsEqualTo(const Vec2f& v, const float tol) 
{
    float dx = n_abs(v.x-x);
    float dy = n_abs(v.y-y);
    if (dx > tol)
	{
		return false;
	}
    else if (dy > tol) 
	{
		return false;
	}
    return true;
};*/
VXMATH_INLINE void Vec2f::Reset( void )
{
    x = 0.0f;
    y = 0.0f;
}

#endif //MATHVEC2F
