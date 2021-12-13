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

#ifndef GOTVQT_VIDEOSTREAM_H_
#define GOTVQT_VIDEOSTREAM_H_

#include <deque>
#include <list>
#include <memory>

#include <QtCore/QObject>

#include "AbstractVideoFrame.h"
#include "AbstractVideoStream.h"
#include "Enums.h"
#include "SharedExportCore.h"

class GoTvPtoPMediaPlayer;

/*!
    \class GoTvPtoPVideoStream VideoStream.h core/VideoStream.h
    \ingroup GOTVQtCore
    \brief Video memory stream

    GoTvPtoPVideoStream sets proper callbacks to get YUV frames from libGOTV.
    This class should be subclassed and implement frameUpdated to specify what to do with the frame.

    \see GoTvPtoPQmlVideoStream
    \see GoTvPtoPAbstractVideoFrame
    \since GOTV-Qt 1.1
 */
class GOTVQT_CORE_EXPORT GoTvPtoPVideoStream : public QObject,
                                         public GoTvPtoPAbstractVideoStream
{
    Q_OBJECT
public:
    /*!
        \brief GoTvPtoPVideoStream constructor
        \param format rendering format
        \param parent parent object
     */
    explicit GoTvPtoPVideoStream(GoTvPtoP::RenderFormat format,
                            QObject *parent = 0);
    ~GoTvPtoPVideoStream();

    /*!
        \brief Rendering format
        \return current rendering format
     */
    GoTvPtoP::RenderFormat format() const { return _format; } // LCOV_EXCL_LINE

    /*!
        \brief Initialise video memory stream with player
        \param player media player
     */
    void init(GoTvPtoPMediaPlayer *player);

    /*!
        \brief Prepare video memory stream for deletion
     */
    void deinit();

    /*!
        \brief Get current frame
        \return current frame
     */
    std::shared_ptr<const GoTvPtoPAbstractVideoFrame> renderFrame() const { return _renderFrame; } // LCOV_EXCL_LINE

private:
    Q_INVOKABLE virtual void frameUpdated() = 0;

    unsigned formatCallback(char *chroma,
                            unsigned *width,
                            unsigned *height,
                            unsigned *pitches,
                            unsigned *lines);
    void formatCleanUpCallback();

    void *lockCallback(void **planes);
    void unlockCallback(void *picture,
                        void *const *planes);
    void displayCallback(void *picture);

    std::shared_ptr<GoTvPtoPAbstractVideoFrame> cloneFrame(std::shared_ptr<GoTvPtoPAbstractVideoFrame> frame);

    GoTvPtoP::RenderFormat _format;
    GoTvPtoPMediaPlayer *_player;

    std::deque<std::shared_ptr<GoTvPtoPAbstractVideoFrame>> _frames;
    std::list<std::shared_ptr<GoTvPtoPAbstractVideoFrame>> _lockedFrames;
    std::shared_ptr<GoTvPtoPAbstractVideoFrame> _renderFrame;
};

#endif // GOTVQT_VIDEOSTREAM_H_
