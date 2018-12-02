/*
 * Copyright © 2014 Red Hat, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * GnuTLS is free software; you can redistribute it and/or
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

#include <gnutls_int.h>
#include <gnutls_errors.h>
#include <libgnu/gnutls.h>
#include <libgnu/abstract_gnu.h>
#include <libgnu/pkcs12.h>
#include <libgnu/system-keys.h>
#include "system-keys.h"
#include <gnutls_sig.h>
#include <gnutls_pk.h>

void gnutls_system_key_iter_deinit(gnutls_system_key_iter_t iter)
{
	return;
}

int
gnutls_system_key_iter_get_info(gnutls_system_key_iter_t *iter,
			        unsigned cert_type,
			        char **cert_url,
			        char **key_url,
			        char **label,
			        gnutls_datum_t *der,
			        unsigned int flags)
{
	return GNUTLS_E_UNIMPLEMENTED_FEATURE;
}

int gnutls_system_key_delete(const char *cert_url, const char *key_url)
{
	return GNUTLS_E_UNIMPLEMENTED_FEATURE;
}

int gnutls_system_key_add_x509(gnutls_x509_crt_t crt, gnutls_x509_privkey_t privkey,
				const char *label, char **cert_url, char **key_url)
{
	return GNUTLS_E_UNIMPLEMENTED_FEATURE;
}

int
_gnutls_privkey_import_system_url(gnutls_privkey_t pkey,
			          const char *url)
{
	return GNUTLS_E_UNIMPLEMENTED_FEATURE;
}

int
_gnutls_x509_crt_import_system_url(gnutls_x509_crt_t crt, const char *url)
{
	return GNUTLS_E_UNIMPLEMENTED_FEATURE;
}

int _gnutls_system_key_init(void)
{
	return 0;
}

void _gnutls_system_key_deinit(void)
{
	return;
}
