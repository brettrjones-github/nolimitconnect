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

#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include <gotvptop/gotvptop.h>

#include "gotvptop_config.h"
#include "core/Enums.h"
#include "core/Error.h"
#include "core/Instance.h"
#include "core/ModuleDescription.h"


void logCallback(void *data,
                 int level,
                 const libgotvptop_log_t *ctx,
                 const char *fmt,
                 va_list args)
{
    Q_UNUSED(ctx)

    GoTvPtoPInstance *instance = static_cast<GoTvPtoPInstance *>(data);
    if (instance->logLevel() > level) {
        return;
    }

    char *result;
    if (vasprintf(&result, fmt, args) < 0) {
        return; // LCOV_EXCL_LINE
    }

    QString message(result);
    free(result);

    message.prepend("libgotvptop: ");

    switch (level) {
    case GoTvPtoP::ErrorLevel:
        qCritical(message.toUtf8().data(), NULL);
        break;
    case GoTvPtoP::WarningLevel:
        qWarning(message.toUtf8().data(), NULL);
        break;
    case GoTvPtoP::NoticeLevel:
    case GoTvPtoP::DebugLevel:
    default:
        qDebug(message.toUtf8().data(), NULL);
        break;
    }
}

GoTvPtoPInstance::GoTvPtoPInstance(const QStringList &args,
                         QObject *parent)
    : QObject(parent),
      _gotvptopInstance(0),
      _status(false),
      _logLevel(GoTvPtoP::ErrorLevel)
{
// Convert arguments to required format
#if defined(Q_OS_WIN32) // Will be removed on Windows if confirmed working
    char **argv = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); ++i)
        argv[i] = (char *)qstrdup(args.at(i).toUtf8().data());
#else
    char *argv[args.count()];
    for (int i = 0; i < args.count(); ++i)
        argv[i] = (char *)qstrdup(args.at(i).toUtf8().data());
#endif

    // Create new libgotvptop instance
    _gotvptopInstance = libgotvptop_new(args.count(), argv);

    qRegisterMetaType<GoTvPtoP::Meta>("GoTvPtoP::Meta");
    qRegisterMetaType<GoTvPtoP::State>("GoTvPtoP::State");

    GoTvPtoPError::showErrmsg();

    // Check if instance is running
    if (_gotvptopInstance) {
        libgotvptop_log_set(_gotvptopInstance, logCallback, this);

        _status = true;
        qDebug() << "GOTV-Qt" << libVersion() << "initialised";
        qDebug() << "Using libgotvptop version:" << version();
    } else {
        qCritical() << "GOTV-Qt Error: libgotvptop failed to load!";
    }
}

GoTvPtoPInstance::~GoTvPtoPInstance()
{
    if (_status && _gotvptopInstance) {
        libgotvptop_release(_gotvptopInstance);
    }
}

libgotvptop_instance_t *GoTvPtoPInstance::core()
{
    return _gotvptopInstance;
}

bool GoTvPtoPInstance::status() const
{
    return _status;
}

GoTvPtoP::LogLevel GoTvPtoPInstance::logLevel() const
{
    return _logLevel;
}

void GoTvPtoPInstance::setLogLevel(GoTvPtoP::LogLevel level)
{
    _logLevel = level;
}

QString GoTvPtoPInstance::libVersion()
{
    QString version;
#if defined(LIBGOTVQT_VERSION)
    version.append(QString(LIBGOTVQT_VERSION));
#else
    version.append(QString("Unknown"));
#endif //LIBGOTVQT_VERSION

#if defined(LIBGOTVQT_VERSION_VCS)
    if (QString(LIBGOTVQT_VERSION_VCS) != "0" && QString(LIBGOTVQT_VERSION_VCS) != "") {
        version.append("-" + QString(LIBGOTVQT_VERSION_VCS));
    }
#endif //LIBGOTVQT_VERSION

    return version;
}

int GoTvPtoPInstance::libVersionMajor()
{
    int version = -1;
#if defined(LIBGOTVQT_VERSION_MAJOR)
    version = LIBGOTVQT_VERSION_MAJOR;
#endif //LIBGOTVQT_VERSION

    return version;
}

int GoTvPtoPInstance::libVersionMinor()
{
    int version = -1;
#if defined(LIBGOTVQT_VERSION_MINOR)
    version = LIBGOTVQT_VERSION_MINOR;
#endif //LIBGOTVQT_VERSION

    return version;
}

QString GoTvPtoPInstance::changeset()
{
    // Returns libgotvptop changeset
    return QString(libgotvptop_get_changeset());
}

QString GoTvPtoPInstance::compiler()
{
    // Returns libgotvptop compiler version
    return QString(libgotvptop_get_compiler());
}

QString GoTvPtoPInstance::version()
{
    // Returns libgotvptop version
    return QString(libgotvptop_get_version());
}

void GoTvPtoPInstance::setUserAgent(const QString &application,
                               const QString &version)
{
    QString applicationOutput = application + " " + version;
    QString httpOutput = application + "/" + version + " " + "GOTV-Qt" + "/" + libVersion(); // "AppName/1.2.3 GOTV-Qt/1.2.3"
    libgotvptop_set_user_agent(_gotvptopInstance, applicationOutput.toUtf8().data(), httpOutput.toUtf8().data());
}

void GoTvPtoPInstance::setAppId(const QString &id,
                           const QString &version,
                           const QString &icon)
{
    libgotvptop_set_app_id(_gotvptopInstance, id.toUtf8().data(), version.toUtf8().data(), icon.toUtf8().data());
}

QList<GoTvPtoPModuleDescription *> GoTvPtoPInstance::audioFilterList() const
{
    libgotvptop_module_description_t *original = libgotvptop_audio_filter_list_get(_gotvptopInstance);
    if (original == NULL) {
        return QList<GoTvPtoPModuleDescription *>(); // LCOV_EXCL_LINE
    }

    libgotvptop_module_description_t *list = original;
    QList<GoTvPtoPModuleDescription *> audioFilters;
    do {
        GoTvPtoPModuleDescription *module = new GoTvPtoPModuleDescription(GoTvPtoPModuleDescription::AudioFilter, list->psz_name);
        module->setLongName(list->psz_longname);
        module->setShortName(list->psz_shortname);
        module->setHelp(list->psz_help);
        audioFilters << module;

        list = list->p_next;
    } while (list->p_next);

    libgotvptop_module_description_list_release(original);

    return audioFilters;
}

QList<GoTvPtoPModuleDescription *> GoTvPtoPInstance::videoFilterList() const
{
    libgotvptop_module_description_t *original = libgotvptop_video_filter_list_get(_gotvptopInstance);
    if (original == NULL) {
        return QList<GoTvPtoPModuleDescription *>(); // LCOV_EXCL_LINE
    }

    libgotvptop_module_description_t *list = original;
    QList<GoTvPtoPModuleDescription *> videoFilters;
    do {
        GoTvPtoPModuleDescription *module = new GoTvPtoPModuleDescription(GoTvPtoPModuleDescription::VideoFilter, list->psz_name);
        module->setLongName(list->psz_longname);
        module->setShortName(list->psz_shortname);
        module->setHelp(list->psz_help);
        videoFilters << module;

        list = list->p_next;
    } while (list->p_next);

    libgotvptop_module_description_list_release(original);

    return videoFilters;
}
