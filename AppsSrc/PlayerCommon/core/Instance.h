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

#ifndef GOTVQT_GOTVINSTANCE_H_
#define GOTVQT_GOTVINSTANCE_H_

#include <QtCore/QObject>

#include "Enums.h"
#include "SharedExportCore.h"

class GoTvPtoPModuleDescription;

struct libgotvptop_instance_t;

/*!
    \class GoTvPtoPInstance Instance.h core/Instance.h
    \ingroup GOTVQtCore
    \brief Main instance

    A basic Instance manager for GOTV-Qt library.
    It provides main instance controls.
*/
class GOTVQT_CORE_EXPORT GoTvPtoPInstance : public QObject
{
    Q_OBJECT
public:
    /*!
        \brief GoTvPtoPInstance constructor.

        This is mandatory for using GOTV-Qt and all its other classes.

        \param args libgotvptop arguments (QStringList)
        \param parent Instance's parent object (QObject *)
    */
    explicit GoTvPtoPInstance(const QStringList &args,
                         QObject *parent = NULL);

    /*!
        GoTvPtoPInstance destructor
    */
    ~GoTvPtoPInstance();

    /*!
        \brief Returns libgotvptop instance object.
        \return libgotvptop instance (libgotvptop_instance_t *)
    */
    libgotvptop_instance_t *core();

    /*!
        \brief Returns libgotvptop initialisation status.
        \return libgotvptop status (bool)
    */
    bool status() const;

    /*!
        \brief Returns current log level (default GoTvPtoP::ErrorLevel)
        \return log level
        \since GOTV-Qt 1.1
    */
    GoTvPtoP::LogLevel logLevel() const;

    /*!
        \brief Set current log level
        \param level desired log level
        \see GoTvPtoP::LogLevel
        \since GOTV-Qt 1.1
    */
    void setLogLevel(GoTvPtoP::LogLevel level);

    /*!
        \brief GOTV-Qt version info
        \return a string containing the GOTV-Qt version (QString)
    */
    static QString libVersion();

    /*!
        \brief GOTV-Qt major version
        \return GOTV-Qt major version (int)
    */
    static int libVersionMajor();

    /*!
        \brief GOTV-Qt minor version
        \return GOTV-Qt minor version (int)
    */
    static int libVersionMinor();

    /*!
        \brief libgotvptop version info
        \return a string containing the libgotvptop version (QString)
    */
    static QString version();

    /*!
        \brief libgotvptop compiler info
        \return a string containing the compiler version (QString)
    */
    static QString compiler();

    /*!
        \brief libgotvptop changeset info
        \return a string containing the changeset (QString)
    */
    static QString changeset();

    /*!
        \brief Sets the application name.

        libgotvptop passes this as the user agent string when a protocol requires it.

        \param application Application name (QString)
        \param version Application version (QString)

        \see setAppId
    */
    void setUserAgent(const QString &application,
                      const QString &version);

    /*!
        \brief Sets the application some meta-information.

        \param id Java-style application identifier, e.g. "com.acme.foobar"
        \param version application version numbers, e.g. "1.2.3"
        \param icon application icon name, e.g. "foobar"

        \see setUserAgent
        \since GOTV-Qt 1.1
    */
    void setAppId(const QString &id,
                  const QString &version,
                  const QString &icon);

    /*!
        \brief List audio filter modules
        \return audio filter module description list
    */
    QList<GoTvPtoPModuleDescription *> audioFilterList() const;

    /*!
        \brief List video filter modules
        \return video filter module description list
    */
    QList<GoTvPtoPModuleDescription *> videoFilterList() const;

private:
    libgotvptop_instance_t *_gotvptopInstance;
    bool _status;
    GoTvPtoP::LogLevel _logLevel;
};

#endif // GOTVQT_GOTVINSTANCE_H_
