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

#ifndef GOTVQT_MEDIALISTPLAYER_H_
#define GOTVQT_MEDIALISTPLAYER_H_

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Enums.h"
#include "SharedExportCore.h"

class NlcPtoPInstance;
class NlcPtoPMediaList;
class NlcPtoPMediaPlayer;
class NlcPtoPMedia;

struct libgotvptop_event_t;
struct libgotvptop_event_manager_t;
struct libgotvptop_media_t;
struct libgotvptop_media_list_player_t;

/*!
    \class NlcPtoPMediaListPlayer MediaListPlayer.h core/MediaListPlayer.h
    \ingroup GOTVQtCore
    \brief Media List Player

    A basic MediaListPlayer manager for GOTV-Qt library.
    It provides internal playlist support.
    Requires a valid NlcPtoPMediaPlayer.
*/
class GOTVQT_CORE_EXPORT NlcPtoPMediaListPlayer : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief NlcPtoPMediaListPlayer constructor.

        Create NlcPtoPMediaListPlayer without new core.

        \param instance instance object
    */
    explicit NlcPtoPMediaListPlayer(NlcPtoPInstance *instance);

    /*!
        \brief NlcPtoPMediaListPlayer constructor.

        Create NlcPtoPMediaListPlayer with specific NlcPtoPMediaPlayer core.

        \param player player object
        \param instance instance object
    */
    explicit NlcPtoPMediaListPlayer(NlcPtoPMediaPlayer *player,
                                NlcPtoPInstance *instance);

    /*!
        NlcPtoPMediaListPlayer destructor
    */
    ~NlcPtoPMediaListPlayer();

    /*!
        \brief Returns libgotvptop media list player object.
        \return libgotvptop media list player (libgotvptop_media_list_player_t *)
    */
    libgotvptop_media_list_player_t *core();

    /*!
        \brief Get current media list
        \return current media list (NlcPtoPMediaList *)
    */
    NlcPtoPMediaList *currentMediaList();

    /*!
        \brief Get media player core
        \return media player core (NlcPtoPMediaPlayer *)
    */
    NlcPtoPMediaPlayer *mediaPlayer();

    /*!
        \brief Get playback mode
        \return playback mode
    */
    NlcPtoP::PlaybackMode playbackMode() const;

    /*!
        \brief Set media list to play
        \param list media list object (NlcPtoPMediaList *)
    */
    void setMediaList(NlcPtoPMediaList *list);

    /*!
        \brief Set playback mode
        \param mode playback mode
    */
    void setPlaybackMode(const NlcPtoP::PlaybackMode &mode);

public slots:
    /*!
        \brief Starts playing item at specific position.
        \param index item position
    */
    void itemAt(int index);

    /*!
        \brief Starts playing next item in the list.
    */
    void next();

    /*!
        \brief Starts playing media list.
    */
    void play();

    /*!
        \brief Starts playing previous item in the list.
    */
    void previous();

    /*!
        \brief Stops playing
    */
    void stop();

signals:
    /*!
        \brief Signal sent when played
    */
    void played();

    /*!
        \brief Signal sent on next item set
        \param media next media item
        \since GOTV-Qt 1.1
    */
    void nextItemSet(NlcPtoPMedia *media);

    /*!
        \brief Signal sent on next item set
        \param media next media item
    */
    void nextItemSet(libgotvptop_media_t *media);

    /*!
        \brief Signal sent when stopped
    */
    void stopped();

private:
    static void libgotvptop_callback(const libgotvptop_event_t *event,
                                void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libgotvptop_media_list_player_t *_gotvptopMediaListPlayer;
    libgotvptop_event_manager_t *_gotvptopEvents;

    NlcPtoPMediaList *_list;
    NlcPtoPMediaPlayer *_player;

    NlcPtoP::PlaybackMode _mode;
};

#endif // GOTVQT_MEDIALISTPLAYER_H_
