/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Airportdestination_H_
#define	_Airportdestination_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Airport.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Airportdestination */
typedef Airport_t	 Airportdestination_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Airportdestination;
asn_struct_free_f Airportdestination_free;
asn_struct_print_f Airportdestination_print;
asn_constr_check_f Airportdestination_constraint;
ber_type_decoder_f Airportdestination_decode_ber;
der_type_encoder_f Airportdestination_encode_der;
xer_type_decoder_f Airportdestination_decode_xer;
xer_type_encoder_f Airportdestination_encode_xer;
per_type_decoder_f Airportdestination_decode_uper;
per_type_encoder_f Airportdestination_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Airportdestination_H_ */
#include <asn_internal.h>
