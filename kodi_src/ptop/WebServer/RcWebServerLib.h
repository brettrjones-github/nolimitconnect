//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

//#define USE_WEB_FILESHARE 1  // define this to enable sharing files through web page
#include <GoTvCore/GoTvP2P/WebServer/RcWebPageSettings.h>
#include <GoTvCore/GoTvP2P/WebServer/RcWebSkt.h>
#include <GoTvCore/GoTvP2P/WebServer/RcWebServer.h>
#ifdef USE_WEB_FILESHARE
	#include <GoTvCore/GoTvP2P/WebServer/RcWebPageBuilder.h>
#endif //USE_WEB_FILESHARE
