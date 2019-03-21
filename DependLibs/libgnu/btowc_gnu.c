/* Convert unibyte character to wide character.
   Copyright (C) 2008, 2010-2016 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2008.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config_libgnu.h>
#ifndef _MSC_VER

/* Specification.  */
#include <wchar_gnu.h>

#if !HAVE_WINT_T && !defined(wint_t)
# define wint_t int
# ifndef WEOF
#  define WEOF -1
# endif
#endif // !HAVE_WINT_T && !defined(wint_t)

#include <stdio.h>
#include <stdlib.h>

wint_t btowc (int c)
{
  if (c != EOF)
    {
      char buf[1];
      wchar_t wc;

      buf[0] = c;
      if (mbtowc (&wc, buf, 1) >= 0)
        return wc;
    }
  return WEOF;
}

#endif // _MSC_VER