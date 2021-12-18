#pragma once
#include <AppDependLibrariesConfig.h>
#if defined(TARGET_OS_WINDOWS)
# define USE_TCP_LOOPBACK 1
# define NOT_HAVE_SA_LEN 1
#endif // defined(TARGET_OS_WINDOWS)
