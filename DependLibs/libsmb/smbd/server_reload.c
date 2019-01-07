/*
   Unix SMB/CIFS implementation.
   Main SMB server routines
   Copyright (C) Andrew Tridgell		1992-1998
   Copyright (C) Martin Pool			2002
   Copyright (C) Jelmer Vernooij		2002-2003
   Copyright (C) Volker Lendecke		1993-2007
   Copyright (C) Jeremy Allison			1993-2007

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"
#include "smbd/smbd.h"
#include "smbd/globals.h"
#include "nt_printing.h"
#include "printing/pcap.h"
#include "printing/load.h"
#include "auth.h"
#include "messages.h"

/****************************************************************************
 purge stale printers and reload from pre-populated pcap cache
**************************************************************************/
void reload_printers(struct tevent_context *ev,
		     struct messaging_context *msg_ctx)
{
	struct auth_serversupplied_info *session_info = NULL;
	struct spoolss_PrinterInfo2 *pinfo2 = NULL;
	int n_services;
	int pnum;
	int snum;
	const char *pname;
	const char *sname;
	NTSTATUS status;

	load_printers(ev, msg_ctx);

	n_services = lp_numservices();
	pnum = lp_servicenumber(PRINTERS_NAME);

	DEBUG_OUT (10, ("reloading printer services from pcap cache\n"));

	status = make_session_info_system(talloc_tos(), &session_info);
	if (!NT_STATUS_IS_OK(status)) {
		DEBUG_OUT (3, ("reload_printers: "
			  "Could not create system session_info\n"));
		/* can't remove stale printers before we
		 * are fully initilized */
		return;
	}

	/*
	 * Add default config for printers added to smb.conf file and remove
	 * stale printers
	 */
	for (snum = 0; snum < n_services; snum++) {
		/* avoid removing PRINTERS_NAME */
		if (snum == pnum) {
			continue;
		}

		/* skip no-printer services */
		if (!(lp_snum_ok(snum) && lp_print_ok(snum))) {
			continue;
		}

		sname = lp_const_servicename(snum);
		pname = lp_printername(snum);

		/* check printer, but avoid removing non-autoloaded printers */
		if (lp_autoloaded(snum) && !pcap_printername_ok(pname)) {
			DEBUG_OUT (3, ("removing stale printer %s\n", pname));

			if (is_printer_published(session_info, session_info,
						 msg_ctx,
						 NULL, lp_servicename(snum),
						 NULL, &pinfo2)) {
				nt_printer_publish(session_info,
						   session_info,
						   msg_ctx,
						   pinfo2,
						   DSPRINT_UNPUBLISH);
				TALLOC_FREE(pinfo2);
			}
			nt_printer_remove(session_info, session_info, msg_ctx,
					  pname);
			lp_killservice(snum);
		} else {
			DEBUG_OUT (8, ("Adding default registry entry for printer "
				  "[%s], if it doesn't exist.\n", sname));
			nt_printer_add(session_info, session_info, msg_ctx,
				       sname);
		}
	}

	/* Make sure deleted printers are gone */
	load_printers(ev, msg_ctx);

	TALLOC_FREE(session_info);
}

/****************************************************************************
 Reload the services file.
**************************************************************************/

bool reload_services(struct messaging_context *msg_ctx, int smb_sock,
		     bool test)
{
	bool ret;

	if (lp_loaded()) {
		char *fname = lp_configfile();
		if (file_exist(fname) &&
		    !strcsequal(fname, get_dyn_CONFIGFILE())) {
			set_dyn_CONFIGFILE(fname);
			test = False;
		}
		TALLOC_FREE(fname);
	}

	reopen_logs();

	if (test && !lp_file_list_changed())
		return(True);

	lp_killunused(conn_snum_used);

	ret = lp_load(get_dyn_CONFIGFILE(), False, False, True, True);

	/* perhaps the config filename is now set */
	if (!test)
		reload_services(msg_ctx, smb_sock, True);

	reopen_logs();

	load_interfaces();

	if (smb_sock != -1) {
		set_socket_options(smb_sock,"SO_KEEPALIVE");
		set_socket_options(smb_sock, lp_socket_options());
	}

	mangle_reset_cache();
	reset_stat_cache();

	/* this forces service parameters to be flushed */
	set_current_service(NULL,0,True);

	return(ret);
}

/****************************************************************************
 Notify smbds of new printcap data
**************************************************************************/
void reload_pcap_change_notify(struct tevent_context *ev,
			       struct messaging_context *msg_ctx)
{
	/*
	 * Reload the printers first in the background process so that
	 * newly added printers get default values created in the registry.
	 *
	 * This will block the process for some time (~1 sec per printer), but
	 * it doesn't block smbd's servering clients.
	 */
	reload_printers(ev, msg_ctx);

	message_send_all(msg_ctx, MSG_PRINTER_PCAP, NULL, 0, NULL);
}
