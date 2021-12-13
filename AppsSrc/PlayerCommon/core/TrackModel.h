/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#ifndef GOTV_TRACK_MODEL_H
#define GOTV_TRACK_MODEL_H

#include <QAbstractListModel>

#include "SharedExportCore.h"

/*!
    \class GoTvPtoPTrackModel TrackModel.h core/TrackModel.h
    \ingroup GOTVQtCore
    \brief Track model

    A model for displaying audio, video and other tracks

    \since GOTV-Qt 1.1
*/
class GOTVQT_CORE_EXPORT GoTvPtoPTrackModel : public QAbstractListModel // LCOV_EXCL_LINE
{
    Q_OBJECT
    /*!
        \brief Current tracks count
        \see count
        \see countChanged
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    /*!
        \enum Roles
        \brief Model data roles
    */
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole
    };

    /*!
        \brief GoTvPtoPTrackModel constructor.
        \param parent parent object
    */
    explicit GoTvPtoPTrackModel(QObject *parent = 0);

    /*!
        \brief GoTvPtoPTrackModel constructor.
        \param tracks tracks map
        \param parent parent object
    */
    explicit GoTvPtoPTrackModel(const QMap<int, QString> &tracks,
                           QObject *parent = 0);

    /*!
        \brief GoTvPtoPTrackModel constructor.
        \param other another GoTvPtoPTrackModel instance
    */
    explicit GoTvPtoPTrackModel(const GoTvPtoPTrackModel &other);

    /*!
        \brief Get row count
        \param parent parent model index
        \return count
    */
    int rowCount(const QModelIndex &parent) const;

    /*!
        \brief Model role names
        \return role names hash
    */
    QHash<int, QByteArray> roleNames() const;

    /*!
        \brief Read data from model
        \param index model index
        \param role required role
    */
    QVariant data(const QModelIndex &index, int role) const;

    /*!
        \brief Read data from model
        \param row model row
        \param role required role
    */
    QVariant data(const int row, int role) const;

    /*!
        \brief Clear the model
    */
    void clear();

    /*!
        \brief Load tracks into model
        \param data tracks map id-title
    */
    void load(const QMap<int, QString> &data);

    /*!
        \brief Insert track into model
        \param id track id
        \param title track title
    */
    void insert(const int id, const QString &title);

    /*!
        \brief Get tracks count
        \return count
    */
    int count() const;

signals:
    /*!
        \brief Count changed signal
    */
    void countChanged();

private:
    QMap<int, QString> _tracks;
};

Q_DECLARE_METATYPE(GoTvPtoPTrackModel)

#endif //GOTV_TRACK_MODEL_H
