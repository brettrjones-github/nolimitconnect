/*================================================================
** Copyright 2000, Clark Cooper
** All rights reserved.
**
** This is free software. You are permitted to copy, distribute, or modify
** it under the terms of the MIT/X license (contained in the COPYING file
** with this distribution.)
*/

#ifndef WINCONFIG_H_
#define WINCONFIG_H_

// cannot include GoTvDependLibrariesConfig because of name clash with EXP_ELEMENT_TYPE
//#include <GoTvDependLibrariesConfig.h>

//#ifdef TARGET_OS_WINDOWS

    #define WIN32_LEAN_AND_MEAN
    #include <WinSock2.h>
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN

    #include <memory.h>
    #include <string.h>

    #define HAVE_MEMMOVE 1
    #define XML_NS 1
    #define XML_DTD 1
    #define XML_CONTEXT_BYTES 1024

    /* we will assume all Windows platforms are little endian */
    #define BYTEORDER 1234

//#endif // TARGET_OS_WINDOWS

/* Windows has memmove() available. */
//#define HAVE_MEMMOVE defined in GoTvDependLibrariesConfig

#endif /* ndef WINCONFIG_H */
