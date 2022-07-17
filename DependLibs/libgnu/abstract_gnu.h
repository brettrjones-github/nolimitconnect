/*
 * Copyright (C) 2010-2012 Free Software Foundation, Inc.
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

#ifndef ABSTRACT_GNU_H
#define ABSTRACT_GNU_H	1


#include <libgnu/config_libgnu.h>


#define GNUTLS_MAX_PK_PARAMS 16
#define MAX_PRIV_PARAMS_SIZE GNUTLS_MAX_PK_PARAMS	/* ok for RSA and DSA */
#define GNUTLS_TL_VERIFY_CRL 1
#define GNUTLS_TL_USE_IN_TLS (1<<1)
#define GNUTLS_TL_NO_DUPLICATES (1<<2)
#define GNUTLS_TL_NO_DUPLICATE_KEY (1<<3)
#define GNUTLS_TL_GET_COPY (1<<4)

NLC_BEGIN_CDECLARES

/* Gnutls error codes. The mapping to a TLS alert is also shown in
* comments.
*/

#define GNUTLS_E_SUCCESS 0
#define	GNUTLS_E_UNKNOWN_COMPRESSION_ALGORITHM -3
#define	GNUTLS_E_UNKNOWN_CIPHER_TYPE -6
#define	GNUTLS_E_LARGE_PACKET -7
#define GNUTLS_E_UNSUPPORTED_VERSION_PACKET -8	/* GNUTLS_A_PROTOCOL_VERSION */
#define GNUTLS_E_UNEXPECTED_PACKET_LENGTH -9	/* GNUTLS_A_RECORD_OVERFLOW */
#define GNUTLS_E_INVALID_SESSION -10
#define GNUTLS_E_FATAL_ALERT_RECEIVED -12
#define GNUTLS_E_UNEXPECTED_PACKET -15	/* GNUTLS_A_UNEXPECTED_MESSAGE */
#define GNUTLS_E_WARNING_ALERT_RECEIVED -16
#define GNUTLS_E_ERROR_IN_FINISHED_PACKET -18
#define GNUTLS_E_UNEXPECTED_HANDSHAKE_PACKET -19
#define	GNUTLS_E_UNKNOWN_CIPHER_SUITE -21	/* GNUTLS_A_HANDSHAKE_FAILURE */
#define	GNUTLS_E_UNWANTED_ALGORITHM -22
#define	GNUTLS_E_MPI_SCAN_FAILED -23
#define GNUTLS_E_DECRYPTION_FAILED -24	/* GNUTLS_A_DECRYPTION_FAILED, GNUTLS_A_BAD_RECORD_MAC */
#define GNUTLS_E_MEMORY_ERROR -25
#define GNUTLS_E_DECOMPRESSION_FAILED -26	/* GNUTLS_A_DECOMPRESSION_FAILURE */
#define GNUTLS_E_COMPRESSION_FAILED -27
#define GNUTLS_E_AGAIN -28
#define GNUTLS_E_EXPIRED -29
#define GNUTLS_E_DB_ERROR -30
#define GNUTLS_E_SRP_PWD_ERROR -31
#define GNUTLS_E_INSUFFICIENT_CREDENTIALS -32
#define GNUTLS_E_INSUFICIENT_CREDENTIALS GNUTLS_E_INSUFFICIENT_CREDENTIALS	/* for backwards compatibility only */
#define GNUTLS_E_INSUFFICIENT_CRED GNUTLS_E_INSUFFICIENT_CREDENTIALS
#define GNUTLS_E_INSUFICIENT_CRED GNUTLS_E_INSUFFICIENT_CREDENTIALS	/* for backwards compatibility only */

#define GNUTLS_E_HASH_FAILED -33
#define GNUTLS_E_BASE64_DECODING_ERROR -34

#define	GNUTLS_E_MPI_PRINT_FAILED -35
#define GNUTLS_E_REHANDSHAKE -37	/* GNUTLS_A_NO_RENEGOTIATION */
#define GNUTLS_E_GOT_APPLICATION_DATA -38
#define GNUTLS_E_RECORD_LIMIT_REACHED -39
#define GNUTLS_E_ENCRYPTION_FAILED -40

#define GNUTLS_E_PK_ENCRYPTION_FAILED -44
#define GNUTLS_E_PK_DECRYPTION_FAILED -45
#define GNUTLS_E_PK_SIGN_FAILED -46
#define GNUTLS_E_X509_UNSUPPORTED_CRITICAL_EXTENSION -47
#define GNUTLS_E_KEY_USAGE_VIOLATION -48
#define GNUTLS_E_NO_CERTIFICATE_FOUND -49	/* GNUTLS_A_BAD_CERTIFICATE */
#define GNUTLS_E_INVALID_REQUEST -50
#define GNUTLS_E_SHORT_MEMORY_BUFFER -51
#define GNUTLS_E_INTERRUPTED -52
#define GNUTLS_E_PUSH_ERROR -53
#define GNUTLS_E_PULL_ERROR -54
#define GNUTLS_E_RECEIVED_ILLEGAL_PARAMETER -55	/* GNUTLS_A_ILLEGAL_PARAMETER */
#define GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE -56
#define GNUTLS_E_PKCS1_WRONG_PAD -57
#define GNUTLS_E_RECEIVED_ILLEGAL_EXTENSION -58
#define GNUTLS_E_INTERNAL_ERROR -59
#define GNUTLS_E_DH_PRIME_UNACCEPTABLE -63
#define GNUTLS_E_FILE_ERROR -64
#define GNUTLS_E_TOO_MANY_EMPTY_PACKETS -78
#define GNUTLS_E_UNKNOWN_PK_ALGORITHM -80
#define GNUTLS_E_TOO_MANY_HANDSHAKE_PACKETS -81

/* returned if you need to generate temporary RSA
* parameters. These are needed for export cipher suites.
*/
#define GNUTLS_E_NO_TEMPORARY_RSA_PARAMS -84

#define GNUTLS_E_NO_COMPRESSION_ALGORITHMS -86
#define GNUTLS_E_NO_CIPHER_SUITES -87

#define GNUTLS_E_OPENPGP_GETKEY_FAILED -88
#define GNUTLS_E_PK_SIG_VERIFY_FAILED -89

#define GNUTLS_E_ILLEGAL_SRP_USERNAME -90
#define GNUTLS_E_SRP_PWD_PARSING_ERROR -91
#define GNUTLS_E_NO_TEMPORARY_DH_PARAMS -93

/* For certificate and key stuff
*/
#define GNUTLS_E_ASN1_ELEMENT_NOT_FOUND -67
#define GNUTLS_E_ASN1_IDENTIFIER_NOT_FOUND -68
#define GNUTLS_E_ASN1_DER_ERROR -69
#define GNUTLS_E_ASN1_VALUE_NOT_FOUND -70
#define GNUTLS_E_ASN1_GENERIC_ERROR -71
#define GNUTLS_E_ASN1_VALUE_NOT_VALID -72
#define GNUTLS_E_ASN1_TAG_ERROR -73
#define GNUTLS_E_ASN1_TAG_IMPLICIT -74
#define GNUTLS_E_ASN1_TYPE_ANY_ERROR -75
#define GNUTLS_E_ASN1_SYNTAX_ERROR -76
#define GNUTLS_E_ASN1_DER_OVERFLOW -77
#define GNUTLS_E_OPENPGP_UID_REVOKED -79
#define GNUTLS_E_CERTIFICATE_ERROR -43
#define GNUTLS_E_X509_CERTIFICATE_ERROR GNUTLS_E_CERTIFICATE_ERROR
#define GNUTLS_E_CERTIFICATE_KEY_MISMATCH -60
#define GNUTLS_E_UNSUPPORTED_CERTIFICATE_TYPE -61	/* GNUTLS_A_UNSUPPORTED_CERTIFICATE */
#define GNUTLS_E_X509_UNKNOWN_SAN -62
#define GNUTLS_E_OPENPGP_FINGERPRINT_UNSUPPORTED -94
#define GNUTLS_E_X509_UNSUPPORTED_ATTRIBUTE -95
#define GNUTLS_E_UNKNOWN_HASH_ALGORITHM -96
#define GNUTLS_E_UNKNOWN_PKCS_CONTENT_TYPE -97
#define GNUTLS_E_UNKNOWN_PKCS_BAG_TYPE -98
#define GNUTLS_E_INVALID_PASSWORD -99
#define GNUTLS_E_MAC_VERIFY_FAILED -100	/* for PKCS #12 MAC */
#define GNUTLS_E_CONSTRAINT_ERROR -101

#define GNUTLS_E_WARNING_IA_IPHF_RECEIVED -102
#define GNUTLS_E_WARNING_IA_FPHF_RECEIVED -103

#define GNUTLS_E_IA_VERIFY_FAILED -104
#define GNUTLS_E_UNKNOWN_ALGORITHM -105
#define GNUTLS_E_UNSUPPORTED_SIGNATURE_ALGORITHM -106
#define GNUTLS_E_SAFE_RENEGOTIATION_FAILED -107
#define GNUTLS_E_UNSAFE_RENEGOTIATION_DENIED -108
#define GNUTLS_E_UNKNOWN_SRP_USERNAME -109
#define GNUTLS_E_PREMATURE_TERMINATION -110

#define GNUTLS_E_BASE64_ENCODING_ERROR -201
#define GNUTLS_E_INCOMPATIBLE_GCRYPT_LIBRARY -202	/* obsolete */
#define GNUTLS_E_INCOMPATIBLE_CRYPTO_LIBRARY -202
#define GNUTLS_E_INCOMPATIBLE_LIBTASN1_LIBRARY -203

#define GNUTLS_E_OPENPGP_KEYRING_ERROR -204
#define GNUTLS_E_X509_UNSUPPORTED_OID -205

#define GNUTLS_E_RANDOM_FAILED -206
#define GNUTLS_E_BASE64_UNEXPECTED_HEADER_ERROR -207

#define GNUTLS_E_OPENPGP_SUBKEY_ERROR -208

#define GNUTLS_E_CRYPTO_ALREADY_REGISTERED GNUTLS_E_ALREADY_REGISTERED
#define GNUTLS_E_ALREADY_REGISTERED -209

#define GNUTLS_E_HANDSHAKE_TOO_LARGE -210

#define GNUTLS_E_CRYPTODEV_IOCTL_ERROR -211
#define GNUTLS_E_CRYPTODEV_DEVICE_ERROR -212

#define GNUTLS_E_CHANNEL_BINDING_NOT_AVAILABLE -213
#define GNUTLS_E_BAD_COOKIE -214
#define GNUTLS_E_OPENPGP_PREFERRED_KEY_ERROR -215
#define GNUTLS_E_INCOMPAT_DSA_KEY_WITH_TLS_PROTOCOL -216
#define GNUTLS_E_INSUFFICIENT_SECURITY -217

#define GNUTLS_E_HEARTBEAT_PONG_RECEIVED -292
#define GNUTLS_E_HEARTBEAT_PING_RECEIVED -293

/* PKCS11 related */
#define GNUTLS_E_PKCS11_ERROR -300
#define GNUTLS_E_PKCS11_LOAD_ERROR -301
#define GNUTLS_E_PARSING_ERROR -302
#define GNUTLS_E_PKCS11_PIN_ERROR -303

#define GNUTLS_E_PKCS11_SLOT_ERROR -305
#define GNUTLS_E_LOCKING_ERROR -306
#define GNUTLS_E_PKCS11_ATTRIBUTE_ERROR -307
#define GNUTLS_E_PKCS11_DEVICE_ERROR -308
#define GNUTLS_E_PKCS11_DATA_ERROR -309
#define GNUTLS_E_PKCS11_UNSUPPORTED_FEATURE_ERROR -310
#define GNUTLS_E_PKCS11_KEY_ERROR -311
#define GNUTLS_E_PKCS11_PIN_EXPIRED -312
#define GNUTLS_E_PKCS11_PIN_LOCKED -313
#define GNUTLS_E_PKCS11_SESSION_ERROR -314
#define GNUTLS_E_PKCS11_SIGNATURE_ERROR -315
#define GNUTLS_E_PKCS11_TOKEN_ERROR -316
#define GNUTLS_E_PKCS11_USER_ERROR -317

#define GNUTLS_E_CRYPTO_INIT_FAILED -318
#define GNUTLS_E_TIMEDOUT -319
#define GNUTLS_E_USER_ERROR -320
#define GNUTLS_E_ECC_NO_SUPPORTED_CURVES -321
#define GNUTLS_E_ECC_UNSUPPORTED_CURVE -322
#define GNUTLS_E_PKCS11_REQUESTED_OBJECT_NOT_AVAILBLE -323
#define GNUTLS_E_CERTIFICATE_LIST_UNSORTED -324
#define GNUTLS_E_ILLEGAL_PARAMETER -325
#define GNUTLS_E_NO_PRIORITIES_WERE_SET -326
#define GNUTLS_E_X509_UNSUPPORTED_EXTENSION -327
#define GNUTLS_E_SESSION_EOF -328

#define GNUTLS_E_TPM_ERROR -329
#define GNUTLS_E_TPM_KEY_PASSWORD_ERROR -330
#define GNUTLS_E_TPM_SRK_PASSWORD_ERROR -331
#define GNUTLS_E_TPM_SESSION_ERROR -332
#define GNUTLS_E_TPM_KEY_NOT_FOUND -333
#define GNUTLS_E_TPM_UNINITIALIZED -334
#define GNUTLS_E_TPM_NO_LIB -335

#define GNUTLS_E_NO_CERTIFICATE_STATUS -340
#define GNUTLS_E_OCSP_RESPONSE_ERROR -341
#define GNUTLS_E_RANDOM_DEVICE_ERROR -342
#define GNUTLS_E_AUTH_ERROR -343
#define GNUTLS_E_NO_APPLICATION_PROTOCOL -344
#define GNUTLS_E_SOCKETS_INIT_ERROR -345
#define GNUTLS_E_KEY_IMPORT_FAILED -346
#define GNUTLS_E_INAPPROPRIATE_FALLBACK -347 /*GNUTLS_A_INAPPROPRIATE_FALLBACK*/
#define GNUTLS_E_CERTIFICATE_VERIFICATION_ERROR -348

#define GNUTLS_E_SELF_TEST_ERROR -400
#define GNUTLS_E_NO_SELF_TEST -401
#define GNUTLS_E_LIB_IN_ERROR_STATE -402
#define GNUTLS_E_PK_GENERATION_ERROR -403
#define GNUTLS_E_IDNA_ERROR -404

#define GNUTLS_E_NEED_FALLBACK -405

#define GNUTLS_E_UNIMPLEMENTED_FEATURE -1250

#define GNUTLS_E_APPLICATION_ERROR_MAX -65000
#define GNUTLS_E_APPLICATION_ERROR_MIN -65500

/**
* gnutls_compression_method_t:
* @GNUTLS_COMP_UNKNOWN: Unknown compression method.
* @GNUTLS_COMP_NULL: The NULL compression method (no compression).
* @GNUTLS_COMP_DEFLATE: The DEFLATE compression method from zlib.
* @GNUTLS_COMP_ZLIB: Same as %GNUTLS_COMP_DEFLATE.
*
* Enumeration of different TLS compression methods.
*/
typedef enum {
	GNUTLS_COMP_UNKNOWN = 0,
	GNUTLS_COMP_NULL = 1,
	GNUTLS_COMP_DEFLATE = 2,
	GNUTLS_COMP_ZLIB = GNUTLS_COMP_DEFLATE
} gnutls_compression_method_t;


 /**
 * gnutls_protocol_t:
 * @GNUTLS_SSL3: SSL version 3.0.
 * @GNUTLS_TLS1_0: TLS version 1.0.
 * @GNUTLS_TLS1: Same as %GNUTLS_TLS1_0.
 * @GNUTLS_TLS1_1: TLS version 1.1.
 * @GNUTLS_TLS1_2: TLS version 1.2.
 * @GNUTLS_DTLS1_0: DTLS version 1.0.
 * @GNUTLS_DTLS1_2: DTLS version 1.2.
 * @GNUTLS_DTLS0_9: DTLS version 0.9 (Cisco AnyConnect / OpenSSL 0.9.8e).
 * @GNUTLS_VERSION_MAX: Maps to the highest supported TLS version.
 * @GNUTLS_VERSION_UNKNOWN: Unknown SSL/TLS version.
 *
 * Enumeration of different SSL/TLS protocol versions.
 */
typedef enum {
	GNUTLS_SSL3 = 1,
	GNUTLS_TLS1_0 = 2,
	GNUTLS_TLS1 = GNUTLS_TLS1_0,
	GNUTLS_TLS1_1 = 3,
	GNUTLS_TLS1_2 = 4,

	GNUTLS_DTLS0_9 = 200,
	GNUTLS_DTLS1_0 = 201,	/* 201 */
	GNUTLS_DTLS1_2 = 202,
	GNUTLS_DTLS_VERSION_MIN = GNUTLS_DTLS0_9,
	GNUTLS_DTLS_VERSION_MAX = GNUTLS_DTLS1_2,
	GNUTLS_TLS_VERSION_MAX = GNUTLS_TLS1_2,
	GNUTLS_VERSION_UNKNOWN = 0xff	/* change it to 0xffff */
} gnutls_protocol_t;

/**
* gnutls_sec_param_t:
* @GNUTLS_SEC_PARAM_UNKNOWN: Cannot be known
* @GNUTLS_SEC_PARAM_INSECURE: Less than 42 bits of security
* @GNUTLS_SEC_PARAM_EXPORT: 42 bits of security
* @GNUTLS_SEC_PARAM_VERY_WEAK: 64 bits of security
* @GNUTLS_SEC_PARAM_WEAK: 72 bits of security
* @GNUTLS_SEC_PARAM_LOW: 80 bits of security
* @GNUTLS_SEC_PARAM_LEGACY: 96 bits of security
* @GNUTLS_SEC_PARAM_MEDIUM: 112 bits of security (used to be %GNUTLS_SEC_PARAM_NORMAL)
* @GNUTLS_SEC_PARAM_HIGH: 128 bits of security
* @GNUTLS_SEC_PARAM_ULTRA: 192 bits of security
* @GNUTLS_SEC_PARAM_FUTURE: 256 bits of security
*
* Enumeration of security parameters for passive attacks.
*/
typedef enum {
	GNUTLS_SEC_PARAM_UNKNOWN = 0,
	GNUTLS_SEC_PARAM_INSECURE = 5,
	GNUTLS_SEC_PARAM_EXPORT = 10,
	GNUTLS_SEC_PARAM_VERY_WEAK = 15,
	GNUTLS_SEC_PARAM_WEAK = 20,
	GNUTLS_SEC_PARAM_LOW = 25,
	GNUTLS_SEC_PARAM_LEGACY = 30,
	GNUTLS_SEC_PARAM_MEDIUM = 35,
	GNUTLS_SEC_PARAM_HIGH = 40,
	GNUTLS_SEC_PARAM_ULTRA = 45,
	GNUTLS_SEC_PARAM_FUTURE = 50
} gnutls_sec_param_t;

/**
* gnutls_x509_subject_alt_name_t:
* @GNUTLS_SAN_DNSNAME: DNS-name SAN.
* @GNUTLS_SAN_RFC822NAME: E-mail address SAN.
* @GNUTLS_SAN_URI: URI SAN.
* @GNUTLS_SAN_IPADDRESS: IP address SAN.
* @GNUTLS_SAN_OTHERNAME: OtherName SAN.
* @GNUTLS_SAN_DN: DN SAN.
* @GNUTLS_SAN_OTHERNAME_XMPP: Virtual SAN, used by
*   gnutls_x509_crt_get_subject_alt_othername_oid.
*
* Enumeration of different subject alternative names types.
*/
typedef enum gnutls_x509_subject_alt_name_t {
	GNUTLS_SAN_DNSNAME = 1,
	GNUTLS_SAN_RFC822NAME = 2,
	GNUTLS_SAN_URI = 3,
	GNUTLS_SAN_IPADDRESS = 4,
	GNUTLS_SAN_OTHERNAME = 5,
	GNUTLS_SAN_DN = 6,
	/* The following are "virtual" subject alternative name types, in
	that they are represented by an otherName value and an OID.
	Used by gnutls_x509_crt_get_subject_alt_othername_oid.  */
	GNUTLS_SAN_OTHERNAME_XMPP = 1000
} gnutls_x509_subject_alt_name_t;

#define GNUTLS_MAC_SHA GNUTLS_MAC_SHA1
#define GNUTLS_DIG_SHA GNUTLS_DIG_SHA1

/**
* gnutls_mac_algorithm_t:
* @GNUTLS_MAC_UNKNOWN: Unknown MAC algorithm.
* @GNUTLS_MAC_NULL: NULL MAC algorithm (empty output).
* @GNUTLS_MAC_MD5: HMAC-MD5 algorithm.
* @GNUTLS_MAC_SHA1: HMAC-SHA-1 algorithm.
* @GNUTLS_MAC_RMD160: HMAC-RMD160 algorithm.
* @GNUTLS_MAC_MD2: HMAC-MD2 algorithm.
* @GNUTLS_MAC_SHA256: HMAC-SHA-256 algorithm.
* @GNUTLS_MAC_SHA384: HMAC-SHA-384 algorithm.
* @GNUTLS_MAC_SHA512: HMAC-SHA-512 algorithm.
* @GNUTLS_MAC_SHA224: HMAC-SHA-224 algorithm.
* @GNUTLS_MAC_AEAD: MAC implicit through AEAD cipher.
* @GNUTLS_MAC_UMAC_96: The UMAC-96 MAC algorithm.
* @GNUTLS_MAC_UMAC_128: The UMAC-128 MAC algorithm.
*
* Enumeration of different Message Authentication Code (MAC)
* algorithms.
*/
typedef enum {
	GNUTLS_MAC_UNKNOWN = 0,
	GNUTLS_MAC_NULL = 1,
	GNUTLS_MAC_MD5 = 2,
	GNUTLS_MAC_SHA1 = 3,
	GNUTLS_MAC_RMD160 = 4,
	GNUTLS_MAC_MD2 = 5,
	GNUTLS_MAC_SHA256 = 6,
	GNUTLS_MAC_SHA384 = 7,
	GNUTLS_MAC_SHA512 = 8,
	GNUTLS_MAC_SHA224 = 9,
	/* If you add anything here, make sure you align with
	gnutls_digest_algorithm_t. */
	GNUTLS_MAC_AEAD = 200,	/* indicates that MAC is on the cipher */
	GNUTLS_MAC_UMAC_96 = 201,
	GNUTLS_MAC_UMAC_128 = 202
} gnutls_mac_algorithm_t;

/**
* gnutls_digest_algorithm_t:
* @GNUTLS_DIG_UNKNOWN: Unknown hash algorithm.
* @GNUTLS_DIG_NULL: NULL hash algorithm (empty output).
* @GNUTLS_DIG_MD5: MD5 algorithm.
* @GNUTLS_DIG_SHA1: SHA-1 algorithm.
* @GNUTLS_DIG_RMD160: RMD160 algorithm.
* @GNUTLS_DIG_MD2: MD2 algorithm.
* @GNUTLS_DIG_SHA256: SHA-256 algorithm.
* @GNUTLS_DIG_SHA384: SHA-384 algorithm.
* @GNUTLS_DIG_SHA512: SHA-512 algorithm.
* @GNUTLS_DIG_SHA224: SHA-224 algorithm.
*
* Enumeration of different digest (hash) algorithms.
*/
typedef enum {
	GNUTLS_DIG_UNKNOWN = GNUTLS_MAC_UNKNOWN,
	GNUTLS_DIG_NULL = GNUTLS_MAC_NULL,
	GNUTLS_DIG_MD5 = GNUTLS_MAC_MD5,
	GNUTLS_DIG_SHA1 = GNUTLS_MAC_SHA1,
	GNUTLS_DIG_RMD160 = GNUTLS_MAC_RMD160,
	GNUTLS_DIG_MD2 = GNUTLS_MAC_MD2,
	GNUTLS_DIG_SHA256 = GNUTLS_MAC_SHA256,
	GNUTLS_DIG_SHA384 = GNUTLS_MAC_SHA384,
	GNUTLS_DIG_SHA512 = GNUTLS_MAC_SHA512,
	GNUTLS_DIG_SHA224 = GNUTLS_MAC_SHA224
	/* If you add anything here, make sure you align with
	gnutls_mac_algorithm_t. */
} gnutls_digest_algorithm_t;

/**
* gnutls_credentials_type_t:
* @GNUTLS_CRD_CERTIFICATE: Certificate credential.
* @GNUTLS_CRD_ANON: Anonymous credential.
* @GNUTLS_CRD_SRP: SRP credential.
* @GNUTLS_CRD_PSK: PSK credential.
* @GNUTLS_CRD_IA: IA credential.
*
* Enumeration of different credential types.
*/
typedef enum {
	GNUTLS_CRD_CERTIFICATE = 1,
	GNUTLS_CRD_ANON,
	GNUTLS_CRD_SRP,
	GNUTLS_CRD_PSK,
	GNUTLS_CRD_IA
} gnutls_credentials_type_t;

/**
* gnutls_kx_algorithm_t:
* @GNUTLS_KX_UNKNOWN: Unknown key-exchange algorithm.
* @GNUTLS_KX_RSA: RSA key-exchange algorithm.
* @GNUTLS_KX_DHE_DSS: DHE-DSS key-exchange algorithm.
* @GNUTLS_KX_DHE_RSA: DHE-RSA key-exchange algorithm.
* @GNUTLS_KX_ECDHE_RSA: ECDHE-RSA key-exchange algorithm.
* @GNUTLS_KX_ECDHE_ECDSA: ECDHE-ECDSA key-exchange algorithm.
* @GNUTLS_KX_ANON_DH: Anon-DH key-exchange algorithm.
* @GNUTLS_KX_ANON_ECDH: Anon-ECDH key-exchange algorithm.
* @GNUTLS_KX_SRP: SRP key-exchange algorithm.
* @GNUTLS_KX_RSA_EXPORT: RSA-EXPORT key-exchange algorithm (defunc).
* @GNUTLS_KX_SRP_RSA: SRP-RSA key-exchange algorithm.
* @GNUTLS_KX_SRP_DSS: SRP-DSS key-exchange algorithm.
* @GNUTLS_KX_PSK: PSK key-exchange algorithm.
* @GNUTLS_KX_DHE_PSK: DHE-PSK key-exchange algorithm.
* @GNUTLS_KX_ECDHE_PSK: ECDHE-PSK key-exchange algorithm.
* @GNUTLS_KX_RSA_PSK: RSA-PSK key-exchange algorithm.
*
* Enumeration of different key exchange algorithms.
*/
typedef enum {
	GNUTLS_KX_UNKNOWN = 0,
	GNUTLS_KX_RSA = 1,
	GNUTLS_KX_DHE_DSS = 2,
	GNUTLS_KX_DHE_RSA = 3,
	GNUTLS_KX_ANON_DH = 4,
	GNUTLS_KX_SRP = 5,
	GNUTLS_KX_RSA_EXPORT = 6,
	GNUTLS_KX_SRP_RSA = 7,
	GNUTLS_KX_SRP_DSS = 8,
	GNUTLS_KX_PSK = 9,
	GNUTLS_KX_DHE_PSK = 10,
	GNUTLS_KX_ANON_ECDH = 11,
	GNUTLS_KX_ECDHE_RSA = 12,
	GNUTLS_KX_ECDHE_ECDSA = 13,
	GNUTLS_KX_ECDHE_PSK = 14,
	GNUTLS_KX_RSA_PSK = 15
} gnutls_kx_algorithm_t;
/**
* gnutls_certificate_status_t:
* @GNUTLS_CERT_INVALID: The certificate is not signed by one of the
*   known authorities or the signature is invalid (deprecated by the flags
*   %GNUTLS_CERT_SIGNATURE_FAILURE and %GNUTLS_CERT_SIGNER_NOT_FOUND).
* @GNUTLS_CERT_SIGNATURE_FAILURE: The signature verification failed.
* @GNUTLS_CERT_REVOKED: Certificate is revoked by its authority.  In X.509 this will be
*   set only if CRLs are checked.
* @GNUTLS_CERT_SIGNER_NOT_FOUND: The certificate's issuer is not known.
*   This is the case if the issuer is not included in the trusted certificate list.
* @GNUTLS_CERT_SIGNER_NOT_CA: The certificate's signer was not a CA. This
*   may happen if this was a version 1 certificate, which is common with
*   some CAs, or a version 3 certificate without the basic constrains extension.
* @GNUTLS_CERT_SIGNER_CONSTRAINTS_FAILURE: The certificate's signer constraints were
*   violated.
* @GNUTLS_CERT_INSECURE_ALGORITHM:  The certificate was signed using an insecure
*   algorithm such as MD2 or MD5. These algorithms have been broken and
*   should not be trusted.
* @GNUTLS_CERT_NOT_ACTIVATED: The certificate is not yet activated.
* @GNUTLS_CERT_EXPIRED: The certificate has expired.
* @GNUTLS_CERT_REVOCATION_DATA_SUPERSEDED: The revocation data are old and have been superseded.
* @GNUTLS_CERT_REVOCATION_DATA_ISSUED_IN_FUTURE: The revocation data have a future issue date.
* @GNUTLS_CERT_UNEXPECTED_OWNER: The owner is not the expected one.
* @GNUTLS_CERT_MISMATCH: The certificate presented isn't the expected one (TOFU)
* @GNUTLS_CERT_PURPOSE_MISMATCH: The certificate or an intermediate does not match the intended purpose (extended key usage).
*
* Enumeration of certificate status codes.  Note that the status
* bits may have different meanings in OpenPGP keys and X.509
* certificate verification.
*/
typedef enum {
	GNUTLS_CERT_INVALID = 1 << 1,
	GNUTLS_CERT_REVOKED = 1 << 5,
	GNUTLS_CERT_SIGNER_NOT_FOUND = 1 << 6,
	GNUTLS_CERT_SIGNER_NOT_CA = 1 << 7,
	GNUTLS_CERT_INSECURE_ALGORITHM = 1 << 8,
	GNUTLS_CERT_NOT_ACTIVATED = 1 << 9,
	GNUTLS_CERT_EXPIRED = 1 << 10,
	GNUTLS_CERT_SIGNATURE_FAILURE = 1 << 11,
	GNUTLS_CERT_REVOCATION_DATA_SUPERSEDED = 1 << 12,
	GNUTLS_CERT_UNEXPECTED_OWNER = 1 << 14,
	GNUTLS_CERT_REVOCATION_DATA_ISSUED_IN_FUTURE = 1 << 15,
	GNUTLS_CERT_SIGNER_CONSTRAINTS_FAILURE = 1 << 16,
	GNUTLS_CERT_MISMATCH = 1 << 17,
	GNUTLS_CERT_PURPOSE_MISMATCH = 1 << 18
} gnutls_certificate_status_t;

/**
* gnutls_certificate_request_t:
* @GNUTLS_CERT_IGNORE: Ignore certificate.
* @GNUTLS_CERT_REQUEST: Request certificate.
* @GNUTLS_CERT_REQUIRE: Require certificate.
*
* Enumeration of certificate request types.
*/
typedef enum {
	GNUTLS_CERT_IGNORE = 0,
	GNUTLS_CERT_REQUEST = 1,
	GNUTLS_CERT_REQUIRE = 2
} gnutls_certificate_request_t;

/**
* gnutls_close_request_t:
* @GNUTLS_SHUT_RDWR: Disallow further receives/sends.
* @GNUTLS_SHUT_WR: Disallow further sends.
*
* Enumeration of how TLS session should be terminated.  See gnutls_bye().
*/
typedef enum {
	GNUTLS_SHUT_RDWR = 0,
	GNUTLS_SHUT_WR = 1
} gnutls_close_request_t;


/**
* gnutls_certificate_type_t:
* @GNUTLS_CRT_UNKNOWN: Unknown certificate type.
* @GNUTLS_CRT_X509: X.509 Certificate.
* @GNUTLS_CRT_OPENPGP: OpenPGP certificate.
* @GNUTLS_CRT_RAW: Raw public key (SubjectPublicKey)
*
* Enumeration of different certificate types.
*/
typedef enum {
	GNUTLS_CRT_UNKNOWN = 0,
	GNUTLS_CRT_X509 = 1,
	GNUTLS_CRT_OPENPGP = 2,
	GNUTLS_CRT_RAW = 3
} gnutls_certificate_type_t;


/**
* gnutls_params_type_t:
* @GNUTLS_PARAMS_RSA_EXPORT: Session RSA-EXPORT parameters (defunc).
* @GNUTLS_PARAMS_DH: Session Diffie-Hellman parameters.
* @GNUTLS_PARAMS_ECDH: Session Elliptic-Curve Diffie-Hellman parameters.
*
* Enumeration of different TLS session parameter types.
*/
typedef enum {
	GNUTLS_PARAMS_RSA_EXPORT = 1,
	GNUTLS_PARAMS_DH = 2,
	GNUTLS_PARAMS_ECDH = 3
} gnutls_params_type_t;

/**
* gnutls_handshake_description_t:
* @GNUTLS_HANDSHAKE_HELLO_REQUEST: Hello request.
* @GNUTLS_HANDSHAKE_HELLO_VERIFY_REQUEST: DTLS Hello verify request.
* @GNUTLS_HANDSHAKE_CLIENT_HELLO: Client hello.
* @GNUTLS_HANDSHAKE_SERVER_HELLO: Server hello.
* @GNUTLS_HANDSHAKE_NEW_SESSION_TICKET: New session ticket.
* @GNUTLS_HANDSHAKE_CERTIFICATE_PKT: Certificate packet.
* @GNUTLS_HANDSHAKE_SERVER_KEY_EXCHANGE: Server key exchange.
* @GNUTLS_HANDSHAKE_CERTIFICATE_REQUEST: Certificate request.
* @GNUTLS_HANDSHAKE_SERVER_HELLO_DONE: Server hello done.
* @GNUTLS_HANDSHAKE_CERTIFICATE_VERIFY: Certificate verify.
* @GNUTLS_HANDSHAKE_CLIENT_KEY_EXCHANGE: Client key exchange.
* @GNUTLS_HANDSHAKE_FINISHED: Finished.
* @GNUTLS_HANDSHAKE_CERTIFICATE_STATUS: Certificate status (OCSP).
* @GNUTLS_HANDSHAKE_SUPPLEMENTAL: Supplemental.
* @GNUTLS_HANDSHAKE_CHANGE_CIPHER_SPEC: Change Cipher Spec.
* @GNUTLS_HANDSHAKE_CLIENT_HELLO_V2: SSLv2 Client Hello.
*
* Enumeration of different TLS handshake packets.
*/
typedef enum {
	GNUTLS_HANDSHAKE_HELLO_REQUEST = 0,
	GNUTLS_HANDSHAKE_CLIENT_HELLO = 1,
	GNUTLS_HANDSHAKE_SERVER_HELLO = 2,
	GNUTLS_HANDSHAKE_HELLO_VERIFY_REQUEST = 3,
	GNUTLS_HANDSHAKE_NEW_SESSION_TICKET = 4,
	GNUTLS_HANDSHAKE_CERTIFICATE_PKT = 11,
	GNUTLS_HANDSHAKE_SERVER_KEY_EXCHANGE = 12,
	GNUTLS_HANDSHAKE_CERTIFICATE_REQUEST = 13,
	GNUTLS_HANDSHAKE_SERVER_HELLO_DONE = 14,
	GNUTLS_HANDSHAKE_CERTIFICATE_VERIFY = 15,
	GNUTLS_HANDSHAKE_CLIENT_KEY_EXCHANGE = 16,
	GNUTLS_HANDSHAKE_FINISHED = 20,
	GNUTLS_HANDSHAKE_CERTIFICATE_STATUS = 22,
	GNUTLS_HANDSHAKE_SUPPLEMENTAL = 23,
	GNUTLS_HANDSHAKE_CHANGE_CIPHER_SPEC = 254,
	GNUTLS_HANDSHAKE_CLIENT_HELLO_V2 = 1024
} gnutls_handshake_description_t;

/**
* gnutls_privkey_type_t:
* @GNUTLS_PRIVKEY_X509: X.509 private key, #gnutls_x509_privkey_t.
* @GNUTLS_PRIVKEY_OPENPGP: OpenPGP private key, #gnutls_openpgp_privkey_t.
* @GNUTLS_PRIVKEY_PKCS11: PKCS11 private key, #gnutls_pkcs11_privkey_t.
* @GNUTLS_PRIVKEY_EXT: External private key, operating using callbacks.
*
* Enumeration of different private key types.
*/
typedef enum {
	GNUTLS_PRIVKEY_X509,
	GNUTLS_PRIVKEY_OPENPGP,
	GNUTLS_PRIVKEY_PKCS11,
	GNUTLS_PRIVKEY_EXT
} gnutls_privkey_type_t;

#define GNUTLS_HANDSHAKE_ANY ((unsigned int)-1)
/* exported for other gnutls headers. This is the maximum number of
* algorithms (ciphers, kx or macs).
*/
#define GNUTLS_MAX_ALGORITHM_NUM 32
#define GNUTLS_MAX_SESSION_ID_SIZE 32


typedef int( *gnutls_pin_callback_t ) ( void *userdata, int attempt,
										const char *token_url,
										const char *token_label,
										unsigned int flags,
										char *pin, size_t pin_max );

struct pin_info_st {
	gnutls_pin_callback_t cb;
	void *data;
};

typedef void *bigint_t;

/**
* gnutls_bigint_format_t:
* @GNUTLS_MPI_FORMAT_USG: Raw unsigned integer format.
* @GNUTLS_MPI_FORMAT_STD: Raw signed integer format, always a leading
*   zero when positive.
* @GNUTLS_MPI_FORMAT_PGP: The pgp integer format.
*
* Enumeration of different bignum integer encoding formats.
*/
typedef enum {
	/* raw unsigned integer format */
	GNUTLS_MPI_FORMAT_USG = 0,
	/* raw signed integer format - always a leading zero when positive */
	GNUTLS_MPI_FORMAT_STD = 1,
	/* the pgp integer format */
	GNUTLS_MPI_FORMAT_PGP = 2
} gnutls_bigint_format_t;

typedef void *bigint_t;
/**
* gnutls_bigint_format_t:
* @GNUTLS_MPI_FORMAT_USG: Raw unsigned integer format.
* @GNUTLS_MPI_FORMAT_STD: Raw signed integer format, always a leading
*   zero when positive.
* @GNUTLS_MPI_FORMAT_PGP: The pgp integer format.
*
* Enumeration of different bignum integer encoding formats.
*/

/* Multi precision integer arithmetic */
typedef struct gnutls_crypto_bigint
{
	int( *bigint_init ) ( bigint_t* );
	int( *bigint_init_multi ) ( bigint_t*, ... );
	void( *bigint_release ) ( bigint_t n );
	void( *bigint_clear ) ( bigint_t n );	/* zeros the int */
											/* 0 for equality, > 0 for m1>m2, < 0 for m1<m2 */
	int( *bigint_cmp ) ( const bigint_t m1, const bigint_t m2 );
	/* as bigint_cmp */
	int( *bigint_cmp_ui ) ( const bigint_t m1, unsigned long m2 );
	/* r = a % b */
	int( *bigint_modm ) ( bigint_t r, const bigint_t a, const bigint_t b );
	/* a = b -> ret == a */
	int( *bigint_set ) ( bigint_t a, const bigint_t b );
	bigint_t( *bigint_copy ) ( const bigint_t a );
	/* a = b -> ret == a */
	int( *bigint_set_ui ) ( bigint_t a, unsigned long b );
	unsigned int( *bigint_get_nbits ) ( const bigint_t a );
	/* w = b ^ e mod m */
	int( *bigint_powm ) ( bigint_t w, const bigint_t b,
						  const bigint_t e, const bigint_t m );
	/* w = a + b mod m */
	int( *bigint_addm ) ( bigint_t w, const bigint_t a,
						  const bigint_t b, const bigint_t m );
	/* w = a - b mod m */
	int( *bigint_subm ) ( bigint_t w, const bigint_t a,
						  const bigint_t b, const bigint_t m );
	/* w = a * b mod m */
	int( *bigint_mulm ) ( bigint_t w, const bigint_t a,
						  const bigint_t b, const bigint_t m );
	/* w = a + b */ int( *bigint_add ) ( bigint_t w,
										 const bigint_t a,
										 const bigint_t b );
	/* w = a - b */ int( *bigint_sub ) ( bigint_t w,
										 const bigint_t a,
										 const bigint_t b );
	/* w = a * b */
	int( *bigint_mul ) ( bigint_t w, const bigint_t a,
						 const bigint_t b );
	/* w = a + b */
	int( *bigint_add_ui ) ( bigint_t w, const bigint_t a,
							unsigned long b );
	/* w = a - b */
	int( *bigint_sub_ui ) ( bigint_t w, const bigint_t a,
							unsigned long b );
	/* w = a * b */
	int( *bigint_mul_ui ) ( bigint_t w, const bigint_t a,
							unsigned long b );
	/* q = a / b */
	int( *bigint_div ) ( bigint_t q, const bigint_t a,
						 const bigint_t b );
	/* 0 if prime */
	int( *bigint_prime_check ) ( const bigint_t pp );

	/* reads a bigint from a buffer */
	/* stores a bigint into the buffer.  returns
	* GNUTLS_E_SHORT_MEMORY_BUFFER if buf_size is not sufficient to
	* store this integer, and updates the buf_size;
	*/
	int( *bigint_scan ) ( bigint_t m, const void *buf, size_t buf_size,
						  gnutls_bigint_format_t format );
	int( *bigint_print ) ( const bigint_t a, void *buf,
						   size_t * buf_size,
						   gnutls_bigint_format_t format );
} gnutls_crypto_bigint_st;

/**
* gnutls_pk_flag_t:
* @GNUTLS_PK_FLAG_NONE: No flag.
*
* Enumeration of public-key flag.
*/
typedef enum {
	GNUTLS_PK_FLAG_NONE = 0
} gnutls_pk_flag_t;

/**
* gnutls_pk_algorithm_t:
* @GNUTLS_PK_UNKNOWN: Unknown public-key algorithm.
* @GNUTLS_PK_RSA: RSA public-key algorithm.
* @GNUTLS_PK_DSA: DSA public-key algorithm.
* @GNUTLS_PK_DH: Diffie-Hellman algorithm. Used to generate parameters.
* @GNUTLS_PK_EC: Elliptic curve algorithm. Used to generate parameters.
*
* Enumeration of different public-key algorithms.
*/
typedef enum {
	GNUTLS_PK_UNKNOWN = 0,
	GNUTLS_PK_RSA = 1,
	GNUTLS_PK_DSA = 2,
	GNUTLS_PK_DH = 3,
	GNUTLS_PK_EC = 4
} gnutls_pk_algorithm_t;
/**
* gnutls_x509_crt_fmt_t:
* @GNUTLS_X509_FMT_DER: X.509 certificate in DER format (binary).
* @GNUTLS_X509_FMT_PEM: X.509 certificate in PEM format (text).
*
* Enumeration of different certificate encoding formats.
*/
typedef enum {
	GNUTLS_X509_FMT_DER = 0,
	GNUTLS_X509_FMT_PEM = 1
} gnutls_x509_crt_fmt_t;

/*
* gnutls_certificate_print_formats_t:
*@GNUTLS_CRT_PRINT_FULL: Full information about certificate.
* @GNUTLS_CRT_PRINT_FULL_NUMBERS: Full information about certificate and include easy to parse public key parameters.
* @GNUTLS_CRT_PRINT_COMPACT: Information about certificate name in one line, plus identification of the public key.
* @GNUTLS_CRT_PRINT_ONELINE: Information about certificate in one line.
* @GNUTLS_CRT_PRINT_UNSIGNED_FULL: All info for an unsigned certificate.
*
* Enumeration of different certificate printing variants.
*/

typedef enum gnutls_certificate_print_formats {
	GNUTLS_CRT_PRINT_FULL = 0,
	GNUTLS_CRT_PRINT_ONELINE = 1,
	GNUTLS_CRT_PRINT_UNSIGNED_FULL = 2,
	GNUTLS_CRT_PRINT_COMPACT = 3,
	GNUTLS_CRT_PRINT_FULL_NUMBERS = 4
} gnutls_certificate_print_formats_t;
/**
* gnutls_sign_algorithm_t:
* @GNUTLS_SIGN_UNKNOWN: Unknown signature algorithm.
* @GNUTLS_SIGN_RSA_SHA1: Digital signature algorithm RSA with SHA-1
* @GNUTLS_SIGN_RSA_SHA: Same as %GNUTLS_SIGN_RSA_SHA1.
* @GNUTLS_SIGN_DSA_SHA1: Digital signature algorithm DSA with SHA-1
* @GNUTLS_SIGN_DSA_SHA224: Digital signature algorithm DSA with SHA-224
* @GNUTLS_SIGN_DSA_SHA256: Digital signature algorithm DSA with SHA-256
* @GNUTLS_SIGN_DSA_SHA384: Digital signature algorithm DSA with SHA-384
* @GNUTLS_SIGN_DSA_SHA512: Digital signature algorithm DSA with SHA-512
* @GNUTLS_SIGN_DSA_SHA: Same as %GNUTLS_SIGN_DSA_SHA1.
* @GNUTLS_SIGN_RSA_MD5: Digital signature algorithm RSA with MD5.
* @GNUTLS_SIGN_RSA_MD2: Digital signature algorithm RSA with MD2.
* @GNUTLS_SIGN_RSA_RMD160: Digital signature algorithm RSA with RMD-160.
* @GNUTLS_SIGN_RSA_SHA256: Digital signature algorithm RSA with SHA-256.
* @GNUTLS_SIGN_RSA_SHA384: Digital signature algorithm RSA with SHA-384.
* @GNUTLS_SIGN_RSA_SHA512: Digital signature algorithm RSA with SHA-512.
* @GNUTLS_SIGN_RSA_SHA224: Digital signature algorithm RSA with SHA-224.
* @GNUTLS_SIGN_ECDSA_SHA1: ECDSA with SHA1.
* @GNUTLS_SIGN_ECDSA_SHA256: Digital signature algorithm ECDSA with SHA-256.
* @GNUTLS_SIGN_ECDSA_SHA384: Digital signature algorithm ECDSA with SHA-384.
* @GNUTLS_SIGN_ECDSA_SHA512: Digital signature algorithm ECDSA with SHA-512.
* @GNUTLS_SIGN_ECDSA_SHA224: Digital signature algorithm ECDSA with SHA-224.
*
* Enumeration of different digital signature algorithms.
*/
typedef enum {
	GNUTLS_SIGN_UNKNOWN = 0,
	GNUTLS_SIGN_RSA_SHA1 = 1,
	GNUTLS_SIGN_RSA_SHA = GNUTLS_SIGN_RSA_SHA1,
	GNUTLS_SIGN_DSA_SHA1 = 2,
	GNUTLS_SIGN_DSA_SHA = GNUTLS_SIGN_DSA_SHA1,
	GNUTLS_SIGN_RSA_MD5 = 3,
	GNUTLS_SIGN_RSA_MD2 = 4,
	GNUTLS_SIGN_RSA_RMD160 = 5,
	GNUTLS_SIGN_RSA_SHA256 = 6,
	GNUTLS_SIGN_RSA_SHA384 = 7,
	GNUTLS_SIGN_RSA_SHA512 = 8,
	GNUTLS_SIGN_RSA_SHA224 = 9,
	GNUTLS_SIGN_DSA_SHA224 = 10,
	GNUTLS_SIGN_DSA_SHA256 = 11,
	GNUTLS_SIGN_ECDSA_SHA1 = 12,
	GNUTLS_SIGN_ECDSA_SHA224 = 13,
	GNUTLS_SIGN_ECDSA_SHA256 = 14,
	GNUTLS_SIGN_ECDSA_SHA384 = 15,
	GNUTLS_SIGN_ECDSA_SHA512 = 16,
	GNUTLS_SIGN_DSA_SHA384 = 17,
	GNUTLS_SIGN_DSA_SHA512 = 18
} gnutls_sign_algorithm_t;

/**
* gnutls_ecc_curve_t:
* @GNUTLS_ECC_CURVE_INVALID: Cannot be known
* @GNUTLS_ECC_CURVE_SECP192R1: the SECP192R1 curve
* @GNUTLS_ECC_CURVE_SECP224R1: the SECP224R1 curve
* @GNUTLS_ECC_CURVE_SECP256R1: the SECP256R1 curve
* @GNUTLS_ECC_CURVE_SECP384R1: the SECP384R1 curve
* @GNUTLS_ECC_CURVE_SECP521R1: the SECP521R1 curve
*
* Enumeration of ECC curves.
*/
typedef enum {
	GNUTLS_ECC_CURVE_INVALID = 0,
	GNUTLS_ECC_CURVE_SECP224R1,
	GNUTLS_ECC_CURVE_SECP256R1,
	GNUTLS_ECC_CURVE_SECP384R1,
	GNUTLS_ECC_CURVE_SECP521R1,
	GNUTLS_ECC_CURVE_SECP192R1
} gnutls_ecc_curve_t;
/**
* gnutls_cipher_algorithm_t:
* @GNUTLS_CIPHER_UNKNOWN: Value to identify an unknown/unsupported algorithm.
* @GNUTLS_CIPHER_NULL: The NULL (identity) encryption algorithm.
* @GNUTLS_CIPHER_ARCFOUR_128: ARCFOUR stream cipher with 128-bit keys.
* @GNUTLS_CIPHER_3DES_CBC: 3DES in CBC mode.
* @GNUTLS_CIPHER_AES_128_CBC: AES in CBC mode with 128-bit keys.
* @GNUTLS_CIPHER_AES_192_CBC: AES in CBC mode with 192-bit keys.
* @GNUTLS_CIPHER_AES_256_CBC: AES in CBC mode with 256-bit keys.
* @GNUTLS_CIPHER_ARCFOUR_40: ARCFOUR stream cipher with 40-bit keys.
* @GNUTLS_CIPHER_CAMELLIA_128_CBC: Camellia in CBC mode with 128-bit keys.
* @GNUTLS_CIPHER_CAMELLIA_192_CBC: Camellia in CBC mode with 192-bit keys.
* @GNUTLS_CIPHER_CAMELLIA_256_CBC: Camellia in CBC mode with 256-bit keys.
* @GNUTLS_CIPHER_RC2_40_CBC: RC2 in CBC mode with 40-bit keys.
* @GNUTLS_CIPHER_DES_CBC: DES in CBC mode (56-bit keys).
* @GNUTLS_CIPHER_AES_128_GCM: AES in GCM mode with 128-bit keys.
* @GNUTLS_CIPHER_AES_256_GCM: AES in GCM mode with 256-bit keys.
* @GNUTLS_CIPHER_AES_128_CCM: AES in CCM mode with 128-bit keys.
* @GNUTLS_CIPHER_AES_256_CCM: AES in CCM mode with 256-bit keys.
* @GNUTLS_CIPHER_AES_128_CCM_8: AES in CCM mode with 64-bit tag and 128-bit keys.
* @GNUTLS_CIPHER_AES_256_CCM_8: AES in CCM mode with 64-bit tag and 256-bit keys.
* @GNUTLS_CIPHER_CAMELLIA_128_GCM: CAMELLIA in GCM mode with 128-bit keys.
* @GNUTLS_CIPHER_CAMELLIA_256_GCM: CAMELLIA in GCM mode with 256-bit keys.
* @GNUTLS_CIPHER_SALSA20_256: Salsa20 with 256-bit keys.
* @GNUTLS_CIPHER_ESTREAM_SALSA20_256: Estream's Salsa20 variant with 256-bit keys.
* @GNUTLS_CIPHER_CHACHA20_POLY1305: The Chacha20 cipher with the Poly1305 authenticator (AEAD).
* @GNUTLS_CIPHER_IDEA_PGP_CFB: IDEA in CFB mode (placeholder - unsupported).
* @GNUTLS_CIPHER_3DES_PGP_CFB: 3DES in CFB mode (placeholder - unsupported).
* @GNUTLS_CIPHER_CAST5_PGP_CFB: CAST5 in CFB mode (placeholder - unsupported).
* @GNUTLS_CIPHER_BLOWFISH_PGP_CFB: Blowfish in CFB mode (placeholder - unsupported).
* @GNUTLS_CIPHER_SAFER_SK128_PGP_CFB: Safer-SK in CFB mode with 128-bit keys (placeholder - unsupported).
* @GNUTLS_CIPHER_AES128_PGP_CFB: AES in CFB mode with 128-bit keys (placeholder - unsupported).
* @GNUTLS_CIPHER_AES192_PGP_CFB: AES in CFB mode with 192-bit keys (placeholder - unsupported).
* @GNUTLS_CIPHER_AES256_PGP_CFB: AES in CFB mode with 256-bit keys (placeholder - unsupported).
* @GNUTLS_CIPHER_TWOFISH_PGP_CFB: Twofish in CFB mode (placeholder - unsupported).
*
* Enumeration of different symmetric encryption algorithms.
*/
typedef enum gnutls_cipher_algorithm {
	GNUTLS_CIPHER_UNKNOWN = 0,
	GNUTLS_CIPHER_NULL = 1,
	GNUTLS_CIPHER_ARCFOUR_128 = 2,
	GNUTLS_CIPHER_3DES_CBC = 3,
	GNUTLS_CIPHER_AES_128_CBC = 4,
	GNUTLS_CIPHER_AES_256_CBC = 5,
	GNUTLS_CIPHER_ARCFOUR_40 = 6,
	GNUTLS_CIPHER_CAMELLIA_128_CBC = 7,
	GNUTLS_CIPHER_CAMELLIA_256_CBC = 8,
	GNUTLS_CIPHER_AES_192_CBC = 9,
	GNUTLS_CIPHER_AES_128_GCM = 10,
	GNUTLS_CIPHER_AES_256_GCM = 11,
	GNUTLS_CIPHER_CAMELLIA_192_CBC = 12,
	GNUTLS_CIPHER_SALSA20_256 = 13,
	GNUTLS_CIPHER_ESTREAM_SALSA20_256 = 14,
	GNUTLS_CIPHER_CAMELLIA_128_GCM = 15,
	GNUTLS_CIPHER_CAMELLIA_256_GCM = 16,
	GNUTLS_CIPHER_RC2_40_CBC = 17,
	GNUTLS_CIPHER_DES_CBC = 18,
	GNUTLS_CIPHER_AES_128_CCM = 19,
	GNUTLS_CIPHER_AES_256_CCM = 20,
	GNUTLS_CIPHER_AES_128_CCM_8 = 21,
	GNUTLS_CIPHER_AES_256_CCM_8 = 22,
	GNUTLS_CIPHER_CHACHA20_POLY1305 = 23,

	/* used only for PGP internals. Ignored in TLS/SSL
	*/
	GNUTLS_CIPHER_IDEA_PGP_CFB = 200,
	GNUTLS_CIPHER_3DES_PGP_CFB = 201,
	GNUTLS_CIPHER_CAST5_PGP_CFB = 202,
	GNUTLS_CIPHER_BLOWFISH_PGP_CFB = 203,
	GNUTLS_CIPHER_SAFER_SK128_PGP_CFB = 204,
	GNUTLS_CIPHER_AES128_PGP_CFB = 205,
	GNUTLS_CIPHER_AES192_PGP_CFB = 206,
	GNUTLS_CIPHER_AES256_PGP_CFB = 207,
	GNUTLS_CIPHER_TWOFISH_PGP_CFB = 208
} gnutls_cipher_algorithm_t;

/**
* gnutls_openpgp_crt_status_t:
* @GNUTLS_OPENPGP_CERT: Send entire certificate.
* @GNUTLS_OPENPGP_CERT_FINGERPRINT: Send only certificate fingerprint.
*
* Enumeration of ways to send OpenPGP certificate.
*/
typedef enum {
	GNUTLS_OPENPGP_CERT = 0,
	GNUTLS_OPENPGP_CERT_FINGERPRINT = 1
} gnutls_openpgp_crt_status_t;

#define GNUTLS_OPENPGP_KEYID_SIZE 8
#define GNUTLS_OPENPGP_V4_FINGERPRINT_SIZE 20
typedef unsigned char
gnutls_openpgp_keyid_t[ GNUTLS_OPENPGP_KEYID_SIZE ];

typedef struct gnutls_datum_t {
	unsigned char *data;
	unsigned int size;
} gnutls_datum_t;

typedef struct gnutls_x509_ext_st {
	char *oid;
	unsigned int critical;
	gnutls_datum_t data;
} gnutls_x509_ext_st;

typedef struct {
	bigint_t params[ GNUTLS_MAX_PK_PARAMS ];
	unsigned int params_nr;	/* the number of parameters */
	unsigned int flags;
	gnutls_pk_algorithm_t algo;
} gnutls_pk_params_st;

struct gnutls_pubkey_st {
	gnutls_pk_algorithm_t pk_algorithm;
	unsigned int bits;	/* an indication of the security parameter */

						/* the size of params depends on the public
						* key algorithm
						* RSA: [0] is modulus
						*      [1] is public exponent
						* DSA: [0] is p
						*      [1] is q
						*      [2] is g
						*      [3] is public key
						*/
	gnutls_pk_params_st params;

#ifdef ENABLE_OPENPGP
	uint8_t openpgp_key_id[ GNUTLS_OPENPGP_KEYID_SIZE ];
	unsigned int openpgp_key_id_set;

	uint8_t openpgp_key_fpr[ GNUTLS_OPENPGP_V4_FINGERPRINT_SIZE ];
	unsigned int openpgp_key_fpr_set : 1;
#endif

	unsigned int key_usage;	/* bits from GNUTLS_KEY_* */

	struct pin_info_st pin;
};

typedef enum transport_t {
	GNUTLS_STREAM,
	GNUTLS_DGRAM
} transport_t;

typedef struct {
	const char *name;
	gnutls_protocol_t id;	/* gnutls internal version number */
	unsigned age;		/* internal ordering by protocol age */
	uint8_t major;		/* defined by the protocol */
	uint8_t minor;		/* defined by the protocol */
	transport_t transport;	/* Type of transport, stream or datagram */
	bool supported;	/* 0 not supported, > 0 is supported */
	bool explicit_iv;
	bool extensions;	/* whether it supports extensions */
	bool selectable_sighash;	/* whether signatures can be selected */
	bool selectable_prf;	/* whether the PRF is ciphersuite-defined */
	bool obsolete;		/* Do not use this protocol version as record version */
} version_entry_st;

/* Session stuff
*/
typedef struct {
	void *iov_base;		/* Starting address */
	size_t iov_len;		/* Number of bytes to transfer */
} giovec_t;


/* This structure is used both for MACs and digests
*/
typedef struct mac_entry_st {
	const char *name;
	const char *oid;	/* OID of the hash - if it is a hash */
	gnutls_mac_algorithm_t id;
	unsigned output_size;
	unsigned key_size;
	unsigned nonce_size;
	unsigned placeholder;	/* if set, then not a real MAC */
	unsigned secure;	/* must be set to zero if this hash is known to be broken */
	unsigned block_size;	/* internal block size for HMAC */
} mac_entry_st;


/**
* gnutls_vdata_types_t:
* @GNUTLS_DT_UNKNOWN: Unknown data type.
* @GNUTLS_DT_DNS_HOSTNAME: The data contain a null-terminated DNS hostname; the hostname will be
*   matched using the RFC6125 rules.
* @GNUTLS_DT_RFC822NAME: The data contain a null-terminated email address; the email will be
*   matched against the RFC822Name field of the certificate, or the EMAIL DN component if the
*   former isn't available. Prior to matching the email address will be converted to ACE
*   (ASCII-compatible-encoding).
* @GNUTLS_DT_KEY_PURPOSE_OID: The data contain a null-terminated key purpose OID. It will be matched
*   against the certificate's Extended Key Usage extension.
*
* Enumeration of different typed-data options. They are used as input to certificate
* verification functions to provide information about the name and purpose of the
* certificate. Only a single option of a type can be provided to the relevant functions.
*/
typedef enum {
	GNUTLS_DT_UNKNOWN = 0,
	GNUTLS_DT_DNS_HOSTNAME = 1,
	GNUTLS_DT_KEY_PURPOSE_OID = 2,
	GNUTLS_DT_RFC822NAME = 3
} gnutls_vdata_types_t;

typedef struct {
	gnutls_vdata_types_t type;
	unsigned char *data;
	unsigned int size;
} gnutls_typed_vdata_st;


typedef void *( *gnutls_alloc_function ) ( size_t );
typedef void *( *gnutls_calloc_function ) ( size_t, size_t );
typedef int( *gnutls_is_secure_function ) ( const void * );
typedef void( *gnutls_free_function ) ( void * );
typedef void *( *gnutls_realloc_function ) ( void *, size_t );
/* a variant of memset that doesn't get optimized out */
void gnutls_memset( void *data, int c, size_t size );

/* For use in callbacks */
extern NLC_EXPORT gnutls_alloc_function		gnutls_malloc;
extern NLC_EXPORT gnutls_realloc_function		gnutls_realloc;
extern NLC_EXPORT gnutls_calloc_function		gnutls_calloc;
extern NLC_EXPORT gnutls_free_function			gnutls_free;

extern NLC_EXPORT char *( *gnutls_strdup ) ( const char * );

/* for message digests */

typedef int( *gnutls_db_store_func ) ( void *, gnutls_datum_t key, gnutls_datum_t data );
typedef int( *gnutls_db_remove_func ) ( void *, gnutls_datum_t key );
typedef gnutls_datum_t( *gnutls_db_retr_func ) ( void *, gnutls_datum_t key );

typedef int( *hash_func ) ( void *handle, const void *text, size_t size );
typedef int( *nonce_func ) ( void *handle, const void *text, size_t size );
typedef int( *output_func ) ( void *src_ctx, void *digest, size_t digestsize );
typedef void( *hash_deinit_func ) ( void *handle );

typedef struct {
	const mac_entry_st *e;
	hash_func hash;
	output_func output;
	hash_deinit_func deinit;

	const void *key;	/* esoteric use by SSL3 MAC functions */
	int keysize;

	void *handle;
} digest_hd_st;

typedef struct mac_hd {
	const mac_entry_st *e;
	int mac_len;

	hash_func hash;
	nonce_func setnonce;
	output_func output;
	hash_deinit_func deinit;

	void *handle;
} mac_hd_st;

typedef time_t( *gnutls_time_func ) ( time_t * t );

typedef int( *mutex_init_func ) ( void **mutex );
typedef int( *mutex_lock_func ) ( void **mutex );
typedef int( *mutex_unlock_func ) ( void **mutex );
typedef int( *mutex_deinit_func ) ( void **mutex );



/* x.509 types */

struct gnutls_x509_privkey_int;
typedef struct gnutls_x509_privkey_int *gnutls_x509_privkey_t;

struct gnutls_x509_crq_int;
typedef struct gnutls_x509_crq_int *gnutls_x509_crq_t;

struct gnutls_openpgp_keyring_int;
typedef struct gnutls_openpgp_keyring_int *gnutls_openpgp_keyring_t;

struct gnutls_x509_crt_int;
typedef struct gnutls_x509_crt_int * gnutls_x509_crt_t;

struct gnutls_x509_crl_int;
typedef struct gnutls_x509_crl_int *gnutls_x509_crl_t;

struct gnutls_pubkey_st;
typedef struct gnutls_pubkey_st *gnutls_pubkey_t;

struct gnutls_privkey_st;
typedef struct gnutls_privkey_st *gnutls_privkey_t;

struct gnutls_openpgp_crt_int;
typedef struct gnutls_openpgp_crt_int *gnutls_openpgp_crt_t;

struct gnutls_openpgp_privkey_int;
typedef struct gnutls_openpgp_privkey_int *gnutls_openpgp_privkey_t;

struct gnutls_pkcs11_privkey_st;
typedef struct gnutls_pkcs11_privkey_st *gnutls_pkcs11_privkey_t;

struct gnutls_session_int;
typedef struct gnutls_session_int *gnutls_session_t;

struct gnutls_dh_params_int;
typedef struct gnutls_dh_params_int *gnutls_dh_params_t;

/* XXX ugly. */
struct gnutls_x509_privkey_int;
typedef struct gnutls_x509_privkey_int *gnutls_rsa_params_t;

struct gnutls_priority_st;
typedef struct gnutls_priority_st *gnutls_priority_t;
typedef void *gnutls_ext_priv_data_t;


/* Credential structures - used in gnutls_credentials_set(); */
struct gnutls_certificate_credentials_st;
typedef struct gnutls_certificate_credentials_st *	gnutls_certificate_credentials_t;
typedef gnutls_certificate_credentials_t			gnutls_certificate_server_credentials;
typedef gnutls_certificate_credentials_t			gnutls_certificate_client_credentials;
typedef struct gnutls_anon_server_credentials_st *	gnutls_anon_server_credentials_t;
typedef struct gnutls_anon_client_credentials_st *	gnutls_anon_client_credentials_t;

typedef int gnutls_certificate_verify_function( gnutls_session_t );

typedef void( *gnutls_log_func ) ( int, const char * );
typedef void( *gnutls_audit_log_func ) ( gnutls_session_t, const char * );
typedef int( *gnutls_handshake_post_client_hello_func ) ( gnutls_session_t );
typedef int( *gnutls_handshake_hook_func ) ( gnutls_session_t,
											 unsigned int htype,
											 unsigned post,
											 unsigned int incoming,
											 const gnutls_datum_t *msg );


/* If you want to change this, then also change the define in
* gnutls_int.h, and recompile.
*/
typedef void *gnutls_transport_ptr_t;
typedef ssize_t( *gnutls_pull_func ) ( gnutls_transport_ptr_t, void *,
									   size_t );
typedef ssize_t( *gnutls_push_func ) ( gnutls_transport_ptr_t, const void *,
									   size_t );

int gnutls_system_recv_timeout( gnutls_transport_ptr_t ptr, unsigned int ms );
typedef int( *gnutls_pull_timeout_func ) ( gnutls_transport_ptr_t,
										   unsigned int ms );

typedef ssize_t( *gnutls_vec_push_func ) ( gnutls_transport_ptr_t,
										   const giovec_t * iov, int iovcnt );

typedef int( *gnutls_errno_func ) ( gnutls_transport_ptr_t );

/**
* gnutls_pcert_st:
* @pubkey: public key of parsed certificate.
* @cert: certificate itself of parsed certificate
* @type: type of certificate, a #gnutls_certificate_type_t type.
*
* A parsed certificate.
*/
typedef struct gnutls_pcert_st {
	gnutls_pubkey_t pubkey;
	gnutls_datum_t cert;
	gnutls_certificate_type_t type;
} gnutls_pcert_st;


#if 0
/* This will be defined as macro. */
void gnutls_transport_set_int( gnutls_session_t session, int r );
#endif

void gnutls_transport_set_int2( gnutls_session_t session, int r, int s );
#define gnutls_transport_set_int(s, i) gnutls_transport_set_int2(s, i, i)

void gnutls_transport_get_int2( gnutls_session_t session, int *r, int *s );
int gnutls_transport_get_int( gnutls_session_t session );

void gnutls_transport_set_ptr( gnutls_session_t session,
							   gnutls_transport_ptr_t ptr );
void gnutls_transport_set_ptr2( gnutls_session_t session,
								gnutls_transport_ptr_t recv_ptr,
								gnutls_transport_ptr_t send_ptr );

gnutls_transport_ptr_t gnutls_transport_get_ptr( gnutls_session_t session );
void gnutls_transport_get_ptr2( gnutls_session_t session,
								gnutls_transport_ptr_t * recv_ptr,
								gnutls_transport_ptr_t * send_ptr );

void gnutls_transport_set_vec_push_function( gnutls_session_t session,
											 gnutls_vec_push_func vec_func );
void gnutls_transport_set_push_function( gnutls_session_t session,
										 gnutls_push_func push_func );
void gnutls_transport_set_pull_function( gnutls_session_t session,
										 gnutls_pull_func pull_func );

void gnutls_transport_set_pull_timeout_function( gnutls_session_t session,
												 gnutls_pull_timeout_func
												 func );

void gnutls_transport_set_errno_function( gnutls_session_t session,
										  gnutls_errno_func errno_func );

void gnutls_transport_set_errno( gnutls_session_t session, int err );

/* session specific
*/
void gnutls_session_set_ptr( gnutls_session_t session, void *ptr );
void *gnutls_session_get_ptr( gnutls_session_t session );

void gnutls_openpgp_send_cert( gnutls_session_t session,
							   gnutls_openpgp_crt_status_t status );

/* This function returns the hash of the given data.
*/
int gnutls_fingerprint( gnutls_digest_algorithm_t algo,
						const gnutls_datum_t * data, void *result,
						size_t * result_size );

typedef int( *gnutls_verify_output_function )( gnutls_x509_crt_t cert, 
											   gnutls_x509_crt_t issuer,	/* The issuer if verification failed because of him. might be null.*/
											   gnutls_x509_crl_t crl_x509,	/* The CRL that caused verification failure
																		* if any. Might be null.
																		*/
											   unsigned int verification_output );


typedef int( *gnutls_privkey_sign_func ) ( gnutls_privkey_t key,
										   void *userdata,
										   const gnutls_datum_t *
										   raw_data,
										   gnutls_datum_t * signature );
typedef int( *gnutls_privkey_decrypt_func ) ( gnutls_privkey_t key,
											  void *userdata,
											  const gnutls_datum_t *
											  ciphertext,
											  gnutls_datum_t * plaintext );

typedef void( *gnutls_privkey_deinit_func ) ( gnutls_privkey_t key,
											  void *userdata );
/* returns information on the public key associated with userdata */
typedef int( *gnutls_privkey_info_func ) ( gnutls_privkey_t key, unsigned int flags, void *userdata );
NLC_END_CDECLARES


#include <libgnu/pkcs11.h>

//#include <libgnu/x509.h>
#include <libgnu/openpgp.h>
#include <libgnu/tpm.h>
#include <libgnu/libtasn1.h>

NLC_BEGIN_CDECLARES

//#include <libgnu/abstract_gnu.h>
/* Public key operations */

#define GNUTLS_PUBKEY_VERIFY_FLAG_TLS_RSA GNUTLS_PUBKEY_VERIFY_FLAG_TLS1_RSA
/**
 * gnutls_pubkey_flags:
 * @GNUTLS_PUBKEY_DISABLE_CALLBACKS: The following flag disables call to PIN callbacks. Only
 *   relevant to TPM keys.
 * @GNUTLS_PUBKEY_GET_OPENPGP_FINGERPRINT: request an OPENPGP fingerprint instead of the default.
 *
 * Enumeration of different certificate import flags.
 */
typedef enum gnutls_pubkey_flags {
	GNUTLS_PUBKEY_DISABLE_CALLBACKS = 1 << 2,
	GNUTLS_PUBKEY_GET_OPENPGP_FINGERPRINT = 1 << 3
} gnutls_pubkey_flags_t;


#define GNUTLS_PUBKEY_VERIFY_FLAG_TLS1_RSA GNUTLS_VERIFY_USE_TLS1_RSA

/* Should return the public key algorithm (gnutls_pk_algorithm_t) */
#define GNUTLS_PRIVKEY_INFO_PK_ALGO 1

/* Should return the preferred signature algorithm (gnutls_sign_algorithm_t) or 0. */
#define GNUTLS_PRIVKEY_INFO_SIGN_ALGO (1<<1)


int gnutls_pubkey_init(gnutls_pubkey_t * key);
void gnutls_pubkey_deinit(gnutls_pubkey_t key);

int gnutls_pubkey_verify_params(gnutls_pubkey_t key);

void gnutls_pubkey_set_pin_function(gnutls_pubkey_t key,
				    gnutls_pin_callback_t fn,
				    void *userdata);

int gnutls_pubkey_get_pk_algorithm(gnutls_pubkey_t key,
				   unsigned int *bits);

int gnutls_pubkey_import_x509(gnutls_pubkey_t key,
			      gnutls_x509_crt_t crt, unsigned int flags);
int gnutls_pubkey_import_x509_crq(gnutls_pubkey_t key,
				  gnutls_x509_crq_t crq,
				  unsigned int flags);
int gnutls_pubkey_import_pkcs11(gnutls_pubkey_t key,
				gnutls_pkcs11_obj_t obj,
				unsigned int flags);
int gnutls_pubkey_import_openpgp(gnutls_pubkey_t key,
				 gnutls_openpgp_crt_t crt,
				 unsigned int flags);

int gnutls_pubkey_import_openpgp_raw(gnutls_pubkey_t pkey,
				     const gnutls_datum_t * data,
				     gnutls_openpgp_crt_fmt_t format,
				     const gnutls_openpgp_keyid_t
				     keyid, unsigned int flags);
int gnutls_pubkey_import_x509_raw(gnutls_pubkey_t pkey,
				  const gnutls_datum_t * data,
				  gnutls_x509_crt_fmt_t format,
				  unsigned int flags);

int
gnutls_pubkey_import_privkey(gnutls_pubkey_t key,
			     gnutls_privkey_t pkey,
			     unsigned int usage, unsigned int flags);

int
gnutls_pubkey_import_tpm_url(gnutls_pubkey_t pkey,
			     const char *url,
			     const char *srk_password, unsigned int flags);

int
gnutls_pubkey_import_url(gnutls_pubkey_t key, const char *url,
			 unsigned int flags);

int
gnutls_pubkey_import_tpm_raw(gnutls_pubkey_t pkey,
			     const gnutls_datum_t * fdata,
			     gnutls_tpmkey_fmt_t format,
			     const char *srk_password, unsigned int flags);

int gnutls_pubkey_get_preferred_hash_algorithm(gnutls_pubkey_t key,
					       gnutls_digest_algorithm_t
					       * hash, unsigned int *mand);

#define gnutls_pubkey_get_pk_rsa_raw gnutls_pubkey_export_rsa_raw
int gnutls_pubkey_export_rsa_raw(gnutls_pubkey_t key,
				 gnutls_datum_t * m, gnutls_datum_t * e);

#define gnutls_pubkey_get_pk_dsa_raw gnutls_pubkey_export_dsa_raw
int gnutls_pubkey_export_dsa_raw(gnutls_pubkey_t key,
				 gnutls_datum_t * p,
				 gnutls_datum_t * q,
				 gnutls_datum_t * g, gnutls_datum_t * y);

#define gnutls_pubkey_get_pk_ecc_raw gnutls_pubkey_export_ecc_raw
int gnutls_pubkey_export_ecc_raw(gnutls_pubkey_t key,
				 gnutls_ecc_curve_t * curve,
				 gnutls_datum_t * x, gnutls_datum_t * y);

#define gnutls_pubkey_get_pk_ecc_x962 gnutls_pubkey_export_ecc_x962
int gnutls_pubkey_export_ecc_x962(gnutls_pubkey_t key,
				  gnutls_datum_t * parameters,
				  gnutls_datum_t * ecpoint);

int gnutls_pubkey_export(gnutls_pubkey_t key,
			 gnutls_x509_crt_fmt_t format,
			 void *output_data, size_t * output_data_size);

int gnutls_pubkey_export2(gnutls_pubkey_t key,
			  gnutls_x509_crt_fmt_t format,
			  gnutls_datum_t * out);

int gnutls_pubkey_get_key_id(gnutls_pubkey_t key,
			     unsigned int flags,
			     unsigned char *output_data,
			     size_t * output_data_size);

int
gnutls_pubkey_get_openpgp_key_id(gnutls_pubkey_t key,
				 unsigned int flags,
				 unsigned char *output_data,
				 size_t * output_data_size,
				 unsigned int *subkey);

int gnutls_pubkey_get_key_usage(gnutls_pubkey_t key, unsigned int *usage);
int gnutls_pubkey_set_key_usage(gnutls_pubkey_t key, unsigned int usage);

int gnutls_pubkey_import(gnutls_pubkey_t key,
			 const gnutls_datum_t * data,
			 gnutls_x509_crt_fmt_t format);


#define gnutls_pubkey_import_pkcs11_url(key, url, flags) gnutls_pubkey_import_url(key, url, flags)

int gnutls_pubkey_import_dsa_raw(gnutls_pubkey_t key,
				 const gnutls_datum_t * p,
				 const gnutls_datum_t * q,
				 const gnutls_datum_t * g,
				 const gnutls_datum_t * y);
int gnutls_pubkey_import_rsa_raw(gnutls_pubkey_t key,
				 const gnutls_datum_t * m,
				 const gnutls_datum_t * e);

int
gnutls_pubkey_import_ecc_x962(gnutls_pubkey_t key,
			      const gnutls_datum_t * parameters,
			      const gnutls_datum_t * ecpoint);

int
gnutls_pubkey_import_ecc_raw(gnutls_pubkey_t key,
			     gnutls_ecc_curve_t curve,
			     const gnutls_datum_t * x,
			     const gnutls_datum_t * y);

int
gnutls_pubkey_encrypt_data(gnutls_pubkey_t key,
			   unsigned int flags,
			   const gnutls_datum_t * plaintext,
			   gnutls_datum_t * ciphertext);

int gnutls_x509_crt_set_pubkey(gnutls_x509_crt_t crt, gnutls_pubkey_t key);

int gnutls_x509_crq_set_pubkey(gnutls_x509_crq_t crq, gnutls_pubkey_t key);

int
gnutls_pubkey_verify_hash2(gnutls_pubkey_t key,
			   gnutls_sign_algorithm_t algo,
			   unsigned int flags,
			   const gnutls_datum_t * hash,
			   const gnutls_datum_t * signature);

int
gnutls_pubkey_verify_data2(gnutls_pubkey_t pubkey,
			   gnutls_sign_algorithm_t algo,
			   unsigned int flags,
			   const gnutls_datum_t * data,
			   const gnutls_datum_t * signature);

/* Private key operations */

int gnutls_privkey_init(gnutls_privkey_t * key);
void gnutls_privkey_deinit(gnutls_privkey_t key);

/* macros to allow specifying a subgroup and group size in gnutls_privkey_generate()
 * and gnutls_x509_privkey_generate() */
#define GNUTLS_SUBGROUP_TO_BITS(group, subgroup) (unsigned int)((subgroup<<16)|(group))
#define GNUTLS_BITS_TO_SUBGROUP(bits) ((bits >> 16) & 0xFFFF)
#define GNUTLS_BITS_TO_GROUP(bits) (bits & 0xFFFF)
#define GNUTLS_BITS_HAVE_SUBGROUP(bits) ((bits) & 0xFFFF0000)

int
gnutls_privkey_generate (gnutls_privkey_t key,
                         gnutls_pk_algorithm_t algo, unsigned int bits,
                         unsigned int flags);

int gnutls_privkey_verify_params(gnutls_privkey_t key);

void gnutls_privkey_set_pin_function (gnutls_privkey_t key,
                                      gnutls_pin_callback_t fn, void *userdata);

int gnutls_privkey_get_pk_algorithm(gnutls_privkey_t key,
				    unsigned int *bits);
gnutls_privkey_type_t gnutls_privkey_get_type(gnutls_privkey_t key);
int gnutls_privkey_status(gnutls_privkey_t key);

/**
 * gnutls_privkey_flags:
 * @GNUTLS_PRIVKEY_SIGN_FLAG_TLS1_RSA: Make an RSA signature on the hashed data as in the TLS protocol.
 * @GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE: When importing a private key, automatically
 *   release it when the structure it was imported is released.
 * @GNUTLS_PRIVKEY_IMPORT_COPY: Copy required values during import.
 * @GNUTLS_PRIVKEY_DISABLE_CALLBACKS: The following flag disables call to PIN callbacks etc.
 *   Only relevant to TPM keys.
 *
 * Enumeration of different certificate import flags.
 */
typedef enum gnutls_privkey_flags {
	GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE = 1,
	GNUTLS_PRIVKEY_IMPORT_COPY = 1 << 1,
	GNUTLS_PRIVKEY_DISABLE_CALLBACKS = 1 << 2,
	GNUTLS_PRIVKEY_SIGN_FLAG_TLS1_RSA = 1 << 4
} gnutls_privkey_flags_t;

int gnutls_privkey_import_pkcs11(gnutls_privkey_t pkey,
				 gnutls_pkcs11_privkey_t key,
				 unsigned int flags);
int gnutls_privkey_import_x509(gnutls_privkey_t pkey,
			       gnutls_x509_privkey_t key,
			       unsigned int flags);
int gnutls_privkey_import_openpgp(gnutls_privkey_t pkey,
				  gnutls_openpgp_privkey_t key,
				  unsigned int flags);

int gnutls_privkey_export_x509(gnutls_privkey_t pkey,
                               gnutls_x509_privkey_t * key);
int gnutls_privkey_export_openpgp(gnutls_privkey_t pkey,
                                  gnutls_openpgp_privkey_t * key);
int
gnutls_privkey_export_pkcs11(gnutls_privkey_t pkey,
                             gnutls_pkcs11_privkey_t *key);

int gnutls_privkey_import_openpgp_raw(gnutls_privkey_t pkey,
				      const gnutls_datum_t * data,
				      gnutls_openpgp_crt_fmt_t
				      format,
				      const gnutls_openpgp_keyid_t
				      keyid, const char *password);

int gnutls_privkey_import_x509_raw(gnutls_privkey_t pkey,
				   const gnutls_datum_t * data,
				   gnutls_x509_crt_fmt_t format,
				   const char *password,
				   unsigned int flags);

int
gnutls_privkey_import_tpm_raw(gnutls_privkey_t pkey,
			      const gnutls_datum_t * fdata,
			      gnutls_tpmkey_fmt_t format,
			      const char *srk_password,
			      const char *key_password,
			      unsigned int flags);

int
gnutls_privkey_import_tpm_url(gnutls_privkey_t pkey,
			      const char *url,
			      const char *srk_password,
			      const char *key_password,
			      unsigned int flags);

int gnutls_privkey_import_url(gnutls_privkey_t key,
			      const char *url, unsigned int flags);

#if 0
/* for documentation purposes */
int gnutls_privkey_import_pkcs11_url(gnutls_privkey_t key, const char *url);
#endif

#define gnutls_privkey_import_pkcs11_url(key, url) gnutls_privkey_import_url(key, url, 0)

int
gnutls_privkey_import_ext(gnutls_privkey_t pkey,
			  gnutls_pk_algorithm_t pk,
			  void *userdata,
			  gnutls_privkey_sign_func sign_func,
			  gnutls_privkey_decrypt_func
			  decrypt_func, unsigned int flags);

int
gnutls_privkey_import_ext2(gnutls_privkey_t pkey,
			   gnutls_pk_algorithm_t pk,
			   void *userdata,
			   gnutls_privkey_sign_func sign_func,
			   gnutls_privkey_decrypt_func
			   decrypt_func,
			   gnutls_privkey_deinit_func deinit_func,
			   unsigned int flags);

int
gnutls_privkey_import_ext3(gnutls_privkey_t pkey,
                           void *userdata,
                           gnutls_privkey_sign_func sign_func,
                           gnutls_privkey_decrypt_func decrypt_func,
                           gnutls_privkey_deinit_func deinit_func,
                           gnutls_privkey_info_func info_func,
                           unsigned int flags);

int gnutls_privkey_import_dsa_raw(gnutls_privkey_t key,
				       const gnutls_datum_t * p,
				       const gnutls_datum_t * q,
				       const gnutls_datum_t * g,
				       const gnutls_datum_t * y,
				       const gnutls_datum_t * x);

int gnutls_privkey_import_rsa_raw(gnutls_privkey_t key,
					const gnutls_datum_t * m,
					const gnutls_datum_t * e,
					const gnutls_datum_t * d,
					const gnutls_datum_t * p,
					const gnutls_datum_t * q,
					const gnutls_datum_t * u,
					const gnutls_datum_t * e1,
					const gnutls_datum_t * e2);
int gnutls_privkey_import_ecc_raw(gnutls_privkey_t key,
				       gnutls_ecc_curve_t curve,
				       const gnutls_datum_t * x,
				       const gnutls_datum_t * y,
				       const gnutls_datum_t * k);


int gnutls_privkey_sign_data(gnutls_privkey_t signer,
			     gnutls_digest_algorithm_t hash,
			     unsigned int flags,
			     const gnutls_datum_t * data,
			     gnutls_datum_t * signature);

#define gnutls_privkey_sign_raw_data(key, flags, data, sig) \
	gnutls_privkey_sign_hash ( key, 0, GNUTLS_PRIVKEY_SIGN_FLAG_TLS1_RSA, data, sig)

int gnutls_privkey_sign_hash(gnutls_privkey_t signer,
			     gnutls_digest_algorithm_t hash_algo,
			     unsigned int flags,
			     const gnutls_datum_t * hash_data,
			     gnutls_datum_t * signature);


int gnutls_privkey_decrypt_data(gnutls_privkey_t key,
				unsigned int flags,
				const gnutls_datum_t * ciphertext,
				gnutls_datum_t * plaintext);

int
gnutls_privkey_export_rsa_raw(gnutls_privkey_t key,
				    gnutls_datum_t * m, gnutls_datum_t * e,
				    gnutls_datum_t * d, gnutls_datum_t * p,
				    gnutls_datum_t * q, gnutls_datum_t * u,
				    gnutls_datum_t * e1,
				    gnutls_datum_t * e2);

int
gnutls_privkey_export_dsa_raw(gnutls_privkey_t key,
			     gnutls_datum_t * p, gnutls_datum_t * q,
			     gnutls_datum_t * g, gnutls_datum_t * y,
			     gnutls_datum_t * x);

int
gnutls_privkey_export_ecc_raw(gnutls_privkey_t key,
				       gnutls_ecc_curve_t * curve,
				       gnutls_datum_t * x,
				       gnutls_datum_t * y,
				       gnutls_datum_t * k);


int gnutls_x509_crt_privkey_sign(gnutls_x509_crt_t crt,
				 gnutls_x509_crt_t issuer,
				 gnutls_privkey_t issuer_key,
				 gnutls_digest_algorithm_t dig,
				 unsigned int flags);

int gnutls_x509_crl_privkey_sign(gnutls_x509_crl_t crl,
				 gnutls_x509_crt_t issuer,
				 gnutls_privkey_t issuer_key,
				 gnutls_digest_algorithm_t dig,
				 unsigned int flags);

int gnutls_x509_crq_privkey_sign(gnutls_x509_crq_t crq,
				 gnutls_privkey_t key,
				 gnutls_digest_algorithm_t dig,
				 unsigned int flags);



/* Do not initialize the "cert" element of
 * the certificate */
#define GNUTLS_PCERT_NO_CERT 1

int gnutls_pcert_import_x509(gnutls_pcert_st * pcert,
			     gnutls_x509_crt_t crt, unsigned int flags);

int gnutls_pcert_import_x509_list(gnutls_pcert_st * pcert,
			     	  gnutls_x509_crt_t *crt, unsigned *ncrt,
			     	  unsigned int flags);

int gnutls_pcert_export_x509(gnutls_pcert_st * pcert,
                             gnutls_x509_crt_t * crt);

int
gnutls_pcert_list_import_x509_raw(gnutls_pcert_st * pcerts,
				  unsigned int *pcert_max,
				  const gnutls_datum_t * data,
				  gnutls_x509_crt_fmt_t format,
				  unsigned int flags);

int gnutls_pcert_import_x509_raw(gnutls_pcert_st * pcert,
				 const gnutls_datum_t * cert,
				 gnutls_x509_crt_fmt_t format,
				 unsigned int flags);

int gnutls_pcert_import_openpgp_raw(gnutls_pcert_st * pcert,
				    const gnutls_datum_t * cert,
				    gnutls_openpgp_crt_fmt_t
				    format,
				    gnutls_openpgp_keyid_t keyid,
				    unsigned int flags);

int gnutls_pcert_import_openpgp(gnutls_pcert_st * pcert,
				gnutls_openpgp_crt_t crt,
				unsigned int flags);

int gnutls_pcert_export_openpgp(gnutls_pcert_st * pcert,
                                gnutls_openpgp_crt_t * crt);

void gnutls_pcert_deinit(gnutls_pcert_st * pcert);

/* For certificate credentials */
	/* This is the same as gnutls_certificate_retrieve_function()
	 * but retrieves a gnutls_pcert_st which requires much less processing
	 * within the library.
	 */
typedef int gnutls_certificate_retrieve_function2(gnutls_session_t,
				  const gnutls_datum_t *req_ca_rdn,
				  int nreqs,
				  const gnutls_pk_algorithm_t *pk_algos,
				  int pk_algos_length,
				  gnutls_pcert_st**,
				  unsigned int *pcert_length,
				  gnutls_privkey_t *privkey);


void gnutls_certificate_set_retrieve_function2
    (gnutls_certificate_credentials_t cred,
     gnutls_certificate_retrieve_function2 * func);

int
gnutls_certificate_set_key(gnutls_certificate_credentials_t res,
			   const char **names,
			   int names_size,
			   gnutls_pcert_st * pcert_list,
			   int pcert_list_size, gnutls_privkey_t key);

int
gnutls_pubkey_print(gnutls_pubkey_t pubkey,
		    gnutls_certificate_print_formats_t format,
		    gnutls_datum_t * out);


struct gnutls_privkey_st {
	gnutls_privkey_type_t type;
	gnutls_pk_algorithm_t pk_algorithm;

	union {
		gnutls_x509_privkey_t x509;
#ifdef ENABLE_PKCS11
		gnutls_pkcs11_privkey_t pkcs11;
#endif
#ifdef ENABLE_OPENPGP
		gnutls_openpgp_privkey_t openpgp;
#endif

		struct {
			gnutls_privkey_sign_func sign_func;
			gnutls_privkey_decrypt_func decrypt_func;
			gnutls_privkey_deinit_func deinit_func;
			gnutls_privkey_info_func info_func;
			void *userdata;
		} ext;
	} key;

	unsigned int flags;
	gnutls_sign_algorithm_t preferred_sign_algo;
	struct pin_info_st pin;
};


typedef struct gnutls_x509_dn_st {
	ASN1_TYPE asn;
} gnutls_x509_dn_st;

typedef struct gnutls_x509_crt_int {
	ASN1_TYPE cert;
	int use_extensions;
	int expanded;		/* a certificate has been expanded */

						/* These two cached values allow fast calls to
						* get_raw_*_dn(). */
	gnutls_datum_t raw_dn;
	gnutls_datum_t raw_issuer_dn;
	gnutls_datum_t raw_spki;

	gnutls_datum_t der;
	struct pin_info_st pin;

	/* backwards compatibility for gnutls_x509_crt_get_subject()
	* and gnutls_x509_crt_get_issuer() */
	gnutls_x509_dn_st dn;
	gnutls_x509_dn_st idn;
} gnutls_x509_crt_int;

typedef struct gnutls_x509_privkey_int {
	/* the size of params depends on the public
	* key algorithm
	*/
	gnutls_pk_params_st params;

	gnutls_pk_algorithm_t pk_algorithm;
	unsigned expanded;

	ASN1_TYPE key;
	struct pin_info_st pin;
} gnutls_x509_privkey_int;


int _gnutls_privkey_get_public_mpis( gnutls_privkey_t key,
									 gnutls_pk_params_st * );

int pubkey_to_bits( gnutls_pk_algorithm_t pk, gnutls_pk_params_st * params );
int _gnutls_pubkey_compatible_with_sig( gnutls_session_t,
										gnutls_pubkey_t pubkey,
										const version_entry_st * ver,
										gnutls_sign_algorithm_t sign );
int
_gnutls_pubkey_get_mpis( gnutls_pubkey_t key, gnutls_pk_params_st * params );

int
pubkey_verify_hashed_data( gnutls_pk_algorithm_t pk,
						   const mac_entry_st * algo,
						   const gnutls_datum_t * hash,
						   const gnutls_datum_t * signature,
						   gnutls_pk_params_st * issuer_params );

int pubkey_verify_data( gnutls_pk_algorithm_t pk,
						const mac_entry_st * algo,
						const gnutls_datum_t * data,
						const gnutls_datum_t * signature,
						gnutls_pk_params_st * issuer_params );



const mac_entry_st *_gnutls_dsa_q_to_hash( gnutls_pk_algorithm_t algo,
										   const gnutls_pk_params_st *
										   params, unsigned int *hash_len );

int
_gnutls_privkey_get_mpis( gnutls_privkey_t key, gnutls_pk_params_st * params );

gnutls_sign_algorithm_t
_gnutls_privkey_get_preferred_sign_algo( gnutls_privkey_t key );


int _gnutls_x509_verify_algorithm( gnutls_digest_algorithm_t * hash,
							   const gnutls_datum_t * signature,
							   gnutls_pk_algorithm_t pk,
							   gnutls_pk_params_st * issuer_params );
int _gnutls_x509_verify_data( const mac_entry_st * me,
							  const gnutls_datum_t * data,
							  const gnutls_datum_t * signature,
							  gnutls_x509_crt_t issuer );
int _gnutls_asn1_encode_privkey( gnutls_pk_algorithm_t pk, ASN1_TYPE * c2,
								 gnutls_pk_params_st * params );

/* extensions.c */
int _gnutls_x509_crl_get_extension_oid( gnutls_x509_crl_t crl,
										int indx, void *oid,
										size_t * sizeof_oid );

int _gnutls_x509_crl_set_extension( gnutls_x509_crl_t crl,
									const char *ext_id,
									const gnutls_datum_t * ext_data,
									unsigned int critical );

int
_gnutls_x509_crl_get_extension( gnutls_x509_crl_t crl,
								const char *extension_id, int indx,
								gnutls_datum_t * data,
								unsigned int *critical );

int
_gnutls_x509_crt_get_extension( gnutls_x509_crt_t cert,
								const char *extension_id, int indx,
								gnutls_datum_t * data, unsigned int *critical );

int _gnutls_x509_crt_get_extension_oid( gnutls_x509_crt_t cert,
										int indx, void *ret,
										size_t * ret_size );
int _gnutls_x509_crt_set_extension( gnutls_x509_crt_t cert,
									const char *extension_id,
									const gnutls_datum_t * ext_data,
									unsigned int critical );

int
_gnutls_x509_ext_extract_number( uint8_t * number,
								 size_t * nr_size,
								 uint8_t * extnValue, int extnValueLen );
int
_gnutls_x509_ext_gen_number( const uint8_t * nuber, size_t nr_size,
							 gnutls_datum_t * der_ext );


int
_gnutls_write_general_name( ASN1_TYPE ext, const char *ext_name,
							gnutls_x509_subject_alt_name_t type,
							const void *data, unsigned int data_size );

int _gnutls_x509_ext_gen_subject_alt_name( gnutls_x509_subject_alt_name_t
										   type, const void *data,
										   unsigned int data_size,
										   const gnutls_datum_t * prev_der_ext,
										   gnutls_datum_t * der_ext );
int _gnutls_x509_ext_gen_auth_key_id( const void *id, size_t id_size,
									  gnutls_datum_t * der_data );

/* mpi.c */
int _gnutls_x509_crq_get_mpis( gnutls_x509_crq_t cert,
							   gnutls_pk_params_st * );

int _gnutls_x509_crt_get_mpis( gnutls_x509_crt_t cert,
							   gnutls_pk_params_st * params );

int _gnutls_x509_read_pubkey_params( gnutls_pk_algorithm_t, uint8_t * der,
									 int dersize,
									 gnutls_pk_params_st * params );

int _gnutls_x509_read_pubkey( gnutls_pk_algorithm_t, uint8_t * der,
							  int dersize, gnutls_pk_params_st * params );

int _gnutls_x509_write_ecc_params( gnutls_ecc_curve_t curve,
								   gnutls_datum_t * der );
int _gnutls_x509_write_ecc_pubkey( gnutls_pk_params_st * params,
								   gnutls_datum_t * der );

int
_gnutls_x509_write_pubkey_params( gnutls_pk_algorithm_t algo,
								  gnutls_pk_params_st * params,
								  gnutls_datum_t * der );
int _gnutls_x509_write_pubkey( gnutls_pk_algorithm_t,
							   gnutls_pk_params_st * params,
							   gnutls_datum_t * der );

int _gnutls_x509_read_uint( ASN1_TYPE node, const char *value,
							unsigned int *ret );

int _gnutls_x509_read_der_int( uint8_t * der, int dersize, bigint_t * out );

int _gnutls_x509_read_int( ASN1_TYPE node, const char *value,
						   bigint_t * ret_mpi );
int _gnutls_x509_write_int( ASN1_TYPE node, const char *value, bigint_t mpi,
							int lz );

int _gnutls_x509_read_key_int( ASN1_TYPE node, const char *value,
							   bigint_t * ret_mpi );
int _gnutls_x509_write_key_int( ASN1_TYPE node, const char *value, bigint_t mpi,
								int lz );

int _gnutls_x509_write_uint32( ASN1_TYPE node, const char *value,
							   uint32_t num );

int _gnutls_x509_write_sig_params( ASN1_TYPE dst, const char *dst_name,
								   gnutls_pk_algorithm_t pk_algorithm,
								   gnutls_digest_algorithm_t, unsigned legacy );
void gnutls_x509_crt_set_pin_function( gnutls_x509_crt_t crt,
									   gnutls_pin_callback_t fn,
									   void *userdata );
void gnutls_x509_privkey_set_pin_function( gnutls_x509_privkey_t key,
										   gnutls_pin_callback_t fn,
										   void *userdata );

int gnutls_x509_privkey_cpy( gnutls_x509_privkey_t dst,
							 gnutls_x509_privkey_t src );
int gnutls_x509_privkey_import( gnutls_x509_privkey_t key,
								const gnutls_datum_t * data,
								gnutls_x509_crt_fmt_t format );
int gnutls_x509_privkey_import_pkcs8( gnutls_x509_privkey_t key,
									  const gnutls_datum_t * data,
									  gnutls_x509_crt_fmt_t format,
									  const char *password,
									  unsigned int flags );
int gnutls_x509_privkey_import_openssl( gnutls_x509_privkey_t key,
										const gnutls_datum_t * data,
										const char *password );

int
gnutls_pkcs8_info( const gnutls_datum_t * data, gnutls_x509_crt_fmt_t format,
				   unsigned int *schema, unsigned int *cipher,
				   void *salt, unsigned int *salt_size,
				   unsigned int *iter_count, char **oid );

int gnutls_x509_privkey_import2( gnutls_x509_privkey_t key,
								 const gnutls_datum_t * data,
								 gnutls_x509_crt_fmt_t format,
								 const char *password, unsigned int flags );

int gnutls_x509_privkey_import_rsa_raw( gnutls_x509_privkey_t key,
										const gnutls_datum_t * m,
										const gnutls_datum_t * e,
										const gnutls_datum_t * d,
										const gnutls_datum_t * p,
										const gnutls_datum_t * q,
										const gnutls_datum_t * u );
int gnutls_x509_privkey_import_rsa_raw2( gnutls_x509_privkey_t key,
										 const gnutls_datum_t * m,
										 const gnutls_datum_t * e,
										 const gnutls_datum_t * d,
										 const gnutls_datum_t * p,
										 const gnutls_datum_t * q,
										 const gnutls_datum_t * u,
										 const gnutls_datum_t * e1,
										 const gnutls_datum_t * e2 );
int gnutls_x509_privkey_import_ecc_raw( gnutls_x509_privkey_t key,
										gnutls_ecc_curve_t curve,
										const gnutls_datum_t * x,
										const gnutls_datum_t * y,
										const gnutls_datum_t * k );

int gnutls_x509_privkey_fix( gnutls_x509_privkey_t key );

int gnutls_x509_privkey_export_dsa_raw( gnutls_x509_privkey_t key,
										gnutls_datum_t * p,
										gnutls_datum_t * q,
										gnutls_datum_t * g,
										gnutls_datum_t * y,
										gnutls_datum_t * x );
int gnutls_x509_privkey_import_dsa_raw( gnutls_x509_privkey_t key,
										const gnutls_datum_t * p,
										const gnutls_datum_t * q,
										const gnutls_datum_t * g,
										const gnutls_datum_t * y,
										const gnutls_datum_t * x );

int gnutls_x509_privkey_get_pk_algorithm( gnutls_x509_privkey_t key );
int gnutls_x509_privkey_get_pk_algorithm2( gnutls_x509_privkey_t
										   key, unsigned int *bits );
int gnutls_x509_privkey_get_key_id( gnutls_x509_privkey_t key,
									unsigned int flags,
									unsigned char *output_data,
									size_t * output_data_size );

int gnutls_x509_privkey_generate( gnutls_x509_privkey_t key,
								  gnutls_pk_algorithm_t algo,
								  unsigned int bits, unsigned int flags );
int gnutls_x509_privkey_verify_params( gnutls_x509_privkey_t key );

int gnutls_x509_privkey_export( gnutls_x509_privkey_t key,
								gnutls_x509_crt_fmt_t format,
								void *output_data,
								size_t * output_data_size );
int gnutls_x509_privkey_export2( gnutls_x509_privkey_t key,
								 gnutls_x509_crt_fmt_t format,
								 gnutls_datum_t * out );
int gnutls_x509_privkey_export_pkcs8( gnutls_x509_privkey_t key,
									  gnutls_x509_crt_fmt_t format,
									  const char *password,
									  unsigned int flags,
									  void *output_data,
									  size_t * output_data_size );
int gnutls_x509_privkey_export2_pkcs8( gnutls_x509_privkey_t key,
									   gnutls_x509_crt_fmt_t format,
									   const char *password,
									   unsigned int flags,
									   gnutls_datum_t * out );
int gnutls_x509_privkey_export_rsa_raw2( gnutls_x509_privkey_t key,
										 gnutls_datum_t * m,
										 gnutls_datum_t * e,
										 gnutls_datum_t * d,
										 gnutls_datum_t * p,
										 gnutls_datum_t * q,
										 gnutls_datum_t * u,
										 gnutls_datum_t * e1,
										 gnutls_datum_t * e2 );
int gnutls_x509_privkey_export_rsa_raw( gnutls_x509_privkey_t key,
										gnutls_datum_t * m,
										gnutls_datum_t * e,
										gnutls_datum_t * d,
										gnutls_datum_t * p,
										gnutls_datum_t * q,
										gnutls_datum_t * u );
int gnutls_x509_privkey_export_ecc_raw( gnutls_x509_privkey_t key,
										gnutls_ecc_curve_t * curve,
										gnutls_datum_t * x,
										gnutls_datum_t * y,
										gnutls_datum_t * k );

int gnutls_x509_privkey_sign_data( gnutls_x509_privkey_t key,
								   gnutls_digest_algorithm_t digest,
								   unsigned int flags,
								   const gnutls_datum_t * data,
								   void *signature,
								   size_t * signature_size );

/* Certificate request stuff.
*/

int gnutls_x509_crq_sign2( gnutls_x509_crq_t crq,
						   gnutls_x509_privkey_t key,
						   gnutls_digest_algorithm_t dig,
						   unsigned int flags );

int gnutls_x509_crq_print( gnutls_x509_crq_t crq,
						   gnutls_certificate_print_formats_t
						   format, gnutls_datum_t * out );

int gnutls_x509_crq_verify( gnutls_x509_crq_t crq, unsigned int flags );

int gnutls_x509_crq_init( gnutls_x509_crq_t * crq );
void gnutls_x509_crq_deinit( gnutls_x509_crq_t crq );
int gnutls_x509_crq_import( gnutls_x509_crq_t crq,
							const gnutls_datum_t * data,
							gnutls_x509_crt_fmt_t format );

int gnutls_x509_crq_get_private_key_usage_period( gnutls_x509_crq_t
												  cert,
												  time_t *
												  activation,
												  time_t *
												  expiration, unsigned int
												  *critical );

int gnutls_x509_crq_get_dn( gnutls_x509_crq_t crq, char *buf,
							size_t * sizeof_buf );
int gnutls_x509_crq_get_dn2( gnutls_x509_crq_t crq, gnutls_datum_t * dn );
int gnutls_x509_crq_get_dn_oid( gnutls_x509_crq_t crq, int indx,
								void *oid, size_t * sizeof_oid );
int gnutls_x509_crq_get_dn_by_oid( gnutls_x509_crq_t crq,
								   const char *oid, int indx,
								   unsigned int raw_flag, void *buf,
								   size_t * sizeof_buf );
int gnutls_x509_crq_set_dn( gnutls_x509_crq_t crq, const char *dn,
							const char **err );
int gnutls_x509_crq_set_dn_by_oid( gnutls_x509_crq_t crq,
								   const char *oid,
								   unsigned int raw_flag,
								   const void *data,
								   unsigned int sizeof_data );
int gnutls_x509_crq_set_version( gnutls_x509_crq_t crq,
								 unsigned int version );
int gnutls_x509_crq_get_version( gnutls_x509_crq_t crq );
int gnutls_x509_crq_set_key( gnutls_x509_crq_t crq,
							 gnutls_x509_privkey_t key );

int gnutls_x509_crq_set_challenge_password( gnutls_x509_crq_t crq,
											const char *pass );
int gnutls_x509_crq_get_challenge_password( gnutls_x509_crq_t crq,
											char *pass,
											size_t * sizeof_pass );

int gnutls_x509_crq_set_attribute_by_oid( gnutls_x509_crq_t crq,
										  const char *oid,
										  void *buf, size_t sizeof_buf );
int gnutls_x509_crq_get_attribute_by_oid( gnutls_x509_crq_t crq,
										  const char *oid, int indx,
										  void *buf, size_t * sizeof_buf );

int gnutls_x509_crq_export( gnutls_x509_crq_t crq,
							gnutls_x509_crt_fmt_t format,
							void *output_data, size_t * output_data_size );
int gnutls_x509_crq_export2( gnutls_x509_crq_t crq,
							 gnutls_x509_crt_fmt_t format,
							 gnutls_datum_t * out );

int gnutls_x509_crt_set_crq( gnutls_x509_crt_t crt, gnutls_x509_crq_t crq );
int gnutls_x509_crt_set_crq_extensions( gnutls_x509_crt_t crt,
										gnutls_x509_crq_t crq );

int gnutls_x509_crq_set_private_key_usage_period( gnutls_x509_crq_t
												  crq,
												  time_t activation,
												  time_t expiration );
int gnutls_x509_crq_set_key_rsa_raw( gnutls_x509_crq_t crq,
									 const gnutls_datum_t * m,
									 const gnutls_datum_t * e );
int gnutls_x509_crq_set_subject_alt_name( gnutls_x509_crq_t crq,
										  gnutls_x509_subject_alt_name_t
										  nt, const void *data,
										  unsigned int data_size,
										  unsigned int flags );

int gnutls_x509_crq_set_key_usage( gnutls_x509_crq_t crq,
								   unsigned int usage );
int gnutls_x509_crq_set_basic_constraints( gnutls_x509_crq_t crq,
										   unsigned int ca,
										   int pathLenConstraint );
int gnutls_x509_crq_set_key_purpose_oid( gnutls_x509_crq_t crq,
										 const void *oid,
										 unsigned int critical );
int gnutls_x509_crq_get_key_purpose_oid( gnutls_x509_crq_t crq,
										 int indx, void *oid,
										 size_t * sizeof_oid,
										 unsigned int *critical );

int gnutls_x509_crq_get_extension_data( gnutls_x509_crq_t crq,
										int indx, void *data,
										size_t * sizeof_data );
int
gnutls_x509_crq_get_extension_data2( gnutls_x509_crq_t crq,
									 unsigned indx,
									 gnutls_datum_t * data );
int gnutls_x509_crq_get_extension_info( gnutls_x509_crq_t crq,
										int indx, void *oid,
										size_t * sizeof_oid,
										unsigned int *critical );
int gnutls_x509_crq_get_attribute_data( gnutls_x509_crq_t crq,
										int indx, void *data,
										size_t * sizeof_data );
int gnutls_x509_crq_get_attribute_info( gnutls_x509_crq_t crq,
										int indx, void *oid,
										size_t * sizeof_oid );
int gnutls_x509_crq_get_pk_algorithm( gnutls_x509_crq_t crq,
									  unsigned int *bits );

int gnutls_x509_crq_get_key_id( gnutls_x509_crq_t crq,
								unsigned int flags,
								unsigned char *output_data,
								size_t * output_data_size );
int gnutls_x509_crq_get_key_rsa_raw( gnutls_x509_crq_t crq,
									 gnutls_datum_t * m,
									 gnutls_datum_t * e );

int gnutls_x509_crq_get_key_usage( gnutls_x509_crq_t crq,
								   unsigned int *key_usage,
								   unsigned int *critical );
int gnutls_x509_crq_get_basic_constraints( gnutls_x509_crq_t crq,
										   unsigned int *critical,
										   unsigned int *ca, int *pathlen );
int gnutls_x509_crq_get_subject_alt_name( gnutls_x509_crq_t crq,
										  unsigned int seq,
										  void *ret,
										  size_t * ret_size,
										  unsigned int *ret_type,
										  unsigned int *critical );
int gnutls_x509_crq_get_subject_alt_othername_oid( gnutls_x509_crq_t
												   crq,
												   unsigned int seq,
												   void *ret,
												   size_t * ret_size );

int gnutls_x509_crq_get_extension_by_oid( gnutls_x509_crq_t crq,
										  const char *oid, int indx,
										  void *buf,
										  size_t * sizeof_buf,
										  unsigned int *critical );
int
gnutls_x509_crt_get_extension_by_oid2( gnutls_x509_crt_t cert,
									   const char *oid, int indx,
									   gnutls_datum_t *output,
									   unsigned int *critical );

typedef struct gnutls_x509_trust_list_st *gnutls_x509_trust_list_t;
typedef struct gnutls_x509_trust_list_iter *gnutls_x509_trust_list_iter_t;

int
gnutls_x509_trust_list_init( gnutls_x509_trust_list_t * list,
							 unsigned int size );

void
gnutls_x509_trust_list_deinit( gnutls_x509_trust_list_t list,
							   unsigned int all );

int gnutls_x509_trust_list_get_issuer( gnutls_x509_trust_list_t
									   list, gnutls_x509_crt_t cert,
									   gnutls_x509_crt_t * issuer,
									   unsigned int flags );

int gnutls_x509_trust_list_get_issuer_by_dn( gnutls_x509_trust_list_t list,
											 const gnutls_datum_t *dn,
											 gnutls_x509_crt_t *issuer,
											 unsigned int flags );

int gnutls_x509_trust_list_get_issuer_by_subject_key_id( gnutls_x509_trust_list_t list,
														 const gnutls_datum_t *dn,
														 const gnutls_datum_t *spki,
														 gnutls_x509_crt_t *issuer,
														 unsigned int flags );

int
gnutls_x509_trust_list_add_cas( gnutls_x509_trust_list_t list,
								const gnutls_x509_crt_t * clist,
								unsigned clist_size, unsigned int flags );
int gnutls_x509_trust_list_remove_cas( gnutls_x509_trust_list_t
									   list,
									   const gnutls_x509_crt_t *
									   clist, int clist_size );

int gnutls_x509_trust_list_add_named_crt( gnutls_x509_trust_list_t
										  list,
										  gnutls_x509_crt_t cert,
										  const void *name,
										  size_t name_size,
										  unsigned int flags );

int
gnutls_x509_trust_list_add_crls( gnutls_x509_trust_list_t list,
								 const gnutls_x509_crl_t *
								 crl_list, int crl_size,
								 unsigned int flags,
								 unsigned int verification_flags );


int
gnutls_x509_trust_list_iter_get_ca( gnutls_x509_trust_list_t list,
									gnutls_x509_trust_list_iter_t *iter,
									gnutls_x509_crt_t *crt );

void gnutls_x509_trust_list_iter_deinit( gnutls_x509_trust_list_iter_t iter );


int gnutls_x509_trust_list_verify_named_crt( gnutls_x509_trust_list_t list, gnutls_x509_crt_t cert,
											  const void *name, size_t name_size, unsigned int flags,
											  unsigned int *verify_val, gnutls_verify_output_function func );

int gnutls_x509_trust_list_verify_crt2( gnutls_x509_trust_list_t list,
									gnutls_x509_crt_t * cert_list,
									unsigned int cert_list_size,
									gnutls_typed_vdata_st * data,
									unsigned int elements,
									unsigned int flags,
									unsigned int *voutput,
									gnutls_verify_output_function func );

int
gnutls_x509_trust_list_verify_crt( gnutls_x509_trust_list_t list,
								   gnutls_x509_crt_t * cert_list,
								   unsigned int cert_list_size,
								   unsigned int flags,
								   unsigned int *verify,
								   gnutls_verify_output_function func );

/* trust list convenience functions */
int
gnutls_x509_trust_list_add_trust_mem( gnutls_x509_trust_list_t
									  list,
									  const gnutls_datum_t * cas,
									  const gnutls_datum_t * crls,
									  gnutls_x509_crt_fmt_t type,
									  unsigned int tl_flags,
									  unsigned int tl_vflags );

int
gnutls_x509_trust_list_add_trust_file( gnutls_x509_trust_list_t
									   list, const char *ca_file,
									   const char *crl_file,
									   gnutls_x509_crt_fmt_t type,
									   unsigned int tl_flags,
									   unsigned int tl_vflags );

int
gnutls_x509_trust_list_add_trust_dir( gnutls_x509_trust_list_t list,
									  const char *ca_dir,
									  const char *crl_dir,
									  gnutls_x509_crt_fmt_t type,
									  unsigned int tl_flags,
									  unsigned int tl_vflags );

int
gnutls_x509_trust_list_remove_trust_file( gnutls_x509_trust_list_t
										  list,
										  const char *ca_file,
										  gnutls_x509_crt_fmt_t type );

int
gnutls_x509_trust_list_remove_trust_mem( gnutls_x509_trust_list_t
										 list,
										 const gnutls_datum_t *
										 cas, gnutls_x509_crt_fmt_t type );

int
gnutls_x509_trust_list_add_system_trust( gnutls_x509_trust_list_t
										 list,
										 unsigned int tl_flags,
										 unsigned int tl_vflags );

void gnutls_certificate_set_trust_list
( gnutls_certificate_credentials_t res,
  gnutls_x509_trust_list_t tlist, unsigned flags );
void gnutls_certificate_get_trust_list
( gnutls_certificate_credentials_t res,
  gnutls_x509_trust_list_t *tlist );


void gnutls_x509_ext_deinit( gnutls_x509_ext_st *ext );

int
gnutls_x509_ext_print( gnutls_x509_ext_st *exts, unsigned int exts_size,
					   gnutls_certificate_print_formats_t format,
					   gnutls_datum_t * out );
void gnutls_pk_params_release( gnutls_pk_params_st * p );
void gnutls_pk_params_clear( gnutls_pk_params_st * p );
void gnutls_pk_params_init( gnutls_pk_params_st * p );

NLC_END_CDECLARES
#endif // ABSTRACT_GNU_H
