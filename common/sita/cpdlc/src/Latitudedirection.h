/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Latitudedirection_H_
#define	_Latitudedirection_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Latitudedirection {
	Latitudedirection_north	= 0,
	Latitudedirection_south	= 1
} e_Latitudedirection;

/* Latitudedirection */
typedef long	 Latitudedirection_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Latitudedirection;
asn_struct_free_f Latitudedirection_free;
asn_struct_print_f Latitudedirection_print;
asn_constr_check_f Latitudedirection_constraint;
ber_type_decoder_f Latitudedirection_decode_ber;
der_type_encoder_f Latitudedirection_encode_der;
xer_type_decoder_f Latitudedirection_decode_xer;
xer_type_encoder_f Latitudedirection_encode_xer;
per_type_decoder_f Latitudedirection_decode_uper;
per_type_encoder_f Latitudedirection_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Latitudedirection_H_ */
#include <asn_internal.h>