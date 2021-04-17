#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "GuiInterface/OsInterface/OsInterface.h"
#ifdef TARGET_OS_ANDROID

class IAndroid : public OsInterface
{
public:
    IAndroid( IGoTv& gotv );
    virtual ~IAndroid( ) = default;

    //=== stages of create ===//
    bool                        doPreStartup() override;
    bool                        doStartup() override;

    //=== stages of run ===//
    //bool                        initRun( const CAppParamParser &params ) override;
    //bool                        doRun() override;

    //=== stages of destroy ===//
    void                        doPreShutdown() override;
    void                        doShutdown() override;

    //=== utilities ===//
    //bool                        initDirectories( ) override;
};

#endif // TARGET_OS_ANDROID
