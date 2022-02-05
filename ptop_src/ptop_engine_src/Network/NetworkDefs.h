#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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

#define UDP_ANNOUNCE_BROADCAST_INTERVAL			4

#define DIRECT_CONNECT_TIMEOUT					7000
#define LAN_CONNECT_TIMEOUT						5000
#define MY_PROXY_CONNECT_TIMEOUT				7000
#define TO_PROXY_CONNECT_TIMEOUT				7000
#define THROUGH_PROXY_RESPONSE_TIMEOUT			12000
#define TRY_CONNECT_TO_PROXY_SECONDS			10 // try to connect to proxy interval

#define NETSERVICE_RX_URL_HDR_TIMEOUT			30000
#define NETSERVICE_RX_DATA_TIMEOUT				20000
#define IS_PORT_OPEN_RX_HDR_TIMEOUT				30000
#define IS_PORT_OPEN_RX_DATA_TIMEOUT			30000
#define QUERY_HOST_ID_RX_HDR_TIMEOUT			5000
#define QUERY_HOST_ID_RX_DATA_TIMEOUT			5000
#define PING_TEST_RX_HDR_TIMEOUT			    5000
#define PING_TEST_RX_DATA_TIMEOUT			    5000

#define PORT_TEST_CONNECT_TO_CLIENT1_TIMEOUT	3000
#define PORT_TEST_CONNECT_TO_CLIENT2_TIMEOUT	12000
#define PONG_RX_TIMEOUT							12000

