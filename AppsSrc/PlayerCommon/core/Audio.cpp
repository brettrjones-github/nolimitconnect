/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

/* MSVC support fix */
#if defined(_MSC_VER)
#include <BaseTsd.h>
#endif
/* MSVC + MinGW support fix */
#if defined(_WIN32)
#define LIBNLC_USE_PTHREAD_CANCEL 1
#endif

#include <gotvptop/gotvptop.h>
#include <gotvptop_common.h>
#include <gotvptop_variables.h>

#include "core/Audio.h"
#include "core/Error.h"
#include "core/MediaPlayer.h"

/*!
    \private
*/
class NlcPtoPAudioCallbackHelper
{
public:
    static int volumeCallback(gotvptop_object_t *obj,
                              const char *name,
                              gotvptop_value_t oldVal,
                              gotvptop_value_t newVal,
                              void *data)
    {
        Q_UNUSED(obj)
        Q_UNUSED(name)
        Q_UNUSED(oldVal)

        NlcPtoPAudio *core = static_cast<NlcPtoPAudio *>(data);
        emit core->volumeChangedF(newVal.f_float);
        int vol = newVal.f_float < 0 ? -1 : qRound(newVal.f_float * 100.f);
        emit core->volumeChanged(vol);
        return NLC_SUCCESS;
    }

    static int muteCallback(gotvptop_object_t *obj,
                            const char *name,
                            gotvptop_value_t oldVal,
                            gotvptop_value_t newVal,
                            void *data)
    {
        Q_UNUSED(obj);
        Q_UNUSED(name);
        Q_UNUSED(oldVal);

        NlcPtoPAudio *core = static_cast<NlcPtoPAudio *>(data);
        emit core->muteChanged(newVal.b_bool);
        return NLC_SUCCESS;
    }
};

NlcPtoPAudio::NlcPtoPAudio(NlcPtoPMediaPlayer *player)
    : QObject(player),
      _gotvptopMediaPlayer(player->core())
{
    var_AddCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "volume", NlcPtoPAudioCallbackHelper::volumeCallback, this);
    var_AddCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "mute", NlcPtoPAudioCallbackHelper::muteCallback, this);
}

NlcPtoPAudio::~NlcPtoPAudio()
{
    var_DelCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "volume", NlcPtoPAudioCallbackHelper::volumeCallback, this);
    var_DelCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "mute", NlcPtoPAudioCallbackHelper::muteCallback, this);
}

bool NlcPtoPAudio::getMute() const
{
    bool mute = false;
    if (_gotvptopMediaPlayer) {
        mute = libgotvptop_audio_get_mute(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();
    }

    return mute;
}

void NlcPtoPAudio::setVolume(int volume)
{
    if (_gotvptopMediaPlayer) {
        // Don't change if volume is the same
        if (volume != NlcPtoPAudio::volume()) {
            libgotvptop_audio_set_volume(_gotvptopMediaPlayer, volume);
            NlcPtoPError::showErrmsg();
        }
    }
}

void NlcPtoPAudio::setTrack(int track)
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_audio_set_track(_gotvptopMediaPlayer, track);
        NlcPtoPError::showErrmsg();
    }
}

bool NlcPtoPAudio::toggleMute() const
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_audio_toggle_mute(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();
    }

    return getMute();
}

void NlcPtoPAudio::setMute(bool mute) const
{
    if (_gotvptopMediaPlayer && mute != getMute()) {
        libgotvptop_audio_set_mute(_gotvptopMediaPlayer, mute);
        NlcPtoPError::showErrmsg();
    }
}

int NlcPtoPAudio::track() const
{
    int track = -1;
    if (_gotvptopMediaPlayer) {
        track = libgotvptop_audio_get_track(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();
    }

    return track;
}

int NlcPtoPAudio::trackCount() const
{
    int count = -1;
    if (_gotvptopMediaPlayer) {
        count = libgotvptop_audio_get_track_count(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();
    }

    return count;
}

QStringList NlcPtoPAudio::trackDescription() const
{
    QStringList descriptions;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if (trackCount() > 1) {
            for (int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> NlcPtoPAudio::trackIds() const
{
    QList<int> ids;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();

        ids << desc->i_id;
        if (trackCount() > 1) {
            for (int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}

QMap<int, QString> NlcPtoPAudio::tracks() const
{
    QMap<int, QString> tracks;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc, *first;
        first = desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();

        if (desc != NULL) {
            tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
            if (trackCount() > 1) {
                for (int i = 1; i < trackCount(); i++) {
                    desc = desc->p_next;
                    tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
                }
            }
            libgotvptop_track_description_list_release(first);
        }
    }

    return tracks;
}

int NlcPtoPAudio::volume() const
{
    int volume = -1;
    if (_gotvptopMediaPlayer) {
        volume = libgotvptop_audio_get_volume(_gotvptopMediaPlayer);
        NlcPtoPError::showErrmsg();
    }

    return volume;
}

NlcPtoP::AudioChannel NlcPtoPAudio::channel() const
{
    NlcPtoP::AudioChannel channel = NlcPtoP::AudioChannelError;
    if (_gotvptopMediaPlayer) {
        channel = NlcPtoP::AudioChannel(libgotvptop_audio_get_channel(_gotvptopMediaPlayer));
        NlcPtoPError::showErrmsg();
    }

    return channel;
}

void NlcPtoPAudio::setChannel(NlcPtoP::AudioChannel channel)
{
    if (_gotvptopMediaPlayer) {
        // Don't change if channel is the same
        if (channel != NlcPtoPAudio::channel()) {
            libgotvptop_audio_set_channel(_gotvptopMediaPlayer, channel);
            NlcPtoPError::showErrmsg();
        }
    }
}
