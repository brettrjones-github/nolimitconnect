/*
 * Summary: internal header only used during the compilation of libxslt
 * Description: internal header only used during the compilation of libxslt
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef _LIBXSLT_CONFIG_H_
#define _LIBXSLT_CONFIG_H_

#include <NlcDependLibrariesConfig.h>


	/**
	* LIBXSLT_DOTTED_VERSION:
	*
	* the version string like "1.2.3"
	*/
#define LIBXSLT_DOTTED_VERSION "1.1.29"

	/**
	* LIBXSLT_VERSION:
	*
	* the version number: 1.2.3 value is 10203
	*/
#define LIBXSLT_VERSION 10129

	/**
	* LIBXSLT_VERSION_STRING:
	*
	* the version number string, 1.2.3 value is "10203"
	*/
#define LIBXSLT_VERSION_STRING "10129"



	/**
	* WITH_XSLT_DEBUG:
	*
	* Activate the compilation of the debug reporting. Speed penalty
	* is insignifiant and being able to run xsltpoc -v is useful. On
	* by default unless --without-debug is passed to configure
	*/



#if 1
#define WITH_XSLT_DEBUG
#endif

#if 1
	/**
	* DEBUG_MEMORY:
	*
	* should be activated only when debugging libxslt. It replaces the
	* allocator with a collect and debug shell to the libc allocator.
	* Use configure --with-mem-debug to activate it on both library
	*/
#define DEBUG_MEMORY

	/**
	* DEBUG_MEMORY_LOCATION:
	*
	* should be activated only when debugging libxslt.
	* DEBUG_MEMORY_LOCATION should be activated only when libxml has
	* been configured with --with-debug-mem too
	*/
#define DEBUG_MEMORY_LOCATION
#endif

#define LIBXSLT_STATIC 1 // Nlc uses static libs wherever possible
#if !defined LIBXSLT_PUBLIC
#if (defined (__CYGWIN__) || defined _MSC_VER) && !defined IN_LIBXSLT && !defined LIBXSLT_STATIC && !defined(LIB_STATIC)
#define LIBXSLT_PUBLIC __declspec(dllimport)
#else
#define LIBXSLT_PUBLIC
#endif
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <io.h>
#include <direct.h>
#endif

#ifdef TARGET_OS_WINDOWS
# include "libxslt_config_windows.h"
#define LIBXSLT_VERSION_EXTRA "-win32"
#elif TARGET_OS_ANDROID
# include "libxslt_config_android.h"
#define	LIBXSLT_VERSION_EXTRA "-android"
#elif TARGET_OS_LINUX
# include "libxslt_config_linux.h"
#define	LIBXSLT_VERSION_EXTRA "-linux"
#else
echo error libxslt no os defined
#endif // TARGET_OS_WINDOWS

#include <libxml2/xmlversion.h>


#endif /* ! _LIBXSLT_CONFIG_H_ */
