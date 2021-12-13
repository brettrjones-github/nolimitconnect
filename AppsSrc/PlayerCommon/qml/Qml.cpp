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

void GoTvPtoPQml::registerTypes()
{
    QByteArray module(LIBGOTVQT_QML_MODULE);
    const char *m = module.data();

    qmlRegisterUncreatableType<GoTvPtoP>(m, 1, 1, "GoTvPtoP", QStringLiteral("GoTvPtoP cannot be instantiated directly"));
    qmlRegisterUncreatableType<GoTvPtoPQmlSource>(m, 1, 1, "GoTvPtoPSource", QStringLiteral("GoTvPtoPQmlSource cannot be instantiated directly"));
    qmlRegisterUncreatableType<GoTvPtoPTrackModel>(m, 1, 1, "GoTvPtoPTrackModel", QStringLiteral("GoTvPtoPTrackModel cannot be instantiated directly"));

    qmlRegisterType<GoTvPtoPQmlPlayer>(m, 1, 1, "GoTvPtoPPlayer");
    qmlRegisterType<GoTvPtoPQmlVideoOutput>(m, 1, 1, "GoTvPtoPVideoOutput");

    // Deprecated
    qmlRegisterType<GoTvPtoPQmlVideoPlayer>(m, 1, 0, "GoTvPtoPVideoPlayer");
}
