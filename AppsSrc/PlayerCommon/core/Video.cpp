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

#include <gotvptop/gotvptop.h>

#include "core/Error.h"
#include "core/MediaPlayer.h"
#include "core/Video.h"

GoTvPtoPVideo::GoTvPtoPVideo(GoTvPtoPMediaPlayer *player)
    : QObject(player),
      _gotvptopMediaPlayer(player->core()) {}

GoTvPtoPVideo::~GoTvPtoPVideo() {}

GoTvPtoP::Ratio GoTvPtoPVideo::aspectRatio() const
{
    QString ratio = "";
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        ratio = libgotvptop_video_get_aspect_ratio(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return GoTvPtoP::Ratio(GoTvPtoP::ratio().indexOf(ratio));
}

GoTvPtoP::Ratio GoTvPtoPVideo::cropGeometry() const
{
    QString crop = "";
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        crop = libgotvptop_video_get_crop_geometry(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return GoTvPtoP::Ratio(GoTvPtoP::ratio().indexOf(crop));
}

void GoTvPtoPVideo::hideLogo()
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_logo_int(_gotvptopMediaPlayer, libgotvptop_logo_enable, 0);
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::hideMarquee()
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Enable, 0);
        GoTvPtoPError::showErrmsg();
    }
}

GoTvPtoP::Scale GoTvPtoPVideo::scale() const
{
    float scale = 0;
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        scale = libgotvptop_video_get_scale(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return GoTvPtoP::Scale(GoTvPtoP::scale().indexOf(scale));
}

void GoTvPtoPVideo::setAspectRatio(const GoTvPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        QString ratioOut = ratio == GoTvPtoP::Ignore ? "" : GoTvPtoP::ratio()[ratio];
        libgotvptop_video_set_aspect_ratio(_gotvptopMediaPlayer, ratioOut.toUtf8().data());
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setCropGeometry(const GoTvPtoP::Ratio &ratio)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        QString ratioOut = ratio == GoTvPtoP::Ignore ? "" : GoTvPtoP::ratio()[ratio];
        libgotvptop_video_set_crop_geometry(_gotvptopMediaPlayer, ratioOut.toUtf8().data());
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setDeinterlace(const GoTvPtoP::Deinterlacing &filter)
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_video_set_deinterlace(_gotvptopMediaPlayer, GoTvPtoP::deinterlacing()[filter].toUtf8().data());
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setScale(const GoTvPtoP::Scale &scale)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_scale(_gotvptopMediaPlayer, GoTvPtoP::scale()[scale]);
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setSubtitle(int subtitle)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_spu(_gotvptopMediaPlayer, subtitle);
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setSubtitleFile(const QString &subtitle)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_subtitle_file(_gotvptopMediaPlayer, subtitle.toUtf8().data());
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setTeletextPage(int page)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_teletext(_gotvptopMediaPlayer, page);
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::setTrack(int track)
{
    if (_gotvptopMediaPlayer) {
        libgotvptop_video_set_track(_gotvptopMediaPlayer, track);
        GoTvPtoPError::showErrmsg();
    }
}

void GoTvPtoPVideo::showLogo(const QString &file,
                        int x,
                        int y,
                        int opacity)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_logo_string(_gotvptopMediaPlayer, libgotvptop_logo_file, file.toUtf8().data());
        libgotvptop_video_set_logo_int(_gotvptopMediaPlayer, libgotvptop_logo_x, x);
        libgotvptop_video_set_logo_int(_gotvptopMediaPlayer, libgotvptop_logo_y, y);
        libgotvptop_video_set_logo_int(_gotvptopMediaPlayer, libgotvptop_logo_opacity, opacity);

        libgotvptop_video_set_logo_int(_gotvptopMediaPlayer, libgotvptop_logo_enable, 1);
    }
}

void GoTvPtoPVideo::showMarquee(const QString &text,
                           int x,
                           int y,
                           int timeout,
                           int opacity,
                           int size,
                           int color,
                           int refresh)
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_set_marquee_string(_gotvptopMediaPlayer, libgotvptop_marquee_Text, text.toUtf8().data());
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_X, x);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Y, y);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Timeout, timeout);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Opacity, opacity);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Size, size);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Color, color);
        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Refresh, refresh);

        libgotvptop_video_set_marquee_int(_gotvptopMediaPlayer, libgotvptop_marquee_Enable, 1);
    }
}

QSize GoTvPtoPVideo::size() const
{
    unsigned x = 640;
    unsigned y = 480;

    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_video_get_size(_gotvptopMediaPlayer, 0, &x, &y);
        GoTvPtoPError::showErrmsg();
    }

    return QSize(x, y);
}

int GoTvPtoPVideo::subtitle() const
{
    int subtitle = -1;
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        subtitle = libgotvptop_video_get_spu(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return subtitle;
}

int GoTvPtoPVideo::subtitleCount() const
{
    int count = -1;
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        count = libgotvptop_video_get_spu_count(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return count;
}

QStringList GoTvPtoPVideo::subtitleDescription() const
{
    QStringList descriptions;

    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_video_get_spu_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if (subtitleCount() > 1) {
            for (int i = 1; i < subtitleCount(); i++) {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> GoTvPtoPVideo::subtitleIds() const
{
    QList<int> ids;

    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_video_get_spu_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        ids << desc->i_id;
        if (subtitleCount() > 1) {
            for (int i = 1; i < subtitleCount(); i++) {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}

QMap<int, QString> GoTvPtoPVideo::subtitles() const
{
    QMap<int, QString> tracks;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc, *first;
        first = desc = libgotvptop_video_get_spu_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        if (desc != NULL) {
            tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
            if (subtitleCount() > 1) {
                for (int i = 1; i < subtitleCount(); i++) {
                    desc = desc->p_next;
                    tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
                }
            }
            libgotvptop_track_description_list_release(first);
        }
    }

    return tracks;
}

bool GoTvPtoPVideo::takeSnapshot(const QString &path) const
{
    bool success = false;
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        success = libgotvptop_video_take_snapshot(_gotvptopMediaPlayer, 0, path.toUtf8().data(), 0, 0) + 1;
        GoTvPtoPError::showErrmsg();
    }

    return success;
}

int GoTvPtoPVideo::teletextPage() const
{
    int page = -1;
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        page = libgotvptop_video_get_teletext(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return page;
}

void GoTvPtoPVideo::toggleTeletextTransparency()
{
    if (_gotvptopMediaPlayer && libgotvptop_media_player_has_vout(_gotvptopMediaPlayer)) {
        libgotvptop_toggle_teletext(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }
}

int GoTvPtoPVideo::track() const
{
    int track = -1;
    if (_gotvptopMediaPlayer) {
        track = libgotvptop_video_get_track(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return track;
}

int GoTvPtoPVideo::trackCount() const
{
    int count = -1;
    if (_gotvptopMediaPlayer) {
        count = libgotvptop_video_get_track_count(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();
    }

    return count;
}

QStringList GoTvPtoPVideo::trackDescription() const
{
    QStringList descriptions;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_video_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if (trackCount() > 1) {
            for (int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> GoTvPtoPVideo::trackIds() const
{
    QList<int> ids;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc;
        desc = libgotvptop_video_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        ids << desc->i_id;
        if (trackCount() > 1) {
            for (int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}

QMap<int, QString> GoTvPtoPVideo::tracks() const
{
    QMap<int, QString> tracks;

    if (_gotvptopMediaPlayer) {
        libgotvptop_track_description_t *desc, *first;
        first = desc = libgotvptop_video_get_track_description(_gotvptopMediaPlayer);
        GoTvPtoPError::showErrmsg();

        if (desc != NULL) {
            tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
            if (trackCount() > 1) {
                for (int i = 1; i < trackCount(); i++) {
                    desc = desc->p_next;
                    tracks.insert(desc->i_id, QString().fromUtf8(desc->psz_name));
                }
            }
            libgotvptop_track_description_list_release(first);
        }
    }

    return tracks;
}
