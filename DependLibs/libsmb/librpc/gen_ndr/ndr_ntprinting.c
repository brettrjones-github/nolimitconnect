/* parser auto-generated by pidl */

#include "includes.h"
#include "librpc/gen_ndr/ndr_ntprinting.h"

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_form(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_form *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 4));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->position));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->flag));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->width));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->length));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->left));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->top));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->right));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->bottom));
			NDR_CHECK(ndr_push_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_ntprinting_form(struct ndr_pull *ndr, int ndr_flags, struct ntprinting_form *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_pull_align(ndr, 4));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->position));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->flag));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->width));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->length));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->left));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->top));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->right));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->bottom));
			NDR_CHECK(ndr_pull_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_form(struct ndr_print *ndr, const char *name, const struct ntprinting_form *r)
{
	ndr_print_struct(ndr, name, "ntprinting_form");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_uint32(ndr, "position", r->position);
		ndr_print_uint32(ndr, "flag", r->flag);
		ndr_print_uint32(ndr, "width", r->width);
		ndr_print_uint32(ndr, "length", r->length);
		ndr_print_uint32(ndr, "left", r->left);
		ndr_print_uint32(ndr, "top", r->top);
		ndr_print_uint32(ndr, "right", r->right);
		ndr_print_uint32(ndr, "bottom", r->bottom);
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_driver(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_driver *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 4));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->version));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->name));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->environment));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->driverpath));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->datafile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->configfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->helpfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->monitorname));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->defaultdatatype));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string_array = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NOTERM|LIBNDR_FLAG_REMAINING);
				NDR_CHECK(ndr_push_string_array(ndr, NDR_SCALARS, r->dependent_files));
				ndr->flags = _flags_save_string_array;
			}
			NDR_CHECK(ndr_push_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_ntprinting_driver(struct ndr_pull *ndr, int ndr_flags, struct ntprinting_driver *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_pull_align(ndr, 4));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->version));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->name));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->environment));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->driverpath));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->datafile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->configfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->helpfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->monitorname));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->defaultdatatype));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string_array = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NOTERM|LIBNDR_FLAG_REMAINING);
				NDR_CHECK(ndr_pull_string_array(ndr, NDR_SCALARS, &r->dependent_files));
				ndr->flags = _flags_save_string_array;
			}
			NDR_CHECK(ndr_pull_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_driver(struct ndr_print *ndr, const char *name, const struct ntprinting_driver *r)
{
	ndr_print_struct(ndr, name, "ntprinting_driver");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_uint32(ndr, "version", r->version);
		ndr_print_string(ndr, "name", r->name);
		ndr_print_string(ndr, "environment", r->environment);
		ndr_print_string(ndr, "driverpath", r->driverpath);
		ndr_print_string(ndr, "datafile", r->datafile);
		ndr_print_string(ndr, "configfile", r->configfile);
		ndr_print_string(ndr, "helpfile", r->helpfile);
		ndr_print_string(ndr, "monitorname", r->monitorname);
		ndr_print_string(ndr, "defaultdatatype", r->defaultdatatype);
		ndr_print_string_array(ndr, "dependent_files", r->dependent_files);
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_devicemode(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_devicemode *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 5));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->devicename));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->formname));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->specversion));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->driverversion));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->size));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->driverextra));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->orientation));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->papersize));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->paperlength));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->paperwidth));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->scale));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->copies));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->defaultsource));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->printquality));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->color));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->duplex));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->yresolution));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->ttoption));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->collate));
			NDR_CHECK(ndr_push_uint16(ndr, NDR_SCALARS, r->logpixels));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->fields));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->bitsperpel));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->pelswidth));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->pelsheight));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->displayflags));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->displayfrequency));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->icmmethod));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->icmintent));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->mediatype));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->dithertype));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->reserved1));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->reserved2));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->panningwidth));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->panningheight));
			NDR_CHECK(ndr_push_unique_ptr(ndr, r->nt_dev_private));
			NDR_CHECK(ndr_push_trailer_align(ndr, 5));
		}
		if (ndr_flags & NDR_BUFFERS) {
			if (r->nt_dev_private) {
				NDR_CHECK(ndr_push_DATA_BLOB(ndr, NDR_SCALARS, *r->nt_dev_private));
			}
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_ntprinting_devicemode(struct ndr_pull *ndr, int ndr_flags, struct ntprinting_devicemode *r)
{
	uint32_t _ptr_nt_dev_private;
	TALLOC_CTX *_mem_save_nt_dev_private_0;
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_pull_align(ndr, 5));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->devicename));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->formname));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->specversion));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->driverversion));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->size));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->driverextra));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->orientation));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->papersize));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->paperlength));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->paperwidth));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->scale));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->copies));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->defaultsource));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->printquality));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->color));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->duplex));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->yresolution));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->ttoption));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->collate));
			NDR_CHECK(ndr_pull_uint16(ndr, NDR_SCALARS, &r->logpixels));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->fields));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->bitsperpel));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->pelswidth));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->pelsheight));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->displayflags));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->displayfrequency));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->icmmethod));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->icmintent));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->mediatype));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->dithertype));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->reserved1));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->reserved2));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->panningwidth));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->panningheight));
			NDR_CHECK(ndr_pull_generic_ptr(ndr, &_ptr_nt_dev_private));
			if (_ptr_nt_dev_private) {
				NDR_PULL_ALLOC(ndr, r->nt_dev_private);
			} else {
				r->nt_dev_private = NULL;
			}
			NDR_CHECK(ndr_pull_trailer_align(ndr, 5));
		}
		if (ndr_flags & NDR_BUFFERS) {
			if (r->nt_dev_private) {
				_mem_save_nt_dev_private_0 = NDR_PULL_GET_MEM_CTX(ndr);
				NDR_PULL_SET_MEM_CTX(ndr, r->nt_dev_private, 0);
				NDR_CHECK(ndr_pull_DATA_BLOB(ndr, NDR_SCALARS, r->nt_dev_private));
				NDR_PULL_SET_MEM_CTX(ndr, _mem_save_nt_dev_private_0, 0);
			}
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_devicemode(struct ndr_print *ndr, const char *name, const struct ntprinting_devicemode *r)
{
	ndr_print_struct(ndr, name, "ntprinting_devicemode");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_string(ndr, "devicename", r->devicename);
		ndr_print_string(ndr, "formname", r->formname);
		ndr_print_uint16(ndr, "specversion", r->specversion);
		ndr_print_uint16(ndr, "driverversion", r->driverversion);
		ndr_print_uint16(ndr, "size", r->size);
		ndr_print_uint16(ndr, "driverextra", r->driverextra);
		ndr_print_uint16(ndr, "orientation", r->orientation);
		ndr_print_uint16(ndr, "papersize", r->papersize);
		ndr_print_uint16(ndr, "paperlength", r->paperlength);
		ndr_print_uint16(ndr, "paperwidth", r->paperwidth);
		ndr_print_uint16(ndr, "scale", r->scale);
		ndr_print_uint16(ndr, "copies", r->copies);
		ndr_print_uint16(ndr, "defaultsource", r->defaultsource);
		ndr_print_uint16(ndr, "printquality", r->printquality);
		ndr_print_uint16(ndr, "color", r->color);
		ndr_print_uint16(ndr, "duplex", r->duplex);
		ndr_print_uint16(ndr, "yresolution", r->yresolution);
		ndr_print_uint16(ndr, "ttoption", r->ttoption);
		ndr_print_uint16(ndr, "collate", r->collate);
		ndr_print_uint16(ndr, "logpixels", r->logpixels);
		ndr_print_uint32(ndr, "fields", r->fields);
		ndr_print_uint32(ndr, "bitsperpel", r->bitsperpel);
		ndr_print_uint32(ndr, "pelswidth", r->pelswidth);
		ndr_print_uint32(ndr, "pelsheight", r->pelsheight);
		ndr_print_uint32(ndr, "displayflags", r->displayflags);
		ndr_print_uint32(ndr, "displayfrequency", r->displayfrequency);
		ndr_print_uint32(ndr, "icmmethod", r->icmmethod);
		ndr_print_uint32(ndr, "icmintent", r->icmintent);
		ndr_print_uint32(ndr, "mediatype", r->mediatype);
		ndr_print_uint32(ndr, "dithertype", r->dithertype);
		ndr_print_uint32(ndr, "reserved1", r->reserved1);
		ndr_print_uint32(ndr, "reserved2", r->reserved2);
		ndr_print_uint32(ndr, "panningwidth", r->panningwidth);
		ndr_print_uint32(ndr, "panningheight", r->panningheight);
		ndr_print_ptr(ndr, "nt_dev_private", r->nt_dev_private);
		ndr->depth++;
		if (r->nt_dev_private) {
			ndr_print_DATA_BLOB(ndr, "nt_dev_private", *r->nt_dev_private);
		}
		ndr->depth--;
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_printer_data(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_printer_data *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 4));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->ptr));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->name));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->type));
			NDR_CHECK(ndr_push_DATA_BLOB(ndr, NDR_SCALARS, r->data));
			NDR_CHECK(ndr_push_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_ntprinting_printer_data(struct ndr_pull *ndr, int ndr_flags, struct ntprinting_printer_data *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_pull_align(ndr, 4));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->ptr));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->name));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->type));
			NDR_CHECK(ndr_pull_DATA_BLOB(ndr, NDR_SCALARS, &r->data));
			NDR_CHECK(ndr_pull_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_printer_data(struct ndr_print *ndr, const char *name, const struct ntprinting_printer_data *r)
{
	ndr_print_struct(ndr, name, "ntprinting_printer_data");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_uint32(ndr, "ptr", r->ptr);
		ndr_print_string(ndr, "name", r->name);
		ndr_print_uint32(ndr, "type", r->type);
		ndr_print_DATA_BLOB(ndr, "data", r->data);
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_printer_info(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_printer_info *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 4));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->attributes));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->priority));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->default_priority));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->starttime));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->untiltime));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->status));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->cjobs));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->averageppm));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->changeid));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->c_setprinter));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->setuptime));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->servername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->printername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->sharename));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->portname));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->drivername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->comment));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->location));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->sepfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->printprocessor));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->datatype));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_push_string(ndr, NDR_SCALARS, r->parameters));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_push_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_ntprinting_printer_info(struct ndr_pull *ndr, int ndr_flags, struct ntprinting_printer_info *r)
{
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_pull_align(ndr, 4));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->attributes));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->priority));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->default_priority));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->starttime));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->untiltime));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->status));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->cjobs));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->averageppm));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->changeid));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->c_setprinter));
			NDR_CHECK(ndr_pull_uint32(ndr, NDR_SCALARS, &r->setuptime));
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->servername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->printername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->sharename));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->portname));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->drivername));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->comment));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->location));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->sepfile));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->printprocessor));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->datatype));
				ndr->flags = _flags_save_string;
			}
			{
				uint32_t _flags_save_string = ndr->flags;
				ndr_set_flags(&ndr->flags, LIBNDR_FLAG_STR_UTF8|LIBNDR_FLAG_STR_NULLTERM);
				NDR_CHECK(ndr_pull_string(ndr, NDR_SCALARS, &r->parameters));
				ndr->flags = _flags_save_string;
			}
			NDR_CHECK(ndr_pull_trailer_align(ndr, 4));
		}
		if (ndr_flags & NDR_BUFFERS) {
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_printer_info(struct ndr_print *ndr, const char *name, const struct ntprinting_printer_info *r)
{
	ndr_print_struct(ndr, name, "ntprinting_printer_info");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_uint32(ndr, "attributes", r->attributes);
		ndr_print_uint32(ndr, "priority", r->priority);
		ndr_print_uint32(ndr, "default_priority", r->default_priority);
		ndr_print_uint32(ndr, "starttime", r->starttime);
		ndr_print_uint32(ndr, "untiltime", r->untiltime);
		ndr_print_uint32(ndr, "status", r->status);
		ndr_print_uint32(ndr, "cjobs", r->cjobs);
		ndr_print_uint32(ndr, "averageppm", r->averageppm);
		ndr_print_uint32(ndr, "changeid", r->changeid);
		ndr_print_uint32(ndr, "c_setprinter", r->c_setprinter);
		ndr_print_uint32(ndr, "setuptime", r->setuptime);
		ndr_print_string(ndr, "servername", r->servername);
		ndr_print_string(ndr, "printername", r->printername);
		ndr_print_string(ndr, "sharename", r->sharename);
		ndr_print_string(ndr, "portname", r->portname);
		ndr_print_string(ndr, "drivername", r->drivername);
		ndr_print_string(ndr, "comment", r->comment);
		ndr_print_string(ndr, "location", r->location);
		ndr_print_string(ndr, "sepfile", r->sepfile);
		ndr_print_string(ndr, "printprocessor", r->printprocessor);
		ndr_print_string(ndr, "datatype", r->datatype);
		ndr_print_string(ndr, "parameters", r->parameters);
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_ntprinting_printer(struct ndr_push *ndr, int ndr_flags, const struct ntprinting_printer *r)
{
	uint32_t cntr_printer_data_0;
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		if (ndr_flags & NDR_SCALARS) {
			NDR_CHECK(ndr_push_align(ndr, 5));
			NDR_CHECK(ndr_push_ntprinting_printer_info(ndr, NDR_SCALARS, &r->info));
			NDR_CHECK(ndr_push_unique_ptr(ndr, r->devmode));
			NDR_CHECK(ndr_push_uint32(ndr, NDR_SCALARS, r->count));
			for (cntr_printer_data_0 = 0; cntr_printer_data_0 < r->count; cntr_printer_data_0++) {
				NDR_CHECK(ndr_push_ntprinting_printer_data(ndr, NDR_SCALARS, &r->printer_data[cntr_printer_data_0]));
			}
			NDR_CHECK(ndr_push_trailer_align(ndr, 5));
		}
		if (ndr_flags & NDR_BUFFERS) {
			if (r->devmode) {
				NDR_CHECK(ndr_push_ntprinting_devicemode(ndr, NDR_SCALARS|NDR_BUFFERS, r->devmode));
			}
		}
		ndr->flags = _flags_save_STRUCT;
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_ntprinting_printer(struct ndr_print *ndr, const char *name, const struct ntprinting_printer *r)
{
	uint32_t cntr_printer_data_0;
	ndr_print_struct(ndr, name, "ntprinting_printer");
	if (r == NULL) { ndr_print_null(ndr); return; }
	{
		uint32_t _flags_save_STRUCT = ndr->flags;
		ndr_set_flags(&ndr->flags, LIBNDR_FLAG_NOALIGN);
		ndr->depth++;
		ndr_print_ntprinting_printer_info(ndr, "info", &r->info);
		ndr_print_ptr(ndr, "devmode", r->devmode);
		ndr->depth++;
		if (r->devmode) {
			ndr_print_ntprinting_devicemode(ndr, "devmode", r->devmode);
		}
		ndr->depth--;
		ndr_print_uint32(ndr, "count", r->count);
		ndr->print(ndr, "%s: ARRAY(%d)", "printer_data", (int)r->count);
		ndr->depth++;
		for (cntr_printer_data_0=0;cntr_printer_data_0<r->count;cntr_printer_data_0++) {
			ndr_print_ntprinting_printer_data(ndr, "printer_data", &r->printer_data[cntr_printer_data_0]);
		}
		ndr->depth--;
		ndr->depth--;
		ndr->flags = _flags_save_STRUCT;
	}
}

_PUBLIC_ enum ndr_err_code ndr_push_decode_ntprinting_form(struct ndr_push *ndr, int flags, const struct decode_ntprinting_form *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_push_ntprinting_form(ndr, NDR_SCALARS, &r->in.form));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_decode_ntprinting_form(struct ndr_pull *ndr, int flags, struct decode_ntprinting_form *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_pull_ntprinting_form(ndr, NDR_SCALARS, &r->in.form));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_decode_ntprinting_form(struct ndr_print *ndr, const char *name, int flags, const struct decode_ntprinting_form *r)
{
	ndr_print_struct(ndr, name, "decode_ntprinting_form");
	if (r == NULL) { ndr_print_null(ndr); return; }
	ndr->depth++;
	if (flags & NDR_SET_VALUES) {
		ndr->flags |= LIBNDR_PRINT_SET_VALUES;
	}
	if (flags & NDR_IN) {
		ndr_print_struct(ndr, "in", "decode_ntprinting_form");
		ndr->depth++;
		ndr_print_ntprinting_form(ndr, "form", &r->in.form);
		ndr->depth--;
	}
	if (flags & NDR_OUT) {
		ndr_print_struct(ndr, "out", "decode_ntprinting_form");
		ndr->depth++;
		ndr->depth--;
	}
	ndr->depth--;
}

_PUBLIC_ enum ndr_err_code ndr_push_decode_ntprinting_driver(struct ndr_push *ndr, int flags, const struct decode_ntprinting_driver *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_push_ntprinting_driver(ndr, NDR_SCALARS, &r->in.driver));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_decode_ntprinting_driver(struct ndr_pull *ndr, int flags, struct decode_ntprinting_driver *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_pull_ntprinting_driver(ndr, NDR_SCALARS, &r->in.driver));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_decode_ntprinting_driver(struct ndr_print *ndr, const char *name, int flags, const struct decode_ntprinting_driver *r)
{
	ndr_print_struct(ndr, name, "decode_ntprinting_driver");
	if (r == NULL) { ndr_print_null(ndr); return; }
	ndr->depth++;
	if (flags & NDR_SET_VALUES) {
		ndr->flags |= LIBNDR_PRINT_SET_VALUES;
	}
	if (flags & NDR_IN) {
		ndr_print_struct(ndr, "in", "decode_ntprinting_driver");
		ndr->depth++;
		ndr_print_ntprinting_driver(ndr, "driver", &r->in.driver);
		ndr->depth--;
	}
	if (flags & NDR_OUT) {
		ndr_print_struct(ndr, "out", "decode_ntprinting_driver");
		ndr->depth++;
		ndr->depth--;
	}
	ndr->depth--;
}

_PUBLIC_ enum ndr_err_code ndr_push_decode_ntprinting_printer(struct ndr_push *ndr, int flags, const struct decode_ntprinting_printer *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_push_ntprinting_printer(ndr, NDR_SCALARS|NDR_BUFFERS, &r->in.printer));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ enum ndr_err_code ndr_pull_decode_ntprinting_printer(struct ndr_pull *ndr, int flags, struct decode_ntprinting_printer *r)
{
	if (flags & NDR_IN) {
		NDR_CHECK(ndr_pull_ntprinting_printer(ndr, NDR_SCALARS|NDR_BUFFERS, &r->in.printer));
	}
	if (flags & NDR_OUT) {
	}
	return NDR_ERR_SUCCESS;
}

_PUBLIC_ void ndr_print_decode_ntprinting_printer(struct ndr_print *ndr, const char *name, int flags, const struct decode_ntprinting_printer *r)
{
	ndr_print_struct(ndr, name, "decode_ntprinting_printer");
	if (r == NULL) { ndr_print_null(ndr); return; }
	ndr->depth++;
	if (flags & NDR_SET_VALUES) {
		ndr->flags |= LIBNDR_PRINT_SET_VALUES;
	}
	if (flags & NDR_IN) {
		ndr_print_struct(ndr, "in", "decode_ntprinting_printer");
		ndr->depth++;
		ndr_print_ntprinting_printer(ndr, "printer", &r->in.printer);
		ndr->depth--;
	}
	if (flags & NDR_OUT) {
		ndr_print_struct(ndr, "out", "decode_ntprinting_printer");
		ndr->depth++;
		ndr->depth--;
	}
	ndr->depth--;
}

static const struct ndr_interface_call ntprinting_calls[] = {
	{
		"decode_ntprinting_form",
		sizeof(struct decode_ntprinting_form),
		(ndr_push_flags_fn_t) ndr_push_decode_ntprinting_form,
		(ndr_pull_flags_fn_t) ndr_pull_decode_ntprinting_form,
		(ndr_print_function_t) ndr_print_decode_ntprinting_form,
		{ 0, NULL },
		{ 0, NULL },
	},
	{
		"decode_ntprinting_driver",
		sizeof(struct decode_ntprinting_driver),
		(ndr_push_flags_fn_t) ndr_push_decode_ntprinting_driver,
		(ndr_pull_flags_fn_t) ndr_pull_decode_ntprinting_driver,
		(ndr_print_function_t) ndr_print_decode_ntprinting_driver,
		{ 0, NULL },
		{ 0, NULL },
	},
	{
		"decode_ntprinting_printer",
		sizeof(struct decode_ntprinting_printer),
		(ndr_push_flags_fn_t) ndr_push_decode_ntprinting_printer,
		(ndr_pull_flags_fn_t) ndr_pull_decode_ntprinting_printer,
		(ndr_print_function_t) ndr_print_decode_ntprinting_printer,
		{ 0, NULL },
		{ 0, NULL },
	},
	{ NULL, 0, NULL, NULL, NULL }
};

static const char * const ntprinting_endpoint_strings[] = {
	"ncacn_np:[\\pipe\\ntprinting]", 
};

static const struct ndr_interface_string_array ntprinting_endpoints = {
	.count	= 1,
	.names	= ntprinting_endpoint_strings
};

static const char * const ntprinting_authservice_strings[] = {
	"host", 
};

static const struct ndr_interface_string_array ntprinting_authservices = {
	.count	= 1,
	.names	= ntprinting_authservice_strings
};


const struct ndr_interface_table ndr_table_ntprinting = {
	.name		= "ntprinting",
	.syntax_id	= {
		{0xa025d3cb,0xc605,0x40d6,{0x86,0xe1},{0x4c,0xff,0x18,0xe7,0xdd,0x94}},
		NDR_NTPRINTING_VERSION
	},
	.helpstring	= NDR_NTPRINTING_HELPSTRING,
	.num_calls	= 3,
	.calls		= ntprinting_calls,
	.endpoints	= &ntprinting_endpoints,
	.authservices	= &ntprinting_authservices
};

