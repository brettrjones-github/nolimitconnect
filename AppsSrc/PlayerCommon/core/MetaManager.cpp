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

#include "core/Media.h"
#include "core/MetaManager.h"

GoTvPtoPMetaManager::GoTvPtoPMetaManager(GoTvPtoPMedia *media)
    : _media(media)
{
    libgotvptop_media_parse(media->core());
}

GoTvPtoPMetaManager::~GoTvPtoPMetaManager() {}

QString GoTvPtoPMetaManager::title() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Title));
    return meta;
}

void GoTvPtoPMetaManager::setTitle(const QString &title)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Title, title.toUtf8().data());
}

QString GoTvPtoPMetaManager::artist() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Artist));
    return meta;
}

void GoTvPtoPMetaManager::setArtist(const QString &artist)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Artist, artist.toUtf8().data());
}

QString GoTvPtoPMetaManager::genre() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Genre));
    return meta;
}

void GoTvPtoPMetaManager::setGenre(const QString &genre)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Genre, genre.toUtf8().data());
}

QString GoTvPtoPMetaManager::copyright() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Copyright));
    return meta;
}

void GoTvPtoPMetaManager::setCopyright(const QString &copyright)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Copyright, copyright.toUtf8().data());
}

QString GoTvPtoPMetaManager::album() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Album));
    return meta;
}

void GoTvPtoPMetaManager::setAlbum(const QString &album)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Album, album.toUtf8().data());
}

int GoTvPtoPMetaManager::number() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_TrackNumber));
    return meta.toInt();
}

void GoTvPtoPMetaManager::setNumber(int number)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_TrackNumber, QString().number(number).toUtf8().data());
}

QString GoTvPtoPMetaManager::description() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Description));
    return meta;
}

void GoTvPtoPMetaManager::setDescription(const QString &description)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Description, description.toUtf8().data());
}

QString GoTvPtoPMetaManager::rating() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Rating));
    return meta;
}

int GoTvPtoPMetaManager::year() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Date));
    return meta.toInt();
}

void GoTvPtoPMetaManager::setYear(int year)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Date, QString().number(year).toUtf8().data());
}

QString GoTvPtoPMetaManager::setting() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Setting));
    return meta;
}

QString GoTvPtoPMetaManager::url() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_URL));
    return meta;
}

QString GoTvPtoPMetaManager::language() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Language));
    return meta;
}

void GoTvPtoPMetaManager::setLanguage(const QString &language)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Language, language.toUtf8().data());
}

QString GoTvPtoPMetaManager::publisher() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Publisher));
    return meta;
}

void GoTvPtoPMetaManager::setPublisher(const QString &publisher)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Publisher, publisher.toUtf8().data());
}

QString GoTvPtoPMetaManager::encoder() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_EncodedBy));
    return meta;
}

void GoTvPtoPMetaManager::setEncoder(const QString &encoder)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_EncodedBy, encoder.toUtf8().data());
}

QString GoTvPtoPMetaManager::artwork() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_ArtworkURL));
    return meta;
}

QString GoTvPtoPMetaManager::id() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_TrackID));
    return meta;
}

bool GoTvPtoPMetaManager::saveMeta() const
{
    return libgotvptop_media_save_meta(_media->core());
}
