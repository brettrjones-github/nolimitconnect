#pragma once

enum EAppModule
{
    eAppModuleInvalid = 0,
    eAppModuleAll,
    eAppModuleKodi,
    eAppModulePtoP,
    eAppModuleTest,

    eMaxAppModule // must be last
};

enum EModuleState
{
	eModuleStateUnknown = 0,
	eModuleStateInitialized,
	eModuleStateDeinitialized,
    eModuleStateInitError,

	eMaxModuleState // must be last
};

const char * describeAppModule( EAppModule appModule );
const char * describeModuleState( EModuleState moduleState );