/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_ICAOunitname_H_
#define	_ICAOunitname_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ICAOfacilityidentification.h"
#include "ICAOfacilityfunction.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ICAOunitname */
typedef struct ICAOunitname {
	ICAOfacilityidentification_t	 iCAOfacilityidentification;
	ICAOfacilityfunction_t	 iCAOfacilityfunction;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ICAOunitname_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ICAOunitname;

#ifdef __cplusplus
}
#endif

#endif	/* _ICAOunitname_H_ */
#include <asn_internal.h>
