#pragma once
#include <AppDependLibrariesConfig.h>

#ifdef _MSC_VER
# define dlopen(filename, flags)		LoadLibrary((filename))
# define dlsym(handle, symbol)			GetProcAddress(handle, symbol)
# define dlclose(handle)				FreeLibrary(handle)
#else
# include_next <dlfcn.h>
#endif // _MSC_VER
