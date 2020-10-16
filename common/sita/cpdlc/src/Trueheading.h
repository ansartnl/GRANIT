/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Trueheading_H_
#define	_Trueheading_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Degrees.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Trueheading */
typedef Degrees_t	 Trueheading_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Trueheading;
asn_struct_free_f Trueheading_free;
asn_struct_print_f Trueheading_print;
asn_constr_check_f Trueheading_constraint;
ber_type_decoder_f Trueheading_decode_ber;
der_type_encoder_f Trueheading_encode_der;
xer_type_decoder_f Trueheading_decode_xer;
xer_type_encoder_f Trueheading_encode_xer;
per_type_decoder_f Trueheading_decode_uper;
per_type_encoder_f Trueheading_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Trueheading_H_ */
#include <asn_internal.h>
