/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Runway_H_
#define	_Runway_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Runwaydirection.h"
#include "Runwayconfiguration.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Runway */
typedef struct Runway {
	Runwaydirection_t	 runwaydirection;
	Runwayconfiguration_t	 runwayconfiguration;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Runway_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Runway;

#ifdef __cplusplus
}
#endif

#endif	/* _Runway_H_ */
#include <asn_internal.h>
