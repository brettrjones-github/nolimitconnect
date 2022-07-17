/*****************************************************************************
 * gotvptop_variables.h: variables handling
 *****************************************************************************
 * Copyright (C) 2002-2004 GOTV authors and VideoLAN
 * $Id: 83752b171f82c86164142a6254f513fc9cb7a324 $
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *          Gildas Bazin <gbazin@netcourrier.com>
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

#ifndef GOTV_VARIABLES_H
#define GOTV_VARIABLES_H 1


/**
 * \defgroup variables Variables
 * \ingroup gotvptop_object
 *
 * GOTV object variables and callbacks
 *
 * @{
 * \file
 * GOTV object variables and callbacks interface
 */

#define GOTV_VAR_TYPE      0x00ff
#define GOTV_VAR_CLASS     0x00f0
#define GOTV_VAR_FLAGS     0xff00

/**
 * \defgroup var_type Variable types
 * These are the different types a gotvptop variable can have.
 * @{
 */
#define GOTV_VAR_VOID      0x0010
#define GOTV_VAR_BOOL      0x0020
#define GOTV_VAR_INTEGER   0x0030
#define GOTV_VAR_STRING    0x0040
#define GOTV_VAR_FLOAT     0x0050
#define GOTV_VAR_ADDRESS   0x0070
#define GOTV_VAR_COORDS    0x00A0
/**@}*/

/** \defgroup var_flags Additive flags
 * These flags are added to the type field of the variable. Most as a result of
 * a var_Change() call, but some may be added at creation time
 * @{
 */
#define GOTV_VAR_HASCHOICE 0x0100

#define GOTV_VAR_ISCOMMAND 0x2000

/** Creation flag */
/* If the variable is not found on the current module
   search all parents and finally module config until found */
#define GOTV_VAR_DOINHERIT 0x8000
/**@}*/

/**
 * \defgroup var_action Variable actions
 * These are the different actions that can be used with var_Change().
 * The parameters given are the meaning of the two last parameters of
 * var_Change() when this action is being used.
 * @{
 */

#define GOTV_VAR_SETSTEP             0x0012

/**
 * Set the value of this variable without triggering any callbacks
 * \param p_val The new value
 * \param p_val2 Unused
 */
#define GOTV_VAR_SETVALUE            0x0013

#define GOTV_VAR_SETTEXT             0x0014
#define GOTV_VAR_GETTEXT             0x0015

#define GOTV_VAR_GETMIN              0x0016
#define GOTV_VAR_GETMAX              0x0017
#define GOTV_VAR_GETSTEP             0x0018

#define GOTV_VAR_ADDCHOICE           0x0020
#define GOTV_VAR_DELCHOICE           0x0021
#define GOTV_VAR_CLEARCHOICES        0x0022
#define GOTV_VAR_GETCHOICES          0x0024

#define GOTV_VAR_CHOICESCOUNT        0x0026
#define GOTV_VAR_SETMINMAX           0x0027

/**@}*/

/** \defgroup var_GetAndSet Variable actions
 * These are the different actions that can be used with var_GetAndSet()
 * @{
 */
enum {
    GOTV_VAR_BOOL_TOGGLE, /**< Invert a boolean value (param ignored) */
    GOTV_VAR_INTEGER_ADD, /**< Add parameter to an integer value */
    GOTV_VAR_INTEGER_OR,  /**< Binary OR over an integer bits field */
    GOTV_VAR_INTEGER_NAND,/**< Binary NAND over an integer bits field */
};
/**@}*/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
NLC_API int libgotvptop_var_Create( gotvptop_object_t *, const char *, int );
#define var_Create(a,b,c) libgotvptop_var_Create( GOTV_OBJECT(a), b, c )

NLC_API void libgotvptop_var_Destroy( gotvptop_object_t *, const char * );
#define var_Destroy(a,b) libgotvptop_var_Destroy( GOTV_OBJECT(a), b )

NLC_API int libgotvptop_var_Change( gotvptop_object_t *, const char *, int, gotvptop_value_t *, gotvptop_value_t * );
#define var_Change(a,b,c,d,e) libgotvptop_var_Change( GOTV_OBJECT(a), b, c, d, e )

NLC_API int libgotvptop_var_Type( gotvptop_object_t *, const char * ) GOTV_USED;
#define var_Type(a,b) libgotvptop_var_Type( GOTV_OBJECT(a), b )

NLC_API int libgotvptop_var_Set( gotvptop_object_t *, const char *, gotvptop_value_t );
#define var_Set(a,b,c) libgotvptop_var_Set( GOTV_OBJECT(a), b, c )

NLC_API int libgotvptop_var_Get( gotvptop_object_t *, const char *, gotvptop_value_t * );
#define var_Get(a,b,c) libgotvptop_var_Get( GOTV_OBJECT(a), b, c )

NLC_API int libgotvptop_var_SetChecked( gotvptop_object_t *, const char *, int, gotvptop_value_t );
#define var_SetChecked(o,n,t,v) libgotvptop_var_SetChecked(GOTV_OBJECT(o),n,t,v)

NLC_API int libgotvptop_var_GetChecked( gotvptop_object_t *, const char *, int, gotvptop_value_t * );
#define var_GetChecked(o,n,t,v) libgotvptop_var_GetChecked(GOTV_OBJECT(o),n,t,v)

NLC_API int libgotvptop_var_GetAndSet( gotvptop_object_t *, const char *, int, gotvptop_value_t * );
#define var_GetAndSet(o,n,t,v) libgotvptop_var_GetAndSet(GOTV_OBJECT(o),n,t,v)

NLC_API int libgotvptop_var_Inherit( gotvptop_object_t *, const char *, int, gotvptop_value_t * );
#define var_Inherit(o,n,t,v) libgotvptop_var_Inherit(GOTV_OBJECT(o),n,t,v)


NLC_API void libgotvptop_var_FreeList( gotvptop_value_t *, gotvptop_value_t * );
#define var_FreeList(t,v) libgotvptop_var_FreeList(t,v)


/*****************************************************************************
 * Variable callbacks
 *****************************************************************************
 * int MyCallback( gotvptop_object_t *p_this,
 *                 char const *psz_variable,
 *                 gotvptop_value_t oldvalue,
 *                 gotvptop_value_t newvalue,
 *                 void *p_data);
 *****************************************************************************/
NLC_API void libgotvptop_var_AddCallback( gotvptop_object_t *, const char *, gotvptop_callback_t, void * );
#define var_AddCallback(a,b,c,d) libgotvptop_var_AddCallback( GOTV_OBJECT(a), b, c, d )

NLC_API void libgotvptop_var_DelCallback( gotvptop_object_t *, const char *, gotvptop_callback_t, void * );
#define var_DelCallback(a,b,c,d) libgotvptop_var_DelCallback( GOTV_OBJECT(a), b, c, d )

NLC_API void libgotvptop_var_TriggerCallback( gotvptop_object_t *, const char * );
#define var_TriggerCallback(a,b) libgotvptop_var_TriggerCallback( GOTV_OBJECT(a), b )


NLC_API void libgotvptop_var_AddListCallback( gotvptop_object_t *, const char *, gotvptop_list_callback_t, void * );
#define var_AddListCallback(a,b,c,d) libgotvptop_var_AddListCallback( GOTV_OBJECT(a), b, c, d )

NLC_API void libgotvptop_var_DelListCallback( gotvptop_object_t *, const char *, gotvptop_list_callback_t, void * );
#define var_DelListCallback(a,b,c,d) libgotvptop_var_DelListCallback( GOTV_OBJECT(a), b, c, d )

/*****************************************************************************
 * helpers functions
 *****************************************************************************/

/**
 * Set the value of an integer variable
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 * \param i The new integer value of this variable
 */
static inline int var_SetInteger( gotvptop_object_t *p_obj, const char *psz_name,
                                  int64_t i )
{
    gotvptop_value_t val;
    val.i_int = i;
    return var_SetChecked( p_obj, psz_name, GOTV_VAR_INTEGER, val );
}

/**
 * Set the value of an boolean variable
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 * \param b The new boolean value of this variable
 */
static inline int var_SetBool( gotvptop_object_t *p_obj, const char *psz_name, bool b )
{
    gotvptop_value_t val;
    val.b_bool = b;
    return var_SetChecked( p_obj, psz_name, GOTV_VAR_BOOL, val );
}

static inline int var_SetCoords( gotvptop_object_t *obj, const char *name,
                                 int32_t x, int32_t y )
{
    gotvptop_value_t val;
    val.coords.x = x;
    val.coords.y = y;
    return var_SetChecked (obj, name, GOTV_VAR_COORDS, val);
}
#define var_SetCoords(o,n,x,y) var_SetCoords(GOTV_OBJECT(o),n,x,y)

/**
 * Set the value of a float variable
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 * \param f The new float value of this variable
 */
static inline int var_SetFloat( gotvptop_object_t *p_obj, const char *psz_name, float f )
{
    gotvptop_value_t val;
    val.f_float = f;
    return var_SetChecked( p_obj, psz_name, GOTV_VAR_FLOAT, val );
}

/**
 * Set the value of a string variable
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 * \param psz_string The new string value of this variable
 */
static inline int var_SetString( gotvptop_object_t *p_obj, const char *psz_name, const char *psz_string )
{
    gotvptop_value_t val;
    val.psz_string = (char *)psz_string;
    return var_SetChecked( p_obj, psz_name, GOTV_VAR_STRING, val );
}

/**
 * Set the value of a pointer variable
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 * \param ptr The new pointer value of this variable
 */
static inline
int var_SetAddress( gotvptop_object_t *p_obj, const char *psz_name, void *ptr )
{
    gotvptop_value_t val;
    val.p_address = ptr;
    return var_SetChecked( p_obj, psz_name, GOTV_VAR_ADDRESS, val );
}

#define var_SetInteger(a,b,c)   var_SetInteger( GOTV_OBJECT(a),b,c)
#define var_SetBool(a,b,c)      var_SetBool( GOTV_OBJECT(a),b,c)
#define var_SetFloat(a,b,c)     var_SetFloat( GOTV_OBJECT(a),b,c)
#define var_SetString(a,b,c)    var_SetString( GOTV_OBJECT(a),b,c)
#define var_SetAddress(o, n, p) var_SetAddress(GOTV_OBJECT(o), n, p)


/**
 * Get an integer value
*
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline int64_t var_GetInteger( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    if( !var_GetChecked( p_obj, psz_name, GOTV_VAR_INTEGER, &val ) )
        return val.i_int;
    else
        return 0;
}

/**
 * Get a boolean value
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline bool var_GetBool( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val; val.b_bool = false;

    if( !var_GetChecked( p_obj, psz_name, GOTV_VAR_BOOL, &val ) )
        return val.b_bool;
    else
        return false;
}

static inline void var_GetCoords( gotvptop_object_t *obj, const char *name,
                                  int32_t *px, int32_t *py )
{
    gotvptop_value_t val;

    if (likely(!var_GetChecked (obj, name, GOTV_VAR_COORDS, &val)))
    {
        *px = val.coords.x;
        *py = val.coords.y;
    }
    else
        *px = *py = 0;
}
#define var_GetCoords(o,n,x,y) var_GetCoords(GOTV_OBJECT(o),n,x,y)

/**
 * Get a float value
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline float var_GetFloat( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val; val.f_float = 0.0;
    if( !var_GetChecked( p_obj, psz_name, GOTV_VAR_FLOAT, &val ) )
        return val.f_float;
    else
        return 0.0;
}

/**
 * Get a string value
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED GOTV_MALLOC
static inline char *var_GetString( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val; val.psz_string = NULL;
    if( var_GetChecked( p_obj, psz_name, GOTV_VAR_STRING, &val ) )
        return NULL;
    else
        return val.psz_string;
}

GOTV_USED GOTV_MALLOC
static inline char *var_GetNonEmptyString( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    if( var_GetChecked( p_obj, psz_name, GOTV_VAR_STRING, &val ) )
        return NULL;
    if( val.psz_string && *val.psz_string )
        return val.psz_string;
    free( val.psz_string );
    return NULL;
}

GOTV_USED
static inline void *var_GetAddress( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    if( var_GetChecked( p_obj, psz_name, GOTV_VAR_ADDRESS, &val ) )
        return NULL;
    else
        return val.p_address;
}

/**
 * Increment an integer variable
 * \param p_obj the object that holds the variable
 * \param psz_name the name of the variable
 */
static inline int64_t var_IncInteger( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    val.i_int = 1;
    if( var_GetAndSet( p_obj, psz_name, GOTV_VAR_INTEGER_ADD, &val ) )
        return 0;
    return val.i_int;
}
#define var_IncInteger(a,b) var_IncInteger( GOTV_OBJECT(a), b )

/**
 * Decrement an integer variable
 * \param p_obj the object that holds the variable
 * \param psz_name the name of the variable
 */
static inline int64_t var_DecInteger( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    val.i_int = -1;
    if( var_GetAndSet( p_obj, psz_name, GOTV_VAR_INTEGER_ADD, &val ) )
        return 0;
    return val.i_int;
}
#define var_DecInteger(a,b) var_DecInteger( GOTV_OBJECT(a), b )

static inline uint64_t var_OrInteger( gotvptop_object_t *obj, const char *name,
                                      unsigned v )
{
    gotvptop_value_t val;
    val.i_int = v;
    if( var_GetAndSet( obj, name, GOTV_VAR_INTEGER_OR, &val ) )
        return 0;
    return val.i_int;
}
#define var_OrInteger(a,b,c) var_OrInteger(GOTV_OBJECT(a),b,c)

static inline uint64_t var_NAndInteger( gotvptop_object_t *obj, const char *name,
                                        unsigned v )
{
    gotvptop_value_t val;
    val.i_int = v;
    if( var_GetAndSet( obj, name, GOTV_VAR_INTEGER_NAND, &val ) )
        return 0;
    return val.i_int;
}
#define var_NAndInteger(a,b,c) var_NAndInteger(GOTV_OBJECT(a),b,c)

/**
 * Create a integer variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline int64_t var_CreateGetInteger( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_INTEGER | GOTV_VAR_DOINHERIT );
    return var_GetInteger( p_obj, psz_name );
}

/**
 * Create a boolean variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline bool var_CreateGetBool( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_BOOL | GOTV_VAR_DOINHERIT );
    return var_GetBool( p_obj, psz_name );
}

/**
 * Create a float variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline float var_CreateGetFloat( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_FLOAT | GOTV_VAR_DOINHERIT );
    return var_GetFloat( p_obj, psz_name );
}

/**
 * Create a string variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED GOTV_MALLOC
static inline char *var_CreateGetString( gotvptop_object_t *p_obj,
                                           const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_STRING | GOTV_VAR_DOINHERIT );
    return var_GetString( p_obj, psz_name );
}

GOTV_USED GOTV_MALLOC
static inline char *var_CreateGetNonEmptyString( gotvptop_object_t *p_obj,
                                                   const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_STRING | GOTV_VAR_DOINHERIT );
    return var_GetNonEmptyString( p_obj, psz_name );
}

/**
 * Create an address variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline void *var_CreateGetAddress( gotvptop_object_t *p_obj,
                                           const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_ADDRESS | GOTV_VAR_DOINHERIT );
    return var_GetAddress( p_obj, psz_name );
}

#define var_CreateGetInteger(a,b)   var_CreateGetInteger( GOTV_OBJECT(a),b)
#define var_CreateGetBool(a,b)   var_CreateGetBool( GOTV_OBJECT(a),b)
#define var_CreateGetFloat(a,b)   var_CreateGetFloat( GOTV_OBJECT(a),b)
#define var_CreateGetString(a,b)   var_CreateGetString( GOTV_OBJECT(a),b)
#define var_CreateGetNonEmptyString(a,b)   var_CreateGetNonEmptyString( GOTV_OBJECT(a),b)
#define var_CreateGetAddress(a,b)  var_CreateGetAddress( GOTV_OBJECT(a),b)

/**
 * Create a integer command variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline int64_t var_CreateGetIntegerCommand( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_INTEGER | GOTV_VAR_DOINHERIT
                                   | GOTV_VAR_ISCOMMAND );
    return var_GetInteger( p_obj, psz_name );
}

/**
 * Create a boolean command variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline bool var_CreateGetBoolCommand( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_BOOL | GOTV_VAR_DOINHERIT
                                   | GOTV_VAR_ISCOMMAND );
    return var_GetBool( p_obj, psz_name );
}

/**
 * Create a float command variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED
static inline float var_CreateGetFloatCommand( gotvptop_object_t *p_obj, const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_FLOAT | GOTV_VAR_DOINHERIT
                                   | GOTV_VAR_ISCOMMAND );
    return var_GetFloat( p_obj, psz_name );
}

/**
 * Create a string command variable with inherit and get its value.
 *
 * \param p_obj The object that holds the variable
 * \param psz_name The name of the variable
 */
GOTV_USED GOTV_MALLOC
static inline char *var_CreateGetStringCommand( gotvptop_object_t *p_obj,
                                           const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_STRING | GOTV_VAR_DOINHERIT
                                   | GOTV_VAR_ISCOMMAND );
    return var_GetString( p_obj, psz_name );
}

GOTV_USED GOTV_MALLOC
static inline char *var_CreateGetNonEmptyStringCommand( gotvptop_object_t *p_obj,
                                                   const char *psz_name )
{
    var_Create( p_obj, psz_name, GOTV_VAR_STRING | GOTV_VAR_DOINHERIT
                                   | GOTV_VAR_ISCOMMAND );
    return var_GetNonEmptyString( p_obj, psz_name );
}

#define var_CreateGetIntegerCommand(a,b)   var_CreateGetIntegerCommand( GOTV_OBJECT(a),b)
#define var_CreateGetBoolCommand(a,b)   var_CreateGetBoolCommand( GOTV_OBJECT(a),b)
#define var_CreateGetFloatCommand(a,b)   var_CreateGetFloatCommand( GOTV_OBJECT(a),b)
#define var_CreateGetStringCommand(a,b)   var_CreateGetStringCommand( GOTV_OBJECT(a),b)
#define var_CreateGetNonEmptyStringCommand(a,b)   var_CreateGetNonEmptyStringCommand( GOTV_OBJECT(a),b)

GOTV_USED
static inline int var_CountChoices( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t count;
    if( var_Change( p_obj, psz_name, GOTV_VAR_CHOICESCOUNT, &count, NULL ) )
        return 0;
    return (int)count.i_int;
}
#define var_CountChoices(a,b) var_CountChoices( GOTV_OBJECT(a),b)


static inline bool var_ToggleBool( gotvptop_object_t *p_obj, const char *psz_name )
{
    gotvptop_value_t val;
    if( var_GetAndSet( p_obj, psz_name, GOTV_VAR_BOOL_TOGGLE, &val ) )
        return false;
    return val.b_bool;
}
#define var_ToggleBool(a,b) var_ToggleBool( GOTV_OBJECT(a),b )


GOTV_USED
static inline bool var_InheritBool( gotvptop_object_t *obj, const char *name )
{
    gotvptop_value_t val;

    if( var_Inherit( obj, name, GOTV_VAR_BOOL, &val ) )
        val.b_bool = false;
    return val.b_bool;
}
#define var_InheritBool(o, n) var_InheritBool(GOTV_OBJECT(o), n)

GOTV_USED
static inline int64_t var_InheritInteger( gotvptop_object_t *obj, const char *name )
{
    gotvptop_value_t val;

    if( var_Inherit( obj, name, GOTV_VAR_INTEGER, &val ) )
        val.i_int = 0;
    return val.i_int;
}
#define var_InheritInteger(o, n) var_InheritInteger(GOTV_OBJECT(o), n)

GOTV_USED
static inline float var_InheritFloat( gotvptop_object_t *obj, const char *name )
{
    gotvptop_value_t val;

    if( var_Inherit( obj, name, GOTV_VAR_FLOAT, &val ) )
        val.f_float = 0.;
    return val.f_float;
}
#define var_InheritFloat(o, n) var_InheritFloat(GOTV_OBJECT(o), n)

GOTV_USED GOTV_MALLOC
static inline char *var_InheritString( gotvptop_object_t *obj, const char *name )
{
    gotvptop_value_t val;

    if( var_Inherit( obj, name, GOTV_VAR_STRING, &val ) )
        val.psz_string = NULL;
    else if( val.psz_string && !*val.psz_string )
    {
        free( val.psz_string );
        val.psz_string = NULL;
    }
    return val.psz_string;
}
#define var_InheritString(o, n) var_InheritString(GOTV_OBJECT(o), n)

GOTV_USED
static inline void *var_InheritAddress( gotvptop_object_t *obj, const char *name )
{
    gotvptop_value_t val;

    if( var_Inherit( obj, name, GOTV_VAR_ADDRESS, &val ) )
        val.p_address = NULL;
    return val.p_address;
}
#define var_InheritAddress(o, n) var_InheritAddress(GOTV_OBJECT(o), n)

NLC_API int var_InheritURational( gotvptop_object_t *, unsigned *num, unsigned *den, const char *var );
#define var_InheritURational(a,b,c,d) var_InheritURational(GOTV_OBJECT(a), b, c, d)

#define var_GetInteger(a,b)   var_GetInteger( GOTV_OBJECT(a),b)
#define var_GetBool(a,b)   var_GetBool( GOTV_OBJECT(a),b)
#define var_GetFloat(a,b)   var_GetFloat( GOTV_OBJECT(a),b)
#define var_GetString(a,b)   var_GetString( GOTV_OBJECT(a),b)
#define var_GetNonEmptyString(a,b)   var_GetNonEmptyString( GOTV_OBJECT(a),b)
#define var_GetAddress(a,b)  var_GetAddress( GOTV_OBJECT(a),b)

NLC_API int var_LocationParse(gotvptop_object_t *, const char *mrl, const char *prefix);
#define var_LocationParse(o, m, p) var_LocationParse(GOTV_OBJECT(o), m, p)

/**
 * @}
 */
#endif /*  _GOTV_VARIABLES_H */
