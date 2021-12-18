/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <AppDependLibrariesConfig.h>

#ifndef NPT_CONFIG_ENABLE_LOGGING
# define NPT_CONFIG_ENABLE_LOGGING
#endif // NPT_CONFIG_ENABLE_LOGGING

#ifdef __GNUC__
// under gcc, inline will only take place if optimizations are applied (-O). this will force inline even with optimizations.
# define XBMC_FORCE_INLINE __attribute__((always_inline))
#else
# define XBMC_FORCE_INLINE // donot use GOTV_FORCE_INLINE.. __forceinline causes errors if not declared in correct place
#endif


//============================================================================
//=== Apple Specific ===//
//============================================================================
#if defined(TARGET_OS_APPLE)
# if defined(TARGET_DARWIN)
#  define HAS_GL 1
#  define HAS_SDL 1
#  define HAS_SDL_WIN_EVENTS 1
# endif
# define HAS_ZEROCONF
# define HAS_LINUX_NETWORK
#endif

//============================================================================
//=== Linux Specific ===//
//============================================================================
#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_FREEBSD)
# if defined(HAVE_LIBAVAHI_COMMON) && defined(HAVE_LIBAVAHI_CLIENT)
#  define HAS_ZEROCONF
#  define HAS_AVAHI
# endif
# ifdef HAVE_DBUS
#  define HAS_DBUS
# endif
# ifndef HAVE_QT_GUI
#  define HAS_GL
# endif // HAVE_QT_GUI
# ifdef HAVE_X11
#  define HAS_GLX
#  define HAS_X11_WIN_EVENTS
# endif
# ifdef HAVE_SDL
# define HAS_SDL
#  ifndef HAVE_X11
#   define HAS_SDL_WIN_EVENTS
#  endif
# else
#  ifndef HAVE_X11
#   define HAS_LINUX_EVENTS
#  endif
# endif
# define HAS_LINUX_NETWORK
# ifdef HAVE_LIRC
#  define HAS_LIRC
# endif
# ifdef HAVE_ALSA
#  define HAS_ALSA
# endif
#endif

//============================================================================
//=== Android specific ===//
//============================================================================
#if defined(TARGET_OS_ANDROID)
#undef HAS_LINUX_EVENTS
#undef HAS_LIRC
//#define HAS_ZEROCONF // Defined in AppCompilerConfig.h
//# if defined(HAVE_QT_GUI)
//#  define HAS_AVAHI
//# endif // defined(HAVE_QT_GUI)
#endif

/****************
* default skin
****************/
#if defined(HAS_TOUCH_SKIN) && defined(TARGET_OS_APPLE)
# define DEFAULT_SKIN          "skin.re-touched"
#else
# define DEFAULT_SKIN          "skin.confluence"
#endif // defined(HAS_TOUCH_SKIN) && defined(TARGET_OS_APPLE)

#include <memory> // for std::shared_ptr

#if defined __cplusplus
class CFileItem;
typedef std::shared_ptr<CFileItem> CFileItemPtr;
#endif // defined __cplusplus

#define HAVE_DLL_LIB_CURL			0
#define HAVE_DLL_LIB_ASS			0
#define HAVE_DLL_LIB_IMAGE			0
#define HAVE_DLL_LIB_SHAIRPLAY		0
#define HAVE_DLL_LIB_NFS			0
#define HAVE_DLL_LIB_PLIST			0
#define HAVE_DLL_LIB_CEC			0
#define HAVE_DLL_LIB_DVDNAV			0
#define HAVE_DLL_LIB_EXIF			0
#define HAVE_DLL_LIB_CPLUFF			0

#if defined(TARGET_OS_WINDOWS)
# define HAS_MDNS 1 // ZeroConfig for microsoft
#endif // defined(TARGET_OS_WINDOWS)

//#if defined(TARGET_OS_LINUX)
//# define HAS_AVAHI 1 // ZeroConfig for linux
//#endif // defined(TARGET_OS_WINDOWS)

//#include <vld.h> // Visual Studio memory leak detection from https://marketplace.visualstudio.com/items?itemName=ArkadyShapkin.VisualLeakDetectorforVisualC

#if defined(TARGET_POSIX)
# include <time.h>
# include <sys/time.h>
# include <sys/types.h>
# include <errno.h>
#endif

//#if defined(TARGET_OS_ANDROID)
//# undef HAS_LINUX_EVENTS
//# undef HAS_LIRC
//#endif



#ifdef HAS_DVD_DRIVE
# define HAS_CDDA_RIPPER
#endif


// Useful pixel colour manipulation macros
#define GET_A(color)            ((color >> 24) & 0xFF)
#define GET_R(color)            ((color >> 16) & 0xFF)
#define GET_G(color)            ((color >>  8) & 0xFF)
#define GET_B(color)            ((color >>  0) & 0xFF)


#ifdef TARGET_OS_WINDOWS
//# include <platform/win32/PlatformDefs.h> already included 
#elif defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
//# include <platform/overrides/android/PlatformAndroid.h>
#elif defined(TARGET_OS_LINUX) || (defined(TARGET_OS_ANDROID) && defined(HAVE_QT_GUI))
# include "platform/linux/PlatformDefs.h"
#elif defined(TARGET_OS_APPLE)
echo error APPLE os is not supported
#endif
