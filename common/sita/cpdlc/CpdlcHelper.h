#ifndef CPDLCHELPER_H
#define CPDLCHELPER_H

#include <stdexcept>

#include <QtCore/QMetaType>
#include <QtCore/QString>

class ASN1Exception : public std::runtime_error {
public:
    ASN1Exception(const QString &msg) : std::runtime_error(msg.toStdString()) {}
};

class ASN1DecodeException : public ASN1Exception {
public:
    ASN1DecodeException(const QString &msg) : ASN1Exception(msg) {}
};

class ASN1EncodeException : public ASN1Exception {
public:
    ASN1EncodeException(const QString &msg) : ASN1Exception(msg) {}
};

class ASN1ConstraintsException : public ASN1Exception {
public:
    ASN1ConstraintsException(const QString &msg) : ASN1Exception(msg) {}
};

namespace CpdlcHelper {

enum Type {
    Data,
    ConnectRequest,
    ConnectConfirm,
    DisconnectRequest
};

QByteArray downlinkToPer(const QByteArray &input, Type *type = 0);
QByteArray downlinkToXer(const QByteArray &input, Type *type = 0);

QByteArray uplinkToPer(const QByteArray &input, Type *type = 0);
QByteArray uplinkToXer(const QByteArray &input, Type *type = 0);

};

Q_DECLARE_METATYPE(CpdlcHelper::Type)

#endif // CPDLCHELPER_H
