#pragma once
#include "NlcDependLibrariesConfig.h"
#include "kodi_src/xbmc/xbmc/config_kodi.h"
#if 0

#ifndef NPT_CONFIG_ENABLE_LOGGING
# define NPT_CONFIG_ENABLE_LOGGING
#endif // NPT_CONFIG_ENABLE_LOGGING

#ifdef __GNUC__
// under gcc, inline will only take place if optimizations are applied (-O). this will force inline even with optimizations.
# define XBMC_FORCE_INLINE __attribute__((always_inline))
#else
# define XBMC_FORCE_INLINE // donot use NLC_FORCE_INLINE.. __forceinline causes errors if not declared in correct place
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
#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_FREEBSD) && !defined(TARGET_OS_WINDOWS)
# if defined(HAVE_LIBAVAHI_COMMON) && defined(HAVE_LIBAVAHI_CLIENT)
#  define HAS_ZEROCONF
#  define HAS_AVAHI
# endif
# ifdef HAVE_DBUS
#  define HAS_DBUS
# endif
# ifndef HAVE_QT_GUI
#  define HAS_GL 1
#  ifdef HAVE_X11
#   define HAS_GLX
#   define HAS_X11_WIN_EVENTS
#  endif
# endif // HAVE_QT_GUI
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
#define HAS_ZEROCONF
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

class CFileItem;
typedef std::shared_ptr<CFileItem> CFileItemPtr;

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

//#include <vld.h> // Visual Studio memory leak detection from https://marketplace.visualstudio.com/items?itemName=ArkadyShapkin.VisualLeakDetectorforVisualC
#endif // 0
