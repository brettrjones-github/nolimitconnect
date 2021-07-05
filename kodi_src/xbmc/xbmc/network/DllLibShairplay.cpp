
#include "DllLibShairplay.h"
#include <CoreLib/VxDebug.h>

#if USE_STATIC_LIBS

DllLibShairplay::DllLibShairplay()
    : StaticDll( DLL_PATH_LIBSHAIRPLAY )
{
}

raop_t * DllLibShairplay::raop_init( int max_clients, raop_callbacks_t *callbacks, const char *pemkey )
{
    int errVal = 0;
    raop_t * raopPtr = ::raop_init( max_clients, callbacks, pemkey, &errVal );
    if( errVal )
    {
        LogMsg( LOG_ERROR, "DllLibShairplay::raop_init error %d", errVal );
        return NULL;
    }

    return raopPtr;
}

raop_t * DllLibShairplay::raop_init_from_keyfile( int max_clients, raop_callbacks_t *callbacks, const char *keyfile )
{
    int errVal = 0;
    raop_t * raopPtr = ::raop_init_from_keyfile( max_clients, callbacks, keyfile, &errVal );
    if( errVal )
    {
        LogMsg( LOG_ERROR, "DllLibShairplay::raop_init_from_keyfile error %d", errVal );
        return NULL;
    }

    return raopPtr;
}

void DllLibShairplay::raop_set_log_level( raop_t *raop, int level )
{
    ::raop_set_log_level( raop, level );
}

void DllLibShairplay::raop_set_log_callback( raop_t *raop, raop_log_callback_t callback, void *cls )
{
    ::raop_set_log_callback( raop, callback, cls );
}

int DllLibShairplay::raop_is_running( raop_t *raop )
{
    return ::raop_is_running( raop );
}

int DllLibShairplay::raop_start( raop_t *raop, unsigned short *port, const char *hwaddr, int hwaddrlen, const char *password )
{
    return ::raop_start( raop, port, hwaddr, hwaddrlen, password );
}

void DllLibShairplay::raop_stop( raop_t *raop )
{
    ::raop_stop( raop );
}

void DllLibShairplay::raop_destroy( raop_t *raop )
{
    ::raop_destroy( raop );
}

#endif // USE_STATIC_LIBS