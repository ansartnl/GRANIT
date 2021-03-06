/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_ATCuplinkmessage_H_
#define	_ATCuplinkmessage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ATCmessageheader.h"
#include "ATCuplinkmsgelementid.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ATCuplinkmsgelementid_seqOf;

/* ATCuplinkmessage */
typedef struct ATCuplinkmessage {
	ATCmessageheader_t	 aTCmessageheader;
	ATCuplinkmsgelementid_t	 aTCuplinkmsgelementid;
	struct ATCuplinkmsgelementid_seqOf	*aTCuplinkmsgelementid_seqOf	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ATCuplinkmessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ATCuplinkmessage;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ATCuplinkmsgelementid-seqOf.h"

#endif	/* _ATCuplinkmessage_H_ */
#include <asn_internal.h>
