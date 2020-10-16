/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_ATWaltitudetolerance_H_
#define	_ATWaltitudetolerance_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ATWaltitudetolerance {
	ATWaltitudetolerance_at	= 0,
	ATWaltitudetolerance_atorabove	= 1,
	ATWaltitudetolerance_atorbelow	= 2
} e_ATWaltitudetolerance;

/* ATWaltitudetolerance */
typedef long	 ATWaltitudetolerance_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ATWaltitudetolerance;
asn_struct_free_f ATWaltitudetolerance_free;
asn_struct_print_f ATWaltitudetolerance_print;
asn_constr_check_f ATWaltitudetolerance_constraint;
ber_type_decoder_f ATWaltitudetolerance_decode_ber;
der_type_encoder_f ATWaltitudetolerance_encode_der;
xer_type_decoder_f ATWaltitudetolerance_decode_xer;
xer_type_encoder_f ATWaltitudetolerance_encode_xer;
per_type_decoder_f ATWaltitudetolerance_decode_uper;
per_type_encoder_f ATWaltitudetolerance_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ATWaltitudetolerance_H_ */
#include <asn_internal.h>
