/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2014 Tadej Novak <tadej@tano.si>
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

#ifndef GOTVQT_STATS_H_
#define GOTVQT_STATS_H_

#include "SharedExportCore.h"

/*!
    \struct NlcPtoPStats Stats.h core/Stats.h
    \ingroup GOTVQtCore
    \brief Statistics container

    A direct copy of libgotvptop stats object to remove dependency to libgotvptop.
*/
struct GOTVQT_CORE_EXPORT NlcPtoPStats {
    bool valid;              /*!< GOTV stats success status */
    int read_bytes;          /*!< libGOTV stats */
    float input_bitrate;     /*!< libGOTV stats */
    int demux_read_bytes;    /*!< libGOTV stats */
    float demux_bitrate;     /*!< libGOTV stats */
    int demux_corrupted;     /*!< libGOTV stats */
    int demux_discontinuity; /*!< libGOTV stats */
    int decoded_video;       /*!< libGOTV stats */
    int decoded_audio;       /*!< libGOTV stats */
    int displayed_pictures;  /*!< libGOTV stats */
    int lost_pictures;       /*!< libGOTV stats */
    int played_abuffers;     /*!< libGOTV stats */
    int lost_abuffers;       /*!< libGOTV stats */
    int sent_packets;        /*!< libGOTV stats */
    int sent_bytes;          /*!< libGOTV stats */
    float send_bitrate;      /*!< libGOTV stats */
};

#endif // GOTVQT_STATS_H_
