
#include "DllLibPlist.h"

#if USE_STATIC_LIBS

DllLibPlist::DllLibPlist()
    : StaticDll( DLL_PATH_LIBPLIST )
{
}

void DllLibPlist::plist_from_bin( const char *plist_bin, uint32_t length, plist_t * plist )
{
    ::plist_from_bin( plist_bin, length, plist );
}

plist_t DllLibPlist::plist_new_dict( void )
{
    return ::plist_new_dict( );
}

uint32_t  DllLibPlist::plist_dict_get_size( plist_t node )
{
    return ::plist_dict_get_size( node );
}

void DllLibPlist::plist_get_string_val( plist_t node, char **val )
{
    ::plist_get_string_val( node, val );
}

void DllLibPlist::plist_get_real_val( plist_t node, double *val )
{
    ::plist_get_real_val( node, val );
}

plist_t DllLibPlist::plist_dict_get_item( plist_t node, const char* key )
{
    return ::plist_dict_get_item( node, key );
}

void DllLibPlist::plist_free( plist_t plist )
{
    ::plist_free( plist );
}

void DllLibPlist::plist_to_xml( plist_t plist, char **plist_xml, uint32_t * length )
{
    ::plist_to_xml( plist, plist_xml, length );
}

void DllLibPlist::plist_dict_new_iter( plist_t node, plist_dict_iter *iter )
{
    ::plist_dict_new_iter( node, iter );
}

void DllLibPlist::plist_dict_next_item( plist_t node, plist_dict_iter iter, char **key, plist_t *val )
{
    ::plist_dict_next_item( node, iter, key, val );
}

#endif // USE_STATIC_LIBS