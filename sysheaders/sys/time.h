#pragma once

#ifdef _MSC_VER
# include <AppDependLibrariesConfig.h>
# include <io.h>
#else
# include_next <sys/time.h>
#endif // _MSC_VER