/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Beaconcode_H_
#define	_Beaconcode_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Beaconcodeoctaldigit.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Beaconcode */
typedef struct Beaconcode {
	A_SEQUENCE_OF(Beaconcodeoctaldigit_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Beaconcode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Beaconcode;

#ifdef __cplusplus
}
#endif

#endif	/* _Beaconcode_H_ */
#include <asn_internal.h>
