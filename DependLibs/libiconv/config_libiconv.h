
#pragma once
#include <NlcDependLibrariesConfig.h>

/* Define to 1 to enable a few rarely used encodings. */
// I don't think we need these ( like AIX conversion )
//#define ENABLE_EXTRA 1

/* Define to 1 if the package shall run at any location in the filesystem. */
/* #undef ENABLE_RELOCATABLE */

/* Define to a type if <wchar.h> does not define. */
/* #undef mbstate_t */

/* Define if you have <iconv.h>, the iconv_t type, and the
iconv_open, iconv, iconv_close functions. */
/* #undef HAVE_ICONV */
/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST /* empty by default */


/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
/* #undef HAVE_LANGINFO_CODESET */

/* Define if you have the mbrtowc() function. */
//#define HAVE_MBRTOWC 1
// even if we have mb to wide conversion say no so consistently use iconv for all conversion
#undef HAVE_MBRTOWC

/* Define if you have the wcrtomb() function. */
//#define HAVE_WCRTOMB 1
// even if we have wide to multi-byte conversion say no so consistently use iconv for all conversion
#undef HAVE_WCRTOMB

/* Define to 1 if O_NOFOLLOW works. */
#define HAVE_WORKING_O_NOFOLLOW 0

/* Define if the machine's byte ordering is little endian. */
#if NLC_ARCH_LITTLE_ENDIAN
# define WORDS_LITTLEENDIAN  1
#else
# define WORDS_LITTLEENDIAN  0
#endif // NLC_ARCH_LITTLE_ENDIAN

