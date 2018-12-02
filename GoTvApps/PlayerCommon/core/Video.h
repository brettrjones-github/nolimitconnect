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

#ifndef GOTVQT_VIDEO_H_
#define GOTVQT_VIDEO_H_

#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include "Enums.h"
#include "SharedExportCore.h"

class GoTvPtoPMediaPlayer;

struct libgotvptop_media_player_t;

/*!
    \class GoTvPtoPVideo Video.h core/Video.h
    \ingroup GOTVQtCore
    \brief Video controls

    A group of video controls functions.
*/
class GOTVQT_CORE_EXPORT GoTvPtoPVideo : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief GoTvPtoPVideo constructor.

        This constructor creates a new video manager.

        \param player media player (GoTvPtoPMediaPlayer *)
    */
    explicit GoTvPtoPVideo(GoTvPtoPMediaPlayer *player);

    /*!
        \brief GoTvPtoPVideo destructor
    */
    ~GoTvPtoPVideo();

    /*!
        \brief Get current video aspect ratio.
        \return current video aspect-ratio or NULL if not specified (const GoTvPtoP::Ratio)
    */
    GoTvPtoP::Ratio aspectRatio() const;

    /*!
        \brief Get current crop filter geometry.
        \return current crop filter geometry or NULL if not specified (const GoTvPtoP::Ratio)
    */
    GoTvPtoP::Ratio cropGeometry() const;

    /*!
        \brief Hide logo.
    */
    void hideLogo();

    /*!
        \brief Hide marquee.
    */
    void hideMarquee();

    /*!
        \brief Get current scale ratio.
        \return current scale ratio or NULL if not specified (const GoTvPtoP::Scale)
    */
    GoTvPtoP::Scale scale() const;

    /*!
        \brief Set new video aspect ratio.
        \param ratio new video aspect-ratio or NULL to reset to default (GoTvPtoP::Ratio)
    */
    void setAspectRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Set new crop filter geometry.
        \param ratio new crop filter geometry or NULL to reset to default (GoTvPtoP::Ratio)
    */
    void setCropGeometry(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Enable or disable deinterlace filter.
        \param filter type of deinterlace filter or NULL to reset to default (GoTvPtoP::Deinterlacing)
    */
    void setDeinterlace(const GoTvPtoP::Deinterlacing &filter);

    /*!
        \brief Set new scale ratio.
        \param scale scale ratio or NULL to reset to default (GoTvPtoP::Scale)
    */
    void setScale(const GoTvPtoP::Scale &scale);

    /*!
        \brief Set current video subtitle.
        \param subtitle new video subtitle (int)
    */
    void setSubtitle(int subtitle);

    /*!
        \brief Set new video subtitle file.
        \param subtitle new video subtitle file (QString)
    */
    void setSubtitleFile(const QString &subtitle);

    /*!
        \brief Set new teletext page to retrieve.
        \param page new teletext page number (int)
    */
    void setTeletextPage(int page);

    /*!
        \brief Set current video track.
        \param track new video track (int)
    */
    void setTrack(int track);

    /*!
        \brief Show logo.
        \param file logo file (QString)
        \param x x-coordinate (int)
        \param y y-coordinate (int)
        \param opacity logo opacity (int)
    */
    void showLogo(const QString &file,
                  int x,
                  int y,
                  int opacity = 255);

    /*!
        \brief Show logo.
        \param text text to display (QString)
        \param x x-coordinate (int)
        \param y y-coordinate (int)
        \param timeout marquee timeout
        \param opacity opacity from 0 to 255 (int)
        \param size size (int)
        \param color color in base 16 int (int)
        \param refresh refresh rate (int)
    */
    void showMarquee(const QString &text,
                     int x,
                     int y,
                     int timeout = 0,
                     int opacity = 255,
                     int size = -1,
                     int color = 0xFFFFFF,
                     int refresh = 0);

    /*!
        \brief Get current video size. (currently primary only)
        \return size of the video (QSize)
    */
    QSize size() const;

    /*!
        \brief Get current video subtitle.
        \return the number of current video subtitle, or -1 if none (const int)
    */
    int subtitle() const;

    /*!
        \brief Get number of available video subtitles.
        \return the number of available video subtitles, or -1 if unavailable (const int)
    */
    int subtitleCount() const;

    /*!
        \brief Get the descriptions of available video subtitles.
        \return list with descriptions of available video subtitles (const QStringList)
    */
    QStringList subtitleDescription() const;

    /*!
        \brief Get the ids of available video subtitles.
        \return list with ids of available video subtitles (const QList<int>)
    */
    QList<int> subtitleIds() const;

    /*!
        \brief Get available video subtitles.
        \return dictionary with available video subtitles (const QMap<int, QString>)
        \since GOTV-Qt 1.1
    */
    QMap<int, QString> subtitles() const;

    /*!
        \brief Take a snapshot of current video (currently primary only)
        \param path output path (QString)
        \return true if successfull (bool)
    */
    bool takeSnapshot(const QString &path) const;

    /*!
        \brief Get current teletext page requested.
        \return current teletext page number (int)
    */
    int teletextPage() const;

    /*!
        \brief Toggle teletext transparent status on video output.
    */
    void toggleTeletextTransparency();

    /*!
        \brief Get current video track.
        \return the number of current video track, or -1 if none (const int)
    */
    int track() const;

    /*!
        \brief Get number of available video tracks.
        \return the number of available video tracks, or -1 if unavailable (const int)
    */
    int trackCount() const;

    /*!
        \brief Get the description of available video tracks.
        \return list with description of available video tracks (const QStringList)
    */
    QStringList trackDescription() const;

    /*!
        \brief Get the ids of available video tracks.
        \return list with ids of available video tracks (const QList<int>)
    */
    QList<int> trackIds() const;

    /*!
        \brief Get available video tracks.
        \return dictionary with available video tracks (const QMap<int, QString>)
        \since GOTV-Qt 1.1
    */
    QMap<int, QString> tracks() const;

private:
    libgotvptop_media_player_t *_gotvptopMediaPlayer;
};

#endif // GOTVQT_VIDEO_H_
