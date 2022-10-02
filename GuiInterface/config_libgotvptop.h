#pragma once
#include "config_appcorelibs.h"

#define EQZ_BANDS_MAX 10

/* The frequency tables */
static const float f_vlc_frequency_table_10b[ EQZ_BANDS_MAX ] =
{
    60, 170, 310, 600, 1000, 3000, 6000, 12000, 14000, 16000,
};

static const float f_iso_frequency_table_10b[ EQZ_BANDS_MAX ] =
{
    31.25, 62.5, 125, 250, 500, 1000, 2000, 4000, 8000, 16000,
};

#define NB_PRESETS 18
static const char*const preset_list[ NB_PRESETS ] = {
    "flat", "classical", "club", "dance", "fullbass", "fullbasstreble",
    "fulltreble", "headphones","largehall", "live", "party", "pop", "reggae",
    "rock", "ska", "soft", "softrock", "techno"
};
static const char*const preset_list_text[ NB_PRESETS ] = {
    ( "Flat" ), ( "Classical" ), ( "Club" ), ( "Dance" ), ( "Full bass" ),
    ( "Full bass and treble" ), ( "Full treble" ), ( "Headphones" ),
    ( "Large Hall" ), ( "Live" ), ( "Party" ), ( "Pop" ), ( "Reggae" ),
    ( "Rock" ), ( "Ska" ),  ( "Soft" ), ( "Soft rock" ), ( "Techno" ),
};

typedef struct
{
    const char psz_name[ 16 ];
    int  i_band;
    float f_preamp;
    float f_amp[ EQZ_BANDS_MAX ];
} eqz_preset_t;

static const eqz_preset_t eqz_preset_10b[ NB_PRESETS ] =
{
    {
        "flat", 10, 12.0f,
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    },
    {
        "classical", 10, 12.0f,
        { -1.11022e-15f, -1.11022e-15f, -1.11022e-15f, -1.11022e-15f,
          -1.11022e-15f, -1.11022e-15f, -7.2f, -7.2f, -7.2f, -9.6f }
    },
    {
        "club", 10, 6.0f,
        { -1.11022e-15f, -1.11022e-15f, 8.0f, 5.6f, 5.6f, 5.6f, 3.2f,
          -1.11022e-15f, -1.11022e-15f, -1.11022e-15f }
    },
    {
        "dance", 10, 5.0f,
        { 9.6f, 7.2f, 2.4f, -1.11022e-15f, -1.11022e-15f, -5.6f, -7.2f, -7.2f,
          -1.11022e-15f, -1.11022e-15f }
    },
    {
        "fullbass", 10, 5.0f,
        { -8.0f, 9.6f, 9.6f, 5.6f, 1.6f, -4.0f, -8.0f, -10.4f, -11.2f, -11.2f }
    },
    {
        "fullbasstreble", 10, 4.0f,
        { 7.2f, 5.6f, -1.11022e-15f, -7.2f, -4.8f, 1.6f, 8.0f, 11.2f,
          12.0f, 12.0f }
    },
    {
        "fulltreble", 10, 3.0f,
        { -9.6f, -9.6f, -9.6f, -4.0f, 2.4f, 11.2f, 16.0f, 16.0f, 16.0f, 16.8f }
    },
    {
        "headphones", 10, 4.0f,
        { 4.8f, 11.2f, 5.6f, -3.2f, -2.4f, 1.6f, 4.8f, 9.6f, 12.8f, 14.4f }
    },
    {
        "largehall", 10, 5.0f,
        { 10.4f, 10.4f, 5.6f, 5.6f, -1.11022e-15f, -4.8f, -4.8f, -4.8f,
          -1.11022e-15f, -1.11022e-15f }
    },
    {
        "live", 10, 7.0f,
        { -4.8f, -1.11022e-15f, 4.0f, 5.6f, 5.6f, 5.6f, 4.0f, 2.4f,
          2.4f, 2.4f }
    },
    {
        "party", 10, 6.0f,
        { 7.2f, 7.2f, -1.11022e-15f, -1.11022e-15f, -1.11022e-15f,
          -1.11022e-15f, -1.11022e-15f, -1.11022e-15f, 7.2f, 7.2f }
    },
    {
        "pop", 10, 6.0f,
        { -1.6f, 4.8f, 7.2f, 8.0f, 5.6f, -1.11022e-15f, -2.4f, -2.4f,
          -1.6f, -1.6f }
    },
    {
        "reggae", 10, 8.0f,
        { -1.11022e-15f, -1.11022e-15f, -1.11022e-15f, -5.6f, -1.11022e-15f,
          6.4f, 6.4f, -1.11022e-15f, -1.11022e-15f, -1.11022e-15f }
    },
    {
        "rock", 10, 5.0f,
        { 8.0f, 4.8f, -5.6f, -8.0f, -3.2f, 4.0f, 8.8f, 11.2f, 11.2f, 11.2f }
    },
    {
        "ska", 10, 6.0f,
        { -2.4f, -4.8f, -4.0f, -1.11022e-15f, 4.0f, 5.6f, 8.8f, 9.6f,
          11.2f, 9.6f }
    },
    {
        "soft", 10, 5.0f,
        { 4.8f, 1.6f, -1.11022e-15f, -2.4f, -1.11022e-15f, 4.0f, 8.0f, 9.6f,
          11.2f, 12.0f }
    },
    {
        "softrock", 10, 7.0f,
        { 4.0f, 4.0f, 2.4f, -1.11022e-15f, -4.0f, -5.6f, -3.2f, -1.11022e-15f,
          2.4f, 8.8f }
    },
    {
        "techno", 10, 5.0f,
        { 8.0f, 5.6f, -1.11022e-15f, -5.6f, -4.8f, -1.11022e-15f, 8.0f, 9.6f,
          9.6f, 8.8f }
    },
};
