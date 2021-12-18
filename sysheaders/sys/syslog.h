#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
#else
# include_next <sys/syslog.h>
#endif // _MSC_VER