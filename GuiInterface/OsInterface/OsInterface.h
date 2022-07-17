#pragma once

#include "GuiInterface/IDefs.h"
#include "GuiInterface/INlcEvents.h"
#if ENABLE_KODI
#include "AppParamParser.h"
#endif // ENABLE_KODI

class INlc;
class CAppParamParser;

class OsInterface
{
public:
    OsInterface( INlc& gotv );
    virtual ~OsInterface() = default;

    INlc&                      getINlc() { return m_INlc; }

    // exit of application error code
    virtual void                setRunResultCode( int exitCode )    { m_RunResultCode = exitCode; }
    virtual int                 getRunResultCode()                  { return m_RunResultCode; }

    //=== stages of create ===//
    virtual bool                doPreStartup( ) = 0;
    virtual bool                doStartup( ) = 0;

    //=== stages of run ===//
    virtual bool                initRun( const CAppParamParser& cmdLineParams );
    virtual bool                doRun( EAppModule appModule );

    //=== stages of destroy ===//
    virtual void                doPreShutdown() = 0;
    virtual void                doShutdown() = 0;

    //=== utilities ===//
	virtual bool               initUserPaths(); // basically exe and user data paths
    virtual bool               initDirectories();


protected:
    INlc&                      m_INlc;
    const CAppParamParser*      m_CmdLineParams = nullptr;
    int                         m_RunResultCode = 0;
};
