
#include "ATCdownlinkmessage.h"
#include "ATCuplinkmessage.h"

#include "CpdlcHelper.h"

int writeToByteArray(const void *buffer, size_t size, void *bytearray)
{
    QByteArray *out = (QByteArray*)bytearray;

    out->append((const char*)buffer, size);

    return 0;
}

void checkConstraints(void *message, asn_TYPE_descriptor_s *type)
{
    char errbuf[256];
    size_t errlen = sizeof(errbuf);

    if (asn_check_constraints(type, message, errbuf, &errlen) == -1) {
        throw ASN1ConstraintsException(QString::fromUtf8(errbuf));
    }
}

CpdlcHelper::Type getType(void *message, asn_TYPE_descriptor_s *type)
{
    CpdlcHelper::Type result = CpdlcHelper::Data;

    if (type == &asn_DEF_ATCdownlinkmessage) {
        switch (((ATCdownlinkmessage_t*)message)->aTCdownlinkmsgelementid.present) {
        case ATCdownlinkmsgelementid_PR_dM73Versionnumber:
            result = CpdlcHelper::ConnectConfirm;
            break;
        default:
            break;
        }
    }

    if (type == &asn_DEF_ATCuplinkmessage) {
        switch (((ATCuplinkmessage_t*)message)->aTCuplinkmsgelementid.present) {
        case ATCuplinkmsgelementid_PR_uM163ICAOfacilitydesignationTp4table:
            result = CpdlcHelper::ConnectRequest;
            break;
        case ATCuplinkmsgelementid_PR_uM159Errorinformation:
            result = CpdlcHelper::DisconnectRequest;
            break;
        default:
            break;
        }
    }

    return result;
}

QByteArray toPer(const QByteArray &input, asn_TYPE_descriptor_s *type, CpdlcHelper::Type *cpdlcType)
{
    void *message = 0;

    try {
        asn_dec_rval_t dec = xer_decode(0, type, (void **)&message, input.constData(), input.size());
        if (dec.code != RC_OK) {
            throw ASN1DecodeException("Unable to decode");
        }

        checkConstraints(message, type);

        QByteArray result;
        asn_enc_rval_t enc = uper_encode(type, message, writeToByteArray, &result);
        if (enc.encoded == -1) {
            throw ASN1EncodeException(QString("Unable to encode %1").arg(enc.failed_type->xml_tag));
        }

        if (cpdlcType) {
            *cpdlcType = getType(message, type);
        }

        type->free_struct(type, message, 0);

        return result;
    }
    catch (...) {
        type->free_struct(type, message, 0);
        throw;
    }
}

QByteArray toXer(const QByteArray &input, asn_TYPE_descriptor_s *type, CpdlcHelper::Type *cpdlcType)
{
    void *message = 0;

    try {
        asn_dec_rval_t dec = uper_decode(0, type, (void **)&message, input.constData(), input.size(), 0, 0);
        if (dec.code != RC_OK) {
            throw ASN1DecodeException("Unable to decode");
        }

        checkConstraints(message, type);

        QByteArray result;
        asn_enc_rval_t enc = xer_encode(type, message, XER_F_BASIC, writeToByteArray, &result);
        if (enc.encoded == -1) {
            throw ASN1EncodeException("Unable to encode");
        }

        if (cpdlcType) {
            *cpdlcType = getType(message, type);
        }

        type->free_struct(type, message, 0);

        return result;
    }
    catch (...) {
        type->free_struct(type, message, 0);
        throw;
    }
}

namespace CpdlcHelper {

QByteArray downlinkToPer(const QByteArray &input, Type *type)
{
    return toPer(input, &asn_DEF_ATCdownlinkmessage, type);
}

QByteArray downlinkToXer(const QByteArray &input, Type *type)
{
    return toXer(input, &asn_DEF_ATCdownlinkmessage, type);
}

QByteArray uplinkToPer(const QByteArray &input, Type *type)
{
    return toPer(input, &asn_DEF_ATCuplinkmessage, type);
}

QByteArray uplinkToXer(const QByteArray &input, Type *type)
{
    return toXer(input, &asn_DEF_ATCuplinkmessage, type);
}

}

