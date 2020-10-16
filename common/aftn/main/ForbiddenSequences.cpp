#include "ForbiddenSequences.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ForbiddenSequences::ForbiddenSequences()
{
}

const QByteArray ForbiddenSequences::save() const
{
    QReadLocker locker(&rwLock);

    QByteArray ret;

    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << mForbiddenLatinSequences;
    stream << mForbiddenCyrillicSequences;
    stream << mForbiddenSequences;

    return ret;
}

void ForbiddenSequences::restore(const QByteArray &data)
{
    QWriteLocker locker(&rwLock);

    QDataStream stream(data);
    stream >> mForbiddenLatinSequences;
    stream >> mForbiddenCyrillicSequences;
    stream >> mForbiddenSequences;
}

const QStringList ForbiddenSequences::forbiddenSequences() const
{
    QReadLocker locker(&rwLock);
    return mForbiddenSequences;
}

const QStringList ForbiddenSequences::forbiddenLatinSequences() const
{
    QReadLocker locker(&rwLock);
    return mForbiddenLatinSequences;
}

const QStringList ForbiddenSequences::forbiddenCyrillicSequences() const
{
    QReadLocker locker(&rwLock);
    return mForbiddenCyrillicSequences;
}

const QStringList ForbiddenSequences::forbiddenSequences(ParseVersion::Version v) const
{
    QReadLocker locker(&rwLock);
    switch (v)
    {
    case ParseVersion::PV_LATIN:
        return forbiddenLatinSequences();
    case ParseVersion::PV_CYRILLIC:
        return forbiddenLatinSequences();
    default:
        return forbiddenSequences();
    }
}

bool ForbiddenSequences::hasForbiddenSequences(const QString &text) const
{
    QReadLocker locker(&rwLock);
    return pHasForbiddenSequence(forbiddenSequences(), text);
}

bool ForbiddenSequences::hasForbiddenLatinSequences(const QString &text) const
{
    QReadLocker locker(&rwLock);
    return pHasForbiddenSequence(forbiddenLatinSequences(), text);
}

bool ForbiddenSequences::hasForbiddenCyrillicSequences(const QString &text) const
{
    QReadLocker locker(&rwLock);
    return pHasForbiddenSequence(forbiddenCyrillicSequences(), text);
}

bool ForbiddenSequences::hasForbiddenSequences(const QString &text, ParseVersion::Version v) const
{
    QReadLocker locker(&rwLock);
    return pHasForbiddenSequence(forbiddenSequences(v), text);
}

void ForbiddenSequences::setForbiddenSequences(const QStringList &sequences)
{
    QWriteLocker locker(&rwLock);
    mForbiddenLatinSequences.clear();
    mForbiddenCyrillicSequences.clear();
    mForbiddenSequences.clear();
    pAddForbiddenSequences(sequences);
}

void ForbiddenSequences::addForbiddenSequences(const QStringList &sequences)
{
    QWriteLocker locker(&rwLock);
    pAddForbiddenSequences(sequences);
}

void ForbiddenSequences::addForbiddenSequence(const QString &sequence, ParseVersion::Version v)
{
    QWriteLocker locker(&rwLock);
    pAddForbiddenSequence(sequence, v);
}

bool ForbiddenSequences::pHasForbiddenSequence(const QStringList &list, const QString &text) const
{
    foreach (const QString &seq, list)
        if (text.contains(seq))
            return true;
    return false;
}

void ForbiddenSequences::pAddForbiddenSequences(const QStringList &sequences)
{
    foreach (const QString &s, sequences)
        pAddForbiddenSequence(s);
}

void ForbiddenSequences::pAddForbiddenSequence(const QString &sequence, ParseVersion::Version v)
{
    switch (v)
    {
    case ParseVersion::PV_LATIN:
        mForbiddenLatinSequences << sequence;
        mForbiddenSequences << sequence;
        break;
    case ParseVersion::PV_CYRILLIC:
        mForbiddenCyrillicSequences << sequence;
        mForbiddenSequences << sequence;
        break;
    case ParseVersion::PV_UNKNOWN:
    {
        ParseVersion::Version definedVersion = ParseVersion::textVersion(sequence, ParseVersion::PV_UNKNOWN);
        switch (definedVersion)
        {
        case ParseVersion::PV_LATIN:
            mForbiddenLatinSequences << sequence;
            mForbiddenSequences << sequence;
            break;
        case ParseVersion::PV_CYRILLIC:
            mForbiddenCyrillicSequences << sequence;
            mForbiddenSequences << sequence;
            break;
        case ParseVersion::PV_UNKNOWN:
            mForbiddenLatinSequences << sequence;
            mForbiddenCyrillicSequences << sequence;
            mForbiddenSequences << sequence;
            break;
        }
    }
        break;
    }
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
