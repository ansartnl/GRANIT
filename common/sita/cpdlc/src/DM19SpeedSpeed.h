/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_DM19SpeedSpeed_H_
#define	_DM19SpeedSpeed_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Speed;

/* DM19SpeedSpeed */
typedef struct DM19SpeedSpeed {
	A_SEQUENCE_OF(struct Speed) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DM19SpeedSpeed_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DM19SpeedSpeed;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Speed.h"

#endif	/* _DM19SpeedSpeed_H_ */
#include <asn_internal.h>