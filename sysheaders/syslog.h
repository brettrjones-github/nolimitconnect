#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
#include <CoreLib/VxDebug.h>

#else
# ifdef TARGET_OS_LINUX
#  include <stdio.h>
# else
#  include <io.h>
# endif // TARGET_OS_LINUX
#endif // _MSC_VER
