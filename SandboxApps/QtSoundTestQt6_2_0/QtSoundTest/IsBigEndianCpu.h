#ifndef IS_BIG_ENDIAN_CPU_H
#define IS_BIG_ENDIAN_CPU_H

#ifdef TARGET_OS_WINDOWS
# include <WinSock2.h>
#else
# include <arpa/inet.h>
# ifndef ntohl
// some systems require netinet/in.h instead of arpa/inet.h
#  include <netinet/in.h>
# endif // ntohl
#endif

#define IsBigEndianCpu() ((ntohl(4L) == 4L) ? true : false)

#define htonU64(val)	(IsBigEndianCpu() ? (val) : ((htonl(((uint64_t)val >> 32) & 0xFFFFFFFF)) | ((uint64_t)(htonl((uint64_t)val & 0xFFFFFFFF)) << 32) ) )
#define ntohU64(val)	(IsBigEndianCpu() ? (val) : ((ntohl(((uint64_t)val >> 32) & 0xFFFFFFFF)) | ((uint64_t)(ntohl((uint64_t)val & 0xFFFFFFFF)) << 32) ) )

#define swapU16(val)	(((val>>8) & 0x00ff) | ((val<<8) & 0xff00))
#define swapU32(val)	(((val>>24) & 0x000000ff) | ((val>>8)  & 0x0000ff00) | ((val<<8)  & 0x00ff0000) | ((val<<24) & 0xff000000))
#define swapU64(val)	val = (((uint64_t)swapU32((val >> 32) & 0xFFFFFFFF) | (uint64_t)swapU32(val& 0xFFFFFFFF))

#endif // IS_BIG_ENDIAN_CPU_H
