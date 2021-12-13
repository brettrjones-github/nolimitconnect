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

#include "core/YUVVideoFrame.h"
#include "qml/QmlSource.h"
#include "qml/QmlVideoOutput.h"
#include "qml/rendering/VideoNode.h"

GoTvPtoPQmlVideoOutput::GoTvPtoPQmlVideoOutput()
    : _fillMode(GoTvPtoP::PreserveAspectFit),
      _source(0),
      _frameUpdated(false)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

GoTvPtoPQmlVideoOutput::~GoTvPtoPQmlVideoOutput()
{
    setSource(0);
}

GoTvPtoPQmlSource *GoTvPtoPQmlVideoOutput::source() const
{
    return _source;
}

void GoTvPtoPQmlVideoOutput::setSource(GoTvPtoPQmlSource *source)
{
    if (source == _source)
        return;

    if (_source)
        _source->deregisterVideoOutput(this);

    _source = source;

    if (_source)
        _source->registerVideoOutput(this);

    emit sourceChanged();
}

int GoTvPtoPQmlVideoOutput::fillMode() const
{
    return _fillMode;
}

void GoTvPtoPQmlVideoOutput::setFillMode(int mode)
{
    if (_fillMode == mode)
        return;

    _fillMode = GoTvPtoP::FillMode(mode);

    update();

    emit fillModeChanged();
}

int GoTvPtoPQmlVideoOutput::aspectRatio() const
{
    return _aspectRatio;
}

void GoTvPtoPQmlVideoOutput::setAspectRatio(int aspectRatio)
{
    if (_aspectRatio == aspectRatio)
        return;

    _aspectRatio = GoTvPtoP::Ratio(aspectRatio);

    update();

    emit aspectRatioChanged();
}

int GoTvPtoPQmlVideoOutput::cropRatio() const
{
    return _cropRatio;
}

void GoTvPtoPQmlVideoOutput::setCropRatio(int cropRatio)
{
    if (_cropRatio == cropRatio)
        return;

    _cropRatio = GoTvPtoP::Ratio(cropRatio);

    update();

    emit cropRatioChanged();
}

QSGNode *GoTvPtoPQmlVideoOutput::updatePaintNode(QSGNode *oldNode,
                                            UpdatePaintNodeData *data)
{
    Q_UNUSED(data)

    VideoNode *node = static_cast<VideoNode *>(oldNode);
    if (!_frame) {
        delete node;
        return 0;
    }

    if (!node)
        node = new VideoNode;

    QRectF outRect(0, 0, width(), height());
    QRectF srcRect(0, 0, 1., 1.);

    if (fillMode() != GoTvPtoP::Stretch) {
        const uint16_t fw = _frame->width;
        const uint16_t fh = _frame->height;

        qreal frameAspectTmp = qreal(fw) / fh;
        QSizeF aspectRatioSize = GoTvPtoP::ratioSize(_aspectRatio);
        if (aspectRatioSize.width() != 0 && aspectRatioSize.height() != 0) {
            frameAspectTmp = aspectRatioSize.width() / aspectRatioSize.height();
        }
        QSizeF cropRatioSize = GoTvPtoP::ratioSize(_cropRatio);
        if (cropRatioSize.width() != 0 && cropRatioSize.height() != 0) {
            const qreal cropAspect = cropRatioSize.width() / cropRatioSize.height();

            if (frameAspectTmp > cropAspect) {
                srcRect.setX((1. - cropAspect / frameAspectTmp) / 2);
                srcRect.setWidth(1. - srcRect.x() - srcRect.x());
            } else if (frameAspectTmp < cropAspect) {
                srcRect.setY((1. - frameAspectTmp / cropAspect) / 2);
                srcRect.setHeight(1. - srcRect.y() - srcRect.y());
            }

            frameAspectTmp = cropAspect;
        }
        const qreal itemAspect = width() / height();
        const qreal frameAspect = frameAspectTmp;

        if (fillMode() == GoTvPtoP::PreserveAspectFit) {
            qreal outWidth = width();
            qreal outHeight = height();
            if (frameAspect > itemAspect)
                outHeight = outWidth / frameAspect;
            else if (frameAspect < itemAspect)
                outWidth = outHeight * frameAspect;

            outRect = QRectF((width() - outWidth) / 2, (height() - outHeight) / 2,
                             outWidth, outHeight);
        } else if (fillMode() == GoTvPtoP::PreserveAspectCrop) {
            if (frameAspect > itemAspect) {
                srcRect.setX((1. - itemAspect / frameAspect) / 2);
                srcRect.setWidth(1. - srcRect.x() - srcRect.x());
            } else if (frameAspect < itemAspect) {
                srcRect.setY((1. - frameAspect / itemAspect) / 2);
                srcRect.setHeight(1. - srcRect.y() - srcRect.y());
            }
        }
    }

    if (_frameUpdated) {
        node->setFrame(_frame);
        _frameUpdated = false;
    }
    node->setRect(outRect, srcRect);

    return node;
}

void GoTvPtoPQmlVideoOutput::presentFrame(const std::shared_ptr<const GoTvPtoPYUVVideoFrame> &frame)
{
    _frame = frame;
    _frameUpdated = true;
    update();
}
