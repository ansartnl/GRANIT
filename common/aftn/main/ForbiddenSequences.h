#ifndef FORBIDDENSEQUENCES_H
#define FORBIDDENSEQUENCES_H

#include "ParseVersion.h"

#include <QtCore/QReadWriteLock>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class ForbiddenSequences
{
public:
    ForbiddenSequences();

    const QByteArray save() const;
    void restore(const QByteArray &data);

    const QStringList forbiddenSequences() const;
    const QStringList forbiddenLatinSequences() const;
    const QStringList forbiddenCyrillicSequences() const;
    const QStringList forbiddenSequences(ParseVersion::Version v) const;
    bool hasForbiddenSequences(const QString &text) const;
    bool hasForbiddenLatinSequences(const QString &text) const;
    bool hasForbiddenCyrillicSequences(const QString &text) const;
    bool hasForbiddenSequences(const QString &text, ParseVersion::Version v) const;

    void setForbiddenSequences(const QStringList &sequences);
    void addForbiddenSequences(const QStringList &sequences);
    void addForbiddenSequence(const QString &sequence, ParseVersion::Version v = ParseVersion::PV_UNKNOWN);

private:
    bool pHasForbiddenSequence(const QStringList &list, const QString &text) const;

    void pAddForbiddenSequences(const QStringList &sequences);
    void pAddForbiddenSequence(const QString &sequence, ParseVersion::Version v = ParseVersion::PV_UNKNOWN);

private:
    QStringList mForbiddenLatinSequences;
    QStringList mForbiddenCyrillicSequences;
    QStringList mForbiddenSequences;

    mutable QReadWriteLock rwLock;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // FORBIDDENSEQUENCES_H
