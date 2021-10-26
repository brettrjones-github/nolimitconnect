#pragma once

#include "GuiInterface/IDefs.h"
#include "GuiInterface/IGoTvEvents.h"
#if ENABLE_KODI
#include "AppParamParser.h"
#endif // ENABLE_KODI

class IGoTv;
class CAppParamParser;

class OsInterface
{
public:
    OsInterface( IGoTv& gotv );
    virtual ~OsInterface() = default;

    IGoTv&                      getIGoTv() { return m_IGoTv; }

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
    IGoTv&                      m_IGoTv;
    const CAppParamParser*      m_CmdLineParams = nullptr;
    int                         m_RunResultCode = 0;
};
