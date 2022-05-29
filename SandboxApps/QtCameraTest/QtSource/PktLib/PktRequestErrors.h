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
// http://www.nolimitconnect.org
//============================================================================

//http statuses
#define PKT_REQ_STATUS_OK				200 //"200 OK"
#define PKT_REQ_STATUS_CREATED			201	//"201 Created"
#define PKT_REQ_STATUS_ACCEPTED			202	//"202 Accepted"
#define PKT_REQ_ERR_NO_CONTENT			204	//"204 No Content"
#define PKT_REQ_ERR_MOVED_PERM			301	//"301 Moved Permanently"
#define PKT_REQ_ERR_MOVED_TEMP			302	//"301 Moved Temporarily"
#define PKT_REQ_ERR_NOT_MODIFIED		304 //"304 Not Modified"
#define PKT_REQ_ERR_BAD_REQUEST			400 //"400 Bad Request"
#define PKT_REQ_ERR_UNAUTHORIZED		401 //"401 Unauthorized"
#define PKT_REQ_ERR_FORBIDDEN			403 //"403 Forbidden"
#define PKT_REQ_ERR_NOT_FOUND			404 //"404 Not Found"
#define PKT_REQ_ERR_INTERNAL_SERVER_ERR 500	//"500 Internal Server Error"
#define PKT_REQ_ERR_NOT_IMPLEMENTED		501 //"501 Not Implemented"
#define PKT_REQ_ERR_BAD_GATEWAY			502 //"502 Bad Gateway"
#define PKT_REQ_ERR_SERVICE_UNAVAIL		503 //"503 Service Unavailable"
//custom statuses
#define PKT_REQ_ERR_BANDWITH_LIMIT		600 //"600 Refused because of Bandwidth limit
#define PKT_REQ_ERR_CONNECT_LIMIT		601	//"601 Refused because of Connection limit
#define PKT_REQ_ERR_SERVICE_DISABLED	602	//"602 Refused because Service was disabled
#define PKT_REQ_ERR_ALL_THREADS_BUSY	603	//"603 Refused because all threads busy
