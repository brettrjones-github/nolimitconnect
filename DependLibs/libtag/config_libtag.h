#include <AppDependLibrariesConfig.h>

/* Defined if your compiler supports some byte swap functions */
/* #undef HAVE_BOOST_BYTESWAP */
#ifndef _MSC_VER
# define   HAVE_GCC_BYTESWAP 1
#endif // _MSC_VER
/* #undef HAVE_GLIBC_BYTESWAP */
/* #undef HAVE_MSC_BYTESWAP */
/* #undef HAVE_MAC_BYTESWAP */
/* #undef HAVE_OPENBSD_BYTESWAP */

/* Defined if your compiler supports some atomic operations */
#define   HAVE_STD_ATOMIC 1
/* #undef HAVE_BOOST_ATOMIC */
/* #undef HAVE_GCC_ATOMIC */
/* #undef HAVE_MAC_ATOMIC */
/* #undef HAVE_WIN_ATOMIC */
/* #undef HAVE_IA64_ATOMIC */

/* Defined if your compiler supports some safer version of vsprintf */
#define   HAVE_VSNPRINTF 1
/* #undef HAVE_VSPRINTF_S */

/* Defined if your compiler supports ISO _strdup */
/* #undef HAVE_ISO_STRDUP */

/* Defined if zlib is installed */
#define   HAVE_ZLIB 1
/* #undef HAVE_BOOST_ZLIB */

/* Indicates whether debug messages are shown even in release mode */
/* #undef TRACE_IN_RELEASE */

#define TESTS_DIR "/home/jack/kodi-krypton/tools/depends/target/libtag/arm-linux-androideabi-android-21/tests/"


#define   TAGLIB_WITH_ASF 1
#define   TAGLIB_WITH_MP4 1
