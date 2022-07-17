/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
/* A GNU-like <signal_gnu.h>.

   Copyright (C) 2006-2016 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#pragma once
#include <config_libgnu.h>
#include <libgnu/unistd_gnu.h>

#include <libpthread/pthread.h>
#include <sys/types.h>
#include <signal.h>

NLC_BEGIN_CDECLARES



/* _GL_CXXALIAS_SYS_CAST (func, rettype, parameters);
   is like  _GL_CXXALIAS_SYS (func, rettype, parameters);
   except that the C function func may have a slightly different declaration.
   A cast is used to silence the "invalid conversion" error that would
   otherwise occur.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIAS_SYS_CAST(func,rettype,parameters) \
    namespace GNULIB_NAMESPACE                          \
    {                                                   \
      static rettype (*func) parameters =               \
        reinterpret_cast<rettype(*)parameters>(::func); \
    }                                                   \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_SYS_CAST(func,rettype,parameters) \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIAS_SYS_CAST2 (func, rettype, parameters, rettype2, parameters2);
   is like  _GL_CXXALIAS_SYS (func, rettype, parameters);
   except that the C function is picked among a set of overloaded functions,
   namely the one with rettype2 and parameters2.  Two consecutive casts
   are used to silence the "cannot find a match" and "invalid conversion"
   errors that would otherwise occur.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
  /* The outer cast must be a reinterpret_cast.
     The inner cast: When the function is defined as a set of overloaded
     functions, it works as a static_cast<>, choosing the designated variant.
     When the function is defined as a single variant, it works as a
     reinterpret_cast<>. The parenthesized cast syntax works both ways.  */
# define _GL_CXXALIAS_SYS_CAST2(func,rettype,parameters,rettype2,parameters2) \
    namespace GNULIB_NAMESPACE                                                \
    {                                                                         \
      static rettype (*func) parameters =                                     \
        reinterpret_cast<rettype(*)parameters>(                               \
          (rettype2(*)parameters2)(::func));                                  \
    }                                                                         \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#else
# define _GL_CXXALIAS_SYS_CAST2(func,rettype,parameters,rettype2,parameters2) \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIASWARN (func);
   causes a warning to be emitted when ::func is used but not when
   GNULIB_NAMESPACE::func is used.  func must be defined without overloaded
   variants.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIASWARN(func) \
   _GL_CXXALIASWARN_1 (func, GNULIB_NAMESPACE)
# define _GL_CXXALIASWARN_1(func,namespace) \
   _GL_CXXALIASWARN_2 (func, namespace)
/* To work around GCC bug <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43881>,
   we enable the warning only when not optimizing.  */
# if !__OPTIMIZE__
#  define _GL_CXXALIASWARN_2(func,namespace) \
    _GL_WARN_ON_USE (func, \
                     "The symbol ::" #func " refers to the system function. " \
                     "Use " #namespace "::" #func " instead.")
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
#  define _GL_CXXALIASWARN_2(func,namespace) \
     extern __typeof__ (func) func
# else
#  define _GL_CXXALIASWARN_2(func,namespace) \
     NLC_EXTERN_C int _gl_cxxalias_dummy
# endif
#else
# define _GL_CXXALIASWARN(func) \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#endif

/* _GL_CXXALIASWARN1 (func, rettype, parameters_and_attributes);
   causes a warning to be emitted when the given overloaded variant of ::func
   is used but not when GNULIB_NAMESPACE::func is used.  */
#if defined __cplusplus && defined GNULIB_NAMESPACE
# define _GL_CXXALIASWARN1(func,rettype,parameters_and_attributes) \
   _GL_CXXALIASWARN1_1 (func, rettype, parameters_and_attributes, \
                        GNULIB_NAMESPACE)
# define _GL_CXXALIASWARN1_1(func,rettype,parameters_and_attributes,namespace) \
   _GL_CXXALIASWARN1_2 (func, rettype, parameters_and_attributes, namespace)
/* To work around GCC bug <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43881>,
   we enable the warning only when not optimizing.  */
# if !__OPTIMIZE__
#  define _GL_CXXALIASWARN1_2(func,rettype,parameters_and_attributes,namespace) \
    _GL_WARN_ON_USE_CXX (func, rettype, parameters_and_attributes, \
                         "The symbol ::" #func " refers to the system function. " \
                         "Use " #namespace "::" #func " instead.")
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
#  define _GL_CXXALIASWARN1_2(func,rettype,parameters_and_attributes,namespace) \
     extern __typeof__ (func) func
# else
#  define _GL_CXXALIASWARN1_2(func,rettype,parameters_and_attributes,namespace) \
     NLC_EXTERN_C int _gl_cxxalias_dummy
# endif
#else
# define _GL_CXXALIASWARN1(func,rettype,parameters_and_attributes) \
    NLC_EXTERN_C int _gl_cxxalias_dummy
#endif


/* The definition of _GL_ARG_NONNULL is copied here.  */
/* _GL_ARG_NONNULL((n,...,m)) tells the compiler and static analyzer tools
   that the values passed as arguments n, ..., m must be non-NULL pointers.
   n = 1 stands for the first argument, n = 2 for the second argument etc.  */
#ifndef _GL_ARG_NONNULL
# if (__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || __GNUC__ > 3
#  define _GL_ARG_NONNULL(params) __attribute__ ((__nonnull__ params))
# else
#  define _GL_ARG_NONNULL(params)
# endif
#endif

/* The definition of _GL_WARN_ON_USE is copied here.  */
#ifndef _GL_WARN_ON_USE

# if 4 < __GNUC__ || (__GNUC__ == 4 && 3 <= __GNUC_MINOR__)
/* A compiler attribute is available in gcc versions 4.3.0 and later.  */
#  define _GL_WARN_ON_USE(function, message) \
extern __typeof__ (function) function __attribute__ ((__warning__ (message)))
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
/* Verify the existence of the function.  */
#  define _GL_WARN_ON_USE(function, message) \
extern __typeof__ (function) function
# else /* Unsupported.  */
#  define _GL_WARN_ON_USE(function, message) \
_GL_WARN_EXTERN_C int _gl_warn_on_use
# endif
#endif

/* _GL_WARN_ON_USE_CXX (function, rettype, parameters_and_attributes, "string")
   is like _GL_WARN_ON_USE (function, "string"), except that the function is
   declared with the given prototype, consisting of return type, parameters,
   and attributes.
   This variant is useful for overloaded functions in C++. _GL_WARN_ON_USE does
   not work in this case.  */
#ifndef _GL_WARN_ON_USE_CXX
# if 4 < __GNUC__ || (__GNUC__ == 4 && 3 <= __GNUC_MINOR__)
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
extern rettype function parameters_and_attributes \
     __attribute__ ((__warning__ (msg)))
# elif __GNUC__ >= 3 && GNULIB_STRICT_CHECKING
/* Verify the existence of the function.  */
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
extern rettype function parameters_and_attributes
# else /* Unsupported.  */
#  define _GL_WARN_ON_USE_CXX(function,rettype,parameters_and_attributes,msg) \
_GL_WARN_EXTERN_C int _gl_warn_on_use
# endif
#endif


/* On AIX, sig_atomic_t already includes volatile.  C99 requires that
   'volatile sig_atomic_t' ignore the extra modifier, but C89 did not.
   Hence, redefine this to a non-volatile type as needed.  */
#if ! 1
# if !GNULIB_defined_sig_atomic_t
typedef int rpl_sig_atomic_t;
#  undef sig_atomic_t
#  define sig_atomic_t rpl_sig_atomic_t
#  define GNULIB_defined_sig_atomic_t 1
# endif
#endif

/* A set or mask of signals.  */
#if !1
# if !GNULIB_defined_sigset_t
typedef unsigned int sigset_t;
#  define GNULIB_defined_sigset_t 1
# endif
#endif

/* Define sighandler_t, the type of signal handlers.  A GNU extension.  */
#if !1
# ifdef __cplusplus
extern "C" {
# endif
# if !GNULIB_defined_sighandler_t
typedef void (*sighandler_t) (int);
#  define GNULIB_defined_sighandler_t 1
# endif
# ifdef __cplusplus
}
# endif
#endif


#if 1
# ifndef SIGPIPE
/* Define SIGPIPE to a value that does not overlap with other signals.  */
#  define SIGPIPE 13
#  define GNULIB_defined_SIGPIPE 1
/* To actually use SIGPIPE, you also need the gnulib modules 'sigprocmask',
   'write', 'stdio'.  */
# endif
#endif


/* Maximum signal number + 1.  */
#ifndef NSIG
# if defined __TANDEM
#  define NSIG 32
# endif
#endif


#if 0
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   undef pthread_sigmask
#   define pthread_sigmask rpl_pthread_sigmask
#  endif
_GL_FUNCDECL_RPL (pthread_sigmask, int,
                  (int how, const sigset_t *new_mask, sigset_t *old_mask));
_GL_CXXALIAS_RPL (pthread_sigmask, int,
                  (int how, const sigset_t *new_mask, sigset_t *old_mask));
#else
# if !1
_GL_FUNCDECL_SYS (pthread_sigmask, int,
                  (int how, const sigset_t *new_mask, sigset_t *old_mask));
# endif
_GL_CXXALIAS_SYS (pthread_sigmask, int,
                  (int how, const sigset_t *new_mask, sigset_t *old_mask));
#endif

#if 0
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   undef raise
#   define raise rpl_raise
#  endif
_GL_FUNCDECL_RPL (raise, int, (int sig));
_GL_CXXALIAS_RPL (raise, int, (int sig));
#else
#  if !1
_GL_FUNCDECL_SYS (raise, int, (int sig));
#  endif
_GL_CXXALIAS_SYS (raise, int, (int sig));
#endif //  # if 0


# if !1

#  ifndef GNULIB_defined_signal_blocking
#   define GNULIB_defined_signal_blocking 1
#  endif

/* Maximum signal number + 1.  */
#  ifndef NSIG
#   define NSIG 32
#  endif

/* This code supports only 32 signals.  */
#  if !GNULIB_defined_verify_NSIG_constraint
typedef int verify_NSIG_constraint[NSIG <= 32 ? 1 : -1];
#   define GNULIB_defined_verify_NSIG_constraint 1
#  endif

# endif


/* Install the handler FUNC for signal SIG, and return the previous
   handler.  */
# if !GNULIB_defined_function_taking_int_returning_void_t
typedef void (*_gl_function_taking_int_returning_void_t) (int);
#  define GNULIB_defined_function_taking_int_returning_void_t 1
# endif

# if !1
#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
#   define signal rpl_signal
#  endif
_GL_FUNCDECL_RPL (signal, _gl_function_taking_int_returning_void_t,
                  (int sig, _gl_function_taking_int_returning_void_t func));
_GL_CXXALIAS_RPL (signal, _gl_function_taking_int_returning_void_t,
                  (int sig, _gl_function_taking_int_returning_void_t func));
# else
_GL_CXXALIAS_SYS (signal, _gl_function_taking_int_returning_void_t,
                  (int sig, _gl_function_taking_int_returning_void_t func));
# endif

# if !1 && GNULIB_defined_SIGPIPE
/* Raise signal SIGPIPE.  */
NLC_EXTERN_C int _gl_raise_SIGPIPE (void);
# endif


//# if !GNULIB_defined_siginfo_types
#ifdef TARGET_OS_WINDOWS

/* Present to allow compilation, but unsupported by gnulib.  */
union sigval
{
  int sival_int;
  void *sival_ptr;
};

/* Present to allow compilation, but unsupported by gnulib.  */
struct siginfo_t
{
  int si_signo;
  int si_code;
  int si_errno;
  pid_t si_pid;
  uid_t si_uid;
  void *si_addr;
  int si_status;
  long si_band;
  union sigval si_value;
};
typedef struct siginfo_t siginfo_t;

int sigismember( const sigset_t *set, int sig );
int sigemptyset( sigset_t *set );
int sigaddset( sigset_t *set, int sig );
int sigdelset( sigset_t *set, int sig );
int sigfillset( sigset_t *set );
int sigpending( sigset_t *set );
int sigprocmask( int operation, const sigset_t *set, sigset_t *old_set );



/* We assume that platforms which lack the sigaction() function also lack
   the 'struct sigaction' type, and vice versa.  */


struct sigaction
{
  union
  {
    void (*_sa_handler) (int);
    /* Present to allow compilation, but unsupported by gnulib.  POSIX
       says that implementations may, but not must, make sa_sigaction
       overlap with sa_handler, but we know of no implementation where
       they do not overlap.  */
    void (*_sa_sigaction) (int, siginfo_t *, void *);
  } _sa_func;
  sigset_t sa_mask;
  /* Not all POSIX flags are supported.  */
  int sa_flags;
};

#  define sa_handler _sa_func._sa_handler
#  define sa_sigaction _sa_func._sa_sigaction
/* Unsupported flags are not present.  */
#  define SA_RESETHAND 1
#  define SA_NODEFER 2
#  define SA_RESTART 4

#  define GNULIB_defined_struct_sigaction 1
extern int gnu_sigaction( int, const struct sigaction*, struct sigaction* );

#endif // TARGET_OS_WINDOWS

//extern int gnu_sigaction( int, const struct sigaction *sigAction );
NLC_END_CDECLARES

//# define sa_sigaction sa_handler


//#elif defined GNULIB_POSIXCHECK
//# undef sigaction
//# if HAVE_RAW_DECL_SIGACTION
//_GL_WARN_ON_USE (sigaction, "sigaction is unportable - "
//                 "use the gnulib module sigaction for portability");

//# endif // GNULIB_defined_struct_sigaction


/* Some systems don't have SA_NODEFER.  */
#ifndef SA_NODEFER
# define SA_NODEFER 0
#endif


