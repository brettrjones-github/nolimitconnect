/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#ifndef GOTVQT_COMMON_H_
#define GOTVQT_COMMON_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "SharedExportCore.h"

/*!
    \defgroup GOTVQtCore GOTV-Qt Core (GOTVQtCore)
    \brief Core classes and functionality for main media playback
 */

/*!
    \namespace GoTvPtoPCommon Common.h core/Common.h
    \ingroup GOTVQtCore
	\brief Common functions
*/
namespace GoTvPtoPCommon
{
    /*!
		\brief Common libgotvptop arguments
		\return libgotvptop arguments (QStringList)
	*/
    GOTVQT_CORE_EXPORT QStringList args();

    /*!
        \brief Set plugin path
        \param path plugin path (QString)
        \return success status
    */
    GOTVQT_CORE_EXPORT bool setPluginPath(const QString &path);
}

#endif // GOTVQT_COMMON_H_
