/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Routeinformationadditional_H_
#define	_Routeinformationadditional_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ATWalongtrackwaypointsequence;
struct Reportingpoints;
struct Interceptcoursefromsequence;
struct Holdatwaypointsequence;
struct Waypointspeedaltitudesequence;
struct RTArequiredtimearrivalsequence;

/* Routeinformationadditional */
typedef struct Routeinformationadditional {
	struct ATWalongtrackwaypointsequence	*aTWalongtrackwaypointsequence	/* OPTIONAL */;
	struct Reportingpoints	*reportingpoints	/* OPTIONAL */;
	struct Interceptcoursefromsequence	*interceptcoursefromsequence	/* OPTIONAL */;
	struct Holdatwaypointsequence	*holdatwaypointsequence	/* OPTIONAL */;
	struct Waypointspeedaltitudesequence	*waypointspeedaltitudesequence	/* OPTIONAL */;
	struct RTArequiredtimearrivalsequence	*rTArequiredtimearrivalsequenc	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Routeinformationadditional_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Routeinformationadditional;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ATWalongtrackwaypointsequence.h"
#include "Reportingpoints.h"
#include "Interceptcoursefromsequence.h"
#include "Holdatwaypointsequence.h"
#include "Waypointspeedaltitudesequence.h"
#include "RTArequiredtimearrivalsequence.h"

#endif	/* _Routeinformationadditional_H_ */
#include <asn_internal.h>