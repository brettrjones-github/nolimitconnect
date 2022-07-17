/*****************************************************************************
 * gotvptop_memstream.h:
 *****************************************************************************
 * Copyright (C) 2016 Rémi Denis-Courmont
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

#ifndef GOTV_MEMSTREAM_H
# define GOTV_MEMSTREAM_H 1

# include <stdarg.h>
# include <stdio.h>

struct gotvptop_memstream
{
    union
    {
        FILE *stream;
        int error;
    };
    char *ptr;
    size_t length;
};

NLC_API
int gotvptop_memstream_open(struct gotvptop_memstream *ms);

NLC_API
int gotvptop_memstream_flush(struct gotvptop_memstream *ms) GOTV_USED;

NLC_API
int gotvptop_memstream_close(struct gotvptop_memstream *ms) GOTV_USED;

NLC_API
size_t gotvptop_memstream_write(struct gotvptop_memstream *ms,
                           const void *ptr, size_t len);

NLC_API
int gotvptop_memstream_putc(struct gotvptop_memstream *ms, int c);

NLC_API
int gotvptop_memstream_puts(struct gotvptop_memstream *ms, const char *str);

NLC_API
int gotvptop_memstream_vprintf(struct gotvptop_memstream *ms, const char *fmt,
                          va_list args);

NLC_API
int gotvptop_memstream_printf(struct gotvptop_memstream *s, const char *fmt,
                         ...) GOTV_FORMAT(2,3);

# ifdef __GNUC__
static inline int gotvptop_memstream_puts_len(struct gotvptop_memstream *ms,
                                         const char *str, size_t len)
{
    return (gotvptop_memstream_write(ms, str, len) == len) ? (int)len : EOF;
}
#  define gotvptop_memstream_puts(ms,s) \
    (__builtin_constant_p(__builtin_strlen(s)) ? \
        gotvptop_memstream_puts_len(ms,s,__builtin_strlen(s)) : \
        gotvptop_memstream_puts(ms,s))
# endif
#endif /* GOTV_MEMSTREAM_H */
