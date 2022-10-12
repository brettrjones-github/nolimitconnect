#pragma once
#include <NlcDependLibrariesConfig.h>

/* we need this since some compilers (like MSVC) leave assert()s on release code (and we don't want to use their ASSERT) */
#ifdef DEBUG
# include <assert.h>
# define FLAC__ASSERT(x) assert(x)
# define FLAC__ASSERT_DECLARATION(x) x
#else
# define FLAC__ASSERT(x)
# define FLAC__ASSERT_DECLARATION(x)
#endif
