#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "MessageCreator.h"

#include <QtCore/QHash>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class MessageFactory
{
private:
    MessageFactory();
    ~MessageFactory();

public:
    static MessageFactory * instance();

    const QStringList supportedTypes(int disambiguation) const;

    AftnMessage * create(const QString &type, const QVariantMap &map
                         , int disambiguation = TypeKey::kDefaultDisambiguation) const;

    const TelegramErrorList lastErrors() const;

private:
    void installCreators();

    void installCreator(const PMessageCreator &creator);

private:
    QHash < TypeKey, PMessageCreator > mCreators;
    QHash < int, QSet < QString > > mSupportedTypes;
    mutable TelegramErrorList mErrors;
};

#define messageFactory MessageFactory::instance()

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // MESSAGEFACTORY_H
