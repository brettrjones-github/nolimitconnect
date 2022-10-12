#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
#include <libgnu/unistd_gnu.h>
#else
# include_next <pwd.h>
#endif // _MSC_VER

