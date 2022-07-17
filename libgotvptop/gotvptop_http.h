/*****************************************************************************
 * gotvptop_http.h: Shared code for HTTP clients
 *****************************************************************************
 * Copyright (C) 2001-2008 GOTV authors and VideoLAN
 * $Id: ed7e1613da0bc45b680f3887846f80cf118f36a4 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Christophe Massiot <massiot@via.ecp.fr>
 *          Rémi Denis-Courmont <rem # videolan.org>
 *          Antoine Cellerier <dionoea at videolan dot org>
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

#ifndef GOTV_HTTP_H
#define GOTV_HTTP_H 1

/**
 * \file
 * This file defines functions, structures, enums and macros shared between
 * HTTP clients.
 */

#include <gotvptop_url.h>
#include <gotvptop_arrays.h>

/* RFC 2617: Basic and Digest Access Authentication */
typedef struct gotvptop_http_auth_t
{
    char *psz_realm;
    char *psz_domain;
    char *psz_nonce;
    char *psz_opaque;
    char *psz_stale;
    char *psz_algorithm;
    char *psz_qop;
    int i_nonce;
    char *psz_cnonce;
    char *psz_HA1; /* stored H(A1) value if algorithm = "MD5-sess" */
} gotvptop_http_auth_t;


NLC_API void gotvptop_http_auth_Init( gotvptop_http_auth_t * );
NLC_API void gotvptop_http_auth_Deinit( gotvptop_http_auth_t * );
NLC_API void gotvptop_http_auth_ParseWwwAuthenticateHeader
            ( gotvptop_object_t *, gotvptop_http_auth_t * , const char * );
NLC_API int gotvptop_http_auth_ParseAuthenticationInfoHeader
            ( gotvptop_object_t *, gotvptop_http_auth_t *,
              const char *, const char *,
              const char *, const char *,
              const char * );
NLC_API char *gotvptop_http_auth_FormatAuthorizationHeader
            ( gotvptop_object_t *, gotvptop_http_auth_t *,
              const char *, const char *,
              const char *, const char * ) GOTV_USED;

/* RFC 6265: cookies */

typedef struct gotvptop_http_cookie_jar_t gotvptop_http_cookie_jar_t;

NLC_API gotvptop_http_cookie_jar_t * gotvptop_http_cookies_new( void ) GOTV_USED;
NLC_API void gotvptop_http_cookies_destroy( gotvptop_http_cookie_jar_t * p_jar );

/**
 * Parse a value of an incoming Set-Cookie header and append the
 * cookie to the cookie jar if appropriate.
 *
 * @param jar cookie jar object
 * @param cookie header field value of Set-Cookie
 * @return true, if the cookie was added, false otherwise
 */
NLC_API bool gotvptop_http_cookies_store( gotvptop_http_cookie_jar_t *jar,
    const char *cookie, const char *host, const char *path );

/**
 * Returns a cookie value that match the given URL.
 *
 * @param p_jar a cookie jar
 * @param p_url the URL for which the cookies are returned
 * @return A string consisting of semicolon-separated cookie NAME=VALUE pairs.
 */
NLC_API char *gotvptop_http_cookies_fetch( gotvptop_http_cookie_jar_t *jar, bool secure,
                                      const char *host, const char *path );

#endif /* GOTV_HTTP_H */
