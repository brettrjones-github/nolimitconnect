#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <stdlib.h>
# define bswap_16		_byteswap_ushort
# define bswap_32		_byteswap_ulong
# define bswap_64		_byteswap_uint64
#else
# include_next <byteswap.h>
#endif // _MSC_VER

