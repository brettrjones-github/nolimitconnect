#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

class PktAssetGetReq : public PktBaseGetReq
{
public:
    PktAssetGetReq();
};

class PktAssetGetReply : public PktBaseGetReply
{
public:
    PktAssetGetReply();
};

class PktAssetSendReq : public PktBaseSendReq
{
public:
	PktAssetSendReq();
};

class PktAssetSendReply : public PktBaseSendReply
{
public:
	PktAssetSendReply();
};

//============================================================================
// Asset chunk packets
//============================================================================
class PktAssetChunkReq : public PktBaseChunkReq
{
public:
	PktAssetChunkReq();
};

class PktAssetChunkReply : public PktBaseChunkReply
{
public:
	PktAssetChunkReply();
};

//============================================================================
// PktAssetGetComplete
//============================================================================
class PktAssetGetCompleteReq : public PktBaseGetCompleteReq
{
public:
    PktAssetGetCompleteReq();
};

class PktAssetGetCompleteReply : public PktBaseGetCompleteReply
{
public:
    PktAssetGetCompleteReply();
};

//============================================================================
// PktAssetSendComplete
//============================================================================
class PktAssetSendCompleteReq : public PktBaseSendCompleteReq
{
public:
	PktAssetSendCompleteReq();
};

class PktAssetSendCompleteReply : public PktBaseSendCompleteReply
{
public:
	PktAssetSendCompleteReply();
};

//============================================================================
// PktAssetXferErr
//============================================================================
class PktAssetXferErr : public PktBaseXferErr
{
public:
	PktAssetXferErr();
};

#pragma pack(pop) 

