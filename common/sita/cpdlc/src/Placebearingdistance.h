/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Placebearingdistance_H_
#define	_Placebearingdistance_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Fixname.h"
#include "Degrees.h"
#include "Distance.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct LatitudeLongitude;

/* Placebearingdistance */
typedef struct Placebearingdistance {
	Fixname_t	 fixname;
	struct LatitudeLongitude	*latitudelongitude	/* OPTIONAL */;
	Degrees_t	 degrees;
	Distance_t	 distance;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Placebearingdistance_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Placebearingdistance;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LatitudeLongitude.h"

#endif	/* _Placebearingdistance_H_ */
#include <asn_internal.h>
