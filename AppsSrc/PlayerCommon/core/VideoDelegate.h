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

#ifndef GOTVQT_VIDEODELEGATE_H_
#define GOTVQT_VIDEODELEGATE_H_

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

#include "SharedExportCore.h"

class NlcPtoPMediaPlayer;

/*!
    \class NlcPtoPVideoDelegate VideoDelegate.h core/VideoDelegate.h
    \ingroup GOTVQtCore
    \brief Video delegate

    This is a helper class, to separate core from display.
*/
class GOTVQT_CORE_EXPORT NlcPtoPVideoDelegate
{
public:
    /*!
        \brief NlcPtoPVideoDelegate constructor
    */
    NlcPtoPVideoDelegate() {}

    /*!
        \brief NlcPtoPVideoDelegate destructor
    */
    virtual ~NlcPtoPVideoDelegate() {}

    /*!
        \brief Request new video frame.

        Request new video frame and its widget ID to set it in the instance.
        \return widget ID (WId)
        \sa NlcPtoPMediaPlayer::NlcPtoPMediaPlayer()
    */
    virtual WId request() = 0;

    /*!
        \brief Release current video frame.
    */
    virtual void release() = 0;
};

#endif // GOTVQT_VIDEODELEGATE_H_
