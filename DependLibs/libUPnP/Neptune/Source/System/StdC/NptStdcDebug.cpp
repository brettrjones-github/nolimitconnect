/*****************************************************************
|
|      File: NptStdcDebug.c
|
|      Neptune - Debug Support: StdC Implementation
|
|      (c) 2002-2009 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <libUPnP/config_libupnp.h>
#ifndef TARGET_OS_WINDOWS
#include <stdarg.h>
#include <stdio.h>

#include "NptConfig.h"
#include "NptDefs.h"
#include "NptTypes.h"
#include "NptDebug.h"

/*----------------------------------------------------------------------
|       NPT_DebugOuput
+---------------------------------------------------------------------*/
void
NPT_DebugOutput(const char* message)
{
    printf("%s", message);
}

#endif // TARGET_OS_WINDOWS
