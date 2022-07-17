/*****************************************************************************
 * gotvptop_meta.h: Stream meta-data
 *****************************************************************************
 * Copyright (C) 2004 GOTV authors and VideoLAN
 * $Id: 954a8342b726adb7d91496a740ebd6bc51a8f037 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GOTV_META_H
#define GOTV_META_H 1

/**
 * \file
 * This file defines functions and structures for stream meta-data in gotvptop
 *
 */

typedef enum gotvptop_meta_type_t
{
    gotvptop_meta_Title,
    gotvptop_meta_Artist,
    gotvptop_meta_Genre,
    gotvptop_meta_Copyright,
    gotvptop_meta_Album,
    gotvptop_meta_TrackNumber,
    gotvptop_meta_Description,
    gotvptop_meta_Rating,
    gotvptop_meta_Date,
    gotvptop_meta_Setting,
    gotvptop_meta_URL,
    gotvptop_meta_Language,
    gotvptop_meta_NowPlaying,
    gotvptop_meta_ESNowPlaying,
    gotvptop_meta_Publisher,
    gotvptop_meta_EncodedBy,
    gotvptop_meta_ArtworkURL,
    gotvptop_meta_TrackID,
    gotvptop_meta_TrackTotal,
    gotvptop_meta_Director,
    gotvptop_meta_Season,
    gotvptop_meta_Episode,
    gotvptop_meta_ShowName,
    gotvptop_meta_Actors,
    gotvptop_meta_AlbumArtist,
    gotvptop_meta_DiscNumber,
    gotvptop_meta_DiscTotal
} gotvptop_meta_type_t;

#define GOTV_META_TYPE_COUNT 27

#define ITEM_PREPARSED       1
#define ITEM_ART_FETCHED     2
#define ITEM_ART_NOTFOUND    4

/**
 * Basic function to deal with meta
 */
struct gotvptop_meta_t;

NLC_API gotvptop_meta_t * gotvptop_meta_New( void ) GOTV_USED;
NLC_API void gotvptop_meta_Delete( gotvptop_meta_t *m );
NLC_API void gotvptop_meta_Set( gotvptop_meta_t *p_meta, gotvptop_meta_type_t meta_type, const char *psz_val );
NLC_API const char * gotvptop_meta_Get( const gotvptop_meta_t *p_meta, gotvptop_meta_type_t meta_type );

NLC_API void gotvptop_meta_AddExtra( gotvptop_meta_t *m, const char *psz_name, const char *psz_value );
NLC_API const char * gotvptop_meta_GetExtra( const gotvptop_meta_t *m, const char *psz_name );
NLC_API unsigned gotvptop_meta_GetExtraCount( const gotvptop_meta_t *m );

/**
 * Allocate a copy of all extra meta names and a table with it.
 * Be sure to free both the returned pointers and its name.
 */
NLC_API char ** gotvptop_meta_CopyExtraNames( const gotvptop_meta_t *m ) GOTV_USED;

NLC_API void gotvptop_meta_Merge( gotvptop_meta_t *dst, const gotvptop_meta_t *src );

NLC_API int gotvptop_meta_GetStatus( gotvptop_meta_t *m );
NLC_API void gotvptop_meta_SetStatus( gotvptop_meta_t *m, int status );

/**
 * Returns a localizes string describing the meta
 */
NLC_API const char * gotvptop_meta_TypeToLocalizedString( gotvptop_meta_type_t meta_type );

typedef struct meta_export_t
{
    struct gotvptop_common_members obj;
    input_item_t *p_item;
    const char *psz_file;
} meta_export_t;

NLC_API int input_item_WriteMeta(gotvptop_object_t *, input_item_t *);

/* Setters for meta.
 * Warning: Make sure to use the input_item meta setters (defined in gotvptop_input_item.h)
 * instead of those one. */
#define gotvptop_meta_SetTitle( meta, b )       gotvptop_meta_Set( meta, gotvptop_meta_Title, b )
#define gotvptop_meta_SetArtist( meta, b )      gotvptop_meta_Set( meta, gotvptop_meta_Artist, b )
#define gotvptop_meta_SetGenre( meta, b )       gotvptop_meta_Set( meta, gotvptop_meta_Genre, b )
#define gotvptop_meta_SetCopyright( meta, b )   gotvptop_meta_Set( meta, gotvptop_meta_Copyright, b )
#define gotvptop_meta_SetAlbum( meta, b )       gotvptop_meta_Set( meta, gotvptop_meta_Album, b )
#define gotvptop_meta_SetTrackNum( meta, b )    gotvptop_meta_Set( meta, gotvptop_meta_TrackNumber, b )
#define gotvptop_meta_SetDescription( meta, b ) gotvptop_meta_Set( meta, gotvptop_meta_Description, b )
#define gotvptop_meta_SetRating( meta, b )      gotvptop_meta_Set( meta, gotvptop_meta_Rating, b )
#define gotvptop_meta_SetDate( meta, b )        gotvptop_meta_Set( meta, gotvptop_meta_Date, b )
#define gotvptop_meta_SetSetting( meta, b )     gotvptop_meta_Set( meta, gotvptop_meta_Setting, b )
#define gotvptop_meta_SetURL( meta, b )         gotvptop_meta_Set( meta, gotvptop_meta_URL, b )
#define gotvptop_meta_SetLanguage( meta, b )    gotvptop_meta_Set( meta, gotvptop_meta_Language, b )
#define gotvptop_meta_SetNowPlaying( meta, b )  gotvptop_meta_Set( meta, gotvptop_meta_NowPlaying, b )
#define gotvptop_meta_SetPublisher( meta, b )   gotvptop_meta_Set( meta, gotvptop_meta_Publisher, b )
#define gotvptop_meta_SetEncodedBy( meta, b )   gotvptop_meta_Set( meta, gotvptop_meta_EncodedBy, b )
#define gotvptop_meta_SetArtURL( meta, b )      gotvptop_meta_Set( meta, gotvptop_meta_ArtworkURL, b )
#define gotvptop_meta_SetTrackID( meta, b )     gotvptop_meta_Set( meta, gotvptop_meta_TrackID, b )
#define gotvptop_meta_SetTrackTotal( meta, b )  gotvptop_meta_Set( meta, gotvptop_meta_TrackTotal, b )
#define gotvptop_meta_SetDirector( meta, b )    gotvptop_meta_Set( meta, gotvptop_meta_Director, b )
#define gotvptop_meta_SetSeason( meta, b )      gotvptop_meta_Set( meta, gotvptop_meta_Season, b )
#define gotvptop_meta_SetEpisode( meta, b )     gotvptop_meta_Set( meta, gotvptop_meta_Episode, b )
#define gotvptop_meta_SetShowName( meta, b )    gotvptop_meta_Set( meta, gotvptop_meta_ShowName, b )
#define gotvptop_meta_SetActors( meta, b )      gotvptop_meta_Set( meta, gotvptop_meta_Actors, b )
#define gotvptop_meta_SetAlbumArtist( meta, b ) gotvptop_meta_Set( meta, gotvptop_meta_AlbumArtist, b )
#define gotvptop_meta_SetDiscNumber( meta, b )  gotvptop_meta_Set( meta, gotvptop_meta_DiscNumber, b )

#define GOTV_META_TITLE              gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Title )
#define GOTV_META_ARTIST             gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Artist )
#define GOTV_META_GENRE              gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Genre )
#define GOTV_META_COPYRIGHT          gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Copyright )
#define GOTV_META_ALBUM              gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Album )
#define GOTV_META_TRACK_NUMBER       gotvptop_meta_TypeToLocalizedString( gotvptop_meta_TrackNumber )
#define GOTV_META_DESCRIPTION        gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Description )
#define GOTV_META_RATING             gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Rating )
#define GOTV_META_DATE               gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Date )
#define GOTV_META_SETTING            gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Setting )
#define GOTV_META_URL                gotvptop_meta_TypeToLocalizedString( gotvptop_meta_URL )
#define GOTV_META_LANGUAGE           gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Language )
#define GOTV_META_NOW_PLAYING        gotvptop_meta_TypeToLocalizedString( gotvptop_meta_NowPlaying )
#define GOTV_META_PUBLISHER          gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Publisher )
#define GOTV_META_ENCODED_BY         gotvptop_meta_TypeToLocalizedString( gotvptop_meta_EncodedBy )
#define GOTV_META_ART_URL            gotvptop_meta_TypeToLocalizedString( gotvptop_meta_ArtworkURL )
#define GOTV_META_TRACKID            gotvptop_meta_TypeToLocalizedString( gotvptop_meta_TrackID )
#define GOTV_META_DIRECTOR           gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Director )
#define GOTV_META_SEASON             gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Season )
#define GOTV_META_EPISODE            gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Episode )
#define GOTV_META_SHOW_NAME          gotvptop_meta_TypeToLocalizedString( gotvptop_meta_ShowName )
#define GOTV_META_ACTORS             gotvptop_meta_TypeToLocalizedString( gotvptop_meta_Actors )
#define GOTV_META_ALBUMARTIST        gotvptop_meta_TypeToLocalizedString( gotvptop_meta_AlbumArtist )
#define GOTV_META_DISCNUMBER         gotvptop_meta_TypeToLocalizedString( gotvptop_meta_DiscNumber )

#define GOTV_META_EXTRA_MB_ALBUMID   "MB_ALBUMID"

#endif
