/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#include "UM90AltitudePosition.h"

int
UM90AltitudePosition_constraint(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	/* Replace with underlying type checker */
	td->check_constraints = asn_DEF_AltitudePosition.check_constraints;
	return td->check_constraints(td, sptr, ctfailcb, app_key);
}

/*
 * This type is implemented using AltitudePosition,
 * so here we adjust the DEF accordingly.
 */
static void
UM90AltitudePosition_1_inherit_TYPE_descriptor(asn_TYPE_descriptor_t *td) {
	td->free_struct    = asn_DEF_AltitudePosition.free_struct;
	td->print_struct   = asn_DEF_AltitudePosition.print_struct;
	td->check_constraints = asn_DEF_AltitudePosition.check_constraints;
	td->ber_decoder    = asn_DEF_AltitudePosition.ber_decoder;
	td->der_encoder    = asn_DEF_AltitudePosition.der_encoder;
	td->xer_decoder    = asn_DEF_AltitudePosition.xer_decoder;
	td->xer_encoder    = asn_DEF_AltitudePosition.xer_encoder;
	td->uper_decoder   = asn_DEF_AltitudePosition.uper_decoder;
	td->uper_encoder   = asn_DEF_AltitudePosition.uper_encoder;
	if(!td->per_constraints)
		td->per_constraints = asn_DEF_AltitudePosition.per_constraints;
	td->elements       = asn_DEF_AltitudePosition.elements;
	td->elements_count = asn_DEF_AltitudePosition.elements_count;
	td->specifics      = asn_DEF_AltitudePosition.specifics;
}

void
UM90AltitudePosition_free(asn_TYPE_descriptor_t *td,
		void *struct_ptr, int contents_only) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	td->free_struct(td, struct_ptr, contents_only);
}

int
UM90AltitudePosition_print(asn_TYPE_descriptor_t *td, const void *struct_ptr,
		int ilevel, asn_app_consume_bytes_f *cb, void *app_key) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->print_struct(td, struct_ptr, ilevel, cb, app_key);
}

asn_dec_rval_t
UM90AltitudePosition_decode_ber(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const void *bufptr, size_t size, int tag_mode) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->ber_decoder(opt_codec_ctx, td, structure, bufptr, size, tag_mode);
}

asn_enc_rval_t
UM90AltitudePosition_encode_der(asn_TYPE_descriptor_t *td,
		void *structure, int tag_mode, ber_tlv_tag_t tag,
		asn_app_consume_bytes_f *cb, void *app_key) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->der_encoder(td, structure, tag_mode, tag, cb, app_key);
}

asn_dec_rval_t
UM90AltitudePosition_decode_xer(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const char *opt_mname, const void *bufptr, size_t size) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->xer_decoder(opt_codec_ctx, td, structure, opt_mname, bufptr, size);
}

asn_enc_rval_t
UM90AltitudePosition_encode_xer(asn_TYPE_descriptor_t *td, void *structure,
		int ilevel, enum xer_encoder_flags_e flags,
		asn_app_consume_bytes_f *cb, void *app_key) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->xer_encoder(td, structure, ilevel, flags, cb, app_key);
}

asn_dec_rval_t
UM90AltitudePosition_decode_uper(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints, void **structure, asn_per_data_t *per_data) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->uper_decoder(opt_codec_ctx, td, constraints, structure, per_data);
}

asn_enc_rval_t
UM90AltitudePosition_encode_uper(asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints,
		void *structure, asn_per_outp_t *per_out) {
	UM90AltitudePosition_1_inherit_TYPE_descriptor(td);
	return td->uper_encoder(td, constraints, structure, per_out);
}

static const ber_tlv_tag_t asn_DEF_UM90AltitudePosition_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_UM90AltitudePosition = {
	"UM90AltitudePosition",
	"UM90AltitudePosition",
	UM90AltitudePosition_free,
	UM90AltitudePosition_print,
	UM90AltitudePosition_constraint,
	UM90AltitudePosition_decode_ber,
	UM90AltitudePosition_encode_der,
	UM90AltitudePosition_decode_xer,
	UM90AltitudePosition_encode_xer,
	UM90AltitudePosition_decode_uper,
	UM90AltitudePosition_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_UM90AltitudePosition_tags_1,
	sizeof(asn_DEF_UM90AltitudePosition_tags_1)
		/sizeof(asn_DEF_UM90AltitudePosition_tags_1[0]), /* 1 */
	asn_DEF_UM90AltitudePosition_tags_1,	/* Same as above */
	sizeof(asn_DEF_UM90AltitudePosition_tags_1)
		/sizeof(asn_DEF_UM90AltitudePosition_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	0, 0,	/* Defined elsewhere */
	0	/* No specifics */
};

