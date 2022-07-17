/*****************************************************************************
 * gotvptop_httpd.h: builtin HTTP/RTSP server.
 *****************************************************************************
 * Copyright (C) 2004-2006 GOTV authors and VideoLAN
 * $Id: 29b8229e7b6baa7a8eedf37f6fdb1e97bf6a9ca7 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GOTV_HTTPD_H
#define GOTV_HTTPD_H 1

/**
 * \file
 * HTTP/RTSP server API.
 */

enum
{
    HTTPD_MSG_NONE,

    /* answer */
    HTTPD_MSG_ANSWER,

    /* http request */
    HTTPD_MSG_GET,
    HTTPD_MSG_HEAD,
    HTTPD_MSG_POST,

    /* rtsp request */
    HTTPD_MSG_OPTIONS,
    HTTPD_MSG_DESCRIBE,
    HTTPD_MSG_SETUP,
    HTTPD_MSG_PLAY,
    HTTPD_MSG_PAUSE,
    HTTPD_MSG_GETPARAMETER,
    HTTPD_MSG_TEARDOWN,

    /* just to track the count of MSG */
    HTTPD_MSG_MAX
};

enum
{
    HTTPD_PROTO_NONE,
    HTTPD_PROTO_HTTP,  /* HTTP/1.x */
    HTTPD_PROTO_RTSP,  /* RTSP/1.x */
    HTTPD_PROTO_HTTP0, /* HTTP/0.x */
};

typedef struct httpd_host_t   httpd_host_t;
typedef struct httpd_client_t httpd_client_t;
/* create a new host */
NLC_API httpd_host_t *gotvptop_http_HostNew( gotvptop_object_t * ) GOTV_USED;
NLC_API httpd_host_t *gotvptop_https_HostNew( gotvptop_object_t * ) GOTV_USED;
NLC_API httpd_host_t *gotvptop_rtsp_HostNew( gotvptop_object_t * ) GOTV_USED;
/* delete a host */
NLC_API void httpd_HostDelete( httpd_host_t * );

typedef struct
{
    char * name;
    char * value;
} httpd_header;

typedef struct httpd_message_t
{
    httpd_client_t *cl; /* NULL if not throught a connection e gotvptop internal */

    uint8_t i_type;
    uint8_t i_proto;
    uint8_t i_version;

    /* for an answer */
    int     i_status;

    /* for a query */
    char    *psz_url;
    /* FIXME find a clean way to handle GET(psz_args)
       and POST(body) through the same code */
    uint8_t *psz_args;

    /* options */
    size_t       i_headers;
    httpd_header *p_headers;

    /* body */
    int64_t i_body_offset;
    int     i_body;
    uint8_t *p_body;

} httpd_message_t;

typedef struct httpd_url_t      httpd_url_t;
typedef struct httpd_callback_sys_t httpd_callback_sys_t;
typedef int    (*httpd_callback_t)( httpd_callback_sys_t *, httpd_client_t *, httpd_message_t *answer, const httpd_message_t *query );
/* register a new url */
NLC_API httpd_url_t * httpd_UrlNew( httpd_host_t *, const char *psz_url, const char *psz_user, const char *psz_password ) GOTV_USED;
/* register callback on a url */
NLC_API int httpd_UrlCatch( httpd_url_t *, int i_msg, httpd_callback_t, httpd_callback_sys_t * );
/* delete a url */
NLC_API void httpd_UrlDelete( httpd_url_t * );

NLC_API char* httpd_ClientIP( const httpd_client_t *cl, char *, int * );
NLC_API char* httpd_ServerIP( const httpd_client_t *cl, char *, int * );

/* High level */

typedef struct httpd_file_t     httpd_file_t;
typedef struct httpd_file_sys_t httpd_file_sys_t;
typedef int (*httpd_file_callback_t)( httpd_file_sys_t *, httpd_file_t *, uint8_t *psz_request, uint8_t **pp_data, int *pi_data );
NLC_API httpd_file_t * httpd_FileNew( httpd_host_t *, const char *psz_url, const char *psz_mime, const char *psz_user, const char *psz_password, httpd_file_callback_t pf_fill, httpd_file_sys_t * ) GOTV_USED;
NLC_API httpd_file_sys_t * httpd_FileDelete( httpd_file_t * );


typedef struct httpd_handler_t  httpd_handler_t;
typedef int (*httpd_handler_callback_t)( void *, httpd_handler_t *, char *psz_url, uint8_t *psz_request, int i_type, uint8_t *p_in, int i_in, char *psz_remote_addr, char *psz_remote_host, uint8_t **pp_data, int *pi_data );
NLC_API httpd_handler_t * httpd_HandlerNew( httpd_host_t *, const char *psz_url, const char *psz_user, const char *psz_password, httpd_handler_callback_t pf_fill, void * ) GOTV_USED;
NLC_API void * httpd_HandlerDelete( httpd_handler_t * );

typedef struct httpd_redirect_t httpd_redirect_t;
NLC_API httpd_redirect_t * httpd_RedirectNew( httpd_host_t *, const char *psz_url_dst, const char *psz_url_src ) GOTV_USED;
NLC_API void httpd_RedirectDelete( httpd_redirect_t * );


typedef struct httpd_stream_t httpd_stream_t;
NLC_API httpd_stream_t * httpd_StreamNew( httpd_host_t *, const char *psz_url, const char *psz_mime, const char *psz_user, const char *psz_password ) GOTV_USED;
NLC_API void httpd_StreamDelete( httpd_stream_t * );
NLC_API int httpd_StreamHeader( httpd_stream_t *, uint8_t *p_data, int i_data );
NLC_API int httpd_StreamSend( httpd_stream_t *, const block_t *p_block );
NLC_API int httpd_StreamSetHTTPHeaders(httpd_stream_t *, const httpd_header *, size_t);

/* Msg functions facilities */
NLC_API void httpd_MsgAdd( httpd_message_t *, const char *psz_name, const char *psz_value, ... ) GOTV_FORMAT( 3, 4 );
/* return "" if not found. The string is not allocated */
NLC_API const char * httpd_MsgGet( const httpd_message_t *, const char *psz_name );

#endif /* _GOTV_HTTPD_H */
