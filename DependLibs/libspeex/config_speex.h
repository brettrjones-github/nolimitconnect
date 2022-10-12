
#pragma once

#include <NlcDependLibrariesConfig.h>

#define SPEEX_EXPORT	NLC_EXPORT

#ifdef NLC_FLOATING_POINT
# define FLOATING_POINT 1
#else
# define FIXED_POINT 1
#endif // NLC_FLOATING_POINT

#ifdef FLOATING_POINT
#  define USE_SMALLFT
#else
#  define USE_KISS_FFT 1
#endif

#ifndef M_PI
# define M_PI           3.14159265358979323846  /* pi */
#endif

#ifdef _MSC_VER
# pragma warning( disable: 4305 ) // truncation from 'double' to 'float'
#endif // _MSC_VER
