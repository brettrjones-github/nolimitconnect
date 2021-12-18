#pragma once
#include "AppDependLibrariesConfig.h"
/* The earliest ABI compatible version or undefined */
#define CP_ABI_COMPATIBILITY "0.1"

#ifdef TARGET_OS_WINDOWS
/* File name separator character */
# define CP_FNAMESEP_CHAR '\\'

/* File name separator string */
# define CP_FNAMESEP_STR "\\"
#else
/* File name separator character */
# define CP_FNAMESEP_CHAR '/'

/* File name separator string */
# define CP_FNAMESEP_STR "/"
#endif // TARGET_OS_WINDOWS

#define CP_DATADIR GOTV_DEFAULT_DATA_DIR

/* Define to use GNU Libtool libltdl */
/* #undef DLOPEN_LIBTOOL */

/* Define to use Posix dlopen */
#define DLOPEN_POSIX /**/

/* Define to 1 if translation of program messages to the user's native
language is requested. */
/* #undef ENABLE_NLS */

/* Define to 1 if you have the MacOS X function CFLocaleCopyCurrent in the
CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the MacOS X function CFPreferencesCopyAppValue in
the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Define if the GNU dcgettext() function is already present or preinstalled.
*/
/* #undef HAVE_DCGETTEXT */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if the GNU gettext() function is already present or preinstalled. */
/* #undef HAVE_GETTEXT */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "cpluff"

#define CP_EXPORT	GOTV_EXPORT
#define CP_IMPORT	GOTV_IMPORT
#define CP_HIDDEN	GOTV_HIDDEN

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
#ifdef TARGET_OS_WINDOWS
# include "cpluffdef_win32.h"
#else
# include  "cpluffdef_linux.h"
#endif // TARGET_OS_WINDOWS
