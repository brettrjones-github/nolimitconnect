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

GoTvPtoPMediaListPlayer::GoTvPtoPMediaListPlayer(GoTvPtoPInstance *instance)
    : QObject(instance),
      _list(0),
      _mode(GoTvPtoP::DefaultPlayback)
{
    _player = new GoTvPtoPMediaPlayer(instance);

    _gotvptopMediaListPlayer = libgotvptop_media_list_player_new(instance->core());
    _gotvptopEvents = libgotvptop_media_list_player_event_manager(_gotvptopMediaListPlayer);
    libgotvptop_media_list_player_set_media_player(_gotvptopMediaListPlayer, _player->core());

    createCoreConnections();

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMediaListPlayer::GoTvPtoPMediaListPlayer(GoTvPtoPMediaPlayer *player,
                                       GoTvPtoPInstance *instance)
    : QObject(instance),
      _list(0),
      _mode(GoTvPtoP::DefaultPlayback)
{
    _player = player;

    _gotvptopMediaListPlayer = libgotvptop_media_list_player_new(instance->core());
    _gotvptopEvents = libgotvptop_media_list_player_event_manager(_gotvptopMediaListPlayer);
    libgotvptop_media_list_player_set_media_player(_gotvptopMediaListPlayer, _player->core());

    createCoreConnections();

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMediaListPlayer::~GoTvPtoPMediaListPlayer()
{
    removeCoreConnections();

    libgotvptop_media_list_player_release(_gotvptopMediaListPlayer);

    GoTvPtoPError::showErrmsg();
}

libgotvptop_media_list_player_t *GoTvPtoPMediaListPlayer::core()
{
    return _gotvptopMediaListPlayer;
}

GoTvPtoPMediaList *GoTvPtoPMediaListPlayer::currentMediaList()
{
    return _list;
}

GoTvPtoPMediaPlayer *GoTvPtoPMediaListPlayer::mediaPlayer()
{
    return _player;
}

void GoTvPtoPMediaListPlayer::createCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListPlayerPlayed
         << libgotvptop_MediaListPlayerNextItemSet
         << libgotvptop_MediaListPlayerStopped;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_attach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMediaListPlayer::removeCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListPlayerPlayed
         << libgotvptop_MediaListPlayerNextItemSet
         << libgotvptop_MediaListPlayerStopped;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_detach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMediaListPlayer::itemAt(int index)
{
    libgotvptop_media_list_player_play_item_at_index(_gotvptopMediaListPlayer, index);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaListPlayer::next()
{
    libgotvptop_media_list_player_next(_gotvptopMediaListPlayer);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaListPlayer::play()
{
    libgotvptop_media_list_player_play(_gotvptopMediaListPlayer);

    GoTvPtoPError::showErrmsg();
}

GoTvPtoP::PlaybackMode GoTvPtoPMediaListPlayer::playbackMode() const
{
    return _mode;
}

void GoTvPtoPMediaListPlayer::previous()
{
    libgotvptop_media_list_player_previous(_gotvptopMediaListPlayer);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaListPlayer::stop()
{
    libgotvptop_media_list_player_stop(_gotvptopMediaListPlayer);

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaListPlayer::setMediaList(GoTvPtoPMediaList *list)
{
    _list = list;
    libgotvptop_media_list_player_set_media_list(_gotvptopMediaListPlayer, list->core());

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaListPlayer::setPlaybackMode(const GoTvPtoP::PlaybackMode &mode)
{
    _mode = mode;
    libgotvptop_media_list_player_set_playback_mode(_gotvptopMediaListPlayer, libgotvptop_playback_mode_t(mode));
}

void GoTvPtoPMediaListPlayer::libgotvptop_callback(const libgotvptop_event_t *event,
                                         void *data)
{
    GoTvPtoPMediaListPlayer *core = static_cast<GoTvPtoPMediaListPlayer *>(data);

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
