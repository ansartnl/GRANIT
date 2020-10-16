/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#include "Placebearingdistance.h"

static asn_TYPE_member_t asn_MBR_Placebearingdistance_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Placebearingdistance, fixname),
		(ASN_TAG_CLASS_UNIVERSAL | (22 << 2)),
		0,
		&asn_DEF_Fixname,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fixname"
		},
	{ ATF_POINTER, 1, offsetof(struct Placebearingdistance, latitudelongitude),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_LatitudeLongitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"latitudelongitude"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Placebearingdistance, degrees),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Degrees,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"degrees"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Placebearingdistance, distance),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Distance,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"distance"
		},
};
static const int asn_MAP_Placebearingdistance_oms_1[] = { 1 };
static const ber_tlv_tag_t asn_DEF_Placebearingdistance_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Placebearingdistance_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 1, 0, 0 }, /* latitudelongitude */
    { (ASN_TAG_CLASS_UNIVERSAL | (22 << 2)), 0, 0, 0 }, /* fixname */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 2, 0, 1 }, /* degreesmagnetic */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 3, -1, 0 }, /* distancenm */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 2, 0, 1 }, /* degreestrue */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 3, -1, 0 } /* distancekm */
};
static asn_SEQUENCE_specifics_t asn_SPC_Placebearingdistance_specs_1 = {
	sizeof(struct Placebearingdistance),
	offsetof(struct Placebearingdistance, _asn_ctx),
	asn_MAP_Placebearingdistance_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_Placebearingdistance_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_Placebearingdistance = {
	"Placebearingdistance",
	"Placebearingdistance",
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
	asn_DEF_Placebearingdistance_tags_1,
	sizeof(asn_DEF_Placebearingdistance_tags_1)
		/sizeof(asn_DEF_Placebearingdistance_tags_1[0]), /* 1 */
	asn_DEF_Placebearingdistance_tags_1,	/* Same as above */
	sizeof(asn_DEF_Placebearingdistance_tags_1)
		/sizeof(asn_DEF_Placebearingdistance_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_Placebearingdistance_1,
	4,	/* Elements count */
	&asn_SPC_Placebearingdistance_specs_1	/* Additional specs */
};

