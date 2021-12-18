/*
 * List Abstract Data Type
 * Copyright (C) 1997 Kaz Kylheku <kaz@ashi.footprints.net>
 *
 * Free Software License:
 *
 * All rights are reserved by the author, with the following exceptions:
 * Permission is granted to freely reproduce and distribute this software,
 * possibly in exchange for a fee, provided that this copyright notice appears
 * intact. Permission is also granted to adapt this software to produce
 * derivative works, as long as the modified versions carry this copyright
 * notice and additional notices stating that the work has been modified.
 * This source code may be translated into executable form and incorporated
 * into proprietary software; there is no requirement for such software to
 * contain a copyright notice related to this source.
 *
 * $Id: list.h,v 1.19 1999/11/14 20:46:19 kaz Exp $
 * $Name: kazlib_1_20 $
 */

/*
 * Modified by Johannes Lehtinen in 2006-2007.
 * Included the definition of CP_HIDDEN macro and used it in declarations and
 * definitions to hide Kazlib symbols when building a shared C-Pluff library.
 */

#ifndef LIST_H
#define LIST_H

#include "AppDependLibrariesConfig.h"

#ifdef TARGET_OS_WINDOWS
#include "libcpluff/win32/cpluffdef.h"
#else
#include "libcpluff/cpluffdef_linux.h"
#endif

#include <limits.h>

#ifdef KAZLIB_SIDEEFFECT_DEBUG
#include "sfx.h"
#define LIST_SFX_CHECK(E) SFX_CHECK(E)
#else
#define LIST_SFX_CHECK(E) (E)
#endif

/*
 * Blurb for inclusion into C++ translation units
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long listcount_t;
#define LISTCOUNT_T_MAX ULONG_MAX

typedef struct lnode_t {
    #if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
    struct lnode_t *cp_list_next;
    struct lnode_t *cp_list_prev;
    void *cp_list_data;
    #else
    int cp_list_dummy;
    #endif
} lnode_t;

typedef struct lnodepool_t {
    #if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
    struct lnode_t *cp_list_pool;
    struct lnode_t *cp_list_free;
    listcount_t cp_list_size;
    #else
    int cp_list_dummy;
    #endif
} lnodepool_t;

typedef struct cp_list_t {
    #if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
    lnode_t cp_list_nilnode;
    listcount_t cp_list_nodecount;
    listcount_t cp_list_maxcount;
    #else
    int cp_list_dummy;
    #endif
} cp_list_t;

CP_HIDDEN lnode_t *lnode_create(void *);
CP_HIDDEN lnode_t *lnode_init(lnode_t *, void *);
CP_HIDDEN void lnode_destroy(lnode_t *);
CP_HIDDEN void lnode_put(lnode_t *, void *);
CP_HIDDEN void *lnode_get(lnode_t *);
CP_HIDDEN int lnode_is_in_a_list(lnode_t *);

#if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
#define lnode_put(N, D)		((N)->cp_list_data = (D))
#define lnode_get(N)		((N)->cp_list_data)
#endif

CP_HIDDEN lnodepool_t *lnode_pool_init(lnodepool_t *, lnode_t *, listcount_t);
CP_HIDDEN lnodepool_t *lnode_pool_create(listcount_t);
CP_HIDDEN void lnode_pool_destroy(lnodepool_t *);
CP_HIDDEN lnode_t *lnode_borrow(lnodepool_t *, void *);
CP_HIDDEN void lnode_return(lnodepool_t *, lnode_t *);
CP_HIDDEN int lnode_pool_isempty(lnodepool_t *);
CP_HIDDEN int lnode_pool_isfrom(lnodepool_t *, lnode_t *);

CP_HIDDEN cp_list_t *cp_list_init(cp_list_t *, listcount_t);
CP_HIDDEN cp_list_t *cp_list_create(listcount_t);
CP_HIDDEN void cp_list_destroy(cp_list_t *);
CP_HIDDEN void cp_list_destroy_nodes(cp_list_t *);
CP_HIDDEN void cp_list_return_nodes(cp_list_t *, lnodepool_t *);

CP_HIDDEN listcount_t cp_list_count(cp_list_t *);
CP_HIDDEN int cp_list_isempty(cp_list_t *);
CP_HIDDEN int cp_list_isfull(cp_list_t *);
CP_HIDDEN int cp_list_contains(cp_list_t *, lnode_t *);

CP_HIDDEN void cp_list_append(cp_list_t *, lnode_t *);
CP_HIDDEN void cp_list_prepend(cp_list_t *, lnode_t *);
CP_HIDDEN void cp_list_ins_before(cp_list_t *, lnode_t *, lnode_t *);
CP_HIDDEN void cp_list_ins_after(cp_list_t *, lnode_t *, lnode_t *);

CP_HIDDEN lnode_t *cp_list_first(cp_list_t *);
CP_HIDDEN lnode_t *cp_list_last(cp_list_t *);
CP_HIDDEN lnode_t *cp_list_next(cp_list_t *, lnode_t *);
CP_HIDDEN lnode_t *cp_list_prev(cp_list_t *, lnode_t *);

CP_HIDDEN lnode_t *cp_list_del_first(cp_list_t *);
CP_HIDDEN lnode_t *cp_list_del_last(cp_list_t *);
CP_HIDDEN lnode_t *cp_list_delete(cp_list_t *, lnode_t *);

CP_HIDDEN void cp_list_process(cp_list_t *, void *, void (*)(cp_list_t *, lnode_t *, void *));

CP_HIDDEN int cp_list_verify(cp_list_t *);

#if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
#define lnode_pool_isempty(P)	((P)->cp_list_free == 0)
#define cp_list_count(L)		((L)->cp_list_nodecount)
#define cp_list_isempty(L)		((L)->cp_list_nodecount == 0)
#define cp_list_isfull(L)		(LIST_SFX_CHECK(L)->cp_list_nodecount == (L)->cp_list_maxcount)
#define cp_list_next(L, N)		(LIST_SFX_CHECK(N)->cp_list_next == &(L)->cp_list_nilnode ? NULL : (N)->cp_list_next)
#define cp_list_prev(L, N)		(LIST_SFX_CHECK(N)->cp_list_prev == &(L)->cp_list_nilnode ? NULL : (N)->cp_list_prev)
#define cp_list_first(L)		cp_list_next(LIST_SFX_CHECK(L), &(L)->cp_list_nilnode)
#define cp_list_last(L)		cp_list_prev(LIST_SFX_CHECK(L), &(L)->cp_list_nilnode)
#endif

#if defined(LIST_IMPLEMENTATION) || !defined(KAZLIB_OPAQUE_DEBUG)
#define cp_list_append(L, N)	cp_list_ins_before(LIST_SFX_CHECK(L), N, &(L)->cp_list_nilnode)
#define cp_list_prepend(L, N)	cp_list_ins_after(LIST_SFX_CHECK(L), N, &(L)->cp_list_nilnode)
#define cp_list_del_first(L)	cp_list_delete(LIST_SFX_CHECK(L), cp_list_first(L))
#define cp_list_del_last(L)	cp_list_delete(LIST_SFX_CHECK(L), cp_list_last(L))
#endif

/* destination list on the left, source on the right */

CP_HIDDEN void cp_list_extract(cp_list_t *, cp_list_t *, lnode_t *, lnode_t *);
CP_HIDDEN void cp_list_transfer(cp_list_t *, cp_list_t *, lnode_t *first);
CP_HIDDEN void cp_list_merge(cp_list_t *, cp_list_t *, int (const void *, const void *));
CP_HIDDEN void cp_list_sort(cp_list_t *, int (const void *, const void *));
CP_HIDDEN lnode_t *cp_list_find(cp_list_t *, const void *, int (const void *, const void *));
CP_HIDDEN int cp_list_is_sorted(cp_list_t *, int (const void *, const void *));

#ifdef __cplusplus
}
#endif

#endif
