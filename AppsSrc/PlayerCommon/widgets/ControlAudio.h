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

#ifndef GOTVQT_CONTROLAUDIO_H_
#define GOTVQT_CONTROLAUDIO_H_

#include <QtCore/QList>
#include <QtCore/QMap>

#include <core/Enums.h>

#include "SharedExportWidgets.h"

class QAction;
class QTimer;

class GoTvPtoPAudio;
class GoTvPtoPMediaPlayer;

/*!
    \class GoTvPtoPControlAudio ControlAudio.h GOTVQtWidgets/ControlAudio.h
    \ingroup GOTVQtWidgets
    \brief Audio control class

    This is one of GOTV-Qt control classes.
    It provides audio tracks management.
*/
class GOTVQT_WIDGETS_EXPORT GoTvPtoPControlAudio : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief GoTvPtoPControlAudio constructor
        \param player media player
        \param language default audio language
        \param parent audio controller's parent object
    */
    explicit GoTvPtoPControlAudio(GoTvPtoPMediaPlayer *player,
                             const QString &language = 0,
                             QObject *parent = 0);

    /*!
        \brief AudioControl destructor
    */
    ~GoTvPtoPControlAudio();

    /*!
        \brief Reset all settings and selected audio tracks.

        Usually called on media change.
    */
    void reset();

    /*!
        \brief Set default audio language
        \param language comma separated languages
    */
    void setDefaultAudioLanguage(const QString &language);

signals:
    /*!
        \brief Signal sending actions for changing audio tracks
        \param actions list of actions
        \param type type of actions
    */
    void actions(QList<QAction *> actions,
                 const GoTvPtoP::ActionsType type);

    /*!
        \brief Signal sending actions for changing audio tracks
        \param actions list of actions
    */
    void audioTracks(QList<QAction *> actions);

private slots:
    void clean();
    void update();
    void updateActions();

private:
    GoTvPtoPAudio *_gotvptopAudio;
    GoTvPtoPMediaPlayer *_gotvptopMediaPlayer;

    QTimer *_timer;

    QList<QAction *> _actionList;
    QMap<QString, int> _desc;
    QMap<int, int> _id;

    bool _manualLanguage;
    QStringList _preferedLanguage;
};

#endif // GOTVQT_CONTROLAUDIO_H_
