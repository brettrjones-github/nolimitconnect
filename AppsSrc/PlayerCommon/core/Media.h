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

#ifndef GOTVQT_MEDIA_H_
#define GOTVQT_MEDIA_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "Enums.h"
#include "SharedExportCore.h"

class NlcPtoPInstance;
struct NlcPtoPStats;

struct libgotvptop_event_t;
struct libgotvptop_event_manager_t;
struct libgotvptop_media_t;

/*!
    \class NlcPtoPMedia Media.h core/Media.h
    \ingroup GOTVQtCore
    \brief Media item

    An abstract representation of a playable media.
    It consists of a media location and various optional meta data.
*/
class GOTVQT_CORE_EXPORT NlcPtoPMedia : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief NlcPtoPMedia constructor.

        This constructor creates a new media instance from a media location.

        \param location location of the media (QString)
        \param localFile true, if media is local file (bool)
        \param instance main libgotvptop instance (NlcPtoPInstance *)
    */
    explicit NlcPtoPMedia(const QString &location,
                      bool localFile,
                      NlcPtoPInstance *instance);

    /*!
        \brief NlcPtoPMedia constructor.

        This constructor creates a new media instance from a remote media location.
        Provided for convenience.

        \param location remote location of the media (QString)
        \param instance main libgotvptop instance (NlcPtoPInstance *)
    */
    explicit NlcPtoPMedia(const QString &location,
                      NlcPtoPInstance *instance);

    /*!
        \brief NlcPtoPMedia constructor.

        This constructor creates a new media instance from an existing one.

        \param media libgotvptop media item (libgotvptop_media_t *)
    */
    NlcPtoPMedia(libgotvptop_media_t *media);

    /*!
        \brief NlcPtoPMedia destructor
    */
    ~NlcPtoPMedia();

    /*!
        \brief libgotvptop media item
        \return libgotvptop media item (libgotvptop_media_t *)
    */
    libgotvptop_media_t *core();

    /*!
        \brief Get parsed status

        Know when it is OK to read metadata and track information

        \return parsed status (const bool)
        \since GOTV-Qt 1.1
    */
    bool parsed() const;

    /*!
        \brief Parse media information

        Parse media information: metadata and track information.
        This call is async.

        \see parsedChanged
        \since GOTV-Qt 1.1
    */
    void parse();

    /*!
        \brief Current media location

        \return QString media location
    */
    QString currentLocation() const;

    /*!
        \brief Get media stats

        \return NlcPtoPStats media stats object
    */
    NlcPtoPStats *getStats();

    /*!
        \brief Get media state
        \return current media state
        \since GOTV-Qt 1.1
    */
    NlcPtoP::State state() const;

    /*!
        \brief Get media duration
        \return duration
        \since GOTV-Qt 1.1
    */
    qint64 duration() const;

    /*!
        \brief Duplicate (provided for convenience)

        Apply duplicate options with desired mux but without transcoding.

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \return QString output file
        \sa record()
    */
    QString duplicate(const QString &name,
                      const QString &path,
                      const NlcPtoP::Mux &mux);

    /*!
        \brief Duplicate (provided for convenience)

        Apply duplicate options with desired mux and transcoding (experimental).

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \param audioCodec audio codec (NlcPtoP::AudioCodec)
        \param videoCodec video codec (NlcPtoP::VideoCodec)
        \return QString output file
        \sa record()
    */
    QString duplicate(const QString &name,
                      const QString &path,
                      const NlcPtoP::Mux &mux,
                      const NlcPtoP::AudioCodec &audioCodec,
                      const NlcPtoP::VideoCodec &videoCodec);

    /*!
        \brief Duplicate (provided for convenience)

        Apply duplicate options with desired mux, transcoding
        and some other options (experimental).

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \param audioCodec audio codec (NlcPtoP::AudioCodec)
        \param videoCodec video codec (NlcPtoP::VideoCodec)
        \param bitrate video bitrate (int)
        \param fps frames per second (int)
        \param scale video scale (int)
        \return QString output file
        \sa record()
    */
    QString duplicate(const QString &name,
                      const QString &path,
                      const NlcPtoP::Mux &mux,
                      const NlcPtoP::AudioCodec &audioCodec,
                      const NlcPtoP::VideoCodec &videoCodec,
                      int bitrate,
                      int fps,
                      int scale);

    /*!
        \brief Merge

        Apply merge options with desired mux but without transcoding.

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \return QString output file
    */
    QString merge(const QString &name,
                  const QString &path,
                  const NlcPtoP::Mux &mux);

    /*!
        \brief Record

        Apply recording options with desired mux but without transcoding.

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \param duplicate also duplicate on screen (bool)
        \return QString output file
    */
    QString record(const QString &name,
                   const QString &path,
                   const NlcPtoP::Mux &mux,
                   bool duplicate = false);

    /*!
        \brief Record

        Apply recording options with desired mux and transcoding (experimental).

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \param audioCodec audio codec (NlcPtoP::AudioCodec)
        \param videoCodec video codec (NlcPtoP::VideoCodec)
        \param duplicate also duplicate on screen (bool)
        \return QString output file
    */
    QString record(const QString &name,
                   const QString &path,
                   const NlcPtoP::Mux &mux,
                   const NlcPtoP::AudioCodec &audioCodec,
                   const NlcPtoP::VideoCodec &videoCodec,
                   bool duplicate = false);

    /*!
        \brief Record

        Apply recording options with desired mux, transcoding
        and some other options (experimental).

        \param name output file name (QString)
        \param path output path (QString)
        \param mux output mux (NlcPtoP::Mux)
        \param audioCodec audio codec (NlcPtoP::AudioCodec)
        \param videoCodec video codec (NlcPtoP::VideoCodec)
        \param bitrate video bitrate (int)
        \param fps frames per second (int)
        \param scale video scale (int)
        \param duplicate also duplicate on screen (bool)
        \return QString output file
    */
    QString record(const QString &name,
                   const QString &path,
                   const NlcPtoP::Mux &mux,
                   const NlcPtoP::AudioCodec &audioCodec,
                   const NlcPtoP::VideoCodec &videoCodec,
                   int bitrate,
                   int fps,
                   int scale,
                   bool duplicate = false);

    /*!
        \brief Set program
        \param program program PID (int)
    */
    void setProgram(int program);

    /*!
        \brief Set media option
        \param option media option (QString)
    */
    void setOption(const QString &option);

    /*!
        \brief Set media options
        \param options media options (QStringList)
    */
    void setOptions(const QStringList &options);

signals:
    /*!
        \brief Signal sent on meta change
        \param meta new meta
    */
    void metaChanged(const NlcPtoP::Meta &meta);

    /*!
        \brief Signal sent on subitem added
        \param subitem subitem that was added
    */
    void subitemAdded(libgotvptop_media_t *subitem);

    /*!
        \brief Signal sent on duration change
        \param duration new duration
    */
    void durationChanged(int duration);

    /*!
        \brief Signal sent on parsed change
        \param status new parsed status
        \deprecated Deprecated since GOTV-Qt 1.1, will be removed in 2.0
    */
    Q_DECL_DEPRECATED void parsedChanged(int status);

    /*!
        \brief Signal sent on parsed change
        \param status new parsed status
    */
    void parsedChanged(bool status);

    /*!
        \brief Signal sent on freed
        \param media freed libgotvptop_media_t object
    */
    void freed(libgotvptop_media_t *media);

    /*!
        \brief Signal sent on state change
        \param state new state
    */
    void stateChanged(const NlcPtoP::State &state);

private:
    void initMedia(const QString &location,
                   bool localFile,
                   NlcPtoPInstance *instance);

    static void libgotvptop_callback(const libgotvptop_event_t *event,
                                void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libgotvptop_media_t *_gotvptopMedia;
    libgotvptop_event_manager_t *_gotvptopEvents;

    QString _currentLocation;
};

#endif // GOTVQT_MEDIA_H_
