/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_UM88PositionPosition_H_
#define	_UM88PositionPosition_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Position;

/* UM88PositionPosition */
typedef struct UM88PositionPosition {
	A_SEQUENCE_OF(struct Position) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UM88PositionPosition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UM88PositionPosition;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Position.h"

#endif	/* _UM88PositionPosition_H_ */
#include <asn_internal.h>
