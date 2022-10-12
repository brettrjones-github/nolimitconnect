#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <ws2tcpip.h>
#else
# include_next <netdb.h>
#endif // _MSC_VER
