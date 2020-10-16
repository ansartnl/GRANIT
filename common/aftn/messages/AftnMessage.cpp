#include "AftnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AftnMessage::AftnMessage()
{
    setVersion(ParseVersion::PV_UNKNOWN);
}

AftnMessage::~AftnMessage()
{
}

QString AftnMessage::adexp() const
{
    return QString("Not supported for this type");
}

QString AftnMessage::type() const
{
    return mType;
}

void AftnMessage::setVersion(ParseVersion::Version v)
{
    mType = Transliteration::toVersion(mType, v);
}

const TelegramErrorList AftnMessage::errors() const
{
    return mErrors;
}

const TelegramErrorSet AftnMessage::uniqueErrors() const
{
    return TelegramErrorSet(mErrors.toSet());
}

void AftnMessage::addError(const TelegramError &err)
{
    mErrors << err;
}

void AftnMessage::addErrors(const TelegramErrorList &err)
{
    mErrors << err;
}

void AftnMessage::addErrors(const TelegramErrorSet &err)
{
    foreach (const TelegramError &e, err)
        mErrors << e;
}

void AftnMessage::setErrors(const TelegramErrorList &err)
{
    mErrors = err;
}

void AftnMessage::clearErrors()
{
    mErrors.clear();
}

bool AftnMessage::hasErrors() const
{
    return !mErrors.isEmpty();
}

bool AftnMessage::noErrors() const
{
    return mErrors.isEmpty();
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
