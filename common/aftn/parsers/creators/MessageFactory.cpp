#include "MessageFactory.h"

#include "AftnParameters.h"
#include "AllCreators.h"
#include "SupportedTypes.h"

#define INSTALL_CREATOR(T) installCreator(PMessageCreator(new T))

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

MessageFactory::MessageFactory()
{
    installCreators();
    Q_ASSERT_X(mCreators.contains(SupportedTypes::defaultType()), "MessageFactory"
               , "No MessageCreator is connected to default message type");
}

MessageFactory::~MessageFactory()
{
}

MessageFactory * MessageFactory::instance()
{
    static MessageFactory *mFactory = new MessageFactory();
    return mFactory;
}

const QStringList MessageFactory::supportedTypes(int disambiguation) const
{
    return mSupportedTypes[disambiguation].toList();
}

AftnMessage * MessageFactory::create(const QString &type, const QVariantMap &map, int disambiguation) const
{
    PMessageCreator creator = mCreators[TypeKey(type, disambiguation)];
    if (creator.isNull())
        creator = mCreators[TypeKey(SupportedTypes::defaultType())];
    AftnMessage *ret = creator->create(type, map);
    mErrors = creator->lastErrors();
    return ret;
}

const TelegramErrorList MessageFactory::lastErrors() const
{
    return mErrors;
}

void MessageFactory::installCreators()
{
    INSTALL_CREATOR(AcpCreator);
    INSTALL_CREATOR(AimCreator);
    INSTALL_CREATOR(AnmCreator);
    INSTALL_CREATOR(CslCreator);
    INSTALL_CREATOR(RrpCreator);
    INSTALL_CREATOR(RrpCreator2);
    INSTALL_CREATOR(SamCreator);
    INSTALL_CREATOR(ScnCreator);
    INSTALL_CREATOR(SlcCreator);
    INSTALL_CREATOR(SltCreator);
    INSTALL_CREATOR(SpsCreator);

    INSTALL_CREATOR(AlrCreatorShort);
    INSTALL_CREATOR(AlrCreator);
    INSTALL_CREATOR(AnpCreator);
    INSTALL_CREATOR(ApwCreator);
    INSTALL_CREATOR(ApzCreator);
    INSTALL_CREATOR(RviCreator);
    INSTALL_CREATOR(RvmCreator);

    INSTALL_CREATOR(AldCreator);
    INSTALL_CREATOR(ArrCreator);
    INSTALL_CREATOR(ChgCreator);
    INSTALL_CREATOR(CnlCreator);
    INSTALL_CREATOR(CnlRplCreator);
    INSTALL_CREATOR(DepCreator);
    INSTALL_CREATOR(DlaCreator);
    INSTALL_CREATOR(FlaCreator);
    INSTALL_CREATOR(FplCreator);
    INSTALL_CREATOR(PlnCreator);
    INSTALL_CREATOR(PlrCreator);
    INSTALL_CREATOR(PpnCreator);
    INSTALL_CREATOR(RcfCreator);
    INSTALL_CREATOR(RplCreator);
    INSTALL_CREATOR(RqpCreator);
    INSTALL_CREATOR(RqsCreator);
    INSTALL_CREATOR(SplCreator);
    INSTALL_CREATOR(UlsCreator);

    INSTALL_CREATOR(MeteoCreator);

    INSTALL_CREATOR(AckCreator);
    INSTALL_CREATOR(GplCreator);
    INSTALL_CREATOR(ManCreator);
    INSTALL_CREATOR(NonStandardRviCreator);
    INSTALL_CREATOR(NonStandardRvmCreator);
    INSTALL_CREATOR(RejCreator);
    INSTALL_CREATOR(ResCreator);
    INSTALL_CREATOR(RqnCreator);
    INSTALL_CREATOR(RtcCreator);
    INSTALL_CREATOR(TplCreator);
    INSTALL_CREATOR(UnknownCreator);
    INSTALL_CREATOR(UtpCreator);

    INSTALL_CREATOR(NotamCreator);
}

void MessageFactory::installCreator(const PMessageCreator &creator)
{
    QStringList types = creator->types();
    foreach (const QString &type, types)
    {
        if (aftnParameters->supportedTypes()->isSupported(type))
        {
            QList < TypeKey > keys = creator->keys(type);
            foreach (const TypeKey &key, keys)
            {
                if (!aftnParameters->supportedTypes()->isDisabled(key.type, key.disambiguation))
                {
                    mCreators.insert(key, creator);
                    if (!mSupportedTypes.contains(key.disambiguation))
                        mSupportedTypes.insert(key.disambiguation, QSet < QString >());
                    mSupportedTypes[key.disambiguation] << key.type;
                }
            }
        }
    }
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
