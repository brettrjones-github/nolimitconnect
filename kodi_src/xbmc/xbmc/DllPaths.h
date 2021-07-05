/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#ifdef TARGET_WINDOWS
/* libraries */
# define DLL_PATH_CPLUFF        "special://xbmcbin/cpluff.dll"
# define DLL_PATH_LIBCURL       "special://xbmcbin/libcurl.dll"
# define DLL_PATH_LIBNFS        "special://xbmcbin/libnfs.dll"
# define DLL_PATH_LIBPLIST      "special://xbmcbin/libplist.dll"
# define DLL_PATH_LIBSHAIRPLAY  "special://xbmcbin/shairplay.dll"
# define DLL_PATH_LIBCEC        "special://xbmcbin/cec.dll"

/* VideoPlayer */
# define DLL_PATH_LIBASS        "special://xbmcbin/libass.dll"
# define DLL_PATH_LIBDVDNAV     "special://xbmcbin/libdvdnav.dll"

/* libbluray */
# define DLL_PATH_LIBBLURAY     "special://xbmcbin/libbluray.dll"

#else
# define PREFIX_USR_PATH        "/usr/"
# define DLL_PATH_LIBUI         "libui.so"

/* libraries */
# define DLL_PATH_CPLUFF        "special://xbmcbin/cpluff.so"
# define DLL_PATH_LIBCURL       "special://xbmcbin/libcurl.so"
# define DLL_PATH_LIBNFS        "special://xbmcbin/libnfs.so"
# define DLL_PATH_LIBPLIST      "special://xbmcbin/libplist.so"
# define DLL_PATH_LIBSHAIRPLAY  "special://xbmcbin/shairplay.so"
# define DLL_PATH_LIBCEC        "special://xbmcbin/cec.so"

/* VideoPlayer */
# define DLL_PATH_LIBASS        "special://xbmcbin/libass.so"
# define DLL_PATH_LIBDVDNAV     "special://xbmcbin/libdvdnav.so"

/* libbluray */
# define DLL_PATH_LIBBLURAY     "special://xbmcbin/libbluray.so"
#endif

