#pragma once

#ifdef _MSC_VER
# include <AppDependLibrariesConfig.h>
#else
# include_next <netinet/tcp.h>
#endif // _MSC_VER
