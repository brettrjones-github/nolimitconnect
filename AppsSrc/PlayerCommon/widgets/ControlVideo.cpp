/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QTimer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include "core/Error.h"
#include "core/MediaPlayer.h"
#include "core/Video.h"

#include "widgets/ControlVideo.h"

GoTvPtoPControlVideo::GoTvPtoPControlVideo(GoTvPtoPMediaPlayer *player,
                                 const QString &language,
                                 QObject *parent)
    : QObject(parent),
      _gotvptopMediaPlayer(player),
      _gotvptopVideo(player->video()),
      _actionSubList(QList<QAction *>()),
      _descSub(QMap<QString, int>()),
      _idSub(QMap<int, int>()),
      _actionVideoList(QList<QAction *>()),
      _descVideo(QMap<QString, int>()),
      _idVideo(QMap<int, int>()),
      _manualLanguage(false)
{
    if (!language.isNull() && !language.isEmpty())
        _preferedLanguage = language.split(" / ");

    _timerSubtitles = new QTimer(this);
    connect(_timerSubtitles, SIGNAL(timeout()), this, SLOT(updateSubtitleActions()));
    _timerVideo = new QTimer(this);
    connect(_timerVideo, SIGNAL(timeout()), this, SLOT(updateVideoActions()));

    _timerSubtitles->start(1000);
    _timerVideo->start(1000);
}

GoTvPtoPControlVideo::~GoTvPtoPControlVideo()
{
    delete _timerSubtitles;
    delete _timerVideo;
}

void GoTvPtoPControlVideo::updateSubtitleActions()
{
    qDeleteAll(_actionSubList);
    _actionSubList.clear();
    _descSub.clear();
    _idSub.clear();

    if (!(_gotvptopMediaPlayer->state() == GoTvPtoP::Playing || _gotvptopMediaPlayer->state() == GoTvPtoP::Paused)) {
        emit actions(_actionSubList, GoTvPtoP::Subtitles);
        emit subtitleTracks(_actionSubList);
        return;
    }

    if (_gotvptopVideo->subtitleCount() > 0) {
        QStringList desc = _gotvptopVideo->subtitleDescription();
        //QList<int> ids = _gotvptopVideo->subtitleIds();
        for (int i = 0; i < desc.size(); i++) {
            _descSub.insert(desc[i], i); //_descSub.insert(desc[i], ids[i]);
            _idSub.insert(i, i);         //_idSub.insert(ids[i], i);
            _actionSubList << new QAction(desc[i], this);
        }
    } else {
        emit actions(_actionSubList, GoTvPtoP::Subtitles);
        emit subtitleTracks(_actionSubList);
        _timerSubtitles->start(1000);
        return;
    }

    foreach (QAction *action, _actionSubList) {
        action->setCheckable(true);
        connect(action, SIGNAL(triggered()), this, SLOT(updateSubtitles()));

        if (!_manualLanguage) {
            foreach (const QString &language, _preferedLanguage) {
                if (action->text().contains(language, Qt::CaseInsensitive)) {
                    action->trigger();
                    _manualLanguage = true;
                }
            }
        }
    }

    _actionSubList[_idSub[_gotvptopVideo->subtitle()]]->setChecked(true);

    emit actions(_actionSubList, GoTvPtoP::Subtitles);
    emit subtitleTracks(_actionSubList);

    _timerSubtitles->start(60000);
}

void GoTvPtoPControlVideo::updateSubtitles()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    int id = _descSub.value(action->text());

    _gotvptopVideo->setSubtitle(id);
}

void GoTvPtoPControlVideo::loadSubtitle(const QString &subtitle)
{
    if (subtitle.isEmpty())
        return;

    _gotvptopVideo->setSubtitleFile(subtitle);

    _timerSubtitles->start(1000);
}

void GoTvPtoPControlVideo::updateVideoActions()
{
    qDeleteAll(_actionVideoList);
    _actionVideoList.clear();
    _descVideo.clear();
    _idVideo.clear();

    if (!(_gotvptopMediaPlayer->state() == GoTvPtoP::Playing || _gotvptopMediaPlayer->state() == GoTvPtoP::Paused)) {
        emit actions(_actionVideoList, GoTvPtoP::VideoTrack);
        emit videoTracks(_actionVideoList);
        return;
    }

    if (_gotvptopVideo->trackCount() > 0) {
        QStringList desc = _gotvptopVideo->trackDescription();
        QList<int> ids = _gotvptopVideo->trackIds();
        for (int i = 0; i < desc.size(); i++) {
            _descVideo.insert(desc[i], ids[i]);
            _idVideo.insert(ids[i], i);
            _actionVideoList << new QAction(desc[i], this);
        }
    } else {
        emit actions(_actionVideoList, GoTvPtoP::VideoTrack);
        emit videoTracks(_actionVideoList);
        _timerVideo->start(1000);
        return;
    }

    foreach (QAction *action, _actionVideoList) {
        action->setCheckable(true);
        connect(action, SIGNAL(triggered()), this, SLOT(updateVideo()));
    }

    _actionVideoList[_idVideo[_gotvptopVideo->track()]]->setChecked(true);

    emit actions(_actionVideoList, GoTvPtoP::VideoTrack);
    emit videoTracks(_actionVideoList);

    _timerVideo->start(60000);
}

void GoTvPtoPControlVideo::updateVideo()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    int id = _descVideo.value(action->text());

    _gotvptopVideo->setTrack(id);
}

void GoTvPtoPControlVideo::reset()
{
    _timerSubtitles->start(1000);
    _timerVideo->start(1000);
    _manualLanguage = false;
}

void GoTvPtoPControlVideo::setDefaultSubtitleLanguage(const QString &language)
{
    _preferedLanguage = language.split(" / ");
}
