#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <ws2tcpip.h>
# include <io.h>
# include "socket.h"
struct sockaddr_un 
{
	sa_family_t sun_family;               /* AF_UNIX */
	char        sun_path[ 108 ];            /* pathname */
};

#else
# include_next <sys/un.h>
#endif // _MSC_VER