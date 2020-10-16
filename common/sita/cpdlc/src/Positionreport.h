/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Positionreport_H_
#define	_Positionreport_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Positioncurrent.h"
#include "Timeatpositioncurrent.h"
#include "Altitude.h"
#include "Turbulence.h"
#include "Icing.h"
#include "Speedground.h"
#include "Supplementaryinformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Position;
struct Time;
struct Remainingfuel;
struct Temperature;
struct Winds;
struct Speed;
struct Verticalchange;
struct Degrees;
struct Distance;
struct Altitude;

/* Positionreport */
typedef struct Positionreport {
	Positioncurrent_t	 positioncurrent;
	Timeatpositioncurrent_t	 timeatpositioncurrent;
	Altitude_t	 altitude;
	struct Position	*fixnext	/* OPTIONAL */;
	struct Time	*timeetaatfixnext	/* OPTIONAL */;
	struct Position	*fixnextplusone	/* OPTIONAL */;
	struct Time	*timeetadestination	/* OPTIONAL */;
	struct Remainingfuel	*remainingfuel	/* OPTIONAL */;
	struct Temperature	*temperature	/* OPTIONAL */;
	struct Winds	*winds	/* OPTIONAL */;
	Turbulence_t	*turbulence	/* OPTIONAL */;
	Icing_t	*icing	/* OPTIONAL */;
	struct Speed	*speed	/* OPTIONAL */;
	Speedground_t	*speedground	/* OPTIONAL */;
	struct Verticalchange	*verticalchange	/* OPTIONAL */;
	struct Degrees	*trackangle	/* OPTIONAL */;
	struct Degrees	*trueheading	/* OPTIONAL */;
	struct Distance	*distance	/* OPTIONAL */;
	Supplementaryinformation_t	*supplementaryinformation	/* OPTIONAL */;
	struct Position	*reportedwaypointposition	/* OPTIONAL */;
	struct Time	*reportedwaypointtime	/* OPTIONAL */;
	struct Altitude	*reportedwaypointaltitude	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Positionreport_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Positionreport;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Fixnext.h"
#include "Timeetaatfixnext.h"
#include "Fixnextplusone.h"
#include "Timeetadestination.h"
#include "Remainingfuel.h"
#include "Temperature.h"
#include "Winds.h"
#include "Speed.h"
#include "Verticalchange.h"
#include "Trackangle.h"
#include "Trueheading.h"
#include "Distance.h"
#include "Reportedwaypointposition.h"
#include "Reportedwaypointtime.h"
#include "Reportedwaypointaltitude.h"

#endif	/* _Positionreport_H_ */
#include <asn_internal.h>
