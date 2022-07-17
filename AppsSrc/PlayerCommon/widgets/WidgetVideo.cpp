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

#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolBar>
#endif

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#include <qx11info_x11.h>
#endif

#include "core/Error.h"
#include "core/MediaPlayer.h"
#include "core/Video.h"

#include "widgets/WidgetVideo.h"

NlcPtoPWidgetVideo::NlcPtoPWidgetVideo(NlcPtoPMediaPlayer *player,
                               QWidget *parent)
    : QFrame(parent)
{
    _gotvptopMediaPlayer = player;

    connect(_gotvptopMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));

    initWidgetVideo();
}

NlcPtoPWidgetVideo::NlcPtoPWidgetVideo(QWidget *parent)
    : QFrame(parent),
      _gotvptopMediaPlayer(0)
{
    initWidgetVideo();
}

NlcPtoPWidgetVideo::~NlcPtoPWidgetVideo()
{
    release();
}

void NlcPtoPWidgetVideo::initWidgetVideo()
{
    _enableSettings = false;
    _defaultAspectRatio = NlcPtoP::Original;
    _defaultCropRatio = NlcPtoP::Original;
    _defaultDeinterlacing = NlcPtoP::Disabled;
    _defaultScale = NlcPtoP::NoScale;
    _currentAspectRatio = NlcPtoP::Original;
    _currentCropRatio = NlcPtoP::Original;
    _currentDeinterlacing = NlcPtoP::Disabled;
    _currentScale = NlcPtoP::NoScale;

    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = 0;

    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    setPalette(plt);
}

void NlcPtoPWidgetVideo::setMediaPlayer(NlcPtoPMediaPlayer *player)
{
    _gotvptopMediaPlayer = player;

    connect(_gotvptopMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
}

void NlcPtoPWidgetVideo::setCurrentAspectRatio(const NlcPtoP::Ratio &ratio)
{
    _currentAspectRatio = ratio;
}

void NlcPtoPWidgetVideo::setCurrentCropRatio(const NlcPtoP::Ratio &ratio)
{
    _currentCropRatio = ratio;
}

void NlcPtoPWidgetVideo::setCurrentDeinterlacing(const NlcPtoP::Deinterlacing &deinterlacing)
{
    _currentDeinterlacing = deinterlacing;
}

void NlcPtoPWidgetVideo::setCurrentScale(const NlcPtoP::Scale &scale)
{
    _currentScale = scale;
}

void NlcPtoPWidgetVideo::setDefaultAspectRatio(const NlcPtoP::Ratio &ratio)
{
    _defaultAspectRatio = ratio;
}

void NlcPtoPWidgetVideo::setDefaultCropRatio(const NlcPtoP::Ratio &ratio)
{
    _defaultCropRatio = ratio;
}

void NlcPtoPWidgetVideo::setDefaultDeinterlacing(const NlcPtoP::Deinterlacing &deinterlacing)
{
    _defaultDeinterlacing = deinterlacing;
}

void NlcPtoPWidgetVideo::setDefaultScale(const NlcPtoP::Scale &scale)
{
    _defaultScale = scale;
}

void NlcPtoPWidgetVideo::enableDefaultSettings()
{
    initDefaultSettings();

    enablePreviousSettings();
}

void NlcPtoPWidgetVideo::enablePreviousSettings()
{
    _enableSettings = true;
}

void NlcPtoPWidgetVideo::initDefaultSettings()
{
    _currentAspectRatio = defaultAspectRatio();
    _currentCropRatio = defaultCropRatio();
    _currentDeinterlacing = defaultDeinterlacing();
    _currentScale = defaultScale();
}

void NlcPtoPWidgetVideo::applyPreviousSettings()
{
    if (!_enableSettings)
        return;

    if (!_gotvptopMediaPlayer)
        return;

    _gotvptopMediaPlayer->video()->setAspectRatio(_currentAspectRatio);
    _gotvptopMediaPlayer->video()->setCropGeometry(_currentCropRatio);
    _gotvptopMediaPlayer->video()->setScale(_currentScale);
    _gotvptopMediaPlayer->video()->setDeinterlace(_currentDeinterlacing);

    _enableSettings = false;
}

void NlcPtoPWidgetVideo::setAspectRatio(const NlcPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer) {
        _currentAspectRatio = ratio;
        _gotvptopMediaPlayer->video()->setAspectRatio(ratio);
    }
}

void NlcPtoPWidgetVideo::setCropRatio(const NlcPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer) {
        _currentCropRatio = ratio;
        _gotvptopMediaPlayer->video()->setCropGeometry(ratio);
    }
}

void NlcPtoPWidgetVideo::setDeinterlacing(const NlcPtoP::Deinterlacing &deinterlacing)
{
    if (_gotvptopMediaPlayer) {
        _currentDeinterlacing = deinterlacing;
        _gotvptopMediaPlayer->video()->setDeinterlace(deinterlacing);
    }
}

void NlcPtoPWidgetVideo::setScale(const NlcPtoP::Scale &scale)
{
    if (_gotvptopMediaPlayer) {
        _currentScale = scale;
        _gotvptopMediaPlayer->video()->setScale(scale);
    }
}

void NlcPtoPWidgetVideo::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId NlcPtoPWidgetVideo::request()
{
    if (_video)
        return 0;

    _video = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    _video->setPalette(plt);
    _video->setAutoFillBackground(true);
    _video->setMouseTracking(true);
/* Indicates that the widget wants to draw directly onto the screen.
       Widgets with this attribute set do not participate in composition
       management */
/* This is currently disabled on X11 as it does not seem to improve
     * performance, but causes the video widget to be transparent... */
#if QT_VERSION < 0x050000 && !defined(Q_WS_X11)
    _video->setAttribute(Qt::WA_PaintOnScreen, true);
#endif

    _layout->addWidget(_video);

    sync();
    return _video->winId();
}

void NlcPtoPWidgetVideo::release()
{
    if (_video) {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }

    updateGeometry();
}
