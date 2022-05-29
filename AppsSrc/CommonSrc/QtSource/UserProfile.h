#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include <QString>

class UserProfile
{
public:
	UserProfile()
	{
		m_strGreeting = "Lets Communicate!";
		m_strAboutMe = "I'm Wonderful!";
		m_strUrl3 = "http://www.nolimitconnect.org";
		m_strPicturePath = "";
	}

    //=== vars ===//
    QString			m_strGreeting;
    QString			m_strAboutMe;
    QString			m_strPicturePath;
    QString			m_strUrl1;
    QString			m_strUrl2;
    QString			m_strUrl3;
    QString			m_strDonation;
};
