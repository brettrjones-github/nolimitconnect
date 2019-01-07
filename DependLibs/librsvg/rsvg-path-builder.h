/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sw=4 sts=4 ts=4 expandtab: */
/*
   rsvg-path-builder.h: Hold SVG paths to replay them to a cairo_t later

   Copyright (C) 2000 Eazel, Inc.
   Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>
   Copyright (C) 2016 Federico Mena Quintero <federico@gnome.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Authors: Raph Levien <raph@artofcode.com>
            Federico Mena Quintero <federico@gnome.org>
*/

#ifndef RSVG_PATH_BUILDER_H
#define RSVG_PATH_BUILDER_H

#include <glib.h>
#include <cairo.h>

GOTV_BEGIN_CDECLARES

/* All the following functions and types are implemented in the Rust code.
 */

typedef struct _RsvgPathBuilder RsvgPathBuilder;

G_GNUC_INTERNAL
RsvgPathBuilder *rsvg_path_builder_new (void);

G_GNUC_INTERNAL
void rsvg_path_builder_destroy (RsvgPathBuilder *builder);

G_GNUC_INTERNAL
void rsvg_path_builder_move_to (RsvgPathBuilder *builder,
                                double x,
                                double y);
G_GNUC_INTERNAL
void rsvg_path_builder_line_to (RsvgPathBuilder *builder,
                                double x,
                                double y);
G_GNUC_INTERNAL
void rsvg_path_builder_curve_to (RsvgPathBuilder *builder,
                                 double x1,
                                 double y1,
                                 double x2,
                                 double y2,
                                 double x3,
                                 double y3);

G_GNUC_INTERNAL
void rsvg_path_builder_arc (RsvgPathBuilder *builder,
                            double x1, double y1,
                            double rx, double ry,
                            double x_axis_rotation,
                            gboolean large_arc_flag, gboolean sweep_flag,
                            double x2, double y2);

G_GNUC_INTERNAL
void rsvg_path_builder_close_path (RsvgPathBuilder *builder);

G_GNUC_INTERNAL
void rsvg_path_builder_add_to_cairo_context (RsvgPathBuilder *builder, cairo_t *cr);

G_GNUC_INTERNAL
RsvgPathBuilder *rsvg_path_parser_from_str_into_builder (const char *path_str);

GOTV_END_CDECLARES

#endif /* RSVG_PATH_BUILDER_H */
