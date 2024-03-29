/*
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

/* This file contains functions which are wrappers for the key exchange
 * part of TLS. They are called by the handshake functions (gnutls_handshake)
 */

#include "gnutls_int.h"
#include "gnutls_handshake.h"
#include "gnutls_kx.h"
#include "gnutls_dh.h"
#include "gnutls_errors.h"
#include <algorithms_gnu.h>
#include "debug.h"
#include "gnutls_mpi.h"
#include <gnutls_state.h>
#include <gnutls_datum.h>
#include <gnutls_mbuffers.h>

/* This is a temporary function to be used before the generate_*
   internal API is changed to use mbuffers. For now we don't avoid the
   extra alloc + memcpy. */
static int
send_handshake(gnutls_session_t session, uint8_t * data, size_t size,
	       gnutls_handshake_description_t type)
{
	mbuffer_st *bufel;

	if (data == NULL && size == 0)
		return _gnutls_send_handshake(session, NULL, type);

	if (data == NULL && size > 0) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	bufel = _gnutls_handshake_alloc(session, size);
	if (bufel == NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	_mbuffer_set_udata(bufel, data, size);

	return _gnutls_send_handshake(session, bufel, type);
}


/* This file contains important thing for the TLS handshake procedure.
 */

#define MASTER_SECRET "master secret"
#define MASTER_SECRET_SIZE (sizeof(MASTER_SECRET)-1)

#define EXT_MASTER_SECRET "extended master secret"
#define EXT_MASTER_SECRET_SIZE (sizeof(EXT_MASTER_SECRET)-1)

static int generate_normal_master(gnutls_session_t session,
				  gnutls_datum_t *, int);

int _gnutls_generate_master(gnutls_session_t session, int keep_premaster)
{
	if (session->internals.resumed == RESUME_FALSE)
		return generate_normal_master(session, &session->key.key,
					      keep_premaster);
	else if (session->internals.premaster_set) {
		gnutls_datum_t premaster;
		premaster.size =
		    sizeof(session->internals.resumed_security_parameters.
			   master_secret);
		premaster.data =
		    session->internals.resumed_security_parameters.
		    master_secret;
		return generate_normal_master(session, &premaster, 1);
	}
	return 0;
}

static void write_nss_key_log(gnutls_session_t session, const gnutls_datum_t *premaster)
{
	char buf[512];
	char buf2[512];
	FILE *fp;
	static const char *keylogfile = NULL;
	static unsigned checked_env = 0;

	if (!checked_env) 
	{
		checked_env = 1;
#ifdef HAVE_SECURE_GETENV
    keylogfile = secure_getenv("SSLKEYLOGFILE");
#else
    keylogfile = getenv("SSLKEYLOGFILE");
#endif // HAVE_SECURE_GETENV
	}

	if (keylogfile == NULL)
		return;

	fp = fopen(keylogfile, "a");
	if (fp == NULL)
		return;

	fprintf(fp, "CLIENT_RANDOM %s %s\n", 
		 _gnutls_bin2hex(session->security_parameters.
				 client_random, 32, buf,
				 sizeof(buf), NULL),
		 _gnutls_bin2hex(session->security_parameters.
				 master_secret, GNUTLS_MASTER_SIZE,
				 buf2, sizeof(buf2), NULL));
	fclose(fp);
}

/* here we generate the TLS Master secret.
 */
static int
generate_normal_master(gnutls_session_t session,
		       gnutls_datum_t * premaster, int keep_premaster)
{
	int ret = 0;
	char buf[512];

	_gnutls_hard_log("INT: PREMASTER SECRET[%d]: %s\n",
			 premaster->size, _gnutls_bin2hex(premaster->data,
							  premaster->size,
							  buf, sizeof(buf),
							  NULL));
	_gnutls_hard_log("INT: CLIENT RANDOM[%d]: %s\n", 32,
			 _gnutls_bin2hex(session->security_parameters.
					 client_random, 32, buf,
					 sizeof(buf), NULL));
	_gnutls_hard_log("INT: SERVER RANDOM[%d]: %s\n", 32,
			 _gnutls_bin2hex(session->security_parameters.
					 server_random, 32, buf,
					 sizeof(buf), NULL));

	if (session->security_parameters.ext_master_secret == 0) {
		uint8_t rnd[2 * GNUTLS_RANDOM_SIZE + 1];
		memcpy(rnd, session->security_parameters.client_random,
		       GNUTLS_RANDOM_SIZE);
		memcpy(&rnd[GNUTLS_RANDOM_SIZE],
		       session->security_parameters.server_random,
		       GNUTLS_RANDOM_SIZE);

		if (get_num_version(session) == GNUTLS_SSL3) {
			ret =
			    _gnutls_ssl3_generate_random(premaster->data,
							 premaster->size, rnd,
							 2 * GNUTLS_RANDOM_SIZE,
							 GNUTLS_MASTER_SIZE,
							 session->security_parameters.
							 master_secret);
		} else {
			ret =
			    _gnutls_PRF(session, premaster->data, premaster->size,
					MASTER_SECRET, MASTER_SECRET_SIZE,
					rnd, 2 * GNUTLS_RANDOM_SIZE,
					GNUTLS_MASTER_SIZE,
					session->security_parameters.
					master_secret);
		}
	} else {
		gnutls_datum_t shash = {NULL, 0};

		/* draft-ietf-tls-session-hash-02 */
		ret = _gnutls_handshake_get_session_hash(session, &shash);
		if (ret < 0)
			return gnutls_assert_val(ret);
		if (get_num_version(session) == GNUTLS_SSL3)
			return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);

		ret =
		    _gnutls_PRF(session, premaster->data, premaster->size,
				EXT_MASTER_SECRET, EXT_MASTER_SECRET_SIZE,
				shash.data, shash.size,
				GNUTLS_MASTER_SIZE,
				session->security_parameters.
				master_secret);

		gnutls_free(shash.data);
	}

	if (!keep_premaster)
		_gnutls_free_temp_key_datum(premaster);

	if (ret < 0)
		return ret;

	_gnutls_hard_log("INT: MASTER SECRET: %s\n",
			 _gnutls_bin2hex(session->security_parameters.
					 master_secret, GNUTLS_MASTER_SIZE,
					 buf, sizeof(buf), NULL));

	write_nss_key_log(session, premaster);

	return ret;
}


/* This is called when we want to receive the key exchange message of the
 * server. It does nothing if this type of message is not required
 * by the selected ciphersuite. 
 */
int _gnutls_send_server_kx_message(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;

	if (session->internals.auth_struct->gnutls_generate_server_kx ==
	    NULL)
		return 0;

	_gnutls_buffer_init(&data);

	if (again == 0) {
		ret =
		    session->internals.auth_struct->
		    gnutls_generate_server_kx(session, &data);

		if (ret == GNUTLS_E_INT_RET_0) {
			gnutls_assert();
			ret = 0;
			goto cleanup;
		}

		if (ret < 0) {
			gnutls_assert();
			goto cleanup;
		}
	}

	ret = send_handshake(session, data.data, data.length,
			     GNUTLS_HANDSHAKE_SERVER_KEY_EXCHANGE);
	if (ret < 0) {
		gnutls_assert();
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}

/* This function sends a certificate request message to the
 * client.
 */
int _gnutls_send_server_crt_request(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;

	if (session->internals.auth_struct->
	    gnutls_generate_server_crt_request == NULL)
		return 0;

	if (session->internals.send_cert_req <= 0)
		return 0;

	_gnutls_buffer_init(&data);

	if (again == 0) {
		ret =
		    session->internals.auth_struct->
		    gnutls_generate_server_crt_request(session, &data);

		if (ret < 0) {
			gnutls_assert();
			goto cleanup;
		}
	}

	ret = send_handshake(session, data.data, data.length,
			     GNUTLS_HANDSHAKE_CERTIFICATE_REQUEST);
	if (ret < 0) {
		gnutls_assert();
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}


/* This is the function for the client to send the key
 * exchange message 
 */
int _gnutls_send_client_kx_message(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;

	if (session->internals.auth_struct->gnutls_generate_client_kx ==
	    NULL)
		return 0;

	_gnutls_buffer_init(&data);

	if (again == 0) {
		ret =
		    session->internals.auth_struct->
		    gnutls_generate_client_kx(session, &data);
		if (ret < 0) {
			gnutls_assert();
			goto cleanup;
		}
	}
	ret = send_handshake(session, data.data, data.length,
			     GNUTLS_HANDSHAKE_CLIENT_KEY_EXCHANGE);
	if (ret < 0) {
		gnutls_assert();
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}


/* This is the function for the client to send the certificate
 * verify message
 */
int
_gnutls_send_client_certificate_verify(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;

	/* This is a packet that is only sent by the client
	 */
	if (session->security_parameters.entity == GNUTLS_SERVER)
		return 0;

	/* if certificate verify is not needed just exit 
	 */
	if (session->key.crt_requested == 0)
		return 0;


	if (session->internals.auth_struct->
	    gnutls_generate_client_crt_vrfy == NULL) {
		gnutls_assert();
		return 0;	/* this algorithm does not support cli_crt_vrfy 
				 */
	}

	_gnutls_buffer_init(&data);

	if (again == 0) {
		ret =
		    session->internals.auth_struct->
		    gnutls_generate_client_crt_vrfy(session, &data);
		if (ret < 0) {
			gnutls_assert();
			goto cleanup;
		}

		if (ret == 0)
			goto cleanup;

	}
	ret = send_handshake(session, data.data, data.length,
			     GNUTLS_HANDSHAKE_CERTIFICATE_VERIFY);

	if (ret < 0) {
		gnutls_assert();
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}

/* This is called when we want send our certificate
 */
int _gnutls_send_client_certificate(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;


	if (session->key.crt_requested == 0)
		return 0;

	if (session->internals.auth_struct->
	    gnutls_generate_client_certificate == NULL)
		return 0;

	_gnutls_buffer_init(&data);

	if (again == 0) {
		if (get_num_version(session) != GNUTLS_SSL3 ||
		    session->internals.selected_cert_list_length > 0) {
			/* TLS 1.0 or SSL 3.0 with a valid certificate 
			 */
			ret =
			    session->internals.auth_struct->
			    gnutls_generate_client_certificate(session,
							       &data);

			if (ret < 0) {
				gnutls_assert();
				goto cleanup;
			}
		}
	}

	/* In the SSL 3.0 protocol we need to send a
	 * no certificate alert instead of an
	 * empty certificate.
	 */
	if (get_num_version(session) == GNUTLS_SSL3 &&
	    session->internals.selected_cert_list_length == 0) {
		ret =
		    gnutls_alert_send(session, GNUTLS_AL_WARNING,
				      GNUTLS_A_SSL3_NO_CERTIFICATE);

	} else {		/* TLS 1.0 or SSL 3.0 with a valid certificate 
				 */
		ret = send_handshake(session, data.data, data.length,
				     GNUTLS_HANDSHAKE_CERTIFICATE_PKT);
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}


/* This is called when we want send our certificate
 */
int _gnutls_send_server_certificate(gnutls_session_t session, int again)
{
	gnutls_buffer_st data;
	int ret = 0;


	if (session->internals.auth_struct->
	    gnutls_generate_server_certificate == NULL)
		return 0;

	_gnutls_buffer_init(&data);

	if (again == 0) {
		ret =
		    session->internals.auth_struct->
		    gnutls_generate_server_certificate(session, &data);

		if (ret < 0) {
			gnutls_assert();
			goto cleanup;
		}
	}
	ret = send_handshake(session, data.data, data.length,
			     GNUTLS_HANDSHAKE_CERTIFICATE_PKT);
	if (ret < 0) {
		gnutls_assert();
	}

      cleanup:
	_gnutls_buffer_clear(&data);
	return ret;
}


int _gnutls_recv_server_kx_message(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;
	unsigned int optflag = 0;

	if (session->internals.auth_struct->gnutls_process_server_kx !=
	    NULL) {
		/* Server key exchange packet is optional for PSK. */
		if (_gnutls_session_is_psk(session))
			optflag = 1;

		ret =
		    _gnutls_recv_handshake(session,
					   GNUTLS_HANDSHAKE_SERVER_KEY_EXCHANGE,
					   optflag, &buf);
		if (ret < 0) {
			gnutls_assert();
			return ret;
		}

		ret =
		    session->internals.auth_struct->
		    gnutls_process_server_kx(session, buf.data,
					     buf.length);
		_gnutls_buffer_clear(&buf);

		if (ret < 0) {
			gnutls_assert();
			return ret;
		}

	}
	return ret;
}

int _gnutls_recv_server_crt_request(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;

	if (session->internals.auth_struct->
	    gnutls_process_server_crt_request != NULL) {

		ret =
		    _gnutls_recv_handshake(session,
					   GNUTLS_HANDSHAKE_CERTIFICATE_REQUEST,
					   1, &buf);
		if (ret < 0)
			return ret;

		if (ret == 0 && buf.length == 0) {
			_gnutls_buffer_clear(&buf);
			return 0;	/* ignored */
		}

		ret =
		    session->internals.auth_struct->
		    gnutls_process_server_crt_request(session, buf.data,
						      buf.length);
		_gnutls_buffer_clear(&buf);
		if (ret < 0)
			return ret;

	}
	return ret;
}

int _gnutls_recv_client_kx_message(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;


	/* Do key exchange only if the algorithm permits it */
	if (session->internals.auth_struct->gnutls_process_client_kx !=
	    NULL) {

		ret =
		    _gnutls_recv_handshake(session,
					   GNUTLS_HANDSHAKE_CLIENT_KEY_EXCHANGE,
					   0, &buf);
		if (ret < 0)
			return ret;

		ret =
		    session->internals.auth_struct->
		    gnutls_process_client_kx(session, buf.data,
					     buf.length);
		_gnutls_buffer_clear(&buf);
		if (ret < 0)
			return ret;

	}

	return ret;
}


int _gnutls_recv_client_certificate(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;
	int optional;

	if (session->internals.auth_struct->
	    gnutls_process_client_certificate == NULL)
		return 0;

	/* if we have not requested a certificate then just return
	 */
	if (session->internals.send_cert_req == 0) {
		return 0;
	}

	if (session->internals.send_cert_req == GNUTLS_CERT_REQUIRE)
		optional = 0;
	else
		optional = 1;

	ret =
	    _gnutls_recv_handshake(session,
				   GNUTLS_HANDSHAKE_CERTIFICATE_PKT,
				   optional, &buf);

	if (ret < 0) {
		/* Handle the case of old SSL3 clients who send
		 * a warning alert instead of an empty certificate to indicate
		 * no certificate.
		 */
		if (optional != 0 &&
		    ret == GNUTLS_E_WARNING_ALERT_RECEIVED &&
		    get_num_version(session) == GNUTLS_SSL3 &&
		    gnutls_alert_get(session) ==
		    GNUTLS_A_SSL3_NO_CERTIFICATE) {

			/* SSL3 does not send an empty certificate,
			 * but this alert. So we just ignore it.
			 */
			gnutls_assert();
			return 0;
		}

		/* certificate was required 
		 */
		if ((ret == GNUTLS_E_WARNING_ALERT_RECEIVED
		     || ret == GNUTLS_E_FATAL_ALERT_RECEIVED)
		    && optional == 0) {
			gnutls_assert();
			return GNUTLS_E_NO_CERTIFICATE_FOUND;
		}

		return ret;
	}

	if (ret == 0 && buf.length == 0 && optional != 0) {
		/* Client has not sent the certificate message.
		 * well I'm not sure we should accept this
		 * behaviour.
		 */
		gnutls_assert();
		ret = 0;
		goto cleanup;
	}
	ret =
	    session->internals.auth_struct->
	    gnutls_process_client_certificate(session, buf.data,
					      buf.length);

	if (ret < 0 && ret != GNUTLS_E_NO_CERTIFICATE_FOUND) {
		gnutls_assert();
		goto cleanup;
	}

	/* ok we should expect a certificate verify message now 
	 */
	if (ret == GNUTLS_E_NO_CERTIFICATE_FOUND && optional != 0)
		ret = 0;
	else
		session->key.crt_requested = 1;

      cleanup:
	_gnutls_buffer_clear(&buf);
	return ret;
}

int _gnutls_recv_server_certificate(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;

	if (session->internals.auth_struct->
	    gnutls_process_server_certificate != NULL) {

		ret =
		    _gnutls_recv_handshake(session,
					   GNUTLS_HANDSHAKE_CERTIFICATE_PKT,
					   0, &buf);
		if (ret < 0) {
			gnutls_assert();
			return ret;
		}

		ret =
		    session->internals.auth_struct->
		    gnutls_process_server_certificate(session, buf.data,
						      buf.length);
		_gnutls_buffer_clear(&buf);
		if (ret < 0) {
			gnutls_assert();
			return ret;
		}
	}

	return ret;
}


/* Recv the client certificate verify. This packet may not
 * arrive if the peer did not send us a certificate.
 */
int
_gnutls_recv_client_certificate_verify_message(gnutls_session_t session)
{
	gnutls_buffer_st buf;
	int ret = 0;


	if (session->internals.auth_struct->
	    gnutls_process_client_crt_vrfy == NULL)
		return 0;

	if (session->internals.send_cert_req == 0 ||
	    session->key.crt_requested == 0) {
		return 0;
	}

	ret =
	    _gnutls_recv_handshake(session,
				   GNUTLS_HANDSHAKE_CERTIFICATE_VERIFY,
				   1, &buf);
	if (ret < 0)
		return ret;

	if (ret == 0 && buf.length == 0
	    && session->internals.send_cert_req == GNUTLS_CERT_REQUIRE) {
		/* certificate was required */
		gnutls_assert();
		ret = GNUTLS_E_NO_CERTIFICATE_FOUND;
		goto cleanup;
	}

	ret =
	    session->internals.auth_struct->
	    gnutls_process_client_crt_vrfy(session, buf.data, buf.length);

      cleanup:
	_gnutls_buffer_clear(&buf);
	return ret;
}
