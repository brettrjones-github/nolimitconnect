
/* Python wrapper functions auto-generated by pidl */
#include <libpython/Include/Python.h>
#include "includes.h"
#include <pytalloc.h>
#include "librpc/rpc/pyrpc.h"
#include "librpc/rpc/pyrpc_util.h"
#include "librpc/gen_ndr/ndr_w32time.h"
#include "librpc/gen_ndr/ndr_w32time_c.h"

staticforward PyTypeObject w32time_InterfaceType;

void initw32time(void);static PyTypeObject *ClientConnection_Type;

const struct PyNdrRpcMethodDef py_ndr_w32time_methods[] = {
	{ NULL }
};

static PyObject *interface_w32time_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_dcerpc_interface_init_helper(type, args, kwargs, &ndr_table_w32time);
}

#define PY_DOC_W32TIME "Win32 Time Server"
static PyTypeObject w32time_InterfaceType = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "w32time.w32time",
	.tp_basicsize = sizeof(dcerpc_InterfaceObject),
	.tp_doc = "w32time(binding, lp_ctx=None, credentials=None) -> connection\n"
"\n"
"binding should be a DCE/RPC binding string (for example: ncacn_ip_tcp:127.0.0.1)\n"
"lp_ctx should be a path to a smb.conf file or a param.LoadParm object\n"
"credentials should be a credentials.Credentials object.\n\n"PY_DOC_W32TIME,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new = interface_w32time_new,
};

static PyMethodDef w32time_methods[] = {
	{ NULL, NULL, 0, NULL }
};

void initw32time(void)
{
	PyObject *m;
	PyObject *dep_samba_dcerpc_base;

	dep_samba_dcerpc_base = PyImport_ImportModule("samba.dcerpc.base");
	if (dep_samba_dcerpc_base == NULL)
		return;

	ClientConnection_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_base, "ClientConnection");
	if (ClientConnection_Type == NULL)
		return;

	w32time_InterfaceType.tp_base = ClientConnection_Type;

	if (PyType_Ready(&w32time_InterfaceType) < 0)
		return;
	if (!PyInterface_AddNdrRpcMethods(&w32time_InterfaceType, py_ndr_w32time_methods))
		return;

#ifdef PY_W32TIME_PATCH
	PY_W32TIME_PATCH(&w32time_InterfaceType);
#endif

	m = Py_InitModule3("w32time", w32time_methods, "w32time DCE/RPC");
	if (m == NULL)
		return;

	Py_INCREF((PyObject *)(void *)&w32time_InterfaceType);
	PyModule_AddObject(m, "w32time", (PyObject *)(void *)&w32time_InterfaceType);
#ifdef PY_MOD_W32TIME_PATCH
	PY_MOD_W32TIME_PATCH(m);
#endif

}
