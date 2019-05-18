//============================================================================
// Copyright (C) 2017 Brett R. Jones
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
// http://www.gotvptop.net
//============================================================================
#include "config_corelib.h"

#include "ObjectCommon.h"

//============================================================================
ObjectCommon::ObjectCommon()
: m_ObjectName( "UNKNOWN" )
{
}

//============================================================================
ObjectCommon::ObjectCommon( std::string nameText )
: m_ObjectName( nameText )
{
}

//============================================================================
void ObjectCommon::setObjName( std::string nameText )
{
	m_ObjectName = nameText;
}

