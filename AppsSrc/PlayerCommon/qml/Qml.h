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

#ifndef GOTVQT_QML_H_
#define GOTVQT_QML_H_

#include "SharedExportQml.h"

/*!
    \defgroup GOTVQtQml GOTV-Qt Qml (GOTVQtQml)
    \brief QML classes for controlling video and media playback
 */

namespace GoTvPtoPQml
{
    /*!
        \brief Register QML types as GOTVQt

        Intended for use without plugin.

        Include into QML file as
        import GOTVQt VERSION_MAJOR.VERSION_MINOR

        Object names: GoTvPtoPPlayer, GoTvPtoPVideoObject

        \since GOTV-Qt 1.1
     */
    GOTVQT_QML_EXPORT void registerTypes();
}

#endif // GOTVQT_QML_H_
