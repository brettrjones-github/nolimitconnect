/*****************************************************************************
 * gotvptop_threads.h : threads implementation for the VideoLAN client
 * This header provides portable declarations for mutexes & conditions
 *****************************************************************************
 * Copyright (C) 1999, 2002 GOTV authors and VideoLAN
 * Copyright © 2007-2016 Rémi Denis-Courmont
 *
 * Authors: Jean-Marc Dressler <polux@via.ecp.fr>
 *          Samuel Hocevar <sam@via.ecp.fr>
 *          Gildas Bazin <gbazin@netcourrier.com>
 *          Christophe Massiot <massiot@via.ecp.fr>
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

#ifndef GOTV_THREADS_H_
#define GOTV_THREADS_H_

#include "gotvptop_config.h"

/**
 * \ingroup os
 * \defgroup thread Threads and synchronization primitives
 * @{
 * \file
 * Thread primitive declarations
 */

/**
 * Issues an explicit deferred cancellation point.
 *
 * This has no effects if thread cancellation is disabled.
 * This can be called when there is a rather slow non-sleeping operation.
 * This is also used to force a cancellation point in a function that would
 * otherwise <em>not always</em> be one (block_FifoGet() is an example).
 */
NLC_API void gotvptop_testcancel(void);

#if defined (TARGET_OS_WINDOWS)
//==== windows implementation ===//
# include <process.h>
# ifndef ETIMEDOUT
#  define ETIMEDOUT 10060 /* This is the value in winsock.h. */
# endif

 #ifdef HAVE_POLL_H
#  include <poll.h>
# elif defined(HAVE_SYS_POLL_H)
#  include <sys/poll.h>
# endif


typedef struct gotvptop_thread *gotvptop_thread_t;
# define GOTV_THREAD_CANCELED NULL
# define LIBGOTV_NEED_SLEEP
typedef struct
{
    bool dynamic;
    union
    {
        struct
        {
            bool locked;
            unsigned long contention;
        };
        CRITICAL_SECTION mutex;
    };
} gotvptop_mutex_t;
# define GOTV_STATIC_MUTEX { false, { { false, 0 } } }
# define LIBGOTV_NEED_CONDVAR
# define LIBGOTV_NEED_SEMAPHORE
# define LIBGOTV_NEED_RWLOCK
typedef struct gotvptop_threadvar *gotvptop_threadvar_t;
typedef struct gotvptop_timer *gotvptop_timer_t;

# define GOTV_THREAD_PRIORITY_LOW      0
# define GOTV_THREAD_PRIORITY_INPUT    THREAD_PRIORITY_ABOVE_NORMAL
# define GOTV_THREAD_PRIORITY_AUDIO    THREAD_PRIORITY_HIGHEST
# define GOTV_THREAD_PRIORITY_VIDEO    0
# define GOTV_THREAD_PRIORITY_OUTPUT   THREAD_PRIORITY_ABOVE_NORMAL
# define GOTV_THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_TIME_CRITICAL

extern int     win32_poll( struct pollfd *fds, unsigned int nfsd, int timeout ); // implemented in VxFunctionsMissingInWindows.cpp.. copied from libnfs

static inline int gotvptop_poll(struct pollfd *fds, unsigned nfds, int timeout)
{
    int val;

    gotvptop_testcancel();
    val = win32_poll(fds, nfds, timeout);
    if (val < 0)
        gotvptop_testcancel();
    return val;
}
# define poll(u,n,t) gotvptop_poll(u, n, t)

#elif defined (__OS2__)
# include <errno.h>

typedef struct gotvptop_thread *gotvptop_thread_t;
#define GOTV_THREAD_CANCELED NULL
typedef struct
{
    bool dynamic;
    union
    {
        struct
        {
            bool locked;
            unsigned long contention;
        };
        HMTX hmtx;
    };
} gotvptop_mutex_t;
#define GOTV_STATIC_MUTEX { false, { { false, 0 } } }
typedef struct
{
    HEV      hev;
    unsigned waiters;
    HEV      hevAck;
    unsigned signaled;
} gotvptop_cond_t;
#define GOTV_STATIC_COND { NULLHANDLE, 0, NULLHANDLE, 0 }
#define LIBGOTV_NEED_SEMAPHORE
#define LIBGOTV_NEED_RWLOCK
typedef struct gotvptop_threadvar *gotvptop_threadvar_t;
typedef struct gotvptop_timer *gotvptop_timer_t;

# define GOTV_THREAD_PRIORITY_LOW      0
# define GOTV_THREAD_PRIORITY_INPUT \
                                    MAKESHORT(PRTYD_MAXIMUM / 2, PRTYC_REGULAR)
# define GOTV_THREAD_PRIORITY_AUDIO    MAKESHORT(PRTYD_MAXIMUM, PRTYC_REGULAR)
# define GOTV_THREAD_PRIORITY_VIDEO    0
# define GOTV_THREAD_PRIORITY_OUTPUT \
                                    MAKESHORT(PRTYD_MAXIMUM / 2, PRTYC_REGULAR)
# define GOTV_THREAD_PRIORITY_HIGHEST  MAKESHORT(0, PRTYC_TIMECRITICAL)

# define pthread_sigmask  sigprocmask

static inline int gotvptop_poll (struct pollfd *fds, unsigned nfds, int timeout)
{
    static int (*gotvptop_poll_os2)(struct pollfd *, unsigned, int) = NULL;

    if (!gotvptop_poll_os2)
    {
        HMODULE hmod;
        CHAR szFailed[CCHMAXPATH];

        if (DosLoadModule(szFailed, sizeof(szFailed), "gotvptopcore", &hmod))
            return -1;

        if (DosQueryProcAddr(hmod, 0, "_gotvptop_poll_os2", (PFN *)&gotvptop_poll_os2))
            return -1;
    }

    return (*gotvptop_poll_os2)(fds, nfds, timeout);
}
# define poll(u,n,t) gotvptop_poll(u, n, t)

#elif defined (TARGET_OS_ANDROID)      /* pthreads subset without pthread_cancel() */
//==== android implementation ===//

# include <unistd.h>
# include <pthread.h>
# include <poll.h>
# define LIBGOTV_USE_PTHREAD_CLEANUP   1
# define LIBGOTV_NEED_SLEEP
# define LIBGOTV_NEED_CONDVAR
# define LIBGOTV_NEED_SEMAPHORE
# define LIBGOTV_NEED_RWLOCK

typedef struct gotvptop_thread *gotvptop_thread_t;
#define GOTV_THREAD_CANCELED NULL
typedef pthread_mutex_t gotvptop_mutex_t;
#define GOTV_STATIC_MUTEX PTHREAD_MUTEX_INITIALIZER

typedef pthread_key_t   gotvptop_threadvar_t;
typedef struct gotvptop_timer *gotvptop_timer_t;

# define GOTV_THREAD_PRIORITY_LOW      0
# define GOTV_THREAD_PRIORITY_INPUT    0
# define GOTV_THREAD_PRIORITY_AUDIO    0
# define GOTV_THREAD_PRIORITY_VIDEO    0
# define GOTV_THREAD_PRIORITY_OUTPUT   0
# define GOTV_THREAD_PRIORITY_HIGHEST  0

static inline int gotvptop_poll (struct pollfd *fds, unsigned nfds, int timeout)
{
    int val;

    do
    {
        int ugly_timeout = ((unsigned)timeout >= 50) ? 50 : timeout;
        if (timeout >= 0)
            timeout -= ugly_timeout;

        gotvptop_testcancel ();
        val = poll (fds, nfds, ugly_timeout);
    }
    while (val == 0 && timeout != 0);

    return val;
}

# define poll(u,n,t) gotvptop_poll(u, n, t)

#elif defined (__APPLE__)
//==== apple implementation ===//

# define _APPLE_C_SOURCE    1 /* Proper pthread semantics on OSX */
# include <unistd.h>
# include <pthread.h>
/* Unnamed POSIX semaphores not supported on Mac OS X */
# include <mach/semaphore.h>
# include <mach/task.h>
# define LIBGOTV_USE_PTHREAD           1
# define LIBGOTV_USE_PTHREAD_CLEANUP   1

typedef pthread_t       gotvptop_thread_t;
#define GOTV_THREAD_CANCELED PTHREAD_CANCELED
typedef pthread_mutex_t gotvptop_mutex_t;
#define GOTV_STATIC_MUTEX PTHREAD_MUTEX_INITIALIZER
typedef pthread_cond_t gotvptop_cond_t;
#define GOTV_STATIC_COND PTHREAD_COND_INITIALIZER
typedef semaphore_t     gotvptop_sem_t;
typedef pthread_rwlock_t gotvptop_rwlock_t;
#define GOTV_STATIC_RWLOCK PTHREAD_RWLOCK_INITIALIZER
typedef pthread_key_t   gotvptop_threadvar_t;
typedef struct gotvptop_timer *gotvptop_timer_t;

# define GOTV_THREAD_PRIORITY_LOW      0
# define GOTV_THREAD_PRIORITY_INPUT   22
# define GOTV_THREAD_PRIORITY_AUDIO   22
# define GOTV_THREAD_PRIORITY_VIDEO    0
# define GOTV_THREAD_PRIORITY_OUTPUT  22
# define GOTV_THREAD_PRIORITY_HIGHEST 22

#else /* POSIX threads */
# include <unistd.h> /* _POSIX_SPIN_LOCKS */
# include <pthread.h>
# include <semaphore.h>

/**
 * Whether LibGOTV threads are based on POSIX threads.
 */
# define LIBGOTV_USE_PTHREAD           1

/**
 * Whether LibGOTV thread cancellation is based on POSIX threads.
 */
# define LIBGOTV_USE_PTHREAD_CLEANUP   1

/**
 * Thread handle.
 */
typedef struct
{
    pthread_t handle;
} gotvptop_thread_t;

/**
 * Return value of a canceled thread.
 */
#define GOTV_THREAD_CANCELED PTHREAD_CANCELED

/**
 * Mutex.
 *
 * Storage space for a mutual exclusion lock.
 */
typedef pthread_mutex_t gotvptop_mutex_t;

/**
 * Static initializer for (static) mutex.
 */
#define GOTV_STATIC_MUTEX PTHREAD_MUTEX_INITIALIZER

/**
 * Condition variable.
 *
 * Storage space for a thread condition variable.
 */
typedef pthread_cond_t  gotvptop_cond_t;

/**
 * Static initializer for (static) condition variable.
 *
 * \note
 * The condition variable will use the default clock, which is OS-dependent.
 * Therefore, where timed waits are necessary the condition variable should
 * always be initialized dynamically explicit instead of using this
 * initializer.
 */
#define GOTV_STATIC_COND  PTHREAD_COND_INITIALIZER

/**
 * Semaphore.
 *
 * Storage space for a thread-safe semaphore.
 */
typedef sem_t           gotvptop_sem_t;

/**
 * Read/write lock.
 *
 * Storage space for a slim reader/writer lock.
 */
typedef pthread_rwlock_t gotvptop_rwlock_t;

/**
 * Static initializer for (static) read/write lock.
 */
#define GOTV_STATIC_RWLOCK PTHREAD_RWLOCK_INITIALIZER

/**
 * Thread-local key handle.
 */
typedef pthread_key_t   gotvptop_threadvar_t;

/**
 * Threaded timer handle.
 */
typedef struct gotvptop_timer *gotvptop_timer_t;

# define GOTV_THREAD_PRIORITY_LOW      0
# define GOTV_THREAD_PRIORITY_INPUT   10
# define GOTV_THREAD_PRIORITY_AUDIO    5
# define GOTV_THREAD_PRIORITY_VIDEO    0
# define GOTV_THREAD_PRIORITY_OUTPUT  15
# define GOTV_THREAD_PRIORITY_HIGHEST 20
//==== end of apple implementation ===//
#endif

//==== common code ===//
#ifdef LIBGOTV_NEED_CONDVAR
typedef struct
{
    unsigned value;
} gotvptop_cond_t;
# define GOTV_STATIC_COND { 0 }
#endif

#ifdef LIBGOTV_NEED_SEMAPHORE
typedef struct gotvptop_sem
{
    gotvptop_mutex_t lock;
    gotvptop_cond_t  wait;
    unsigned    value;
} gotvptop_sem_t;
#endif

#ifdef LIBGOTV_NEED_RWLOCK
typedef struct gotvptop_rwlock
{
    gotvptop_mutex_t   mutex;
    gotvptop_cond_t    wait;
    long          state;
} gotvptop_rwlock_t;
# define GOTV_STATIC_RWLOCK { GOTV_STATIC_MUTEX, GOTV_STATIC_COND, 0 }
#endif

/**
 * Initializes a fast mutex.
 *
 * Recursive locking of a fast mutex is undefined behaviour. (In debug builds,
 * recursive locking will cause an assertion failure.)
 */
NLC_API void gotvptop_mutex_init(gotvptop_mutex_t *);

/**
 * Initializes a recursive mutex.
 * \warning This is strongly discouraged. Please use normal mutexes.
 */
NLC_API void gotvptop_mutex_init_recursive(gotvptop_mutex_t *);

/**
 * Deinitializes a mutex.
 *
 * The mutex must not be locked, otherwise behaviour is undefined.
 */
NLC_API void gotvptop_mutex_destroy(gotvptop_mutex_t *);

/**
 * Acquires a mutex.
 *
 * If needed, this waits for any other thread to release it.
 *
 * \warning Beware of deadlocks when locking multiple mutexes at the same time,
 * or when using mutexes from callbacks.
 *
 * \note This function is not a cancellation point.
 */
NLC_API void gotvptop_mutex_lock(gotvptop_mutex_t *);

/**
 * Tries to acquire a mutex.
 *
 * This function acquires the mutex if and only if it is not currently held by
 * another thread. This function never sleeps and can be used in delay-critical
 * code paths.
 *
 * \note This function is not a cancellation point.
 *
 * \warning If this function fails, then the mutex is held... by another
 * thread. The calling thread must deal with the error appropriately. That
 * typically implies postponing the operations that would have required the
 * mutex. If the thread cannot defer those operations, then it must use
 * gotvptop_mutex_lock(). If in doubt, use gotvptop_mutex_lock() instead.
 *
 * @return 0 if the mutex could be acquired, an error code otherwise.
 */
NLC_API int gotvptop_mutex_trylock( gotvptop_mutex_t * ) GOTV_USED;

/**
 * Releases a mutex.
 *
 * If the mutex is not held by the calling thread, the behaviour is undefined.
 *
 * \note This function is not a cancellation point.
 */
NLC_API void gotvptop_mutex_unlock(gotvptop_mutex_t *);

/**
 * Initializes a condition variable.
 */
NLC_API void gotvptop_cond_init(gotvptop_cond_t *);

/**
 * Initializes a condition variable (wall clock).
 *
 * This function initializes a condition variable for timed waiting using the
 * UTC wall clock time. The time reference is the same as with time() and with
 * timespec_get() and TIME_UTC.
 * gotvptop_cond_timedwait_daytime() must be instead of
 * gotvptop_cond_timedwait() for actual waiting.
 */
void gotvptop_cond_init_daytime(gotvptop_cond_t *);

/**
 * Deinitializes a condition variable.
 *
 * No threads shall be waiting or signaling the condition, otherwise the
 * behavior is undefined.
 */
NLC_API void gotvptop_cond_destroy(gotvptop_cond_t *);

/**
 * Wakes up one thread waiting on a condition variable.
 *
 * If any thread is currently waiting on the condition variable, at least one
 * of those threads will be woken up. Otherwise, this function has no effects.
 *
 * \note This function is not a cancellation point.
 */
NLC_API void gotvptop_cond_signal(gotvptop_cond_t *);

/**
 * Wakes up all threads waiting on a condition variable.
 *
 * \note This function is not a cancellation point.
 */
NLC_API void gotvptop_cond_broadcast(gotvptop_cond_t *);

/**
 * Waits on a condition variable.
 *
 * The calling thread will be suspended until another thread calls
 * gotvptop_cond_signal() or gotvptop_cond_broadcast() on the same condition variable,
 * the thread is cancelled with gotvptop_cancel(), or the system causes a
 * <em>spurious</em> unsolicited wake-up.
 *
 * A mutex is needed to wait on a condition variable. It must <b>not</b> be
 * a recursive mutex. Although it is possible to use the same mutex for
 * multiple condition, it is not valid to use different mutexes for the same
 * condition variable at the same time from different threads.
 *
 * The canonical way to use a condition variable to wait for event foobar is:
 @code
   gotvptop_mutex_lock(&lock);
   mutex_cleanup_push(&lock); // release the mutex in case of cancellation

   while (!foobar)
       gotvptop_cond_wait(&wait, &lock);

   // -- foobar is now true, do something about it here --

   gotvptop_cleanup_pop();
   gotvptop_mutex_unlock(&lock);
  @endcode
 *
 * \note This function is a cancellation point. In case of thread cancellation,
 * the mutex is always locked before cancellation proceeds.
 *
 * \param cond condition variable to wait on
 * \param mutex mutex which is unlocked while waiting,
 *              then locked again when waking up.
 */
NLC_API void gotvptop_cond_wait(gotvptop_cond_t *cond, gotvptop_mutex_t *mutex);

/**
 * Waits on a condition variable up to a certain date.
 *
 * This works like gotvptop_cond_wait() but with an additional time-out.
 * The time-out is expressed as an absolute timestamp using the same arbitrary
 * time reference as the mdate() and mwait() functions.
 *
 * \note This function is a cancellation point. In case of thread cancellation,
 * the mutex is always locked before cancellation proceeds.
 *
 * \param cond condition variable to wait on
 * \param mutex mutex which is unlocked while waiting,
 *              then locked again when waking up
 * \param deadline <b>absolute</b> timeout
 *
 * \warning If the variable was initialized with gotvptop_cond_init_daytime(), or
 * was statically initialized with \ref GOTV_STATIC_COND, the time reference
 * used by this function is unspecified (depending on the implementation, it
 * might be the Unix epoch or the mdate() clock).
 *
 * \return 0 if the condition was signaled, an error code in case of timeout.
 */
NLC_API int gotvptop_cond_timedwait(gotvptop_cond_t *cond, gotvptop_mutex_t *mutex,
                               mtime_t deadline);

int gotvptop_cond_timedwait_daytime(gotvptop_cond_t *, gotvptop_mutex_t *, time_t);

/**
 * Initializes a semaphore.
 *
 * @param count initial semaphore value (typically 0)
 */
NLC_API void gotvptop_sem_init(gotvptop_sem_t *, unsigned count);

/**
 * Deinitializes a semaphore.
 */
NLC_API void gotvptop_sem_destroy(gotvptop_sem_t *);

/**
 * Increments the value of a semaphore.
 *
 * \note This function is not a cancellation point.
 *
 * \return 0 on success, EOVERFLOW in case of integer overflow.
 */
NLC_API int gotvptop_sem_post(gotvptop_sem_t *);

/**
 * Waits on a semaphore.
 *
 * This function atomically waits for the semaphore to become non-zero then
 * decrements it, and returns. If the semaphore is non-zero on entry, it is
 * immediately decremented.
 *
 * \note This function may be a point of cancellation.
 */
NLC_API void gotvptop_sem_wait(gotvptop_sem_t *);

/**
 * Initializes a read/write lock.
 */
NLC_API void gotvptop_rwlock_init(gotvptop_rwlock_t *);

/**
 * Destroys an initialized unused read/write lock.
 */
NLC_API void gotvptop_rwlock_destroy(gotvptop_rwlock_t *);

/**
 * Acquires a read/write lock for reading.
 *
 * \note Recursion is allowed.
 * \note This function may be a point of cancellation.
 */
NLC_API void gotvptop_rwlock_rdlock(gotvptop_rwlock_t *);

/**
 * Acquires a read/write lock for writing. Recursion is not allowed.
 * \note This function may be a point of cancellation.
 */
NLC_API void gotvptop_rwlock_wrlock(gotvptop_rwlock_t *);

/**
 * Releases a read/write lock.
 *
 * The calling thread must hold the lock. Otherwise behaviour is undefined.
 *
 * \note This function is not a cancellation point.
 */
NLC_API void gotvptop_rwlock_unlock(gotvptop_rwlock_t *);

/**
 * Allocates a thread-specific variable.
 *
 * @param key where to store the thread-specific variable handle
 * @param destr a destruction callback. It is called whenever a thread exits
 * and the thread-specific variable has a non-NULL value.
 *
 * @return 0 on success, a system error code otherwise.
 * This function can actually fail: on most systems, there is a fixed limit to
 * the number of thread-specific variables in a given process.
 */
NLC_API int gotvptop_threadvar_create(gotvptop_threadvar_t *key, void (*destr) (void *));

/**
 * Deallocates a thread-specific variable.
 */
NLC_API void gotvptop_threadvar_delete(gotvptop_threadvar_t *);

/**
 * Sets a thread-specific variable.

 * \param key thread-local variable key (created with gotvptop_threadvar_create())
 * \param value new value for the variable for the calling thread
 * \return 0 on success, a system error code otherwise.
 */
NLC_API int gotvptop_threadvar_set(gotvptop_threadvar_t key, void *value);

/**
 * Gets the value of a thread-local variable for the calling thread.
 * This function cannot fail.
 *
 * \return the value associated with the given variable for the calling
 * or NULL if no value was set.
 */
NLC_API void *gotvptop_threadvar_get(gotvptop_threadvar_t);

/**
 * Waits on an address.
 *
 * Puts the calling thread to sleep if a specific value is stored at a
 * specified address. The thread will sleep until it is woken up by a call to
 * gotvptop_addr_signal() or gotvptop_addr_broadcast() in another thread, or spuriously.
 *
 * If the value does not match, do nothing and return immediately.
 *
 * \param addr address to check for
 * \param val value to match at the address
 */
void gotvptop_addr_wait(void *addr, unsigned val);

/**
 * Waits on an address with a time-out.
 *
 * This function operates as gotvptop_addr_wait() but provides an additional
 * time-out. If the time-out elapses, the thread resumes and the function
 * returns.
 *
 * \param addr address to check for
 * \param val value to match at the address
 * \param delay time-out duration
 *
 * \return true if the function was woken up before the time-out,
 * false if the time-out elapsed.
 */
bool gotvptop_addr_timedwait(void *addr, unsigned val, mtime_t delay);

/**
 * Wakes up one thread on an address.
 *
 * Wakes up (at least) one of the thread sleeping on the specified address.
 * The address must be equal to the first parameter given by at least one
 * thread sleeping within the gotvptop_addr_wait() or gotvptop_addr_timedwait()
 * functions. If no threads are found, this function does nothing.
 *
 * \param addr address identifying which threads may be woken up
 */
void gotvptop_addr_signal(void *addr);

/**
 * Wakes up all thread on an address.
 *
 * Wakes up all threads sleeping on the specified address (if any).
 * Any thread sleeping within a call to gotvptop_addr_wait() or gotvptop_addr_timedwait()
 * with the specified address as first call parameter will be woken up.
 *
 * \param addr address identifying which threads to wake up
 */
void gotvptop_addr_broadcast(void *addr);

/**
 * Creates and starts a new thread.
 *
 * The thread must be <i>joined</i> with gotvptop_join() to reclaim resources
 * when it is not needed anymore.
 *
 * @param th storage space for the handle of the new thread (cannot be NULL)
 *           [OUT]
 * @param entry entry point for the thread
 * @param data data parameter given to the entry point
 * @param priority thread priority value
 * @return 0 on success, a standard error code on error.
 * @note In case of error, the value of *th is undefined.
 */
NLC_API int gotvptop_clone(gotvptop_thread_t *th, void *(*entry)(void *), void *data,
                      int priority) GOTV_USED;

/**
 * Marks a thread as cancelled.
 *
 * Next time the target thread reaches a cancellation point (while not having
 * disabled cancellation), it will run its cancellation cleanup handler, the
 * thread variable destructors, and terminate.
 *
 * gotvptop_join() must be used regardless of a thread being cancelled or not, to
 * avoid leaking resources.
 */
NLC_API void gotvptop_cancel(gotvptop_thread_t);

/**
 * Waits for a thread to complete (if needed), then destroys it.
 *
 * \note This is a cancellation point. In case of cancellation, the thread is
 * <b>not</b> joined.

 * \warning A thread cannot join itself (normally GOTV will abort if this is
 * attempted). Also a detached thread <b>cannot</b> be joined.
 *
 * @param th thread handle
 * @param result [OUT] pointer to write the thread return value or NULL
 */
NLC_API void gotvptop_join(gotvptop_thread_t th, void **result);

/**
 * Disables thread cancellation.
 *
 * This functions saves the current cancellation state (enabled or disabled),
 * then disables cancellation for the calling thread. It must be called before
 * entering a piece of code that is not cancellation-safe, unless it can be
 * proven that the calling thread will not be cancelled.
 *
 * \note This function is not a cancellation point.
 *
 * \return Previous cancellation state (opaque value for gotvptop_restorecancel()).
 */
NLC_API int gotvptop_savecancel(void);

/**
 * Restores the cancellation state.
 *
 * This function restores the cancellation state of the calling thread to
 * a state previously saved by gotvptop_savecancel().
 *
 * \note This function is not a cancellation point.
 *
 * \param state previous state as returned by gotvptop_savecancel().
 */
NLC_API void gotvptop_restorecancel(int state);

/**
 * Internal handler for thread cancellation.
 *
 * Do not call this function directly. Use wrapper macros instead:
 * gotvptop_cleanup_push(), gotvptop_cleanup_pop().
 */
NLC_API void gotvptop_control_cancel(int cmd, ...);

/**
 * Thread handle.
 *
 * This function returns the thread handle of the calling thread.
 *
 * \note The exact type of the thread handle depends on the platform,
 * including an integer type, a pointer type or a compound type of any size.
 * If you need an integer identifier, use gotvptop_thread_id() instead.
 *
 * \note gotvptop_join(gotvptop_thread_self(), NULL) is undefined,
 * as it obviously does not make any sense (it might result in a deadlock, but
 * there are no warranties that it will).
 *
 * \return the thread handle
 */
NLC_API gotvptop_thread_t gotvptop_thread_self(void) GOTV_USED;

/**
 * Thread identifier.
 *
 * This function returns the identifier of the calling thread. The identifier
 * cannot change for the entire duration of the thread, and no other thread can
 * have the same identifier at the same time in the same process. Typically,
 * the identifier is also unique across all running threads of all existing
 * processes, but that depends on the operating system.
 *
 * There are no particular semantics to the thread ID with LibGOTV.
 * It is provided mainly for tracing and debugging.
 *
 * \warning This function is not currently implemented on all supported
 * platforms. Where not implemented, it returns (unsigned long)-1.
 *
 * \return the thread identifier (or -1 if unimplemented)
 */
NLC_API unsigned long gotvptop_thread_id(void) GOTV_USED;

/**
 * Precision monotonic clock.
 *
 * In principles, the clock has a precision of 1 MHz. But the actual resolution
 * may be much lower, especially when it comes to sleeping with mwait() or
 * msleep(). Most general-purpose operating systems provide a resolution of
 * only 100 to 1000 Hz.
 *
 * \warning The origin date (time value "zero") is not specified. It is
 * typically the time the kernel started, but this is platform-dependent.
 * If you need wall clock time, use gettimeofday() instead.
 *
 * \return a timestamp in microseconds.
 */
NLC_API mtime_t mdate(void);

/**
 * Waits until a deadline.
 *
 * \param deadline timestamp to wait for (\ref mdate())
 *
 * \note The deadline may be exceeded due to OS scheduling.
 * \note This function is a cancellation point.
 */
NLC_API void mwait(mtime_t deadline);

/**
 * Waits for an interval of time.
 *
 * \param delay how long to wait (in microseconds)
 *
 * \note The delay may be exceeded due to OS scheduling.
 * \note This function is a cancellation point.
 */
NLC_API void msleep(mtime_t delay);

#define GOTV_HARD_MIN_SLEEP   10000 /* 10 milliseconds = 1 tick at 100Hz */
#define GOTV_SOFT_MIN_SLEEP 9000000 /* 9 seconds */

#if defined (__GNUC__) && !defined (__clang__)
/* Linux has 100, 250, 300 or 1000Hz
 *
 * HZ=100 by default on FreeBSD, but some architectures use a 1000Hz timer
 */

static
__attribute__((unused))
__attribute__((noinline))
__attribute__((error("sorry, cannot sleep for such short a time")))
mtime_t impossible_delay( mtime_t delay )
{
    (void) delay;
    return GOTV_HARD_MIN_SLEEP;
}

static
__attribute__((unused))
__attribute__((noinline))
__attribute__((warning("use proper event handling instead of short delay")))
mtime_t harmful_delay( mtime_t delay )
{
    return delay;
}

# define check_delay( d ) \
    ((__builtin_constant_p(d < GOTV_HARD_MIN_SLEEP) \
   && (d < GOTV_HARD_MIN_SLEEP)) \
       ? impossible_delay(d) \
       : ((__builtin_constant_p(d < GOTV_SOFT_MIN_SLEEP) \
       && (d < GOTV_SOFT_MIN_SLEEP)) \
           ? harmful_delay(d) \
           : d))

static
__attribute__((unused))
__attribute__((noinline))
__attribute__((error("deadlines can not be constant")))
mtime_t impossible_deadline( mtime_t deadline )
{
    return deadline;
}

# define check_deadline( d ) \
    (__builtin_constant_p(d) ? impossible_deadline(d) : d)
#else
# define check_delay(d) (d)
# define check_deadline(d) (d)
#endif

#define msleep(d) msleep(check_delay(d))
#define mwait(d) mwait(check_deadline(d))

/**
 * Initializes an asynchronous timer.
 *
 * \param id pointer to timer to be initialized
 * \param func function that the timer will call
 * \param data parameter for the timer function
 * \return 0 on success, a system error code otherwise.
 *
 * \warning Asynchronous timers are processed from an unspecified thread.
 * \note Multiple occurrences of a single interval timer are serialized:
 * they cannot run concurrently.
 */
NLC_API int gotvptop_timer_create(gotvptop_timer_t *id, void (*func)(void *), void *data)
GOTV_USED;

/**
 * Destroys an initialized timer.
 *
 * If needed, the timer is first disarmed. Behaviour is undefined if the
 * specified timer is not initialized.
 *
 * \warning This function <b>must</b> be called before the timer data can be
 * freed and before the timer callback function can be unmapped/unloaded.
 *
 * \param timer timer to destroy
 */
NLC_API void gotvptop_timer_destroy(gotvptop_timer_t timer);

/**
 * Arms or disarms an initialized timer.
 *
 * This functions overrides any previous call to itself.
 *
 * \note A timer can fire later than requested due to system scheduling
 * limitations. An interval timer can fail to trigger sometimes, either because
 * the system is busy or suspended, or because a previous iteration of the
 * timer is still running. See also gotvptop_timer_getoverrun().
 *
 * \param timer initialized timer
 * \param absolute the timer value origin is the same as mdate() if true,
 *                 the timer value is relative to now if false.
 * \param value zero to disarm the timer, otherwise the initial time to wait
 *              before firing the timer.
 * \param interval zero to fire the timer just once, otherwise the timer
 *                 repetition interval.
 */
NLC_API void gotvptop_timer_schedule(gotvptop_timer_t timer, bool absolute,
                                mtime_t value, mtime_t interval);

/**
 * Fetches and resets the overrun counter for a timer.
 *
 * This functions returns the number of times that the interval timer should
 * have fired, but the callback was not invoked due to scheduling problems.
 * The call resets the counter to zero.
 *
 * \param timer initialized timer
 * \return the timer overrun counter (typically zero)
 */
NLC_API unsigned gotvptop_timer_getoverrun(gotvptop_timer_t) GOTV_USED;

/**
 * Count CPUs.
 *
 * \return number of available (logical) CPUs.
 */
NLC_API unsigned gotvptop_GetCPUCount(void);

enum
{
    GOTV_CLEANUP_PUSH,
    GOTV_CLEANUP_POP,
    GOTV_CANCEL_ADDR_SET,
    GOTV_CANCEL_ADDR_CLEAR,
};

#if defined (LIBGOTV_USE_PTHREAD_CLEANUP)
/**
 * Registers a thread cancellation handler.
 *
 * This pushes a function to run if the thread is cancelled (or otherwise
 * exits prematurely).
 *
 * If multiple procedures are registered,
 * they are handled in last-in first-out order.
 *
 * \note Any call to gotvptop_cleanup_push() <b>must</b> paired with a call to
 * gotvptop_cleanup_pop().
 * \warning Branching into or out of the block between these two function calls
 * is not allowed (read: it will likely crash the whole process).
 *
 * \param routine procedure to call if the thread ends
 * \param arg argument for the procedure
 */
# define gotvptop_cleanup_push( routine, arg ) pthread_cleanup_push (routine, arg)

/**
 * Unregisters the last cancellation handler.
 *
 * This pops the cancellation handler that was last pushed with
 * gotvptop_cleanup_push() in the calling thread.
 */
# define gotvptop_cleanup_pop( ) pthread_cleanup_pop (0)

#else
typedef struct gotvptop_cleanup_t gotvptop_cleanup_t;

struct gotvptop_cleanup_t
{
    gotvptop_cleanup_t *next;
    void         (*proc) (void *);
    void          *data;
};

/* This macros opens a code block on purpose. This is needed for multiple
 * calls within a single function. This also prevent Win32 developers from
 * writing code that would break on POSIX (POSIX opens a block as well). */
# define gotvptop_cleanup_push( routine, arg ) \
    do { \
        gotvptop_cleanup_t gotvptop_cleanup_data = { NULL, routine, arg, }; \
        gotvptop_control_cancel (GOTV_CLEANUP_PUSH, &gotvptop_cleanup_data)

# define gotvptop_cleanup_pop( ) \
        gotvptop_control_cancel (GOTV_CLEANUP_POP); \
    } while (0)

#endif /* !LIBGOTV_USE_PTHREAD_CLEANUP */

static inline void gotvptop_cleanup_lock (void *lock)
{
    gotvptop_mutex_unlock ((gotvptop_mutex_t *)lock);
}
#define mutex_cleanup_push( lock ) gotvptop_cleanup_push (gotvptop_cleanup_lock, lock)

static inline void gotvptop_cancel_addr_set(void *addr)
{
    gotvptop_control_cancel(GOTV_CANCEL_ADDR_SET, addr);
}

static inline void gotvptop_cancel_addr_clear(void *addr)
{
    gotvptop_control_cancel(GOTV_CANCEL_ADDR_CLEAR, addr);
}

/**
 * Helper C++ class to lock a mutex.
 *
 * The mutex is locked when the object is created, and unlocked when the object
 * is destroyed.
 */
class gotvptop_mutex_locker
{
    private:
        gotvptop_mutex_t *lock;
    public:
        gotvptop_mutex_locker (gotvptop_mutex_t *m) : lock (m)
        {
            gotvptop_mutex_lock (lock);
        }

        ~gotvptop_mutex_locker (void)
        {
            gotvptop_mutex_unlock (lock);
        }
};

enum
{
   GOTV_AVCODEC_MUTEX = 0,
   GOTV_GCRYPT_MUTEX,
   GOTV_XLIB_MUTEX,
   GOTV_MOSAIC_MUTEX,
   GOTV_HIGHLIGHT_MUTEX,
#ifdef _WIN32
   GOTV_MTA_MUTEX,
#endif
   /* Insert new entry HERE */
   GOTV_MAX_MUTEX
};

/**
 * Internal handler for global mutexes.
 *
 * Do not use this function directly. Use helper macros instead:
 * gotvptop_global_lock(), gotvptop_global_unlock().
 */
NLC_API void gotvptop_global_mutex(unsigned, bool);

/**
 * Acquires a global mutex.
 */
#define gotvptop_global_lock( n ) gotvptop_global_mutex(n, true)

/**
 * Releases a global mutex.
 */
#define gotvptop_global_unlock( n ) gotvptop_global_mutex(n, false)

/** @} */

#endif /* !_GOTV_THREADS_H */
