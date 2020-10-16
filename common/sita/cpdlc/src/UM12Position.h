/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_UM12Position_H_
#define	_UM12Position_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Position.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UM12Position */
typedef Position_t	 UM12Position_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UM12Position;
asn_struct_free_f UM12Position_free;
asn_struct_print_f UM12Position_print;
asn_constr_check_f UM12Position_constraint;
ber_type_decoder_f UM12Position_decode_ber;
der_type_encoder_f UM12Position_encode_der;
xer_type_decoder_f UM12Position_decode_xer;
xer_type_encoder_f UM12Position_encode_xer;
per_type_decoder_f UM12Position_decode_uper;
per_type_encoder_f UM12Position_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _UM12Position_H_ */
#include <asn_internal.h>