#pragma once
#include <GoTvDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
#else
# include_next <sys/ipc.h>
#endif // _MSC_VER