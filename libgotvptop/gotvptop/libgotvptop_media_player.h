/*****************************************************************************
 * libgotvptop_media_player.h:  libgotvptop_media_player external API
 *****************************************************************************
 * Copyright (C) 1998-2015 GOTV authors and VideoLAN
 * $Id: 4336df9442b5bae28ec93d540f2ee5907f34e077 $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Paul Saman <jpsaman@videolan.org>
 *          Pierre d'Herbemont <pdherbemont@videolan.org>
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

#ifndef GOTV_LIBGOTV_MEDIA_PLAYER_H
#define GOTV_LIBGOTV_MEDIA_PLAYER_H 1

#include "gotvptop/libgotvptop.h"
#include "gotvptop/libgotvptop_media.h"
#include "gotvptop_common.h"
#include "gotvptop/libgotvptop_renderer_discoverer.h"


# include <stdbool.h>

/** \defgroup libgotvptop_media_player LibGOTV media player
 * \ingroup libgotvptop
 * A LibGOTV media player plays one media (usually in a custom drawable).
 * @{
 * \file
 * LibGOTV simple media player external API
 */

typedef struct libgotvptop_media_player_t libgotvptop_media_player_t;

/**
 * Description for video, audio tracks and subtitles. It contains
 * id, name (description string) and pointer to next record.
 */
typedef struct libgotvptop_track_description_t
{
    int   i_id;
    char *psz_name;
    struct libgotvptop_track_description_t *p_next;

} libgotvptop_track_description_t;

/**
 * Description for titles
 */
enum
{
    libgotvptop_title_menu          = 0x01,
    libgotvptop_title_interactive   = 0x02
};

typedef struct libgotvptop_title_description_t
{
    int64_t i_duration; /**< duration in milliseconds */
    char *psz_name; /**< title name */
    unsigned i_flags; /**< info if item was recognized as a menu, interactive or plain content by the demuxer */
} libgotvptop_title_description_t;

/**
 * Description for chapters
 */
typedef struct libgotvptop_chapter_description_t
{
    int64_t i_time_offset; /**< time-offset of the chapter in milliseconds */
    int64_t i_duration; /**< duration of the chapter in milliseconds */
    char *psz_name; /**< chapter name */
} libgotvptop_chapter_description_t;

/**
 * Description for audio output. It contains
 * name, description and pointer to next record.
 */
typedef struct libgotvptop_audio_output_t
{
    char *psz_name;
    char *psz_description;
    struct libgotvptop_audio_output_t *p_next;

} libgotvptop_audio_output_t;

/**
 * Description for audio output device.
 */
typedef struct libgotvptop_audio_output_device_t
{
    struct libgotvptop_audio_output_device_t *p_next; /**< Next entry in list */
    char *psz_device; /**< Device identifier string */
    char *psz_description; /**< User-friendly device description */
    /* More fields may be added here in later versions */
} libgotvptop_audio_output_device_t;

/**
 * Marq options definition
 */
typedef enum libgotvptop_video_marquee_option_t {
    libgotvptop_marquee_Enable = 0,
    libgotvptop_marquee_Text,                  /** string argument */
    libgotvptop_marquee_Color,
    libgotvptop_marquee_Opacity,
    libgotvptop_marquee_Position,
    libgotvptop_marquee_Refresh,
    libgotvptop_marquee_Size,
    libgotvptop_marquee_Timeout,
    libgotvptop_marquee_X,
    libgotvptop_marquee_Y
} libgotvptop_video_marquee_option_t;

/**
 * Navigation mode
 */
typedef enum libgotvptop_navigate_mode_t
{
    libgotvptop_navigate_activate = 0,
    libgotvptop_navigate_up,
    libgotvptop_navigate_down,
    libgotvptop_navigate_left,
    libgotvptop_navigate_right,
    libgotvptop_navigate_popup
} libgotvptop_navigate_mode_t;

/**
 * Enumeration of values used to set position (e.g. of video title).
 */
typedef enum libgotvptop_position_t {
    libgotvptop_position_disable=-1,
    libgotvptop_position_center,
    libgotvptop_position_left,
    libgotvptop_position_right,
    libgotvptop_position_top,
    libgotvptop_position_top_left,
    libgotvptop_position_top_right,
    libgotvptop_position_bottom,
    libgotvptop_position_bottom_left,
    libgotvptop_position_bottom_right
} libgotvptop_position_t;

/**
 * Enumeration of teletext keys than can be passed via
 * libgotvptop_video_set_teletext()
 */
typedef enum libgotvptop_teletext_key_t {
    libgotvptop_teletext_key_red = 'r' << 16,
    libgotvptop_teletext_key_green = 'g' << 16,
    libgotvptop_teletext_key_yellow = 'y' << 16,
    libgotvptop_teletext_key_blue = 'b' << 16,
    libgotvptop_teletext_key_index = 'i' << 16,
} libgotvptop_teletext_key_t;

/**
 * Opaque equalizer handle.
 *
 * Equalizer settings can be applied to a media player.
 */
typedef struct libgotvptop_equalizer_t libgotvptop_equalizer_t;

/**
 * Create an empty Media Player object
 *
 * \param p_libgotvptop_instance the libgotvptop instance in which the Media Player
 *        should be created.
 * \return a new media player object, or NULL on error.
 */
LIBGOTV_API libgotvptop_media_player_t * libgotvptop_media_player_new( libgotvptop_instance_t *p_libgotvptop_instance );

/**
 * Create a Media Player object from a Media
 *
 * \param p_md the media. Afterwards the p_md can be safely
 *        destroyed.
 * \return a new media player object, or NULL on error.
 */
LIBGOTV_API libgotvptop_media_player_t * libgotvptop_media_player_new_from_media( libgotvptop_media_t *p_md );

/**
 * Release a media_player after use
 * Decrement the reference count of a media player object. If the
 * reference count is 0, then libgotvptop_media_player_release() will
 * release the media player object. If the media player object
 * has been released, then it should not be used again.
 *
 * \param p_mi the Media Player to free
 */
LIBGOTV_API void libgotvptop_media_player_release( libgotvptop_media_player_t *p_mi );

/**
 * Retain a reference to a media player object. Use
 * libgotvptop_media_player_release() to decrement reference count.
 *
 * \param p_mi media player object
 */
LIBGOTV_API void libgotvptop_media_player_retain( libgotvptop_media_player_t *p_mi );

/**
 * Set the media that will be used by the media_player. If any,
 * previous md will be released.
 *
 * \param p_mi the Media Player
 * \param p_md the Media. Afterwards the p_md can be safely
 *        destroyed.
 */
LIBGOTV_API void libgotvptop_media_player_set_media( libgotvptop_media_player_t *p_mi,
                                                   libgotvptop_media_t *p_md );

/**
 * Get the media used by the media_player.
 *
 * \param p_mi the Media Player
 * \return the media associated with p_mi, or NULL if no
 *         media is associated
 */
LIBGOTV_API libgotvptop_media_t * libgotvptop_media_player_get_media( libgotvptop_media_player_t *p_mi );

/**
 * Get the Event Manager from which the media player send event.
 *
 * \param p_mi the Media Player
 * \return the event manager associated with p_mi
 */
LIBGOTV_API libgotvptop_event_manager_t * libgotvptop_media_player_event_manager ( libgotvptop_media_player_t *p_mi );

/**
 * is_playing
 *
 * \param p_mi the Media Player
 * \return 1 if the media player is playing, 0 otherwise
 *
 * \libgotvptop_return_bool
 */
LIBGOTV_API int libgotvptop_media_player_is_playing ( libgotvptop_media_player_t *p_mi );

/**
 * Play
 *
 * \param p_mi the Media Player
 * \return 0 if playback started (and was already started), or -1 on error.
 */
LIBGOTV_API int libgotvptop_media_player_play ( libgotvptop_media_player_t *p_mi );

/**
 * Pause or resume (no effect if there is no media)
 *
 * \param mp the Media Player
 * \param do_pause play/resume if zero, pause if non-zero
 * \version LibGOTV 1.1.1 or later
 */
LIBGOTV_API void libgotvptop_media_player_set_pause ( libgotvptop_media_player_t *mp,
                                                    int do_pause );

/**
 * Toggle pause (no effect if there is no media)
 *
 * \param p_mi the Media Player
 */
LIBGOTV_API void libgotvptop_media_player_pause ( libgotvptop_media_player_t *p_mi );

/**
 * Stop (no effect if there is no media)
 *
 * \param p_mi the Media Player
 */
LIBGOTV_API void libgotvptop_media_player_stop ( libgotvptop_media_player_t *p_mi );

/**
 * Set a renderer to the media player
 *
 * \note must be called before the first call of libgotvptop_media_player_play() to
 * take effect.
 *
 * \see libgotvptop_renderer_discoverer_new
 *
 * \param p_mi the Media Player
 * \param p_item an item discovered by libgotvptop_renderer_discoverer_start()
 * \return 0 on success, -1 on error.
 * \version LibGOTV 3.0.0 or later
 */
LIBGOTV_API int libgotvptop_media_player_set_renderer( libgotvptop_media_player_t *p_mi,
                                                 libgotvptop_renderer_item_t *p_item );

/**
 * Callback prototype to allocate and lock a picture buffer.
 *
 * Whenever a new video frame needs to be decoded, the lock callback is
 * invoked. Depending on the video chroma, one or three pixel planes of
 * adequate dimensions must be returned via the second parameter. Those
 * planes must be aligned on 32-bytes boundaries.
 *
 * \param opaque private pointer as passed to libgotvptop_video_set_callbacks() [IN]
 * \param planes start address of the pixel planes (LibGOTV allocates the array
 *             of void pointers, this callback must initialize the array) [OUT]
 * \return a private pointer for the display and unlock callbacks to identify
 *         the picture buffers
 */
typedef void *(*libgotvptop_video_lock_cb)(void *opaque, void **planes);

/**
 * Callback prototype to unlock a picture buffer.
 *
 * When the video frame decoding is complete, the unlock callback is invoked.
 * This callback might not be needed at all. It is only an indication that the
 * application can now read the pixel values if it needs to.
 *
 * \note A picture buffer is unlocked after the picture is decoded,
 * but before the picture is displayed.
 *
 * \param opaque private pointer as passed to libgotvptop_video_set_callbacks() [IN]
 * \param picture private pointer returned from the @ref libgotvptop_video_lock_cb
 *                callback [IN]
 * \param planes pixel planes as defined by the @ref libgotvptop_video_lock_cb
 *               callback (this parameter is only for convenience) [IN]
 */
typedef void (*libgotvptop_video_unlock_cb)(void *opaque, void *picture,
                                       void *const *planes);

/**
 * Callback prototype to display a picture.
 *
 * When the video frame needs to be shown, as determined by the media playback
 * clock, the display callback is invoked.
 *
 * \param opaque private pointer as passed to libgotvptop_video_set_callbacks() [IN]
 * \param picture private pointer returned from the @ref libgotvptop_video_lock_cb
 *                callback [IN]
 */
typedef void (*libgotvptop_video_display_cb)(void *opaque, void *picture);

/**
 * Callback prototype to configure picture buffers format.
 * This callback gets the format of the video as output by the video decoder
 * and the chain of video filters (if any). It can opt to change any parameter
 * as it needs. In that case, LibGOTV will attempt to convert the video format
 * (rescaling and chroma conversion) but these operations can be CPU intensive.
 *
 * \param opaque pointer to the private pointer passed to
 *               libgotvptop_video_set_callbacks() [IN/OUT]
 * \param chroma pointer to the 4 bytes video format identifier [IN/OUT]
 * \param width pointer to the pixel width [IN/OUT]
 * \param height pointer to the pixel height [IN/OUT]
 * \param pitches table of scanline pitches in bytes for each pixel plane
 *                (the table is allocated by LibGOTV) [OUT]
 * \param lines table of scanlines count for each plane [OUT]
 * \return the number of picture buffers allocated, 0 indicates failure
 *
 * \note
 * For each pixels plane, the scanline pitch must be bigger than or equal to
 * the number of bytes per pixel multiplied by the pixel width.
 * Similarly, the number of scanlines must be bigger than of equal to
 * the pixel height.
 * Furthermore, we recommend that pitches and lines be multiple of 32
 * to not break assumptions that might be held by optimized code
 * in the video decoders, video filters and/or video converters.
 */
typedef unsigned (*libgotvptop_video_format_cb)(void **opaque, char *chroma,
                                           unsigned *width, unsigned *height,
                                           unsigned *pitches,
                                           unsigned *lines);

/**
 * Callback prototype to configure picture buffers format.
 *
 * \param opaque private pointer as passed to libgotvptop_video_set_callbacks()
 *               (and possibly modified by @ref libgotvptop_video_format_cb) [IN]
 */
typedef void (*libgotvptop_video_cleanup_cb)(void *opaque);


/**
 * Set callbacks and private data to render decoded video to a custom area
 * in memory.
 * Use libgotvptop_video_set_format() or libgotvptop_video_set_format_callbacks()
 * to configure the decoded format.
 *
 * \warning Rendering video into custom memory buffers is considerably less
 * efficient than rendering in a custom window as normal.
 *
 * For optimal perfomances, GOTV media player renders into a custom window, and
 * does not use this function and associated callbacks. It is <b>highly
 * recommended</b> that other LibGOTV-based application do likewise.
 * To embed video in a window, use libgotvptop_media_player_set_xid() or equivalent
 * depending on the operating system.
 *
 * If window embedding does not fit the application use case, then a custom
 * LibGOTV video output display plugin is required to maintain optimal video
 * rendering performances.
 *
 * The following limitations affect performance:
 * - Hardware video decoding acceleration will either be disabled completely,
 *   or require (relatively slow) copy from video/DSP memory to main memory.
 * - Sub-pictures (subtitles, on-screen display, etc.) must be blent into the
 *   main picture by the CPU instead of the GPU.
 * - Depending on the video format, pixel format conversion, picture scaling,
 *   cropping and/or picture re-orientation, must be performed by the CPU
 *   instead of the GPU.
 * - Memory copying is required between LibGOTV reference picture buffers and
 *   application buffers (between lock and unlock callbacks).
 *
 * \param mp the media player
 * \param lock callback to lock video memory (must not be NULL)
 * \param unlock callback to unlock video memory (or NULL if not needed)
 * \param display callback to display video (or NULL if not needed)
 * \param opaque private pointer for the three callbacks (as first parameter)
 * \version LibGOTV 1.1.1 or later
 */
LIBGOTV_API
void libgotvptop_video_set_callbacks( libgotvptop_media_player_t *mp,
                                 libgotvptop_video_lock_cb lock,
                                 libgotvptop_video_unlock_cb unlock,
                                 libgotvptop_video_display_cb display,
                                 void *opaque );

/**
 * Set decoded video chroma and dimensions.
 * This only works in combination with libgotvptop_video_set_callbacks(),
 * and is mutually exclusive with libgotvptop_video_set_format_callbacks().
 *
 * \param mp the media player
 * \param chroma a four-characters string identifying the chroma
 *               (e.g. "RV32" or "YUYV")
 * \param width pixel width
 * \param height pixel height
 * \param pitch line pitch (in bytes)
 * \version LibGOTV 1.1.1 or later
 * \bug All pixel planes are expected to have the same pitch.
 * To use the YCbCr color space with chrominance subsampling,
 * consider using libgotvptop_video_set_format_callbacks() instead.
 */
LIBGOTV_API
void libgotvptop_video_set_format( libgotvptop_media_player_t *mp, const char *chroma,
                              unsigned width, unsigned height,
                              unsigned pitch );

/**
 * Set decoded video chroma and dimensions. This only works in combination with
 * libgotvptop_video_set_callbacks().
 *
 * \param mp the media player
 * \param setup callback to select the video format (cannot be NULL)
 * \param cleanup callback to release any allocated resources (or NULL)
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API
void libgotvptop_video_set_format_callbacks( libgotvptop_media_player_t *mp,
                                        libgotvptop_video_format_cb setup,
                                        libgotvptop_video_cleanup_cb cleanup );

/**
 * Set the NSView handler where the media player should render its video output.
 *
 * Use the vout called "macosx".
 *
 * The drawable is an NSObject that follow the GOTVOpenGLVideoViewEmbedding
 * protocol:
 *
 * @code{.m}
 * \@protocol GOTVOpenGLVideoViewEmbedding <NSObject>
 * - (void)addVoutSubview:(NSView *)view;
 * - (void)removeVoutSubview:(NSView *)view;
 * \@end
 * @endcode
 *
 * Or it can be an NSView object.
 *
 * If you want to use it along with Qt see the QMacCocoaViewContainer. Then
 * the following code should work:
 * @code{.mm}
 * {
 *     NSView *video = [[NSView alloc] init];
 *     QMacCocoaViewContainer *container = new QMacCocoaViewContainer(video, parent);
 *     libgotvptop_media_player_set_nsobject(mp, video);
 *     [video release];
 * }
 * @endcode
 *
 * You can find a live example in GOTVVideoView in GOTVKit.framework.
 *
 * \param p_mi the Media Player
 * \param drawable the drawable that is either an NSView or an object following
 * the GOTVOpenGLVideoViewEmbedding protocol.
 */
LIBGOTV_API void libgotvptop_media_player_set_nsobject ( libgotvptop_media_player_t *p_mi, void * drawable );

/**
 * Get the NSView handler previously set with libgotvptop_media_player_set_nsobject().
 *
 * \param p_mi the Media Player
 * \return the NSView handler or 0 if none where set
 */
LIBGOTV_API void * libgotvptop_media_player_get_nsobject ( libgotvptop_media_player_t *p_mi );

/**
 * Set an X Window System drawable where the media player should render its
 * video output. The call takes effect when the playback starts. If it is
 * already started, it might need to be stopped before changes apply.
 * If LibGOTV was built without X11 output support, then this function has no
 * effects.
 *
 * By default, LibGOTV will capture input events on the video rendering area.
 * Use libgotvptop_video_set_mouse_input() and libgotvptop_video_set_key_input() to
 * disable that and deliver events to the parent window / to the application
 * instead. By design, the X11 protocol delivers input events to only one
 * recipient.
 *
 * \warning
 * The application must call the XInitThreads() function from Xlib before
 * libgotvptop_new(), and before any call to XOpenDisplay() directly or via any
 * other library. Failure to call XInitThreads() will seriously impede LibGOTV
 * performance. Calling XOpenDisplay() before XInitThreads() will eventually
 * crash the process. That is a limitation of Xlib.
 *
 * \param p_mi media player
 * \param drawable X11 window ID
 *
 * \note
 * The specified identifier must correspond to an existing Input/Output class
 * X11 window. Pixmaps are <b>not</b> currently supported. The default X11
 * server is assumed, i.e. that specified in the DISPLAY environment variable.
 *
 * \warning
 * LibGOTV can deal with invalid X11 handle errors, however some display drivers
 * (EGL, GLX, VA and/or VDPAU) can unfortunately not. Thus the window handle
 * must remain valid until playback is stopped, otherwise the process may
 * abort or crash.
 *
 * \bug
 * No more than one window handle per media player instance can be specified.
 * If the media has multiple simultaneously active video tracks, extra tracks
 * will be rendered into external windows beyond the control of the
 * application.
 */
LIBGOTV_API void libgotvptop_media_player_set_xwindow(libgotvptop_media_player_t *p_mi,
                                                uint32_t drawable);

/**
 * Get the X Window System window identifier previously set with
 * libgotvptop_media_player_set_xwindow(). Note that this will return the identifier
 * even if GOTV is not currently using it (for instance if it is playing an
 * audio-only input).
 *
 * \param p_mi the Media Player
 * \return an X window ID, or 0 if none where set.
 */
LIBGOTV_API uint32_t libgotvptop_media_player_get_xwindow ( libgotvptop_media_player_t *p_mi );

/**
 * Set a Win32/Win64 API window handle (HWND) where the media player should
 * render its video output. If LibGOTV was built without Win32/Win64 API output
 * support, then this has no effects.
 *
 * \param p_mi the Media Player
 * \param drawable windows handle of the drawable
 */
LIBGOTV_API void libgotvptop_media_player_set_hwnd ( libgotvptop_media_player_t *p_mi, void *drawable );

/**
 * Get the Windows API window handle (HWND) previously set with
 * libgotvptop_media_player_set_hwnd(). The handle will be returned even if LibGOTV
 * is not currently outputting any video to it.
 *
 * \param p_mi the Media Player
 * \return a window handle or NULL if there are none.
 */
LIBGOTV_API void *libgotvptop_media_player_get_hwnd ( libgotvptop_media_player_t *p_mi );

/**
 * Set the android context.
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \param p_mi the media player
 * \param p_awindow_handler org.videolan.libgotvptop.AWindow jobject owned by the
 *        org.videolan.libgotvptop.MediaPlayer class from the libgotvptop-android project.
 */
LIBGOTV_API void libgotvptop_media_player_set_android_context( libgotvptop_media_player_t *p_mi,
                                                         void *p_awindow_handler );

/**
 * Set the EFL Evas Object.
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \param p_mi the media player
 * \param p_evas_object a valid EFL Evas Object (Evas_Object)
 * \return -1 if an error was detected, 0 otherwise.
 */
LIBGOTV_API int libgotvptop_media_player_set_evas_object( libgotvptop_media_player_t *p_mi,
                                                    void *p_evas_object );


/**
 * Callback prototype for audio playback.
 *
 * The LibGOTV media player decodes and post-processes the audio signal
 * asynchronously (in an internal thread). Whenever audio samples are ready
 * to be queued to the output, this callback is invoked.
 *
 * The number of samples provided per invocation may depend on the file format,
 * the audio coding algorithm, the decoder plug-in, the post-processing
 * filters and timing. Application must not assume a certain number of samples.
 *
 * The exact format of audio samples is determined by libgotvptop_audio_set_format()
 * or libgotvptop_audio_set_format_callbacks() as is the channels layout.
 *
 * Note that the number of samples is per channel. For instance, if the audio
 * track sampling rate is 48000 Hz, then 1200 samples represent 25 milliseconds
 * of audio signal - regardless of the number of audio channels.
 *
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 * \param samples pointer to a table of audio samples to play back [IN]
 * \param count number of audio samples to play back
 * \param pts expected play time stamp (see libgotvptop_delay())
 */
typedef void (*libgotvptop_audio_play_cb)(void *data, const void *samples,
                                     unsigned count, int64_t pts);

/**
 * Callback prototype for audio pause.
 *
 * LibGOTV invokes this callback to pause audio playback.
 *
 * \note The pause callback is never called if the audio is already paused.
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 * \param pts time stamp of the pause request (should be elapsed already)
 */
typedef void (*libgotvptop_audio_pause_cb)(void *data, int64_t pts);

/**
 * Callback prototype for audio resumption.
 *
 * LibGOTV invokes this callback to resume audio playback after it was
 * previously paused.
 *
 * \note The resume callback is never called if the audio is not paused.
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 * \param pts time stamp of the resumption request (should be elapsed already)
 */
typedef void (*libgotvptop_audio_resume_cb)(void *data, int64_t pts);

/**
 * Callback prototype for audio buffer flush.
 *
 * LibGOTV invokes this callback if it needs to discard all pending buffers and
 * stop playback as soon as possible. This typically occurs when the media is
 * stopped.
 *
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 */
typedef void (*libgotvptop_audio_flush_cb)(void *data, int64_t pts);

/**
 * Callback prototype for audio buffer drain.
 *
 * LibGOTV may invoke this callback when the decoded audio track is ending.
 * There will be no further decoded samples for the track, but playback should
 * nevertheless continue until all already pending buffers are rendered.
 *
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 */
typedef void (*libgotvptop_audio_drain_cb)(void *data);

/**
 * Callback prototype for audio volume change.
 * \param data data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 * \param volume software volume (1. = nominal, 0. = mute)
 * \param mute muted flag
 */
typedef void (*libgotvptop_audio_set_volume_cb)(void *data,
                                           float volume, bool mute);

/**
 * Sets callbacks and private data for decoded audio.
 *
 * Use libgotvptop_audio_set_format() or libgotvptop_audio_set_format_callbacks()
 * to configure the decoded audio format.
 *
 * \note The audio callbacks override any other audio output mechanism.
 * If the callbacks are set, LibGOTV will <b>not</b> output audio in any way.
 *
 * \param mp the media player
 * \param play callback to play audio samples (must not be NULL)
 * \param pause callback to pause playback (or NULL to ignore)
 * \param resume callback to resume playback (or NULL to ignore)
 * \param flush callback to flush audio buffers (or NULL to ignore)
 * \param drain callback to drain audio buffers (or NULL to ignore)
 * \param opaque private pointer for the audio callbacks (as first parameter)
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API
void libgotvptop_audio_set_callbacks( libgotvptop_media_player_t *mp,
                                 libgotvptop_audio_play_cb play,
                                 libgotvptop_audio_pause_cb pause,
                                 libgotvptop_audio_resume_cb resume,
                                 libgotvptop_audio_flush_cb flush,
                                 libgotvptop_audio_drain_cb drain,
                                 void *opaque );

/**
 * Set callbacks and private data for decoded audio. This only works in
 * combination with libgotvptop_audio_set_callbacks().
 * Use libgotvptop_audio_set_format() or libgotvptop_audio_set_format_callbacks()
 * to configure the decoded audio format.
 *
 * \param mp the media player
 * \param set_volume callback to apply audio volume,
 *                   or NULL to apply volume in software
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API
void libgotvptop_audio_set_volume_callback( libgotvptop_media_player_t *mp,
                                       libgotvptop_audio_set_volume_cb set_volume );

/**
 * Callback prototype to setup the audio playback.
 *
 * This is called when the media player needs to create a new audio output.
 * \param opaque pointer to the data pointer passed to
 *               libgotvptop_audio_set_callbacks() [IN/OUT]
 * \param format 4 bytes sample format [IN/OUT]
 * \param rate sample rate [IN/OUT]
 * \param channels channels count [IN/OUT]
 * \return 0 on success, anything else to skip audio playback
 */
typedef int (*libgotvptop_audio_setup_cb)(void **data, char *format, unsigned *rate,
                                     unsigned *channels);

/**
 * Callback prototype for audio playback cleanup.
 *
 * This is called when the media player no longer needs an audio output.
 * \param opaque data pointer as passed to libgotvptop_audio_set_callbacks() [IN]
 */
typedef void (*libgotvptop_audio_cleanup_cb)(void *data);

/**
 * Sets decoded audio format via callbacks.
 *
 * This only works in combination with libgotvptop_audio_set_callbacks().
 *
 * \param mp the media player
 * \param setup callback to select the audio format (cannot be NULL)
 * \param cleanup callback to release any allocated resources (or NULL)
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API
void libgotvptop_audio_set_format_callbacks( libgotvptop_media_player_t *mp,
                                        libgotvptop_audio_setup_cb setup,
                                        libgotvptop_audio_cleanup_cb cleanup );

/**
 * Sets a fixed decoded audio format.
 *
 * This only works in combination with libgotvptop_audio_set_callbacks(),
 * and is mutually exclusive with libgotvptop_audio_set_format_callbacks().
 *
 * \param mp the media player
 * \param format a four-characters string identifying the sample format
 *               (e.g. "S16N" or "FL32")
 * \param rate sample rate (expressed in Hz)
 * \param channels channels count
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API
void libgotvptop_audio_set_format( libgotvptop_media_player_t *mp, const char *format,
                              unsigned rate, unsigned channels );

/** \bug This might go away ... to be replaced by a broader system */

/**
 * Get the current movie length (in ms).
 *
 * \param p_mi the Media Player
 * \return the movie length (in ms), or -1 if there is no media.
 */
LIBGOTV_API libgotvptop_time_t libgotvptop_media_player_get_length( libgotvptop_media_player_t *p_mi );

/**
 * Get the current movie time (in ms).
 *
 * \param p_mi the Media Player
 * \return the movie time (in ms), or -1 if there is no media.
 */
LIBGOTV_API libgotvptop_time_t libgotvptop_media_player_get_time( libgotvptop_media_player_t *p_mi );

/**
 * Set the movie time (in ms). This has no effect if no media is being played.
 * Not all formats and protocols support this.
 *
 * \param p_mi the Media Player
 * \param i_time the movie time (in ms).
 */
LIBGOTV_API void libgotvptop_media_player_set_time( libgotvptop_media_player_t *p_mi, libgotvptop_time_t i_time );

/**
 * Get movie position as percentage between 0.0 and 1.0.
 *
 * \param p_mi the Media Player
 * \return movie position, or -1. in case of error
 */
LIBGOTV_API float libgotvptop_media_player_get_position( libgotvptop_media_player_t *p_mi );

/**
 * Set movie position as percentage between 0.0 and 1.0.
 * This has no effect if playback is not enabled.
 * This might not work depending on the underlying input format and protocol.
 *
 * \param p_mi the Media Player
 * \param f_pos the position
 */
LIBGOTV_API void libgotvptop_media_player_set_position( libgotvptop_media_player_t *p_mi, float f_pos );

/**
 * Set movie chapter (if applicable).
 *
 * \param p_mi the Media Player
 * \param i_chapter chapter number to play
 */
LIBGOTV_API void libgotvptop_media_player_set_chapter( libgotvptop_media_player_t *p_mi, int i_chapter );

/**
 * Get movie chapter.
 *
 * \param p_mi the Media Player
 * \return chapter number currently playing, or -1 if there is no media.
 */
LIBGOTV_API int libgotvptop_media_player_get_chapter( libgotvptop_media_player_t *p_mi );

/**
 * Get movie chapter count
 *
 * \param p_mi the Media Player
 * \return number of chapters in movie, or -1.
 */
LIBGOTV_API int libgotvptop_media_player_get_chapter_count( libgotvptop_media_player_t *p_mi );

/**
 * Is the player able to play
 *
 * \param p_mi the Media Player
 * \return boolean
 *
 * \libgotvptop_return_bool
 */
LIBGOTV_API int libgotvptop_media_player_will_play( libgotvptop_media_player_t *p_mi );

/**
 * Get title chapter count
 *
 * \param p_mi the Media Player
 * \param i_title title
 * \return number of chapters in title, or -1
 */
LIBGOTV_API int libgotvptop_media_player_get_chapter_count_for_title(
                       libgotvptop_media_player_t *p_mi, int i_title );

/**
 * Set movie title
 *
 * \param p_mi the Media Player
 * \param i_title title number to play
 */
LIBGOTV_API void libgotvptop_media_player_set_title( libgotvptop_media_player_t *p_mi, int i_title );

/**
 * Get movie title
 *
 * \param p_mi the Media Player
 * \return title number currently playing, or -1
 */
LIBGOTV_API int libgotvptop_media_player_get_title( libgotvptop_media_player_t *p_mi );

/**
 * Get movie title count
 *
 * \param p_mi the Media Player
 * \return title number count, or -1
 */
LIBGOTV_API int libgotvptop_media_player_get_title_count( libgotvptop_media_player_t *p_mi );

/**
 * Set previous chapter (if applicable)
 *
 * \param p_mi the Media Player
 */
LIBGOTV_API void libgotvptop_media_player_previous_chapter( libgotvptop_media_player_t *p_mi );

/**
 * Set next chapter (if applicable)
 *
 * \param p_mi the Media Player
 */
LIBGOTV_API void libgotvptop_media_player_next_chapter( libgotvptop_media_player_t *p_mi );

/**
 * Get the requested movie play rate.
 * @warning Depending on the underlying media, the requested rate may be
 * different from the real playback rate.
 *
 * \param p_mi the Media Player
 * \return movie play rate
 */
LIBGOTV_API float libgotvptop_media_player_get_rate( libgotvptop_media_player_t *p_mi );

/**
 * Set movie play rate
 *
 * \param p_mi the Media Player
 * \param rate movie play rate to set
 * \return -1 if an error was detected, 0 otherwise (but even then, it might
 * not actually work depending on the underlying media protocol)
 */
LIBGOTV_API int libgotvptop_media_player_set_rate( libgotvptop_media_player_t *p_mi, float rate );

/**
 * Get current movie state
 *
 * \param p_mi the Media Player
 * \return the current state of the media player (playing, paused, ...) \see libgotvptop_state_t
 */
LIBGOTV_API libgotvptop_state_t libgotvptop_media_player_get_state( libgotvptop_media_player_t *p_mi );

/**
 * How many video outputs does this media player have?
 *
 * \param p_mi the media player
 * \return the number of video outputs
 */
LIBGOTV_API unsigned libgotvptop_media_player_has_vout( libgotvptop_media_player_t *p_mi );

/**
 * Is this media player seekable?
 *
 * \param p_mi the media player
 * \return true if the media player can seek
 *
 * \libgotvptop_return_bool
 */
LIBGOTV_API int libgotvptop_media_player_is_seekable( libgotvptop_media_player_t *p_mi );

/**
 * Can this media player be paused?
 *
 * \param p_mi the media player
 * \return true if the media player can pause
 *
 * \libgotvptop_return_bool
 */
LIBGOTV_API int libgotvptop_media_player_can_pause( libgotvptop_media_player_t *p_mi );

/**
 * Check if the current program is scrambled
 *
 * \param p_mi the media player
 * \return true if the current program is scrambled
 *
 * \libgotvptop_return_bool
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API int libgotvptop_media_player_program_scrambled( libgotvptop_media_player_t *p_mi );

/**
 * Display the next frame (if supported)
 *
 * \param p_mi the media player
 */
LIBGOTV_API void libgotvptop_media_player_next_frame( libgotvptop_media_player_t *p_mi );

/**
 * Navigate through DVD Menu
 *
 * \param p_mi the Media Player
 * \param navigate the Navigation mode
 * \version libGOTV 2.0.0 or later
 */
LIBGOTV_API void libgotvptop_media_player_navigate( libgotvptop_media_player_t* p_mi,
                                              unsigned navigate );

/**
 * Set if, and how, the video title will be shown when media is played.
 *
 * \param p_mi the media player
 * \param position position at which to display the title, or libgotvptop_position_disable to prevent the title from being displayed
 * \param timeout title display timeout in milliseconds (ignored if libgotvptop_position_disable)
 * \version libGOTV 2.1.0 or later
 */
LIBGOTV_API void libgotvptop_media_player_set_video_title_display( libgotvptop_media_player_t *p_mi, libgotvptop_position_t position, unsigned int timeout );

/**
 * Add a slave to the current media player.
 *
 * \note If the player is playing, the slave will be added directly. This call
 * will also update the slave list of the attached libgotvptop_media_t.
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \see libgotvptop_media_slaves_add
 *
 * \param p_mi the media player
 * \param i_type subtitle or audio
 * \param psz_uri Uri of the slave (should contain a valid scheme).
 * \param b_select True if this slave should be selected when it's loaded
 *
 * \return 0 on success, -1 on error.
 */
LIBGOTV_API
int libgotvptop_media_player_add_slave( libgotvptop_media_player_t *p_mi,
                                   libgotvptop_media_slave_type_t i_type,
                                   const char *psz_uri, bool b_select );

/**
 * Release (free) libgotvptop_track_description_t
 *
 * \param p_track_description the structure to release
 */
LIBGOTV_API void libgotvptop_track_description_list_release( libgotvptop_track_description_t *p_track_description );

/** \defgroup libgotvptop_video LibGOTV video controls
 * @{
 */

/**
 * Toggle fullscreen status on non-embedded video outputs.
 *
 * @warning The same limitations applies to this function
 * as to libgotvptop_set_fullscreen().
 *
 * \param p_mi the media player
 */
LIBGOTV_API void libgotvptop_toggle_fullscreen( libgotvptop_media_player_t *p_mi );

/**
 * Enable or disable fullscreen.
 *
 * @warning With most window managers, only a top-level windows can be in
 * full-screen mode. Hence, this function will not operate properly if
 * libgotvptop_media_player_set_xwindow() was used to embed the video in a
 * non-top-level window. In that case, the embedding window must be reparented
 * to the root window <b>before</b> fullscreen mode is enabled. You will want
 * to reparent it back to its normal parent when disabling fullscreen.
 *
 * \param p_mi the media player
 * \param b_fullscreen boolean for fullscreen status
 */
LIBGOTV_API void libgotvptop_set_fullscreen( libgotvptop_media_player_t *p_mi, int b_fullscreen );

/**
 * Get current fullscreen status.
 *
 * \param p_mi the media player
 * \return the fullscreen status (boolean)
 *
 * \libgotvptop_return_bool
 */
LIBGOTV_API int libgotvptop_get_fullscreen( libgotvptop_media_player_t *p_mi );

/**
 * Enable or disable key press events handling, according to the LibGOTV hotkeys
 * configuration. By default and for historical reasons, keyboard events are
 * handled by the LibGOTV video widget.
 *
 * \note On X11, there can be only one subscriber for key press and mouse
 * click events per window. If your application has subscribed to those events
 * for the X window ID of the video widget, then LibGOTV will not be able to
 * handle key presses and mouse clicks in any case.
 *
 * \warning This function is only implemented for X11 and Win32 at the moment.
 *
 * \param p_mi the media player
 * \param on true to handle key press events, false to ignore them.
 */
LIBGOTV_API
void libgotvptop_video_set_key_input( libgotvptop_media_player_t *p_mi, unsigned on );

/**
 * Enable or disable mouse click events handling. By default, those events are
 * handled. This is needed for DVD menus to work, as well as a few video
 * filters such as "puzzle".
 *
 * \see libgotvptop_video_set_key_input().
 *
 * \warning This function is only implemented for X11 and Win32 at the moment.
 *
 * \param p_mi the media player
 * \param on true to handle mouse click events, false to ignore them.
 */
LIBGOTV_API
void libgotvptop_video_set_mouse_input( libgotvptop_media_player_t *p_mi, unsigned on );

/**
 * Get the pixel dimensions of a video.
 *
 * \param p_mi media player
 * \param num number of the video (starting from, and most commonly 0)
 * \param px pointer to get the pixel width [OUT]
 * \param py pointer to get the pixel height [OUT]
 * \return 0 on success, -1 if the specified video does not exist
 */
LIBGOTV_API
int libgotvptop_video_get_size( libgotvptop_media_player_t *p_mi, unsigned num,
                           unsigned *px, unsigned *py );

/**
 * Get the mouse pointer coordinates over a video.
 * Coordinates are expressed in terms of the decoded video resolution,
 * <b>not</b> in terms of pixels on the screen/viewport (to get the latter,
 * you can query your windowing system directly).
 *
 * Either of the coordinates may be negative or larger than the corresponding
 * dimension of the video, if the cursor is outside the rendering area.
 *
 * @warning The coordinates may be out-of-date if the pointer is not located
 * on the video rendering area. LibGOTV does not track the pointer if it is
 * outside of the video widget.
 *
 * @note LibGOTV does not support multiple pointers (it does of course support
 * multiple input devices sharing the same pointer) at the moment.
 *
 * \param p_mi media player
 * \param num number of the video (starting from, and most commonly 0)
 * \param px pointer to get the abscissa [OUT]
 * \param py pointer to get the ordinate [OUT]
 * \return 0 on success, -1 if the specified video does not exist
 */
LIBGOTV_API
int libgotvptop_video_get_cursor( libgotvptop_media_player_t *p_mi, unsigned num,
                             int *px, int *py );

/**
 * Get the current video scaling factor.
 * See also libgotvptop_video_set_scale().
 *
 * \param p_mi the media player
 * \return the currently configured zoom factor, or 0. if the video is set
 * to fit to the output window/drawable automatically.
 */
LIBGOTV_API float libgotvptop_video_get_scale( libgotvptop_media_player_t *p_mi );

/**
 * Set the video scaling factor. That is the ratio of the number of pixels on
 * screen to the number of pixels in the original decoded video in each
 * dimension. Zero is a special value; it will adjust the video to the output
 * window/drawable (in windowed mode) or the entire screen.
 *
 * Note that not all video outputs support scaling.
 *
 * \param p_mi the media player
 * \param f_factor the scaling factor, or zero
 */
LIBGOTV_API void libgotvptop_video_set_scale( libgotvptop_media_player_t *p_mi, float f_factor );

/**
 * Get current video aspect ratio.
 *
 * \param p_mi the media player
 * \return the video aspect ratio or NULL if unspecified
 * (the result must be released with free() or libgotvptop_free()).
 */
LIBGOTV_API char *libgotvptop_video_get_aspect_ratio( libgotvptop_media_player_t *p_mi );

/**
 * Set new video aspect ratio.
 *
 * \param p_mi the media player
 * \param psz_aspect new video aspect-ratio or NULL to reset to default
 * \note Invalid aspect ratios are ignored.
 */
LIBGOTV_API void libgotvptop_video_set_aspect_ratio( libgotvptop_media_player_t *p_mi, const char *psz_aspect );

/**
 * Create a video viewpoint structure.
 *
 * \version LibGOTV 3.0.0 and later
 *
 * \return video viewpoint or NULL
 *         (the result must be released with free() or libgotvptop_free()).
 */
LIBGOTV_API libgotvptop_video_viewpoint_t *libgotvptop_video_new_viewpoint(void);

/**
 * Update the video viewpoint information.
 *
 * \note It is safe to call this function before the media player is started.
 *
 * \version LibGOTV 3.0.0 and later
 *
 * \param p_mi the media player
 * \param p_viewpoint video viewpoint allocated via libgotvptop_video_new_viewpoint()
 * \param b_absolute if true replace the old viewpoint with the new one. If
 * false, increase/decrease it.
 * \return -1 in case of error, 0 otherwise
 *
 * \note the values are set asynchronously, it will be used by the next frame displayed.
 */
LIBGOTV_API int libgotvptop_video_update_viewpoint( libgotvptop_media_player_t *p_mi,
                                              const libgotvptop_video_viewpoint_t *p_viewpoint,
                                              bool b_absolute);

/**
 * Get current video subtitle.
 *
 * \param p_mi the media player
 * \return the video subtitle selected, or -1 if none
 */
LIBGOTV_API int libgotvptop_video_get_spu( libgotvptop_media_player_t *p_mi );

/**
 * Get the number of available video subtitles.
 *
 * \param p_mi the media player
 * \return the number of available video subtitles
 */
LIBGOTV_API int libgotvptop_video_get_spu_count( libgotvptop_media_player_t *p_mi );

/**
 * Get the description of available video subtitles.
 *
 * \param p_mi the media player
 * \return list containing description of available video subtitles.
 * It must be freed with libgotvptop_track_description_list_release()
 */
LIBGOTV_API libgotvptop_track_description_t *
        libgotvptop_video_get_spu_description( libgotvptop_media_player_t *p_mi );

/**
 * Set new video subtitle.
 *
 * \param p_mi the media player
 * \param i_spu video subtitle track to select (i_id from track description)
 * \return 0 on success, -1 if out of range
 */
LIBGOTV_API int libgotvptop_video_set_spu( libgotvptop_media_player_t *p_mi, int i_spu );

/**
 * Get the current subtitle delay. Positive values means subtitles are being
 * displayed later, negative values earlier.
 *
 * \param p_mi media player
 * \return time (in microseconds) the display of subtitles is being delayed
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API int64_t libgotvptop_video_get_spu_delay( libgotvptop_media_player_t *p_mi );

/**
 * Set the subtitle delay. This affects the timing of when the subtitle will
 * be displayed. Positive values result in subtitles being displayed later,
 * while negative values will result in subtitles being displayed earlier.
 *
 * The subtitle delay will be reset to zero each time the media changes.
 *
 * \param p_mi media player
 * \param i_delay time (in microseconds) the display of subtitles should be delayed
 * \return 0 on success, -1 on error
 * \version LibGOTV 2.0.0 or later
 */
LIBGOTV_API int libgotvptop_video_set_spu_delay( libgotvptop_media_player_t *p_mi, int64_t i_delay );

/**
 * Get the full description of available titles
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \param p_mi the media player
 * \param titles address to store an allocated array of title descriptions
 *        descriptions (must be freed with libgotvptop_title_descriptions_release()
 *        by the caller) [OUT]
 *
 * \return the number of titles (-1 on error)
 */
LIBGOTV_API int libgotvptop_media_player_get_full_title_descriptions( libgotvptop_media_player_t *p_mi,
                                                                libgotvptop_title_description_t ***titles );

/**
 * Release a title description
 *
 * \version LibGOTV 3.0.0 and later
 *
 * \param p_titles title description array to release
 * \param i_count number of title descriptions to release
 */
LIBGOTV_API
    void libgotvptop_title_descriptions_release( libgotvptop_title_description_t **p_titles,
                                            unsigned i_count );

/**
 * Get the full description of available chapters
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \param p_mi the media player
 * \param i_chapters_of_title index of the title to query for chapters (uses current title if set to -1)
 * \param pp_chapters address to store an allocated array of chapter descriptions
 *        descriptions (must be freed with libgotvptop_chapter_descriptions_release()
 *        by the caller) [OUT]
 *
 * \return the number of chapters (-1 on error)
 */
LIBGOTV_API int libgotvptop_media_player_get_full_chapter_descriptions( libgotvptop_media_player_t *p_mi,
                                                                  int i_chapters_of_title,
                                                                  libgotvptop_chapter_description_t *** pp_chapters );

/**
 * Release a chapter description
 *
 * \version LibGOTV 3.0.0 and later
 *
 * \param p_chapters chapter description array to release
 * \param i_count number of chapter descriptions to release
 */
LIBGOTV_API
void libgotvptop_chapter_descriptions_release( libgotvptop_chapter_description_t **p_chapters,
                                          unsigned i_count );

/**
 * Get current crop filter geometry.
 *
 * \param p_mi the media player
 * \return the crop filter geometry or NULL if unset
 */
LIBGOTV_API char *libgotvptop_video_get_crop_geometry( libgotvptop_media_player_t *p_mi );

/**
 * Set new crop filter geometry.
 *
 * \param p_mi the media player
 * \param psz_geometry new crop filter geometry (NULL to unset)
 */
LIBGOTV_API
void libgotvptop_video_set_crop_geometry( libgotvptop_media_player_t *p_mi, const char *psz_geometry );

/**
 * Get current teletext page requested or 0 if it's disabled.
 *
 * Teletext is disabled by default, call libgotvptop_video_set_teletext() to enable
 * it.
 *
 * \param p_mi the media player
 * \return the current teletext page requested.
 */
LIBGOTV_API int libgotvptop_video_get_teletext( libgotvptop_media_player_t *p_mi );

/**
 * Set new teletext page to retrieve.
 *
 * This function can also be used to send a teletext key.
 *
 * \param p_mi the media player
 * \param i_page teletex page number requested. This value can be 0 to disable
 * teletext, a number in the range ]0;1000[ to show the requested page, or a
 * \ref libgotvptop_teletext_key_t. 100 is the default teletext page.
 */
LIBGOTV_API void libgotvptop_video_set_teletext( libgotvptop_media_player_t *p_mi, int i_page );

/**
 * Get number of available video tracks.
 *
 * \param p_mi media player
 * \return the number of available video tracks (int)
 */
LIBGOTV_API int libgotvptop_video_get_track_count( libgotvptop_media_player_t *p_mi );

/**
 * Get the description of available video tracks.
 *
 * \param p_mi media player
 * \return list with description of available video tracks, or NULL on error.
 * It must be freed with libgotvptop_track_description_list_release()
 */
LIBGOTV_API libgotvptop_track_description_t *
        libgotvptop_video_get_track_description( libgotvptop_media_player_t *p_mi );

/**
 * Get current video track.
 *
 * \param p_mi media player
 * \return the video track ID (int) or -1 if no active input
 */
LIBGOTV_API int libgotvptop_video_get_track( libgotvptop_media_player_t *p_mi );

/**
 * Set video track.
 *
 * \param p_mi media player
 * \param i_track the track ID (i_id field from track description)
 * \return 0 on success, -1 if out of range
 */
LIBGOTV_API
int libgotvptop_video_set_track( libgotvptop_media_player_t *p_mi, int i_track );

/**
 * Take a snapshot of the current video window.
 *
 * If i_width AND i_height is 0, original size is used.
 * If i_width XOR i_height is 0, original aspect-ratio is preserved.
 *
 * \param p_mi media player instance
 * \param num number of video output (typically 0 for the first/only one)
 * \param psz_filepath the path of a file or a folder to save the screenshot into
 * \param i_width the snapshot's width
 * \param i_height the snapshot's height
 * \return 0 on success, -1 if the video was not found
 */
LIBGOTV_API
int libgotvptop_video_take_snapshot( libgotvptop_media_player_t *p_mi, unsigned num,
                                const char *psz_filepath, unsigned int i_width,
                                unsigned int i_height );

/**
 * Enable or disable deinterlace filter
 *
 * \param p_mi libgotvptop media player
 * \param psz_mode type of deinterlace filter, NULL to disable
 */
LIBGOTV_API void libgotvptop_video_set_deinterlace( libgotvptop_media_player_t *p_mi,
                                                  const char *psz_mode );

/**
 * Get an integer marquee option value
 *
 * \param p_mi libgotvptop media player
 * \param option marq option to get \see libgotvptop_video_marquee_int_option_t
 */
LIBGOTV_API int libgotvptop_video_get_marquee_int( libgotvptop_media_player_t *p_mi,
                                                 unsigned option );

/**
 * Get a string marquee option value
 *
 * \param p_mi libgotvptop media player
 * \param option marq option to get \see libgotvptop_video_marquee_string_option_t
 */
LIBGOTV_API char *libgotvptop_video_get_marquee_string( libgotvptop_media_player_t *p_mi,
                                                      unsigned option );

/**
 * Enable, disable or set an integer marquee option
 *
 * Setting libgotvptop_marquee_Enable has the side effect of enabling (arg !0)
 * or disabling (arg 0) the marq filter.
 *
 * \param p_mi libgotvptop media player
 * \param option marq option to set \see libgotvptop_video_marquee_int_option_t
 * \param i_val marq option value
 */
LIBGOTV_API void libgotvptop_video_set_marquee_int( libgotvptop_media_player_t *p_mi,
                                                  unsigned option, int i_val );

/**
 * Set a marquee string option
 *
 * \param p_mi libgotvptop media player
 * \param option marq option to set \see libgotvptop_video_marquee_string_option_t
 * \param psz_text marq option value
 */
LIBGOTV_API void libgotvptop_video_set_marquee_string( libgotvptop_media_player_t *p_mi,
                                                     unsigned option, const char *psz_text );

/** option values for libgotvptop_video_{get,set}_logo_{int,string} */
enum libgotvptop_video_logo_option_t {
    libgotvptop_logo_enable,
    libgotvptop_logo_file,           /**< string argument, "file,d,t;file,d,t;..." */
    libgotvptop_logo_x,
    libgotvptop_logo_y,
    libgotvptop_logo_delay,
    libgotvptop_logo_repeat,
    libgotvptop_logo_opacity,
    libgotvptop_logo_position
};

/**
 * Get integer logo option.
 *
 * \param p_mi libgotvptop media player instance
 * \param option logo option to get, values of libgotvptop_video_logo_option_t
 */
LIBGOTV_API int libgotvptop_video_get_logo_int( libgotvptop_media_player_t *p_mi,
                                              unsigned option );

/**
 * Set logo option as integer. Options that take a different type value
 * are ignored.
 * Passing libgotvptop_logo_enable as option value has the side effect of
 * starting (arg !0) or stopping (arg 0) the logo filter.
 *
 * \param p_mi libgotvptop media player instance
 * \param option logo option to set, values of libgotvptop_video_logo_option_t
 * \param value logo option value
 */
LIBGOTV_API void libgotvptop_video_set_logo_int( libgotvptop_media_player_t *p_mi,
                                               unsigned option, int value );

/**
 * Set logo option as string. Options that take a different type value
 * are ignored.
 *
 * \param p_mi libgotvptop media player instance
 * \param option logo option to set, values of libgotvptop_video_logo_option_t
 * \param psz_value logo option value
 */
LIBGOTV_API void libgotvptop_video_set_logo_string( libgotvptop_media_player_t *p_mi,
                                      unsigned option, const char *psz_value );


/** option values for libgotvptop_video_{get,set}_adjust_{int,float,bool} */
enum libgotvptop_video_adjust_option_t {
    libgotvptop_adjust_Enable = 0,
    libgotvptop_adjust_Contrast,
    libgotvptop_adjust_Brightness,
    libgotvptop_adjust_Hue,
    libgotvptop_adjust_Saturation,
    libgotvptop_adjust_Gamma
};

/**
 * Get integer adjust option.
 *
 * \param p_mi libgotvptop media player instance
 * \param option adjust option to get, values of libgotvptop_video_adjust_option_t
 * \version LibGOTV 1.1.1 and later.
 */
LIBGOTV_API int libgotvptop_video_get_adjust_int( libgotvptop_media_player_t *p_mi,
                                                unsigned option );

/**
 * Set adjust option as integer. Options that take a different type value
 * are ignored.
 * Passing libgotvptop_adjust_enable as option value has the side effect of
 * starting (arg !0) or stopping (arg 0) the adjust filter.
 *
 * \param p_mi libgotvptop media player instance
 * \param option adust option to set, values of libgotvptop_video_adjust_option_t
 * \param value adjust option value
 * \version LibGOTV 1.1.1 and later.
 */
LIBGOTV_API void libgotvptop_video_set_adjust_int( libgotvptop_media_player_t *p_mi,
                                                 unsigned option, int value );

/**
 * Get float adjust option.
 *
 * \param p_mi libgotvptop media player instance
 * \param option adjust option to get, values of libgotvptop_video_adjust_option_t
 * \version LibGOTV 1.1.1 and later.
 */
LIBGOTV_API float libgotvptop_video_get_adjust_float( libgotvptop_media_player_t *p_mi,
                                                    unsigned option );

/**
 * Set adjust option as float. Options that take a different type value
 * are ignored.
 *
 * \param p_mi libgotvptop media player instance
 * \param option adust option to set, values of libgotvptop_video_adjust_option_t
 * \param value adjust option value
 * \version LibGOTV 1.1.1 and later.
 */
LIBGOTV_API void libgotvptop_video_set_adjust_float( libgotvptop_media_player_t *p_mi,
                                                   unsigned option, float value );

/** @} video */

/** \defgroup libgotvptop_audio LibGOTV audio controls
 * @{
 */

/**
 * Audio device types
 */
typedef enum libgotvptop_audio_output_device_types_t {
    libgotvptop_AudioOutputDevice_Error  = -1,
    libgotvptop_AudioOutputDevice_Mono   =  1,
    libgotvptop_AudioOutputDevice_Stereo =  2,
    libgotvptop_AudioOutputDevice_2F2R   =  4,
    libgotvptop_AudioOutputDevice_3F2R   =  5,
    libgotvptop_AudioOutputDevice_5_1    =  6,
    libgotvptop_AudioOutputDevice_6_1    =  7,
    libgotvptop_AudioOutputDevice_7_1    =  8,
    libgotvptop_AudioOutputDevice_SPDIF  = 10
} libgotvptop_audio_output_device_types_t;

/**
 * Audio channels
 */
typedef enum libgotvptop_audio_output_channel_t {
    libgotvptop_AudioChannel_Error   = -1,
    libgotvptop_AudioChannel_Stereo  =  1,
    libgotvptop_AudioChannel_RStereo =  2,
    libgotvptop_AudioChannel_Left    =  3,
    libgotvptop_AudioChannel_Right   =  4,
    libgotvptop_AudioChannel_Dolbys  =  5
} libgotvptop_audio_output_channel_t;


/**
 * Gets the list of available audio output modules.
 *
 * \param p_instance libgotvptop instance
 * \return list of available audio outputs. It must be freed with
*          \see libgotvptop_audio_output_list_release \see libgotvptop_audio_output_t .
 *         In case of error, NULL is returned.
 */
LIBGOTV_API libgotvptop_audio_output_t *
libgotvptop_audio_output_list_get( libgotvptop_instance_t *p_instance );

/**
 * Frees the list of available audio output modules.
 *
 * \param p_list list with audio outputs for release
 */
LIBGOTV_API
void libgotvptop_audio_output_list_release( libgotvptop_audio_output_t *p_list );

/**
 * Selects an audio output module.
 * \note Any change will take be effect only after playback is stopped and
 * restarted. Audio output cannot be changed while playing.
 *
 * \param p_mi media player
 * \param psz_name name of audio output,
 *               use psz_name of \see libgotvptop_audio_output_t
 * \return 0 if function succeeded, -1 on error
 */
LIBGOTV_API int libgotvptop_audio_output_set( libgotvptop_media_player_t *p_mi,
                                        const char *psz_name );

/**
 * Gets a list of potential audio output devices,
 * \see libgotvptop_audio_output_device_set().
 *
 * \note Not all audio outputs support enumerating devices.
 * The audio output may be functional even if the list is empty (NULL).
 *
 * \note The list may not be exhaustive.
 *
 * \warning Some audio output devices in the list might not actually work in
 * some circumstances. By default, it is recommended to not specify any
 * explicit audio device.
 *
 * \param mp media player
 * \return A NULL-terminated linked list of potential audio output devices.
 * It must be freed with libgotvptop_audio_output_device_list_release()
 * \version LibGOTV 2.2.0 or later.
 */
LIBGOTV_API libgotvptop_audio_output_device_t *
libgotvptop_audio_output_device_enum( libgotvptop_media_player_t *mp );

/**
 * Gets a list of audio output devices for a given audio output module,
 * \see libgotvptop_audio_output_device_set().
 *
 * \note Not all audio outputs support this. In particular, an empty (NULL)
 * list of devices does <b>not</b> imply that the specified audio output does
 * not work.
 *
 * \note The list might not be exhaustive.
 *
 * \warning Some audio output devices in the list might not actually work in
 * some circumstances. By default, it is recommended to not specify any
 * explicit audio device.
 *
 * \param p_instance libgotvptop instance
 * \param aout audio output name
 *                 (as returned by libgotvptop_audio_output_list_get())
 * \return A NULL-terminated linked list of potential audio output devices.
 * It must be freed with libgotvptop_audio_output_device_list_release()
 * \version LibGOTV 2.1.0 or later.
 */
LIBGOTV_API libgotvptop_audio_output_device_t *
libgotvptop_audio_output_device_list_get( libgotvptop_instance_t *p_instance,
                                     const char *aout );

/**
 * Frees a list of available audio output devices.
 *
 * \param p_list list with audio outputs for release
 * \version LibGOTV 2.1.0 or later.
 */
LIBGOTV_API void libgotvptop_audio_output_device_list_release(
                                        libgotvptop_audio_output_device_t *p_list );

/**
 * Configures an explicit audio output device.
 *
 * If the module paramater is NULL, audio output will be moved to the device
 * specified by the device identifier string immediately. This is the
 * recommended usage.
 *
 * A list of adequate potential device strings can be obtained with
 * libgotvptop_audio_output_device_enum().
 *
 * However passing NULL is supported in LibGOTV version 2.2.0 and later only;
 * in earlier versions, this function would have no effects when the module
 * parameter was NULL.
 *
 * If the module parameter is not NULL, the device parameter of the
 * corresponding audio output, if it exists, will be set to the specified
 * string. Note that some audio output modules do not have such a parameter
 * (notably MMDevice and PulseAudio).
 *
 * A list of adequate potential device strings can be obtained with
 * libgotvptop_audio_output_device_list_get().
 *
 * \note This function does not select the specified audio output plugin.
 * libgotvptop_audio_output_set() is used for that purpose.
 *
 * \warning The syntax for the device parameter depends on the audio output.
 *
 * Some audio output modules require further parameters (e.g. a channels map
 * in the case of ALSA).
 *
 * \param mp media player
 * \param module If NULL, current audio output module.
 *               if non-NULL, name of audio output module
                 (\see libgotvptop_audio_output_t)
 * \param device_id device identifier string
 * \return Nothing. Errors are ignored (this is a design bug).
 */
LIBGOTV_API void libgotvptop_audio_output_device_set( libgotvptop_media_player_t *mp,
                                                const char *module,
                                                const char *device_id );

/**
 * Get the current audio output device identifier.
 *
 * This complements libgotvptop_audio_output_device_set().
 *
 * \warning The initial value for the current audio output device identifier
 * may not be set or may be some unknown value. A LibGOTV application should
 * compare this value against the known device identifiers (e.g. those that
 * were previously retrieved by a call to libgotvptop_audio_output_device_enum or
 * libgotvptop_audio_output_device_list_get) to find the current audio output device.
 *
 * It is possible that the selected audio output device changes (an external
 * change) without a call to libgotvptop_audio_output_device_set. That may make this
 * method unsuitable to use if a LibGOTV application is attempting to track
 * dynamic audio device changes as they happen.
 *
 * \param mp media player
 * \return the current audio output device identifier
 *         NULL if no device is selected or in case of error
 *         (the result must be released with free() or libgotvptop_free()).
 * \version LibGOTV 3.0.0 or later.
 */
LIBGOTV_API char *libgotvptop_audio_output_device_get( libgotvptop_media_player_t *mp );

/**
 * Toggle mute status.
 *
 * \param p_mi media player
 * \warning Toggling mute atomically is not always possible: On some platforms,
 * other processes can mute the GOTV audio playback stream asynchronously. Thus,
 * there is a small race condition where toggling will not work.
 * See also the limitations of libgotvptop_audio_set_mute().
 */
LIBGOTV_API void libgotvptop_audio_toggle_mute( libgotvptop_media_player_t *p_mi );

/**
 * Get current mute status.
 *
 * \param p_mi media player
 * \return the mute status (boolean) if defined, -1 if undefined/unapplicable
 */
LIBGOTV_API int libgotvptop_audio_get_mute( libgotvptop_media_player_t *p_mi );

/**
 * Set mute status.
 *
 * \param p_mi media player
 * \param status If status is true then mute, otherwise unmute
 * \warning This function does not always work. If there are no active audio
 * playback stream, the mute status might not be available. If digital
 * pass-through (S/PDIF, HDMI...) is in use, muting may be unapplicable. Also
 * some audio output plugins do not support muting at all.
 * \note To force silent playback, disable all audio tracks. This is more
 * efficient and reliable than mute.
 */
LIBGOTV_API void libgotvptop_audio_set_mute( libgotvptop_media_player_t *p_mi, int status );

/**
 * Get current software audio volume.
 *
 * \param p_mi media player
 * \return the software volume in percents
 * (0 = mute, 100 = nominal / 0dB)
 */
LIBGOTV_API int libgotvptop_audio_get_volume( libgotvptop_media_player_t *p_mi );

/**
 * Set current software audio volume.
 *
 * \param p_mi media player
 * \param i_volume the volume in percents (0 = mute, 100 = 0dB)
 * \return 0 if the volume was set, -1 if it was out of range
 */
LIBGOTV_API int libgotvptop_audio_set_volume( libgotvptop_media_player_t *p_mi, int i_volume );

/**
 * Get number of available audio tracks.
 *
 * \param p_mi media player
 * \return the number of available audio tracks (int), or -1 if unavailable
 */
LIBGOTV_API int libgotvptop_audio_get_track_count( libgotvptop_media_player_t *p_mi );

/**
 * Get the description of available audio tracks.
 *
 * \param p_mi media player
 * \return list with description of available audio tracks, or NULL.
 * It must be freed with libgotvptop_track_description_list_release()
 */
LIBGOTV_API libgotvptop_track_description_t *
        libgotvptop_audio_get_track_description( libgotvptop_media_player_t *p_mi );

/**
 * Get current audio track.
 *
 * \param p_mi media player
 * \return the audio track ID or -1 if no active input.
 */
LIBGOTV_API int libgotvptop_audio_get_track( libgotvptop_media_player_t *p_mi );

/**
 * Set current audio track.
 *
 * \param p_mi media player
 * \param i_track the track ID (i_id field from track description)
 * \return 0 on success, -1 on error
 */
LIBGOTV_API int libgotvptop_audio_set_track( libgotvptop_media_player_t *p_mi, int i_track );

/**
 * Get current audio channel.
 *
 * \param p_mi media player
 * \return the audio channel \see libgotvptop_audio_output_channel_t
 */
LIBGOTV_API int libgotvptop_audio_get_channel( libgotvptop_media_player_t *p_mi );

/**
 * Set current audio channel.
 *
 * \param p_mi media player
 * \param channel the audio channel, \see libgotvptop_audio_output_channel_t
 * \return 0 on success, -1 on error
 */
LIBGOTV_API int libgotvptop_audio_set_channel( libgotvptop_media_player_t *p_mi, int channel );

/**
 * Get current audio delay.
 *
 * \param p_mi media player
 * \return the audio delay (microseconds)
 * \version LibGOTV 1.1.1 or later
 */
LIBGOTV_API int64_t libgotvptop_audio_get_delay( libgotvptop_media_player_t *p_mi );

/**
 * Set current audio delay. The audio delay will be reset to zero each time the media changes.
 *
 * \param p_mi media player
 * \param i_delay the audio delay (microseconds)
 * \return 0 on success, -1 on error
 * \version LibGOTV 1.1.1 or later
 */
LIBGOTV_API int libgotvptop_audio_set_delay( libgotvptop_media_player_t *p_mi, int64_t i_delay );

/**
 * Get the number of equalizer presets.
 *
 * \return number of presets
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API unsigned libgotvptop_audio_equalizer_get_preset_count( void );

/**
 * Get the name of a particular equalizer preset.
 *
 * This name can be used, for example, to prepare a preset label or menu in a user
 * interface.
 *
 * \param u_index index of the preset, counting from zero
 * \return preset name, or NULL if there is no such preset
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API const char *libgotvptop_audio_equalizer_get_preset_name( unsigned u_index );

/**
 * Get the number of distinct frequency bands for an equalizer.
 *
 * \return number of frequency bands
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API unsigned libgotvptop_audio_equalizer_get_band_count( void );

/**
 * Get a particular equalizer band frequency.
 *
 * This value can be used, for example, to create a label for an equalizer band control
 * in a user interface.
 *
 * \param u_index index of the band, counting from zero
 * \return equalizer band frequency (Hz), or -1 if there is no such band
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API float libgotvptop_audio_equalizer_get_band_frequency( unsigned u_index );

/**
 * Create a new default equalizer, with all frequency values zeroed.
 *
 * The new equalizer can subsequently be applied to a media player by invoking
 * libgotvptop_media_player_set_equalizer().
 *
 * The returned handle should be freed via libgotvptop_audio_equalizer_release() when
 * it is no longer needed.
 *
 * \return opaque equalizer handle, or NULL on error
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API libgotvptop_equalizer_t *libgotvptop_audio_equalizer_new( void );

/**
 * Create a new equalizer, with initial frequency values copied from an existing
 * preset.
 *
 * The new equalizer can subsequently be applied to a media player by invoking
 * libgotvptop_media_player_set_equalizer().
 *
 * The returned handle should be freed via libgotvptop_audio_equalizer_release() when
 * it is no longer needed.
 *
 * \param u_index index of the preset, counting from zero
 * \return opaque equalizer handle, or NULL on error
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API libgotvptop_equalizer_t *libgotvptop_audio_equalizer_new_from_preset( unsigned u_index );

/**
 * Release a previously created equalizer instance.
 *
 * The equalizer was previously created by using libgotvptop_audio_equalizer_new() or
 * libgotvptop_audio_equalizer_new_from_preset().
 *
 * It is safe to invoke this method with a NULL p_equalizer parameter for no effect.
 *
 * \param p_equalizer opaque equalizer handle, or NULL
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API void libgotvptop_audio_equalizer_release( libgotvptop_equalizer_t *p_equalizer );

/**
 * Set a new pre-amplification value for an equalizer.
 *
 * The new equalizer settings are subsequently applied to a media player by invoking
 * libgotvptop_media_player_set_equalizer().
 *
 * The supplied amplification value will be clamped to the -20.0 to +20.0 range.
 *
 * \param p_equalizer valid equalizer handle, must not be NULL
 * \param f_preamp preamp value (-20.0 to 20.0 Hz)
 * \return zero on success, -1 on error
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API int libgotvptop_audio_equalizer_set_preamp( libgotvptop_equalizer_t *p_equalizer, float f_preamp );

/**
 * Get the current pre-amplification value from an equalizer.
 *
 * \param p_equalizer valid equalizer handle, must not be NULL
 * \return preamp value (Hz)
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API float libgotvptop_audio_equalizer_get_preamp( libgotvptop_equalizer_t *p_equalizer );

/**
 * Set a new amplification value for a particular equalizer frequency band.
 *
 * The new equalizer settings are subsequently applied to a media player by invoking
 * libgotvptop_media_player_set_equalizer().
 *
 * The supplied amplification value will be clamped to the -20.0 to +20.0 range.
 *
 * \param p_equalizer valid equalizer handle, must not be NULL
 * \param f_amp amplification value (-20.0 to 20.0 Hz)
 * \param u_band index, counting from zero, of the frequency band to set
 * \return zero on success, -1 on error
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API int libgotvptop_audio_equalizer_set_amp_at_index( libgotvptop_equalizer_t *p_equalizer, float f_amp, unsigned u_band );

/**
 * Get the amplification value for a particular equalizer frequency band.
 *
 * \param p_equalizer valid equalizer handle, must not be NULL
 * \param u_band index, counting from zero, of the frequency band to get
 * \return amplification value (Hz); NaN if there is no such frequency band
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API float libgotvptop_audio_equalizer_get_amp_at_index( libgotvptop_equalizer_t *p_equalizer, unsigned u_band );

/**
 * Apply new equalizer settings to a media player.
 *
 * The equalizer is first created by invoking libgotvptop_audio_equalizer_new() or
 * libgotvptop_audio_equalizer_new_from_preset().
 *
 * It is possible to apply new equalizer settings to a media player whether the media
 * player is currently playing media or not.
 *
 * Invoking this method will immediately apply the new equalizer settings to the audio
 * output of the currently playing media if there is any.
 *
 * If there is no currently playing media, the new equalizer settings will be applied
 * later if and when new media is played.
 *
 * Equalizer settings will automatically be applied to subsequently played media.
 *
 * To disable the equalizer for a media player invoke this method passing NULL for the
 * p_equalizer parameter.
 *
 * The media player does not keep a reference to the supplied equalizer so it is safe
 * for an application to release the equalizer reference any time after this method
 * returns.
 *
 * \param p_mi opaque media player handle
 * \param p_equalizer opaque equalizer handle, or NULL to disable the equalizer for this media player
 * \return zero on success, -1 on error
 * \version LibGOTV 2.2.0 or later
 */
LIBGOTV_API int libgotvptop_media_player_set_equalizer( libgotvptop_media_player_t *p_mi, libgotvptop_equalizer_t *p_equalizer );

/**
 * Media player roles.
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * See \ref libgotvptop_media_player_set_role()
 */
typedef enum libgotvptop_media_player_role {
    libgotvptop_role_None = 0, /**< Don't use a media player role */
    libgotvptop_role_Music,   /**< Music (or radio) playback */
    libgotvptop_role_Video, /**< Video playback */
    libgotvptop_role_Communication, /**< Speech, real-time communication */
    libgotvptop_role_Game, /**< Video game */
    libgotvptop_role_Notification, /**< User interaction feedback */
    libgotvptop_role_Animation, /**< Embedded animation (e.g. in web page) */
    libgotvptop_role_Production, /**< Audio editting/production */
    libgotvptop_role_Accessibility, /**< Accessibility */
    libgotvptop_role_Test /** Testing */
#define libgotvptop_role_Last libgotvptop_role_Test
} libgotvptop_media_player_role_t;

/**
 * Gets the media role.
 *
 * \version LibGOTV 3.0.0 and later.
 *
 * \param p_mi media player
 * \return the media player role (\ref libgotvptop_media_player_role_t)
 */
LIBGOTV_API int libgotvptop_media_player_get_role(libgotvptop_media_player_t *p_mi);

/**
 * Sets the media role.
 *
 * \param p_mi media player
 * \param role the media player role (\ref libgotvptop_media_player_role_t)
 * \return 0 on success, -1 on error
 */
LIBGOTV_API int libgotvptop_media_player_set_role(libgotvptop_media_player_t *p_mi,
                                            unsigned role);

/** @} audio */

/** @} media_player */

#endif /* GOTV_LIBGOTV_MEDIA_PLAYER_H */
