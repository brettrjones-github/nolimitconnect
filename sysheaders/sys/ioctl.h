#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
# include <libgnu/djgpp/ioctl_gnu.h>
#else
# include_next <sys/ioctl.h>
#endif // _MSC_VER