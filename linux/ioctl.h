#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
# include <sys/ioctl.h>
#else
# include_next <sys/ioctl.h>
#endif // _MSC_VER

