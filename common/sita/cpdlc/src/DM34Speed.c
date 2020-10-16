/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ATCTwoWayDataLinkCommunications"
 * 	found in "../cpdlc.asn"
 * 	`asn1c -S /home/alex/src/asn1c/skeletons/ -fcompound-names`
 */

#include "DM34Speed.h"

int
DM34Speed_constraint(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	/* Replace with underlying type checker */
	td->check_constraints = asn_DEF_Speed.check_constraints;
	return td->check_constraints(td, sptr, ctfailcb, app_key);
}

/*
 * This type is implemented using Speed,
 * so here we adjust the DEF accordingly.
 */
static void
DM34Speed_1_inherit_TYPE_descriptor(asn_TYPE_descriptor_t *td) {
	td->free_struct    = asn_DEF_Speed.free_struct;
	td->print_struct   = asn_DEF_Speed.print_struct;
	td->check_constraints = asn_DEF_Speed.check_constraints;
	td->ber_decoder    = asn_DEF_Speed.ber_decoder;
	td->der_encoder    = asn_DEF_Speed.der_encoder;
	td->xer_decoder    = asn_DEF_Speed.xer_decoder;
	td->xer_encoder    = asn_DEF_Speed.xer_encoder;
	td->uper_decoder   = asn_DEF_Speed.uper_decoder;
	td->uper_encoder   = asn_DEF_Speed.uper_encoder;
	if(!td->per_constraints)
		td->per_constraints = asn_DEF_Speed.per_constraints;
	td->elements       = asn_DEF_Speed.elements;
	td->elements_count = asn_DEF_Speed.elements_count;
	td->specifics      = asn_DEF_Speed.specifics;
}

void
DM34Speed_free(asn_TYPE_descriptor_t *td,
		void *struct_ptr, int contents_only) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	td->free_struct(td, struct_ptr, contents_only);
}

int
DM34Speed_print(asn_TYPE_descriptor_t *td, const void *struct_ptr,
		int ilevel, asn_app_consume_bytes_f *cb, void *app_key) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->print_struct(td, struct_ptr, ilevel, cb, app_key);
}

asn_dec_rval_t
DM34Speed_decode_ber(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const void *bufptr, size_t size, int tag_mode) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->ber_decoder(opt_codec_ctx, td, structure, bufptr, size, tag_mode);
}

asn_enc_rval_t
DM34Speed_encode_der(asn_TYPE_descriptor_t *td,
		void *structure, int tag_mode, ber_tlv_tag_t tag,
		asn_app_consume_bytes_f *cb, void *app_key) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->der_encoder(td, structure, tag_mode, tag, cb, app_key);
}

asn_dec_rval_t
DM34Speed_decode_xer(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const char *opt_mname, const void *bufptr, size_t size) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->xer_decoder(opt_codec_ctx, td, structure, opt_mname, bufptr, size);
}

asn_enc_rval_t
DM34Speed_encode_xer(asn_TYPE_descriptor_t *td, void *structure,
		int ilevel, enum xer_encoder_flags_e flags,
		asn_app_consume_bytes_f *cb, void *app_key) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->xer_encoder(td, structure, ilevel, flags, cb, app_key);
}

asn_dec_rval_t
DM34Speed_decode_uper(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints, void **structure, asn_per_data_t *per_data) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->uper_decoder(opt_codec_ctx, td, constraints, structure, per_data);
}

asn_enc_rval_t
DM34Speed_encode_uper(asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints,
		void *structure, asn_per_outp_t *per_out) {
	DM34Speed_1_inherit_TYPE_descriptor(td);
	return td->uper_encoder(td, constraints, structure, per_out);
}

static asn_per_constraints_t asn_PER_type_DM34Speed_constr_1 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  7 }	/* (0..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_descriptor_t asn_DEF_DM34Speed = {
	"DM34Speed",
	"DM34Speed",
	DM34Speed_free,
	DM34Speed_print,
	DM34Speed_constraint,
	DM34Speed_decode_ber,
	DM34Speed_encode_der,
	DM34Speed_decode_xer,
	DM34Speed_encode_xer,
	DM34Speed_decode_uper,
	DM34Speed_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	0, 0,	/* Defined elsewhere */
	0	/* No specifics */
};

