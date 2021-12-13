/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2014 Tadej Novak <tadej@tano.si>
*
* Based on Phonon multimedia library
* Copyright (C) 2012 Harald Sitter <sitter@kde.org>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <gotvptop/gotvptop.h>

#include "core/MediaPlayer.h"
#include "core/VideoMemoryStream.h"

static inline GoTvPtoPVideoMemoryStream *p_this(void *opaque) { return static_cast<GoTvPtoPVideoMemoryStream *>(opaque); }
static inline GoTvPtoPVideoMemoryStream *p_this(void **opaque) { return static_cast<GoTvPtoPVideoMemoryStream *>(*opaque); }
#define P_THIS p_this(opaque)

GoTvPtoPVideoMemoryStream::GoTvPtoPVideoMemoryStream() { }

GoTvPtoPVideoMemoryStream::~GoTvPtoPVideoMemoryStream() { }

static int lcm(int a, int b)
{
    return a * b / GCD( a, b );
}

unsigned GoTvPtoPVideoMemoryStream::setPitchAndLines(const gotvptop_chroma_description_t *desc,
                                                unsigned width,
                                                unsigned height,
                                                unsigned *pitches,
                                                unsigned *lines,
                                                unsigned *visiblePitches,
                                                unsigned *visibleLines)
{
    // Mostly taken from gotvptop/src/misc/picture.c
    // Simple alignment would be an option but I trust the GOTV guys they know what they are doing.
    int i_modulo_w = 1;
    int i_modulo_h = 1;
    unsigned int i_ratio_h  = 1;
    for( unsigned i = 0; i < desc->plane_count; i++ )
    {
        i_modulo_w = lcm( i_modulo_w, 8 * desc->p[i].w.den );
        i_modulo_h = lcm( i_modulo_h, 8 * desc->p[i].h.den );
        if( i_ratio_h < desc->p[i].h.den )
            i_ratio_h = desc->p[i].h.den;

    }
    i_modulo_h = lcm( i_modulo_h, 32 );

    const int i_width_aligned  = ( width  + i_modulo_w - 1 ) / i_modulo_w * i_modulo_w;
    const int i_height_aligned = ( height + i_modulo_h - 1 ) / i_modulo_h * i_modulo_h;
    const int i_height_extra   = 2 * i_ratio_h; /* This one is a hack for some ASM functions */

    unsigned int bufferSize = 0;
    for(unsigned i = 0; i < desc->plane_count; ++i)
    {
        pitches[i] = i_width_aligned * desc->p[i].w.num / desc->p[i].w.den * desc->pixel_size;
        if (visiblePitches)
            visiblePitches[i] = width * desc->p[i].w.num / desc->p[i].w.den * desc->pixel_size;

        lines[i] = (i_height_aligned + i_height_extra ) * desc->p[i].h.num / desc->p[i].h.den;
        if (visibleLines)
            visibleLines[i] = height * desc->p[i].h.num / desc->p[i].h.den;

        bufferSize += pitches[i] * lines[i];
    }

    return bufferSize;
}

void GoTvPtoPVideoMemoryStream::setCallbacks(GoTvPtoPMediaPlayer *player)
{
    libgotvptop_video_set_callbacks(player->core(),
                               lockCallbackInternal,
                               unlockCallbackInternal,
                               displayCallbackInternal,
                               this);
    libgotvptop_video_set_format_callbacks(player->core(),
                                      formatCallbackInternal,
                                      formatCleanUpCallbackInternal);
}

void GoTvPtoPVideoMemoryStream::unsetCallbacks(GoTvPtoPMediaPlayer *player)
{
    libgotvptop_video_set_callbacks(player->core(), 0, 0, 0, 0);
    libgotvptop_video_set_format_callbacks(player->core(), 0, 0);
}


void *GoTvPtoPVideoMemoryStream::lockCallbackInternal(void *opaque,
                                              void **planes)
{
    return P_THIS->lockCallback(planes);
}

void GoTvPtoPVideoMemoryStream::unlockCallbackInternal(void *opaque,
                                               void *picture,
                                               void *const*planes)
{
    P_THIS->unlockCallback(picture, planes);
}

void GoTvPtoPVideoMemoryStream::displayCallbackInternal(void *opaque,
                                                void *picture)
{
    P_THIS->displayCallback(picture);
}

unsigned GoTvPtoPVideoMemoryStream::formatCallbackInternal(void **opaque,
                                                   char *chroma,
                                                   unsigned *width,
                                                   unsigned *height,
                                                   unsigned *pitches,
                                                   unsigned *lines)
{
    return P_THIS->formatCallback(chroma, width, height, pitches, lines);
}

void GoTvPtoPVideoMemoryStream::formatCleanUpCallbackInternal(void *opaque)
{
    P_THIS->formatCleanUpCallback();
}
