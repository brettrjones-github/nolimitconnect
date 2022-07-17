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

#ifndef GOTVQT_QMLSOURCE_H_
#define GOTVQT_QMLSOURCE_H_

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>

#include "SharedExportQml.h"

class NlcPtoPMediaPlayer;

class NlcPtoPQmlVideoOutput;
class NlcPtoPQmlVideoStream;

/*!
    \class NlcPtoPQmlSource QmlSource.h GOTVQtQml/QmlSource.h
    \ingroup GOTVQtQml
    \brief QML video source

    A special class which represents video source.
    NlcPtoPQmlVideoOutput can connect to any class that subclasses this one.

    \see NlcPtoPQmlPlayer
    \see NlcPtoPQmlVideoOutput

    \since GOTV-Qt 1.1
 */
class GOTVQT_QML_EXPORT NlcPtoPQmlSource : public QObject,
                                      public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    /*!
        \brief NlcPtoPQmlSource constructor
        \param parent parent object
     */
    NlcPtoPQmlSource(QObject *parent);
    ~NlcPtoPQmlSource();

    /*!
        \brief Set media player to use
        \param player media player
     */
    void setPlayer(NlcPtoPMediaPlayer *player);

    /*!
        \brief Remove player from source
    */
    void removePlayer();

    /*!
        \brief Register video output
        \param output QML video output
     */
    virtual void registerVideoOutput(NlcPtoPQmlVideoOutput *output);

    /*!
        \brief Deregister video output
        \param output QML video output
     */
    virtual void deregisterVideoOutput(NlcPtoPQmlVideoOutput *output);

private:
    // LCOV_EXCL_START
    void classBegin() override {}
    void componentComplete() override {}
    // LCOV_EXCL_END

    NlcPtoPQmlVideoStream *_videoStream;
};

#endif // GOTVQT_QMLSOURCE_H_
