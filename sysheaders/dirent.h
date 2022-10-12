/*
 * dirent.h - dirent API for Microsoft Visual Studio
 *
 * Copyright (C) 2006-2012 Toni Ronkko
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * ``Software''), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL TONI RONKKO BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * Version 1.13, Dec 12 2012, Toni Ronkko
 * Use traditional 8+3 file name if the name cannot be represented in the
 * default ANSI code page.  Now compiles again with MSVC 6.0.  Thanks to
 * Konstantin Khomoutov for testing.
 *
 * Version 1.12.1, Oct 1 2012, Toni Ronkko
 * Bug fix: renamed wide-character DIR structure _wDIR to _WDIR (with
 * capital W) in order to maintain compatibility with MingW.
 *
 * Version 1.12, Sep 30 2012, Toni Ronkko
 * Define PATH_MAX and NAME_MAX.  Added wide-character variants _wDIR, 
 * _wdirent, _wopendir(), _wreaddir(), _wclosedir() and _wrewinddir().
 * Thanks to Edgar Buerkle and Jan Nijtmans for ideas and code.
 *
 * Do not include windows.h.  This allows dirent.h to be integrated more
 * easily into programs using winsock.  Thanks to Fernando Azaldegui.
 *
 * Version 1.11, Mar 15, 2011, Toni Ronkko
 * Defined FILE_ATTRIBUTE_DEVICE for MSVC 6.0.
 *
 * Version 1.10, Aug 11, 2010, Toni Ronkko
 * Added d_type and d_namlen fields to dirent structure.  The former is
 * especially useful for determining whether directory entry represents a
 * file or a directory.  For more information, see
 * http://www.delorie.com/gnu/docs/glibc/libc_270.html
 *
 * Improved conformance to the standards.  For example, errno is now set
 * properly on failure and assert() is never used.  Thanks to Peter Brockam
 * for suggestions.
 *
 * Fixed a bug in rewinddir(): when using relative directory names, change
 * of working directory no longer causes rewinddir() to fail.
 *
 * Version 1.9, Dec 15, 2009, John Cunningham
 * Added rewinddir member function
 *
 * Version 1.8, Jan 18, 2008, Toni Ronkko
 * Using FindFirstFileA and WIN32_FIND_DATAA to avoid converting string
 * between multi-byte and unicode representations.  This makes the
 * code simpler and also allows the code to be compiled under MingW.  Thanks
 * to Azriel Fasten for the suggestion.
 *
 * Mar 4, 2007, Toni Ronkko
 * Bug fix: due to the strncpy_s() function this file only compiled in
 * Visual Studio 2005.  Using the new string functions only when the
 * compiler version allows.
 *
 * Nov  2, 2006, Toni Ronkko
 * Major update: removed support for Watcom C, MS-DOS and Turbo C to
 * simplify the file, updated the code to compile cleanly on Visual
 * Studio 2005 with both unicode and multi-byte character strings,
 * removed rewinddir() as it had a bug.
 *
 * Aug 20, 2006, Toni Ronkko
 * Removed all remarks about MSVC 1.0, which is antiqued now.  Simplified
 * comments by removing SGML tags.
 *
 * May 14 2002, Toni Ronkko
 * Embedded the function definitions directly to the header so that no
 * source modules need to be included in the Visual Studio project.  Removed
 * all the dependencies to other projects so that this header file can be
 * used independently.
 *
 * May 28 1998, Toni Ronkko
 * First version.
 *****************************************************************************/

#include <NlcDependLibrariesConfig.h>

#ifdef _MSC_VER
# include <CoreLib/dirent_sim.h>
#else
# include_next <dirent.h>
#endif // _MSC_VER

