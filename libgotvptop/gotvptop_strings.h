/*****************************************************************************
 * gotvptop_strings.h: String functions
 *****************************************************************************
 * Copyright (C) 2006 GOTV authors and VideoLAN
 * $Id: 9828fbd728ee791d409568405f280634132b91a6 $
 *
 * Authors: Antoine Cellerier <dionoea at videolan dot org>
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

#ifndef GOTV_STRINGS_H
#define GOTV_STRINGS_H 1

/**
 * \defgroup strings String helpers
 * @{
 * \file
 * Helper functions for nul-terminated strings
 */

static inline int gotvptop_ascii_toupper( int c )
{
    if ( c >= 'a' && c <= 'z' )
        return c + ( 'A' - 'a' );
    else
        return c;
}

static inline int gotvptop_ascii_tolower( int c )
{
    if ( c >= 'A' && c <= 'Z' )
        return c + ( 'a' - 'A' );
    else
        return c;
}

/**
 * Compare two ASCII strings ignoring case.
 *
 * The result is independent of the locale. If there are non-ASCII
 * characters in the strings, their cases are NOT ignored in the
 * comparison.
 */
static inline int gotvptop_ascii_strcasecmp( const char *psz1, const char *psz2 )
{
    const char *s1 = psz1;
    const char *s2 = psz2;
    int d = gotvptop_ascii_tolower( *s1 ) - gotvptop_ascii_tolower( *s2 );
    while ( *s1 && d == 0)
    {
        s1++;
        s2++;
        d = gotvptop_ascii_tolower( *s1 ) - gotvptop_ascii_tolower( *s2 );
    }

    return d;
}

static inline int gotvptop_ascii_strncasecmp( const char *psz1, const char *psz2, size_t n )
{
    const char *s1 = psz1;
    const char *s2 = psz2;
    const char *s1end = psz1 + n;
    int d = gotvptop_ascii_tolower( *s1 ) - gotvptop_ascii_tolower( *s2 );
    while ( *s1 && s1 < s1end && d == 0)
    {
        s1++;
        s2++;
        d = gotvptop_ascii_tolower( *s1 ) - gotvptop_ascii_tolower( *s2 );
    }

    if (s1 == s1end)
        return 0;
    else
        return d;
}

/**
 * Decodes XML entities.
 *
 * Decodes a null-terminated UTF-8 string of XML character data into a regular
 * nul-terminated UTF-8 string. In other words, replaces XML entities and
 * numerical character references with the corresponding characters.
 *
 * This function operates in place (the output is always of smaller or equal
 * length than the input) and always succeeds.
 *
 * \param str null-terminated string [IN/OUT]
 */
NLC_API void gotvptop_xml_decode(char *st);

/**
 * Encodes XML entites.
 *
 * Substitutes unsafe characters in a null-terminated UTF-8 strings with an
 * XML entity or numerical character reference.
 *
 * \param str null terminated UTF-8 string
 * \return On success, a heap-allocated null-terminated string is returned.
 * If the input string was not a valid UTF-8 sequence, NULL is returned and
 * errno is set to EILSEQ.
 * If there was not enough memory, NULL is returned and errno is to ENOMEM.
 */
NLC_API char *gotvptop_xml_encode(const char *str) GOTV_MALLOC;

NLC_API char * gotvptop_b64_encode_binary( const uint8_t *, size_t );
NLC_API char * gotvptop_b64_encode( const char * );

NLC_API size_t gotvptop_b64_decode_binary_to_buffer( uint8_t *p_dst, size_t i_dst_max, const char *psz_src );
NLC_API size_t gotvptop_b64_decode_binary( uint8_t **pp_dst, const char *psz_src );
NLC_API char * gotvptop_b64_decode( const char *psz_src );

/**
 * Convenience wrapper for strftime().
 *
 * Formats the current time into a heap-allocated string.
 *
 * @param tformat time format (as with C strftime())
 * @return an allocated string (must be free()'d), or NULL on memory error.
 */
NLC_API char *gotvptop_strftime( const char * );

/**
 * Formats input meta-data.
 *
 * Formats input and input item meta-informations into a heap-allocated string.
 */
NLC_API char *gotvptop_strfinput( input_thread_t *, const char * );

static inline char *str_format( input_thread_t *input, const char *fmt )
{
    char *s1 = gotvptop_strftime( fmt );
    char *s2 = gotvptop_strfinput( input, s1 );
    free( s1 );
    return s2;
}

NLC_API int gotvptop_filenamecmp(const char *, const char *);

void filename_sanitize(char *);

/**
 * @}
 */

#endif
