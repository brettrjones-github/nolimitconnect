/*****************************************************************************
 * gotvptop_events.h: events definitions
 * Interface used to send events.
 *****************************************************************************
 * Copyright (C) 2007 GOTV authors and VideoLAN
 * $Id: 22efabef07b8b7bff3b3ad531b78ecd3bea2d2b3 $
 *
 * Authors: Pierre d'Herbemont
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

#ifndef GOTV_EVENTS_H
# define GOTV_EVENTS_H

#include <gotvptop_arrays.h>
#include <gotvptop_meta.h>

/**
 * \file
 * This file is the interface definition for events
 * (implementation in src/misc/events.c)
 */

/*****************************************************************************
 * Documentation
 *****************************************************************************/
/*
 **** Background
 *
 * This implements a way to send and receive event for an object (which can be
 * a simple C struct or less).
 *
 * This is in direct concurrency with the Variable based Callback
 * (see src/misc/variables.c).
 *
 * It has the following advantages over Variable based Callback:
 * - No need to implement the whole gotvptop_common_members in the object,
 * thus it reduce it size. This is especially true for input_item_t which
 * doesn't have gotvptop_common_members. This is the first reason of existence of
 * this implementation.
 * - Libgotvptop can easily be based upon that.
 * - Existing event are clearly declared (in include/gotvptop_events.h)
 *
 *
 **** Example usage
 *
 * (gotvptop_cool_object_t doesn't need to have the gotvptop_common_members.)
 *
 * struct gotvptop_cool_object_t
 * {
 *        ...
 *        gotvptop_event_manager_t p_event_manager;
 *        ...
 * }
 *
 * gotvptop_my_cool_object_new()
 * {
 *        ...
 *        gotvptop_event_manager_init( &p_self->p_event_manager, p_self, p_a_libgotvptop_object );
 *        ...
 * }
 *
 * gotvptop_my_cool_object_release()
 * {
 *         ...
 *         gotvptop_event_manager_fini( &p_self->p_event_manager );
 *         ...
 * }
 *
 * gotvptop_my_cool_object_do_something()
 * {
 *        ...
 *        gotvptop_event_t event;
 *        event.type = gotvptop_MyCoolObjectDidSomething;
 *        event.u.my_cool_object_did_something.what_it_did = kSomething;
 *        gotvptop_event_send( &p_self->p_event_manager, &event );
 * }
 * */

  /*****************************************************************************
 * Event Type
 *****************************************************************************/

/* List of event */
typedef enum gotvptop_event_type_t {
    /* Input item events */
    gotvptop_InputItemMetaChanged,
    gotvptop_InputItemSubItemTreeAdded,
    gotvptop_InputItemDurationChanged,
    gotvptop_InputItemPreparsedChanged,
    gotvptop_InputItemNameChanged,
    gotvptop_InputItemInfoChanged,
    gotvptop_InputItemErrorWhenReadingChanged,
    gotvptop_InputItemPreparseEnded,
} gotvptop_event_type_t;

typedef struct gotvptop_event_listeners_group_t
{
    DECL_ARRAY(struct gotvptop_event_listener_t *) listeners;
} gotvptop_event_listeners_group_t;

/* Event manager type */
typedef struct gotvptop_event_manager_t
{
    void * p_obj;
    gotvptop_mutex_t lock;
    gotvptop_event_listeners_group_t events[gotvptop_InputItemPreparseEnded + 1];
} gotvptop_event_manager_t;

/* Event definition */
typedef struct gotvptop_event_t
{
    gotvptop_event_type_t type;
    void * p_obj; /* Sender object, automatically filled by gotvptop_event_send() */
    union gotvptop_event_type_specific
    {
        /* Input item events */
        struct gotvptop_input_item_meta_changed
        {
            gotvptop_meta_type_t meta_type;
        } input_item_meta_changed;
        struct gotvptop_input_item_subitem_added
        {
            input_item_t * p_new_child;
        } input_item_subitem_added;
        struct gotvptop_input_item_subitem_tree_added
        {
            input_item_node_t * p_root;
        } input_item_subitem_tree_added;
        struct gotvptop_input_item_duration_changed
        {
            mtime_t new_duration;
        } input_item_duration_changed;
        struct gotvptop_input_item_preparsed_changed
        {
            int new_status;
        } input_item_preparsed_changed;
        struct gotvptop_input_item_name_changed
        {
            const char * new_name;
        } input_item_name_changed;
        struct gotvptop_input_item_info_changed
        {
            void * unused;
        } input_item_info_changed;
        struct input_item_error_when_reading_changed
        {
            bool new_value;
        } input_item_error_when_reading_changed;
        struct input_item_preparse_ended
        {
            int new_status;
        } input_item_preparse_ended;
    } u;
} gotvptop_event_t;

/* Event callback type */
typedef void ( *gotvptop_event_callback_t )( const gotvptop_event_t *, void * );

 /*****************************************************************************
 * Event manager
 *****************************************************************************/

/*
 * p_obj points to the object that owns the event manager, and from
 * which events are sent
 */
void gotvptop_event_manager_init( gotvptop_event_manager_t * p_em, void * p_obj );

/*
 * Destroy
 */
void gotvptop_event_manager_fini( gotvptop_event_manager_t * p_em );

/*
 * Send an event to the listener attached to this p_em.
 */
void gotvptop_event_send( gotvptop_event_manager_t * p_em, gotvptop_event_t * );

/*
 * Add a callback for an event.
 */
NLC_API int gotvptop_event_attach( gotvptop_event_manager_t * p_event_manager,
                              gotvptop_event_type_t event_type,
                              gotvptop_event_callback_t pf_callback,
                              void *p_user_data );

/*
 * Remove a callback for an event.
 */
NLC_API void gotvptop_event_detach( gotvptop_event_manager_t *p_event_manager,
                               gotvptop_event_type_t event_type,
                               gotvptop_event_callback_t pf_callback,
                               void *p_user_data );

#endif /* GOTV_EVENTS_H */
