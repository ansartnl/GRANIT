/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#ifndef	_ICAOfacilityfunction_H_
#define	_ICAOfacilityfunction_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ICAOfacilityfunction {
	ICAOfacilityfunction_center	= 0,
	ICAOfacilityfunction_approach	= 1,
	ICAOfacilityfunction_tower	= 2,
	ICAOfacilityfunction_final	= 3,
	ICAOfacilityfunction_groundControl	= 4,
	ICAOfacilityfunction_clearanceDelivery	= 5,
	ICAOfacilityfunction_departure	= 6,
	ICAOfacilityfunction_control	= 7
} e_ICAOfacilityfunction;

/* ICAOfacilityfunction */
typedef long	 ICAOfacilityfunction_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ICAOfacilityfunction;
asn_struct_free_f ICAOfacilityfunction_free;
asn_struct_print_f ICAOfacilityfunction_print;
asn_constr_check_f ICAOfacilityfunction_constraint;
ber_type_decoder_f ICAOfacilityfunction_decode_ber;
der_type_encoder_f ICAOfacilityfunction_encode_der;
xer_type_decoder_f ICAOfacilityfunction_decode_xer;
xer_type_encoder_f ICAOfacilityfunction_encode_xer;
per_type_decoder_f ICAOfacilityfunction_decode_uper;
per_type_encoder_f ICAOfacilityfunction_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ICAOfacilityfunction_H_ */
#include <asn_internal.h>
