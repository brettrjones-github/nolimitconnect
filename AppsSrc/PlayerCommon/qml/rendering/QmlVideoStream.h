/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
* Copyright (C) 2014-2015, Sergey Radionov <rsatom_gmail.com>
*
* This file is based on QmlGoTvPtoP library
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

#ifndef GOTVQT_QMLRENDERING_QMLVIDEOSTREAM_H_
#define GOTVQT_QMLRENDERING_QMLVIDEOSTREAM_H_

#include "core/VideoStream.h"

class GoTvPtoPQmlVideoOutput;

class GoTvPtoPQmlVideoStream : public GoTvPtoPVideoStream
{
    Q_OBJECT
public:
    explicit GoTvPtoPQmlVideoStream(QObject *parent = 0);
    ~GoTvPtoPQmlVideoStream();

    void registerVideoOutput(GoTvPtoPQmlVideoOutput *output);
    void deregisterVideoOutput(GoTvPtoPQmlVideoOutput *output);

    QList<GoTvPtoPQmlVideoOutput *> attachedOutputs() const { return _attachedOutputs; }

private:
    Q_INVOKABLE virtual void frameUpdated();

    QList<GoTvPtoPQmlVideoOutput *> _attachedOutputs;
};

#endif // GOTVQT_QMLRENDERING_QMLVIDEOSTREAM_H_
