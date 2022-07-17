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

#ifndef GOTVQT_MEDIALIST_H_
#define GOTVQT_MEDIALIST_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "Enums.h"
#include "SharedExportCore.h"

class NlcPtoPInstance;
class NlcPtoPMedia;

struct libgotvptop_event_t;
struct libgotvptop_event_manager_t;
struct libgotvptop_media_t;
struct libgotvptop_media_list_t;

/*!
    \class NlcPtoPMediaList MediaList.h core/MediaList.h
    \ingroup GOTVQtCore
    \brief Media list item

    NlcPtoPMediaList holds multiple NlcPtoPMedia items to play in sequence.
*/
class GOTVQT_CORE_EXPORT NlcPtoPMediaList : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief NlcPtoPMediaList constructor.

        This constructor creates a new media list instance.

        \param instance main libgotvptop instance
    */
    explicit NlcPtoPMediaList(NlcPtoPInstance *instance);

    /*!
        \brief NlcPtoPMediaList destructor
    */
    ~NlcPtoPMediaList();

    /*!
        \brief libgotvptop media list item
        \return libgotvptop media list item (libgotvptop_media_list_t *)
    */
    libgotvptop_media_list_t *core();

    /*!
        \brief Add media item to the list
        \param media media item
    */
    void addMedia(NlcPtoPMedia *media);

    /*!
        \brief Get media item at selected index
        \param index item position
        \return media item (NlcPtoPMedia)
    */
    NlcPtoPMedia *at(int index);

    /*!
        \brief libgotvptop media list item
        \return items count (int)
    */
    int count();

    /*!
        \brief Index of media item
        \param media media item
        \return media item index (int)
    */
    int indexOf(NlcPtoPMedia *media);

    /*!
        \brief Index of media item (core)
        \param media media item
        \return media item index (int)
    */
    int indexOf(libgotvptop_media_t *media);

    /*!
        \brief Insert media item at the specific position of the list.
        \param media media item
        \param index item position
    */
    void insertMedia(NlcPtoPMedia *media,
                     int index);

    /*!
        \brief Remove media item from the specific position of the list.
        \param index item position
    */
    void removeMedia(int index);

signals:
    /*!
        \brief Signal sent on item added
        \param item item that was added
        \param index index of item
    */
    void itemAdded(libgotvptop_media_t *item,
                   int index);

    /*!
        \brief Signal sent when item will be added
        \param item item that will be added
        \param index index of item
    */
    void willAddItem(libgotvptop_media_t *item,
                     int index);

    /*!
        \brief Signal sent on item deleted
        \param item item that was added
        \param index index of item
    */
    void itemDeleted(libgotvptop_media_t *item,
                     int index);

    /*!
        \brief Signal sent when item will be deleted
        \param item item that will be deleted
        \param index index of item
    */
    void willDeleteItem(libgotvptop_media_t *item,
                        int index);

private:
    void lock();
    void unlock();

    static void libgotvptop_callback(const libgotvptop_event_t *event,
                                void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libgotvptop_media_list_t *_gotvptopMediaList;
    libgotvptop_event_manager_t *_gotvptopEvents;

    QList<NlcPtoPMedia *> _list;
};

#endif // GOTVQT_MEDIALIST_H_
