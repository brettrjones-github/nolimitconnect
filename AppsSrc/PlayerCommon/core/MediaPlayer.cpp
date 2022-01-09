/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include "core/Audio.h"
#include "core/Error.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaPlayer.h"
#include "core/Video.h"
#include "core/VideoDelegate.h"

#if LIBNO_LIMIT_VERSION >= 0x020200
#include "core/Equalizer.h"
#endif

GoTvPtoPMediaPlayer::GoTvPtoPMediaPlayer(GoTvPtoPInstance *instance)
    : QObject(instance)
{
    _gotvptopMediaPlayer = libgotvptop_media_player_new(instance->core());
    _gotvptopEvents = libgotvptop_media_player_event_manager(_gotvptopMediaPlayer);

    /* Disable mouse and keyboard events */
    libgotvptop_video_set_key_input(_gotvptopMediaPlayer, false);
    libgotvptop_video_set_mouse_input(_gotvptopMediaPlayer, false);

    GoTvPtoPError::showErrmsg();

    _gotvptopAudio = new GoTvPtoPAudio(this);
    _gotvptopVideo = new GoTvPtoPVideo(this);
#if LIBNO_LIMIT_VERSION >= 0x020200
    _gotvptopEqualizer = new GoTvPtoPEqualizer(this);
#endif

    _videoWidget = 0;
    _media = 0;

    createCoreConnections();

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMediaPlayer::~GoTvPtoPMediaPlayer()
{
    removeCoreConnections();

    delete _gotvptopAudio;
    delete _gotvptopVideo;
#if LIBNO_LIMIT_VERSION >= 0x020200
    delete _gotvptopEqualizer;
#endif

    libgotvptop_media_player_release(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();
}

libgotvptop_media_player_t *GoTvPtoPMediaPlayer::core() const
{
    return _gotvptopMediaPlayer;
}

GoTvPtoPAudio *GoTvPtoPMediaPlayer::audio() const
{
    return _gotvptopAudio;
}

GoTvPtoPVideo *GoTvPtoPMediaPlayer::video() const
{
    return _gotvptopVideo;
}

#if LIBNO_LIMIT_VERSION >= 0x020200
GoTvPtoPEqualizer *GoTvPtoPMediaPlayer::equalizer() const
{
    return _gotvptopEqualizer;
}
#endif

void GoTvPtoPMediaPlayer::createCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaPlayerMediaChanged
         << libgotvptop_MediaPlayerNothingSpecial
         << libgotvptop_MediaPlayerOpening
         << libgotvptop_MediaPlayerBuffering
         << libgotvptop_MediaPlayerPlaying
         << libgotvptop_MediaPlayerPaused
         << libgotvptop_MediaPlayerStopped
         << libgotvptop_MediaPlayerForward
         << libgotvptop_MediaPlayerBackward
         << libgotvptop_MediaPlayerEndReached
         << libgotvptop_MediaPlayerEncounteredError
         << libgotvptop_MediaPlayerTimeChanged
         << libgotvptop_MediaPlayerPositionChanged
         << libgotvptop_MediaPlayerSeekableChanged
         << libgotvptop_MediaPlayerPausableChanged
         << libgotvptop_MediaPlayerTitleChanged
         << libgotvptop_MediaPlayerSnapshotTaken
         << libgotvptop_MediaPlayerLengthChanged
         << libgotvptop_MediaPlayerVout;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_attach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMediaPlayer::removeCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaPlayerMediaChanged
         << libgotvptop_MediaPlayerNothingSpecial
         << libgotvptop_MediaPlayerOpening
         << libgotvptop_MediaPlayerBuffering
         << libgotvptop_MediaPlayerPlaying
         << libgotvptop_MediaPlayerPaused
         << libgotvptop_MediaPlayerStopped
         << libgotvptop_MediaPlayerForward
         << libgotvptop_MediaPlayerBackward
         << libgotvptop_MediaPlayerEndReached
         << libgotvptop_MediaPlayerEncounteredError
         << libgotvptop_MediaPlayerTimeChanged
         << libgotvptop_MediaPlayerPositionChanged
         << libgotvptop_MediaPlayerSeekableChanged
         << libgotvptop_MediaPlayerPausableChanged
         << libgotvptop_MediaPlayerTitleChanged
         << libgotvptop_MediaPlayerSnapshotTaken
         << libgotvptop_MediaPlayerLengthChanged
         << libgotvptop_MediaPlayerVout;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_detach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

bool GoTvPtoPMediaPlayer::hasVout() const
{
    bool status = false;
    if (_gotvptopMediaPlayer) {
        status = libgotvptop_media_player_has_vout(_gotvptopMediaPlayer);
    }

    return status;
}

int GoTvPtoPMediaPlayer::length() const
{
    libgotvptop_time_t length = libgotvptop_media_player_get_length(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();

    return length;
}

GoTvPtoPMedia *GoTvPtoPMediaPlayer::currentMedia() const
{
    return _media;
}

libgotvptop_media_t *GoTvPtoPMediaPlayer::currentMediaCore()
{
    libgotvptop_media_t *media = libgotvptop_media_player_get_media(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();

    return media;
}

void GoTvPtoPMediaPlayer::open(GoTvPtoPMedia *media)
{
    _media = media;
    libgotvptop_media_player_set_media(_gotvptopMediaPlayer, media->core());

    GoTvPtoPError::showErrmsg();

    play();
}

void GoTvPtoPMediaPlayer::openOnly(GoTvPtoPMedia *media)
{
    _media = media;
    libgotvptop_media_player_set_media(_gotvptopMediaPlayer, media->core());

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::play()
{
    if (!_gotvptopMediaPlayer)
        return;

    if (_videoWidget) {
        _currentWId = _videoWidget->request();
    } else {
        _currentWId = 0;
    }

    /* Get our media instance to use our window */
    if (_currentWId) {
#if defined(Q_OS_WIN32)
        libgotvptop_media_player_set_hwnd(_gotvptopMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_DARWIN)
        libgotvptop_media_player_set_nsobject(_gotvptopMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_UNIX)
        libgotvptop_media_player_set_xwindow(_gotvptopMediaPlayer, _currentWId);
#endif
    }

    libgotvptop_media_player_play(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::pause()
{
    if (!_gotvptopMediaPlayer)
        return;

    if (libgotvptop_media_player_can_pause(_gotvptopMediaPlayer))
        libgotvptop_media_player_set_pause(_gotvptopMediaPlayer, true);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::togglePause()
{
    if (!_gotvptopMediaPlayer)
        return;

    if (libgotvptop_media_player_can_pause(_gotvptopMediaPlayer))
        libgotvptop_media_player_pause(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::resume()
{
    if (!_gotvptopMediaPlayer)
        return;

    if (libgotvptop_media_player_can_pause(_gotvptopMediaPlayer))
        libgotvptop_media_player_set_pause(_gotvptopMediaPlayer, false);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::setTime(int time)
{
    if (!(state() == GoTvPtoP::Buffering
          || state() == GoTvPtoP::Playing
          || state() == GoTvPtoP::Paused))
        return;

    libgotvptop_media_player_set_time(_gotvptopMediaPlayer, time);

    if (state() == GoTvPtoP::Paused)
        emit timeChanged(time);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::setVideoWidget(GoTvPtoPVideoDelegate *widget)
{
    _videoWidget = widget;
}

bool GoTvPtoPMediaPlayer::seekable() const
{
    if (!libgotvptop_media_player_get_media(_gotvptopMediaPlayer))
        return false;

    bool seekable = libgotvptop_media_player_is_seekable(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();

    return seekable;
}

GoTvPtoP::State GoTvPtoPMediaPlayer::state() const
{
    // It's possible that the gotvptop doesn't play anything
    // so check before
    if (!libgotvptop_media_player_get_media(_gotvptopMediaPlayer))
        return GoTvPtoP::Idle;

    libgotvptop_state_t state;
    state = libgotvptop_media_player_get_state(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();

    return GoTvPtoP::State(state);
}

void GoTvPtoPMediaPlayer::stop()
{
    if (!_gotvptopMediaPlayer)
        return;

    if (_videoWidget)
        _videoWidget->release();
    _currentWId = 0;

    libgotvptop_media_player_stop(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();
}

int GoTvPtoPMediaPlayer::time() const
{
    libgotvptop_time_t time = libgotvptop_media_player_get_time(_gotvptopMediaPlayer);

    GoTvPtoPError::showErrmsg();

    return time;
}

GoTvPtoPVideoDelegate *GoTvPtoPMediaPlayer::videoWidget() const
{
    return _videoWidget;
}

void GoTvPtoPMediaPlayer::libgotvptop_callback(const libgotvptop_event_t *event,
                                     void *data)
{
    GoTvPtoPMediaPlayer *core = static_cast<GoTvPtoPMediaPlayer *>(data);

    switch (event->type) {
    case libgotvptop_MediaPlayerMediaChanged:
        emit core->mediaChanged(event->u.media_player_media_changed.new_media);
        break;
    case libgotvptop_MediaPlayerNothingSpecial:
        emit core->nothingSpecial();
        break;
    case libgotvptop_MediaPlayerOpening:
        emit core->opening();
        break;
    case libgotvptop_MediaPlayerBuffering:
        emit core->buffering(event->u.media_player_buffering.new_cache);
        emit core->buffering(qRound(event->u.media_player_buffering.new_cache));
        break;
    case libgotvptop_MediaPlayerPlaying:
        emit core->playing();
        break;
    case libgotvptop_MediaPlayerPaused:
        emit core->paused();
        break;
    case libgotvptop_MediaPlayerStopped:
        emit core->stopped();
        break;
    case libgotvptop_MediaPlayerForward:
        emit core->forward();
        break;
    case libgotvptop_MediaPlayerBackward:
        emit core->backward();
        break;
    case libgotvptop_MediaPlayerEndReached:
        emit core->end();
        break;
    case libgotvptop_MediaPlayerEncounteredError:
        emit core->error();
        break;
    case libgotvptop_MediaPlayerTimeChanged:
        emit core->timeChanged(event->u.media_player_time_changed.new_time);
        break;
    case libgotvptop_MediaPlayerPositionChanged:
        emit core->positionChanged(event->u.media_player_position_changed.new_position);
        break;
    case libgotvptop_MediaPlayerSeekableChanged:
        emit core->seekableChanged(event->u.media_player_seekable_changed.new_seekable);
        break;
    case libgotvptop_MediaPlayerPausableChanged:
        emit core->pausableChanged(event->u.media_player_pausable_changed.new_pausable);
        break;
    case libgotvptop_MediaPlayerTitleChanged:
        emit core->titleChanged(event->u.media_player_title_changed.new_title);
        break;
    case libgotvptop_MediaPlayerSnapshotTaken:
        emit core->snapshotTaken(event->u.media_player_snapshot_taken.psz_filename);
        break;
    case libgotvptop_MediaPlayerLengthChanged:
        emit core->lengthChanged(event->u.media_player_length_changed.new_length);
        break;
    case libgotvptop_MediaPlayerVout:
        emit core->vout(event->u.media_player_vout.new_count);
        break;
    default:
        break;
    }

    if (event->type >= libgotvptop_MediaPlayerNothingSpecial
        && event->type <= libgotvptop_MediaPlayerEncounteredError) {
        emit core->stateChanged();
    }
}

float GoTvPtoPMediaPlayer::position()
{
    if (!_gotvptopMediaPlayer)
        return -1;

    return libgotvptop_media_player_get_position(_gotvptopMediaPlayer);
}

float GoTvPtoPMediaPlayer::sampleAspectRatio()
{
    if (!_gotvptopMediaPlayer)
        return 0.0;
#if LIBNO_LIMIT_VERSION >= 0x020100
    float sar = 0.0;

    libgotvptop_media_track_t **tracks;
    unsigned tracksCount;
    tracksCount = libgotvptop_media_tracks_get(_media->core(), &tracks);
    if (tracksCount > 0) {
        for (unsigned i = 0; i < tracksCount; i++) {
            libgotvptop_media_track_t *track = tracks[i];
            if (track->i_type == libgotvptop_track_video && track->i_id == 0) {
                libgotvptop_video_track_t *videoTrack = track->video;
                if (videoTrack->i_sar_num > 0)
                    sar = (float)videoTrack->i_sar_den / (float)videoTrack->i_sar_num;
            }
        }
        libgotvptop_media_tracks_release(tracks, tracksCount);
    }

    return sar;
#else
    return 1.0;
#endif // LIBNO_LIMIT_VERSION >= 0x020100
}

void GoTvPtoPMediaPlayer::setPosition(float pos)
{
    libgotvptop_media_player_set_position(_gotvptopMediaPlayer, pos);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaPlayer::setPlaybackRate(float rate)
{
    libgotvptop_media_player_set_rate(_gotvptopMediaPlayer, rate);

    GoTvPtoPError::showErrmsg();
}

float GoTvPtoPMediaPlayer::playbackRate()
{
    if (!_gotvptopMediaPlayer)
        return -1;

    return libgotvptop_media_player_get_rate(_gotvptopMediaPlayer);
}
