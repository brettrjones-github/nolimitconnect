/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#ifndef GOTVQT_MODULEDESCRIPTION_H_
#define GOTVQT_MODULEDESCRIPTION_H_

#include <QtCore/QString>

#include "SharedExportCore.h"

/*!
    \class GoTvPtoPModuleDescription ModuleDescription.h core/ModuleDescription.h
    \ingroup GOTVQtCore
    \brief GOTV module description
*/
class GOTVQT_CORE_EXPORT GoTvPtoPModuleDescription
{
public:
    /*!
        \enum Type
        \brief Module type
    */
    enum Type {
        AudioFilter,
        VideoFilter
    };

    /*!
        \brief Basic constructor
        \param type module type
        \param name module name
    */
    explicit GoTvPtoPModuleDescription(Type type,
                                  const QString &name);

    /*!
        \brief GoTvPtoPModuleDescription destructor
    */
    ~GoTvPtoPModuleDescription();

    /*!
        \brief Get module type
        \return module type
    */
    inline Type type() const { return _type; }

    /*!
        \brief Get module name
        \return module name
    */
    inline QString name() const { return _name; }

    /*!
        \brief Get short module readable name
        \return short module human readable name
    */
    inline QString shortName() const { return _shortName; }

    /*!
        \brief Set short module readable name
        \param name short module human readable name
    */
    void setShortName(const QString &name);

    /*!
        \brief Get long module readable name
        \return long module human readable name
    */
    inline QString longName() const { return _longName; }

    /*!
        \brief Set long module readable name
        \param name long module human readable name
    */
    void setLongName(const QString &name);

    /*!
        \brief Get module help
        \return module help
    */
    inline QString help() const { return _help; }

    /*!
        \brief Set module help
        \param help new module help
    */
    void setHelp(const QString &help);

private:
    Type _type;
    QString _name;
    QString _shortName;
    QString _longName;
    QString _help;
};

#endif // GOTVQT_MODULEDESCRIPTION_H_
