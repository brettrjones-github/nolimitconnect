
#include "config_corelib.h"


#include "VxAppInfo.h"
#include "VxGlobals.h"
#include <cstddef>

//=============================================================================
const char* VxAppInfo::getAppName( void )
{
	return VxGetApplicationTitle();
}

//=============================================================================
const char* VxAppInfo::getAppNameNoSpaces( void )
{
	return VxGetApplicationNameNoSpaces();
}

//=============================================================================
const char* VxAppInfo::getAppNameNoSpacesLowerCase( void )
{
    return VxGetApplicationNameNoSpacesLowerCase();
}

//=============================================================================
const char* VxAppInfo::getCompanyWebsite( void )
{
    return VxGetCompanyWebsite();
}

//=============================================================================
const char* VxAppInfo::getCompanyDomain( void )
{
    return VxGetCompanyDomain();
}

//=============================================================================
int VxAppInfo::getVersionMajor( void )
{
	uint16_t	appVer = VxGetAppVersion();
	return ( ( appVer >> 8 ) & 0xff );
}

//=============================================================================
int VxAppInfo::getVersionMinor( void )
{
	uint16_t	appVer = VxGetAppVersion();
	return ( appVer & 0xff );
}

//=============================================================================
const char* VxAppInfo::getVersionSuffix( void )
{
	return "RC1";
}

//=============================================================================
std::string VxAppInfo::getVersionString( void )
{
    char versionBuf[512];
    sprintf( versionBuf, "%d.%d %s", getVersionMajor(), getVersionMinor(), getVersionSuffix() );
    return versionBuf;
}

//=============================================================================
const char* VxAppInfo::getSCMID( void )
{
	return "@APP_SCMID@";
}
