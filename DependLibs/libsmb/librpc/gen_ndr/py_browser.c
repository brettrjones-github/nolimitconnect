
/* Python wrapper functions auto-generated by pidl */
#include <libpython/Include/Python.h>
#include "includes.h"
#include <pytalloc.h>
#include "librpc/rpc/pyrpc.h"
#include "librpc/rpc/pyrpc_util.h"
#include "librpc/gen_ndr/ndr_browser.h"
#include "librpc/gen_ndr/ndr_browser_c.h"

#include "librpc/gen_ndr/srvsvc.h"
staticforward PyTypeObject BrowserrSrvInfo100Ctr_Type;
staticforward PyTypeObject BrowserrSrvInfo101Ctr_Type;
staticforward PyTypeObject BrowserrSrvInfo_Type;
staticforward PyTypeObject browser_InterfaceType;

void initbrowser(void);static PyTypeObject *srvsvc_NetSrvInfo100_Type;
static PyTypeObject *Object_Type;
static PyTypeObject *srvsvc_NetSrvInfo101_Type;
static PyTypeObject *ClientConnection_Type;

static PyObject *py_BrowserrSrvInfo100Ctr_get_entries_read(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo100Ctr *object = (struct BrowserrSrvInfo100Ctr *)py_talloc_get_ptr(obj);
	PyObject *py_entries_read;
	py_entries_read = PyInt_FromLong(object->entries_read);
	return py_entries_read;
}

static int py_BrowserrSrvInfo100Ctr_set_entries_read(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo100Ctr *object = (struct BrowserrSrvInfo100Ctr *)py_talloc_get_ptr(py_obj);
	PY_CHECK_TYPE(&PyInt_Type, value, return -1;);
	object->entries_read = PyInt_AsLong(value);
	return 0;
}

static PyObject *py_BrowserrSrvInfo100Ctr_get_entries(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo100Ctr *object = (struct BrowserrSrvInfo100Ctr *)py_talloc_get_ptr(obj);
	PyObject *py_entries;
	if (object->entries == NULL) {
		py_entries = Py_None;
		Py_INCREF(py_entries);
	} else {
		py_entries = PyList_New(object->entries_read);
		if (py_entries == NULL) {
			return NULL;
		}
		{
			int entries_cntr_1;
			for (entries_cntr_1 = 0; entries_cntr_1 < object->entries_read; entries_cntr_1++) {
				PyObject *py_entries_1;
				py_entries_1 = py_talloc_reference_ex(srvsvc_NetSrvInfo100_Type, object->entries, &object->entries[entries_cntr_1]);
				PyList_SetItem(py_entries, entries_cntr_1, py_entries_1);
			}
		}
	}
	return py_entries;
}

static int py_BrowserrSrvInfo100Ctr_set_entries(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo100Ctr *object = (struct BrowserrSrvInfo100Ctr *)py_talloc_get_ptr(py_obj);
	talloc_unlink(py_talloc_get_mem_ctx(py_obj), object->entries);
	if (value == Py_None) {
		object->entries = NULL;
	} else {
		object->entries = NULL;
		PY_CHECK_TYPE(&PyList_Type, value, return -1;);
		{
			int entries_cntr_1;
			object->entries = talloc_array_ptrtype(py_talloc_get_mem_ctx(py_obj), object->entries, PyList_GET_SIZE(value));
			if (!object->entries) { return -1;; }
			talloc_set_name_const(object->entries, "ARRAY: object->entries");
			for (entries_cntr_1 = 0; entries_cntr_1 < PyList_GET_SIZE(value); entries_cntr_1++) {
				PY_CHECK_TYPE(srvsvc_NetSrvInfo100_Type, PyList_GET_ITEM(value, entries_cntr_1), return -1;);
				if (talloc_reference(object->entries, py_talloc_get_mem_ctx(PyList_GET_ITEM(value, entries_cntr_1))) == NULL) {
					PyErr_NoMemory();
					return -1;
				}
				object->entries[entries_cntr_1] = *(struct srvsvc_NetSrvInfo100 *)py_talloc_get_ptr(PyList_GET_ITEM(value, entries_cntr_1));
			}
		}
	}
	return 0;
}

static PyGetSetDef py_BrowserrSrvInfo100Ctr_getsetters[] = {
	{ discard_const_p(char, "entries_read"), py_BrowserrSrvInfo100Ctr_get_entries_read, py_BrowserrSrvInfo100Ctr_set_entries_read },
	{ discard_const_p(char, "entries"), py_BrowserrSrvInfo100Ctr_get_entries, py_BrowserrSrvInfo100Ctr_set_entries },
	{ NULL }
};

static PyObject *py_BrowserrSrvInfo100Ctr_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_talloc_new(struct BrowserrSrvInfo100Ctr, type);
}


static PyTypeObject BrowserrSrvInfo100Ctr_Type = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "browser.BrowserrSrvInfo100Ctr",
	.tp_getset = py_BrowserrSrvInfo100Ctr_getsetters,
	.tp_methods = NULL,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_basicsize = sizeof(py_talloc_Object),
	.tp_new = py_BrowserrSrvInfo100Ctr_new,
};


static PyObject *py_BrowserrSrvInfo101Ctr_get_entries_read(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo101Ctr *object = (struct BrowserrSrvInfo101Ctr *)py_talloc_get_ptr(obj);
	PyObject *py_entries_read;
	py_entries_read = PyInt_FromLong(object->entries_read);
	return py_entries_read;
}

static int py_BrowserrSrvInfo101Ctr_set_entries_read(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo101Ctr *object = (struct BrowserrSrvInfo101Ctr *)py_talloc_get_ptr(py_obj);
	PY_CHECK_TYPE(&PyInt_Type, value, return -1;);
	object->entries_read = PyInt_AsLong(value);
	return 0;
}

static PyObject *py_BrowserrSrvInfo101Ctr_get_entries(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo101Ctr *object = (struct BrowserrSrvInfo101Ctr *)py_talloc_get_ptr(obj);
	PyObject *py_entries;
	if (object->entries == NULL) {
		py_entries = Py_None;
		Py_INCREF(py_entries);
	} else {
		py_entries = PyList_New(object->entries_read);
		if (py_entries == NULL) {
			return NULL;
		}
		{
			int entries_cntr_1;
			for (entries_cntr_1 = 0; entries_cntr_1 < object->entries_read; entries_cntr_1++) {
				PyObject *py_entries_1;
				py_entries_1 = py_talloc_reference_ex(srvsvc_NetSrvInfo101_Type, object->entries, &object->entries[entries_cntr_1]);
				PyList_SetItem(py_entries, entries_cntr_1, py_entries_1);
			}
		}
	}
	return py_entries;
}

static int py_BrowserrSrvInfo101Ctr_set_entries(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo101Ctr *object = (struct BrowserrSrvInfo101Ctr *)py_talloc_get_ptr(py_obj);
	talloc_unlink(py_talloc_get_mem_ctx(py_obj), object->entries);
	if (value == Py_None) {
		object->entries = NULL;
	} else {
		object->entries = NULL;
		PY_CHECK_TYPE(&PyList_Type, value, return -1;);
		{
			int entries_cntr_1;
			object->entries = talloc_array_ptrtype(py_talloc_get_mem_ctx(py_obj), object->entries, PyList_GET_SIZE(value));
			if (!object->entries) { return -1;; }
			talloc_set_name_const(object->entries, "ARRAY: object->entries");
			for (entries_cntr_1 = 0; entries_cntr_1 < PyList_GET_SIZE(value); entries_cntr_1++) {
				PY_CHECK_TYPE(srvsvc_NetSrvInfo101_Type, PyList_GET_ITEM(value, entries_cntr_1), return -1;);
				if (talloc_reference(object->entries, py_talloc_get_mem_ctx(PyList_GET_ITEM(value, entries_cntr_1))) == NULL) {
					PyErr_NoMemory();
					return -1;
				}
				object->entries[entries_cntr_1] = *(struct srvsvc_NetSrvInfo101 *)py_talloc_get_ptr(PyList_GET_ITEM(value, entries_cntr_1));
			}
		}
	}
	return 0;
}

static PyGetSetDef py_BrowserrSrvInfo101Ctr_getsetters[] = {
	{ discard_const_p(char, "entries_read"), py_BrowserrSrvInfo101Ctr_get_entries_read, py_BrowserrSrvInfo101Ctr_set_entries_read },
	{ discard_const_p(char, "entries"), py_BrowserrSrvInfo101Ctr_get_entries, py_BrowserrSrvInfo101Ctr_set_entries },
	{ NULL }
};

static PyObject *py_BrowserrSrvInfo101Ctr_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_talloc_new(struct BrowserrSrvInfo101Ctr, type);
}


static PyTypeObject BrowserrSrvInfo101Ctr_Type = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "browser.BrowserrSrvInfo101Ctr",
	.tp_getset = py_BrowserrSrvInfo101Ctr_getsetters,
	.tp_methods = NULL,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_basicsize = sizeof(py_talloc_Object),
	.tp_new = py_BrowserrSrvInfo101Ctr_new,
};

PyObject *py_import_BrowserrSrvInfoUnion(TALLOC_CTX *mem_ctx, int level, union BrowserrSrvInfoUnion *in)
{
	PyObject *ret;

	switch (level) {
		case 100:
			if (in->info100 == NULL) {
				ret = Py_None;
				Py_INCREF(ret);
			} else {
				ret = py_talloc_reference_ex(&BrowserrSrvInfo100Ctr_Type, in->info100, in->info100);
			}
			return ret;

		case 101:
			if (in->info101 == NULL) {
				ret = Py_None;
				Py_INCREF(ret);
			} else {
				ret = py_talloc_reference_ex(&BrowserrSrvInfo101Ctr_Type, in->info101, in->info101);
			}
			return ret;

		default:
			ret = Py_None;
			Py_INCREF(ret);
			return ret;

	}
	PyErr_SetString(PyExc_TypeError, "unknown union level");
	return NULL;
}

union BrowserrSrvInfoUnion *py_export_BrowserrSrvInfoUnion(TALLOC_CTX *mem_ctx, int level, PyObject *in)
{
	union BrowserrSrvInfoUnion *ret = talloc_zero(mem_ctx, union BrowserrSrvInfoUnion);
	switch (level) {
		case 100:
			if (in == Py_None) {
				ret->info100 = NULL;
			} else {
				ret->info100 = NULL;
				PY_CHECK_TYPE(&BrowserrSrvInfo100Ctr_Type, in, talloc_free(ret); return NULL;);
				if (talloc_reference(mem_ctx, py_talloc_get_mem_ctx(in)) == NULL) {
					PyErr_NoMemory();
					talloc_free(ret); return NULL;
				}
				ret->info100 = (struct BrowserrSrvInfo100Ctr *)py_talloc_get_ptr(in);
			}
			break;

		case 101:
			if (in == Py_None) {
				ret->info101 = NULL;
			} else {
				ret->info101 = NULL;
				PY_CHECK_TYPE(&BrowserrSrvInfo101Ctr_Type, in, talloc_free(ret); return NULL;);
				if (talloc_reference(mem_ctx, py_talloc_get_mem_ctx(in)) == NULL) {
					PyErr_NoMemory();
					talloc_free(ret); return NULL;
				}
				ret->info101 = (struct BrowserrSrvInfo101Ctr *)py_talloc_get_ptr(in);
			}
			break;

		default:
			break;

	}

	return ret;
}


static PyObject *py_BrowserrSrvInfo_get_level(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo *object = (struct BrowserrSrvInfo *)py_talloc_get_ptr(obj);
	PyObject *py_level;
	py_level = PyInt_FromLong(object->level);
	return py_level;
}

static int py_BrowserrSrvInfo_set_level(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo *object = (struct BrowserrSrvInfo *)py_talloc_get_ptr(py_obj);
	PY_CHECK_TYPE(&PyInt_Type, value, return -1;);
	object->level = PyInt_AsLong(value);
	return 0;
}

static PyObject *py_BrowserrSrvInfo_get_info(PyObject *obj, void *closure)
{
	struct BrowserrSrvInfo *object = (struct BrowserrSrvInfo *)py_talloc_get_ptr(obj);
	PyObject *py_info;
	py_info = py_import_BrowserrSrvInfoUnion(py_talloc_get_mem_ctx(obj), object->level, &object->info);
	if (py_info == NULL) {
		return NULL;
	}
	return py_info;
}

static int py_BrowserrSrvInfo_set_info(PyObject *py_obj, PyObject *value, void *closure)
{
	struct BrowserrSrvInfo *object = (struct BrowserrSrvInfo *)py_talloc_get_ptr(py_obj);
	{
		union BrowserrSrvInfoUnion *info_switch_0;
		info_switch_0 = py_export_BrowserrSrvInfoUnion(py_talloc_get_mem_ctx(py_obj), object->level, value);
		if (info_switch_0 == NULL) {
			return -1;
		}
		object->info = *info_switch_0;
	}
	return 0;
}

static PyGetSetDef py_BrowserrSrvInfo_getsetters[] = {
	{ discard_const_p(char, "level"), py_BrowserrSrvInfo_get_level, py_BrowserrSrvInfo_set_level },
	{ discard_const_p(char, "info"), py_BrowserrSrvInfo_get_info, py_BrowserrSrvInfo_set_info },
	{ NULL }
};

static PyObject *py_BrowserrSrvInfo_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_talloc_new(struct BrowserrSrvInfo, type);
}


static PyTypeObject BrowserrSrvInfo_Type = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "browser.BrowserrSrvInfo",
	.tp_getset = py_BrowserrSrvInfo_getsetters,
	.tp_methods = NULL,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_basicsize = sizeof(py_talloc_Object),
	.tp_new = py_BrowserrSrvInfo_new,
};


static bool pack_py_BrowserrQueryOtherDomains_args_in(PyObject *args, PyObject *kwargs, struct BrowserrQueryOtherDomains *r)
{
	PyObject *py_server_unc;
	PyObject *py_info;
	const char *kwnames[] = {
		"server_unc", "info", NULL
	};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO:BrowserrQueryOtherDomains", discard_const_p(char *, kwnames), &py_server_unc, &py_info)) {
		return false;
	}

	if (py_server_unc == Py_None) {
		r->in.server_unc = NULL;
	} else {
		r->in.server_unc = NULL;
		if (PyUnicode_Check(py_server_unc)) {
			r->in.server_unc = PyString_AS_STRING(PyUnicode_AsEncodedString(py_server_unc, "utf-8", "ignore"));
		} else if (PyString_Check(py_server_unc)) {
			r->in.server_unc = PyString_AS_STRING(py_server_unc);
		} else {
			PyErr_Format(PyExc_TypeError, "Expected string or unicode object, got %s", Py_TYPE(py_server_unc)->tp_name);
			return false;
		}
	}
	r->in.info = talloc_ptrtype(r, r->in.info);
	PY_CHECK_TYPE(&BrowserrSrvInfo_Type, py_info, return false;);
	if (talloc_reference(r, py_talloc_get_mem_ctx(py_info)) == NULL) {
		PyErr_NoMemory();
		return false;
	}
	r->in.info = (struct BrowserrSrvInfo *)py_talloc_get_ptr(py_info);
	return true;
}

static PyObject *unpack_py_BrowserrQueryOtherDomains_args_out(struct BrowserrQueryOtherDomains *r)
{
	PyObject *result;
	PyObject *py_info;
	PyObject *py_total_entries;
	result = PyTuple_New(2);
	py_info = py_talloc_reference_ex(&BrowserrSrvInfo_Type, r->out.info, r->out.info);
	PyTuple_SetItem(result, 0, py_info);
	py_total_entries = PyInt_FromLong(*r->out.total_entries);
	PyTuple_SetItem(result, 1, py_total_entries);
	if (!W_ERROR_IS_OK(r->out.result)) {
		PyErr_SetWERROR(r->out.result);
		return NULL;
	}

	return result;
}

const struct PyNdrRpcMethodDef py_ndr_browser_methods[] = {
	{ "BrowserrQueryOtherDomains", "S.BrowserrQueryOtherDomains(server_unc, info) -> (info, total_entries)", (py_dcerpc_call_fn)dcerpc_BrowserrQueryOtherDomains_r, (py_data_pack_fn)pack_py_BrowserrQueryOtherDomains_args_in, (py_data_unpack_fn)unpack_py_BrowserrQueryOtherDomains_args_out, 2, &ndr_table_browser },
	{ NULL }
};

static PyObject *interface_browser_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_dcerpc_interface_init_helper(type, args, kwargs, &ndr_table_browser);
}

#define PY_DOC_BROWSER "Browsing"
static PyTypeObject browser_InterfaceType = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "browser.browser",
	.tp_basicsize = sizeof(dcerpc_InterfaceObject),
	.tp_doc = "browser(binding, lp_ctx=None, credentials=None) -> connection\n"
"\n"
"binding should be a DCE/RPC binding string (for example: ncacn_ip_tcp:127.0.0.1)\n"
"lp_ctx should be a path to a smb.conf file or a param.LoadParm object\n"
"credentials should be a credentials.Credentials object.\n\n"PY_DOC_BROWSER,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new = interface_browser_new,
};

static PyMethodDef browser_methods[] = {
	{ NULL, NULL, 0, NULL }
};

void initbrowser(void)
{
	PyObject *m;
	PyObject *dep_talloc;
	PyObject *dep_samba_dcerpc_base;
	PyObject *dep_samba_dcerpc_srvsvc;

	dep_talloc = PyImport_ImportModule("talloc");
	if (dep_talloc == NULL)
		return;

	dep_samba_dcerpc_base = PyImport_ImportModule("samba.dcerpc.base");
	if (dep_samba_dcerpc_base == NULL)
		return;

	dep_samba_dcerpc_srvsvc = PyImport_ImportModule("samba.dcerpc.srvsvc");
	if (dep_samba_dcerpc_srvsvc == NULL)
		return;

	srvsvc_NetSrvInfo100_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_srvsvc, "NetSrvInfo100");
	if (srvsvc_NetSrvInfo100_Type == NULL)
		return;

	Object_Type = (PyTypeObject *)PyObject_GetAttrString(dep_talloc, "Object");
	if (Object_Type == NULL)
		return;

	srvsvc_NetSrvInfo101_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_srvsvc, "NetSrvInfo101");
	if (srvsvc_NetSrvInfo101_Type == NULL)
		return;

	ClientConnection_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_base, "ClientConnection");
	if (ClientConnection_Type == NULL)
		return;

	BrowserrSrvInfo100Ctr_Type.tp_base = Object_Type;

	BrowserrSrvInfo101Ctr_Type.tp_base = Object_Type;

	BrowserrSrvInfo_Type.tp_base = Object_Type;

	browser_InterfaceType.tp_base = ClientConnection_Type;

	if (PyType_Ready(&BrowserrSrvInfo100Ctr_Type) < 0)
		return;
	if (PyType_Ready(&BrowserrSrvInfo101Ctr_Type) < 0)
		return;
	if (PyType_Ready(&BrowserrSrvInfo_Type) < 0)
		return;
	if (PyType_Ready(&browser_InterfaceType) < 0)
		return;
	if (!PyInterface_AddNdrRpcMethods(&browser_InterfaceType, py_ndr_browser_methods))
		return;

#ifdef PY_BROWSERRSRVINFO100CTR_PATCH
	PY_BROWSERRSRVINFO100CTR_PATCH(&BrowserrSrvInfo100Ctr_Type);
#endif
#ifdef PY_BROWSERRSRVINFO101CTR_PATCH
	PY_BROWSERRSRVINFO101CTR_PATCH(&BrowserrSrvInfo101Ctr_Type);
#endif
#ifdef PY_BROWSERRSRVINFO_PATCH
	PY_BROWSERRSRVINFO_PATCH(&BrowserrSrvInfo_Type);
#endif
#ifdef PY_BROWSER_PATCH
	PY_BROWSER_PATCH(&browser_InterfaceType);
#endif

	m = Py_InitModule3("browser", browser_methods, "browser DCE/RPC");
	if (m == NULL)
		return;

	Py_INCREF((PyObject *)(void *)&BrowserrSrvInfo100Ctr_Type);
	PyModule_AddObject(m, "BrowserrSrvInfo100Ctr", (PyObject *)(void *)&BrowserrSrvInfo100Ctr_Type);
	Py_INCREF((PyObject *)(void *)&BrowserrSrvInfo101Ctr_Type);
	PyModule_AddObject(m, "BrowserrSrvInfo101Ctr", (PyObject *)(void *)&BrowserrSrvInfo101Ctr_Type);
	Py_INCREF((PyObject *)(void *)&BrowserrSrvInfo_Type);
	PyModule_AddObject(m, "BrowserrSrvInfo", (PyObject *)(void *)&BrowserrSrvInfo_Type);
	Py_INCREF((PyObject *)(void *)&browser_InterfaceType);
	PyModule_AddObject(m, "browser", (PyObject *)(void *)&browser_InterfaceType);
#ifdef PY_MOD_BROWSER_PATCH
	PY_MOD_BROWSER_PATCH(m);
#endif

}
