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

#ifndef GOTVQT_YUVVIDEOFRAME_H_
#define GOTVQT_YUVVIDEOFRAME_H_

#include "AbstractVideoFrame.h"
#include "SharedExportCore.h"

/*!
    \struct GoTvPtoPYUVVideoFrame YUVVideoFrame.h core/YUVVideoFrame.h
    \ingroup GOTVQtCore
    \brief Video YUV frame data container
    \since GOTV-Qt 1.1
*/
struct GOTVQT_CORE_EXPORT GoTvPtoPYUVVideoFrame : GoTvPtoPAbstractVideoFrame {
    /*!
        \brief GoTvPtoPVideoFrame constructor.
        This construction ensures data is set and containers prepared.
        \param width
        \param height
        \param pitches
        \param lines
    */
    GoTvPtoPYUVVideoFrame(unsigned *width,
                     unsigned *height,
                     unsigned *pitches,
                     unsigned *lines);

    /*!
        \brief GoTvPtoPVideoFrame constructor.
        This construction copies an existing frame into a new one.
        \param frame existing shared pointer to a frame
    */
    GoTvPtoPYUVVideoFrame(const std::shared_ptr<GoTvPtoPYUVVideoFrame> &frame);

    ~GoTvPtoPYUVVideoFrame();
};

#endif // GOTVQT_YUVVIDEOFRAME_H_
