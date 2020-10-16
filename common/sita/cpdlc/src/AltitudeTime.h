/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_AltitudeTime_H_
#define	_AltitudeTime_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Altitude.h"
#include "Time.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AltitudeTime */
typedef struct AltitudeTime {
	Altitude_t	 altitude;
	Time_t	 time;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AltitudeTime_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AltitudeTime;

#ifdef __cplusplus
}
#endif

#endif	/* _AltitudeTime_H_ */
#include <asn_internal.h>
