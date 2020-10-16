/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#include "ICAOunitnameFrequency.h"

static asn_TYPE_member_t asn_MBR_ICAOunitnameFrequency_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ICAOunitnameFrequency, iCAOunitname),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ICAOunitname,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iCAOunitname"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ICAOunitnameFrequency, frequency),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Frequency,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"frequency"
		},
};
static const ber_tlv_tag_t asn_DEF_ICAOunitnameFrequency_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ICAOunitnameFrequency_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 0, 0, 0 }, /* iCAOunitname */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 1, 0, 0 }, /* frequencyhf */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* frequencyvhf */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 1, 0, 0 }, /* frequencyuhf */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 1, 0, 0 } /* frequencysatchannel */
};
static asn_SEQUENCE_specifics_t asn_SPC_ICAOunitnameFrequency_specs_1 = {
	sizeof(struct ICAOunitnameFrequency),
	offsetof(struct ICAOunitnameFrequency, _asn_ctx),
	asn_MAP_ICAOunitnameFrequency_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ICAOunitnameFrequency = {
	"ICAOunitnameFrequency",
	"ICAOunitnameFrequency",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_ICAOunitnameFrequency_tags_1,
	sizeof(asn_DEF_ICAOunitnameFrequency_tags_1)
		/sizeof(asn_DEF_ICAOunitnameFrequency_tags_1[0]), /* 1 */
	asn_DEF_ICAOunitnameFrequency_tags_1,	/* Same as above */
	sizeof(asn_DEF_ICAOunitnameFrequency_tags_1)
		/sizeof(asn_DEF_ICAOunitnameFrequency_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ICAOunitnameFrequency_1,
	2,	/* Elements count */
	&asn_SPC_ICAOunitnameFrequency_specs_1	/* Additional specs */
};

