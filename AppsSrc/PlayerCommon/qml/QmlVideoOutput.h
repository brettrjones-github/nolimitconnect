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

#ifndef GOTVQT_QMLVIDEOOUTPUT_H_
#define GOTVQT_QMLVIDEOOUTPUT_H_

#include "config_gotvapps.h"

#include <memory>

#include <QQuickItem>
#include <QPointer>

#include <core/Enums.h>

#include "SharedExportQml.h"

struct NlcPtoPYUVVideoFrame;
class NlcPtoPQmlSource;

/*!
    \class NlcPtoPQmlVideoOutput QmlVideoOutput.h GOTVQtQml/QmlVideoOutput.h
    \ingroup GOTVQtQml
    \brief QML video output

    A QML video output that can be used to display video inside QML.

    \since GOTV-Qt 1.1
 */
class GOTVQT_QML_EXPORT NlcPtoPQmlVideoOutput : public QQuickItem
{
    Q_OBJECT

    /*!
        \brief Current video source
        \see source
        \see setSource
        \see sourceChanged
     */
    Q_PROPERTY(NlcPtoPQmlSource *source READ source WRITE setSource NOTIFY sourceChanged)

    /*!
        \brief Current fill mode
        \see fillMode
        \see setFillMode
        \see fillModeChanged
     */
    Q_PROPERTY(int fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)

    /*!
        \brief Current aspect ratio
        \see aspectRatio
        \see setAspectRatio
        \see aspectRatioChanged
     */
    Q_PROPERTY(int aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)

    /*!
        \brief Current crop ratio
        \see cropRatio
        \see setCropRatio
        \see cropRatioChanged
     */
    Q_PROPERTY(int cropRatio READ cropRatio WRITE setCropRatio NOTIFY cropRatioChanged)

public:
    NlcPtoPQmlVideoOutput();
    ~NlcPtoPQmlVideoOutput();

    /*!
        \brief Current video source
        \return source

        Used as property in QML.
     */
    NlcPtoPQmlSource *source() const;

    /*!
        \brief Set video source
        \param source video source

        Used as property in QML.
     */
    void setSource(NlcPtoPQmlSource *source);

    /*!
        \brief Current fill mode
        \return fill mode

        Used as property in QML.
     */
    int fillMode() const;

    /*!
        \brief Set fill mode
        \param mode fill mode

        Used as property in QML.
     */
    void setFillMode(int mode);

    /*!
        \brief Current aspect ratio
        \return aspect ratio

        Used as property in QML.
     */
    int aspectRatio() const;

    /*!
        \brief Set aspect ratio
        \param aspectRatio new aspect ratio

        No effect when fillMode equals NlcPtoP::Stretch.
        Used as property in QML.
     */
    void setAspectRatio(int aspectRatio);

    /*!
        \brief Current crop ratio
        \return crop ratio

        Used as property in QML.
     */
    int cropRatio() const;

    /*!
        \brief Set crop ratio
        \param cropRatio new crop ratio

        No effect when fillMode equals NlcPtoP::Stretch.
        Used as property in QML.
     */
    void setCropRatio(int cropRatio);

public slots:
    /*!
        \brief Set frame which will be rendered in the output.
        \param frame
     */
    void presentFrame(const std::shared_ptr<const NlcPtoPYUVVideoFrame> &frame);

signals:
    /*!
        \brief Source changed signal
     */
    void sourceChanged();

    /*!
        \brief Fill mode changed signal
     */
    void fillModeChanged();

    /*!
        \brief Fill mode changed signal
     */
    void aspectRatioChanged();

    /*!
        \brief Fill mode changed signal
     */
    void cropRatioChanged();

private:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode,
                                     UpdatePaintNodeData *data);

    NlcPtoP::FillMode _fillMode;
    NlcPtoP::Ratio _aspectRatio;
    NlcPtoP::Ratio _cropRatio;

    QPointer<NlcPtoPQmlSource> _source;

    bool _frameUpdated;
    std::shared_ptr<const NlcPtoPYUVVideoFrame> _frame;
};

#endif // GOTVQT_QMLVIDEOOUTPUT_H_
