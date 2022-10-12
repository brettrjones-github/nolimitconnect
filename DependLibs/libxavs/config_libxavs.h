#pragma once
#include <NlcDependLibrariesConfig.h>

#undef HAVE_MMX // libxavs seems to be missing some functions and uses defined instead of #if
#undef HAVE_MMXEXT // libxavs seems to be missing some functions and uses defined instead of #if
#undef HAVE_SSE2 // libxavs FIXME need assembly
