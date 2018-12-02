/* header auto-generated by pidl */

#include "librpc/ndr/libndr.h"
#include "librpc/gen_ndr/dnsp.h"

#ifndef _HEADER_NDR_dnsp
#define _HEADER_NDR_dnsp

#include "../librpc/ndr/ndr_dnsp.h"
#define NDR_DNSP_UUID "bdd66e9e-d45f-4202-85c0-6132edc4f30a"
#define NDR_DNSP_VERSION 0.0
#define NDR_DNSP_NAME "dnsp"
#define NDR_DNSP_HELPSTRING "DNSP interfaces"
extern const struct ndr_interface_table ndr_table_dnsp;
#define NDR_DECODE_DNSSRVRPCRECORD (0x00)

#define NDR_DNSP_CALL_COUNT (1)
void ndr_print_dns_record_type(struct ndr_print *ndr, const char *name, enum dns_record_type r);
void ndr_print_dns_record_rank(struct ndr_print *ndr, const char *name, enum dns_record_rank r);
enum ndr_err_code ndr_push_dnsp_soa(struct ndr_push *ndr, int ndr_flags, const struct dnsp_soa *r);
enum ndr_err_code ndr_pull_dnsp_soa(struct ndr_pull *ndr, int ndr_flags, struct dnsp_soa *r);
void ndr_print_dnsp_soa(struct ndr_print *ndr, const char *name, const struct dnsp_soa *r);
enum ndr_err_code ndr_push_dnsp_mx(struct ndr_push *ndr, int ndr_flags, const struct dnsp_mx *r);
enum ndr_err_code ndr_pull_dnsp_mx(struct ndr_pull *ndr, int ndr_flags, struct dnsp_mx *r);
void ndr_print_dnsp_mx(struct ndr_print *ndr, const char *name, const struct dnsp_mx *r);
enum ndr_err_code ndr_push_dnsp_hinfo(struct ndr_push *ndr, int ndr_flags, const struct dnsp_hinfo *r);
enum ndr_err_code ndr_pull_dnsp_hinfo(struct ndr_pull *ndr, int ndr_flags, struct dnsp_hinfo *r);
void ndr_print_dnsp_hinfo(struct ndr_print *ndr, const char *name, const struct dnsp_hinfo *r);
enum ndr_err_code ndr_push_dnsp_srv(struct ndr_push *ndr, int ndr_flags, const struct dnsp_srv *r);
enum ndr_err_code ndr_pull_dnsp_srv(struct ndr_pull *ndr, int ndr_flags, struct dnsp_srv *r);
void ndr_print_dnsp_srv(struct ndr_print *ndr, const char *name, const struct dnsp_srv *r);
void ndr_print_dnsRecordData(struct ndr_print *ndr, const char *name, const union dnsRecordData *r);
enum ndr_err_code ndr_push_dnsp_DnssrvRpcRecord(struct ndr_push *ndr, int ndr_flags, const struct dnsp_DnssrvRpcRecord *r);
enum ndr_err_code ndr_pull_dnsp_DnssrvRpcRecord(struct ndr_pull *ndr, int ndr_flags, struct dnsp_DnssrvRpcRecord *r);
void ndr_print_dnsp_DnssrvRpcRecord(struct ndr_print *ndr, const char *name, const struct dnsp_DnssrvRpcRecord *r);
void ndr_print_decode_DnssrvRpcRecord(struct ndr_print *ndr, const char *name, int flags, const struct decode_DnssrvRpcRecord *r);
#endif /* _HEADER_NDR_dnsp */
