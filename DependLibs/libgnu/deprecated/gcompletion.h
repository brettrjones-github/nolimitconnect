/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#if !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

#ifndef __G_COMPLETION_H__
#define __G_COMPLETION_H__

#include <libgnu/glist.h>

GOTV_BEGIN_CDECLARES

typedef struct _GCompletion     GCompletion;

typedef gchar*          (*GCompletionFunc)      (gpointer);

/* GCompletion
 */

typedef gint (*GCompletionStrncmpFunc) (const gchar *s1,
                                        const gchar *s2,
                                        gsize        n);

struct _GCompletion
{
  GList* items;
  GCompletionFunc func;
 
  gchar* prefix;
  GList* cache;
  GCompletionStrncmpFunc strncmp_func;
};

GLIB_DEPRECATED_IN_2_26
GCompletion* g_completion_new           (GCompletionFunc func);
GLIB_DEPRECATED_IN_2_26
void         g_completion_add_items     (GCompletion*    cmp,
                                         GList*          items);
GLIB_DEPRECATED_IN_2_26
void         g_completion_remove_items  (GCompletion*    cmp,
                                         GList*          items);
GLIB_DEPRECATED_IN_2_26
void         g_completion_clear_items   (GCompletion*    cmp);
GLIB_DEPRECATED_IN_2_26
GList*       g_completion_complete      (GCompletion*    cmp,
                                         const gchar*    prefix,
                                         gchar**         new_prefix);
GLIB_DEPRECATED_IN_2_26
GList*       g_completion_complete_utf8 (GCompletion  *cmp,
                                         const gchar*    prefix,
                                         gchar**         new_prefix);
GLIB_DEPRECATED_IN_2_26
void         g_completion_set_compare   (GCompletion *cmp,
                                         GCompletionStrncmpFunc strncmp_func);
GLIB_DEPRECATED_IN_2_26
void         g_completion_free          (GCompletion*    cmp);

GOTV_END_CDECLARES

#endif /* __G_COMPLETION_H__ */
