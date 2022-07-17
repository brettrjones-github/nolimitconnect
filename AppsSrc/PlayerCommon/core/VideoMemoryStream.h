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

#ifndef GOTVQT_VIDEOMEMORYSTREAM_H_
#define GOTVQT_VIDEOMEMORYSTREAM_H_

#include "SharedExportCore.h"

#if defined(GOTVQT_CORE_LIBRARY) || defined(GOTVQT_QML_LIBRARY)
/* MSVC support fix */
#if defined(_MSC_VER)
#   include <BaseTsd.h>
#endif
/* MSVC + MinGW support fix */
#if defined (_WIN32)
#   define LIBNLC_USE_PTHREAD_CANCEL 1
#endif
#include <gotvptop_common.h>
#include <gotvptop_fourcc.h>
#else
struct gotvptop_chroma_description_t;
#endif

class NlcPtoPMediaPlayer;

/*!
    \class NlcPtoPVideoMemoryStream VideoMemoryStream.h core/VideoMemoryStream.h
    \ingroup GOTVQtCore
    \brief Video memory stream (deprecated)

    NlcPtoPVideoMemoryStream is a template class for creating own video rendering engines.

    \see NlcPtoPAbstractVideoStream
    \deprecated Deprecated since GOTV-Qt 1.1, will be removed in 2.0
 */
class Q_DECL_DEPRECATED GOTVQT_CORE_EXPORT NlcPtoPVideoMemoryStream
{
public:
    explicit NlcPtoPVideoMemoryStream();
    virtual ~NlcPtoPVideoMemoryStream();
    
    /*!
        \brief Set required information for rendering video

        \returns overall buffersize needed
     */
    static unsigned setPitchAndLines(const gotvptop_chroma_description_t *chromaDescription,
                                     unsigned width,
                                     unsigned height,
                                     unsigned *pitches,
                                     unsigned *lines,
                                     unsigned *visiblePitches = 0,
                                     unsigned *visibleLines = 0);

    /*!
        \brief Set NlcPtoPMediaPlayer callbacks
        \param player media player (NlcPtoPMediaPlayer *)
     */
    void setCallbacks(NlcPtoPMediaPlayer *player);

    /*!
        \brief Unset NlcPtoPMediaPlayer callbacks
        \param player media player (NlcPtoPMediaPlayer *)
     */
    void unsetCallbacks(NlcPtoPMediaPlayer *player);

protected:
    /*!
        \brief Lock callback
     */
    virtual void *lockCallback(void **planes) = 0;

    /*!
        \brief Unlock callback
     */
    virtual void unlockCallback(void *picture,
                                void *const *planes) = 0;

    /*!
        \brief Display callback
     */
    virtual void displayCallback(void *picture) = 0;

    /*!
        \brief Format callback
     */
    virtual unsigned formatCallback(char *chroma,
                                    unsigned *width,
                                    unsigned *height,
                                    unsigned *pitches,
                                    unsigned *lines) = 0;

    /*!
        \brief Format cleanup callback
     */
    virtual void formatCleanUpCallback() = 0;

private:
    static void *lockCallbackInternal(void *opaque,
                                      void **planes);
    static void unlockCallbackInternal(void *opaque,
                                       void *picture,
                                       void *const *planes);
    static void displayCallbackInternal(void *opaque,
                                        void *picture);

    static unsigned formatCallbackInternal(void **opaque,
                                           char *chroma,
                                           unsigned *width,
                                           unsigned *height,
                                           unsigned *pitches,
                                           unsigned *lines);
    static void formatCleanUpCallbackInternal(void *opaque);
};

#endif // GOTVQT_VIDEOMEMORYSTREAM_H_
