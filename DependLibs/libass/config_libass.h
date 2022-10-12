#pragma once
#include <NlcDependLibrariesConfig.h>
#include <CoreLib/VxMathDef.h>

//#define CONFIG_HARFBUZZ 0
#define CONFIG_RASTERIZER 1
//#define CONFIG_CORETEXT 0 // apple ?
#ifdef TARGET_OS_WINDOWS
# define CONFIG_DIRECTWRITE 1
#endif // TARGET_OS_WINDOWS
//#define CONFIG_FONTCONFIG 0
//#define CONFIG_ICONV 0
//#define CONFIG_ASM 0
