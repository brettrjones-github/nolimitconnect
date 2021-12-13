
#include "AppSettings.h"

#include <QApplication>

namespace
{
    const QString g_AppName( QObject::tr( "NetClient" ) );
    const QString g_DomainName( "nolimitconnect.com" );
}

//============================================================================
AppSettings::AppSettings()
    : QSettings( g_AppName, g_DomainName )
    , m_AppName( g_AppName )
    , m_AppDomain( g_DomainName )
{
};
