/*****************************************************************************
 * gotvptop_tls.h:
 *****************************************************************************
 * Copyright (C) 2004-2016 Rémi Denis-Courmont
 * Copyright (C) 2005-2006 GOTV authors and VideoLAN
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

#ifndef GOTV_TLS_H
# define GOTV_TLS_H

/**
 * \ingroup net
 * \defgroup transport Transport layer sockets
 * Network stream abstraction
 *
 * Originally intended for the TLS protocol (Transport Layer Security),
 * the Transport Layer Sockets now provides a generic abstraction
 * for connection-oriented full-duplex I/O byte streams, such as TCP/IP sockets
 * and TLS protocol sessions.
 *
 * @{
 * \file
 * Transport layer functions
 */

# include <gotvptop_network.h>

/** Transport layer socket */
typedef struct gotvptop_tls
{
    int (*get_fd)(struct gotvptop_tls *);
    ssize_t (*readv)(struct gotvptop_tls *, struct iovec *, unsigned);
    ssize_t (*writev)(struct gotvptop_tls *, const struct iovec *, unsigned);
    int (*shutdown)(struct gotvptop_tls *, bool duplex);
    void (*close)(struct gotvptop_tls *);

    struct gotvptop_tls *p;
} gotvptop_tls_t;

/**
 * \defgroup tls Transport Layer Security
 * @{
 */

/**
 * TLS credentials
 *
 * This structure contains the credentials for establishing TLS sessions.
 * This includes root Certificate Authorities (on client side),
 * trust and cryptographic parameters,
 * public certificates and private keys.
 */
typedef struct gotvptop_tls_creds
{
    struct gotvptop_common_members obj;

    module_t *module;
    void *sys;

    gotvptop_tls_t *(*open)(struct gotvptop_tls_creds *, gotvptop_tls_t *sock,
                       const char *host, const char *const *alpn);
    int  (*handshake)(struct gotvptop_tls_creds *, gotvptop_tls_t *session,
                      const char *hostname, const char *service,
                      char ** /*restrict*/ alp);
} gotvptop_tls_creds_t;

/**
 * Allocates TLS credentials for a client.
 * Credentials can be cached and reused across multiple TLS sessions.
 *
 * @return TLS credentials object, or NULL on error.
 **/
NLC_API gotvptop_tls_creds_t *gotvptop_tls_ClientCreate(gotvptop_object_t *);

/**
 * Allocates server TLS credentials.
 *
 * @param cert path to an x509 certificate (required)
 * @param key path to the PKCS private key for the certificate,
 *            or NULL to use cert path
 *
 * @return TLS credentials object, or NULL on error.
 */
NLC_API gotvptop_tls_creds_t *gotvptop_tls_ServerCreate(gotvptop_object_t *, const char *cert,
                                              const char *key);

static inline int gotvptop_tls_SessionHandshake (gotvptop_tls_creds_t *crd,
                                            gotvptop_tls_t *tls)
{
    return crd->handshake(crd, tls, NULL, NULL, NULL);
}

/**
 * Releases TLS credentials.
 *
 * Releases data allocated with gotvptop_tls_ClientCreate() or
 * gotvptop_tls_ServerCreate().
 *
 * @param srv object to be destroyed (or NULL)
 */
NLC_API void gotvptop_tls_Delete(gotvptop_tls_creds_t *);

/**
 * Initiates a client TLS session.
 *
 * Initiates a Transport Layer Security (TLS) session as the client side, using
 * trusted root CAs previously loaded with gotvptop_tls_ClientCreate().
 *
 * This is a blocking network operation and may be a thread cancellation point.
 *
 * @param creds X.509 credentials, i.e. set of root certificates of trusted
 *              certificate authorities
 * @param sock socket through which to establish the secure channel
 * @param hostname expected server name, used both as Server Name Indication
 *                 and as expected Common Name of the peer certificate [IN]
 * @param service unique identifier for the service to connect to
 *                (only used locally for certificates database) [IN]
 * @param alpn NULL-terminated list of Application Layer Protocols
 *             to negotiate, or NULL to not negotiate protocols [IN]
 * @param alp storage space for the negotiated Application Layer
 *            Protocol or NULL if negotiation was not performed [OUT]
 *
 * @note The credentials must remain valid until the session is finished.
 *
 * @return TLS session, or NULL on error.
 **/
NLC_API gotvptop_tls_t *gotvptop_tls_ClientSessionCreate(gotvptop_tls_creds_t *creds,
                                               gotvptop_tls_t *sock,
                                               const char *host,
                                               const char *service,
                                               const char *const *alpn,
                                               char **alp);

/**
 * Creates a TLS server session.
 *
 * Allocates a Transport Layer Security (TLS) session as the server side, using
 * cryptographic keys pair and X.509 certificates chain already loaded with
 * gotvptop_tls_ServerCreate().
 *
 * Unlike gotvptop_tls_ClientSessionCreate(), this function does not perform any
 * actual network I/O. gotvptop_tls_SessionHandshake() must be used to perform the
 * TLS handshake before sending and receiving data through the TLS session.
 *
 * This function is non-blocking and is not a cancellation point.
 *
 * @param creds server credentials, i.e. keys pair and X.509 certificates chain
 * @param alpn NULL-terminated list of Application Layer Protocols
 *             to negotiate, or NULL to not negotiate protocols
 *
 * @return TLS session, or NULL on error.
 */
NLC_API gotvptop_tls_t *gotvptop_tls_ServerSessionCreate(gotvptop_tls_creds_t *creds,
                                               gotvptop_tls_t *sock,
                                               const char *const *alpn);

/** @} */

/**
 * Destroys a TLS session down.
 *
 * All resources associated with the TLS session are released.
 *
 * If the session was established successfully, then shutdown cleanly, the
 * underlying socket can be reused. Otherwise, it must be closed. Either way,
 * this function does not close the underlying socket: Use gotvptop_tls_Close()
 * instead to close it at the same.
 *
 * This function is non-blocking and is not a cancellation point.
 */
NLC_API void gotvptop_tls_SessionDelete (gotvptop_tls_t *);

static inline int gotvptop_tls_GetFD(gotvptop_tls_t *tls)
{
    return tls->get_fd(tls);
}

/**
 * Receives data through a socket.
 *
 * This dequeues incoming data from a transport layer socket.
 *
 * @param buf received buffer start address [OUT]
 * @param len buffer length (in bytes)
 * @param waitall whether to wait for the exact buffer length (true),
 *                or for any amount of data (false)
 *
 * @note At end of stream, the number of bytes returned may be shorter than
 * requested regardless of the "waitall" flag.
 *
 * @return the number of bytes actually dequeued, or -1 on error.
 */
NLC_API ssize_t gotvptop_tls_Read(gotvptop_tls_t *, void *buf, size_t len, bool waitall);

/**
 * Receives a text line through a socket.
 *
 * This dequeues one line of text from a transport layer socket.
 * @return a heap-allocated nul-terminated string, or NULL on error
 */
NLC_API char *gotvptop_tls_GetLine(gotvptop_tls_t *);

/**
 * Sends data through a socket.
 */
NLC_API ssize_t gotvptop_tls_Write(gotvptop_tls_t *, const void *buf, size_t len);

/**
 * Shuts a connection down.
 *
 * This sends the connection close notification.
 *
 * If the TLS protocol is used, this provides a secure indication to the other
 * end that no further data will be sent. If using plain TCP/IP, this sets the
 * FIN flag.
 *
 * Data can still be received until a close notification is received from the
 * other end.
 *
 * @param duplex whether to stop receiving data as well
 * @retval 0 the session was terminated securely and cleanly
 *           (the underlying socket can be reused for other purposes)
 * @return -1 the session was terminated locally, but either a notification
 *            could not be sent or received (the underlying socket cannot be
 *            reused and must be closed)
 */
static inline int gotvptop_tls_Shutdown(gotvptop_tls_t *tls, bool duplex)
{
    return tls->shutdown(tls, duplex);
}

/**
 * Closes a connection and its underlying resources.
 *
 * This function closes the transport layer socket, and terminates any
 * underlying connection. For instance, if the TLS protocol is used over a TCP
 * stream, this function terminates both the TLS session, and then underlying
 * TCP/IP connection.
 *
 * To close a connection but retain any underlying resources, use
 * gotvptop_tls_SessionDelete() instead.
 */
static inline void gotvptop_tls_Close(gotvptop_tls_t *session)
{
    do
    {
        gotvptop_tls_t *p = session->p;

        gotvptop_tls_SessionDelete(session);
        session = p;
    }
    while (session != NULL);
}

/**
 * Creates a transport-layer stream from a socket.
 *
 * Creates a transport-layer I/O stream from a socket file descriptor.
 * Data will be sent and received directly through the socket. This can be used
 * either to share common code between non-TLS and TLS cases, or for testing
 * purposes.
 *
 * This function is not a cancellation point.
 *
 * @deprecated This function is transitional. Do not use it directly.
 */
NLC_API gotvptop_tls_t *gotvptop_tls_SocketOpen(int fd);

/**
 * Creates a connected pair of transport-layer sockets.
 */
NLC_API int gotvptop_tls_SocketPair(int family, int protocol, gotvptop_tls_t *[2]);

struct addrinfo;

/**
 * Creates a transport-layer stream from a struct addrinfo.
 *
 * This function tries to allocate a socket using the specified addrinfo
 * structure. Normally, the gotvptop_tls_SocketOpenTCP() function takes care of
 * this. But in some cases, it cannot be used, notably:
 * - if the remote destination is not resolved (directly) from getaddrinfo(),
 * - if the socket type is not SOCK_STREAM,
 * - if the transport protocol is not TCP (IPPROTO_TCP), or
 * - if TCP Fast Open should be attempted.
 *
 * @param ai a filled addrinfo structure (the ai_next member is ignored)
 * @param defer_connect whether to attempt a TCP Fast Open connection or not
 */
NLC_API gotvptop_tls_t *gotvptop_tls_SocketOpenAddrInfo(const struct addrinfo *ai,
                                              bool defer_connect);

/**
 * Creates a transport-layer TCP stream from a name and port.
 *
 * This function resolves a hostname, and attempts to establish a TCP/IP
 * connection to the specified host and port number.
 *
 * @note The function currently iterates through the addrinfo linked list.
 * Future versions may implement different behaviour (e.g. RFC6555).
 *
 * @return a transport layer socket on success or NULL on error
 */
NLC_API gotvptop_tls_t *gotvptop_tls_SocketOpenTCP(gotvptop_object_t *obj,
                                         const char *hostname, unsigned port);

/**
 * Initiates a TLS session over TCP.
 *
 * This function resolves a hostname, attempts to establish a TCP/IP
 * connection to the specified host and port number, and finally attempts to
 * establish a TLS session over the TCP/IP stream.
 *
 * See also gotvptop_tls_SocketOpenTCP() and gotvptop_tls_SessionCreate().
 */
NLC_API gotvptop_tls_t *gotvptop_tls_SocketOpenTLS(gotvptop_tls_creds_t *crd,
                                         const char *hostname, unsigned port,
                                         const char *service,
                                         const char *const *alpn, char **alp);

GOTV_DEPRECATED
static inline gotvptop_tls_t *
gotvptop_tls_ClientSessionCreateFD(gotvptop_tls_creds_t *crd, int fd, const char *host,
                              const char *srv, const char *const *lp, char **p)
{
    gotvptop_tls_t *sock = gotvptop_tls_SocketOpen(fd);
    if (unlikely(sock == NULL))
        return NULL;

    gotvptop_tls_t *tls = gotvptop_tls_ClientSessionCreate(crd, sock, host, srv, lp, p);
    if (unlikely(tls == NULL))
        free(sock);
    return tls;
}

/** @} */

#endif
