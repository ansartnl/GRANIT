/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_ATCuplinkmsgelementid_seqOf_H_
#define	_ATCuplinkmsgelementid_seqOf_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ATCuplinkmsgelementid;

/* ATCuplinkmsgelementid-seqOf */
typedef struct ATCuplinkmsgelementid_seqOf {
	A_SEQUENCE_OF(struct ATCuplinkmsgelementid) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ATCuplinkmsgelementid_seqOf_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ATCuplinkmsgelementid_seqOf;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ATCuplinkmsgelementid.h"

#endif	/* _ATCuplinkmsgelementid_seqOf_H_ */
#include <asn_internal.h>
