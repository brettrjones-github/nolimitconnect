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
// http://www.nolimitconnect.com
//============================================================================


#include "PktsBaseXfer.h"

#pragma pack(push) 
#pragma pack(1)

class PktThumbGetReq : public PktBaseGetReq
{
public:
    PktThumbGetReq();
};

class PktThumbGetReply : public PktBaseGetReply
{
public:
    PktThumbGetReply();
};

class  PktThumbSendReq : public PktBaseSendReq
{
public:
     PktThumbSendReq();
};

class  PktThumbSendReply : public PktBaseSendReply
{
public:
     PktThumbSendReply();
};

//============================================================================
// Asset chunk packets
//============================================================================
class  PktThumbChunkReq : public PktBaseChunkReq
{
public:
     PktThumbChunkReq();
};

class  PktThumbChunkReply : public PktBaseChunkReply
{
public:
     PktThumbChunkReply();
};

//============================================================================
// PktAssetGetComplete
//============================================================================
class PktThumbGetCompleteReq : public PktBaseGetCompleteReq
{
public:
    PktThumbGetCompleteReq();
};

class PktThumbGetCompleteReply : public PktBaseGetCompleteReply
{
public:
    PktThumbGetCompleteReply();
};

//============================================================================
//  PktThumbSendComplete
//============================================================================
class  PktThumbSendCompleteReq : public PktBaseSendCompleteReq
{
public:
     PktThumbSendCompleteReq();
};

class  PktThumbSendCompleteReply : public PktBaseSendCompleteReply
{
public:
     PktThumbSendCompleteReply();
};

//============================================================================
//  PktThumbXferErr
//============================================================================
class  PktThumbXferErr : public PktBaseXferErr
{
public:
    PktThumbXferErr();
};

//============================================================================
// File Find packets
//============================================================================
class PktThumbFindReq : public PktBaseFindReq
{
public:
    PktThumbFindReq();
};

//============================================================================
//  PktThumbXferListRequest
//============================================================================
class  PktThumbListReq : public PktBaseListReq
{
public:
    PktThumbListReq();
};

class  PktThumbListReply : public PktBaseListReply
{
public:
    PktThumbListReply();
};

#pragma pack(pop)

