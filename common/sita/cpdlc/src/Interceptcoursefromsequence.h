/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Interceptcoursefromsequence_H_
#define	_Interceptcoursefromsequence_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Interceptcoursefrom;

/* Interceptcoursefromsequence */
typedef struct Interceptcoursefromsequence {
	A_SEQUENCE_OF(struct Interceptcoursefrom) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Interceptcoursefromsequence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Interceptcoursefromsequence;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Interceptcoursefrom.h"

#endif	/* _Interceptcoursefromsequence_H_ */
#include <asn_internal.h>
