#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
typedef uint8_t			__u8;
typedef uint16_t		__u16;
typedef uint32_t		__u32;
typedef uint64_t		__u64;
#else
typedef uint8_t			__u8;
typedef uint16_t		__u16;
typedef uint32_t		__u32;
//typedef uint64_t		__u64;
# include_next <asm/types.h>
#endif // _MSC_VER
