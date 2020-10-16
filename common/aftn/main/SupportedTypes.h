#ifndef SUPPORTEDTYPES_H
#define SUPPORTEDTYPES_H

#include "ParseVersion.h"

#include <QtCore/QReadWriteLock>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class SupportedTypes
{
public:
    SupportedTypes();

    const QByteArray save() const;
    void restore(const QByteArray &data);

    const QStringList supportedTypes(bool andDefault = true) const;
    const QStringList supportedLatinTypes() const;
    const QStringList supportedCyrillicTypes() const;
    const QStringList supportedMixedTypes() const;
    bool isSupported(const QString &type, bool orDefault = true) const;
    bool isLatin(const QString &type) const;
    bool isCyrillic(const QString &type) const;
    bool isMixed(const QString &type) const;
    ParseVersion::Version version(const QString &type) const;

    void setSupportedTypes(const QStringList &types);
    void addSupportedTypes(const QStringList &types);
    void addSupportedType(const QString &type, ParseVersion::Version v = ParseVersion::PV_UNKNOWN);
    void setSupportedMixedTypes(const QStringList &types);
    void addSupportedMixedTypes(const QStringList &types);
    void addSupportedMixedType(const QString &type);

    static const QString defaultType();
    static bool isDefault(const QString &type);

    void addDisabledDisambiguation(const QString &type, int disambiguation);
    void clearDisabledDisambiguations();
    bool isDisabled(const QString &type, int disambiguation);

private:
    void pAddSupportedTypes(const QStringList &types);
    void pAddSupportedType(const QString &type, ParseVersion::Version v = ParseVersion::PV_UNKNOWN);
    void pAddSupportedMixedTypes(const QStringList &types);

private:
    QStringList mSupportedLatinTypes;
    QStringList mSupportedCyrillicTypes;
    QStringList mSupportedMixedTypes;

    QMultiHash < QString, int > mDisabledDisambiguations;

    mutable QReadWriteLock rwLock;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SUPPORTEDTYPES_H
