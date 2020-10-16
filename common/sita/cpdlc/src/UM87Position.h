/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_UM87Position_H_
#define	_UM87Position_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Position.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UM87Position */
typedef Position_t	 UM87Position_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UM87Position;
asn_struct_free_f UM87Position_free;
asn_struct_print_f UM87Position_print;
asn_constr_check_f UM87Position_constraint;
ber_type_decoder_f UM87Position_decode_ber;
der_type_encoder_f UM87Position_encode_der;
xer_type_decoder_f UM87Position_decode_xer;
xer_type_encoder_f UM87Position_encode_xer;
per_type_decoder_f UM87Position_decode_uper;
per_type_encoder_f UM87Position_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _UM87Position_H_ */
#include <asn_internal.h>
