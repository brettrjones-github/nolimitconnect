/* -*- c -*-
 * Copyright (C) 2000-2012 Free Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/* This file contains the types and prototypes for all the
 * high level functionality of the gnutls main library.
 *
 * If the optional C++ binding was built, it is available in
 * gnutls/gnutlsxx.h.
 *
 * The openssl compatibility layer (which is under the GNU GPL
 * license) is in gnutls/openssl.h.
 *
 * The low level cipher functionality is in gnutls/crypto.h.
 */


#pragma once
#include <libgnu/config_libgnu.h>
#include <libgnu/abstract_gnu.h>
#include <libgnu/libtasn1.h>


//#include <libgnu/x509/x509_int.h>
/*
#include <stddef.h>
#ifndef HAVE_SSIZE_T
#define HAVE_SSIZE_T
#include <sys/types.h>
#endif
#include <time.h>
*/


NLC_BEGIN_CDECLARES

#define GNUTLS_VERSION "3.4.14"

#define GNUTLS_VERSION_MAJOR 3
#define GNUTLS_VERSION_MINOR 4
#define GNUTLS_VERSION_PATCH 14

#define GNUTLS_VERSION_NUMBER 0x03040e

#define GNUTLS_CIPHER_RIJNDAEL_128_CBC GNUTLS_CIPHER_AES_128_CBC
#define GNUTLS_CIPHER_RIJNDAEL_256_CBC GNUTLS_CIPHER_AES_256_CBC
#define GNUTLS_CIPHER_RIJNDAEL_CBC GNUTLS_CIPHER_AES_128_CBC
#define GNUTLS_CIPHER_ARCFOUR GNUTLS_CIPHER_ARCFOUR_128

# define _SYM_EXPORT NLC_EXPORT

#define DEFAULT_MAX_VERIFY_DEPTH 16
#define DEFAULT_MAX_VERIFY_BITS 16*1024
#define MAX_VERIFY_DEPTH 4096

/* Use the following definition globally in your program to disable
 * implicit initialization of gnutls. */
#define GNUTLS_SKIP_GLOBAL_INIT int _gnutls_global_init_skip(void); \
    int _gnutls_global_init_skip(void) {return 1;}


  /*
   * Flags for gnutls_init()
   *
   * @GNUTLS_SERVER: Connection end is a server.
   * @GNUTLS_CLIENT: Connection end is a client.
   * @GNUTLS_DATAGRAM: Connection is datagram oriented (DTLS).
   * @GNUTLS_NONBLOCK: Connection should not block.
   * @GNUTLS_NO_SIGNAL: In systems where SIGPIPE is delivered on send, it will be disabled. That flag has effect in systems which support the MSG_NOSIGNAL sockets flag.
   * @GNUTLS_NO_EXTENSIONS: Do not enable any TLS extensions by default.
   * @GNUTLS_NO_REPLAY_PROTECTION: Disable any replay protection in DTLS.
   *
   */
#define GNUTLS_SERVER 1
#define GNUTLS_CLIENT (1<<1)
#define GNUTLS_DATAGRAM (1<<2)
#define GNUTLS_NONBLOCK (1<<3)
#define GNUTLS_NO_EXTENSIONS (1<<4)
#define GNUTLS_NO_REPLAY_PROTECTION (1<<5)
#define GNUTLS_NO_SIGNAL (1<<6)

/**
 * gnutls_alert_level_t:
 * @GNUTLS_AL_WARNING: Alert of warning severity.
 * @GNUTLS_AL_FATAL: Alert of fatal severity.
 *
 * Enumeration of different TLS alert severities.
 */
typedef enum {
	GNUTLS_AL_WARNING = 1,
	GNUTLS_AL_FATAL
} gnutls_alert_level_t;

/**
 * gnutls_alert_description_t:
 * @GNUTLS_A_CLOSE_NOTIFY: Close notify.
 * @GNUTLS_A_UNEXPECTED_MESSAGE: Unexpected message.
 * @GNUTLS_A_BAD_RECORD_MAC: Bad record MAC.
 * @GNUTLS_A_DECRYPTION_FAILED: Decryption failed.
 * @GNUTLS_A_RECORD_OVERFLOW: Record overflow.
 * @GNUTLS_A_DECOMPRESSION_FAILURE: Decompression failed.
 * @GNUTLS_A_HANDSHAKE_FAILURE: Handshake failed.
 * @GNUTLS_A_SSL3_NO_CERTIFICATE: No certificate.
 * @GNUTLS_A_BAD_CERTIFICATE: Certificate is bad.
 * @GNUTLS_A_UNSUPPORTED_CERTIFICATE: Certificate is not supported.
 * @GNUTLS_A_CERTIFICATE_REVOKED: Certificate was revoked.
 * @GNUTLS_A_CERTIFICATE_EXPIRED: Certificate is expired.
 * @GNUTLS_A_CERTIFICATE_UNKNOWN: Unknown certificate.
 * @GNUTLS_A_ILLEGAL_PARAMETER: Illegal parameter.
 * @GNUTLS_A_UNKNOWN_CA: CA is unknown.
 * @GNUTLS_A_ACCESS_DENIED: Access was denied.
 * @GNUTLS_A_DECODE_ERROR: Decode error.
 * @GNUTLS_A_DECRYPT_ERROR: Decrypt error.
 * @GNUTLS_A_EXPORT_RESTRICTION: Export restriction.
 * @GNUTLS_A_PROTOCOL_VERSION: Error in protocol version.
 * @GNUTLS_A_INSUFFICIENT_SECURITY: Insufficient security.
 * @GNUTLS_A_USER_CANCELED: User canceled.
 * @GNUTLS_A_INTERNAL_ERROR: Internal error.
 * @GNUTLS_A_INAPPROPRIATE_FALLBACK: Inappropriate fallback,
 * @GNUTLS_A_NO_RENEGOTIATION: No renegotiation is allowed.
 * @GNUTLS_A_CERTIFICATE_UNOBTAINABLE: Could not retrieve the
 *   specified certificate.
 * @GNUTLS_A_UNSUPPORTED_EXTENSION: An unsupported extension was
 *   sent.
 * @GNUTLS_A_UNRECOGNIZED_NAME: The server name sent was not
 *   recognized.
 * @GNUTLS_A_UNKNOWN_PSK_IDENTITY: The SRP/PSK username is missing
 *   or not known.
 * @GNUTLS_A_NO_APPLICATION_PROTOCOL: The ALPN protocol requested is
 *   not supported by the peer.
 *
 * Enumeration of different TLS alerts.
 */
typedef enum {
	GNUTLS_A_CLOSE_NOTIFY,
	GNUTLS_A_UNEXPECTED_MESSAGE = 10,
	GNUTLS_A_BAD_RECORD_MAC = 20,
	GNUTLS_A_DECRYPTION_FAILED,
	GNUTLS_A_RECORD_OVERFLOW,
	GNUTLS_A_DECOMPRESSION_FAILURE = 30,
	GNUTLS_A_HANDSHAKE_FAILURE = 40,
	GNUTLS_A_SSL3_NO_CERTIFICATE = 41,
	GNUTLS_A_BAD_CERTIFICATE = 42,
	GNUTLS_A_UNSUPPORTED_CERTIFICATE,
	GNUTLS_A_CERTIFICATE_REVOKED,
	GNUTLS_A_CERTIFICATE_EXPIRED,
	GNUTLS_A_CERTIFICATE_UNKNOWN,
	GNUTLS_A_ILLEGAL_PARAMETER,
	GNUTLS_A_UNKNOWN_CA,
	GNUTLS_A_ACCESS_DENIED,
	GNUTLS_A_DECODE_ERROR = 50,
	GNUTLS_A_DECRYPT_ERROR,
	GNUTLS_A_EXPORT_RESTRICTION = 60,
	GNUTLS_A_PROTOCOL_VERSION = 70,
	GNUTLS_A_INSUFFICIENT_SECURITY,
	GNUTLS_A_INTERNAL_ERROR = 80,
	GNUTLS_A_INAPPROPRIATE_FALLBACK = 86,
	GNUTLS_A_USER_CANCELED = 90,
	GNUTLS_A_NO_RENEGOTIATION = 100,
	GNUTLS_A_UNSUPPORTED_EXTENSION = 110,
	GNUTLS_A_CERTIFICATE_UNOBTAINABLE = 111,
	GNUTLS_A_UNRECOGNIZED_NAME = 112,
	GNUTLS_A_UNKNOWN_PSK_IDENTITY = 115,
	GNUTLS_A_NO_APPLICATION_PROTOCOL = 120
} gnutls_alert_description_t;

const char
    *gnutls_handshake_description_get_name(gnutls_handshake_description_t
					   type);


#define GNUTLS_PK_ECC GNUTLS_PK_EC

const char *gnutls_pk_algorithm_get_name(gnutls_pk_algorithm_t algorithm);



/* macros to allow specifying a specific curve in gnutls_privkey_generate()
 * and gnutls_x509_privkey_generate() */
#define GNUTLS_CURVE_TO_BITS(curve) (unsigned int)(((unsigned int)1<<31)|((unsigned int)(curve)))
#define GNUTLS_BITS_TO_CURVE(bits) (((unsigned int)(bits)) & 0x7FFFFFFF)
#define GNUTLS_BITS_ARE_CURVE(bits) (((unsigned int)(bits)) & 0x80000000)


/* old name */
#define GNUTLS_SEC_PARAM_NORMAL GNUTLS_SEC_PARAM_MEDIUM

/**
 * gnutls_channel_binding_t:
 * @GNUTLS_CB_TLS_UNIQUE: "tls-unique" (RFC 5929) channel binding
 *
 * Enumeration of support channel binding types.
 */
typedef enum {
	GNUTLS_CB_TLS_UNIQUE
} gnutls_channel_binding_t;


typedef struct gnutls_params_st {
	gnutls_params_type_t type;
	union params {
		gnutls_dh_params_t dh;
		gnutls_rsa_params_t rsa_export;
	} params;
	int deinit;
} gnutls_params_st;

typedef int gnutls_params_function(gnutls_session_t, gnutls_params_type_t,
				   gnutls_params_st *);

/* internal functions */

int gnutls_init(gnutls_session_t * session, unsigned int flags);
void gnutls_deinit(gnutls_session_t session);
#define _gnutls_deinit(x) gnutls_deinit(x)

int gnutls_bye(gnutls_session_t session, gnutls_close_request_t how);

int gnutls_handshake(gnutls_session_t session);

#define GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT ((unsigned int)-1)
void gnutls_handshake_set_timeout(gnutls_session_t session,
				  unsigned int ms);
int gnutls_rehandshake(gnutls_session_t session);

gnutls_alert_description_t gnutls_alert_get(gnutls_session_t session);
int gnutls_alert_send(gnutls_session_t session,
		      gnutls_alert_level_t level,
		      gnutls_alert_description_t desc);
int gnutls_alert_send_appropriate(gnutls_session_t session, int err);
const char *gnutls_alert_get_name(gnutls_alert_description_t alert);
const char *gnutls_alert_get_strname(gnutls_alert_description_t alert);

gnutls_sec_param_t gnutls_pk_bits_to_sec_param(gnutls_pk_algorithm_t algo,
					       unsigned int bits);
const char *gnutls_sec_param_get_name(gnutls_sec_param_t param);
unsigned int gnutls_sec_param_to_pk_bits(gnutls_pk_algorithm_t algo,
					 gnutls_sec_param_t param);
unsigned int
gnutls_sec_param_to_symmetric_bits(gnutls_sec_param_t param);

/* Elliptic curves */
const char *gnutls_ecc_curve_get_name(gnutls_ecc_curve_t curve);
const char *gnutls_ecc_curve_get_oid(gnutls_ecc_curve_t curve);

int gnutls_ecc_curve_get_size(gnutls_ecc_curve_t curve);
gnutls_ecc_curve_t gnutls_ecc_curve_get(gnutls_session_t session);

/* get information on the current session */
gnutls_cipher_algorithm_t gnutls_cipher_get(gnutls_session_t session);
gnutls_kx_algorithm_t gnutls_kx_get(gnutls_session_t session);
gnutls_mac_algorithm_t gnutls_mac_get(gnutls_session_t session);
gnutls_compression_method_t
gnutls_compression_get(gnutls_session_t session);
gnutls_certificate_type_t
gnutls_certificate_type_get(gnutls_session_t session);

int gnutls_sign_algorithm_get(gnutls_session_t session);
int gnutls_sign_algorithm_get_client(gnutls_session_t session);

int gnutls_sign_algorithm_get_requested(gnutls_session_t session,
					size_t indx,
					gnutls_sign_algorithm_t * algo);

/* the name of the specified algorithms */
const char *gnutls_cipher_get_name(gnutls_cipher_algorithm_t algorithm);
const char *gnutls_mac_get_name(gnutls_mac_algorithm_t algorithm);

const char *gnutls_digest_get_name(gnutls_digest_algorithm_t algorithm);
const char *gnutls_digest_get_oid(gnutls_digest_algorithm_t algorithm);

const char *gnutls_compression_get_name(gnutls_compression_method_t
					algorithm);
const char *gnutls_kx_get_name(gnutls_kx_algorithm_t algorithm);
const char *gnutls_certificate_type_get_name(gnutls_certificate_type_t
					     type);
const char *gnutls_pk_get_name(gnutls_pk_algorithm_t algorithm);
const char *gnutls_pk_get_oid(gnutls_pk_algorithm_t algorithm);

const char *gnutls_sign_get_name(gnutls_sign_algorithm_t algorithm);
const char *gnutls_sign_get_oid(gnutls_sign_algorithm_t algorithm);

size_t gnutls_cipher_get_key_size(gnutls_cipher_algorithm_t algorithm);
size_t gnutls_mac_get_key_size(gnutls_mac_algorithm_t algorithm);

int gnutls_sign_is_secure(gnutls_sign_algorithm_t algorithm);
gnutls_digest_algorithm_t
gnutls_sign_get_hash_algorithm(gnutls_sign_algorithm_t sign);
gnutls_pk_algorithm_t
gnutls_sign_get_pk_algorithm(gnutls_sign_algorithm_t sign);
gnutls_sign_algorithm_t
gnutls_pk_to_sign(gnutls_pk_algorithm_t pk,
		  gnutls_digest_algorithm_t hash);

#define gnutls_sign_algorithm_get_name gnutls_sign_get_name

gnutls_mac_algorithm_t gnutls_mac_get_id(const char *name);
gnutls_digest_algorithm_t gnutls_digest_get_id(const char *name);

gnutls_compression_method_t gnutls_compression_get_id(const char *name);
gnutls_cipher_algorithm_t gnutls_cipher_get_id(const char *name);
gnutls_kx_algorithm_t gnutls_kx_get_id(const char *name);
gnutls_protocol_t gnutls_protocol_get_id(const char *name);
gnutls_certificate_type_t gnutls_certificate_type_get_id(const char *name);
gnutls_pk_algorithm_t gnutls_pk_get_id(const char *name);
gnutls_sign_algorithm_t gnutls_sign_get_id(const char *name);
gnutls_ecc_curve_t gnutls_ecc_curve_get_id(const char *name);

gnutls_digest_algorithm_t gnutls_oid_to_digest(const char *oid);
gnutls_pk_algorithm_t gnutls_oid_to_pk(const char *oid);
gnutls_sign_algorithm_t gnutls_oid_to_sign(const char *oid);
gnutls_ecc_curve_t gnutls_oid_to_ecc_curve(const char *oid);

  /* list supported algorithms */
const gnutls_ecc_curve_t *gnutls_ecc_curve_list(void);
const gnutls_cipher_algorithm_t *gnutls_cipher_list(void);
const gnutls_mac_algorithm_t *gnutls_mac_list(void);
const gnutls_digest_algorithm_t *gnutls_digest_list(void);
const gnutls_compression_method_t *gnutls_compression_list(void);
const gnutls_protocol_t *gnutls_protocol_list(void);
const gnutls_certificate_type_t *gnutls_certificate_type_list(void);
const gnutls_kx_algorithm_t *gnutls_kx_list(void);
const gnutls_pk_algorithm_t *gnutls_pk_list(void);
const gnutls_sign_algorithm_t *gnutls_sign_list(void);
const char *gnutls_cipher_suite_info(size_t idx,
				     unsigned char *cs_id,
				     gnutls_kx_algorithm_t * kx,
				     gnutls_cipher_algorithm_t * cipher,
				     gnutls_mac_algorithm_t * mac,
				     gnutls_protocol_t * min_version);

  /* error functions */
int gnutls_error_is_fatal(int error);
int gnutls_error_to_alert(int err, int *level);

void gnutls_perror(int error);
const char *gnutls_strerror(int error);
const char *gnutls_strerror_name(int error);

/* Semi-internal functions.
 */
void gnutls_handshake_set_private_extensions(gnutls_session_t session,
					     int allow);
int gnutls_handshake_set_random(gnutls_session_t session,
				const gnutls_datum_t * random);

gnutls_handshake_description_t
gnutls_handshake_get_last_out(gnutls_session_t session);
gnutls_handshake_description_t
gnutls_handshake_get_last_in(gnutls_session_t session);

/* Record layer functions.
 */
#define GNUTLS_HEARTBEAT_WAIT 1
int gnutls_heartbeat_ping(gnutls_session_t session, size_t data_size,
			  unsigned int max_tries, unsigned int flags);
int gnutls_heartbeat_pong(gnutls_session_t session, unsigned int flags);

void gnutls_record_set_timeout(gnutls_session_t session, unsigned int ms);
void gnutls_record_disable_padding(gnutls_session_t session);

void gnutls_record_cork(gnutls_session_t session);
#define GNUTLS_RECORD_WAIT 1
int gnutls_record_uncork(gnutls_session_t session, unsigned int flags);
size_t gnutls_record_discard_queued(gnutls_session_t session);

int
gnutls_record_get_state(gnutls_session_t session,
			unsigned read,
			gnutls_datum_t *mac_key,
			gnutls_datum_t *IV,
			gnutls_datum_t *cipher_key,
			unsigned char seq_number[8]);

int
gnutls_record_set_state(gnutls_session_t session,
			unsigned read,
			unsigned char seq_number[8]);

typedef struct {
	size_t low;
	size_t high;
} gnutls_range_st;

int gnutls_range_split(gnutls_session_t session,
		       const gnutls_range_st * orig,
		       gnutls_range_st * small_range,
		       gnutls_range_st * rem_range);

ssize_t gnutls_record_send(gnutls_session_t session, const void *data,
			   size_t data_size);
ssize_t gnutls_record_send_range(gnutls_session_t session,
				 const void *data, size_t data_size,
				 const gnutls_range_st * range);
ssize_t gnutls_record_recv(gnutls_session_t session, void *data,
			   size_t data_size);

typedef struct mbuffer_st *gnutls_packet_t;

ssize_t
gnutls_record_recv_packet(gnutls_session_t session, 
		   	  gnutls_packet_t *packet);

void gnutls_packet_get(gnutls_packet_t packet, gnutls_datum_t *data, unsigned char *sequence);
void gnutls_packet_deinit(gnutls_packet_t packet);

#define gnutls_read gnutls_record_recv
#define gnutls_write gnutls_record_send
ssize_t gnutls_record_recv_seq(gnutls_session_t session, void *data,
			       size_t data_size, unsigned char *seq);

size_t gnutls_record_overhead_size(gnutls_session_t session);

size_t gnutls_est_record_overhead_size(gnutls_protocol_t version,
				       gnutls_cipher_algorithm_t cipher,
				       gnutls_mac_algorithm_t mac,
				       gnutls_compression_method_t comp,
				       unsigned int flags);

void gnutls_session_enable_compatibility_mode(gnutls_session_t session);
#define gnutls_record_set_max_empty_records(session, x)

int gnutls_record_can_use_length_hiding(gnutls_session_t session);

int gnutls_record_get_direction(gnutls_session_t session);

size_t gnutls_record_get_max_size(gnutls_session_t session);
ssize_t gnutls_record_set_max_size(gnutls_session_t session, size_t size);

size_t gnutls_record_check_pending(gnutls_session_t session);
size_t gnutls_record_check_corked(gnutls_session_t session);

void gnutls_session_force_valid(gnutls_session_t session);

int gnutls_prf(gnutls_session_t session,
	       size_t label_size, const char *label,
	       int server_random_first,
	       size_t extra_size, const char *extra,
	       size_t outsize, char *out);
int gnutls_prf_rfc5705(gnutls_session_t session,
	       size_t label_size, const char *label,
	       size_t context_size, const char *context,
	       size_t outsize, char *out);

int gnutls_prf_raw(gnutls_session_t session,
		   size_t label_size, const char *label,
		   size_t seed_size, const char *seed,
		   size_t outsize, char *out);

/**
 * gnutls_server_name_type_t:
 * @GNUTLS_NAME_DNS: Domain Name System name type.
 *
 * Enumeration of different server name types.
 */
typedef enum {
	GNUTLS_NAME_DNS = 1
} gnutls_server_name_type_t;

int gnutls_server_name_set(gnutls_session_t session,
			   gnutls_server_name_type_t type,
			   const void *name, size_t name_length);

int gnutls_server_name_get(gnutls_session_t session,
			   void *data, size_t * data_length,
			   unsigned int *type, unsigned int indx);

unsigned int gnutls_heartbeat_get_timeout(gnutls_session_t session);
void gnutls_heartbeat_set_timeouts(gnutls_session_t session,
				   unsigned int retrans_timeout,
				   unsigned int total_timeout);

#define GNUTLS_HB_PEER_ALLOWED_TO_SEND (1)
#define GNUTLS_HB_PEER_NOT_ALLOWED_TO_SEND (1<<1)

  /* Heartbeat */
void gnutls_heartbeat_enable(gnutls_session_t session, unsigned int type);

#define GNUTLS_HB_LOCAL_ALLOWED_TO_SEND (1<<2)
int gnutls_heartbeat_allowed(gnutls_session_t session, unsigned int type);

  /* Safe renegotiation */
int gnutls_safe_renegotiation_status(gnutls_session_t session);
unsigned gnutls_session_ext_master_secret_status(gnutls_session_t session);
unsigned gnutls_session_etm_status(gnutls_session_t session);

/**
 * gnutls_supplemental_data_format_type_t:
 * @GNUTLS_SUPPLEMENTAL_UNKNOWN: Unknown data format
 *
 * Enumeration of different supplemental data types (RFC 4680).
 */
typedef enum {
	GNUTLS_SUPPLEMENTAL_UNKNOWN = 0,
} gnutls_supplemental_data_format_type_t;

const char
*gnutls_supplemental_get_name(gnutls_supplemental_data_format_type_t type);

  /* SessionTicket, RFC 5077. */
int gnutls_session_ticket_key_generate(gnutls_datum_t * key);
int gnutls_session_ticket_enable_client(gnutls_session_t session);
int gnutls_session_ticket_enable_server(gnutls_session_t session,
					const gnutls_datum_t * key);

  /* SRTP, RFC 5764 */

/**
 * gnutls_srtp_profile_t:
 * @GNUTLS_SRTP_AES128_CM_HMAC_SHA1_80: 128 bit AES with a 80 bit HMAC-SHA1
 * @GNUTLS_SRTP_AES128_CM_HMAC_SHA1_32: 128 bit AES with a 32 bit HMAC-SHA1
 * @GNUTLS_SRTP_NULL_HMAC_SHA1_80: NULL cipher with a 80 bit HMAC-SHA1
 * @GNUTLS_SRTP_NULL_HMAC_SHA1_32: NULL cipher with a 32 bit HMAC-SHA1
 *
 * Enumeration of different SRTP protection profiles.
 */
typedef enum {
	GNUTLS_SRTP_AES128_CM_HMAC_SHA1_80 = 0x0001,
	GNUTLS_SRTP_AES128_CM_HMAC_SHA1_32 = 0x0002,
	GNUTLS_SRTP_NULL_HMAC_SHA1_80 = 0x0005,
	GNUTLS_SRTP_NULL_HMAC_SHA1_32 = 0x0006
} gnutls_srtp_profile_t;

int gnutls_srtp_set_profile(gnutls_session_t session,
			    gnutls_srtp_profile_t profile);
int gnutls_srtp_set_profile_direct(gnutls_session_t session,
				   const char *profiles,
				   const char **err_pos);
int gnutls_srtp_get_selected_profile(gnutls_session_t session,
				     gnutls_srtp_profile_t * profile);

const char *gnutls_srtp_get_profile_name(gnutls_srtp_profile_t profile);
int gnutls_srtp_get_profile_id(const char *name,
			       gnutls_srtp_profile_t * profile);
int gnutls_srtp_get_keys(gnutls_session_t session,
			 void *key_material,
			 unsigned int key_material_size,
			 gnutls_datum_t * client_key,
			 gnutls_datum_t * client_salt,
			 gnutls_datum_t * server_key,
			 gnutls_datum_t * server_salt);

int gnutls_srtp_set_mki(gnutls_session_t session,
			const gnutls_datum_t * mki);
int gnutls_srtp_get_mki(gnutls_session_t session, gnutls_datum_t * mki);

/* ALPN TLS extension */
#define GNUTLS_ALPN_MAND 1
int gnutls_alpn_get_selected_protocol(gnutls_session_t session,
				      gnutls_datum_t * protocol);
int gnutls_alpn_set_protocols(gnutls_session_t session,
			      const gnutls_datum_t * protocols,
			      unsigned protocols_size, unsigned flags);

int gnutls_key_generate(gnutls_datum_t * key, unsigned int key_size);

/* if you just want some defaults, use the following.
 */

int gnutls_priority_init(gnutls_priority_t * priority_cache,
			 const char *priorities, const char **err_pos);
void gnutls_priority_deinit(gnutls_priority_t priority_cache);
int gnutls_priority_get_cipher_suite_index(gnutls_priority_t pcache,
					   unsigned int idx,
					   unsigned int *sidx);

#define GNUTLS_PRIORITY_LIST_INIT_KEYWORDS 1
#define GNUTLS_PRIORITY_LIST_SPECIAL 2
const char *
gnutls_priority_string_list(unsigned iter, unsigned int flags);

int gnutls_priority_set(gnutls_session_t session,
			gnutls_priority_t priority);
int gnutls_priority_set_direct(gnutls_session_t session,
			       const char *priorities,
			       const char **err_pos);

int gnutls_priority_certificate_type_list(gnutls_priority_t pcache,
					  const unsigned int **list);
int gnutls_priority_sign_list(gnutls_priority_t pcache,
			      const unsigned int **list);
int gnutls_priority_protocol_list(gnutls_priority_t pcache,
				  const unsigned int **list);
int gnutls_priority_compression_list(gnutls_priority_t pcache,
				     const unsigned int **list);
int gnutls_priority_ecc_curve_list(gnutls_priority_t pcache,
				   const unsigned int **list);

int gnutls_priority_kx_list(gnutls_priority_t pcache,
			    const unsigned int **list);
int gnutls_priority_cipher_list(gnutls_priority_t pcache,
				const unsigned int **list);
int gnutls_priority_mac_list(gnutls_priority_t pcache,
			     const unsigned int **list);

  /* for compatibility
   */
int gnutls_set_default_priority(gnutls_session_t session);

/* Returns the name of a cipher suite */
const char *gnutls_cipher_suite_get_name(gnutls_kx_algorithm_t
					 kx_algorithm,
					 gnutls_cipher_algorithm_t
					 cipher_algorithm,
					 gnutls_mac_algorithm_t
					 mac_algorithm);

/* get the currently used protocol version */
gnutls_protocol_t gnutls_protocol_get_version(gnutls_session_t session);

const char *gnutls_protocol_get_name(gnutls_protocol_t version);


/* get/set session 
 */
int gnutls_session_set_data(gnutls_session_t session,
			    const void *session_data,
			    size_t session_data_size);
int gnutls_session_get_data(gnutls_session_t session, void *session_data,
			    size_t * session_data_size);
int gnutls_session_get_data2(gnutls_session_t session,
			     gnutls_datum_t * data);
void gnutls_session_get_random(gnutls_session_t session,
			       gnutls_datum_t * client,
			       gnutls_datum_t * server);
char *gnutls_session_get_desc(gnutls_session_t session);

void gnutls_session_set_verify_function(gnutls_session_t session, gnutls_certificate_verify_function * func);

void gnutls_session_set_verify_cert(gnutls_session_t session,
			       const char *hostname, unsigned flags);

void
gnutls_session_set_verify_cert2(gnutls_session_t session,
				gnutls_typed_vdata_st * data,
				unsigned elements, unsigned flags);

unsigned int gnutls_session_get_verify_cert_status(gnutls_session_t);

int gnutls_session_set_premaster(gnutls_session_t session,
				 unsigned int entity,
				 gnutls_protocol_t version,
				 gnutls_kx_algorithm_t kx,
				 gnutls_cipher_algorithm_t cipher,
				 gnutls_mac_algorithm_t mac,
				 gnutls_compression_method_t comp,
				 const gnutls_datum_t * master,
				 const gnutls_datum_t * session_id);

/* returns the session ID */
#define GNUTLS_MAX_SESSION_ID 32
int gnutls_session_get_id(gnutls_session_t session, void *session_id,
			  size_t * session_id_size);
int gnutls_session_get_id2(gnutls_session_t session,
			   gnutls_datum_t * session_id);

int gnutls_session_set_id(gnutls_session_t session,
			  const gnutls_datum_t * sid);

int gnutls_session_channel_binding(gnutls_session_t session,
				   gnutls_channel_binding_t cbtype,
				   gnutls_datum_t * cb);

/* checks if this session is a resumed one 
 */
int gnutls_session_is_resumed(gnutls_session_t session);
int gnutls_session_resumption_requested(gnutls_session_t session);

void gnutls_db_set_cache_expiration(gnutls_session_t session, int seconds);
unsigned gnutls_db_get_default_cache_expiration(void);

void gnutls_db_remove_session(gnutls_session_t session);
void gnutls_db_set_retrieve_function(gnutls_session_t session,
				     gnutls_db_retr_func retr_func);
void gnutls_db_set_remove_function(gnutls_session_t session,
				   gnutls_db_remove_func rem_func);
void gnutls_db_set_store_function(gnutls_session_t session,
				  gnutls_db_store_func store_func);
void gnutls_db_set_ptr(gnutls_session_t session, void *ptr);
void *gnutls_db_get_ptr(gnutls_session_t session);
int gnutls_db_check_entry(gnutls_session_t session,
			  gnutls_datum_t session_entry);
time_t gnutls_db_check_entry_time(gnutls_datum_t * entry);

  /**
   * gnutls_handshake_hook_func:
   * @session: the current session
   * @htype: the type of the handshake message (%gnutls_handshake_description_t)
   * @post: non zero if this is a post-process/generation call and zero otherwise
   * @incoming: non zero if this is an incoming message and zero if this is an outgoing message
   * @msg: the (const) data of the handshake message without the handshake headers.
   *
   * Function prototype for handshake hooks. It is set using
   * gnutls_handshake_set_hook_function().
   *
   * Returns: Non zero on error.
   */
#define GNUTLS_HOOK_POST (1)
#define GNUTLS_HOOK_PRE (0)
#define GNUTLS_HOOK_BOTH (-1)


void gnutls_handshake_set_hook_function(gnutls_session_t session,
					unsigned int htype, int post,
					gnutls_handshake_hook_func func);

void
gnutls_handshake_set_post_client_hello_function(gnutls_session_t session,
						gnutls_handshake_post_client_hello_func
						func);

void gnutls_handshake_set_max_packet_length(gnutls_session_t session,
					    size_t max);

/* returns libgnutls version (call it with a NULL argument)
 */
const char *gnutls_check_version(const char *req_version);

/* Functions for setting/clearing credentials
 */
void gnutls_credentials_clear(gnutls_session_t session);

/* cred is a structure defined by the kx algorithm
 */
int gnutls_credentials_set(gnutls_session_t session,
			   gnutls_credentials_type_t type, void *cred);
int gnutls_credentials_get(gnutls_session_t session,
			   gnutls_credentials_type_t type, void **cred);
#define gnutls_cred_set	gnutls_credentials_set


void gnutls_anon_free_server_credentials(gnutls_anon_server_credentials_t
					 sc);
int
gnutls_anon_allocate_server_credentials(gnutls_anon_server_credentials_t
					* sc);

void gnutls_anon_set_server_dh_params(gnutls_anon_server_credentials_t res,
				      gnutls_dh_params_t dh_params);

void
gnutls_anon_set_server_params_function(gnutls_anon_server_credentials_t
				       res, gnutls_params_function * func);

void
gnutls_anon_free_client_credentials(gnutls_anon_client_credentials_t sc);
int
gnutls_anon_allocate_client_credentials(gnutls_anon_client_credentials_t
					* sc);

/* CERTFILE is an x509 certificate in PEM form.
 * KEYFILE is a pkcs-1 private key in PEM form (for RSA keys).
 */
void
gnutls_certificate_free_credentials(gnutls_certificate_credentials_t sc);
int
gnutls_certificate_allocate_credentials(gnutls_certificate_credentials_t
					* res);

int
gnutls_certificate_get_issuer(gnutls_certificate_credentials_t sc,
			      gnutls_x509_crt_t cert,
			      gnutls_x509_crt_t * issuer,
			      unsigned int flags);

int gnutls_certificate_get_crt_raw(gnutls_certificate_credentials_t sc,
				   unsigned idx1, unsigned idx2,
				   gnutls_datum_t * cert);

int
gnutls_certificate_get_x509_crt(gnutls_certificate_credentials_t res,
                                unsigned index,
                                gnutls_x509_crt_t **crt_list,
                                unsigned *crt_list_size);

int
gnutls_certificate_get_x509_key(gnutls_certificate_credentials_t res,
                                unsigned index,
                                gnutls_x509_privkey_t *key);

void gnutls_certificate_free_keys(gnutls_certificate_credentials_t sc);
void gnutls_certificate_free_cas(gnutls_certificate_credentials_t sc);
void gnutls_certificate_free_ca_names(gnutls_certificate_credentials_t sc);
void gnutls_certificate_free_crls(gnutls_certificate_credentials_t sc);

void gnutls_certificate_set_dh_params(gnutls_certificate_credentials_t res,
				      gnutls_dh_params_t dh_params);
void gnutls_certificate_set_verify_flags(gnutls_certificate_credentials_t
					 res, unsigned int flags);
unsigned int
gnutls_certificate_get_verify_flags(gnutls_certificate_credentials_t res);

/**
 * gnutls_certificate_flags:
 * @GNUTLS_CERTIFICATE_SKIP_KEY_CERT_MATCH: Skip the key and certificate matching check.
 *
 * Enumeration of different certificate credentials flags.
 */
typedef enum gnutls_certificate_flags {
	GNUTLS_CERTIFICATE_SKIP_KEY_CERT_MATCH = 1
} gnutls_certificate_flags;

void gnutls_certificate_set_flags(gnutls_certificate_credentials_t,
				  unsigned flags);

void gnutls_certificate_set_verify_limits(gnutls_certificate_credentials_t
					  res, unsigned int max_bits,
					  unsigned int max_depth);

unsigned int
gnutls_certificate_get_verify_flags(gnutls_certificate_credentials_t);

int
gnutls_certificate_set_x509_system_trust(gnutls_certificate_credentials_t
					 cred);

int
gnutls_certificate_set_x509_trust_file(gnutls_certificate_credentials_t
				       cred, const char *cafile,
				       gnutls_x509_crt_fmt_t type);
int
gnutls_certificate_set_x509_trust_dir(gnutls_certificate_credentials_t cred,
				      const char *ca_dir,
				      gnutls_x509_crt_fmt_t type);

int gnutls_certificate_set_x509_trust_mem(gnutls_certificate_credentials_t
					  res, const gnutls_datum_t * ca,
					  gnutls_x509_crt_fmt_t type);

int
gnutls_certificate_set_x509_crl_file(gnutls_certificate_credentials_t
				     res, const char *crlfile,
				     gnutls_x509_crt_fmt_t type);
int gnutls_certificate_set_x509_crl_mem(gnutls_certificate_credentials_t
					res, const gnutls_datum_t * CRL,
					gnutls_x509_crt_fmt_t type);

int
gnutls_certificate_set_x509_key_file(gnutls_certificate_credentials_t
				     res, const char *certfile,
				     const char *keyfile,
				     gnutls_x509_crt_fmt_t type);

int
gnutls_certificate_set_x509_key_file2(gnutls_certificate_credentials_t
				      res, const char *certfile,
				      const char *keyfile,
				      gnutls_x509_crt_fmt_t type,
				      const char *pass,
				      unsigned int flags);

int gnutls_certificate_set_x509_key_mem(gnutls_certificate_credentials_t
					res, const gnutls_datum_t * cert,
					const gnutls_datum_t * key,
					gnutls_x509_crt_fmt_t type);

int gnutls_certificate_set_x509_key_mem2(gnutls_certificate_credentials_t
					 res, const gnutls_datum_t * cert,
					 const gnutls_datum_t * key,
					 gnutls_x509_crt_fmt_t type,
					 const char *pass,
					 unsigned int flags);

void gnutls_certificate_send_x509_rdn_sequence(gnutls_session_t session,
					       int status);

int
gnutls_certificate_set_x509_simple_pkcs12_file
(gnutls_certificate_credentials_t res, const char *pkcs12file,
 gnutls_x509_crt_fmt_t type, const char *password);
int
gnutls_certificate_set_x509_simple_pkcs12_mem
(gnutls_certificate_credentials_t res, const gnutls_datum_t * p12blob,
 gnutls_x509_crt_fmt_t type, const char *password);

/* New functions to allow setting already parsed X.509 stuff.
 */

int gnutls_certificate_set_x509_key(gnutls_certificate_credentials_t res,
				    gnutls_x509_crt_t * cert_list,
				    int cert_list_size,
				    gnutls_x509_privkey_t key);
int gnutls_certificate_set_x509_trust(gnutls_certificate_credentials_t res,
				      gnutls_x509_crt_t * ca_list,
				      int ca_list_size);
int gnutls_certificate_set_x509_crl(gnutls_certificate_credentials_t res,
				    gnutls_x509_crl_t * crl_list,
				    int crl_list_size);

int gnutls_certificate_get_x509_key(gnutls_certificate_credentials_t res,
                                    unsigned index,
                                    gnutls_x509_privkey_t *key);
int gnutls_certificate_get_x509_crt(gnutls_certificate_credentials_t res,
                                    unsigned index,
                                    gnutls_x509_crt_t **crt_list,
                                    unsigned *crt_list_size);

  /* OCSP status request extension, RFC 6066 */
typedef int (*gnutls_status_request_ocsp_func)
 (gnutls_session_t session, void *ptr, gnutls_datum_t * ocsp_response);

void
gnutls_certificate_set_ocsp_status_request_function
(gnutls_certificate_credentials_t res,
gnutls_status_request_ocsp_func ocsp_func, void *ptr);

int
gnutls_certificate_set_ocsp_status_request_file
(gnutls_certificate_credentials_t res, const char *response_file,
 unsigned int flags);

int gnutls_ocsp_status_request_enable_client(gnutls_session_t session,
					     gnutls_datum_t * responder_id,
					     size_t responder_id_size,
					     gnutls_datum_t *
					     request_extensions);

int gnutls_ocsp_status_request_get(gnutls_session_t session,
				   gnutls_datum_t * response);

#define GNUTLS_OCSP_SR_IS_AVAIL 1
int gnutls_ocsp_status_request_is_checked(gnutls_session_t session,
					  unsigned int flags);

/* global state functions
 */
int gnutls_global_init(void);
void gnutls_global_deinit(void);

  /**
   * gnutls_time_func:
   * @t: where to store time.
   *
   * Function prototype for time()-like function.  Set with
   * gnutls_global_set_time_function().
   *
   * Returns: Number of seconds since the epoch, or (time_t)-1 on errors.
   */


void gnutls_global_set_mutex(mutex_init_func init,
			     mutex_deinit_func deinit,
			     mutex_lock_func lock,
			     mutex_unlock_func unlock);

void gnutls_global_set_time_function(gnutls_time_func time_func);



/* constant time memcmp */
int gnutls_memcmp(const void *s1, const void *s2, size_t n);

void gnutls_global_set_log_function(gnutls_log_func log_func);
void gnutls_global_set_audit_log_function(gnutls_audit_log_func log_func);
void gnutls_global_set_log_level(int level);

/* Diffie-Hellman parameter handling.
 */
int gnutls_dh_params_init(gnutls_dh_params_t * dh_params);
void gnutls_dh_params_deinit(gnutls_dh_params_t dh_params);
int gnutls_dh_params_import_raw(gnutls_dh_params_t dh_params,
				const gnutls_datum_t * prime,
				const gnutls_datum_t * generator);
int gnutls_dh_params_import_raw2(gnutls_dh_params_t dh_params,
				 const gnutls_datum_t * prime,
				 const gnutls_datum_t * generator,
				 unsigned key_bits);
int gnutls_dh_params_import_pkcs3(gnutls_dh_params_t params,
				  const gnutls_datum_t * pkcs3_params,
				  gnutls_x509_crt_fmt_t format);
int gnutls_dh_params_generate2(gnutls_dh_params_t params,
			       unsigned int bits);
int gnutls_dh_params_export_pkcs3(gnutls_dh_params_t params,
				  gnutls_x509_crt_fmt_t format,
				  unsigned char *params_data,
				  size_t * params_data_size);
int gnutls_dh_params_export2_pkcs3(gnutls_dh_params_t params,
				   gnutls_x509_crt_fmt_t format,
				   gnutls_datum_t * out);
int gnutls_dh_params_export_raw(gnutls_dh_params_t params,
				gnutls_datum_t * prime,
				gnutls_datum_t * generator,
				unsigned int *bits);
int gnutls_dh_params_cpy(gnutls_dh_params_t dst, gnutls_dh_params_t src);


  /**
   * gnutls_random_art_t:
   * @GNUTLS_RANDOM_ART_OPENSSH: OpenSSH-style random art.
   *
   * Enumeration of different random art types.
   */
typedef enum gnutls_random_art {
	GNUTLS_RANDOM_ART_OPENSSH = 1
} gnutls_random_art_t;

int gnutls_random_art(gnutls_random_art_t type,
		      const char *key_type, unsigned int key_size,
		      void *fpr, size_t fpr_size, gnutls_datum_t * art);

/* SRP 
 */

typedef struct gnutls_srp_server_credentials_st
*gnutls_srp_server_credentials_t;
typedef struct gnutls_srp_client_credentials_st
*gnutls_srp_client_credentials_t;

void
gnutls_srp_free_client_credentials(gnutls_srp_client_credentials_t sc);
int
gnutls_srp_allocate_client_credentials(gnutls_srp_client_credentials_t *
				       sc);
int gnutls_srp_set_client_credentials(gnutls_srp_client_credentials_t res,
				      const char *username,
				      const char *password);

void
gnutls_srp_free_server_credentials(gnutls_srp_server_credentials_t sc);
int
gnutls_srp_allocate_server_credentials(gnutls_srp_server_credentials_t *
				       sc);
int gnutls_srp_set_server_credentials_file(gnutls_srp_server_credentials_t
					   res, const char *password_file,
					   const char *password_conf_file);

const char *gnutls_srp_server_get_username(gnutls_session_t session);

void gnutls_srp_set_prime_bits(gnutls_session_t session,
                               unsigned int bits);

int gnutls_srp_verifier(const char *username,
			const char *password,
			const gnutls_datum_t * salt,
			const gnutls_datum_t * generator,
			const gnutls_datum_t * prime,
			gnutls_datum_t * res);

/* The static parameters defined in draft-ietf-tls-srp-05
 * Those should be used as input to gnutls_srp_verifier().
 */
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_4096_group_prime;
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_4096_group_generator;

extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_3072_group_prime;
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_3072_group_generator;

extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_2048_group_prime;
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_2048_group_generator;

extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_1536_group_prime;
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_1536_group_generator;

extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_1024_group_prime;
extern _SYM_EXPORT const gnutls_datum_t gnutls_srp_1024_group_generator;

typedef int gnutls_srp_server_credentials_function(gnutls_session_t,
						   const char *username,
						   gnutls_datum_t * salt,
						   gnutls_datum_t *
						   verifier,
						   gnutls_datum_t *
						   generator,
						   gnutls_datum_t * prime);
void
gnutls_srp_set_server_credentials_function(gnutls_srp_server_credentials_t
					   cred,
					   gnutls_srp_server_credentials_function
					   * func);

typedef int gnutls_srp_client_credentials_function(gnutls_session_t,
						   char **, char **);
void
gnutls_srp_set_client_credentials_function(gnutls_srp_client_credentials_t
					   cred,
					   gnutls_srp_client_credentials_function
					   * func);

int gnutls_srp_base64_encode(const gnutls_datum_t * data, char *result,
			     size_t * result_size);
int gnutls_srp_base64_encode2(const gnutls_datum_t * data,
				   gnutls_datum_t * result);

int gnutls_srp_base64_decode(const gnutls_datum_t * b64_data, char *result,
			     size_t * result_size);
int gnutls_srp_base64_decode2(const gnutls_datum_t * b64_data,
				   gnutls_datum_t * result);

#define gnutls_srp_base64_encode_alloc gnutls_srp_base64_encode2
#define gnutls_srp_base64_decode_alloc gnutls_srp_base64_decode2

void
gnutls_srp_set_server_fake_salt_seed(gnutls_srp_server_credentials_t
				     sc,
				     const gnutls_datum_t * seed,
				     unsigned int salt_length);

/* PSK stuff */
typedef struct gnutls_psk_server_credentials_st
*gnutls_psk_server_credentials_t;
typedef struct gnutls_psk_client_credentials_st
*gnutls_psk_client_credentials_t;

/**
 * gnutls_psk_key_flags:
 * @GNUTLS_PSK_KEY_RAW: PSK-key in raw format.
 * @GNUTLS_PSK_KEY_HEX: PSK-key in hex format.
 *
 * Enumeration of different PSK key flags.
 */
typedef enum gnutls_psk_key_flags {
	GNUTLS_PSK_KEY_RAW = 0,
	GNUTLS_PSK_KEY_HEX
} gnutls_psk_key_flags;

void
gnutls_psk_free_client_credentials(gnutls_psk_client_credentials_t sc);
int
gnutls_psk_allocate_client_credentials(gnutls_psk_client_credentials_t *
				       sc);
int gnutls_psk_set_client_credentials(gnutls_psk_client_credentials_t res,
				      const char *username,
				      const gnutls_datum_t * key,
				      gnutls_psk_key_flags flags);

void
gnutls_psk_free_server_credentials(gnutls_psk_server_credentials_t sc);
int
gnutls_psk_allocate_server_credentials(gnutls_psk_server_credentials_t *
				       sc);
int gnutls_psk_set_server_credentials_file(gnutls_psk_server_credentials_t
					   res, const char *password_file);

int
gnutls_psk_set_server_credentials_hint(gnutls_psk_server_credentials_t
				       res, const char *hint);

const char *gnutls_psk_server_get_username(gnutls_session_t session);
const char *gnutls_psk_client_get_hint(gnutls_session_t session);

typedef int gnutls_psk_server_credentials_function(gnutls_session_t,
						   const char *username,
						   gnutls_datum_t * key);
void
gnutls_psk_set_server_credentials_function(gnutls_psk_server_credentials_t
					   cred,
					   gnutls_psk_server_credentials_function
					   * func);

typedef int gnutls_psk_client_credentials_function(gnutls_session_t,
						   char **username,
						   gnutls_datum_t * key);
void
gnutls_psk_set_client_credentials_function(gnutls_psk_client_credentials_t
					   cred,
					   gnutls_psk_client_credentials_function
					   * func);

int gnutls_hex_encode(const gnutls_datum_t * data, char *result,
		      size_t * result_size);
int gnutls_hex_decode(const gnutls_datum_t * hex_data, void *result,
		      size_t * result_size);

int gnutls_hex_encode2(const gnutls_datum_t * data, gnutls_datum_t *result);
int gnutls_hex_decode2(const gnutls_datum_t * data, gnutls_datum_t *result);

void
gnutls_psk_set_server_dh_params(gnutls_psk_server_credentials_t res,
				gnutls_dh_params_t dh_params);

void
gnutls_psk_set_server_params_function(gnutls_psk_server_credentials_t
				      res, gnutls_params_function * func);



typedef struct gnutls_retr2_st {
	gnutls_certificate_type_t cert_type;
	gnutls_privkey_type_t key_type;

	union {
		gnutls_x509_crt_t *x509;
		gnutls_openpgp_crt_t pgp;
	} cert;
	unsigned int ncerts;	/* one for pgp keys */

	union {
		gnutls_x509_privkey_t x509;
		gnutls_openpgp_privkey_t pgp;
		gnutls_pkcs11_privkey_t pkcs11;
	} key;

	unsigned int deinit_all;	/* if non zero all keys will be deinited */
} gnutls_retr2_st;


  /* Functions that allow auth_info_t structures handling
   */

gnutls_credentials_type_t gnutls_auth_get_type(gnutls_session_t session);
gnutls_credentials_type_t
gnutls_auth_server_get_type(gnutls_session_t session);
gnutls_credentials_type_t
gnutls_auth_client_get_type(gnutls_session_t session);

  /* DH */

void gnutls_dh_set_prime_bits(gnutls_session_t session, unsigned int bits);
int gnutls_dh_get_secret_bits(gnutls_session_t session);
int gnutls_dh_get_peers_public_bits(gnutls_session_t session);
int gnutls_dh_get_prime_bits(gnutls_session_t session);

int gnutls_dh_get_group(gnutls_session_t session, gnutls_datum_t * raw_gen,
			gnutls_datum_t * raw_prime);
int gnutls_dh_get_pubkey(gnutls_session_t session,
			 gnutls_datum_t * raw_key);

  /* X509PKI */


  /* These are set on the credentials structure.
   */

  /* use gnutls_certificate_set_retrieve_function2() in abstract.h
   * instead. It's much more efficient.
   */

typedef int gnutls_certificate_retrieve_function(gnutls_session_t,
						 const
						 gnutls_datum_t *
						 req_ca_rdn,
						 int nreqs,
						 const
						 gnutls_pk_algorithm_t
						 * pk_algos,
						 int
						 pk_algos_length,
						 gnutls_retr2_st *);


void
gnutls_certificate_set_retrieve_function(gnutls_certificate_credentials_t
					 cred,
					 gnutls_certificate_retrieve_function
					 * func);

void
gnutls_certificate_set_verify_function(gnutls_certificate_credentials_t
				       cred,
				       gnutls_certificate_verify_function
				       * func);

void
gnutls_certificate_server_set_request(gnutls_session_t session,
				      gnutls_certificate_request_t req);

  /* get data from the session
   */
const gnutls_datum_t *gnutls_certificate_get_peers(gnutls_session_t
						   session, unsigned int
						   *list_size);
const gnutls_datum_t *gnutls_certificate_get_ours(gnutls_session_t
						  session);

int gnutls_certificate_get_peers_subkey_id(gnutls_session_t session,
					   gnutls_datum_t * id);

time_t gnutls_certificate_activation_time_peers(gnutls_session_t session);
time_t gnutls_certificate_expiration_time_peers(gnutls_session_t session);

int gnutls_certificate_client_get_request_status(gnutls_session_t session);
int gnutls_certificate_verify_peers2(gnutls_session_t session,
				     unsigned int *status);
int gnutls_certificate_verify_peers3(gnutls_session_t session,
				     const char *hostname,
				     unsigned int *status);

int
gnutls_certificate_verify_peers(gnutls_session_t session,
				gnutls_typed_vdata_st * data,
				unsigned int elements,
				unsigned int *status);

int gnutls_certificate_verification_status_print(unsigned int status,
						 gnutls_certificate_type_t
						 type,
						 gnutls_datum_t * out,
						 unsigned int flags);

int gnutls_pem_base64_encode(const char *msg, const gnutls_datum_t * data,
			     char *result, size_t * result_size);
int gnutls_pem_base64_decode(const char *header,
			     const gnutls_datum_t * b64_data,
			     unsigned char *result, size_t * result_size);

int gnutls_pem_base64_encode2(const char *msg,
				   const gnutls_datum_t * data,
				   gnutls_datum_t * result);
int gnutls_pem_base64_decode2(const char *header,
				   const gnutls_datum_t * b64_data,
				   gnutls_datum_t * result);

#define gnutls_pem_base64_encode_alloc gnutls_pem_base64_encode2
#define gnutls_pem_base64_decode_alloc gnutls_pem_base64_decode2

  /* key_usage will be an OR of the following values:
   */

  /* when the key is to be used for signing: */
#define GNUTLS_KEY_DIGITAL_SIGNATURE	128
#define GNUTLS_KEY_NON_REPUDIATION	64
  /* when the key is to be used for encryption: */
#define GNUTLS_KEY_KEY_ENCIPHERMENT	32
#define GNUTLS_KEY_DATA_ENCIPHERMENT	16
#define GNUTLS_KEY_KEY_AGREEMENT	8
#define GNUTLS_KEY_KEY_CERT_SIGN	4
#define GNUTLS_KEY_CRL_SIGN		2
#define GNUTLS_KEY_ENCIPHER_ONLY	1
#define GNUTLS_KEY_DECIPHER_ONLY	32768

void
gnutls_certificate_set_params_function(gnutls_certificate_credentials_t
				       res, gnutls_params_function * func);
void gnutls_anon_set_params_function(gnutls_anon_server_credentials_t res,
				     gnutls_params_function * func);
void gnutls_psk_set_params_function(gnutls_psk_server_credentials_t res,
				    gnutls_params_function * func);

int gnutls_hex2bin(const char *hex_data, size_t hex_size,
		   void *bin_data, size_t * bin_size);

  /* Trust on first use (or ssh like) functions */

  /* stores the provided information to a database
   */
typedef int (*gnutls_tdb_store_func) (const char *db_name,
				      const char *host,
				      const char *service,
				      time_t expiration,
				      const gnutls_datum_t * pubkey);

typedef int (*gnutls_tdb_store_commitment_func) (const char *db_name,
						 const char *host,
						 const char *service,
						 time_t expiration,
						 gnutls_digest_algorithm_t
						 hash_algo,
						 const gnutls_datum_t *
						 hash);

  /* searches for the provided host/service pair that match the
   * provided public key in the database. */
typedef int (*gnutls_tdb_verify_func) (const char *db_name,
				       const char *host,
				       const char *service,
				       const gnutls_datum_t * pubkey);


struct gnutls_tdb_int;
typedef struct gnutls_tdb_int *gnutls_tdb_t;

int gnutls_tdb_init(gnutls_tdb_t * tdb);
void gnutls_tdb_set_store_func(gnutls_tdb_t tdb,
			       gnutls_tdb_store_func store);
void gnutls_tdb_set_store_commitment_func(gnutls_tdb_t tdb,
					  gnutls_tdb_store_commitment_func
					  cstore);
void gnutls_tdb_set_verify_func(gnutls_tdb_t tdb,
				gnutls_tdb_verify_func verify);
void gnutls_tdb_deinit(gnutls_tdb_t tdb);

int gnutls_verify_stored_pubkey(const char *db_name,
				gnutls_tdb_t tdb,
				const char *host,
				const char *service,
				gnutls_certificate_type_t cert_type,
				const gnutls_datum_t * cert,
				unsigned int flags);

int gnutls_store_commitment(const char *db_name,
			    gnutls_tdb_t tdb,
			    const char *host,
			    const char *service,
			    gnutls_digest_algorithm_t hash_algo,
			    const gnutls_datum_t * hash,
			    time_t expiration, unsigned int flags);

int gnutls_store_pubkey(const char *db_name,
			gnutls_tdb_t tdb,
			const char *host,
			const char *service,
			gnutls_certificate_type_t cert_type,
			const gnutls_datum_t * cert,
			time_t expiration, unsigned int flags);

  /* Other helper functions */
int gnutls_load_file(const char *filename, gnutls_datum_t * data);

int gnutls_url_is_supported(const char *url);

  /* PIN callback */

/**
 * gnutls_pin_flag_t:
 * @GNUTLS_PIN_USER: The PIN for the user.
 * @GNUTLS_PIN_SO: The PIN for the security officer (admin).
 * @GNUTLS_PIN_CONTEXT_SPECIFIC: The PIN is for a specific action and key like signing.
 * @GNUTLS_PIN_FINAL_TRY: This is the final try before blocking.
 * @GNUTLS_PIN_COUNT_LOW: Few tries remain before token blocks.
 * @GNUTLS_PIN_WRONG: Last given PIN was not correct.
 *
 * Enumeration of different flags that are input to the PIN function.
 */
typedef enum {
	GNUTLS_PIN_USER = (1 << 0),
	GNUTLS_PIN_SO = (1 << 1),
	GNUTLS_PIN_FINAL_TRY = (1 << 2),
	GNUTLS_PIN_COUNT_LOW = (1 << 3),
	GNUTLS_PIN_CONTEXT_SPECIFIC = (1 << 4),
	GNUTLS_PIN_WRONG = (1 << 5)
} gnutls_pin_flag_t;

#define GNUTLS_PKCS11_PIN_USER GNUTLS_PIN_USER
#define GNUTLS_PKCS11_PIN_SO GNUTLS_PIN_SO
#define GNUTLS_PKCS11_PIN_FINAL_TRY GNUTLS_PIN_FINAL_TRY
#define GNUTLS_PKCS11_PIN_COUNT_LOW  GNUTLS_PIN_COUNT_LOW
#define GNUTLS_PKCS11_PIN_CONTEXT_SPECIFIC GNUTLS_PIN_CONTEXT_SPECIFIC
#define GNUTLS_PKCS11_PIN_WRONG GNUTLS_PIN_WRONG

/**
 * gnutls_pin_callback_t:
 * @userdata: user-controlled data from gnutls_pkcs11_set_pin_function().
 * @attempt: pin-attempt counter, initially 0.
 * @token_url: URL of token.
 * @token_label: label of token.
 * @flags: a #gnutls_pin_flag_t flag.
 * @pin: buffer to hold PIN, of size @pin_max.
 * @pin_max: size of @pin buffer.
 *
 * Callback function type for PKCS#11 or TPM PIN entry.  It is set by
 * functions like gnutls_pkcs11_set_pin_function().
 *
 * The callback should provides the PIN code to unlock the token with
 * label @token_label, specified by the URL @token_url.
 *
 * The PIN code, as a NUL-terminated ASCII string, should be copied
 * into the @pin buffer (of maximum size @pin_max), and return 0 to
 * indicate success.  Alternatively, the callback may return a
 * negative gnutls error code to indicate failure and cancel PIN entry
 * (in which case, the contents of the @pin parameter are ignored).
 *
 * When a PIN is required, the callback will be invoked repeatedly
 * (and indefinitely) until either the returned PIN code is correct,
 * the callback returns failure, or the token refuses login (e.g. when
 * the token is locked due to too many incorrect PINs!).  For the
 * first such invocation, the @attempt counter will have value zero;
 * it will increase by one for each subsequent attempt.
 *
 * Returns: %GNUTLS_E_SUCCESS (0) on success or a negative error code on error.
 *
 * Since: 2.12.0
 **/

#include <libgnu/config_libgnu.h>
#include <libgnu/abstract_gnu.h>
#include <libgnu/x509/gnutls_common.h>

void gnutls_certificate_set_pin_function(gnutls_certificate_credentials_t,
					 gnutls_pin_callback_t fn,
					 void *userdata);

/* Public string related functions */
typedef struct gnutls_buffer_st *gnutls_buffer_t;

int gnutls_buffer_append_data(gnutls_buffer_t, const void *data, size_t data_size);

/* Public extensions related functions */


void gnutls_ext_set_data(gnutls_session_t session, unsigned type,
			 gnutls_ext_priv_data_t);
int gnutls_ext_get_data(gnutls_session_t session, unsigned type,
			gnutls_ext_priv_data_t *);

typedef int (*gnutls_ext_recv_func) (gnutls_session_t session,
				     const unsigned char *data,
				     size_t len);

typedef int (*gnutls_ext_send_func) (gnutls_session_t session,
				     gnutls_buffer_t extdata);

typedef void (*gnutls_ext_deinit_data_func) (gnutls_ext_priv_data_t data);

typedef int (*gnutls_ext_pack_func) (gnutls_ext_priv_data_t data,
				     gnutls_buffer_t packed_data);

typedef int (*gnutls_ext_unpack_func) (gnutls_buffer_t packed_data,
				       gnutls_ext_priv_data_t *data);


/**
 * gnutls_ext_parse_type_t:
 * @GNUTLS_EXT_NONE: Never parsed
 * @GNUTLS_EXT_ANY: Any extension type.
 * @GNUTLS_EXT_APPLICATION: Application extension.
 * @GNUTLS_EXT_TLS: TLS-internal extension.
 * @GNUTLS_EXT_MANDATORY: Extension parsed even if resuming (or extensions are disabled).
 *
 * Enumeration of different TLS extension types.  This flag
 * indicates for an extension whether it is useful to application
 * level or TLS level only.  This is (only) used to parse the
 * application level extensions before the "client_hello" callback
 * is called.
 */
typedef enum {
  GNUTLS_EXT_ANY = 0,
  GNUTLS_EXT_APPLICATION = 1,
  GNUTLS_EXT_TLS = 2,
  GNUTLS_EXT_MANDATORY = 3,
  GNUTLS_EXT_NONE = 4
} gnutls_ext_parse_type_t;

	/* Register a custom tls extension
	 */
int gnutls_ext_register(const char *name, int type, gnutls_ext_parse_type_t parse_type,
				gnutls_ext_recv_func recv_func, gnutls_ext_send_func send_func, 
				gnutls_ext_deinit_data_func deinit_func, gnutls_ext_pack_func pack_func,
				gnutls_ext_unpack_func unpack_func);

/* Public supplemental data related functions */

typedef int (*gnutls_supp_recv_func) (gnutls_session_t session,
			       const unsigned char * data, size_t data_size);
typedef int (*gnutls_supp_send_func) (gnutls_session_t session,
			       gnutls_buffer_t buf);

int gnutls_supplemental_register(const char *name, 
				gnutls_supplemental_data_format_type_t type, 
				gnutls_supp_recv_func supp_recv_func,
				gnutls_supp_send_func supp_send_func);

void gnutls_supplemental_recv(gnutls_session_t session, unsigned do_recv_supplemental);

void gnutls_supplemental_send(gnutls_session_t session, unsigned do_send_supplemental);

/* FIPS140-2 related functions */
int gnutls_fips140_mode_enabled(void);


//#include <x509/x509_int.h>
//#include <fips.h>

#define MAX_STRING_LEN 512

#define GNUTLS_XML_SHOW_ALL 1

#define PEM_CRL "X509 CRL"
#define PEM_X509_CERT "X509 CERTIFICATE"
#define PEM_X509_CERT2 "CERTIFICATE"
#define PEM_PKCS7 "PKCS7"
#define PEM_PKCS12 "PKCS12"
#define PEM_PK "PUBLIC KEY"

/* public key algorithm's OIDs
*/
#define PK_PKIX1_RSA_OID "1.2.840.113549.1.1.1"
#define PK_X509_RSA_OID "2.5.8.1.1"
#define PK_DSA_OID "1.2.840.10040.4.1"
#define PK_GOST_R3410_94_OID "1.2.643.2.2.20"
#define PK_GOST_R3410_2001_OID "1.2.643.2.2.19"

/* signature OIDs
*/
#define SIG_DSA_SHA1_OID "1.2.840.10040.4.3"
/* those two from draft-ietf-pkix-sha2-dsa-ecdsa-06 */
#define SIG_DSA_SHA224_OID "2.16.840.1.101.3.4.3.1"
#define SIG_DSA_SHA256_OID "2.16.840.1.101.3.4.3.2"
#define SIG_DSA_SHA384_OID "2.16.840.1.101.3.4.3.3"
#define SIG_DSA_SHA512_OID "2.16.840.1.101.3.4.3.4"

#define SIG_RSA_MD5_OID "1.2.840.113549.1.1.4"
#define SIG_RSA_MD2_OID "1.2.840.113549.1.1.2"
#define SIG_RSA_SHA1_OID "1.2.840.113549.1.1.5"
#define SIG_RSA_SHA224_OID "1.2.840.113549.1.1.14"
#define SIG_RSA_SHA256_OID "1.2.840.113549.1.1.11"
#define SIG_RSA_SHA384_OID "1.2.840.113549.1.1.12"
#define SIG_RSA_SHA512_OID "1.2.840.113549.1.1.13"
#define SIG_RSA_RMD160_OID "1.3.36.3.3.1.2"
#define SIG_GOST_R3410_94_OID "1.2.643.2.2.4"
#define SIG_GOST_R3410_2001_OID "1.2.643.2.2.3"
#define ISO_SIG_RSA_SHA1_OID "1.3.14.3.2.29"

#define XMPP_OID "1.3.6.1.5.5.7.8.5"

#define ASN1_NULL "\x05\x00"
#define ASN1_NULL_SIZE 2



int _gnutls_x509_set_time( ASN1_TYPE c2, const char *where, time_t tim,
						   int force_general );
int
_gnutls_x509_set_raw_time( ASN1_TYPE c2, const char *where, time_t tim );

int _gnutls_x509_decode_string( unsigned int etype,
								const uint8_t * der, size_t der_size,
								gnutls_datum_t * output,
								unsigned allow_ber );

int _gnutls_x509_encode_string( unsigned int etype,
								const void *input_data, size_t input_size,
								gnutls_datum_t * output );

int _gnutls_x509_dn_to_string( const char *OID, void *value,
							   int value_size, gnutls_datum_t * out );
const char *_gnutls_ldap_string_to_oid( const char *str, unsigned str_len );

time_t _gnutls_x509_get_time( ASN1_TYPE c2, const char *when, int general );

gnutls_x509_subject_alt_name_t _gnutls_x509_san_find_type( char *str_type );

int _gnutls_x509_der_encode_and_copy( ASN1_TYPE src, const char *src_name,
									  ASN1_TYPE dest, const char *dest_name,
									  int str );
int _gnutls_x509_der_encode( ASN1_TYPE src, const char *src_name,
							 gnutls_datum_t * res, int str );

#define _gnutls_x509_export_int(asn1, format, header, out, out_size) \
  _gnutls_x509_export_int_named(asn1, "", format, header, out, out_size)

int _gnutls_x509_export_int_named( ASN1_TYPE asn1_data, const char *name,
								   gnutls_x509_crt_fmt_t format,
								   const char *pem_header,
								   unsigned char *output_data,
								   size_t * output_data_size );

#define _gnutls_x509_export_int2(asn1, format, header, out) \
  _gnutls_x509_export_int_named2(asn1, "", format, header, out)
int _gnutls_x509_export_int_named2( ASN1_TYPE asn1_data, const char *name,
									gnutls_x509_crt_fmt_t format,
									const char *pem_header,
									gnutls_datum_t * out );

int _gnutls_x509_read_value( ASN1_TYPE c, const char *root,
							 gnutls_datum_t * ret );
int _gnutls_x509_read_null_value( ASN1_TYPE c, const char *root,
								  gnutls_datum_t * ret );
int _gnutls_x509_read_string( ASN1_TYPE c, const char *root,
							  gnutls_datum_t * ret, unsigned int etype,
							  unsigned allow_ber );
int _gnutls_x509_write_value( ASN1_TYPE c, const char *root,
							  const gnutls_datum_t * data );

int _gnutls_x509_write_string( ASN1_TYPE c, const char *root,
							   const gnutls_datum_t * data,
							   unsigned int etype );

int _gnutls_x509_encode_and_write_attribute( const char *given_oid,
											 ASN1_TYPE asn1_struct,
											 const char *where,
											 const void *data,
											 int sizeof_data, int multi );
int _gnutls_x509_decode_and_read_attribute( ASN1_TYPE asn1_struct,
											const char *where, char *oid,
											int oid_size,
											gnutls_datum_t * value,
											int multi, int octet );

int _gnutls_x509_get_pk_algorithm( ASN1_TYPE src, const char *src_name,
								   unsigned int *bits );

int
_gnutls_x509_get_signature_algorithm( ASN1_TYPE src, const char *src_name );

int _gnutls_x509_encode_and_copy_PKI_params( ASN1_TYPE dst,
											 const char *dst_name,
											 gnutls_pk_algorithm_t
											 pk_algorithm,
											 gnutls_pk_params_st * params );
int _gnutls_x509_encode_PKI_params( gnutls_datum_t * der,
									gnutls_pk_algorithm_t,
									gnutls_pk_params_st * params );
int _gnutls_asn1_copy_node( ASN1_TYPE * dst, const char *dst_name,
							ASN1_TYPE src, const char *src_name );

int _gnutls_x509_get_signed_data( ASN1_TYPE src, const gnutls_datum_t *der,
								  const char *src_name,
								  gnutls_datum_t * signed_data );
int _gnutls_x509_get_signature( ASN1_TYPE src, const char *src_name,
								gnutls_datum_t * signature );


int _gnutls_get_asn_mpis( ASN1_TYPE asn, const char *root,
						  gnutls_pk_params_st * params );

int _gnutls_get_key_id( gnutls_pk_algorithm_t pk, gnutls_pk_params_st *,
						unsigned char *output_data,
						size_t * output_data_size, unsigned flags );

void _asnstr_append_name( char *name, size_t name_size, const char *part1,
						  const char *part2 );

/* Given a @c2 which it returns an allocated DER encoding of @whom in @out */
NLC_INLINE static int _gnutls_x509_get_raw_field( ASN1_TYPE c2, const char *whom, gnutls_datum_t *out )
{
	return _gnutls_x509_der_encode( c2, whom, out, 0 );
}

int
_gnutls_x509_get_raw_field2( ASN1_TYPE c2, gnutls_datum_t * raw,
							 const char *whom, gnutls_datum_t * dn );

bool
_gnutls_check_if_same_key( gnutls_x509_crt_t cert1,
						   gnutls_x509_crt_t cert2,
						   unsigned is_ca );

bool
_gnutls_check_if_same_key2( gnutls_x509_crt_t cert1,
							gnutls_datum_t *cert2bin );

bool
_gnutls_check_valid_key_id( gnutls_datum_t *key_id,
							gnutls_x509_crt_t cert, time_t now );

bool
_gnutls_check_if_same_cert( gnutls_x509_crt_t cert1,
							gnutls_x509_crt_t cert2 );

bool
_gnutls_check_if_same_cert2( gnutls_x509_crt_t cert1,
							 gnutls_datum_t * cert2bin );

bool _gnutls_check_key_purpose( gnutls_x509_crt_t cert, const char *purpose, unsigned no_any );

time_t _gnutls_x509_generalTime2gtime( const char *ttime );

int _gnutls_get_extension( ASN1_TYPE asn, const char *root,
						   const char *extension_id, int indx,
						   gnutls_datum_t * ret, unsigned int *_critical );

int _gnutls_set_extension( ASN1_TYPE asn, const char *root,
						   const char *ext_id,
						   const gnutls_datum_t * ext_data, unsigned int critical );

int _gnutls_strdatum_to_buf( gnutls_datum_t * d, void *buf,
							 size_t * sizeof_buf );

bool _gnutls_is_same_dn( gnutls_x509_crt_t cert1, gnutls_x509_crt_t cert2 );

int _gnutls_copy_string( gnutls_datum_t* str, uint8_t *out, size_t *out_size );
int _gnutls_copy_data( gnutls_datum_t* str, uint8_t *out, size_t *out_size );

int _san_othername_to_virtual( const char *oid, size_t oid_size );

int _gnutls_x509_decode_ext( const gnutls_datum_t *der, gnutls_x509_ext_st *out );
int x509_raw_crt_to_raw_pubkey( const gnutls_datum_t * cert,
								gnutls_datum_t * rpubkey );

int x509_crt_to_raw_pubkey( gnutls_x509_crt_t crt,
							gnutls_datum_t * rpubkey );

typedef void( *gnutls_cert_vfunc )( gnutls_x509_crt_t );

gnutls_x509_crt_t *_gnutls_sort_clist( gnutls_x509_crt_t
									   sorted[ DEFAULT_MAX_VERIFY_DEPTH ],
									   gnutls_x509_crt_t * clist,
									   unsigned int *clist_size,
									   gnutls_cert_vfunc func );

int _gnutls_check_if_sorted( gnutls_x509_crt_t * crt, int nr );

NLC_INLINE static int _asn1_strict_der_decode( asn1_node * element, const void *ider,
												int len, char *errorDescription )
{
	return asn1_der_decoding2( element, ider, &len, ASN1_DECODE_FLAG_STRICT_DER, errorDescription );
}

#include "tpm.h"
int
gnutls_tpm_privkey_generate( gnutls_pk_algorithm_t pk,
							 unsigned int bits,
							 const char *srk_password,
							 const char *key_password,
							 gnutls_tpmkey_fmt_t format,
							 gnutls_x509_crt_fmt_t pub_format,
							 gnutls_datum_t * privkey,
							 gnutls_datum_t * pubkey, unsigned int flags );

NLC_END_CDECLARES

#include <libgnu/compat.h>

