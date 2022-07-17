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
#include <QtGui/QMouseEvent>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#endif

#include "core/Audio.h"
#include "core/Error.h"
#include "core/MediaPlayer.h"
#include "widgets/WidgetVolumeSlider.h"

NlcPtoPWidgetVolumeSlider::NlcPtoPWidgetVolumeSlider(NlcPtoPMediaPlayer *player,
                                             QWidget *parent)
    : QSlider(parent),
      _gotvptopAudio(player->audio()),
      _gotvptopMediaPlayer(player)
{
    initWidgetVolumeSlider();
}

NlcPtoPWidgetVolumeSlider::NlcPtoPWidgetVolumeSlider(QWidget *parent)
    : QSlider(parent),
      _gotvptopAudio(0),
      _gotvptopMediaPlayer(0)
{
    initWidgetVolumeSlider();
}

NlcPtoPWidgetVolumeSlider::~NlcPtoPWidgetVolumeSlider()
{
    delete _timer;
}

void NlcPtoPWidgetVolumeSlider::initWidgetVolumeSlider()
{
    _lock = false;

    _timer = new QTimer(this);

    connect(_timer, SIGNAL(timeout()), this, SLOT(updateVolume()));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
}

void NlcPtoPWidgetVolumeSlider::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    lock();
}

void NlcPtoPWidgetVolumeSlider::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

    unlock();
}

void NlcPtoPWidgetVolumeSlider::setMediaPlayer(NlcPtoPMediaPlayer *player)
{
    _gotvptopAudio = player->audio();
    _gotvptopMediaPlayer = player;

    _timer->start(100);
}

bool NlcPtoPWidgetVolumeSlider::mute() const
{
    if (!(_gotvptopMediaPlayer->state() == NlcPtoP::Buffering
          || _gotvptopMediaPlayer->state() == NlcPtoP::Playing
          || _gotvptopMediaPlayer->state() == NlcPtoP::Paused))
        return false;
    else
        return _gotvptopAudio->getMute();
}

void NlcPtoPWidgetVolumeSlider::setMute(bool enabled)
{
    if (!(_gotvptopMediaPlayer->state() == NlcPtoP::Buffering
          || _gotvptopMediaPlayer->state() == NlcPtoP::Playing
          || _gotvptopMediaPlayer->state() == NlcPtoP::Paused))
        return;

    if (!enabled) {
        _timer->start(100);
        setDisabled(false);
    } else {
        _timer->stop();
        setDisabled(true);
    }

    _gotvptopAudio->toggleMute();
}

void NlcPtoPWidgetVolumeSlider::setVolume(int volume)
{
    if (_currentVolume == volume)
        return;

    lock();

    _currentVolume = volume;
    setValue(_currentVolume);

    emit newVolume(_currentVolume);

    unlock();
}

void NlcPtoPWidgetVolumeSlider::updateVolume()
{
    if (_lock)
        return;

    if (!_gotvptopMediaPlayer)
        return;

    if (_gotvptopMediaPlayer->state() == NlcPtoP::Buffering
        || _gotvptopMediaPlayer->state() == NlcPtoP::Playing
        || _gotvptopMediaPlayer->state() == NlcPtoP::Paused)
        _gotvptopAudio->setVolume(_currentVolume);
}

int NlcPtoPWidgetVolumeSlider::volume() const
{
    return _currentVolume;
}

void NlcPtoPWidgetVolumeSlider::volumeControl(bool up)
{
    if (up) {
        if (_currentVolume != 200) {
            setVolume(_currentVolume + 1);
        }
    } else {
        if (_currentVolume != 0) {
            setVolume(_currentVolume - 1);
        }
    }
}

void NlcPtoPWidgetVolumeSlider::lock()
{
    _lock = true;
}

void NlcPtoPWidgetVolumeSlider::unlock()
{
    _lock = false;
}
