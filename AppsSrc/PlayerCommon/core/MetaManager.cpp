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

NlcPtoPMetaManager::NlcPtoPMetaManager(NlcPtoPMedia *media)
    : _media(media)
{
    libgotvptop_media_parse(media->core());
}

NlcPtoPMetaManager::~NlcPtoPMetaManager() {}

QString NlcPtoPMetaManager::title() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Title));
    return meta;
}

void NlcPtoPMetaManager::setTitle(const QString &title)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Title, title.toUtf8().data());
}

QString NlcPtoPMetaManager::artist() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Artist));
    return meta;
}

void NlcPtoPMetaManager::setArtist(const QString &artist)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Artist, artist.toUtf8().data());
}

QString NlcPtoPMetaManager::genre() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Genre));
    return meta;
}

void NlcPtoPMetaManager::setGenre(const QString &genre)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Genre, genre.toUtf8().data());
}

QString NlcPtoPMetaManager::copyright() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Copyright));
    return meta;
}

void NlcPtoPMetaManager::setCopyright(const QString &copyright)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Copyright, copyright.toUtf8().data());
}

QString NlcPtoPMetaManager::album() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Album));
    return meta;
}

void NlcPtoPMetaManager::setAlbum(const QString &album)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Album, album.toUtf8().data());
}

int NlcPtoPMetaManager::number() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_TrackNumber));
    return meta.toInt();
}

void NlcPtoPMetaManager::setNumber(int number)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_TrackNumber, QString().number(number).toUtf8().data());
}

QString NlcPtoPMetaManager::description() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Description));
    return meta;
}

void NlcPtoPMetaManager::setDescription(const QString &description)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Description, description.toUtf8().data());
}

QString NlcPtoPMetaManager::rating() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Rating));
    return meta;
}

int NlcPtoPMetaManager::year() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Date));
    return meta.toInt();
}

void NlcPtoPMetaManager::setYear(int year)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Date, QString().number(year).toUtf8().data());
}

QString NlcPtoPMetaManager::setting() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Setting));
    return meta;
}

QString NlcPtoPMetaManager::url() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_URL));
    return meta;
}

QString NlcPtoPMetaManager::language() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Language));
    return meta;
}

void NlcPtoPMetaManager::setLanguage(const QString &language)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Language, language.toUtf8().data());
}

QString NlcPtoPMetaManager::publisher() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_Publisher));
    return meta;
}

void NlcPtoPMetaManager::setPublisher(const QString &publisher)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_Publisher, publisher.toUtf8().data());
}

QString NlcPtoPMetaManager::encoder() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_EncodedBy));
    return meta;
}

void NlcPtoPMetaManager::setEncoder(const QString &encoder)
{
    libgotvptop_media_set_meta(_media->core(), libgotvptop_meta_EncodedBy, encoder.toUtf8().data());
}

QString NlcPtoPMetaManager::artwork() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_ArtworkURL));
    return meta;
}

QString NlcPtoPMetaManager::id() const
{
    QString meta(libgotvptop_media_get_meta(_media->core(), libgotvptop_meta_TrackID));
    return meta;
}

bool NlcPtoPMetaManager::saveMeta() const
{
    return libgotvptop_media_save_meta(_media->core());
}
