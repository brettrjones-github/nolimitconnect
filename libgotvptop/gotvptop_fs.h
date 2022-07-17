/*****************************************************************************
 * gotvptop_fs.h: File system helpers
 *****************************************************************************
 * Copyright © 2006-2010 Rémi Denis-Courmont
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GOTV_FS_H
#define GOTV_FS_H 1

#include <sys/types.h>
#include <dirent.h>

struct stat;
struct iovec;

#ifdef _WIN32
# include <sys/stat.h>
# ifndef stat
#  define stat _stati64
# endif
# ifndef fstat
#  define fstat _fstati64
# endif
# ifndef _MSC_VER
#  undef lseek
#  define lseek _lseeki64
# endif
#endif

#ifdef __ANDROID__
# define lseek lseek64
#endif


/**
 * \defgroup os Operating system
 * @{
 * \defgroup file File system
 * @{
 *
 * \file
 * The functions in this file help with using low-level Unix-style file
 * descriptors, BSD sockets and directories. In general, they retain the
 * prototype and most semantics from their respective standard equivalents.
 * However, there are a few differences:
 *  - On Windows, file path arguments are expected in UTF-8 format.
 *    They are converted to UTF-16 internally, thus enabling access to paths
 *    outside of the local Windows ANSI code page.
 *  - On POSIX systems, file descriptors are created with the close-on-exec
 *    flag set (atomically where possible), so that they do not leak to
 *    child process after fork-and-exec.
 *  - gotvptop_scandir(), inspired by GNU scandir(), passes file names rather than
 *    dirent structure pointers to its callbacks.
 *  - gotvptop_accept() takes an extra boolean for nonblocking mode (compare with
 *    the flags parameter in POSIX.next accept4()).
 *  - Writing functions do not emit a SIGPIPE signal in case of broken pipe.
 *
 * \defgroup fd File descriptors
 * @{
 */

/**
 * Opens a system file handle.
 *
 * @param filename file path to open (with UTF-8 encoding)
 * @param flags open() flags, see the C library open() documentation
 * @return a file handle on success, -1 on error (see errno).
 * @note Contrary to standard open(), this function returns a file handle
 * with the close-on-exec flag preset.
 */
NLC_API int gotvptop_open(const char *filename, int flags, ...) GOTV_USED;

/**
 * Opens a system file handle relative to an existing directory handle.
 *
 * @param dir directory file descriptor
 * @param filename file path to open (with UTF-8 encoding)
 * @param flags open() flags, see the C library open() documentation
 * @return a file handle on success, -1 on error (see errno).
 * @note Contrary to standard open(), this function returns a file handle
 * with the close-on-exec flag preset.
 */
NLC_API int gotvptop_openat(int fd, const char *filename, int flags, ...) GOTV_USED;

NLC_API int gotvptop_mkstemp( char * );

/**
 * Duplicates a file descriptor. The new file descriptor has the close-on-exec
 * descriptor flag preset.
 * @return a new file descriptor, -1 (see errno)
 */
NLC_API int gotvptop_dup(int) GOTV_USED;

/**
 * Creates a pipe (see "man pipe" for further reference). The new file
 * descriptors have the close-on-exec flag preset.
 * @return 0 on success, -1 on error (see errno)
 */
NLC_API int gotvptop_pipe(int [2]) GOTV_USED;

/**
 * Creates an anonymous regular file descriptor, i.e. a descriptor for a
 * temporary file.
 *
 * The file is initially empty. The storage space is automatically reclaimed
 * when all file descriptors referencing it are closed.
 *
 * The new file descriptor has the close-on-exec flag preset.
 *
 * @return a file descriptor on success, -1 on error (see errno)
 */
NLC_API int gotvptop_memfd(void) GOTV_USED;

/**
 * Writes data to a file descriptor. Unlike write(), if EPIPE error occurs,
 * this function does not generate a SIGPIPE signal.
 * @note If the file descriptor is known to be neither a pipe/FIFO nor a
 * connection-oriented socket, the normal write() should be used.
 */
NLC_API ssize_t gotvptop_write(int, const void *, size_t);

/**
 * Writes data from an iovec structure to a file descriptor. Unlike writev(),
 * if EPIPE error occurs, this function does not generate a SIGPIPE signal.
 */
NLC_API ssize_t gotvptop_writev(int, const struct iovec *, int);

/**
 * Closes a file descriptor.
 *
 * This closes a file descriptor. If this is a last file descriptor for the
 * underlying open file, the file is closed too; the exact semantics depend
 * on the type of file.
 *
 * @note The file descriptor is always closed when the function returns, even
 * if the function returns an error. The sole exception is if the file
 * descriptor was not currently valid, and thus cannot be closed (errno will
 * then be set to EBADF).
 *
 * @param fd file descriptor
 * @return Normally, zero is returned.
 * If an I/O error is detected before or while closing, the function may return
 * -1. Such an error is unrecoverable since the descriptor is closed.
 *
 * A nul return value does not necessarily imply that all pending I/O
 * succeeded, since I/O might still occur asynchronously afterwards.
 */
NLC_API int gotvptop_close(int fd);

/**
 * @}
 */

/**
 * Finds file/inode information - like stat().
 * @note As far as possible, fstat() should be used instead.
 *
 * @param filename UTF-8 file path
 */
NLC_API int gotvptop_stat(const char *filename, struct stat *) GOTV_USED;

/**
 * Finds file/inode information, as lstat().
 * Consider using fstat() instead, if possible.
 *
 * @param filename UTF-8 file path
 */
NLC_API int gotvptop_lstat(const char *filename, struct stat *) GOTV_USED;

/**
 * Removes a file.
 *
 * @param filename a UTF-8 string with the name of the file you want to delete.
 * @return A 0 return value indicates success. A -1 return value indicates an
 *        error, and an error code is stored in errno
 */
NLC_API int gotvptop_unlink(const char *filename);

/**
 * Moves a file atomically. This only works within a single file system.
 *
 * @param oldpath path to the file before the move
 * @param newpath intended path to the file after the move
 * @return A 0 return value indicates success. A -1 return value indicates an
 *        error, and an error code is stored in errno
 */
NLC_API int gotvptop_rename(const char *oldpath, const char *newpath);

NLC_API FILE * gotvptop_fopen( const char *filename, const char *mode ) GOTV_USED;

/**
 * \defgroup dir Directories
 * @{
 */

/**
 * Opens a DIR pointer.
 *
 * @param dirname UTF-8 representation of the directory name
 * @return a pointer to the DIR struct, or NULL in case of error.
 * Release with standard closedir().
 */
NLC_API DIR *gotvptop_opendir(const char *dirname) GOTV_USED;

/**
 * Reads the next file name from an open directory.
 *
 * @param dir directory handle as returned by gotvptop_opendir()
 *            (must not be used by another thread concurrently)
 *
 * @return a UTF-8 string of the directory entry. The string is valid until
 * the next call to gotvptop_readdir() or closedir() on the handle.
 * If there are no more entries in the directory, NULL is returned.
 * If an error occurs, errno is set and NULL is returned.
 */
NLC_API const char *gotvptop_readdir(DIR *dir) GOTV_USED;

NLC_API int gotvptop_loaddir( DIR *dir, char ***namelist, int (*select)( const char * ), int (*compar)( const char **, const char ** ) );
NLC_API int gotvptop_scandir( const char *dirname, char ***namelist, int (*select)( const char * ), int (*compar)( const char **, const char ** ) );

/**
 * Creates a directory.
 *
 * @param dirname a UTF-8 string with the name of the directory that you
 *        want to create.
 * @param mode directory permissions
 * @return 0 on success, -1 on error (see errno).
 */
NLC_API int gotvptop_mkdir(const char *dirname, mode_t mode);

/**
 * Determines the current working directory.
 *
 * @return the current working directory (must be free()'d)
 *         or NULL on error
 */
NLC_API char *gotvptop_getcwd(void) GOTV_USED;

/** @} */
/** @} */

#if defined( _WIN32 )
typedef struct gotvptop_DIR
{
    _WDIR *wdir; /* MUST be first, see <gotvptop_fs.h> */
    char *entry;
    union
    {
        DWORD drives;
        bool insert_dot_dot;
    } u;
} gotvptop_DIR;

static inline int gotvptop_closedir( DIR *dir )
{
    gotvptop_DIR *vdir = (gotvptop_DIR *)dir;
    _WDIR *wdir = vdir->wdir;

    free( vdir->entry );
    free( vdir );
    return (wdir != NULL) ? _wclosedir( wdir ) : 0;
}
# undef closedir
# define closedir gotvptop_closedir

static inline void gotvptop_rewinddir( DIR *dir )
{
    _WDIR *wdir = *(_WDIR **)dir;

    _wrewinddir( wdir );
}
# undef rewinddir
# define rewinddir gotvptop_rewinddir
#endif

#ifdef __ANDROID__
# define lseek lseek64
#endif

#endif
