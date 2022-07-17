/*****************************************************************************
 * gotvptop_charset.h: Unicode UTF-8 wrappers function
 *****************************************************************************
 * Copyright (C) 2003-2005 GOTV authors and VideoLAN
 * Copyright © 2005-2010 Rémi Denis-Courmont
 * $Id: 05092257a4a1f09451dc1e956d07b02093908dd4 $
 *
 * Author: Rémi Denis-Courmont <rem # videolan,org>
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

#ifndef GOTV_CHARSET_H
#define GOTV_CHARSET_H 1

/**
 * \file
 * Characters sets handling
 *
 * \ingroup strings
 * @{
 */

/**
 * Decodes a code point from UTF-8.
 *
 * Converts the first character in a UTF-8 sequence into a Unicode code point.
 *
 * \param str an UTF-8 bytes sequence [IN]
 * \param pwc address of a location to store the code point [OUT]
 *
 * \return the number of bytes occupied by the decoded code point
 *
 * \retval (size_t)-1 not a valid UTF-8 sequence
 * \retval 0 null character (i.e. str points to an empty string)
 * \retval 1 (non-null) ASCII character
 * \retval 2-4 non-ASCII character
 */
NLC_API size_t gotvptop_towc(const char *str, uint32_t *restrict pwc);

/**
 * Checks UTF-8 validity.
 *
 * Checks whether a null-terminated string is a valid UTF-8 bytes sequence.
 *
 * \param str string to check
 *
 * \retval str the string is a valid null-terminated UTF-8 sequence
 * \retval NULL the string is not an UTF-8 sequence
 */
GOTV_USED static inline const char *IsUTF8(const char *str)
{
    size_t n;
    uint32_t cp;

    while ((n = gotvptop_towc(str, &cp)) != 0)
        if (likely(n != (size_t)-1))
            str += n;
        else
            return NULL;
    return str;
}

/**
 * Removes non-UTF-8 sequences.
 *
 * Replaces invalid or <i>over-long</i> UTF-8 bytes sequences within a
 * null-terminated string with question marks. This is so that the string can
 * be printed at least partially.
 *
 * \warning Do not use this were correctness is critical. use IsUTF8() and
 * handle the error case instead. This function is mainly for display or debug.
 *
 * \note Converting from Latin-1 to UTF-8 in place is not possible (the string
 * size would be increased). So it is not attempted even if it would otherwise
 * be less disruptive.
 *
 * \retval str the string is a valid null-terminated UTF-8 sequence
 *             (i.e. no changes were made)
 * \retval NULL the string is not an UTF-8 sequence
 */
static inline char *EnsureUTF8(char *str)
{
    char *ret = str;
    size_t n;
    uint32_t cp;

    while ((n = gotvptop_towc(str, &cp)) != 0)
        if (likely(n != (size_t)-1))
            str += n;
        else
        {
            *str++ = '?';
            ret = NULL;
        }
    return ret;
}

/* iconv wrappers (defined in src/extras/libc.c) */
#define GOTV_ICONV_ERR ((size_t) -1)
typedef void *gotvptop_iconv_t;
NLC_API gotvptop_iconv_t gotvptop_iconv_open( const char *, const char * ) GOTV_USED;
NLC_API size_t gotvptop_iconv( gotvptop_iconv_t, const char **, size_t *, char **, size_t * ) GOTV_USED;
NLC_API int gotvptop_iconv_close( gotvptop_iconv_t );

#include <stdarg.h>

NLC_API int utf8_vfprintf( FILE *stream, const char *fmt, va_list ap );
NLC_API int utf8_fprintf( FILE *, const char *, ... ) GOTV_FORMAT( 2, 3 );
NLC_API char * gotvptop_strcasestr(const char *, const char *) GOTV_USED;

NLC_API char * FromCharset( const char *charset, const void *data, size_t data_size ) GOTV_USED;
NLC_API void * ToCharset( const char *charset, const char *in, size_t *outsize ) GOTV_USED;

#ifdef _WIN32
GOTV_USED
static inline char *FromWide (const wchar_t *wide)
{
    size_t len = WideCharToMultiByte (CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return NULL;

    char *out = (char *)malloc (len);

    if (likely(out))
        WideCharToMultiByte (CP_UTF8, 0, wide, -1, out, len, NULL, NULL);
    return out;
}

GOTV_USED
static inline wchar_t *ToWide (const char *utf8)
{
    int len = MultiByteToWideChar (CP_UTF8, 0, utf8, -1, NULL, 0);
    if (len == 0)
        return NULL;

    wchar_t *out = (wchar_t *)malloc (len * sizeof (wchar_t));

    if (likely(out))
        MultiByteToWideChar (CP_UTF8, 0, utf8, -1, out, len);
    return out;
}

GOTV_USED GOTV_MALLOC
static inline char *ToCodePage (unsigned cp, const char *utf8)
{
    wchar_t *wide = ToWide (utf8);
    if (wide == NULL)
        return NULL;

    size_t len = WideCharToMultiByte (cp, 0, wide, -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        free(wide);
        return NULL;
    }

    char *out = (char *)malloc (len);
    if (likely(out != NULL))
        WideCharToMultiByte (cp, 0, wide, -1, out, len, NULL, NULL);
    free (wide);
    return out;
}

GOTV_USED GOTV_MALLOC
static inline char *FromCodePage (unsigned cp, const char *mb)
{
    int len = MultiByteToWideChar (cp, 0, mb, -1, NULL, 0);
    if (len == 0)
        return NULL;

    wchar_t *wide = (wchar_t *)malloc (len * sizeof (wchar_t));
    if (unlikely(wide == NULL))
        return NULL;
    MultiByteToWideChar (cp, 0, mb, -1, wide, len);

    char *utf8 = FromWide (wide);
    free (wide);
    return utf8;
}

GOTV_USED GOTV_MALLOC
static inline char *FromANSI (const char *ansi)
{
    return FromCodePage (GetACP (), ansi);
}

GOTV_USED GOTV_MALLOC
static inline char *ToANSI (const char *utf8)
{
    return ToCodePage (GetACP (), utf8);
}

# ifdef UNICODE
#  define FromT FromWide
#  define ToT   ToWide
# else
#  define FromT FromANSI
#  define ToT   ToANSI
# endif
# define FromLocale    FromANSI
# define ToLocale      ToANSI
# define LocaleFree(s) free((char *)(s))
# define FromLocaleDup FromANSI
# define ToLocaleDup   ToANSI

#elif defined(__OS2__)

GOTV_USED static inline char *FromLocale (const char *locale)
{
    return locale ? FromCharset ((char *)"", locale, strlen(locale)) : NULL;
}

GOTV_USED static inline char *ToLocale (const char *utf8)
{
    size_t outsize;
    return utf8 ? (char *)ToCharset ("", utf8, &outsize) : NULL;
}

GOTV_USED static inline void LocaleFree (const char *str)
{
    free ((char *)str);
}

GOTV_USED static inline char *FromLocaleDup (const char *locale)
{
    return FromCharset ("", locale, strlen(locale));
}

GOTV_USED static inline char *ToLocaleDup (const char *utf8)
{
    size_t outsize;
    return (char *)ToCharset ("", utf8, &outsize);
}

#else

# define FromLocale(l) (l)
# define ToLocale(u)   (u)
# define LocaleFree(s) ((void)(s))
# define FromLocaleDup strdup
# define ToLocaleDup   strdup
#endif

/**
 * Converts a nul-terminated string from ISO-8859-1 to UTF-8.
 */
static inline char *FromLatin1 (const char *latin)
{
    char *str = (char *)malloc (2 * strlen (latin) + 1), *utf8 = str;
    unsigned char c;

    if (str == NULL)
        return NULL;

    while ((c = *(latin++)) != '\0')
    {
         if (c >= 0x80)
         {
             *(utf8++) = 0xC0 | (c >> 6);
             *(utf8++) = 0x80 | (c & 0x3F);
         }
         else
             *(utf8++) = c;
    }
    *(utf8++) = '\0';

    utf8 = (char *)realloc (str, utf8 - str);
    return utf8 ? utf8 : str;
}

/** @} */

NLC_API double us_strtod( const char *, char ** ) GOTV_USED;
NLC_API float us_strtof( const char *, char ** ) GOTV_USED;
NLC_API double us_atof( const char * ) GOTV_USED;
NLC_API int us_vasprintf( char **, const char *, va_list );
NLC_API int us_asprintf( char **, const char *, ... ) GOTV_USED;

#endif
