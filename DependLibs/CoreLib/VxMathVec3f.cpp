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
#include "config_corelib.h"

#include "VxMathVec3f.h"

#include <math.h>

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// global vectors
//
Vec3f Vec3fOrigin();
Vec4f Vec4fOrigin();


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Vec3f stuff

void Vec3fCartesianToOrbital( Vec3f &vIn, Vec3f &vRetVec )
{

	//in spherical coords z = r(radius) x = theta and y = phi
	vRetVec.z = (float)Vec3fLen( vIn );//calculate radius

	if(vRetVec.z)
	{
		vRetVec.y = (float)acos(vIn.y/vRetVec.z);
	}
	else
		vRetVec.x = 0.0F;
	if(vRetVec.z)
	{
		if(vIn.z) //cant divide by 0
			vRetVec.x = (float)atan( FastFabs(vIn.x/vIn.z));
		else
			vRetVec.x = 0.0F;

//		gMsgQue.Print("Result atan = %f", vReturnVec.y);
		if((vIn.x > 0.0F) && (vIn.z < 0.0F) ) // case quadrant2
			vRetVec.x += VX_PI_DIV2_F; //add pi/2
		else if((vIn.x <= 0.0F) && (vIn.z < 0.0F)) //case quadrant3
			vRetVec.x += VX_PI_F; //add pi
		else if((vIn.y < 0.0F) && (vIn.x >= 0.0F)) //case quadrant4
			vRetVec.x += VX_PI_DIV2_X3_F;//add 3 * pi/2
//		gMsgQue.Print("Real angle = %f", vReturnVec.y);
	}
	else
	{
		vRetVec.x = 0.0f;
		vRetVec.y = 0.0f;
	}

}
//void Vec3fDirectorToCartesian( Vec3f &vIn, Vec3f &vRetVec )
//{
/*
//	float r = vIn.fRadius * (float) cos( vIn.fAz );
//	vRetVec.y = r * (float)sin( vIn.fEl );
//	vRetVec.x = r * (float)cos( vIn.fEl );
//	vRetVec.z = vIn.fRadius * (float)sin( vIn.fAz );
	//in spherical coords z = r(radius) x = theta and y = phi
	double dSinZ;
	if(vIn.y)
		vRetVec.y = vIn.z*(float)cos(vIn.y);
	else
		vRetVec.y = 0.0F;
	dSinZ = sin(vIn.y);
	if(vIn.x < PI_DIV2 ) //if in quadrants 1
	{
		if(vIn.y)
		{
			vRetVec.z = (float)(vIn.z*dSinZ*cos(vIn.x));
			vRetVec.x = (float)(vIn.z*dSinZ*sin(vIn.x));
		}
		else
		{  // if no z then use polar equation
			vRetVec.z = (float)(vIn.z*cos(vIn.x));
			vRetVec.x = (float)(vIn.z*sin(vIn.x));
		}
	}
	else if(vIn.x < PI) //quadrant2
	{
		if(vIn.y)
		{
			vRetVec.z =  - (float)(vIn.z*dSinZ*sin(vIn.x));
			vRetVec.x =  - (float)(vIn.z*dSinZ*cos(vIn.x));
		}
		else //if no z use polar equation
		{
			vRetVec.z =   (float)(vIn.z*cos(vIn.x));
			vRetVec.x =   (float)(vIn.z*sin(vIn.x));
		}
	}
	else if( vIn.x < PI_DIV2_X3) //quadrant3
	{
		if(vIn.x)
		{
			vRetVec.z = (float)(vIn.z*dSinZ*cos(vIn.x));
			vRetVec.x = (float)(vIn.z*dSinZ*sin(vIn.x));
		}
		else
		{  // if no z then use polar equation
			vRetVec.z = (float)(vIn.z*cos(vIn.x));
			vRetVec.x = (float)(vIn.z*sin(vIn.x));
		}
	}
	else
	{
		if(vIn.x)
		{
			vRetVec.z =  - (float)(vIn.z*dSinZ*sin(vIn.x));
			vRetVec.x =  - (float)(vIn.z*dSinZ*cos(vIn.x));
		}
		else //if no z use polar equation
		{
			vRetVec.z =  (float)(vIn.z*cos(vIn.x));
			vRetVec.x =  (float)(vIn.z*sin(vIn.x));
		}

	}
	*/
//}

void Vec3fSetOrbitPosRad( float fAzRad, float fElRad, float fDist, Vec3f &vRet )
{
	//compute cartesian position given the spherical parameters
	vRet.z = fDist;

	//rotate point( x = 0, y = 0, z = fDist ) to the given elev and az
	float fSinPitch = (float) sin( fAzRad );
	float fSinRoll = (float) sin( fElRad );
	float fSinYaw = 0;
	float fCosPitch = (float) cos( fAzRad );
	float fCosRoll = (float) cos( fElRad );
	float fCosYaw = 1;

/*
	//general form of rotate
	x = (-1)*x;
	float xa = fCosYaw * x - fSinYaw * z;
	float za = fSinYaw * x + fCosYaw * z;
	x = fCosRoll * xa + fSinRoll * y;
	ya = fCosRoll * y - fSinRoll * xa;
	z = fCosPitch * za - fSinPitch * ya;
	y = fSinPitch * za + fCosPitch * ya;
*/
	//slimed down version cause x and y are 0
	float xa = - fSinYaw * vRet.z;
	float za = fCosYaw * vRet.z;
	vRet.x = fCosRoll * xa;
	float ya = - fSinRoll * xa;
	vRet.z = fCosPitch * za - fSinPitch * ya;
	vRet.y = fSinPitch * za + fCosPitch * ya;

}


//======================================================================
// Rotate a vector
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
// Output:
//----------------------------------------------------------------------

Vec3f Vec3f::Rotate(float cos1, float sin1, float cos2, float sin2)
{
	Vec3f vTemp, vResult;

	vResult.x = x * cos1 + z * -sin1;
	vTemp.y = y;
	vTemp.z = x * sin1 + z * cos1;
	vResult.y = vTemp.y * -cos2 + vTemp.z * sin2;
	vResult.z = vTemp.y * -sin2 + vTemp.z * -cos2;
	return(vResult);
}

//======================================================================
// Reverse rotate  a vector
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
// Output:
//----------------------------------------------------------------------
Vec3f Vec3f::RevRotate(float cos1, float sin1, float cos2, float sin2)
{
	Vec3f vTemp, vResult;

	vTemp.x = x;
	vResult.y = y * cos2 + z * -sin2;
	vTemp.z = y * sin2 + z * cos2;
	vResult.x = vTemp.x * -cos1 + vTemp.z * sin1;
	vResult.z = vTemp.x * -sin1 + vTemp.z * -cos1;
	return(vResult);
}


//======================================================================
// Change Length of a rectangular vector
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
// Output:
//----------------------------------------------------------------------
Vec3f Vec3f::ChangeLengthRectVect(float dLength)
{
	Vec3f vTemp;
	Vec3fCartesianToOrbital( *this, vTemp ); //convert to sherical cords
	vTemp.z = dLength; // change length
	Vec3f vResult;
	Vec3fOrbitalToCartesianRad( vTemp, vResult); //make rectangular again
	return(vResult);
}
//======================================================================
// Round x y and z down to specified resolution
// EXAMPLE: if x = 14.0378 and resolution = 0.02 then result = 14.02
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
// Output:
//----------------------------------------------------------------------
Vec3f Vec3f::RoundToResolution(float dResolution)
{
	double dTemp;
	Vec3f vResult;
	modf(x/dResolution, &dTemp);
	vResult.x = (float)(dTemp * dResolution);
	modf(y/dResolution, &dTemp);
	vResult.y = (float)(dTemp * dResolution);
	modf(z/dResolution, &dTemp);
	vResult.z = (float)(dTemp * dResolution);
	return(vResult);
}
