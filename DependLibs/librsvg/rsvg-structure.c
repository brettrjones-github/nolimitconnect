/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sw=4 sts=4 ts=4 expandtab: */
/*
   rsvg-structure.c: Rsvg's structual elements

   Copyright (C) 2000 Eazel, Inc.
   Copyright (C) 2002 - 2005 Dom Lachowicz <cinamod@hotmail.com>
   Copyright (C) 2003 - 2005 Caleb Moore <c.moore@student.unsw.edu.au>

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

   Authors: Raph Levien <raph@artofcode.com>,
            Dom Lachowicz <cinamod@hotmail.com>,
            Caleb Moore <c.moore@student.unsw.edu.au>
*/

#include "rsvg-structure.h"
#include "rsvg-image.h"
#include "rsvg-css.h"
#include "string.h"

#include <stdio.h>

void
rsvg_node_draw (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
    RsvgState *state;
    GSList *stacksave;

    stacksave = ctx->drawsub_stack;
    if (stacksave) {
        if (stacksave->data != self)
            return;

        ctx->drawsub_stack = stacksave->next;
    }

    state = rsvg_node_get_state (self);

    if (state->visible) {
        rsvg_state_push (ctx);

        self->vtable->draw (self, ctx, dominate);

        rsvg_state_pop (ctx);
    }

    ctx->drawsub_stack = stacksave;
}

static gboolean
draw_child (RsvgNode *node, gpointer data)
{
    RsvgDrawingCtx *ctx;

    ctx = data;

    rsvg_node_draw (node, ctx, 0);

    return TRUE;
}

/* generic function for drawing all of the children of a particular node */
void
_rsvg_node_draw_children (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
    if (dominate != -1) {
        rsvg_state_reinherit_top (ctx, rsvg_node_get_state (self), dominate);

        rsvg_push_discrete_layer (ctx);
    }

    rsvg_node_foreach_child (self, draw_child, ctx);

    if (dominate != -1)
        rsvg_pop_discrete_layer (ctx);
}

/* generic function that doesn't draw anything at all */
static void
_rsvg_node_draw_nothing (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
}

static void
_rsvg_node_dont_set_atts (RsvgNode * node, RsvgHandle * ctx, RsvgPropertyBag * atts)
{
}

void
_rsvg_node_init (RsvgNode * self,
                 RsvgNodeType type,
                 RsvgNodeVtable *vtable)
{
    self->type = type;
    self->parent = NULL;
    self->children = g_ptr_array_new ();
    self->state = rsvg_state_new ();
    self->vtable = g_new (RsvgNodeVtable, 1);

    if (vtable->free) {
        self->vtable->free = vtable->free;
    } else {
        self->vtable->free = _rsvg_node_free;
    }

    if (vtable->draw) {
        self->vtable->draw = vtable->draw;
    } else {
        self->vtable->draw = _rsvg_node_draw_nothing;
    }

    if (vtable->set_atts) {
        self->vtable->set_atts = vtable->set_atts;
    } else {
        self->vtable->set_atts = _rsvg_node_dont_set_atts;
    }
}

void
_rsvg_node_free (RsvgNode * self)
{
    if (self->state != NULL) {
        rsvg_state_free (self->state);
        self->state = NULL;
    }
    if (self->children != NULL) {
        g_ptr_array_free (self->children, TRUE);
        self->children = NULL;
    }

    self->parent = NULL;
    self->type = RSVG_NODE_TYPE_INVALID;

    g_free (self->vtable);
    self->vtable = NULL;

    g_free (self);
}

RsvgNode *
rsvg_new_group (const char *element_name)
{
    RsvgNodeGroup *group;
    RsvgNodeVtable vtable = {
        NULL,
        _rsvg_node_draw_children,
        NULL
    };

    group = g_new (RsvgNodeGroup, 1);
    _rsvg_node_init (&group->super, RSVG_NODE_TYPE_GROUP, &vtable);

    return &group->super;
}

void
rsvg_node_add_child (RsvgNode * self, RsvgNode * child)
{
    g_ptr_array_add (self->children, child);
    child->parent = self;
}

static gboolean
rsvg_node_is_ancestor (RsvgNode * potential_ancestor, RsvgNode * potential_descendant)
{
    /* work our way up the family tree */
    while (TRUE) {
        if (potential_ancestor == potential_descendant)
            return TRUE;
        else if (rsvg_node_get_parent (potential_descendant) == NULL)
            return FALSE;
        else
            potential_descendant = rsvg_node_get_parent (potential_descendant);
    }

    g_assert_not_reached ();
    return FALSE;
}

static void
rsvg_node_use_draw (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
    RsvgNodeUse *use = (RsvgNodeUse *) self;
    RsvgNode *child;
    RsvgState *state;
    cairo_matrix_t affine;
    double x, y, w, h;
    x = rsvg_length_normalize (&use->x, ctx);
    y = rsvg_length_normalize (&use->y, ctx);
    w = rsvg_length_normalize (&use->w, ctx);
    h = rsvg_length_normalize (&use->h, ctx);

    rsvg_state_reinherit_top (ctx, self->state, dominate);

    if (use->link == NULL)
      return;
    child = rsvg_drawing_ctx_acquire_node (ctx, use->link);
    if (!child)
        return;
    else if (rsvg_node_is_ancestor (child, self)) {     /* or, if we're <use>'ing ourself */
        rsvg_drawing_ctx_release_node (ctx, child);
        return;
    }

    state = rsvg_current_state (ctx);
    if (rsvg_node_type (child) != RSVG_NODE_TYPE_SYMBOL) {
        cairo_matrix_init_translate (&affine, x, y);
        cairo_matrix_multiply (&state->affine, &affine, &state->affine);

        rsvg_push_discrete_layer (ctx);
        rsvg_node_draw (child, ctx, 1);
        rsvg_pop_discrete_layer (ctx);
    } else {
        RsvgNodeSymbol *symbol = (RsvgNodeSymbol *) child;

        if (symbol->vbox.active) {
            rsvg_preserve_aspect_ratio
                (symbol->preserve_aspect_ratio,
                 symbol->vbox.rect.width, symbol->vbox.rect.height,
                 &w, &h, &x, &y);

            cairo_matrix_init_translate (&affine, x, y);
            cairo_matrix_multiply (&state->affine, &affine, &state->affine);

            cairo_matrix_init_scale (&affine, w / symbol->vbox.rect.width, h / symbol->vbox.rect.height);
            cairo_matrix_multiply (&state->affine, &affine, &state->affine);

            cairo_matrix_init_translate (&affine, -symbol->vbox.rect.x, -symbol->vbox.rect.y);
            cairo_matrix_multiply (&state->affine, &affine, &state->affine);

            rsvg_drawing_ctx_push_view_box (ctx, symbol->vbox.rect.width, symbol->vbox.rect.height);
            rsvg_push_discrete_layer (ctx);
            if (!state->overflow || (!state->has_overflow && rsvg_node_get_state (child)->overflow))
                rsvg_add_clipping_rect (ctx, symbol->vbox.rect.x, symbol->vbox.rect.y,
                                        symbol->vbox.rect.width, symbol->vbox.rect.height);
        } else {
            cairo_matrix_init_translate (&affine, x, y);
            cairo_matrix_multiply (&state->affine, &affine, &state->affine);
            rsvg_push_discrete_layer (ctx);
        }

        rsvg_state_push (ctx);
        _rsvg_node_draw_children (child, ctx, 1);
        rsvg_state_pop (ctx);
        rsvg_pop_discrete_layer (ctx);
        if (symbol->vbox.active)
            rsvg_drawing_ctx_pop_view_box (ctx);
    }

    rsvg_drawing_ctx_release_node (ctx, child);
}

static void
rsvg_node_svg_draw (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
    RsvgNodeSvg *sself;
    RsvgState *state;
    cairo_matrix_t affine, affine_old, affine_new;
    double nx, ny, nw, nh;
    sself = (RsvgNodeSvg *) self;

    nx = rsvg_length_normalize (&sself->x, ctx);
    ny = rsvg_length_normalize (&sself->y, ctx);
    nw = rsvg_length_normalize (&sself->w, ctx);
    nh = rsvg_length_normalize (&sself->h, ctx);

    rsvg_state_reinherit_top (ctx, rsvg_node_get_state (self), dominate);

    state = rsvg_current_state (ctx);

    affine_old = state->affine;

    if (sself->vbox.active) {
        double x = nx, y = ny, w = nw, h = nh;
        rsvg_preserve_aspect_ratio (sself->preserve_aspect_ratio,
                                    sself->vbox.rect.width, sself->vbox.rect.height,
                                    &w, &h, &x, &y);
        cairo_matrix_init (&affine,
                           w / sself->vbox.rect.width,
                           0,
                           0,
                           h / sself->vbox.rect.height,
                           x - sself->vbox.rect.x * w / sself->vbox.rect.width,
                           y - sself->vbox.rect.y * h / sself->vbox.rect.height);
        cairo_matrix_multiply (&state->affine, &affine, &state->affine);
        rsvg_drawing_ctx_push_view_box (ctx, sself->vbox.rect.width, sself->vbox.rect.height);
    } else {
        cairo_matrix_init_translate (&affine, nx, ny);
        cairo_matrix_multiply (&state->affine, &affine, &state->affine);
        rsvg_drawing_ctx_push_view_box (ctx, nw, nh);
    }

    affine_new = state->affine;

    rsvg_push_discrete_layer (ctx);

    /* Bounding box addition must be AFTER the discrete layer push,
       which must be AFTER the transformation happens. */
    if (!state->overflow && rsvg_node_get_parent (self)) {
        state->affine = affine_old;
        rsvg_add_clipping_rect (ctx, nx, ny, nw, nh);
        state->affine = affine_new;
    }

    rsvg_node_foreach_child (self, draw_child, ctx);

    rsvg_pop_discrete_layer (ctx);
    rsvg_drawing_ctx_pop_view_box (ctx);
}

static void
rsvg_node_svg_set_atts (RsvgNode * self, RsvgHandle * ctx, RsvgPropertyBag * atts)
{
    const char *value;
    RsvgNodeSvg *svg = (RsvgNodeSvg *) self;

    if ((value = rsvg_property_bag_lookup (atts, "viewBox")))
        svg->vbox = rsvg_css_parse_vbox (value);

    if ((value = rsvg_property_bag_lookup (atts, "preserveAspectRatio")))
        svg->preserve_aspect_ratio = rsvg_css_parse_aspect_ratio (value);
    if ((value = rsvg_property_bag_lookup (atts, "width")))
        svg->w = rsvg_length_parse (value, LENGTH_DIR_HORIZONTAL);
    if ((value = rsvg_property_bag_lookup (atts, "height")))
        svg->h = rsvg_length_parse (value, LENGTH_DIR_VERTICAL);
    /*
     * x & y attributes have no effect on outermost svg
     * http://www.w3.org/TR/SVG/struct.html#SVGElement
     */
    if (rsvg_node_get_parent (self)) {
        if ((value = rsvg_property_bag_lookup (atts, "x")))
            svg->x = rsvg_length_parse (value, LENGTH_DIR_HORIZONTAL);

        if ((value = rsvg_property_bag_lookup (atts, "y")))
            svg->y = rsvg_length_parse (value, LENGTH_DIR_VERTICAL);
    }

    /*
     * style element is not loaded yet here, so we need to store those attribues
     * to be applied later.
     */
    svg->atts = rsvg_property_bag_dup(atts);
}

void
_rsvg_node_svg_apply_atts (RsvgNodeSvg * self, RsvgHandle * ctx)
{
    const char *id = NULL, *klazz = NULL, *value;
    if (self->atts && rsvg_property_bag_size (self->atts)) {
        if ((value = rsvg_property_bag_lookup (self->atts, "class")))
            klazz = value;
        if ((value = rsvg_property_bag_lookup (self->atts, "id")))
            id = value;
        rsvg_parse_style_attrs (ctx, rsvg_node_get_state ((RsvgNode *) self), "svg", klazz, id, self->atts);
    }
}

static void
rsvg_node_svg_free (RsvgNode * self)
{
    RsvgNodeSvg *svg = (RsvgNodeSvg *) self;

    if (svg->atts) {
        rsvg_property_bag_free (svg->atts);
        svg->atts = NULL;
    }

    _rsvg_node_free (self);
}

RsvgNode *
rsvg_new_svg (const char *element_name)
{
    RsvgNodeSvg *svg;
    RsvgNodeVtable vtable = {
        rsvg_node_svg_free,
        rsvg_node_svg_draw,
        rsvg_node_svg_set_atts
    };

    svg = g_new (RsvgNodeSvg, 1);
    _rsvg_node_init (&svg->super, RSVG_NODE_TYPE_SVG, &vtable);

    svg->vbox.active = FALSE;
    svg->preserve_aspect_ratio = RSVG_ASPECT_RATIO_XMID_YMID;
    svg->x = rsvg_length_parse ("0", LENGTH_DIR_HORIZONTAL);
    svg->y = rsvg_length_parse ("0", LENGTH_DIR_VERTICAL);
    svg->w = rsvg_length_parse ("100%", LENGTH_DIR_HORIZONTAL);
    svg->h = rsvg_length_parse ("100%", LENGTH_DIR_VERTICAL);
    svg->atts = NULL;
    return &svg->super;
}

static void
rsvg_node_use_free (RsvgNode * node)
{
    RsvgNodeUse *use = (RsvgNodeUse *) node;
    g_free (use->link);
    _rsvg_node_free (node);
}

static void
rsvg_node_use_set_atts (RsvgNode * self, RsvgHandle * ctx, RsvgPropertyBag * atts)
{
    RsvgNodeUse *use;
    const char *value;

    use = (RsvgNodeUse *) self;

    if ((value = rsvg_property_bag_lookup (atts, "x")))
        use->x = rsvg_length_parse (value, LENGTH_DIR_HORIZONTAL);
    if ((value = rsvg_property_bag_lookup (atts, "y")))
        use->y = rsvg_length_parse (value, LENGTH_DIR_VERTICAL);
    if ((value = rsvg_property_bag_lookup (atts, "width")))
        use->w = rsvg_length_parse (value, LENGTH_DIR_HORIZONTAL);
    if ((value = rsvg_property_bag_lookup (atts, "height")))
        use->h = rsvg_length_parse (value, LENGTH_DIR_VERTICAL);

    if ((value = rsvg_property_bag_lookup (atts, "xlink:href"))) {
        g_free (use->link);
        use->link = g_strdup (value);
    }
}

RsvgNode *
rsvg_new_use (const char *element_name)
{
    RsvgNodeUse *use;
    RsvgNodeVtable vtable = {
        rsvg_node_use_free,
        rsvg_node_use_draw,
        rsvg_node_use_set_atts
    };

    use = g_new (RsvgNodeUse, 1);
    _rsvg_node_init (&use->super, RSVG_NODE_TYPE_USE, &vtable);

    use->x = rsvg_length_parse ("0", LENGTH_DIR_HORIZONTAL);
    use->y = rsvg_length_parse ("0", LENGTH_DIR_VERTICAL);
    use->w = rsvg_length_parse ("0", LENGTH_DIR_HORIZONTAL);
    use->h = rsvg_length_parse ("0", LENGTH_DIR_VERTICAL);
    use->link = NULL;
    return (RsvgNode *) use;
}

static void
rsvg_node_symbol_set_atts (RsvgNode * self, RsvgHandle * ctx, RsvgPropertyBag * atts)
{
    RsvgNodeSymbol *symbol = (RsvgNodeSymbol *) self;
    const char *value;

    if ((value = rsvg_property_bag_lookup (atts, "viewBox")))
        symbol->vbox = rsvg_css_parse_vbox (value);
    if ((value = rsvg_property_bag_lookup (atts, "preserveAspectRatio")))
        symbol->preserve_aspect_ratio = rsvg_css_parse_aspect_ratio (value);
}


RsvgNode *
rsvg_new_symbol (const char *element_name)
{
    RsvgNodeSymbol *symbol;
    RsvgNodeVtable vtable = {
        NULL,
        NULL,
        rsvg_node_symbol_set_atts
    };

    symbol = g_new (RsvgNodeSymbol, 1);
    _rsvg_node_init (&symbol->super, RSVG_NODE_TYPE_SYMBOL, &vtable);

    symbol->vbox.active = FALSE;
    symbol->preserve_aspect_ratio = RSVG_ASPECT_RATIO_XMID_YMID;
    return &symbol->super;
}

RsvgNode *
rsvg_new_defs (const char *element_name)
{
    RsvgNodeGroup *group;
    RsvgNodeVtable vtable = {
        NULL,
        NULL,
        NULL,
    };

    group = g_new (RsvgNodeGroup, 1);
    _rsvg_node_init (&group->super, RSVG_NODE_TYPE_DEFS, &vtable);

    return &group->super;
}

static gboolean
draw_child_if_cond_true_and_stop (RsvgNode *node, gpointer data)
{
    RsvgDrawingCtx *ctx;

    ctx = data;

    if (rsvg_node_get_state (node)->cond_true) {
        rsvg_node_draw (node, ctx, 0);

        return FALSE;
    } else {
        return TRUE;
    }
}

static void
_rsvg_node_switch_draw (RsvgNode * self, RsvgDrawingCtx * ctx, int dominate)
{
    rsvg_state_reinherit_top (ctx, rsvg_node_get_state (self), dominate);

    rsvg_push_discrete_layer (ctx);

    rsvg_node_foreach_child (self, draw_child_if_cond_true_and_stop, ctx);

    rsvg_pop_discrete_layer (ctx);
}

RsvgNode *
rsvg_new_switch (const char *element_name)
{
    RsvgNodeGroup *group;
    RsvgNodeVtable vtable = {
        NULL,
        _rsvg_node_switch_draw,
        NULL
    };

    group = g_new (RsvgNodeGroup, 1);
    _rsvg_node_init (&group->super, RSVG_NODE_TYPE_SWITCH, &vtable);

    return &group->super;
}
