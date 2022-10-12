#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
//  strncasecmp etc simulated in CoreLib
# include <string.h>
#else
# include <strings.h>
#endif // _MSC_VER
