/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Airwayidentifier_H_
#define	_Airwayidentifier_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Airwayidentifier */
typedef IA5String_t	 Airwayidentifier_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Airwayidentifier;
asn_struct_free_f Airwayidentifier_free;
asn_struct_print_f Airwayidentifier_print;
asn_constr_check_f Airwayidentifier_constraint;
ber_type_decoder_f Airwayidentifier_decode_ber;
der_type_encoder_f Airwayidentifier_encode_der;
xer_type_decoder_f Airwayidentifier_decode_xer;
xer_type_encoder_f Airwayidentifier_encode_xer;
per_type_decoder_f Airwayidentifier_decode_uper;
per_type_encoder_f Airwayidentifier_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Airwayidentifier_H_ */
#include <asn_internal.h>
