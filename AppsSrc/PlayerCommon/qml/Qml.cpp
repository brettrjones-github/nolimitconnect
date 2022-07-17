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

#include <QtQuick/QQuickItem>

#include "gotvptop_config.h"

#include "core/Enums.h"
#include "core/TrackModel.h"
#include "qml/Qml.h"
#include "qml/QmlPlayer.h"
#include "qml/QmlSource.h"
#include "qml/QmlVideoOutput.h"
#include "qml/QmlVideoPlayer.h"

void NlcPtoPQml::registerTypes()
{
    QByteArray module(LIBGOTVQT_QML_MODULE);
    const char *m = module.data();

    qmlRegisterUncreatableType<NlcPtoP>(m, 1, 1, "NlcPtoP", QStringLiteral("NlcPtoP cannot be instantiated directly"));
    qmlRegisterUncreatableType<NlcPtoPQmlSource>(m, 1, 1, "NlcPtoPSource", QStringLiteral("NlcPtoPQmlSource cannot be instantiated directly"));
    qmlRegisterUncreatableType<NlcPtoPTrackModel>(m, 1, 1, "NlcPtoPTrackModel", QStringLiteral("NlcPtoPTrackModel cannot be instantiated directly"));

    qmlRegisterType<NlcPtoPQmlPlayer>(m, 1, 1, "NlcPtoPPlayer");
    qmlRegisterType<NlcPtoPQmlVideoOutput>(m, 1, 1, "NlcPtoPVideoOutput");

    // Deprecated
    qmlRegisterType<NlcPtoPQmlVideoPlayer>(m, 1, 0, "NlcPtoPVideoPlayer");
}
