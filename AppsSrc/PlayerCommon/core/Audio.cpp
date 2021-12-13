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
#define LIBGOTV_USE_PTHREAD_CANCEL 1
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
class GoTvPtoPAudioCallbackHelper
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

        GoTvPtoPAudio *core = static_cast<GoTvPtoPAudio *>(data);
        emit core->volumeChangedF(newVal.f_float);
        int vol = newVal.f_float < 0 ? -1 : qRound(newVal.f_float * 100.f);
        emit core->volumeChanged(vol);
        return GOTV_SUCCESS;
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

        GoTvPtoPAudio *core = static_cast<GoTvPtoPAudio *>(data);
        emit core->muteChanged(newVal.b_bool);
        return GOTV_SUCCESS;
    }
};

GoTvPtoPAudio::GoTvPtoPAudio(GoTvPtoPMediaPlayer *player)
    : QObject(player),
      _gotvptopMediaPlayer(player->core())
{
    var_AddCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "volume", GoTvPtoPAudioCallbackHelper::volumeCallback, this);
    var_AddCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "mute", GoTvPtoPAudioCallbackHelper::muteCallback, this);
}

GoTvPtoPAudio::~GoTvPtoPAudio()
{
    var_DelCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "volume", GoTvPtoPAudioCallbackHelper::volumeCallback, this);
    var_DelCallback((gotvptop_object_t *)_gotvptopMediaPlayer, "mute", GoTvPtoPAudioCallbackHelper::muteCallback, this);
}

bool GoTvPtoPAudio::getMute() const
{
    bool mute = false;
    if (_gotvptopMediaPlayer) {
        mute = libgotvptop_audio_get_mute(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return mute;
}

void GoTvPtoPAudio::setVolume(int volume)
{
    if (_gotvptopMediaPlayer) {
        // Don't change if volume is the same
        if (volume != GoTvPtoPAudio::volume()) {
            libgotvptop_audio_set_volume(_gotvptopMediaPlayer, volume);
            GoTvPtoPError::showErrmsg();
        }
    }
}

void GoTvPtoPAudio::setTrack(int track)
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_audio_set_track(_gotvptopMediaPlayer, track);
        GoTvPtoPError::showErrmsg();
    }
}

bool GoTvPtoPAudio::toggleMute() const
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_audio_toggle_mute(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return getMute();
}

void GoTvPtoPAudio::setMute(bool mute) const
{
    if (_gotvptopMediaPlayer && mute != getMute()) {
        libgotvptop_audio_set_mute(_gotvptopMediaPlayer, mute);
        GoTvPtoPError::showErrmsg();
    }
}

int GoTvPtoPAudio::track() const
{
    int track = -1;
    if (_gotvptopMediaPlayer) {
        track = libgotvptop_audio_get_track(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return track;
}

int GoTvPtoPAudio::trackCount() const
{
    int count = -1;
    if (_gotvptopMediaPlayer) {
        count = libgotvptop_audio_get_track_count(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return count;
}

QStringList GoTvPtoPAudio::trackDescription() const
{
    QStringList descriptions;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

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

QList<int> GoTvPtoPAudio::trackIds() const
{
    QList<int> ids;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

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

QMap<int, QString> GoTvPtoPAudio::tracks() const
{
    QMap<int, QString> tracks;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc, *first;
        first = desc = libgotvptop_audio_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

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

int GoTvPtoPAudio::volume() const
{
    int volume = -1;
    if (_gotvptopMediaPlayer) {
        volume = libgotvptop_audio_get_volume(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return volume;
}

GoTvPtoP::AudioChannel GoTvPtoPAudio::channel() const
{
    GoTvPtoP::AudioChannel channel = GoTvPtoP::AudioChannelError;
    if (_gotvptopMediaPlayer) {
        channel = GoTvPtoP::AudioChannel(libgotvptop_audio_get_channel(_gotvptopMediaPlayer));
        GoTvPtoPError::showErrmsg();
    }

    return channel;
}

void GoTvPtoPAudio::setChannel(GoTvPtoP::AudioChannel channel)
{
    if (_gotvptopMediaPlayer) {
        // Don't change if channel is the same
        if (channel != GoTvPtoPAudio::channel()) {
            libgotvptop_audio_set_channel(_gotvptopMediaPlayer, channel);
            GoTvPtoPError::showErrmsg();
        }
    }
}
