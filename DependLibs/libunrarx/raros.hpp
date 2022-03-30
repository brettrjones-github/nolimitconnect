#ifndef _RAR_RAROS_
#define _RAR_RAROS_

#include <AppDependLibrariesConfig.h>

#if defined(_XBMC)
#ifndef TARGET_OS_WINDOWS
  #define TARGET_OS_WINDOWS
#endif
  #define SILENT
#endif

#ifdef __EMX__
  #define _EMX
#endif

#ifdef __DJGPP__
  #define _DJGPP
  #define _EMX
#endif

#if (defined(__WIN32__) || defined(_WIN32)) && !defined(TARGET_OS_WINDOWS)
  #define TARGET_OS_WINDOWS
#endif

#ifdef _WIN32_WCE
  #define TARGET_OS_WINDOWS
  #define _WIN_CE
  #ifdef WM_FILECHANGEINFO
    #define PC2002
  #else
    #undef PC2002
  #endif
#endif

#ifdef __BEOS__
  #define _UNIX
  #define _BEOS
#endif

#ifdef __APPLE__
  #define _UNIX
  #define _APPLE
#endif

#if !defined(_EMX) && !defined(TARGET_OS_WINDOWS) && !defined(_BEOS) && !defined(_APPLE)
  #define _UNIX
#endif

#endif
