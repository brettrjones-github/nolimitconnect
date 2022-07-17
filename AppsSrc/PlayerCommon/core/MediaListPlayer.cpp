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

#include <QtCore/QDebug>

#include <gotvptop/gotvptop.h>

#include "core/Error.h"
#include "core/Instance.h"
#include "core/MediaList.h"
#include "core/MediaListPlayer.h"
#include "core/MediaPlayer.h"

NlcPtoPMediaListPlayer::NlcPtoPMediaListPlayer(NlcPtoPInstance *instance)
    : QObject(instance),
      _list(0),
      _mode(NlcPtoP::DefaultPlayback)
{
    _player = new NlcPtoPMediaPlayer(instance);

    _gotvptopMediaListPlayer = libgotvptop_media_list_player_new(instance->core());
    _gotvptopEvents = libgotvptop_media_list_player_event_manager(_gotvptopMediaListPlayer);
    libgotvptop_media_list_player_set_media_player(_gotvptopMediaListPlayer, _player->core());

    createCoreConnections();

    NlcPtoPError::showErrmsg();
}

NlcPtoPMediaListPlayer::NlcPtoPMediaListPlayer(NlcPtoPMediaPlayer *player,
                                       NlcPtoPInstance *instance)
    : QObject(instance),
      _list(0),
      _mode(NlcPtoP::DefaultPlayback)
{
    _player = player;

    _gotvptopMediaListPlayer = libgotvptop_media_list_player_new(instance->core());
    _gotvptopEvents = libgotvptop_media_list_player_event_manager(_gotvptopMediaListPlayer);
    libgotvptop_media_list_player_set_media_player(_gotvptopMediaListPlayer, _player->core());

    createCoreConnections();

    NlcPtoPError::showErrmsg();
}

NlcPtoPMediaListPlayer::~NlcPtoPMediaListPlayer()
{
    removeCoreConnections();

    libgotvptop_media_list_player_release(_gotvptopMediaListPlayer);

    NlcPtoPError::showErrmsg();
}

libgotvptop_media_list_player_t *NlcPtoPMediaListPlayer::core()
{
    return _gotvptopMediaListPlayer;
}

NlcPtoPMediaList *NlcPtoPMediaListPlayer::currentMediaList()
{
    return _list;
}

NlcPtoPMediaPlayer *NlcPtoPMediaListPlayer::mediaPlayer()
{
    return _player;
}

void NlcPtoPMediaListPlayer::createCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListPlayerPlayed
         << libgotvptop_MediaListPlayerNextItemSet
         << libgotvptop_MediaListPlayerStopped;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_attach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void NlcPtoPMediaListPlayer::removeCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListPlayerPlayed
         << libgotvptop_MediaListPlayerNextItemSet
         << libgotvptop_MediaListPlayerStopped;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_detach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void NlcPtoPMediaListPlayer::itemAt(int index)
{
    libgotvptop_media_list_player_play_item_at_index(_gotvptopMediaListPlayer, index);

    NlcPtoPError::showErrmsg();
}

void NlcPtoPMediaListPlayer::next()
{
    libgotvptop_media_list_player_next(_gotvptopMediaListPlayer);

    NlcPtoPError::showErrmsg();
}

void NlcPtoPMediaListPlayer::play()
{
    libgotvptop_media_list_player_play(_gotvptopMediaListPlayer);

    NlcPtoPError::showErrmsg();
}

NlcPtoP::PlaybackMode NlcPtoPMediaListPlayer::playbackMode() const
{
    return _mode;
}

void NlcPtoPMediaListPlayer::previous()
{
    libgotvptop_media_list_player_previous(_gotvptopMediaListPlayer);

    NlcPtoPError::showErrmsg();
}

void NlcPtoPMediaListPlayer::stop()
{
    libgotvptop_media_list_player_stop(_gotvptopMediaListPlayer);

    NlcPtoPError::showErrmsg();
}

void NlcPtoPMediaListPlayer::setMediaList(NlcPtoPMediaList *list)
{
    _list = list;
    libgotvptop_media_list_player_set_media_list(_gotvptopMediaListPlayer, list->core());

    NlcPtoPError::showErrmsg();
}

void NlcPtoPMediaListPlayer::setPlaybackMode(const NlcPtoP::PlaybackMode &mode)
{
    _mode = mode;
    libgotvptop_media_list_player_set_playback_mode(_gotvptopMediaListPlayer, libgotvptop_playback_mode_t(mode));
}

void NlcPtoPMediaListPlayer::libgotvptop_callback(const libgotvptop_event_t *event,
                                         void *data)
{
    NlcPtoPMediaListPlayer *core = static_cast<NlcPtoPMediaListPlayer *>(data);

    switch (event->type) {
    case libgotvptop_MediaListPlayerPlayed:
        emit core->played();
        break;
    case libgotvptop_MediaListPlayerNextItemSet:
        emit core->nextItemSet(event->u.media_list_player_next_item_set.item);
        emit core->nextItemSet(core->currentMediaList()->at(core->currentMediaList()->indexOf(event->u.media_list_player_next_item_set.item)));
        break;
    case libgotvptop_MediaListPlayerStopped:
        emit core->stopped();
        break;
    default:
        break; // LCOV_EXCL_LINE
    }
}
