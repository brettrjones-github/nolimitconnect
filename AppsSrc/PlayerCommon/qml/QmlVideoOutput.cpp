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

NlcPtoPQmlVideoOutput::NlcPtoPQmlVideoOutput()
    : _fillMode(NlcPtoP::PreserveAspectFit),
      _source(0),
      _frameUpdated(false)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

NlcPtoPQmlVideoOutput::~NlcPtoPQmlVideoOutput()
{
    setSource(0);
}

NlcPtoPQmlSource *NlcPtoPQmlVideoOutput::source() const
{
    return _source;
}

void NlcPtoPQmlVideoOutput::setSource(NlcPtoPQmlSource *source)
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

int NlcPtoPQmlVideoOutput::fillMode() const
{
    return _fillMode;
}

void NlcPtoPQmlVideoOutput::setFillMode(int mode)
{
    if (_fillMode == mode)
        return;

    _fillMode = NlcPtoP::FillMode(mode);

    update();

    emit fillModeChanged();
}

int NlcPtoPQmlVideoOutput::aspectRatio() const
{
    return _aspectRatio;
}

void NlcPtoPQmlVideoOutput::setAspectRatio(int aspectRatio)
{
    if (_aspectRatio == aspectRatio)
        return;

    _aspectRatio = NlcPtoP::Ratio(aspectRatio);

    update();

    emit aspectRatioChanged();
}

int NlcPtoPQmlVideoOutput::cropRatio() const
{
    return _cropRatio;
}

void NlcPtoPQmlVideoOutput::setCropRatio(int cropRatio)
{
    if (_cropRatio == cropRatio)
        return;

    _cropRatio = NlcPtoP::Ratio(cropRatio);

    update();

    emit cropRatioChanged();
}

QSGNode *NlcPtoPQmlVideoOutput::updatePaintNode(QSGNode *oldNode,
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

    if (fillMode() != NlcPtoP::Stretch) {
        const uint16_t fw = _frame->width;
        const uint16_t fh = _frame->height;

        qreal frameAspectTmp = qreal(fw) / fh;
        QSizeF aspectRatioSize = NlcPtoP::ratioSize(_aspectRatio);
        if (aspectRatioSize.width() != 0 && aspectRatioSize.height() != 0) {
            frameAspectTmp = aspectRatioSize.width() / aspectRatioSize.height();
        }
        QSizeF cropRatioSize = NlcPtoP::ratioSize(_cropRatio);
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

        if (fillMode() == NlcPtoP::PreserveAspectFit) {
            qreal outWidth = width();
            qreal outHeight = height();
            if (frameAspect > itemAspect)
                outHeight = outWidth / frameAspect;
            else if (frameAspect < itemAspect)
                outWidth = outHeight * frameAspect;

            outRect = QRectF((width() - outWidth) / 2, (height() - outHeight) / 2,
                             outWidth, outHeight);
        } else if (fillMode() == NlcPtoP::PreserveAspectCrop) {
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

void NlcPtoPQmlVideoOutput::presentFrame(const std::shared_ptr<const NlcPtoPYUVVideoFrame> &frame)
{
    _frame = frame;
    _frameUpdated = true;
    update();
}
