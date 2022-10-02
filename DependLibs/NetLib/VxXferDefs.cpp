#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "VxXferDefs.h"


namespace
{
    const char* ENUM_BAD_PARM = "ENUM BAD PARAM ";

    const char* EXferDirectionEnumStrings[] =
    {
        "Xfer Direction Unknown ",
        "Xfer Direction Rx ",
        "Xfer Direction Tx ",

        "Max Xfer Direction "
    };

    const char* EXferErrorEnumStrings[] =
    {
        "Xfer Error None ",
        "Xfer Error Disconnected ",
        "Xfer Error Permission ",
        "Xfer Error File Not Found ",
        "Xfer Error Canceled ",
        "Xfer Error Bad Param ",
        "Xfer Error At Src ",
        "Xfer Error Busy ",
        "Xfer Error Already Downloading ",
        "Xfer Error Already Downloaded ",
        "Xfer Error Already Uploading ",
        "Xfer Error File Create Error ",
        "Xfer Error File Open Append Error ",
        "Xfer Error File Open Error ",
        "Xfer Error File Seek Error ",
        "Xfer Error File Read Error ",
        "Xfer Error File Write Error ",
        "Xfer Error File Move Error ",

        "Max Xfer Error "
    };

    const char* EXferStateEnumStrings[] =
    {
        "Xfer State None ",
        "Xfer State Upload Not Started ",
        "Xfer State Waiting Offer Response ",
        "Xfer State In Upload Que ",
        "Xfer State Begin Upload ",
        "Xfer State In Upload Xfer ",
        "Xfer State Complete dUpload ",
        "Xfer State User Canceled Upload ",
        "Xfer State Upload Offer Rejected ",
        "Xfer State Upload Error ",
        "Xfer State Download Not Started ",
        "Xfer State In Download Que ",
        "Xfer State Begin Download ",
        "Xfer State In Download Xfer ",
        "Xfer State Completed Download ",
        "Xfer State User Canceled Download ",
        "Xfer State Download Error ",

        "Max Xfer State "
    };
};

//============================================================================
const char* DescribeXferDirection( EXferDirection xferDir )
{
    if( xferDir < 0 || eMaxXferDirection <= xferDir )
    {
        return ENUM_BAD_PARM;
    }

    return EXferDirectionEnumStrings[ xferDir ];
}

//============================================================================
const char* DescribeXferError( EXferError xferErr )
{
    if( xferErr < 0 || eMaxXferError <= xferErr )
    {
        return ENUM_BAD_PARM;
    }

    return EXferErrorEnumStrings[ xferErr ];
}

//============================================================================
const char* DescribeXferState( EXferState xferState )
{
    if( xferState < 0 || eMaxXferState <= xferState )
    {
        return ENUM_BAD_PARM;
    }

    return EXferStateEnumStrings[ xferState ];
}
