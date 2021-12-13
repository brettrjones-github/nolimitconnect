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

#include <QtCore/QDebug>
#include <QtQml/QQmlEngine>

#include "config_gotvapps.h"

#include "core/Video.h"
#include "core/Audio.h"
#include "core/Common.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaPlayer.h"
#include "core/TrackModel.h"

#include "qml/QmlVideoPlayer.h"

GoTvPtoPQmlVideoPlayer::GoTvPtoPQmlVideoPlayer(QQuickItem *parent)
    : GoTvPtoPQmlVideoObject(parent),
      _instance(0),
      _media(0),
      _audioManager(0),
      _videoManager(0),
      _deinterlacing(GoTvPtoP::Disabled),
      _hasMedia(false),
      _autoplay(true),
      _seekable(true)

{
    _instance = new GoTvPtoPInstance(GoTvPtoPCommon::args(), this);
    _instance->setUserAgent(qApp->applicationName(), qApp->applicationVersion());
    _player = new GoTvPtoPMediaPlayer(_instance);
    _audioManager = new GoTvPtoPAudio(_player);
    _videoManager = new GoTvPtoPVideo(_player);

    connect(_player, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
    connect(_player, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChangedPrivate(bool)));
    connect(_player, SIGNAL(lengthChanged(int)), this, SIGNAL(lengthChanged()));
    connect(_player, SIGNAL(timeChanged(int)), this, SIGNAL(timeChanged()));
    connect(_player, SIGNAL(positionChanged(float)), this, SIGNAL(positionChanged()));
    connect(_player, SIGNAL(vout(int)), this, SLOT(mediaPlayerVout(int)));

    _audioTrackModel = new GoTvPtoPTrackModel(this);
    _subtitleTrackModel = new GoTvPtoPTrackModel(this);
    _videoTrackModel = new GoTvPtoPTrackModel(this);
}

GoTvPtoPQmlVideoPlayer::~GoTvPtoPQmlVideoPlayer()
{
    _player->stop();

    delete _audioManager;
    delete _videoManager;
    delete _media;
    delete _player;
    delete _instance;
}

void GoTvPtoPQmlVideoPlayer::registerPlugin()
{
    qmlRegisterType<GoTvPtoPQmlVideoPlayer>("GOTVQt", 1, 0, "GoTvPtoPVideoPlayer");
}

void GoTvPtoPQmlVideoPlayer::openInternal()
{
    if(_autoplay)
        _player->open(_media);
    else
        _player->openOnly(_media);

    connectToMediaPlayer(_player);

    _hasMedia = true;
}

int GoTvPtoPQmlVideoPlayer::preferredAudioTrackId()
{
    int currentTrackId = _player->audio()->track();
    if (_audioTrackModel->count() && _audioPreferredLanguages.count()) {
        bool found = false;
        for (int j = 0; !found && j < _audioPreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _audioTrackModel->count(); i++) {
                QString trackTitle = _audioTrackModel->data(i, GoTvPtoPTrackModel::TitleRole).toString();
                if (trackTitle.contains(_audioPreferredLanguages.at(j))) {
                    currentTrackId = _audioTrackModel->data(i, GoTvPtoPTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}

int GoTvPtoPQmlVideoPlayer::preferredSubtitleTrackId()
{
    int currentTrackId = _player->video()->subtitle();
    if (_subtitleTrackModel->count()) {
        bool found = false;
        for (int j = 0; !found && j < _subtitlePreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _subtitleTrackModel->count(); i++) {
                QString trackTitle = _subtitleTrackModel->data(i, GoTvPtoPTrackModel::TitleRole).toString();
                if (trackTitle.contains(_subtitlePreferredLanguages.at(j))) {
                    currentTrackId = _subtitleTrackModel->data(i, GoTvPtoPTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}

GoTvPtoPTrackModel *GoTvPtoPQmlVideoPlayer::audioTrackModel() const
{
    return _audioTrackModel;
}

int GoTvPtoPQmlVideoPlayer::audioTrack() const
{
    return _audioManager->track();
}

void GoTvPtoPQmlVideoPlayer::setAudioTrack(int audioTrack)
{
    _audioManager->setTrack(audioTrack);
    emit audioTrackChanged();
}

QStringList GoTvPtoPQmlVideoPlayer::audioPreferredLanguages() const
{
    return _audioPreferredLanguages;
}

void GoTvPtoPQmlVideoPlayer::setAudioPreferredLanguages(const QStringList &audioPreferredLanguages)
{
    _audioPreferredLanguages = audioPreferredLanguages;
    emit audioPreferredLanguagesChanged();
}

int GoTvPtoPQmlVideoPlayer::subtitleTrack() const
{
    return _videoManager->subtitle();
}

void GoTvPtoPQmlVideoPlayer::setSubtitleTrack(int subtitleTrack)
{
    _videoManager->setSubtitle(subtitleTrack);
    emit subtitleTrackChanged();
}

QStringList GoTvPtoPQmlVideoPlayer::subtitlePreferredLanguages() const
{
    return _subtitlePreferredLanguages;
}

void GoTvPtoPQmlVideoPlayer::setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages)
{
    _subtitlePreferredLanguages = subtitlePreferredLanguages;
    emit subtitlePreferredLanguagesChanged();
}

GoTvPtoPTrackModel *GoTvPtoPQmlVideoPlayer::subtitleTrackModel() const
{
    return _subtitleTrackModel;
}

int GoTvPtoPQmlVideoPlayer::videoTrack() const
{
    return _videoManager->track();
}

void GoTvPtoPQmlVideoPlayer::setVideoTrack(int videoTrack)
{
    _videoManager->setTrack(videoTrack);
    emit videoTrackChanged();
}

GoTvPtoPTrackModel *GoTvPtoPQmlVideoPlayer::videoTrackModel() const
{
    return _videoTrackModel;
}

QString GoTvPtoPQmlVideoPlayer::deinterlacing() const
{
    return GoTvPtoP::deinterlacing()[_deinterlacing];
}

void GoTvPtoPQmlVideoPlayer::setDeinterlacing(const QString &deinterlacing)
{
    _deinterlacing = (GoTvPtoP::Deinterlacing) GoTvPtoP::deinterlacing().indexOf(deinterlacing);
    _player->video()->setDeinterlace(_deinterlacing);
    emit deinterlacingChanged();
}

int GoTvPtoPQmlVideoPlayer::state() const
{
    return (int)_player->state();
}

bool GoTvPtoPQmlVideoPlayer::seekable() const
{
    return _seekable;
}

int GoTvPtoPQmlVideoPlayer::length() const
{
    return _player->length();
}

int GoTvPtoPQmlVideoPlayer::time() const
{
    return _player->time();
}

void GoTvPtoPQmlVideoPlayer::setTime(int time)
{
    _player->setTime( time );
}

float GoTvPtoPQmlVideoPlayer::position() const
{
    return _player->position();
}

void GoTvPtoPQmlVideoPlayer::setPosition(float position)
{
    _player->setPosition( position );
}

void GoTvPtoPQmlVideoPlayer::seekableChangedPrivate(bool seekable)
{
    _seekable = seekable;
    emit seekableChanged();
}

void GoTvPtoPQmlVideoPlayer::mediaParsed(bool parsed)
{
    if(parsed == 1)
    {
        _audioTrackModel->clear();
        _audioTrackModel->load(_audioManager->tracks());

        setAudioTrack(preferredAudioTrackId());
    }
}

void GoTvPtoPQmlVideoPlayer::mediaPlayerVout(int)
{
    _subtitleTrackModel->clear();
    _subtitleTrackModel->load(_videoManager->subtitles());

    setSubtitleTrack(preferredSubtitleTrackId());

    _videoTrackModel->clear();
    _videoTrackModel->load(_videoManager->tracks());

    setVideoTrack(_videoManager->track());
}

bool GoTvPtoPQmlVideoPlayer::autoplay() const
{
    return _autoplay;
}

void GoTvPtoPQmlVideoPlayer::setAutoplay(bool autoplay)
{
    _autoplay = autoplay;
}

QUrl GoTvPtoPQmlVideoPlayer::url() const
{
    if (_media)
        return QUrl(_media->currentLocation());
    else
        return QUrl();
}

void GoTvPtoPQmlVideoPlayer::setUrl(const QUrl &url)
{
    _player->stop();

    if (_media)
        _media->deleteLater();

    if(url.isLocalFile()) {
        _media = new GoTvPtoPMedia(url.toLocalFile(), true, _instance);
    } else {
        _media = new GoTvPtoPMedia(url.toString(QUrl::FullyEncoded), false, _instance);
    }

    connect(_media, static_cast<void (GoTvPtoPMedia::*)(bool)>(&GoTvPtoPMedia::parsedChanged), this, &GoTvPtoPQmlVideoPlayer::mediaParsed);

    openInternal();
}

void GoTvPtoPQmlVideoPlayer::pause()
{
    _player->pause();
}

void GoTvPtoPQmlVideoPlayer::play()
{
    _player->play();
}

void GoTvPtoPQmlVideoPlayer::stop()
{
    _player->stop();
    disconnectFromMediaPlayer(_player);
}

int GoTvPtoPQmlVideoPlayer::volume() const
{
    return _audioManager->volume();
}

void GoTvPtoPQmlVideoPlayer::setVolume(int volume)
{
    _audioManager->setVolume(volume);
    emit volumeChanged();
}

QString GoTvPtoPQmlVideoPlayer::aspectRatio() const
{
    return GoTvPtoP::ratio()[GoTvPtoPQmlVideoObject::aspectRatio()];
}

void GoTvPtoPQmlVideoPlayer::setAspectRatio(const QString &aspectRatio)
{
    GoTvPtoPQmlVideoObject::setAspectRatio( (GoTvPtoP::Ratio) GoTvPtoP::ratio().indexOf(aspectRatio) );
    emit aspectRatioChanged();
}

QString GoTvPtoPQmlVideoPlayer::cropRatio() const
{
    return GoTvPtoP::ratio()[GoTvPtoPQmlVideoObject::cropRatio()];
}

void GoTvPtoPQmlVideoPlayer::setCropRatio(const QString &cropRatio)
{
    GoTvPtoPQmlVideoObject::setCropRatio( (GoTvPtoP::Ratio) GoTvPtoP::ratio().indexOf(cropRatio) );
    emit cropRatioChanged();
}
