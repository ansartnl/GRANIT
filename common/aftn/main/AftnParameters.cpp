#include "AftnParameters.h"

#include "ForbiddenSequences.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AftnParameters::AftnParameters() :
    mSupportedTypes(new SupportedTypes())
  , mForbiddenSequences(new ForbiddenSequences())
  , mNonstandardAlternativeAerodromesEnabled(false)
  , mMessageProcessorFeatures(0)
{
    mSupportedTypes->setSupportedTypes(TelegramType::allTypes());
    mSupportedTypes->setSupportedMixedTypes(QStringList()
                                            << TelegramType::TPL
                                            << TelegramType::ACK << TelegramType::ACK1
                                            << TelegramType::REJ << TelegramType::REJ1
                                            << TelegramType::GPL);
    mForbiddenSequences->setForbiddenSequences(
                QStringList()
                << "ZCZC" << "NNNN"
                << QString::fromUtf8("ЗЦЗЦ") << QString::fromUtf8("НННН")
                << "+:+:" << ",,,,"
                );
}

AftnParameters::~AftnParameters()
{
    delete mForbiddenSequences;
    delete mSupportedTypes;
}

AftnParameters * AftnParameters::instance()
{
    static AftnParameters *mParams = new AftnParameters();
    return mParams;
}

const QByteArray AftnParameters::save() const
{
    QReadLocker locker(&rwLock);

    QByteArray ret;

    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << mSupportedTypes->save();
    stream << mForbiddenSequences->save();
    stream << mNonstandardAlternativeAerodromesEnabled;
    stream << mMessageProcessorFeatures;

    return ret;
}

void AftnParameters::restore(const QByteArray &data)
{
    QWriteLocker locker(&rwLock);

    QByteArray tmp;

    QDataStream stream(data);
    stream >> tmp;
    mSupportedTypes->restore(tmp);
    stream >> tmp;
    mForbiddenSequences->restore(tmp);
    stream >> mNonstandardAlternativeAerodromesEnabled;
    stream >> mMessageProcessorFeatures;
}

SupportedTypes * AftnParameters::supportedTypes() const
{
    return mSupportedTypes;
}

ForbiddenSequences * AftnParameters::forbiddenSequences() const
{
    return mForbiddenSequences;
}

void AftnParameters::setNonstandardAlternativeAerodromes(bool enabled)
{
    QWriteLocker locker(&rwLock);
    mNonstandardAlternativeAerodromesEnabled = enabled;
}

bool AftnParameters::nonstandardAlternativeAerodromesEnabled() const
{
    QReadLocker locker(&rwLock);
    return mNonstandardAlternativeAerodromesEnabled;
}

void AftnParameters::addMessageProcessorFeatures(int f)
{
    mMessageProcessorFeatures |= f;
}

void AftnParameters::removeMessageProcessorFeatures(int f)
{
    mMessageProcessorFeatures &= (~f);
}

void AftnParameters::setMessageProcessorFeatures(int f)
{
    mMessageProcessorFeatures = f;
}

int AftnParameters::messageProcessorFeatures() const
{
    return mMessageProcessorFeatures;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
