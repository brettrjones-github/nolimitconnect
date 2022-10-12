#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef TARGET_OS_WINDOWS
#else
# include_next <arpa/inet.h>
#endif // _MSC_VER
