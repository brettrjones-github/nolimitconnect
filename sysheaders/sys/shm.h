#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
#else
# include_next <sys/shm.h>
#endif // _MSC_VER