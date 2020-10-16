/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_SSRequipmentavailable_H_
#define	_SSRequipmentavailable_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SSRequipmentavailable {
	SSRequipmentavailable_nnil	= 0,
	SSRequipmentavailable_atransponderModeA	= 1,
	SSRequipmentavailable_ctransponderModeAandC	= 2,
	SSRequipmentavailable_xtransponderModeS	= 3,
	SSRequipmentavailable_ptransponderModeSPA	= 4,
	SSRequipmentavailable_itransponderModeSID	= 5,
	SSRequipmentavailable_stransponderModeSPAID	= 6
} e_SSRequipmentavailable;

/* SSRequipmentavailable */
typedef long	 SSRequipmentavailable_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SSRequipmentavailable;
asn_struct_free_f SSRequipmentavailable_free;
asn_struct_print_f SSRequipmentavailable_print;
asn_constr_check_f SSRequipmentavailable_constraint;
ber_type_decoder_f SSRequipmentavailable_decode_ber;
der_type_encoder_f SSRequipmentavailable_encode_der;
xer_type_decoder_f SSRequipmentavailable_decode_xer;
xer_type_encoder_f SSRequipmentavailable_encode_xer;
per_type_decoder_f SSRequipmentavailable_decode_uper;
per_type_encoder_f SSRequipmentavailable_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _SSRequipmentavailable_H_ */
#include <asn_internal.h>
