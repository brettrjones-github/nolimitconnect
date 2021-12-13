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

#include <cmath>
#include <gotvptop/gotvptop.h>

#include "core/Equalizer.h"
#include "core/Error.h"
#include "core/MediaPlayer.h"

GoTvPtoPEqualizer::GoTvPtoPEqualizer(GoTvPtoPMediaPlayer *gotvptopMediaPlayer)
    : QObject(gotvptopMediaPlayer),
      _gotvptopMediaPlayer(gotvptopMediaPlayer),
      _gotvptopEqualizer(libgotvptop_audio_equalizer_new()) {}

GoTvPtoPEqualizer::~GoTvPtoPEqualizer()
{
    if (_gotvptopEqualizer) {
        libgotvptop_audio_equalizer_release(_gotvptopEqualizer);
    }
}

float GoTvPtoPEqualizer::amplificationForBandAt(uint bandIndex) const
{
    if (_gotvptopEqualizer) {
        float ret = libgotvptop_audio_equalizer_get_amp_at_index(_gotvptopEqualizer, bandIndex);
        if (!std::isnan(ret)) {
            return ret;
        }
    }
    return -1.0;
}

uint GoTvPtoPEqualizer::bandCount() const
{
    if (_gotvptopEqualizer) {
        return libgotvptop_audio_equalizer_get_band_count();
    } else {
        return 0;
    }
}

float GoTvPtoPEqualizer::bandFrequency(uint bandIndex) const
{
    if (_gotvptopEqualizer) {
        return libgotvptop_audio_equalizer_get_band_frequency(bandIndex);
    } else {
        return -1.0;
    }
}

float GoTvPtoPEqualizer::preamplification() const
{
    if (_gotvptopEqualizer) {
        return libgotvptop_audio_equalizer_get_preamp(_gotvptopEqualizer);
    } else {
        return 0.0;
    }
}

uint GoTvPtoPEqualizer::presetCount() const
{
    return libgotvptop_audio_equalizer_get_preset_count();
}

QString GoTvPtoPEqualizer::presetNameAt(uint index) const
{
    const char *name = libgotvptop_audio_equalizer_get_preset_name(index);
    if (name == NULL) {
        return QString();
    } else {
        return QString(name);
    }
}

void GoTvPtoPEqualizer::loadFromPreset(uint index)
{
    if (_gotvptopEqualizer) {
        libgotvptop_audio_equalizer_release(_gotvptopEqualizer);
    }
    _gotvptopEqualizer = libgotvptop_audio_equalizer_new_from_preset(index);
    GoTvPtoPError::showErrmsg();
    if (_gotvptopEqualizer) {
        emit presetLoaded();
    }
}

void GoTvPtoPEqualizer::setAmplificationForBandAt(float amp, uint bandIndex)
{
    if (!_gotvptopEqualizer) {
        return;
    }
    libgotvptop_audio_equalizer_set_amp_at_index(_gotvptopEqualizer, amp, bandIndex);
    libgotvptop_media_player_set_equalizer(_gotvptopMediaPlayer->core(), _gotvptopEqualizer);
    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPEqualizer::setEnabled(bool enabled)
{
    if (enabled && _gotvptopEqualizer != NULL) {
        libgotvptop_media_player_set_equalizer(_gotvptopMediaPlayer->core(), _gotvptopEqualizer);
    } else {
        libgotvptop_media_player_set_equalizer(_gotvptopMediaPlayer->core(), NULL);
    }
}

void GoTvPtoPEqualizer::setPreamplification(float value)
{
    if (!_gotvptopEqualizer) {
        return;
    }
    libgotvptop_audio_equalizer_set_preamp(_gotvptopEqualizer, value);
    libgotvptop_media_player_set_equalizer(_gotvptopMediaPlayer->core(), _gotvptopEqualizer);
    GoTvPtoPError::showErrmsg();
}
