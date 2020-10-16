#ifndef AFTNPARAMETERS_H
#define AFTNPARAMETERS_H

#include <QtCore/QReadWriteLock>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class ForbiddenSequences;
class SupportedTypes;

class AftnParameters
{
private:
    AftnParameters();
    ~AftnParameters();

public:
    static AftnParameters * instance();

    const QByteArray save() const;
    void restore(const QByteArray &data);

    SupportedTypes * supportedTypes() const;
    ForbiddenSequences *forbiddenSequences()  const;

    void setNonstandardAlternativeAerodromes(bool enabled);
    bool nonstandardAlternativeAerodromesEnabled() const;

    void addMessageProcessorFeatures(int f);
    void removeMessageProcessorFeatures(int f);
    void setMessageProcessorFeatures(int f);
    int messageProcessorFeatures() const;

private:
    SupportedTypes *mSupportedTypes;
    ForbiddenSequences *mForbiddenSequences;
    bool mNonstandardAlternativeAerodromesEnabled;
    int mMessageProcessorFeatures;

    mutable QReadWriteLock rwLock;
};

#define aftnParameters AftnParameters::instance()

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // AFTNPARAMETERS_H
