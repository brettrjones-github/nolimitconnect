#pragma once
//============================================================================
// Created by Brett R. Jones in 2018 and issued to public domain
//============================================================================

#ifdef DEBUG
# define CMAKE_INTDIR            "Debug"
#else
# define CMAKE_INTDIR            "Release"
#endif

// define one of the following for which type of application
//#define BUILD_KODIP2P_APP 1
//#define BUILD_KODIQT_APP 1
#define BUILD_GOTV_APP 1

//#define ENABLE_ASSERTIONS	0		/* Assertions */

#define PACKAGE_VERSION			"1.0"
#define BUILD_REVISION			"1.0"


#if defined( BUILD_GOTV_APP )

# define HAVE_QT_GUI            1 // define if have qt application interface and rendering

# define PROGRAM_NAME			"NoLimitConnect"
# define PACKAGE_STRING			"NoLimitConnect 1.0"	
# define PACKAGE_URL		    "http://www.nolimitconnect.com"
# define BUILD_KODI_MAIN        1

#elif defined( BUILD_KODIQT_APP )

# define HAVE_QT_GUI            1 // define if have qt application interface and rendering

# define PROGRAM_NAME			"KodiQt"
# define PACKAGE_STRING			"KodiQt 1.0"	
# define PACKAGE_URL		    "http://kodi.tv"
# define BUILD_KODI_MAIN        1
#elif defined( BUILD_KODIP2P_APP )

# define PROGRAM_NAME			"KodiP2P"
# define PACKAGE_STRING			"KodiP2P 1.0"	
# define PACKAGE_URL		    "http://kodi.tv"
# define BUILD_KODI_MAIN        1

#else
echo you must define your application information
#endif // BUILD_GOTV_APP


/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT		PACKAGE_URL
#define PACKAGE_NAME			PROGRAM_NAME
#define PACKAGE_TARNAME			PACKAGE_NAME


/* The time this package was configured for a build */
#define BUILD_TIMESTAMP			"2017-06-11T20:45+0000"
/* Version number: (major<<16) + (minor<<8) + subminor */
#define NO_LIMIT_VERSION_BINARY		0x010002


#ifdef TARGET_POSIX
# ifndef INSTALL_PATH
#  define INSTALL_PATH    "/usr/share/xbmc"
# endif

# ifndef BIN_INSTALL_PATH
#  define BIN_INSTALL_PATH "/usr/lib/xbmc"
# endif
#else
# define BIN_INSTALL_PATH        "$(ProjectDir)../../../installed/kodi/lib/kodi"; 
# define INSTALL_PATH             "C:/Program Files (x86)/kodi/share/kodi"; 
#endif // TARGET_POSIX

#define NPT_CONFIG_ENABLE_LOGGING
#define PLT_HTTP_DEFAULT_SERVER "UPnP/1.0 DLNADOC/1.50 Kodi"
#define PLT_HTTP_DEFAULT_USER_AGENT "UPnP/1.0 DLNADOC/1.50 Kodi" 

// define USE_STATIC_LIBS if want to create/link static libraries where possible
#define USE_STATIC_LIBS 1

#ifdef USE_STATIC_LIBS
# define TAGLIB_STATIC
# define FREETYPE2_STATIC           1
#endif // TARGET_OS_WINDOWS

// uncomment to enable ffmpeg log
//#define ENABLE_FFMPEG_LOG

// define if linking in CoreLib/VxFunctionsMissingInWindows.cpp so standard functions like strptime are not double linked
#define HAVE_WINDOWS_MISSING_FUNCTIONS 1

