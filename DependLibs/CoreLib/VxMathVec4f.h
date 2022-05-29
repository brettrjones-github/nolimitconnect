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

#ifndef MATHVEC4F_H
#define MATHVEC4F_H
#include <CoreLib/config_corelib.h>

//==============================================================================
//
// Vec4f
//
//==============================================================================



class Vec4f
{
public:
    union 
	{
        struct 
		{
            float x;
            float y;
            float z;
			float w;
        };
        float v[4];
    };

    //=== Constructors ===//
	//for speed sake the variables are not initialized with default constructor
	VXMATH_INLINE Vec4f(){}; 
	VXMATH_INLINE Vec4f( const float f ){x = f; y = f; z = f; w = f; }
  	VXMATH_INLINE Vec4f( const float x1, const float y1, const float z1 ) : x(x1), y(y1), z(z1), w(0) {}
    VXMATH_INLINE Vec4f( const float x1, const float y1, const float z1, const float w1) : x(x1), y(y1), z(z1), w(w1) {}; 
	VXMATH_INLINE Vec4f( const Vec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
    VXMATH_INLINE Vec4f( float *p ) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {};

//	VXMATH_INLINE Vec4f( float x1, float y1, float z1, float w1 ){x = x1; y = y1; z = z1; w = w1;}

    //=== Access grants ===//
	//BUGBUG dont do this operator this way
	//compiler bug happens when times vector * matrix 
	//float operator[](int index){ return v[index]; }
	VXMATH_INLINE float operator[](int index){ return (&x)[index]; }


    //=== operators ===//
	//equal operator
	VXMATH_INLINE Vec4f& operator =(const Vec3f& v ) 
	{
        x=v.x; y=v.y; z=v.z; w=0.0f; 
		return *this;
	};
    VXMATH_INLINE Vec4f& operator+=(const Vec4f& v) 
	{
        x+=v.x; y+=v.y; z+=v.z; w+=v.z;
        return *this;
    };
    VXMATH_INLINE Vec4f& operator-=(const Vec4f& v) 
	{
        x-=v.x; y-=v.y; z-=v.z; w-=v.z;
        return *this;
    };
    VXMATH_INLINE Vec4f& operator*=(const float s) 
	{
        x*=s; y*=s; z*=s; w*=s;
        return *this;
    };
    VXMATH_INLINE Vec4f& operator/=(const float s) 
	{
        float fOneOver = 1.0f/s;
        x*=fOneOver; y*=fOneOver; z*=fOneOver; w*=fOneOver;
        return *this;
    };
    //=== methods ===//
	bool IsZero( void );
	bool IsZero( float fTol );
	void Set( const float x1, const float y1, const float z1 );
	void Set( const float x1, const float y1, const float z1, const float w1 );
	void Set( const Vec3f& v0 );
 	void Set( const Vec4f& v0 );
    void Normalize(void); 
    float Len(void); 
    float LenSquared(void); 
    bool IsEqualTo(const Vec3f& v0 );
    bool IsEqualTo(const Vec4f& v0 );
    bool IsEqualTo(const Vec3f& v0, const float& fTolerance);
    bool IsEqualTo(const Vec4f& v0, const float& fTolerance);
	void n_vec_rot_round_vec( Vec3f *vec, float *m );
	void n_vec_rot_round_vec( Vec4f *vec, float *m );
	void Rotate( Vec3f *axis, float angle );
	void Rotate( Vec4f *axis, float angle );
	void RotateX( float angle );
	void RotateY( float angle );
	void RotateZ( float angle );
	void Reset( void );

 


	void Homogenize( void )	{	x /= w;	y /= w;	z /= w;	}
/*
	float Normalize( void )// set this vector to a normalized version of itself
			{Vec4f o;float l,f;l=(float)sqrt(x*x+y*y+z*z);if(l){f=1/l;x=x*f;y=y*f;z=z*f;}return l;}
	Vec4f Normal( void ) //returns normalized version of this vector
			{Vec4f o;float l,f;l=(float)sqrt(x*x+y*y+z*z);if(l){f=1/l;o.x=x*f;o.y=y*f;o.z=z*f;}return o;}
*/
};


//=== Vector 4 Macros ===// 
#define Vec4fSetToMax(v)		(v.x = MAX_F32, v.y = MAX_F32, v.z = MAX_F32, v.w = MAX_F32 )
#define Vec4fSetToMin(v)		(v.x = MIN_F32, v.y = MIN_F32, v.z = MIN_F32, v.w = MAX_F32 )
#define Vec4fAbs(a,b)			(b.x=(a.x>=0.0f)?a.x:-a.x,b.y=(a.y>=0.0f)?a.y:-a.y,b.z=(a.z>=0.0f)?a.z:-a.z,b.w=(a.w>=0.0f)?a.w:-a.w )
#define Vec4fSet( f, g, h, i, v)	(v.x=(f),v.y=(g),v.z=(h),v.w=(i))
#define Vec4fNegate(a,b)		(b.x=-a.x,b.y=-a.y,b.z=-a.z,b.w=-a.w)
#define Vec4fDot(a,b)			(a.x*b.x+a.y*b.y+a.z*b.z)
#define Vec4fCopy(a,b)			(b.x=a.x,b.y=a.y,b.z=a.z,b.w=a.w)
#define Vec4fSub(a,b,c)			(c.x=a.x-b.x,c.y=a.y-b.y,c.z=a.z-b.z,c.w=a.w-b.w)
#define Vec4fAdd(a,b,c)			(c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z,c.w=a.w-b.w)
#define Vec4fMult(a,b,c)		(c.x=a.x*b.x,c.y=a.y*b.y,c.z=a.z*b.z)
#define Vec4fComb(A,a,B,b,c)	(c.x=(A)*a.x+(B)*b.x,c.y=(A)*a.y+(B)*b.y,c.z=(A)*a.z+(B)*b.z)
#define Vec4fScale(a,A,b)		(b.x=(A)*a.x,b.y=(A)*a.y,b.z=(A)*a.z)
#define Vec4fAddScaled(a,A,b,c)	(c.x=(A)*a.x+b.x,c.y=(A)*a.y+b.y,c.z=(A)*a.z+b.z)
#define Vec4fCross(a,b,c)		(c.x=a.y*b.z-a.z*b.y,c.y=a.z*b.x-a.x*b.z,c.z=a.x*b.y-a.y*b.x)
#define Vec4fClear(v)			(v.x=0.0f,v.y=0.0f,v.z=0.0f,v.w=0.0f)
//#define VecLen(a)			(sqrt(VecDot(a,a)))
#define Vec4fEqual(a,b)			((a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w)?1:0)
#define Vec4fIsZero(v)			((v.x==0.0f && v.y==0.0f && v.z==0.0f && v.w==0.0f)?1:0)
#define Vec4fMin( a,b,o)		(o.x = MIN(a.x,b.x),o.y=MIN(a.y,b.y).o.z=MIN(a.z,b.z),o.w=MIN(a.w,b.w))
#define Vec4fMax( a,b,o)		(o.x = MAX(a.x,b.x),o.y=MAX(a.y,b.y),o.z=MAX(a.z,b.z),o.w=MAX(a.w,b.w))
#define Vec4fClampAngle( v )	(ClampAngle(v.x), ClampAngle(v.y), ClampAngle(v.z))


//=====================================================
// Vec4f operators
//=====================================================
//add operator
VXMATH_INLINE Vec4f operator +(const Vec4f& v0, const Vec4f& v1) 
{
    return Vec4f(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z, v0.w+v1.w);
};
//subtract operator
VXMATH_INLINE Vec4f operator -(const Vec4f& v0, const Vec4f& v1) 
{
    return Vec4f(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z, v0.w-v1.w);
};
//multiply operator
VXMATH_INLINE Vec4f operator *(const Vec4f& v0, const float s) 
{
    return Vec4f(v0.x*s, v0.y*s, v0.z*s, v0.w*s);
};
//multiply operator
VXMATH_INLINE Vec4f operator* ( const float s, const Vec4f& v0 )
{
    return Vec4f(v0.x*s, v0.y*s, v0.z*s, v0.w*s);
}
//divide operator
VXMATH_INLINE Vec4f operator /(const Vec4f& v0, const float s) 
{
    float fOneOver = 1.0f/s;
    return Vec4f(v0.x*fOneOver, v0.y*fOneOver, v0.z*fOneOver, v0.w*fOneOver);
};
//negate operator
VXMATH_INLINE Vec4f operator-(const Vec4f& v) 
{
    return Vec4f(-v.x,-v.y,-v.z,-v.w);
};
//dot product
VXMATH_INLINE float operator %(const Vec4f& v0, const Vec4f& v1) 
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
};
//cross product
VXMATH_INLINE Vec4f operator ^(const Vec4f& v0, const Vec4f& v1) 
{
    return Vec4f(  v0.y*v1.z - v0.z*v1.y,
                   v0.z*v1.x - v0.x*v1.z,
                   v0.x*v1.y - v0.y*v1.x,
				   0.0f);
};


//=====================================================
// Vec4f methods
//=====================================================
VXMATH_INLINE bool Vec4f::IsZero( void )
{
	return((x==0.0f && y==0.0f && y==0.0f && w==0.0f )?1:0);
} // return true if x, y, and z is zero
VXMATH_INLINE bool Vec4f::IsZero( float fTol )
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
    else if ( FastFabs( w ) > fTol )
	{
		return false;
	}
    return true;
}

VXMATH_INLINE void Vec4f::Set(float _x, float _y, float _z) 
{
    x=_x; y=_y; z=_z; w=0.0f;
};
VXMATH_INLINE void Vec4f::Set(float _x, float _y, float _z, float _w) 
{
    x=_x; y=_y; z=_z; w=_w; 
};
VXMATH_INLINE void Vec4f::Set( const Vec3f& v) 
{
    x=v.x; y=v.y; z=v.z; w = 0.0f;
};
VXMATH_INLINE void Vec4f::Set( const Vec4f& v) 
{
    x=v.x; y=v.y; z=v.z; w=v.w;
};
VXMATH_INLINE float Vec4f::Len(void) 
{
    return (float) sqrt(x*x+y*y+z*z+w*w);
};
VXMATH_INLINE float Vec4f::LenSquared(void) 
{
    return x*x+y*y+z*z+w*w;
};

// which is faster ?
VXMATH_INLINE void Vec4f::Normalize(void) 
{
    float l = Len();
    if (l > TINY) 
	{
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    } 
	else 
	{
        x = 0.0f;
        y = 1.0f;
        z = 0.0f;
        w = 0.0f;
    };
};
/*
VXMATH_INLINE void Vec4f::Normalize()
{
    float n;
    n = Len();
    if( n > 0.0f ) 
	{
        n   = 1 / n;
        x = x * n;
        y = y * n;
        z = z * n;
        w = w * n;
    }
    else 
	{
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }
}*/
VXMATH_INLINE bool Vec4f::IsEqualTo(const Vec3f& v0 )
{
	return((x==v0.x && y==v0.x && z==v0.z )?1:0);
}
VXMATH_INLINE bool Vec4f::IsEqualTo(const Vec4f& v0 )
{
	return((x==v0.x && y==v0.x && z==v0.z && w == v0.w )?1:0);
}
//which is faster ?
VXMATH_INLINE bool Vec4f::IsEqualTo(const Vec3f& v0, const float& fTol)
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

VXMATH_INLINE bool Vec4f::IsEqualTo(const Vec4f& v0, const float& fTol)
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
    else if ( FastFabs(v0.w-w) > fTol )
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
VXMATH_INLINE  void Vec4f::n_vec_rot_round_vec( Vec3f *vec, float *m )
{
	Vec4f v;
	v = (*vec);

	vec->x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
	vec->y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
	vec->z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
}

VXMATH_INLINE  void Vec4f::n_vec_rot_round_vec( Vec4f *vec, float *m )
{
	Vec4f v;
	v = (*vec);

	vec->x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
	vec->y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
	vec->z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
}
VXMATH_INLINE void Vec4f::Rotate( Vec3f *axis, float angle )
{
	float rot_m[9];
	Vec4f axis2;
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
VXMATH_INLINE void Vec4f::Rotate( Vec4f *axis, float angle )
{
	float rot_m[9];
	Vec4f axis2;
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


VXMATH_INLINE void Vec4f::RotateX( float a )
{
	float rot_m[9];

	rot_m[0] = 1.0f; rot_m[1] = 0.0f;            rot_m[2] =  0.0f;
	rot_m[3] = 0.0f; rot_m[4] = (float) cos(a); rot_m[5] = -(float) sin(a);
	rot_m[6] = 0.0f; rot_m[7] = (float) sin(a); rot_m[8] =  (float) cos(a);

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec4f::RotateY( float a )
{
	float rot_m[9];

	rot_m[0] = (float) cos(a);  rot_m[1] = 0.0f; rot_m[2] = (float) sin(a);
	rot_m[3] = 0.0f;            rot_m[4] = 1.0f; rot_m[5] = 0.0f;
	rot_m[6] = -(float) sin(a); rot_m[7] = 0.0f; rot_m[8] = (float) cos(a);

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec4f::RotateZ( float a )
{
	float rot_m[9];

	rot_m[0] = (float) cos(a); rot_m[1] = -(float) sin(a); rot_m[2] = 0.0f;
	rot_m[3] = (float) sin(a); rot_m[4] =  (float) cos(a); rot_m[5] = 0.0f;
	rot_m[6] = 0.0f;           rot_m[7] = 0.0f;            rot_m[8] = 1.0f;

	n_vec_rot_round_vec( this, rot_m );
}

VXMATH_INLINE void Vec4f::Reset( void )
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}






#endif //MATHVEC4F_H

    




