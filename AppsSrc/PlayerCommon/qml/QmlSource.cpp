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

#include "qml/QmlSource.h"
#include "qml/rendering/QmlVideoStream.h"

NlcPtoPQmlSource::NlcPtoPQmlSource(QObject *parent)
    : QObject(parent),
      _videoStream(new NlcPtoPQmlVideoStream(parent)) {}

NlcPtoPQmlSource::~NlcPtoPQmlSource()
{
    delete _videoStream;
}

void NlcPtoPQmlSource::setPlayer(NlcPtoPMediaPlayer *player)
{
    _videoStream->init(player);
}

void NlcPtoPQmlSource::removePlayer()
{
    _videoStream->deinit();
}

void NlcPtoPQmlSource::registerVideoOutput(NlcPtoPQmlVideoOutput *output)
{
    _videoStream->registerVideoOutput(output);
}

void NlcPtoPQmlSource::deregisterVideoOutput(NlcPtoPQmlVideoOutput *output)
{
    _videoStream->deregisterVideoOutput(output);
}
