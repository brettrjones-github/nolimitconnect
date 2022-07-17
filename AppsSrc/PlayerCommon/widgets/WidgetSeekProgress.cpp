/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QProgressBar>
#else
#include <QtGui/QProgressBar>
#endif

#include "core/Error.h"
#include "core/MediaPlayer.h"

#include "widgets/WidgetSeekProgress.h"

NlcPtoPWidgetSeekProgress::NlcPtoPWidgetSeekProgress(NlcPtoPMediaPlayer *player,
                                             QWidget *parent)
    : NlcPtoPWidgetSeek(player, new QProgressBar(), true, parent),
      _lock(false)
{
    _progress->setMaximumHeight(15);
}

NlcPtoPWidgetSeekProgress::NlcPtoPWidgetSeekProgress(QWidget *parent)
    : NlcPtoPWidgetSeek(new QProgressBar(), true, parent),
      _lock(false)
{
    _progress->setMaximumHeight(15);
}

NlcPtoPWidgetSeekProgress::~NlcPtoPWidgetSeekProgress() {}

void NlcPtoPWidgetSeekProgress::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

    if (!_lock)
        return;

    updateEvent(event->pos());
}

void NlcPtoPWidgetSeekProgress::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    lock();
}

void NlcPtoPWidgetSeekProgress::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

    updateEvent(event->pos());

    unlock();
}

void NlcPtoPWidgetSeekProgress::wheelEvent(QWheelEvent *event)
{
    event->ignore();

    if (!_gotvptopMediaPlayer)
        return;

    if (event->delta() > 0)
        _gotvptopMediaPlayer->setTime(_gotvptopMediaPlayer->time() + _gotvptopMediaPlayer->length() * 0.01);
    else
        _gotvptopMediaPlayer->setTime(_gotvptopMediaPlayer->time() - _gotvptopMediaPlayer->length() * 0.01);
}

void NlcPtoPWidgetSeekProgress::setSliderWidget(QWidget *slider,
                                            bool updateSlider)
{
    Q_UNUSED(slider)
    Q_UNUSED(updateSlider)

#if QT_VERSION >= 0x050000
    Q_UNIMPLEMENTED();
#endif
    Q_ASSERT(!"NlcPtoPWidgetSeekProgress::setSliderWidget() - Changing the slider widget is not allowed.");
}

void NlcPtoPWidgetSeekProgress::updateEvent(const QPoint &pos)
{
    if (!_gotvptopMediaPlayer)
        return;

    if (pos.x() < _progress->pos().x() || pos.x() > _progress->pos().x() + _progress->width())
        return;

    float click = pos.x() - _progress->pos().x();
    float op = _progress->maximum() / _progress->width();
    float newValue = click * op;

    _gotvptopMediaPlayer->setTime(newValue);
    _progress->setValue(newValue);
}

void NlcPtoPWidgetSeekProgress::updateCurrentTime(int time)
{
    if (_lock)
        return;

    NlcPtoPWidgetSeek::updateCurrentTime(time);
}

void NlcPtoPWidgetSeekProgress::updateFullTime(int time)
{
    if (_lock)
        return;

    NlcPtoPWidgetSeek::updateFullTime(time);
}

void NlcPtoPWidgetSeekProgress::lock()
{
    _lock = true;
}

void NlcPtoPWidgetSeekProgress::unlock()
{
    _lock = false;
}
