/*
 * Unix SMB/CIFS implementation.
 * server auto-generated by pidl. DO NOT MODIFY!
 */

#include "includes.h"
#include "ntdomain.h"
#include "librpc/gen_ndr/srv_security.h"


/* Tables */
static struct api_struct api_security_cmds[] = 
{
};

void security_get_pipe_fns(struct api_struct **fns, int *n_fns)
{
	*fns = api_security_cmds;
	*n_fns = sizeof(api_security_cmds) / sizeof(struct api_struct);
}

NTSTATUS rpc_security_init(const struct rpc_srv_callbacks *rpc_srv_cb)
{
	return rpc_srv_register(SMB_RPC_INTERFACE_VERSION, "security", "security", &ndr_table_security, api_security_cmds, sizeof(api_security_cmds) / sizeof(struct api_struct), rpc_srv_cb);
}

NTSTATUS rpc_security_shutdown(void)
{
	return rpc_srv_unregister(&ndr_table_security);
}
