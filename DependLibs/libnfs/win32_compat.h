/*
Copyright (c) 2006 by Dan Kennedy.
Copyright (c) 2006 by Juliusz Chroboczek.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/*Adaptions by memphiz@xbmc.org*/

#ifndef win32_COMPAT_H_
#define win32_COMPAT_H_
#include <libnfs/config_libnfs.h>

#ifdef TARGET_OS_WINDOWS

#define NO_IPv6 1

#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#include <basetsd.h>
#include <io.h>
#include <sys/stat.h>
#include <time.h>

//typedef int uid_t;
//typedef int gid_t;
typedef int socklen_t;


#define F_GETFL  3
#define F_SETFL  4

#ifndef S_IFLNK
#define S_IFLNK        0xA000  /* Link */
#endif

#ifndef S_IFIFO
#define S_IFIFO        0x1000  /* FIFO */
#endif

#ifndef S_IFBLK
#define S_IFBLK        0x3000  /* Block: Is this ever set under w32? */
#endif

#ifndef S_IFSOCK
#define S_IFSOCK 0x0           /* not defined in mingw either */
#endif

#ifndef major
#define major(a) 0
#endif

#ifndef minor
#define minor(a) 0
#endif

#define O_NONBLOCK 0x40000000
#define O_SYNC 0

#define MSG_DONTWAIT 0
#define ssize_t SSIZE_T

#if(_WIN32_WINNT < 0x0600)

#define POLLIN      0x0001    /* There is data to read */
#define POLLPRI     0x0002    /* There is urgent data to read */
#define POLLOUT     0x0004    /* Writing now will not block */
#define POLLERR     0x0008    /* Error condition */
#define POLLHUP     0x0010    /* Hung up */
#define POLLNVAL    0x0020    /* Invalid request: fd not open */

struct pollfd {
    SOCKET fd;        /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};
#endif

//#define close closesocket
#define ioctl ioctlsocket

/* Wrapper macros to call misc. functions win32 is missing */
#define poll(x, y, z)        win32_poll(x, y, z)
#define inet_pton(x,y,z)     win32_inet_pton(x,y,z)
int     win32_inet_pton(int af, const char * src, void * dst);
int     win32_poll(struct pollfd *fds, unsigned int nfsd, int timeout);
int     win32_gettimeofday(struct timeval *tv, struct timezone *tz);
#ifdef __MINGW32__
# define win32_gettimeofday mingw_gettimeofday
#endif

#define DllExport

#endif // TARGET_OS_WINDOWS

#endif//win32_COMPAT_H_
