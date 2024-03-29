/* Close standard output and standard error.
   Copyright (C) 1998-2003, 2006, 2015-2016 Free Software Foundation, Inc.

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

#pragma once
#include <libgnu/config_libgnu.h>

#include <stdlib_gnu.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void close_stdout (void);

#ifdef __cplusplus
}
#endif
