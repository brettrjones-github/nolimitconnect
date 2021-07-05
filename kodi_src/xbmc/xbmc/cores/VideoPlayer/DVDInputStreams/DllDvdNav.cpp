
#include "DllDvdNav.h"
#if USE_STATIC_LIBS
#include "cores/VideoPlayer/DVDInputStreams/dvdnav/dvdnav.h"

dvdnav_status_t DllDvdNav::dvdnav_open( dvdnav_t **dest, const char *path )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_open( dest, path );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_open_stream( dvdnav_t **dest, void *stream, dvdnav_stream_cb *stream_cb )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_open_stream( dest, stream, stream_cb );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_close( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_close( self );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_reset( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_reset( self );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

const char* DllDvdNav::dvdnav_err_to_string( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_err_to_string( self );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_set_readahead_flag( dvdnav_t *self, int32_t read_ahead_flag )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_set_readahead_flag( self, read_ahead_flag );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_set_PGC_positioning_flag( dvdnav_t *self, int32_t pgc_based_flag )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_set_PGC_positioning_flag( self, pgc_based_flag );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_get_next_cache_block( dvdnav_t *self, uint8_t **buf, int32_t *event, int32_t *len )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_next_cache_block( self, buf, event, len );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_free_cache_block( dvdnav_t *self, unsigned char *buf )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_free_cache_block( self, buf );
#else
    return DVDNAV_STATUS_ERR;
#endif  
}

dvdnav_status_t DllDvdNav::dvdnav_still_skip( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_still_skip( self );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_wait_skip( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_wait_skip( self );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_stop( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_stop( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_button_select( dvdnav_t *self, pci_t *pci, int32_t button )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_button_select( self, pci, button );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_button_activate( dvdnav_t *self, pci_t *pci )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_button_activate( self, pci );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_upper_button_select( dvdnav_t *self, pci_t *pci )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_upper_button_select( self, pci );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_lower_button_select( dvdnav_t *self, pci_t *pci )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_lower_button_select( self, pci );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_right_button_select( dvdnav_t *self, pci_t *pci )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_right_button_select( self, pci );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_left_button_select( dvdnav_t *self, pci_t *pci )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_left_button_select( self, pci );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_sector_search( dvdnav_t *self, uint64_t offset, int32_t origin )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_sector_search( self, offset, origin );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

pci_t * DllDvdNav::dvdnav_get_current_nav_pci( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_current_nav_pci( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dsi_t * DllDvdNav::dvdnav_get_current_nav_dsi( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_current_nav_dsi( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_position( dvdnav_t *self, uint32_t *pos, uint32_t *len )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_position( self, pos, len );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_current_title_info( dvdnav_t *self, int32_t *title, int32_t *part )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_current_title_info( self, title, part );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_spu_language_select( dvdnav_t *self, char *code )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_spu_language_select( self, code );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_audio_language_select( dvdnav_t *self, char *code )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_audio_language_select( self, code );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_menu_language_select( dvdnav_t *self, char *code )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_menu_language_select( self, code );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int8_t DllDvdNav::dvdnav_is_domain_vts( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_is_domain_vts( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int8_t DllDvdNav::dvdnav_get_active_spu_stream( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_active_spu_stream( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int8_t DllDvdNav::dvdnav_get_spu_logical_stream( dvdnav_t *self, uint8_t subp_num )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_spu_logical_stream( self, subp_num );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

uint16_t DllDvdNav::dvdnav_spu_stream_to_lang( dvdnav_t *self, uint8_t stream )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_spu_stream_to_lang( self, stream );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_current_highlight( dvdnav_t *self, int32_t *button )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_current_highlight( self, button );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_menu_call( dvdnav_t *self, DVDMenuID_t menu )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_menu_call( self, menu );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_prev_pg_search( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_prev_pg_search( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_next_pg_search( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_next_pg_search( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_highlight_area( pci_t *nav_pci, int32_t button, int32_t mode, dvdnav_highlight_area_t *highlight )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_highlight_area( nav_pci, button, mode, highlight );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_go_up( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_go_up( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int8_t DllDvdNav::dvdnav_get_active_audio_stream( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_active_audio_stream( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

uint16_t DllDvdNav::dvdnav_audio_stream_to_lang( dvdnav_t *self, uint8_t stream )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_audio_stream_to_lang( self, stream );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

vm_t* DllDvdNav::dvdnav_get_vm( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_vm( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int DllDvdNav::dvdnav_get_button_info( dvdnav_t* self, int alpha[ 2 ][ 4 ], int color[ 2 ][ 4 ] )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_button_info( self, alpha, color );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int8_t DllDvdNav::dvdnav_get_audio_logical_stream( dvdnav_t *self, uint8_t audio_num )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_audio_logical_stream( self, audio_num );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_set_region_mask( dvdnav_t *self, int32_t region_mask )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_set_region_mask( self, region_mask );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

uint8_t DllDvdNav::dvdnav_get_video_aspect( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_video_aspect( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

uint8_t DllDvdNav::dvdnav_get_video_scale_permission( dvdnav_t *self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_video_scale_permission( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_number_of_titles( dvdnav_t *self, int32_t *titles )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_number_of_titles( self, titles );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_number_of_parts( dvdnav_t *self, int32_t title, int32_t *parts )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_number_of_parts( self, title, parts );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_title_play( dvdnav_t *self, int32_t title )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_title_play( self, title );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_part_play( dvdnav_t *self, int32_t title, int32_t part )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_part_play( self, title, part );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_audio_attr( dvdnav_t * self, int32_t streamid, audio_attr_t* audio_attributes )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_audio_attr( self, streamid, audio_attributes );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_spu_attr( dvdnav_t * self, int32_t streamid, subp_attr_t* stitle_attributes )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_spu_attr( self, streamid, stitle_attributes );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_time_search( dvdnav_t * self, uint64_t timepos )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_time_search( self, timepos );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t dvdnav_jump_to_sector_by_time( dvdnav_t * self, uint64_t time_in_pts_ticks, int32_t origin );

int64_t DllDvdNav::dvdnav_convert_time( dvd_time_t *time )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_convert_time( time );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_state( dvdnav_t *self, dvd_state_t *save_state )
{
#ifdef HAVE_LIBBLURAY
   return ::dvdnav_get_state( self, save_state );
#else
    return DVDNAV_STATUS_ERR;
#endif 
}

dvdnav_status_t DllDvdNav::dvdnav_set_state( dvdnav_t *self, dvd_state_t *save_state )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_set_state( self, save_state );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_angle_info( dvdnav_t *self, int32_t *current_angle, int32_t *number_of_angles )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_angle_info( self, current_angle, number_of_angles );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t dvdnav_angle_change( dvdnav_t *self, int32_t angle );

dvdnav_status_t DllDvdNav::dvdnav_mouse_activate( dvdnav_t *self, pci_t *pci, int32_t x, int32_t y )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_mouse_activate( self, pci, x, y );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_mouse_select( dvdnav_t *self, pci_t *pci, int32_t x, int32_t y )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_mouse_select( self, pci, x, y );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_title_string( dvdnav_t *self, const char **title_str )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_title_string( self, title_str );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_get_serial_string( dvdnav_t *self, const char **serial_str )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_serial_string( self, serial_str );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

uint32_t DllDvdNav::dvdnav_describe_title_chapters( dvdnav_t* self, uint32_t title, uint64_t** times, uint64_t* duration )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_describe_title_chapters( self, title, times, duration );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

int DllDvdNav::dvdnav_get_video_resolution( dvdnav_t* self, uint32_t* width, uint32_t* height )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_video_resolution( self, width, height );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t DllDvdNav::dvdnav_jump_to_sector_by_time( dvdnav_t * self, uint64_t time_in_pts_ticks, int32_t origin )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_jump_to_sector_by_time( self, time_in_pts_ticks, origin );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

dvdnav_status_t  DllDvdNav::dvdnav_angle_change( dvdnav_t *self, int32_t angle )
{
#ifdef HAVE_LIBBLURAY
    return dvdnav_angle_change( self, angle );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

//extern "C"  int64_t dvdnav_get_current_time( dvdnav_t* self );

int64_t  DllDvdNav::dvdnav_get_current_time( dvdnav_t* self )
{
#ifdef HAVE_LIBBLURAY
    return ::dvdnav_get_current_time( self );
#else
    return DVDNAV_STATUS_ERR;
#endif
}

extern "C"  void dvdnav_free( void * data );

void DllDvdNav::dvdnav_free( void* data )
{
#ifdef HAVE_LIBBLURAY
    ::dvdnav_free( data );
#endif
}

#endif // USE_STATIC_LIBS