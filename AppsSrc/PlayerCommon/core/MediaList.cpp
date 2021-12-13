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
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaList.h"

GoTvPtoPMediaList::GoTvPtoPMediaList(GoTvPtoPInstance *instance)
    : QObject(instance)
{
    _gotvptopMediaList = libgotvptop_media_list_new(instance->core());
    _gotvptopEvents = libgotvptop_media_list_event_manager(_gotvptopMediaList);

    createCoreConnections();

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMediaList::~GoTvPtoPMediaList()
{
    foreach (GoTvPtoPMedia *m, _list)
        delete m;

    removeCoreConnections();

    libgotvptop_media_list_release(_gotvptopMediaList);

    GoTvPtoPError::showErrmsg();
}

libgotvptop_media_list_t *GoTvPtoPMediaList::core()
{
    return _gotvptopMediaList;
}

void GoTvPtoPMediaList::createCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListItemAdded
         << libgotvptop_MediaListWillAddItem
         << libgotvptop_MediaListItemDeleted
         << libgotvptop_MediaListWillDeleteItem;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_attach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMediaList::removeCoreConnections()
{
    QList<libgotvptop_event_e> list;
    list << libgotvptop_MediaListItemAdded
         << libgotvptop_MediaListWillAddItem
         << libgotvptop_MediaListItemDeleted
         << libgotvptop_MediaListWillDeleteItem;

    foreach (const libgotvptop_event_e &event, list) {
        libgotvptop_event_detach(_gotvptopEvents, event, libgotvptop_callback, this);
    }
}

void GoTvPtoPMediaList::addMedia(GoTvPtoPMedia *media)
{
    lock();
    libgotvptop_media_list_add_media(_gotvptopMediaList, media->core());
    _list.append(media);
    unlock();

    GoTvPtoPError::showErrmsg();
}

GoTvPtoPMedia *GoTvPtoPMediaList::at(int index)
{
    return _list[index];
}

int GoTvPtoPMediaList::count()
{
    lock();
    int count = libgotvptop_media_list_count(_gotvptopMediaList);
    unlock();

    GoTvPtoPError::showErrmsg();

    return count;
}

int GoTvPtoPMediaList::indexOf(GoTvPtoPMedia *media)
{
    return _list.indexOf(media);
}

int GoTvPtoPMediaList::indexOf(libgotvptop_media_t *media)
{
    int index;
    lock();
    index = libgotvptop_media_list_index_of_item(_gotvptopMediaList, media);
    unlock();

    GoTvPtoPError::showErrmsg();

    return index;
}

void GoTvPtoPMediaList::insertMedia(GoTvPtoPMedia *media,
                               int index)
{
    lock();
    libgotvptop_media_list_insert_media(_gotvptopMediaList, media->core(), index);
    _list.insert(index, media);
    unlock();

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaList::removeMedia(int index)
{
    lock();
    libgotvptop_media_list_remove_index(_gotvptopMediaList, index);
    delete _list[index];
    _list.removeAt(index);
    unlock();

    GoTvPtoPError::showErrmsg();
}

void GoTvPtoPMediaList::lock()
{
    libgotvptop_media_list_lock(_gotvptopMediaList);
}

void GoTvPtoPMediaList::unlock()
{
    libgotvptop_media_list_unlock(_gotvptopMediaList);
}

void GoTvPtoPMediaList::libgotvptop_callback(const libgotvptop_event_t *event,
                                   void *data)
{
    GoTvPtoPMediaList *core = static_cast<GoTvPtoPMediaList *>(data);

    switch (event->type) {
    case libgotvptop_MediaListItemAdded:
        emit core->itemAdded(event->u.media_list_item_added.item, event->u.media_list_item_added.index);
        break;
    case libgotvptop_MediaListWillAddItem:
        emit core->willAddItem(event->u.media_list_will_add_item.item, event->u.media_list_will_add_item.index);
        break;
    case libgotvptop_MediaListItemDeleted:
        emit core->itemDeleted(event->u.media_list_item_deleted.item, event->u.media_list_item_deleted.index);
        break;
    case libgotvptop_MediaListWillDeleteItem:
        emit core->willDeleteItem(event->u.media_list_will_delete_item.item, event->u.media_list_will_delete_item.index);
        break;
    default:
        break; // LCOV_EXCL_LINE
    }
}
