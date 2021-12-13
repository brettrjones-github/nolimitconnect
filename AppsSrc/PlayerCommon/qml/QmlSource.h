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

class GoTvPtoPMediaPlayer;

class GoTvPtoPQmlVideoOutput;
class GoTvPtoPQmlVideoStream;

/*!
    \class GoTvPtoPQmlSource QmlSource.h GOTVQtQml/QmlSource.h
    \ingroup GOTVQtQml
    \brief QML video source

    A special class which represents video source.
    GoTvPtoPQmlVideoOutput can connect to any class that subclasses this one.

    \see GoTvPtoPQmlPlayer
    \see GoTvPtoPQmlVideoOutput

    \since GOTV-Qt 1.1
 */
class GOTVQT_QML_EXPORT GoTvPtoPQmlSource : public QObject,
                                      public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    /*!
        \brief GoTvPtoPQmlSource constructor
        \param parent parent object
     */
    GoTvPtoPQmlSource(QObject *parent);
    ~GoTvPtoPQmlSource();

    /*!
        \brief Set media player to use
        \param player media player
     */
    void setPlayer(GoTvPtoPMediaPlayer *player);

    /*!
        \brief Remove player from source
    */
    void removePlayer();

    /*!
        \brief Register video output
        \param output QML video output
     */
    virtual void registerVideoOutput(GoTvPtoPQmlVideoOutput *output);

    /*!
        \brief Deregister video output
        \param output QML video output
     */
    virtual void deregisterVideoOutput(GoTvPtoPQmlVideoOutput *output);

private:
    // LCOV_EXCL_START
    void classBegin() override {}
    void componentComplete() override {}
    // LCOV_EXCL_END

    GoTvPtoPQmlVideoStream *_videoStream;
};

#endif // GOTVQT_QMLSOURCE_H_
