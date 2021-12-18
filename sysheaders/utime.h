#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <wchar.h>
# include <sys/utime.h>

#if defined(TARGET_OS_WINDOWS) && defined(_MSC_VER) && (_MSC_VER < 1900)
struct utimbuf
{
	time_t  actime;  /* access time (unused on FAT filesystems) */
	time_t  modtime; /* modification time */
};
#endif // defined(TARGET_OS_WINDOWS) && (_MSC_VER < 1900)

#else
# include_next <utime.h>
#endif // _MSC_VER

