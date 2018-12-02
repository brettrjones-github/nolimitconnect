/*****************************************************************************
 * gotvptop_url.h: URL related macros
 *****************************************************************************
 * Copyright (C) 2002-2006 GOTV authors and VideoLAN
 * $Id: e13b7a5abb57e777e5252c5287bed9f52c7e2de0 $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Rémi Denis-Courmont <rem # videolan.org>
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

#ifndef GOTV_URL_H
# define GOTV_URL_H

/**
 * \file
 * This file defines functions for manipulating URL in gotvptop
 *
 * \ingroup strings
 * @{
 */

/**
 * Converts local path to URL.
 *
 * Builds a URL representation from a local UTF-8 null-terminated file path.
 *
 * @param path file path
 * @param scheme URI scheme to use (default is auto: "file", "fd" or "smb")
 * @return a heap-allocated URI string on success
 * or NULL in case of error (errno will be set accordingly)
 */
GOTV_API char *gotvptop_path2uri(const char *path, const char *scheme) GOTV_MALLOC;

/**
 * Converts a URI to a local path.
 *
 * Builds a local path (UTF-8-encoded null-terminated string) from a URI if
 * the URI scheme allows.
 *
 * @param url URI
 * @return a heap-allocated string or success
 * or NULL on error
 */
GOTV_API char *gotvptop_uri2path(const char *url) GOTV_MALLOC;

/**
 * Decodes an URI component in place.
 *
 * Decodes one null-terminated UTF-8 URI component to aa null-terminated UTF-8
 * string in place.
 *
 * See also gotvptop_uri_decode_duplicate() for the not-in-place variant.
 *
 * \warning <b>This function does NOT decode entire URIs.</b>
 * URI can only be decoded (and encoded) one component at a time
 * (e.g. the host name, one directory, the file name).
 * Complete URIs are always "encoded" (or they are syntaxically invalid).
 * See IETF RFC3986, especially §2.4 for details.
 *
 * \note URI encoding is <b>different</b> from Javascript escaping. Especially,
 * white spaces and Unicode non-ASCII code points are encoded differently.
 *
 * \param str null-terminated component
 * \return str is returned on success. NULL if str was not properly encoded.
 */
GOTV_API char *gotvptop_uri_decode(char *str);

/**
 * Decodes an URI component.
 *
 * See also gotvptop_uri_decode() for the in-place variant.
 *
 * \return a heap-allocated string on success or NULL on error.
 */
GOTV_API char *gotvptop_uri_decode_duplicate(const char *str) GOTV_MALLOC;

/**
 * Encodes a URI component.
 *
 * Substitutes URI-unsafe, URI delimiters and non-ASCII characters into their
 * URI-encoded URI-safe representation. See also IETF RFC3986 §2.
 *
 * @param str nul-terminated UTF-8 representation of the component.
 * @note Obviously, a URI containing nul bytes cannot be passed.
 * @return heap-allocated string, or NULL if out of memory.
 */
GOTV_API char *gotvptop_uri_encode(const char *str) GOTV_MALLOC;

/**
 * Composes an URI.
 *
 * Converts a decomposed/parsed URI structure (\ref gotvptop_url_t) into a
 * nul-terminated URI literal string.
 *
 * See also IETF RFC3986 section 5.3 for details.
 *
 * \bug URI fragments (i.e. HTML anchors) are not handled
 *
 * \return a heap-allocated nul-terminated string or NULL if out of memory
 */
GOTV_API char *gotvptop_uri_compose(const gotvptop_url_t *) GOTV_MALLOC;

/**
 * Resolves an URI reference.
 *
 * Resolves an URI reference relative to a base URI.
 * If the reference is an absolute URI, then this function simply returns a
 * copy of the URI reference.
 *
 * \param base base URI (as a nul-terminated string)
 * \param ref URI reference (also as a nul-terminated string)
 *
 * \return a heap-allocated nul-terminated string representing the resolved
 * absolute URI, or NULL if out of memory.
 */
GOTV_API char *gotvptop_uri_resolve(const char *base, const char *ref) GOTV_MALLOC;

/**
 * Fixes up a URI string.
 *
 * Attempts to convert a nul-terminated string into a syntactically valid URI.
 * If the string is, or may be, a syntactically valid URI, an exact copy is
 * returned. In any case, the result will only contain URI-safe and URI
 * delimiter characters (generic delimiters or sub-delimiters) and all percent
 * signs will be followed by two hexadecimal characters.
 *
 * @return a heap-allocated string, or NULL if on out of memory.
 */
GOTV_API char *gotvptop_uri_fixup(const char *) GOTV_MALLOC;

struct gotvptop_url_t
{
    char *psz_protocol;
    char *psz_username;
    char *psz_password;
    char *psz_host;
    unsigned i_port;
    char *psz_path;
    char *psz_option;

    char *psz_buffer; /* to be freed */
    char *psz_pathbuffer; /* to be freed */
};

/**
 * Parses an URI or IRI.
 *
 * Extracts the following parts from an URI string:
 *  - scheme (i.e. protocol),
 *  - user (deprecated),
 *  - password (also deprecated),
 *  - host name or IP address literal,
 *  - port number,
 *  - path (including the filename preceded by any and all directories)
 *  - request parameters (excluding the leading question mark '?').
 *
 * The function accepts URIs, as well as UTF-8-encoded IRIs. For IRIs, the hier
 * part (specifically, the host name) is assumed to be an IDN and is decoded to
 * ASCII according, so it can be used for DNS resolution. If the host is an
 * IPv6 address literal, brackets are stripped.
 *
 * Any missing part is set to nul. For historical reasons, the target structure
 * is always initialized, even if parsing the URI string fails.
 *
 * On error, errno is set to one of the following value:
 *  - ENOMEM in case of memory allocation failure,
 *  - EINVAL in case of syntax error in the input string.
 *
 * \bug The URI fragment is discarded if present.
 *
 * \note This function allocates memory. gotvptop_UrlClean() must be used free
 * associated the allocations, even if the function fails.
 *
 * \param url structure of URL parts [OUT]
 * \param str nul-terminated URL string to split
 * \retval 0 success
 * \retval -1 failure
 */
GOTV_API int gotvptop_UrlParse(gotvptop_url_t *url, const char *str);

/**
 * Parses an URI or IRI and fix up the path part.
 *
 * \see gotvptop_UrlParse
 * \see gotvptop_uri_fixup
 */
GOTV_API int gotvptop_UrlParseFixup(gotvptop_url_t *url, const char *str);

/**
 * Releases resources allocated by gotvptop_UrlParse().
 */
GOTV_API void gotvptop_UrlClean(gotvptop_url_t *);

/** @} */

#endif
