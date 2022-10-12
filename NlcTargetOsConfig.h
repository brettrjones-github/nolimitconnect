#pragma once
//============================================================================
// Created by Brett R. Jones in 2017 and issued to public domain
//============================================================================

// determine target operating system based on compiler flags
#if( !defined(TARGET_OS_WINDOWS) && !defined(TARGET_OS_ANDROID) && !defined(TARGET_OS_LINUX)  )
// target OS not defined.. try to guess
# if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS)  || defined(_M_X64) || defined(_M_IX86) || defined(__i386__)
#  define TARGET_OS_WINDOWS
# elif defined(__ANDROID__) || defined(_M_ARM)
#  define TARGET_OS_ANDROID
# elif defined( TARGET_OS_APPLE )
echo Nlc Target OS Config error - apple not supported
# endif // defined(WIN32) || defined(_WIN32) || defined(_WINDOWS)  || defined(_M_X64) || defined(__x86_64__) || defined(_M_IX86) || defined(__i386__)
#endif // #if !define(TARGET_OS_WINDOWS) && !define(TARGET_OS_ANDROID) && !define(TARGET_OS_LINUX)

// os target specific defines for convenience
#if defined(TARGET_OS_WINDOWS)
# define MS_WINDOWS
# define TARGET_WINDOWS
# define TARGET_WINDOWS_DESKTOP // need to define this or libxbmc will fail with 'Size': is not a member of 'Windows::Foundation'
//# define TARGET_WINDOWS_STORE // seems to be windows 10 only for microsoft store.. not sure
#elif defined( TARGET_OS_APPLE)
#  define TARGET_DARWIN_OSX
#elif defined( TARGET_OS_RASPBERRY_PI)
# define TARGET_RASPBERRY_PI
# define TARGET_POSIX
#elif defined( TARGET_OS_ANDROID)
# define TARGET_ANDROID
# define TARGET_POSIX 1
#else
// default to assume linux
# define TARGET_LINUX
# ifndef TARGET_POSIX
#  define TARGET_POSIX 1
# endif // TARGET_LINUX
#endif 

#ifdef _DEBUG
# ifndef DEBUG
#  define DEBUG 1 // needs to be one for libtwolame 
# endif
#elif defined(DEBUG)
# ifndef _DEBUG
#  define _DEBUG
# endif
#else
    // not debug
# ifndef NDEBUG
#  define NDEBUG
# endif
# ifndef RELEASE
#  define RELEASE
# endif
#endif 

#include "NlcCommonConfig.h" // compile defines for compile time application switches
