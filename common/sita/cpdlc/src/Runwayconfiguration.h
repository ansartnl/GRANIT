/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_Runwayconfiguration_H_
#define	_Runwayconfiguration_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Runwayconfiguration {
	Runwayconfiguration_left	= 0,
	Runwayconfiguration_right	= 1,
	Runwayconfiguration_center	= 2,
	Runwayconfiguration_none	= 3
} e_Runwayconfiguration;

/* Runwayconfiguration */
typedef long	 Runwayconfiguration_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Runwayconfiguration;
asn_struct_free_f Runwayconfiguration_free;
asn_struct_print_f Runwayconfiguration_print;
asn_constr_check_f Runwayconfiguration_constraint;
ber_type_decoder_f Runwayconfiguration_decode_ber;
der_type_encoder_f Runwayconfiguration_encode_der;
xer_type_decoder_f Runwayconfiguration_decode_xer;
xer_type_encoder_f Runwayconfiguration_encode_xer;
per_type_decoder_f Runwayconfiguration_decode_uper;
per_type_encoder_f Runwayconfiguration_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _Runwayconfiguration_H_ */
#include <asn_internal.h>