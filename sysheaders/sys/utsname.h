#pragma once
#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <io.h>
struct utsname 
{
	char sysname[MAX_PATH];    /* Operating system name (e.g., "Linux") */
	char nodename[ MAX_PATH ];   /* Name within "some implementation-defined
					   network" */
	char release[ MAX_PATH ];    /* Operating system release (e.g., "2.6.28") */
	char version[ MAX_PATH ];    /* Operating system version */
	char machine[ MAX_PATH ];    /* Hardware identifier */
#ifdef _GNU_SOURCE
	char domainname[ MAX_PATH ]; /* NIS or YP domain name */
#endif
};
#else
# include_next <sys/utsname.h>
#endif // _MSC_VER