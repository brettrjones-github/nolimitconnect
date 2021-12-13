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

GoTvPtoPWidgetVideo::GoTvPtoPWidgetVideo(GoTvPtoPMediaPlayer *player,
                               QWidget *parent)
    : QFrame(parent)
{
    _gotvptopMediaPlayer = player;

    connect(_gotvptopMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));

    initWidgetVideo();
}

GoTvPtoPWidgetVideo::GoTvPtoPWidgetVideo(QWidget *parent)
    : QFrame(parent),
      _gotvptopMediaPlayer(0)
{
    initWidgetVideo();
}

GoTvPtoPWidgetVideo::~GoTvPtoPWidgetVideo()
{
    release();
}

void GoTvPtoPWidgetVideo::initWidgetVideo()
{
    _enableSettings = false;
    _defaultAspectRatio = GoTvPtoP::Original;
    _defaultCropRatio = GoTvPtoP::Original;
    _defaultDeinterlacing = GoTvPtoP::Disabled;
    _defaultScale = GoTvPtoP::NoScale;
    _currentAspectRatio = GoTvPtoP::Original;
    _currentCropRatio = GoTvPtoP::Original;
    _currentDeinterlacing = GoTvPtoP::Disabled;
    _currentScale = GoTvPtoP::NoScale;

    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = 0;

    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    setPalette(plt);
}

void GoTvPtoPWidgetVideo::setMediaPlayer(GoTvPtoPMediaPlayer *player)
{
    _gotvptopMediaPlayer = player;

    connect(_gotvptopMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
}

void GoTvPtoPWidgetVideo::setCurrentAspectRatio(const GoTvPtoP::Ratio &ratio)
{
    _currentAspectRatio = ratio;
}

void GoTvPtoPWidgetVideo::setCurrentCropRatio(const GoTvPtoP::Ratio &ratio)
{
    _currentCropRatio = ratio;
}

void GoTvPtoPWidgetVideo::setCurrentDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing)
{
    _currentDeinterlacing = deinterlacing;
}

void GoTvPtoPWidgetVideo::setCurrentScale(const GoTvPtoP::Scale &scale)
{
    _currentScale = scale;
}

void GoTvPtoPWidgetVideo::setDefaultAspectRatio(const GoTvPtoP::Ratio &ratio)
{
    _defaultAspectRatio = ratio;
}

void GoTvPtoPWidgetVideo::setDefaultCropRatio(const GoTvPtoP::Ratio &ratio)
{
    _defaultCropRatio = ratio;
}

void GoTvPtoPWidgetVideo::setDefaultDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing)
{
    _defaultDeinterlacing = deinterlacing;
}

void GoTvPtoPWidgetVideo::setDefaultScale(const GoTvPtoP::Scale &scale)
{
    _defaultScale = scale;
}

void GoTvPtoPWidgetVideo::enableDefaultSettings()
{
    initDefaultSettings();

    enablePreviousSettings();
}

void GoTvPtoPWidgetVideo::enablePreviousSettings()
{
    _enableSettings = true;
}

void GoTvPtoPWidgetVideo::initDefaultSettings()
{
    _currentAspectRatio = defaultAspectRatio();
    _currentCropRatio = defaultCropRatio();
    _currentDeinterlacing = defaultDeinterlacing();
    _currentScale = defaultScale();
}

void GoTvPtoPWidgetVideo::applyPreviousSettings()
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

void GoTvPtoPWidgetVideo::setAspectRatio(const GoTvPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer) {
        _currentAspectRatio = ratio;
        _gotvptopMediaPlayer->video()->setAspectRatio(ratio);
    }
}

void GoTvPtoPWidgetVideo::setCropRatio(const GoTvPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer) {
        _currentCropRatio = ratio;
        _gotvptopMediaPlayer->video()->setCropGeometry(ratio);
    }
}

void GoTvPtoPWidgetVideo::setDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing)
{
    if (_gotvptopMediaPlayer) {
        _currentDeinterlacing = deinterlacing;
        _gotvptopMediaPlayer->video()->setDeinterlace(deinterlacing);
    }
}

void GoTvPtoPWidgetVideo::setScale(const GoTvPtoP::Scale &scale)
{
    if (_gotvptopMediaPlayer) {
        _currentScale = scale;
        _gotvptopMediaPlayer->video()->setScale(scale);
    }
}

void GoTvPtoPWidgetVideo::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId GoTvPtoPWidgetVideo::request()
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

void GoTvPtoPWidgetVideo::release()
{
    if (_video) {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }

    updateGeometry();
}
