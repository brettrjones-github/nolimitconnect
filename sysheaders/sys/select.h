#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
#else
# include_next <sys/select.h>
#endif // _MSC_VER