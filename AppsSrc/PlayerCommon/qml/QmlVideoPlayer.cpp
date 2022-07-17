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

NlcPtoPQmlVideoPlayer::NlcPtoPQmlVideoPlayer(QQuickItem *parent)
    : NlcPtoPQmlVideoObject(parent),
      _instance(0),
      _media(0),
      _audioManager(0),
      _videoManager(0),
      _deinterlacing(NlcPtoP::Disabled),
      _hasMedia(false),
      _autoplay(true),
      _seekable(true)

{
    _instance = new NlcPtoPInstance(NlcPtoPCommon::args(), this);
    _instance->setUserAgent(qApp->applicationName(), qApp->applicationVersion());
    _player = new NlcPtoPMediaPlayer(_instance);
    _audioManager = new NlcPtoPAudio(_player);
    _videoManager = new NlcPtoPVideo(_player);

    connect(_player, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
    connect(_player, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChangedPrivate(bool)));
    connect(_player, SIGNAL(lengthChanged(int)), this, SIGNAL(lengthChanged()));
    connect(_player, SIGNAL(timeChanged(int)), this, SIGNAL(timeChanged()));
    connect(_player, SIGNAL(positionChanged(float)), this, SIGNAL(positionChanged()));
    connect(_player, SIGNAL(vout(int)), this, SLOT(mediaPlayerVout(int)));

    _audioTrackModel = new NlcPtoPTrackModel(this);
    _subtitleTrackModel = new NlcPtoPTrackModel(this);
    _videoTrackModel = new NlcPtoPTrackModel(this);
}

NlcPtoPQmlVideoPlayer::~NlcPtoPQmlVideoPlayer()
{
    _player->stop();

    delete _audioManager;
    delete _videoManager;
    delete _media;
    delete _player;
    delete _instance;
}

void NlcPtoPQmlVideoPlayer::registerPlugin()
{
    qmlRegisterType<NlcPtoPQmlVideoPlayer>("GOTVQt", 1, 0, "NlcPtoPVideoPlayer");
}

void NlcPtoPQmlVideoPlayer::openInternal()
{
    if(_autoplay)
        _player->open(_media);
    else
        _player->openOnly(_media);

    connectToMediaPlayer(_player);

    _hasMedia = true;
}

int NlcPtoPQmlVideoPlayer::preferredAudioTrackId()
{
    int currentTrackId = _player->audio()->track();
    if (_audioTrackModel->count() && _audioPreferredLanguages.count()) {
        bool found = false;
        for (int j = 0; !found && j < _audioPreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _audioTrackModel->count(); i++) {
                QString trackTitle = _audioTrackModel->data(i, NlcPtoPTrackModel::TitleRole).toString();
                if (trackTitle.contains(_audioPreferredLanguages.at(j))) {
                    currentTrackId = _audioTrackModel->data(i, NlcPtoPTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}

int NlcPtoPQmlVideoPlayer::preferredSubtitleTrackId()
{
    int currentTrackId = _player->video()->subtitle();
    if (_subtitleTrackModel->count()) {
        bool found = false;
        for (int j = 0; !found && j < _subtitlePreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _subtitleTrackModel->count(); i++) {
                QString trackTitle = _subtitleTrackModel->data(i, NlcPtoPTrackModel::TitleRole).toString();
                if (trackTitle.contains(_subtitlePreferredLanguages.at(j))) {
                    currentTrackId = _subtitleTrackModel->data(i, NlcPtoPTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}

NlcPtoPTrackModel *NlcPtoPQmlVideoPlayer::audioTrackModel() const
{
    return _audioTrackModel;
}

int NlcPtoPQmlVideoPlayer::audioTrack() const
{
    return _audioManager->track();
}

void NlcPtoPQmlVideoPlayer::setAudioTrack(int audioTrack)
{
    _audioManager->setTrack(audioTrack);
    emit audioTrackChanged();
}

QStringList NlcPtoPQmlVideoPlayer::audioPreferredLanguages() const
{
    return _audioPreferredLanguages;
}

void NlcPtoPQmlVideoPlayer::setAudioPreferredLanguages(const QStringList &audioPreferredLanguages)
{
    _audioPreferredLanguages = audioPreferredLanguages;
    emit audioPreferredLanguagesChanged();
}

int NlcPtoPQmlVideoPlayer::subtitleTrack() const
{
    return _videoManager->subtitle();
}

void NlcPtoPQmlVideoPlayer::setSubtitleTrack(int subtitleTrack)
{
    _videoManager->setSubtitle(subtitleTrack);
    emit subtitleTrackChanged();
}

QStringList NlcPtoPQmlVideoPlayer::subtitlePreferredLanguages() const
{
    return _subtitlePreferredLanguages;
}

void NlcPtoPQmlVideoPlayer::setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages)
{
    _subtitlePreferredLanguages = subtitlePreferredLanguages;
    emit subtitlePreferredLanguagesChanged();
}

NlcPtoPTrackModel *NlcPtoPQmlVideoPlayer::subtitleTrackModel() const
{
    return _subtitleTrackModel;
}

int NlcPtoPQmlVideoPlayer::videoTrack() const
{
    return _videoManager->track();
}

void NlcPtoPQmlVideoPlayer::setVideoTrack(int videoTrack)
{
    _videoManager->setTrack(videoTrack);
    emit videoTrackChanged();
}

NlcPtoPTrackModel *NlcPtoPQmlVideoPlayer::videoTrackModel() const
{
    return _videoTrackModel;
}

QString NlcPtoPQmlVideoPlayer::deinterlacing() const
{
    return NlcPtoP::deinterlacing()[_deinterlacing];
}

void NlcPtoPQmlVideoPlayer::setDeinterlacing(const QString &deinterlacing)
{
    _deinterlacing = (NlcPtoP::Deinterlacing) NlcPtoP::deinterlacing().indexOf(deinterlacing);
    _player->video()->setDeinterlace(_deinterlacing);
    emit deinterlacingChanged();
}

int NlcPtoPQmlVideoPlayer::state() const
{
    return (int)_player->state();
}

bool NlcPtoPQmlVideoPlayer::seekable() const
{
    return _seekable;
}

int NlcPtoPQmlVideoPlayer::length() const
{
    return _player->length();
}

int NlcPtoPQmlVideoPlayer::time() const
{
    return _player->time();
}

void NlcPtoPQmlVideoPlayer::setTime(int time)
{
    _player->setTime( time );
}

float NlcPtoPQmlVideoPlayer::position() const
{
    return _player->position();
}

void NlcPtoPQmlVideoPlayer::setPosition(float position)
{
    _player->setPosition( position );
}

void NlcPtoPQmlVideoPlayer::seekableChangedPrivate(bool seekable)
{
    _seekable = seekable;
    emit seekableChanged();
}

void NlcPtoPQmlVideoPlayer::mediaParsed(bool parsed)
{
    if(parsed == 1)
    {
        _audioTrackModel->clear();
        _audioTrackModel->load(_audioManager->tracks());

        setAudioTrack(preferredAudioTrackId());
    }
}

void NlcPtoPQmlVideoPlayer::mediaPlayerVout(int)
{
    _subtitleTrackModel->clear();
    _subtitleTrackModel->load(_videoManager->subtitles());

    setSubtitleTrack(preferredSubtitleTrackId());

    _videoTrackModel->clear();
    _videoTrackModel->load(_videoManager->tracks());

    setVideoTrack(_videoManager->track());
}

bool NlcPtoPQmlVideoPlayer::autoplay() const
{
    return _autoplay;
}

void NlcPtoPQmlVideoPlayer::setAutoplay(bool autoplay)
{
    _autoplay = autoplay;
}

QUrl NlcPtoPQmlVideoPlayer::url() const
{
    if (_media)
        return QUrl(_media->currentLocation());
    else
        return QUrl();
}

void NlcPtoPQmlVideoPlayer::setUrl(const QUrl &url)
{
    _player->stop();

    if (_media)
        _media->deleteLater();

    if(url.isLocalFile()) {
        _media = new NlcPtoPMedia(url.toLocalFile(), true, _instance);
    } else {
        _media = new NlcPtoPMedia(url.toString(QUrl::FullyEncoded), false, _instance);
    }

    connect(_media, static_cast<void (NlcPtoPMedia::*)(bool)>(&NlcPtoPMedia::parsedChanged), this, &NlcPtoPQmlVideoPlayer::mediaParsed);

    openInternal();
}

void NlcPtoPQmlVideoPlayer::pause()
{
    _player->pause();
}

void NlcPtoPQmlVideoPlayer::play()
{
    _player->play();
}

void NlcPtoPQmlVideoPlayer::stop()
{
    _player->stop();
    disconnectFromMediaPlayer(_player);
}

int NlcPtoPQmlVideoPlayer::volume() const
{
    return _audioManager->volume();
}

void NlcPtoPQmlVideoPlayer::setVolume(int volume)
{
    _audioManager->setVolume(volume);
    emit volumeChanged();
}

QString NlcPtoPQmlVideoPlayer::aspectRatio() const
{
    return NlcPtoP::ratio()[NlcPtoPQmlVideoObject::aspectRatio()];
}

void NlcPtoPQmlVideoPlayer::setAspectRatio(const QString &aspectRatio)
{
    NlcPtoPQmlVideoObject::setAspectRatio( (NlcPtoP::Ratio) NlcPtoP::ratio().indexOf(aspectRatio) );
    emit aspectRatioChanged();
}

QString NlcPtoPQmlVideoPlayer::cropRatio() const
{
    return NlcPtoP::ratio()[NlcPtoPQmlVideoObject::cropRatio()];
}

void NlcPtoPQmlVideoPlayer::setCropRatio(const QString &cropRatio)
{
    NlcPtoPQmlVideoObject::setCropRatio( (NlcPtoP::Ratio) NlcPtoP::ratio().indexOf(cropRatio) );
    emit cropRatioChanged();
}
