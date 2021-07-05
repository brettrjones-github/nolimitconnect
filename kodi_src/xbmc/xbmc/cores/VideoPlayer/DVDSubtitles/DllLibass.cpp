
#include "DllLibass.h"
#if USE_STATIC_LIBS

DllLibass::DllLibass()
:StaticDll( DLL_PATH_LIBASS )
{
}
/*
DllLibass& DllLibass::operator=( const DllLibass& rhs )
{
    StaticDll::operator=( rhs );
     return  *this;
}

DllLibass::DllLibass( DllLibass&& rhs )
:StaticDll( rhs )
{
}

DllLibass& DllLibass::operator=( DllLibass&& rhs )
{
    StaticDll::operator=( rhs );
    return  *this;
}*/

DllLibass::~DllLibass()
{
}

void DllLibass::ass_set_extract_fonts( ASS_Library * p1, int p2 )
{
    ::ass_set_extract_fonts( p1, p2 );
}
    
void DllLibass::ass_set_fonts_dir( ASS_Library * p1, const char * p2 )
{
   ::ass_set_fonts_dir( p1, p2 );
}
 
ASS_Library * DllLibass::ass_library_init( )
{
    return ::ass_library_init( );
}
    
ASS_Renderer * DllLibass::ass_renderer_init( ASS_Library * p1 )
{
    return ::ass_renderer_init( p1 );
}
    
void DllLibass::ass_set_frame_size( ASS_Renderer * p1, int p2, int p3 )
{
    ::ass_set_frame_size( p1, p2, p3 );
}

void DllLibass::ass_set_aspect_ratio( ASS_Renderer * p1, double p2, double p3 )
{
    ::ass_set_aspect_ratio( p1, p2, p3 );
}

void DllLibass::ass_set_margins( ASS_Renderer * p1, int p2, int p3, int p4, int p5 )
{
    ::ass_set_margins( p1, p2, p3, p4, p5 );
}
    
void DllLibass::ass_set_use_margins( ASS_Renderer * p1, int p2 )
{
    ::ass_set_use_margins( p1, p2 );
}
    
void DllLibass::ass_set_line_position( ASS_Renderer * p1, double p2 )
{
    ::ass_set_line_position( p1, p2 );
}

void DllLibass::ass_set_font_scale( ASS_Renderer * p1, double p2 )
{
    ::ass_set_font_scale( p1, p2 );
}
    
ASS_Image * DllLibass::ass_render_frame( ASS_Renderer * p1, ASS_Track * p2, long long p3, int * p4 )
{
    return ::ass_render_frame( p1, p2, p3, p4 );
}
    
ASS_Track * DllLibass::ass_new_track( ASS_Library * p1 )
{
    return ::ass_new_track( p1 );
}
    
ASS_Track * DllLibass::ass_read_file( ASS_Library * p1, char * p2, char * p3 )
{
    return ::ass_read_file( p1, p2, p3 );
}
    
ASS_Track * DllLibass::ass_read_memory( ASS_Library * p1, char * p2, size_t p3, char * p4 )
{
    return ::ass_read_memory( p1, p2, p3, p4 );
}
    
void DllLibass::ass_free_track( ASS_Track * p1 )
{
    ::ass_free_track( p1 );
}
    
void DllLibass::ass_set_fonts( ASS_Renderer* p1, const char* p2, const char* p3, int p4, const char* p5, int p6 )
{
    ::ass_set_fonts( p1, p2, p3, p4, p5, p6 );
}
    
void DllLibass::ass_set_style_overrides( ASS_Library* p1, char** p2 )
{
    ::ass_set_style_overrides( p1, p2 );
}
    
void DllLibass::ass_library_done( ASS_Library* p1 )
{
    ::ass_library_done( p1 );
}
    
void DllLibass::ass_renderer_done( ASS_Renderer* p1 )
{
    ::ass_renderer_done( p1 );
}
    
void DllLibass::ass_process_chunk( ASS_Track* p1, char* p2, int p3, long long p4, long long p5 )
{
    ::ass_process_chunk( p1, p2, p3, p4, p5 );
}
    
void DllLibass::ass_process_codec_private( ASS_Track* p1, char* p2, int p3 )
{
    ::ass_process_codec_private( p1, p2, p3 );
}
    
void DllLibass::ass_set_message_cb( ASS_Library* p1, void( *p2 )( int level, const char *fmt, va_list args, void *data ), void* p3 )
{
    ::ass_set_message_cb( p1, p2, p3 );
}

#endif // USE_STATIC_LIBS