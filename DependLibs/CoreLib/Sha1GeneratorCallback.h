#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

enum ESha1GenResult
{
	eSha1GenResultNoError = 0,
	eSha1GenResultDuplicateRequest,
	eSha1GenResultInvalidParam,
	eSha1GenResultGenerateSha1Failed,

	MaxESha1GenResult
};

class Sha1Info;
class VxGUID;

class Sha1GeneratorCallback
{
public:
	virtual void				callbackSha1GenerateResult( ESha1GenResult sha1GenResult, VxGUID& fileId, Sha1Info& sha1Info ) = 0;
};
