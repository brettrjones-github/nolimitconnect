/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* Based on Phonon multimedia library
* Copyright (C) 2011 Harald Sitter <sitter@kde.org>
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

#include "core/MediaPlayer.h"

#include "qml/QmlVideoObject.h"
#include "qml/painter/GlslPainter.h"

GoTvPtoPQmlVideoObject::GoTvPtoPQmlVideoObject(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      _player(0),
      _geometry(0, 0, 640, 480),
      _boundingRect(0, 0, 0, 0),
      _frameSize(0, 0),
      _graphicsPainter(0),
      _paintedOnce(false),
      _gotSize(false),
      _aspectRatio(GoTvPtoP::Original),
      _cropRatio(GoTvPtoP::Original)
{
    setRenderTarget(InvertedYFramebufferObject);
    setFlag(ItemHasContents, true);
}

GoTvPtoPQmlVideoObject::~GoTvPtoPQmlVideoObject()
{
    if (_graphicsPainter)
        delete _graphicsPainter;
}

QRectF GoTvPtoPQmlVideoObject::boundingRect() const
{
    return _boundingRect;
}

void GoTvPtoPQmlVideoObject::updateBoundingRect()
{
    _boundingRect = QRectF(0, 0, _frameSize.width(), _frameSize.height());

    updateAspectRatio();

    QSizeF scaledFrameSize = _boundingRect.size();
    if (_aspectRatio == GoTvPtoP::Ignore) {
        scaledFrameSize.scale(_geometry.size(), Qt::IgnoreAspectRatio);
    } else {
        scaledFrameSize.scale(_geometry.size(), Qt::KeepAspectRatio);
    }
    _boundingRect.setSize( scaledFrameSize );

    updateCropRatio();

    _boundingRect.moveCenter(_geometry.center());
}

void GoTvPtoPQmlVideoObject::updateAspectRatio()
{
    QSizeF ar = GoTvPtoP::ratioSize( _aspectRatio );

    if( ar.width() != 0 && ar.height() != 0)
    {
        qreal ratio = qMin( _boundingRect.width() / ar.width() , _boundingRect.height() / ar.height() );
        _boundingRect.setWidth( (qreal) ratio * ar.width() );
        _boundingRect.setHeight( (qreal) ratio * ar.height() );
    }
}

void GoTvPtoPQmlVideoObject::updateCropRatio()
{
    QSizeF ar = GoTvPtoP::ratioSize( _cropRatio );

    if( ar.width() != 0 && ar.height() != 0)
    {
        QRectF cropRect = _boundingRect;
        qreal ratio = qMin( cropRect.width() / ar.width() , cropRect.height() / ar.height() );
        cropRect.setWidth( (qreal) ratio * ar.width() );
        cropRect.setHeight( (qreal) ratio * ar.height() );

        QSizeF scaledFrameSize = cropRect.size();
        scaledFrameSize.scale(_geometry.size(), Qt::KeepAspectRatio);


        _boundingRect.setWidth( _boundingRect.width() * ( scaledFrameSize.width() / cropRect.width() ) );
        _boundingRect.setHeight( _boundingRect.height() * ( scaledFrameSize.height() / cropRect.height() ) );
    }
}

GoTvPtoP::Ratio GoTvPtoPQmlVideoObject::cropRatio() const
{
    return _cropRatio;
}

void GoTvPtoPQmlVideoObject::setCropRatio(const GoTvPtoP::Ratio &cropRatio)
{
    _cropRatio = cropRatio;
    updateBoundingRect();
}

GoTvPtoP::Ratio GoTvPtoPQmlVideoObject::aspectRatio() const
{
    return _aspectRatio;
}

void GoTvPtoPQmlVideoObject::setAspectRatio(const GoTvPtoP::Ratio &aspectRatio)
{
    _aspectRatio = aspectRatio;
    updateBoundingRect();
}

void GoTvPtoPQmlVideoObject::paint(QPainter *painter)
{
    lock();
    if( _frame.inited )
    {
        if (!_graphicsPainter)
            _graphicsPainter = new GlslPainter;

        Q_ASSERT(_graphicsPainter);

        if (!_gotSize || _frameSize.isNull()) {
            // TODO: do scaling ourselfs?
            _gotSize = true;
            _frameSize = QSize(_frame.width, _frame.height);
            updateBoundingRect();
        }

        if (!_paintedOnce) {
            painter->fillRect(_boundingRect, Qt::black);
            _paintedOnce = true;
        } else {
            Q_ASSERT(_graphicsPainter);
            _graphicsPainter->setFrame(&_frame);
            if (!_graphicsPainter->inited())
                _graphicsPainter->init();
            _graphicsPainter->paint(painter, _boundingRect, this);
        }
    }

    unlock();
}

void GoTvPtoPQmlVideoObject::geometryChanged(const QRectF &newGeometry,
                                          const QRectF &oldGeometry)
{
    _geometry = newGeometry;
    updateBoundingRect();

    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void GoTvPtoPQmlVideoObject::frameReady()
{
    update();
}

void GoTvPtoPQmlVideoObject::reset()
{
    // Do not reset the spyFormats as they will not change.
    _paintedOnce = false;
    _gotSize = false;

    // The painter is reset because the backend may choose another format for
    // another file (better conversion for some codec etc.)
    if (_graphicsPainter) {
        delete _graphicsPainter;
        _graphicsPainter = 0;
    }
}

void GoTvPtoPQmlVideoObject::connectToMediaPlayer(GoTvPtoPMediaPlayer *player)
{
    setCallbacks(player);
}

void GoTvPtoPQmlVideoObject::disconnectFromMediaPlayer(GoTvPtoPMediaPlayer *player)
{
    // Try to prevent callbacks called after this object is being deleted
    if (player) {
        player->stop();
    }

    unsetCallbacks(player);
}

void GoTvPtoPQmlVideoObject::lock()
{
    _mutex.lock();
}

bool GoTvPtoPQmlVideoObject::tryLock()
{
    return _mutex.tryLock();
}

void GoTvPtoPQmlVideoObject::unlock()
{
    _mutex.unlock();
}

void *GoTvPtoPQmlVideoObject::lockCallback(void **planes)
{
    lock();

    for (unsigned int i = 0; i < _frame.planeCount; ++i) {
        planes[i] = reinterpret_cast<void *>(_frame.plane[i].data());
    }

    return 0; // There is only one buffer, so no need to identify it.
}

void GoTvPtoPQmlVideoObject::unlockCallback(void *picture, void *const*planes)
{
    Q_UNUSED(picture);
    Q_UNUSED(planes);
    unlock();
    // To avoid thread polution do not call frameReady directly, but via the
    // event loop.
    QMetaObject::invokeMethod(this, "frameReady", Qt::QueuedConnection);
}

void GoTvPtoPQmlVideoObject::displayCallback(void *picture)
{
    if( !_frame.inited )
    {
        float sar = _player->sampleAspectRatio();
        if( sar > 0.0 )
        {
            _frame.height *= sar;
            _frame.inited = true;
        }
    }
    Q_UNUSED(picture); // There is only one buffer.
}

unsigned int GoTvPtoPQmlVideoObject::formatCallback(char *chroma,
                                                 unsigned *width, unsigned *height,
                                                 unsigned *pitches, unsigned *lines)
{
    qDebug() << "Format:"
             << "chroma:" << chroma
             << "width:" << *width
             << "height:" << *height
             << "pitches:" << *pitches
             << "lines:" << *lines;

    if (!_graphicsPainter)
        _graphicsPainter = new GlslPainter;

    qstrcpy(chroma, "YV12");
    const gotvptop_chroma_description_t *chromaDesc = gotvptop_fourcc_GetChromaDescription(GOTV_CODEC_YV12);

    Q_ASSERT(chromaDesc);

    _frame.width = *width;
    _frame.height = *height;
    _frame.planeCount = chromaDesc->plane_count;

    qDebug() << chroma << chromaDesc->plane_count << *lines;

    const unsigned int bufferSize = setPitchAndLines(chromaDesc,
                                                     *width, *height,
                                                     pitches, lines,
                                                     (unsigned *) &_frame.visiblePitch, (unsigned *)&_frame.visibleLines);
    for (unsigned int i = 0; i < _frame.planeCount; ++i) {
        _frame.pitch[i] = pitches[i];
        _frame.lines[i] = lines[i];
        _frame.plane[i].resize(pitches[i] * lines[i]);
    }

    return bufferSize;
}

void GoTvPtoPQmlVideoObject::formatCleanUpCallback()
{
    _frame.inited = false;
    // To avoid thread polution do not call reset directly but via the event loop.
    QMetaObject::invokeMethod(this, "reset", Qt::QueuedConnection);
}
