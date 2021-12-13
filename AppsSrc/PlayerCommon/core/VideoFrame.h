/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2014 Tadej Novak <tadej@tano.si>
*
* Based on Phonon multimedia library
* Copyright (C) 2012 Harald Sitter <sitter@kde.org>
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

#ifndef GOTVQT_VIDEOFRAME_H_
#define GOTVQT_VIDEOFRAME_H_

#include <QtCore/QByteArray>

#include "SharedExportCore.h"

/*!
    \struct GoTvPtoPVideoFrame VideoFrame.h core/VideoFrame.h
    \ingroup GOTVQtCore
    \brief Video frame data container (deprecated)
    \see GoTvPtoPAbstractVideoFrame
    \deprecated Deprecated since GOTV-Qt 1.1, will be removed in 2.0
*/
struct Q_DECL_DEPRECATED GOTVQT_CORE_EXPORT GoTvPtoPVideoFrame // LCOV_EXCL_LINE
{
    /*!
        \brief GoTvPtoPVideoFrame constructor.

        This construction ensures default data is set.
    */
    GoTvPtoPVideoFrame();

    bool inited; /*!< initialisation status */

    unsigned int width; /*!< frame width */
    unsigned int height; /*!< frame height */

    unsigned int planeCount; /*!< plane count */

    QByteArray plane[4]; /*!< plane data */

    int pitch[4]; /*!< pitch information */
    int visiblePitch[4]; /*!< visible pitch information */
    int lines[4]; /*!< lines count */
    int visibleLines[4]; /*!< visible lines count */
};

#endif // GOTVQT_VIDEOFRAME_H_
