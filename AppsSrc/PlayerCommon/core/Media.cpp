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

#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <gotvptop/gotvptop.h>

#include "core/Error.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/Stats.h"

GoTvPtoPMedia::GoTvPtoPMedia(const QString &location,
                   bool localFile,
                   GoTvPtoPInstance *instance)
    : QObject(instance)
{
    initMedia(location, localFile, instance);
}

GoTvPtoPMedia::GoTvPtoPMedia(const QString &location,
                   GoTvPtoPInstance *instance)
    : QObject(instance)
{
    initMedia(location, false, instance);
}

GoTvPtoPMedia::GoTvPtoPMedia(libgotvptop_media_t *media)
{
    // Create a new libgotvptop media descriptor from existing one
    _gotvptopMedia = libgotvptop_media_duplicate(media);

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMedia::~GoTvPtoPMedia()
{
    removeCoreConnections();

    libgotvptop_media_release(_gotvptopMedia);

    GoTvPtoPError::showErrmsg();
}

libgotvptop_media_t *GoTvPtoPMedia::core()
{
    return _gotvptopMedia;
}

void GoTvPtoPMedia::initMedia(const QString &location,
                         bool localFile,
                         GoTvPtoPInstance *instance)
{
    _currentLocation = location;
    QString l = location;
    if (localFile)
        l = QDir::toNativeSeparators(l);

    // Create a new libgotvptop media descriptor from location
    if (localFile)
        _gotvptopMedia = libgotvptop_media_new_path(instance->core(), l.toUtf8().data());
    else
        _gotvptopMedia = libgotvptop_media_new_location(instance->core(), l.toUtf8().data());

    _gotvptopEvents = libgotvptop_media_event_manager(_gotvptopMedia);

    createCoreConnections();

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMedia::createCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaMetaChanged
         << libgotvptop_MediaSubItemAdded
         << libgotvptop_MediaDurationChanged
         << libgotvptop_MediaParsedChanged
         << libgotvptop_MediaFreed
         << libgotvptop_MediaStateChanged;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_attach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMedia::removeCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaMetaChanged
         << libgotvptop_MediaSubItemAdded
         << libgotvptop_MediaDurationChanged
         << libgotvptop_MediaParsedChanged
         << libgotvptop_MediaFreed
         << libgotvptop_MediaStateChanged;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_detach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

bool GoTvPtoPMedia::parsed() const
{
    int parsed = libgotvptop_media_is_parsed(_gotvptopMedia);

    GoTvPtoPError::showErrmsg();

    return parsed;
}

void GoTvPtoPMedia::parse()
{
    libgotvptop_media_parse_async(_gotvptopMedia);

    GoTvPtoPError::showErrmsg();
}

QString GoTvPtoPMedia::currentLocation() const
{
    return _currentLocation;
}

GoTvPtoPStats *GoTvPtoPMedia::getStats()
{
    libgotvptop_media_stats_t *coreStats = new libgotvptop_media_stats_t;

    GoTvPtoPStats *stats = new GoTvPtoPStats;
    stats->valid = libgotvptop_media_get_stats(_gotvptopMedia, coreStats);

    stats->read_bytes = coreStats->i_read_bytes;
    stats->input_bitrate = coreStats->f_input_bitrate;
    stats->demux_read_bytes = coreStats->i_demux_read_bytes;
    stats->demux_bitrate = coreStats->f_demux_bitrate;
    stats->demux_corrupted = coreStats->i_demux_corrupted;
    stats->demux_discontinuity = coreStats->i_demux_discontinuity;
    stats->decoded_video = coreStats->i_decoded_video;
    stats->decoded_audio = coreStats->i_decoded_audio;
    stats->displayed_pictures = coreStats->i_displayed_pictures;
    stats->lost_pictures = coreStats->i_lost_pictures;
    stats->played_abuffers = coreStats->i_played_abuffers;
    stats->lost_abuffers = coreStats->i_lost_abuffers;
    stats->sent_packets = coreStats->i_sent_packets;
    stats->sent_bytes = coreStats->i_sent_bytes;
    stats->send_bitrate = coreStats->f_send_bitrate;

    return stats;
}

GoTvPtoP::State GoTvPtoPMedia::state() const
{
    libgotvptop_state_t state;
    state = libgotvptop_media_get_state(_gotvptopMedia);

    GoTvPtoPError::showErrmsg();

    return GoTvPtoP::State(state);
}

qint64 GoTvPtoPMedia::duration() const
{
    libgotvptop_time_t duration = libgotvptop_media_get_duration(_gotvptopMedia);

    GoTvPtoPError::showErrmsg();

    return duration;
}

QString GoTvPtoPMedia::duplicate(const QString &name,
                            const QString &path,
                            const GoTvPtoP::Mux &mux)
{
    return record(name, path, mux, true);
}

QString GoTvPtoPMedia::duplicate(const QString &name,
                            const QString &path,
                            const GoTvPtoP::Mux &mux,
                            const GoTvPtoP::AudioCodec &audioCodec,
                            const GoTvPtoP::VideoCodec &videoCodec)
{
    return record(name, path, mux, audioCodec, videoCodec, true);
}

QString GoTvPtoPMedia::duplicate(const QString &name,
                            const QString &path,
                            const GoTvPtoP::Mux &mux,
                            const GoTvPtoP::AudioCodec &audioCodec,
                            const GoTvPtoP::VideoCodec &videoCodec,
                            int bitrate,
                            int fps,
                            int scale)
{
    return record(name, path, mux, audioCodec, videoCodec, bitrate, fps, scale, true);
}

QString GoTvPtoPMedia::merge(const QString &name,
                        const QString &path,
                        const GoTvPtoP::Mux &mux)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "gather:std{access=file,mux=%1,dst='%2'}";
    parameters = parameters.arg(GoTvPtoP::mux()[mux], l + "." + GoTvPtoP::mux()[mux]);

    option1 = ":sout-keep";
    option2 = ":sout=#%1";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    GoTvPtoPError::showErrmsg();

    return l + "." + GoTvPtoP::mux()[mux];
}

QString GoTvPtoPMedia::record(const QString &name,
                         const QString &path,
                         const GoTvPtoP::Mux &mux,
                         bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "std{access=file,mux=%1,dst='%2'}";
    parameters = parameters.arg(GoTvPtoP::mux()[mux], l + "." + GoTvPtoP::mux()[mux]);

    if (duplicate) {
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    } else {
        option2 = ":sout=#%1";
    }

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    GoTvPtoPError::showErrmsg();

    return l + "." + GoTvPtoP::mux()[mux];
}

QString GoTvPtoPMedia::record(const QString &name,
                         const QString &path,
                         const GoTvPtoP::Mux &mux,
                         const GoTvPtoP::AudioCodec &audioCodec,
                         const GoTvPtoP::VideoCodec &videoCodec,
                         bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "transcode{vcodec=%1,acodec=%2}:std{access=file,mux=%3,dst='%4'}";
    parameters = parameters.arg(GoTvPtoP::videoCodec()[videoCodec], GoTvPtoP::audioCodec()[audioCodec], GoTvPtoP::mux()[mux], l + "." + GoTvPtoP::mux()[mux]);

    if (duplicate) {
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    } else {
        option2 = ":sout=#%1";
    }

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    GoTvPtoPError::showErrmsg();

    return l + "." + GoTvPtoP::mux()[mux];
}

QString GoTvPtoPMedia::record(const QString &name,
                         const QString &path,
                         const GoTvPtoP::Mux &mux,
                         const GoTvPtoP::AudioCodec &audioCodec,
                         const GoTvPtoP::VideoCodec &videoCodec,
                         int bitrate,
                         int fps,
                         int scale,
                         bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "transcode{vcodec=%1,vb=%2,fps=%3,scale=%4,acodec=%5}:std{access=file,mux=%6,dst='%7'}";
    parameters = parameters.arg(GoTvPtoP::videoCodec()[videoCodec], QString::number(bitrate), QString::number(fps), QString::number(scale), GoTvPtoP::audioCodec()[audioCodec], GoTvPtoP::mux()[mux], l + "." + GoTvPtoP::mux()[mux]);

    if (duplicate) {
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    } else {
        option2 = ":sout=#%1";
    }

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    GoTvPtoPError::showErrmsg();

    return l + "." + GoTvPtoP::mux()[mux];
}

void GoTvPtoPMedia::setProgram(int program)
{
    QString option = "program=%1";
    setOption(option.arg(program));
}

void GoTvPtoPMedia::setOption(const QString &option)
{
    libgotvptop_media_add_option(_gotvptopMedia, option.toUtf8().data());

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMedia::setOptions(const QStringList &options)
{
    foreach (const QString &option, options) {
        libgotvptop_media_add_option(_gotvptopMedia, option.toUtf8().data());
    }

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMedia::libgotvptop_callback(const libgotvptop_event_t *event,
                               void *data)
{
    GoTvPtoPMedia *core = static_cast<GoTvPtoPMedia *>(data);

    switch (event->type) {
    case libgotvptop_MediaMetaChanged:
        emit core->metaChanged(GoTvPtoP::Meta(event->u.media_meta_changed.meta_type));
        break;
    case libgotvptop_MediaSubItemAdded:
        emit core->subitemAdded(event->u.media_subitem_added.new_child);
        break;
    case libgotvptop_MediaDurationChanged:
        emit core->durationChanged(event->u.media_duration_changed.new_duration);
        break;
    case libgotvptop_MediaParsedChanged:
        emit core->parsedChanged(event->u.media_parsed_changed.new_status);
        emit core->parsedChanged((bool)event->u.media_parsed_changed.new_status);
        break;
    case libgotvptop_MediaFreed:
        emit core->freed(event->u.media_freed.md);
        break;
    case libgotvptop_MediaStateChanged:
        emit core->stateChanged(GoTvPtoP::State(event->u.media_state_changed.new_state));
        break;
    default:
        break;
    }
}
