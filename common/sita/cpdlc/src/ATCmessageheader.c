/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#include "ATCmessageheader.h"

static asn_TYPE_member_t asn_MBR_ATCmessageheader_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ATCmessageheader, msgidentificationnumber),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_Msgidentificationnumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"msgidentificationnumber"
		},
	{ ATF_POINTER, 2, offsetof(struct ATCmessageheader, msgreferencenumber),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_Msgreferencenumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"msgreferencenumber"
		},
	{ ATF_POINTER, 1, offsetof(struct ATCmessageheader, timestamp),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Timestamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"timestamp"
		},
};
static const int asn_MAP_ATCmessageheader_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_ATCmessageheader_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ATCmessageheader_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 1 }, /* msgidentificationnumber */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, -1, 0 }, /* msgreferencenumber */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 2, 0, 0 } /* timestamp */
};
static asn_SEQUENCE_specifics_t asn_SPC_ATCmessageheader_specs_1 = {
	sizeof(struct ATCmessageheader),
	offsetof(struct ATCmessageheader, _asn_ctx),
	asn_MAP_ATCmessageheader_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_ATCmessageheader_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ATCmessageheader = {
	"ATCmessageheader",
	"ATCmessageheader",
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
	asn_DEF_ATCmessageheader_tags_1,
	sizeof(asn_DEF_ATCmessageheader_tags_1)
		/sizeof(asn_DEF_ATCmessageheader_tags_1[0]), /* 1 */
	asn_DEF_ATCmessageheader_tags_1,	/* Same as above */
	sizeof(asn_DEF_ATCmessageheader_tags_1)
		/sizeof(asn_DEF_ATCmessageheader_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ATCmessageheader_1,
	3,	/* Elements count */
	&asn_SPC_ATCmessageheader_specs_1	/* Additional specs */
};

