#include "SupportedTypes.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SupportedTypes::SupportedTypes()
{
}

const QByteArray SupportedTypes::save() const
{
    QReadLocker locker(&rwLock);

    QByteArray ret;

    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << mSupportedLatinTypes;
    stream << mSupportedCyrillicTypes;
    stream << mSupportedMixedTypes;
    stream << mDisabledDisambiguations;

    return ret;
}

void SupportedTypes::restore(const QByteArray &data)
{
    QWriteLocker locker(&rwLock);

    QDataStream stream(data);
    stream >> mSupportedLatinTypes;
    stream >> mSupportedCyrillicTypes;
    stream >> mSupportedMixedTypes;
    stream >> mDisabledDisambiguations;
}

const QStringList SupportedTypes::supportedTypes(bool andDefault) const
{
    QReadLocker locker(&rwLock);
    QStringList ret = mSupportedLatinTypes + mSupportedCyrillicTypes + mSupportedMixedTypes;
    if (andDefault)
        ret += defaultType();
    return ret;
}

const QStringList SupportedTypes::supportedLatinTypes() const
{
    QReadLocker locker(&rwLock);
    return mSupportedLatinTypes;
}

const QStringList SupportedTypes::supportedCyrillicTypes() const
{
    QReadLocker locker(&rwLock);
    return mSupportedCyrillicTypes;
}

const QStringList SupportedTypes::supportedMixedTypes() const
{
    QReadLocker locker(&rwLock);
    return mSupportedMixedTypes;
}

bool SupportedTypes::isSupported(const QString &type, bool orDefault) const
{
    QReadLocker locker(&rwLock);
    return (isLatin(type) || isCyrillic(type) || isMixed(type) || (orDefault && isDefault(type)));
}

bool SupportedTypes::isLatin(const QString &type) const
{
    QReadLocker locker(&rwLock);
    return mSupportedLatinTypes.contains(type);
}

bool SupportedTypes::isCyrillic(const QString &type) const
{
    QReadLocker locker(&rwLock);
    return mSupportedCyrillicTypes.contains(type);
}

bool SupportedTypes::isMixed(const QString &type) const
{
    QReadLocker locker(&rwLock);
    return mSupportedMixedTypes.contains(type);
}

ParseVersion::Version SupportedTypes::version(const QString &type) const
{
    QReadLocker locker(&rwLock);
    if (isLatin(type))
        return ParseVersion::PV_LATIN;
    if (isCyrillic(type))
        return ParseVersion::PV_CYRILLIC;
    if (isMixed(type))
        return ParseVersion::PV_MIXED;
    return ParseVersion::PV_UNKNOWN;
}

void SupportedTypes::setSupportedTypes(const QStringList &types)
{
    QWriteLocker locker(&rwLock);

    mSupportedLatinTypes.clear();
    mSupportedCyrillicTypes.clear();
    mSupportedMixedTypes.clear();   // not sure about this
    pAddSupportedTypes(types);
}

void SupportedTypes::addSupportedTypes(const QStringList &types)
{
    QWriteLocker locker(&rwLock);
    pAddSupportedTypes(types);
}

void SupportedTypes::addSupportedType(const QString &type, ParseVersion::Version v)
{
    QWriteLocker locker(&rwLock);
    pAddSupportedType(type, v);
}

void SupportedTypes::setSupportedMixedTypes(const QStringList &types)
{
    QWriteLocker locker(&rwLock);
    mSupportedMixedTypes.clear();
    pAddSupportedMixedTypes(types);
}

void SupportedTypes::addSupportedMixedTypes(const QStringList &types)
{
    QWriteLocker locker(&rwLock);
    pAddSupportedMixedTypes(types);
}

void SupportedTypes::addSupportedMixedType(const QString &type)
{
    QReadLocker locker(&rwLock);
    pAddSupportedMixedTypes(QStringList() << type);
}

const QString SupportedTypes::defaultType()
{
    return TelegramType::Unknown;
}

bool SupportedTypes::isDefault(const QString &type)
{
    return (type == defaultType());
}

void SupportedTypes::addDisabledDisambiguation(const QString &type, int disambiguation)
{
    mDisabledDisambiguations.insert(type, disambiguation);
}

void SupportedTypes::clearDisabledDisambiguations()
{
    mDisabledDisambiguations.clear();
}

bool SupportedTypes::isDisabled(const QString &type, int disambiguation)
{
    return mDisabledDisambiguations.values(type).contains(disambiguation);
}

void SupportedTypes::pAddSupportedTypes(const QStringList &types)
{
    foreach (const QString &type, types)
        pAddSupportedType(type);
}

void SupportedTypes::pAddSupportedType(const QString &type, ParseVersion::Version v)
{
    switch (v)
    {
    case ParseVersion::PV_LATIN:
        mSupportedLatinTypes << type;
        break;
    case ParseVersion::PV_CYRILLIC:
        mSupportedCyrillicTypes << type;
        break;
    case ParseVersion::PV_UNKNOWN:
    {
        ParseVersion::Version definedVersion = ParseVersion::textVersion(type, ParseVersion::PV_UNKNOWN);
        switch (definedVersion)
        {
        case ParseVersion::PV_LATIN:
            mSupportedLatinTypes << type;
            break;
        case ParseVersion::PV_CYRILLIC:
            mSupportedCyrillicTypes << type;
            break;
        case ParseVersion::PV_UNKNOWN:
            mSupportedMixedTypes << type;
            break;
        }
    }
        break;
    }
}

void SupportedTypes::pAddSupportedMixedTypes(const QStringList &types)
{
    foreach (const QString &type, types)
        mSupportedMixedTypes << type;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
