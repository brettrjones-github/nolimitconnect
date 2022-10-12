
#pragma once
#include <NlcDependLibrariesConfig.h>

#define HAVE_ZLIB_H 1
#define USE_OPENSSL 1
#define CURL_STATICLIB 1

#if defined(TARGET_OS_WINDOWS)
# include <libcurl/lib/config-win32.h>
#elif defined(TARGET_OS_ANDROID)
# include <libcurl/lib/config-android.h>
#else
# include <libcurl/lib/config-posix.h>
#endif // TARGET_OS_WINDOWS
