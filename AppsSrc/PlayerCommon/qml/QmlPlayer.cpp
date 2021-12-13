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

GoTvPtoPQmlPlayer::GoTvPtoPQmlPlayer(QObject *parent)
    : GoTvPtoPQmlSource(parent),
      _media(0),
      _autoplay(true),
      _deinterlacing(GoTvPtoP::Disabled),
      _audioPreferredLanguages(QStringList()),
      _subtitlePreferredLanguages(QStringList())
{
    _instance = new GoTvPtoPInstance(GoTvPtoPCommon::args(), this);
    _player = new GoTvPtoPMediaPlayer(_instance);

    _audioTrackModel = new GoTvPtoPTrackModel(this);
    _subtitleTrackModel = new GoTvPtoPTrackModel(this);
    _videoTrackModel = new GoTvPtoPTrackModel(this);

    connect(_player, &GoTvPtoPMediaPlayer::lengthChanged, this, &GoTvPtoPQmlPlayer::lengthChanged);
    connect(_player, &GoTvPtoPMediaPlayer::positionChanged, this, &GoTvPtoPQmlPlayer::positionChanged);
    connect(_player, &GoTvPtoPMediaPlayer::seekableChanged, this, &GoTvPtoPQmlPlayer::seekableChanged);
    connect(_player, &GoTvPtoPMediaPlayer::stateChanged, this, &GoTvPtoPQmlPlayer::stateChanged);
    connect(_player, &GoTvPtoPMediaPlayer::timeChanged, this, &GoTvPtoPQmlPlayer::timeChanged);
    connect(_player, &GoTvPtoPMediaPlayer::vout, this, &GoTvPtoPQmlPlayer::mediaPlayerVout);

    setPlayer(_player);
}

GoTvPtoPQmlPlayer::~GoTvPtoPQmlPlayer()
{
    _player->stop();
    removePlayer();

    if (_media)
        delete _media;

    delete _player;
    delete _instance;
}

void GoTvPtoPQmlPlayer::pause()
{
    _player->pause();
}

void GoTvPtoPQmlPlayer::play()
{
    _player->play();
}

void GoTvPtoPQmlPlayer::stop()
{
    _player->stop();
}

bool GoTvPtoPQmlPlayer::autoplay() const
{
    return _autoplay;
}

void GoTvPtoPQmlPlayer::setAutoplay(bool autoplay)
{
    if (_autoplay == autoplay)
        return;

    _autoplay = autoplay;
    emit autoplayChanged();
}

int GoTvPtoPQmlPlayer::deinterlacing() const
{
    return _deinterlacing;
}

void GoTvPtoPQmlPlayer::setDeinterlacing(int deinterlacing)
{
    if (_deinterlacing == deinterlacing)
        return;

    _deinterlacing = GoTvPtoP::Deinterlacing(deinterlacing);
    _player->video()->setDeinterlace(_deinterlacing);
    emit deinterlacingChanged();
}

qint64 GoTvPtoPQmlPlayer::length() const
{
    return _player->length();
}

int GoTvPtoPQmlPlayer::logLevel() const
{
    return _instance->logLevel();
}

void GoTvPtoPQmlPlayer::setLogLevel(int level)
{
    if (level == logLevel())
        return;

    _instance->setLogLevel(GoTvPtoP::LogLevel(level));
    emit logLevelChanged();
}

bool GoTvPtoPQmlPlayer::seekable() const
{
    return _player->seekable();
}

int GoTvPtoPQmlPlayer::state() const
{
    return _player->state();
}

float GoTvPtoPQmlPlayer::position() const
{
    return _player->position();
}

void GoTvPtoPQmlPlayer::setPosition(float position)
{
    _player->setPosition(position);
}

qint64 GoTvPtoPQmlPlayer::time() const
{
    return _player->time();
}

void GoTvPtoPQmlPlayer::setTime(qint64 time)
{
    _player->setTime(time);
}

QUrl GoTvPtoPQmlPlayer::url() const
{
    if (_media)
        return QUrl(_media->currentLocation());
    else
        return QUrl();
}

void GoTvPtoPQmlPlayer::setUrl(const QUrl &url)
{
    if (url == GoTvPtoPQmlPlayer::url())
        return;

    _player->stop();

    if (_media)
        _media->deleteLater();

    if (url.isLocalFile()) {
        _media = new GoTvPtoPMedia(url.toLocalFile(), true, _instance);
    } else {
        _media = new GoTvPtoPMedia(url.toString(QUrl::FullyEncoded), false, _instance);
    }

    connect(_media, static_cast<void (GoTvPtoPMedia::*)(bool)>(&GoTvPtoPMedia::parsedChanged), this, &GoTvPtoPQmlPlayer::mediaParsed);

    openInternal();

    emit urlChanged();
}

int GoTvPtoPQmlPlayer::volume() const
{
    return _player->audio()->volume();
}

void GoTvPtoPQmlPlayer::setVolume(int volume)
{
    if (volume == GoTvPtoPQmlPlayer::volume())
        return;

    _player->audio()->setVolume(volume);
    emit volumeChanged();
}

GoTvPtoPTrackModel *GoTvPtoPQmlPlayer::audioTrackModel() const
{
    return _audioTrackModel;
}

int GoTvPtoPQmlPlayer::audioTrack() const
{
    return _player->audio()->track();
}

void GoTvPtoPQmlPlayer::setAudioTrack(int audioTrack)
{
    if (audioTrack == GoTvPtoPQmlPlayer::audioTrack())
        return;

    _player->audio()->setTrack(audioTrack);
    emit audioTrackChanged();
}

QStringList GoTvPtoPQmlPlayer::audioPreferredLanguages() const
{
    return _audioPreferredLanguages;
}

void GoTvPtoPQmlPlayer::setAudioPreferredLanguages(const QStringList &audioPreferredLanguages)
{
    if (_audioPreferredLanguages == audioPreferredLanguages)
        return;

    _audioPreferredLanguages = audioPreferredLanguages;
    emit audioPreferredLanguagesChanged();
}

GoTvPtoPTrackModel *GoTvPtoPQmlPlayer::subtitleTrackModel() const
{
    return _subtitleTrackModel;
}

int GoTvPtoPQmlPlayer::subtitleTrack() const
{
    return _player->video()->subtitle();
}

void GoTvPtoPQmlPlayer::setSubtitleTrack(int subtitleTrack)
{
    if (subtitleTrack == GoTvPtoPQmlPlayer::subtitleTrack())
        return;

    _player->video()->setSubtitle(subtitleTrack);
    emit subtitleTrackChanged();
}

QStringList GoTvPtoPQmlPlayer::subtitlePreferredLanguages() const
{
    return _subtitlePreferredLanguages;
}

void GoTvPtoPQmlPlayer::setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages)
{
    if (_subtitlePreferredLanguages == subtitlePreferredLanguages)
        return;

    _subtitlePreferredLanguages = subtitlePreferredLanguages;
    emit subtitlePreferredLanguagesChanged();
}

GoTvPtoPTrackModel *GoTvPtoPQmlPlayer::videoTrackModel() const
{
    return _videoTrackModel;
}

int GoTvPtoPQmlPlayer::videoTrack() const
{
    return _player->video()->track();
}

void GoTvPtoPQmlPlayer::setVideoTrack(int videoTrack)
{
    if (videoTrack == GoTvPtoPQmlPlayer::videoTrack())
        return;

    _player->video()->setTrack(videoTrack);
    emit videoTrackChanged();
}

void GoTvPtoPQmlPlayer::mediaParsed(bool parsed)
{
    if (parsed) {
        _audioTrackModel->clear();
        _audioTrackModel->load(_player->audio()->tracks());

        setAudioTrack(preferredAudioTrackId());
    }
}

void GoTvPtoPQmlPlayer::mediaPlayerVout(int)
{
    _subtitleTrackModel->clear();
    _subtitleTrackModel->load(_player->video()->subtitles());

    setSubtitleTrack(preferredSubtitleTrackId());

    _videoTrackModel->clear();
    _videoTrackModel->load(_player->video()->tracks());

    setVideoTrack(_player->video()->track());
}

void GoTvPtoPQmlPlayer::openInternal()
{
    if (_autoplay)
        _player->open(_media);
    else
        _player->openOnly(_media);
}

int GoTvPtoPQmlPlayer::preferredAudioTrackId()
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

int GoTvPtoPQmlPlayer::preferredSubtitleTrackId()
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
