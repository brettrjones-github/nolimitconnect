#pragma once
//============================================================================
// Created by Brett R. Jones in 2017 and issued to public domain
//============================================================================
#include "AppCompilerConfig.h"

//============================================================================
//=== Windows Specific ===//
//============================================================================
#if defined(TARGET_OS_WINDOWS)

//============================================================================
//=== Android Specific ===//
//============================================================================
#elif defined(TARGET_OS_ANDROID)
# define HAVE_LIBGLESV2     1
//# define HAS_MDNS_EMBEDDED  1 // not windows and not compiled with mingw

//============================================================================
//=== Linux Specific ===//
//============================================================================
#elif defined(TARGET_OS_LINUX)

//============================================================================
//=== Apple Specific ===//
//============================================================================
#elif defined(TARGET_OS_APPLE)
echo GoTv Compiler Config error apple not supported
#else 
echo GoTv Compiler Config error no os defined
#endif

//============================================================================
//=== All ===//
//============================================================================

// #define HAVE_LIBAOM      1
// #define HAVE_LIBAVAHI_COMMON
// #define HAVE_LIBBLURAY       // uncomment if have libblueray and want to support DVD drives.. has dependency on java and others
// #define HAVE_LIBBLURAY_BDJ   // uncomment if have libblueray and want to support DVD drives
// #define HAVE_LIBCEC          // uncomment to enable usb-hdmi remote control .. libcec has c# dependencies etc
// #define HAVE_LIBEGL
#define HAVE_LIBGEN_H       1 
// #define HAVE_LIBGIF
// #define HAVE_LIBGLES
#define HAVE_LIBICONV       1
#define HAVE_LIBINTL_H      1 
#define HAVE_LIBMP3LAME     1
#define HAVE_LIBMICROHTTPD  1
#if defined(HAVE_X86_ASM)
# define HAVE_LIBNETTLE      1 /* nettle is enabled */
#endif // defined(HAVE_X86_ASM)

#define HAVE_LIBNFS 
// #define HAVE_LIBNSL
#define HAVE_LIBPLIST       1
#define HAVE_LIBPTHREAD		1	// simulated in vs with libpthread
// #define HAVE_LIBPULSE
// #define HAVE_LIBRTMP
// #define HAVE_LIBSOCKET
// #define HAVE_LIBSHAIRPLAY    // cannot get to link without official apple crap ( sdp_get_connection(sdp_t *sdp) etc )
// #define HAVE_LIBSMBCLIENT
#define HAVE_LIBSSL         1
// #define HAVE_LIBUDEV
#define HAVE_LIBVORBISENC   1
#define HAVE_LIBXSLT        1
#define HAVE_LIBZ           1
#define HAVE_OPENSSL        1    

#define HAVE_LCMS2          1 

// #define HAS_LIB_OPENMAX 1
// #define HAS_LIBSTAGEFRIGHT 1     // android only
// #define HAS_MMAL 1               // hardware

#ifdef HAVE_LIBMICROHTTPD
# define HAS_WEB_SERVER     1
# define HAS_WEB_INTERFACE  1
#endif // HAVE_LIBMICROHTTPD

#if defined(HAVE_LIBMDNSEMBEDDED)
# define HAS_ZEROCONF
# define HAS_MDNS
# define HAS_MDNS_EMBEDDED
#endif // defined(HAVE_LIBMDNSEMBEDDED)

#if defined(HAVE_LIBGIF)
# define HAS_GIFLIB
#endif // defined(HAVE_LIBGIF)

#ifdef HAVE_LIBRTMP
# define HAS_LIBRTMP 1
#endif // HAVE_LIBRTMP

#ifdef HAVE_LIBPULSE
# define HAS_PULSEAUDIO
#endif // HAVE_LIBPULSE

// EGL detected. Dont use GLX!
#ifdef HAVE_LIBEGL
# undef HAS_GLX
# define HAS_EGL
#endif // HAVE_LIBEGL

// GLES2.0 detected. Dont use GL!
#ifdef HAVE_LIBGLESV2
# undef HAS_GL
# define HAS_GLES 2
#endif // HAVE_LIBGLESV2

// GLES1.0 detected. Dont use GL!
#ifdef HAVE_LIBGLES
# undef HAS_GL
# define HAS_GLES 1
#endif // HAVE_LIBGLES

#ifdef HAVE_LIBNFS
# define HAS_FILESYSTEM_NFS
#endif // HAVE_LIBNFS

#if defined(HAS_MDNS_EMBEDDED) && defined( TARGET_OS_WINDOWS)
# define HAS_MDNS           1 
#endif // HAS_MDNS_EMBEDDED

//#define HAS_MYSQL 1 // uses sqlite3 instead

#define HAS_UPNP            1 
#if !defined(TARGET_OS_ANDROID) && !defined(TARGET_OS_LINUX) // BRJ FIXME .. implement existing zero config for android
# define HAS_ZEROCONF        1
#endif // TARGET_OS_ANDROID

//#define HAS_JSONRPC

//#define HAS_FILESYSTEM    1
//#define HAS_FILESYSTEM_CDDA   1
//#define HAS_FILESYSTEM_SAP
//#define HAS_FILESYSTEM_SFTP 1 

#ifdef HAVE_LIBSMBCLIENT
# define HAS_FILESYSTEM_SMB
#endif // HAVE_LIBSMBCLIENT

#ifdef HAVE_MYSQL
# define HAS_MYSQL // we use sqlite3 instead for database
#endif // HAVE_MYSQL


/*! @note Define "USE_DEMUX" at compile time if demuxing in the PVR add-on is used.
 *        Also XBMC's "DVDDemuxPacket.h" file must be in the include path of the add-on,
 *        and the add-on should set bHandlesDemuxing to true.
 */
//#define USE_DEMUX 1
#define HAS_DVDPLAYER 0         // define to 1 if have dvd player support
// #define HAS_DVD_DRIVE        // uncomment to support dvd/bluray drives
// #define HAS_CDDA_RIPPER      // uncomment to play cd disks

#define HAS_ADSPADDONS          1
#define HAS_DVD_SWSCALE         1
#define HAS_EVENT_SERVER        1
#define HAS_PYTHON              1
#define HAS_PVRCLIENTS          1
#define HAS_SCREENSAVER         1
#define HAS_VIDEOPLAYER         1
#define HAS_VIDEO_PLAYBACK      1
#define HAS_VISUALISATION       1


// #define HAS_AIRPLAY          // uncomment for apple airplay support.. has dependencies on proprietary apple crap
// #define HAVE_LIBSHAIRPLAY    // cannot get to link without official apple crap ( sdp_get_connection(sdp_t *sdp) etc )
// #if defined(HAVE_LIBSHAIRPLAY)
// # define HAS_AIRTUNES        // air tunes requires libshareplay
// #endif

//============================================================================
//=== Non-free Components ===//
//============================================================================
#if defined(TARGET_OS_WINDOWS)
# define HAS_FILESYSTEM_RAR
#else
# if defined(HAVE_XBMC_NONFREE)
#  define HAS_FILESYSTEM_RAR
# endif // defined(HAVE_XBMC_NONFREE)
#endif

//#define HAS_WIN32_NETWORK     // defined in AppCompilerConfig.h
//#define HAS_AUDIO 1           // defined in AppCompilerConfig.h
//#define HAS_FILESYSTEM_SMB 1  // defined in AppCompilerConfig.h

//#define HAS_SDL_JOYSTICK  // define for joystick control

//============================================================================
//=== More General defines ===//
//============================================================================
#if defined(HAVE_X11)
# define HAS_EGL
# if !defined(HAVE_LIBGLESV2)
#  define HAS_GLX
# endif // HAVE_LIBGLESV2
#endif // HAVE_X11

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    extern int libintl_version;

    // these are implemented in CoreLib/VxTime.cpp but did not want to include the header everywhere
    int64_t	GetTimeStampMs( void );			      // milli seconds since January 1, 1970 GMT time
    inline int64_t	GetTimeStampSec( void ) { return GetTimeStampMs() / 1000; }

#ifdef __cplusplus
}
#endif // __cplusplus

#define BUILDSUF ""

#ifdef TARGET_OS_WINDOWS
# define SLIBSUF				".dll"
#else
# define SLIBSUF				".so"
#endif // TARGET_OS_WINDOWS

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR				".libs/"
#define LIBDIR					".libs/"
#define LOCALEDIR				".libs/"
#define BISON_LOCALEDIR			".bison/"
#define GOTV_DEFAULT_DATA_DIR   ".ptopdata/"

/* Use C99 variable - size arrays */
// to many issues with the define.. instead define USE_VAR_ARRAYS then let each library define VAR_ARRAYS based on USE_VAR_ARRAYS
//#define  VAR_ARRAYS				1 // some libs use if instead of #if 
#define  USE_VAR_ARRAYS				0 // some libs use if instead of #if 

//============================================================================
//=== Windows Specific ===//
//============================================================================
#if defined(TARGET_OS_WINDOWS)
//#define HAS_WIN32_NETWORK     // defined in AppCompilerConfig.h
//#define HAS_AUDIO 1           // defined in AppCompilerConfig.h
//#define HAS_FILESYSTEM_SMB 1  // defined in AppCompilerConfig.h

//#define HAS_SDL_JOYSTICK  // define for joystick control
# ifndef HAVE_QT_GUI
#  ifndef HAS_DX
#   define HAS_DX 1
#  endif // HAS_DX
# endif // HAVE_QT_GUI

//============================================================================
//=== Android Specific ===//
//============================================================================
#elif defined(TARGET_OS_ANDROID)

# ifndef HAS_GLES
#  define HAS_GLES 1
//#  define HAVE_LIBGLESV2 1 // defined to 1 in first Android Specific secion
# endif // HAS_GLES

//============================================================================
//=== Linux Specific ===//
//============================================================================
#elif defined(TARGET_OS_LINUX)

# if !defined(HAS_GL) && !defined(HAVE_QT_GUI)
#  define HAS_GL 1
#  define HAVE_LIBGL 1
# endif // HAS_GL

# if defined(HAVE_LIBAVAHI_COMMON) && defined(HAVE_LIBAVAHI_CLIENT)
#  define HAS_ZEROCONF
#  define HAS_AVAHI
# endif // defined(HAVE_LIBAVAHI_COMMON) && defined(HAVE_LIBAVAHI_CLIENT)

# ifdef HAVE_DBUS
#  define HAS_DBUS
# endif // HAVE_DBUS

# ifdef HAVE_X11
#  define HAS_X11_WIN_EVENTS
# endif // HAVE_X11

# ifdef HAVE_SDL
#  define HAS_SDL
#  ifndef HAVE_X11
#   define HAS_SDL_WIN_EVENTS
#  endif // HAVE_X11
# else
#  ifndef HAVE_X11
#    define HAS_LINUX_EVENTS
#  endif // HAVE_X11
# endif // HAVE_SDL

# define HAS_LINUX_NETWORK
# ifdef HAVE_LIRC
#  define HAS_LIRC
# endif // HAVE_LIRC

# ifdef HAVE_LIBPULSE
#  define HAS_PULSEAUDIO
# endif // HAVE_LIBPULSE

# ifdef HAVE_ALSA
#  define HAS_ALSA
# endif // HAVE_ALSA

//============================================================================
//=== Apple Specific ===//
//============================================================================
#elif defined(TARGET_OS_APPLE)
# ifndef HAS_GLES
#  define HAS_GLES 1
#  define HAVE_LIBGLESV2 1 // apple phones support gles.. not sure if this is right define though
# endif // HAS_GLES

echo GoTv Compiler Config error apple not supported
#else 
echo GoTv Compiler Config error no os defined
#endif // TARGET_OS_WINDOWS
