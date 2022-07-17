/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "core/Audio.h"
#include "core/Common.h"
#include "core/Instance.h"
#include "core/MediaPlayer.h"
#include "core/Media.h"
#include "core/TrackModel.h"
#include "core/Video.h"

#include "qml/QmlPlayer.h"
#include "qml/QmlSource.h"

NlcPtoPQmlPlayer::NlcPtoPQmlPlayer(QObject *parent)
    : NlcPtoPQmlSource(parent),
      _media(0),
      _autoplay(true),
      _deinterlacing(NlcPtoP::Disabled),
      _audioPreferredLanguages(QStringList()),
      _subtitlePreferredLanguages(QStringList())
{
    _instance = new NlcPtoPInstance(NlcPtoPCommon::args(), this);
    _player = new NlcPtoPMediaPlayer(_instance);

    _audioTrackModel = new NlcPtoPTrackModel(this);
    _subtitleTrackModel = new NlcPtoPTrackModel(this);
    _videoTrackModel = new NlcPtoPTrackModel(this);

    connect(_player, &NlcPtoPMediaPlayer::lengthChanged, this, &NlcPtoPQmlPlayer::lengthChanged);
    connect(_player, &NlcPtoPMediaPlayer::positionChanged, this, &NlcPtoPQmlPlayer::positionChanged);
    connect(_player, &NlcPtoPMediaPlayer::seekableChanged, this, &NlcPtoPQmlPlayer::seekableChanged);
    connect(_player, &NlcPtoPMediaPlayer::stateChanged, this, &NlcPtoPQmlPlayer::stateChanged);
    connect(_player, &NlcPtoPMediaPlayer::timeChanged, this, &NlcPtoPQmlPlayer::timeChanged);
    connect(_player, &NlcPtoPMediaPlayer::vout, this, &NlcPtoPQmlPlayer::mediaPlayerVout);

    setPlayer(_player);
}

NlcPtoPQmlPlayer::~NlcPtoPQmlPlayer()
{
    _player->stop();
    removePlayer();

    if (_media)
        delete _media;

    delete _player;
    delete _instance;
}

void NlcPtoPQmlPlayer::pause()
{
    _player->pause();
}

void NlcPtoPQmlPlayer::play()
{
    _player->play();
}

void NlcPtoPQmlPlayer::stop()
{
    _player->stop();
}

bool NlcPtoPQmlPlayer::autoplay() const
{
    return _autoplay;
}

void NlcPtoPQmlPlayer::setAutoplay(bool autoplay)
{
    if (_autoplay == autoplay)
        return;

    _autoplay = autoplay;
    emit autoplayChanged();
}

int NlcPtoPQmlPlayer::deinterlacing() const
{
    return _deinterlacing;
}

void NlcPtoPQmlPlayer::setDeinterlacing(int deinterlacing)
{
    if (_deinterlacing == deinterlacing)
        return;

    _deinterlacing = NlcPtoP::Deinterlacing(deinterlacing);
    _player->video()->setDeinterlace(_deinterlacing);
    emit deinterlacingChanged();
}

qint64 NlcPtoPQmlPlayer::length() const
{
    return _player->length();
}

int NlcPtoPQmlPlayer::logLevel() const
{
    return _instance->logLevel();
}

void NlcPtoPQmlPlayer::setLogLevel(int level)
{
    if (level == logLevel())
        return;

    _instance->setLogLevel(NlcPtoP::LogLevel(level));
    emit logLevelChanged();
}

bool NlcPtoPQmlPlayer::seekable() const
{
    return _player->seekable();
}

int NlcPtoPQmlPlayer::state() const
{
    return _player->state();
}

float NlcPtoPQmlPlayer::position() const
{
    return _player->position();
}

void NlcPtoPQmlPlayer::setPosition(float position)
{
    _player->setPosition(position);
}

qint64 NlcPtoPQmlPlayer::time() const
{
    return _player->time();
}

void NlcPtoPQmlPlayer::setTime(qint64 time)
{
    _player->setTime(time);
}

QUrl NlcPtoPQmlPlayer::url() const
{
    if (_media)
        return QUrl(_media->currentLocation());
    else
        return QUrl();
}

void NlcPtoPQmlPlayer::setUrl(const QUrl &url)
{
    if (url == NlcPtoPQmlPlayer::url())
        return;

    _player->stop();

    if (_media)
        _media->deleteLater();

    if (url.isLocalFile()) {
        _media = new NlcPtoPMedia(url.toLocalFile(), true, _instance);
    } else {
        _media = new NlcPtoPMedia(url.toString(QUrl::FullyEncoded), false, _instance);
    }

    connect(_media, static_cast<void (NlcPtoPMedia::*)(bool)>(&NlcPtoPMedia::parsedChanged), this, &NlcPtoPQmlPlayer::mediaParsed);

    openInternal();

    emit urlChanged();
}

int NlcPtoPQmlPlayer::volume() const
{
    return _player->audio()->volume();
}

void NlcPtoPQmlPlayer::setVolume(int volume)
{
    if (volume == NlcPtoPQmlPlayer::volume())
        return;

    _player->audio()->setVolume(volume);
    emit volumeChanged();
}

NlcPtoPTrackModel *NlcPtoPQmlPlayer::audioTrackModel() const
{
    return _audioTrackModel;
}

int NlcPtoPQmlPlayer::audioTrack() const
{
    return _player->audio()->track();
}

void NlcPtoPQmlPlayer::setAudioTrack(int audioTrack)
{
    if (audioTrack == NlcPtoPQmlPlayer::audioTrack())
        return;

    _player->audio()->setTrack(audioTrack);
    emit audioTrackChanged();
}

QStringList NlcPtoPQmlPlayer::audioPreferredLanguages() const
{
    return _audioPreferredLanguages;
}

void NlcPtoPQmlPlayer::setAudioPreferredLanguages(const QStringList &audioPreferredLanguages)
{
    if (_audioPreferredLanguages == audioPreferredLanguages)
        return;

    _audioPreferredLanguages = audioPreferredLanguages;
    emit audioPreferredLanguagesChanged();
}

NlcPtoPTrackModel *NlcPtoPQmlPlayer::subtitleTrackModel() const
{
    return _subtitleTrackModel;
}

int NlcPtoPQmlPlayer::subtitleTrack() const
{
    return _player->video()->subtitle();
}

void NlcPtoPQmlPlayer::setSubtitleTrack(int subtitleTrack)
{
    if (subtitleTrack == NlcPtoPQmlPlayer::subtitleTrack())
        return;

    _player->video()->setSubtitle(subtitleTrack);
    emit subtitleTrackChanged();
}

QStringList NlcPtoPQmlPlayer::subtitlePreferredLanguages() const
{
    return _subtitlePreferredLanguages;
}

void NlcPtoPQmlPlayer::setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages)
{
    if (_subtitlePreferredLanguages == subtitlePreferredLanguages)
        return;

    _subtitlePreferredLanguages = subtitlePreferredLanguages;
    emit subtitlePreferredLanguagesChanged();
}

NlcPtoPTrackModel *NlcPtoPQmlPlayer::videoTrackModel() const
{
    return _videoTrackModel;
}

int NlcPtoPQmlPlayer::videoTrack() const
{
    return _player->video()->track();
}

void NlcPtoPQmlPlayer::setVideoTrack(int videoTrack)
{
    if (videoTrack == NlcPtoPQmlPlayer::videoTrack())
        return;

    _player->video()->setTrack(videoTrack);
    emit videoTrackChanged();
}

void NlcPtoPQmlPlayer::mediaParsed(bool parsed)
{
    if (parsed) {
        _audioTrackModel->clear();
        _audioTrackModel->load(_player->audio()->tracks());

        setAudioTrack(preferredAudioTrackId());
    }
}

void NlcPtoPQmlPlayer::mediaPlayerVout(int)
{
    _subtitleTrackModel->clear();
    _subtitleTrackModel->load(_player->video()->subtitles());

    setSubtitleTrack(preferredSubtitleTrackId());

    _videoTrackModel->clear();
    _videoTrackModel->load(_player->video()->tracks());

    setVideoTrack(_player->video()->track());
}

void NlcPtoPQmlPlayer::openInternal()
{
    if (_autoplay)
        _player->open(_media);
    else
        _player->openOnly(_media);
}

int NlcPtoPQmlPlayer::preferredAudioTrackId()
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

int NlcPtoPQmlPlayer::preferredSubtitleTrackId()
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
