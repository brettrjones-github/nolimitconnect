
#include "DllLibCPluff.h"

#if USE_STATIC_LIBS

DllLibCPluff::DllLibCPluff( )
: StaticDll(DLL_PATH_CPLUFF)
{
}

/*
DllLibCPluff& DllLibCPluff::operator=( const DllLibCPluff& rhs )
{
    StaticDll::operator=( rhs );
    return *this;
}

DllLibCPluff::DllLibCPluff( DllLibCPluff&& rhs )
    : StaticDll( rhs )
{

}

DllLibCPluff& DllLibCPluff::operator=( DllLibCPluff&& rhs )
{
    StaticDll::operator=( rhs );
    return *this;
}*/

DllLibCPluff::~DllLibCPluff()
{

}

const char* DllLibCPluff::get_version()
{
    return  cp_get_version();
}

void DllLibCPluff::set_fatal_error_handler( cp_fatal_error_func_t p1 )
{
     return  cp_set_fatal_error_handler( p1 );
}

cp_status_t DllLibCPluff::init()
{
    return  cp_init( );
}

void DllLibCPluff::destroy()
{
    return  cp_destroy( );
}

cp_context_t* DllLibCPluff::create_context( cp_status_t *p1 )
{
    return  cp_create_context( p1 );
}

void DllLibCPluff::destroy_context( cp_context_t *p1 )
{
    return  cp_destroy_context( p1 );
}

cp_status_t DllLibCPluff::register_pcollection( cp_context_t *p1, const char *p2 )
{
    return  cp_register_pcollection( p1, p2 );
}
    
void DllLibCPluff::unregister_pcollection( cp_context_t *p1, const char *p2 )
{
    return  cp_unregister_pcollection( p1, p2 );
}

void DllLibCPluff::unregister_pcollections( cp_context_t *p1 )
{
    return  cp_destroy_context( p1 );
}

cp_status_t DllLibCPluff::register_logger( cp_context_t *p1, cp_logger_func_t p2, void *p3, cp_log_severity_t p4 )
{
    return  cp_register_logger( p1, p2, p3, p4 );
}

void DllLibCPluff::unregister_logger( cp_context_t *p1, cp_logger_func_t p2 )
{
    return  cp_unregister_logger( p1, p2 );
}
    
cp_status_t DllLibCPluff::scan_plugins( cp_context_t *p1, int p2 )
{
     return  cp_scan_plugins( p1, p2 );
}

cp_plugin_info_t* DllLibCPluff::get_plugin_info( cp_context_t *p1, const char *p2, cp_status_t *p3 )
{
    return  cp_get_plugin_info( p1, p2, p3 );
}

cp_plugin_info_t** DllLibCPluff::get_plugins_info( cp_context_t *p1, cp_status_t *p2, int *p3 )
{
    return  cp_get_plugins_info( p1, p2, p3 );
}
    
cp_extension_t** DllLibCPluff::get_extensions_info( cp_context_t *p1, const char *p2, cp_status_t *p3, int *p4 )
{
    return  cp_get_extensions_info( p1, p2, p3, p4 );
}
    
void DllLibCPluff::release_info( cp_context_t *p1, void *p2 )
{
    return  cp_release_info( p1, p2 );
}

cp_cfg_element_t* DllLibCPluff::lookup_cfg_element( cp_cfg_element_t *p1, const char *p2 )
{
    return  cp_lookup_cfg_element( p1, p2 );
}
    
char* DllLibCPluff::lookup_cfg_value( cp_cfg_element_t *p1, const char *p2 )
{
    return  cp_lookup_cfg_value( p1, p2 );
}

cp_status_t DllLibCPluff::define_symbol( cp_context_t *p1, const char *p2, void *p3 )
{
    return  cp_define_symbol( p1, p2, p3 );
}
    
void* DllLibCPluff::resolve_symbol( cp_context_t *p1, const char *p2, const char *p3, cp_status_t *p4 )
{
    return  cp_resolve_symbol( p1, p2, p3, p4 );
}
    
void DllLibCPluff::release_symbol( cp_context_t *p1, const void *p2 )
{
    return  cp_release_symbol( p1, p2 );
}
    
cp_plugin_info_t* DllLibCPluff::load_plugin_descriptor( cp_context_t *p1, const char *p2, cp_status_t *p3 )
{
    return  cp_load_plugin_descriptor( p1, p2, p3 );
}

cp_plugin_info_t* DllLibCPluff::load_plugin_descriptor_from_memory( cp_context_t *p1, const char *p2, unsigned int p3, cp_status_t *p4 )
{
    return  cp_load_plugin_descriptor_from_memory( p1, p2, p3, p4 );
}

cp_status_t DllLibCPluff::uninstall_plugin( cp_context_t *p1, const char *p2 )
{
    return  cp_uninstall_plugin( p1, p2 );
}

#endif // !USE_STATIC_LIBS