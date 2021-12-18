#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <ws2tcpip.h>
# include <io.h>
typedef unsigned int		sa_family_t;

#else
# include_next <sys/socket.h>
#endif // _MSC_VER