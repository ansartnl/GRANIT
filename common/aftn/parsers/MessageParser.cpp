#include "MessageParser.h"

#include "AdexpParser.h"
#include "AftnParameters.h"
#include "AtsNonStandardMessagesParser.h"
#include "MessageFactory.h"
#include "MeteoParser.h"
#include "RestrictionParser.h"
#include "SupportedTypes.h"
#include "SvcParser.h"
#include "TelegramParser.h"
#include "TelegramTypes.h"

#include "RejMessage.h"
#include "SvcMessage.h"
#include "UnknownMessage.h"

#include "aftnmessageprocessor.h"
#include <QtDebug>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

TelegramErrorList standardErrors(const AFTNMessageProcessor::AftnMessageErrors &errorCodes);
TelegramErrorList nonStandardErrors(
        const AtsNonStandardMessagesParser::NonStandardMessageErrors &errorCodes);
TelegramErrorList restrictionErrors(
        const RestrictionParser::RestrictionErrors &errorCodes);
TelegramErrorList meteoErrors(const MeteoParser::MeteoErrors &errorCodes);

MessageParser::MessageParser(QObject *parent) :
    QObject(parent), mFplStandard(0)
{
}

void MessageParser::init()
{
    messageFactory;
}

void MessageParser::setFplStandard(int value)
{
    mFplStandard = value;
}

ParseResult MessageParser::parse(const QString &data, int fplStandard, bool _cutFir, bool _validateRoute)
{
    MessageParser parser;
    parser.setFplStandard(fplStandard);
    parser.cutFir = _cutFir;
    parser.validateRoute = _validateRoute;
    ParseResult ret = parser.parse(data);

    return ret;
}

ParseResult MessageParser::parse(const QString &data, int fplStandard)
{
    MessageParser parser;
    parser.setFplStandard(fplStandard);
    ParseResult ret = parser.parse(data);

    return ret;
}

ParseResult MessageParser::parse(const QString &data)
{
    ParseResult ret;
    ret.arrivalTime = QDateTime::currentDateTimeUtc();
    ret.hasErrors = false;

    TelegramParser p(this);

    ret.telegram = PTelegram(p.parse(data));

    if (TelegramType::isCh(ret.telegram->type()))
        return ret;

    if (p.lastError() != ParserErrors::AftnNoError)
    {
        UnknownMessage *u = new UnknownMessage(data);
        u->addError(TelegramError(TelegramError::AFTN, p.lastError()));
        ret.hasErrors = true;
        ret.messageList << PMessage(u);
        return ret;
    }

    AftnTelegram *t = static_cast < AftnTelegram * > (ret.telegram.data());

    if (t->mAftn.part == AftnStruct::NOT_PART)
    {
        foreach (const QString &m, t->mAftn.text)
        {
            AftnMessage *mes = parseMessage(m);
            if (mes->hasErrors())
                ret.hasErrors = true;
            ret.messageList << PMessage(mes);
        }

        AftnTelegram *at =ret.aftn();
        foreach (PMessage pm, ret.messageList)
        {
            at->mAftn.text.clear();
            at->mAftn.text.append(pm->data());

        }

        if (ret.messageList.count() > 1)
            t->mType = TelegramType::Pack;
        else if (ret.messageList.count() == 1)
            t->mType = ret.messageList[0]->type();
    } else
    {
        t->mType = TelegramType::Part;
    }


    return ret;
}

AftnMessage * MessageParser::parseMessage(const QString &data, int fplStandard)
{
    MessageParser parser;
    parser.setFplStandard(fplStandard);

    AftnMessage * ret = parser.parseMessage(data);
    return ret;
}

AftnMessage * MessageParser::parseMessage(const QString &data)
{
    AftnMessage *mes = parseMessage_p(data);
    mes->addErrors(errors);
    errors.clear();
    return mes;
}

AftnMessage * MessageParser::parseMessage_p(const QString &data)
{
    if (SvcParser::isSvc(data)) {
        SvcStruct s;
        s.text = data;
        s.version = ParseVersion::textVersion(data);
        return new SvcMessage(s);
    }
    AftnMessage *ret = parseNonStandardMessage(data);
    if (!ret)
        ret = parseRestrictionMessage(data);
    if (!ret)
        ret = parseMeteoMessage(data);
    if (!ret)
        ret = parseAdexpMessage(data);
    if (!ret)
        ret = parseStandardMessage(data);
    if ((ret->type() == SupportedTypes::defaultType())
            && !errors.contains(TelegramError(TelegramError::STANDARD, AFTNMessageProcessor::InvalidMessage)))
        errors << TelegramError(TelegramError::STANDARD, AFTNMessageProcessor::InvalidMessage);
    return ret;
}

AftnMessage * MessageParser::parseNonStandardMessage(const QString &data)
{
    AtsNonStandardMessagesParser::NonStandardMessageErrors errorCodes;
    QVariantMap messageMap = AtsNonStandardMessagesParser::processMessage(data, errorCodes);

    if (errorCodes.contains(ParserErrors::UnknownMessage))
        return 0;
    if (!messageMap.contains(MESSAGE_TYPE))
        return 0;

    AftnMessage *ret;

    QString type = messageMap.value(MESSAGE_TYPE).toString();

    if (TelegramType::isGpl(type) || TelegramType::isRtc(type) || TelegramType::isRes(type))
    {
        ret = messageFactory->create(type, messageMap);
    } else
    {
        return 0;
    }
    if (aftnParameters->supportedTypes()->isSupported(type))
        errors << nonStandardErrors(errorCodes);

    return ret;
}

AftnMessage * MessageParser::parseRestrictionMessage(const QString &data)
{
    RestrictionParser::RestrictionErrors errorCodes;
    QVariantMap messageMap = RestrictionParser::processMessage(data, errorCodes);

    if (errorCodes.contains(ParserErrors::UnknownMessageRestriction))
        return 0;
    if (!messageMap.contains(TYPE_MESSAGE))
        return 0;

    AftnMessage *ret;

    QString type = messageMap.value(TYPE_MESSAGE).toString();

    if (TelegramType::isRvm(type)
            || TelegramType::isRvi(type))
    {
        ret = messageFactory->create(type, messageMap, TypeKey::kNonstandardDisambiguation);
    } else
    {
        return 0;
    }
    if (aftnParameters->supportedTypes()->isSupported(type))
        errors << restrictionErrors(errorCodes);

    return ret;
}

AftnMessage * MessageParser::parseMeteoMessage(const QString &data)
{
    MeteoParser mParser;

    QVariantMap messageMap = mParser.parse(data);
    MeteoParser::MeteoErrors errorCodes = mParser.errors();
    if (errorCodes.contains(ParserErrors::NotMeteo))
        return 0;

    QString type = messageMap.value(METEO_MESSAGE_TYPE).toString();
    if (type.isEmpty())
        type = TelegramType::Unknown;

    if (aftnParameters->supportedTypes()->isSupported(type))
        errors << meteoErrors(errorCodes);

    return messageFactory->create(type, messageMap);
}

AftnMessage * MessageParser::parseStandardMessage(const QString &data)
{
    AFTNMessageProcessor mProcessor;

    //mProcessor.setValidateRoute(false);
    mProcessor.setValidateRoute(validateRoute);
    //mProcessor.setSaveOnlyFIR(false);
    mProcessor.setSaveOnlyFIR(cutFir);
    //mProcessor.setFIRName(QString());
    mProcessor.setStandard(AFTNMessageProcessor::Standard(mFplStandard));
    mProcessor.setFeatures(AFTNMessageProcessor::Features(aftnParameters->messageProcessorFeatures()));
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    QVariantMap messageMap = mProcessor.processMessage(data, errorCodes);

    AftnMessage *ret;

    if (!messageMap.contains(AFTN_MESSAGE_TYPE))
    {
        errors << standardErrors(errorCodes);
        return new UnknownMessage(data);
    }

    QString type = messageMap.value(AFTN_MESSAGE_TYPE).toString();

    if (TelegramType::isAlr(type))
    {
        if (messageMap.contains(AFTN_MESSAGE_ADDITIONAL_INFO)
                || messageMap.contains(AFTN_MESSAGE_RESCUE_INFO))
        {
            if (aftnParameters->supportedTypes()->isDisabled(type, TypeKey::kDefaultDisambiguation))
                ret = messageFactory->create(type, messageMap, TypeKey::kNonstandardDisambiguation);
            else
                ret = messageFactory->create(type, messageMap);
        } else
        {
            if (aftnParameters->supportedTypes()->isDisabled(type, TypeKey::kNonstandardDisambiguation))
                ret = messageFactory->create(type, messageMap);
            else
                ret = messageFactory->create(type, messageMap, TypeKey::kNonstandardDisambiguation);
        }
    } else if (TelegramType::isRrp(type))
    {
        if (messageMap.contains(AFTN_MESSAGE_ROUTE))
        {
            if (aftnParameters->supportedTypes()->isDisabled(type, TypeKey::kDefaultDisambiguation))
                ret = messageFactory->create(type, messageMap, TypeKey::kNonstandardDisambiguation);
            else
                ret = messageFactory->create(type, messageMap);
        } else
        {
            if (aftnParameters->supportedTypes()->isDisabled(type, TypeKey::kNonstandardDisambiguation))
                ret = messageFactory->create(type, messageMap);
            else
                ret = messageFactory->create(type, messageMap, TypeKey::kNonstandardDisambiguation);
        }

        errors << messageFactory->lastErrors();
    } else if (messageFactory->supportedTypes(TypeKey::kDefaultDisambiguation).contains(type))
    {
        ret = messageFactory->create(type, messageMap);

        errors << messageFactory->lastErrors();
        if (TelegramType::isRej(ret->type()))
        {
            int rejReasonsStart = data.lastIndexOf(')') + 1;
            static_cast < RejMessage * >(ret)->mRej.reasons
                    = data.mid(rejReasonsStart).split('\n', QString::SkipEmptyParts);
        }
    } else
    {
        ret = new UnknownMessage;
    }
    if (aftnParameters->supportedTypes()->isSupported(type))
        errors << standardErrors(errorCodes);

    return ret;
}

AftnMessage * MessageParser::parseAdexpMessage(const QString &data)
{
    AdexpParser mProcessor;
    mProcessor.setValidateRoute(false);
    mProcessor.setSaveOnlyFIR(false);
    mProcessor.setFIRName(QString());
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    QVariantMap messageMap = mProcessor.processMessage(data, errorCodes);

    AftnMessage *ret = 0;

    if (!messageMap.contains(AFTN_MESSAGE_TYPE))
    {
        //errors << standardErrors(errorCodes);
        return 0;//new UnknownMessage(data);
    }

    QString type = messageMap.value(AFTN_MESSAGE_TYPE).toString();

    if (messageFactory->supportedTypes(TypeKey::kAdexpDisambiguation).contains(type))
    {
        ret = messageFactory->create(type, messageMap);
        errors << messageFactory->lastErrors();
    }
    if (aftnParameters->supportedTypes()->isSupported(type))
        errors << standardErrors(errorCodes);

    return ret;
}

TelegramErrorList standardErrors(const AFTNMessageProcessor::AftnMessageErrors &errorCodes)
{
    TelegramErrorList ret;
    foreach (const AFTNErrorInfo err, errorCodes)
    {
        TelegramError ae;
        ae.type = TelegramError::STANDARD;
        ae.code = err.first;
        ae.elementError = err.second;
        ret.append(ae);
    }

    return ret;
}

TelegramErrorList nonStandardErrors(
        const AtsNonStandardMessagesParser::NonStandardMessageErrors &errorCodes)
{
    TelegramErrorList ret;
    foreach (const ParserErrors::NonStandardMessageError err, errorCodes)
    {
        TelegramError ae;
        ae.type = TelegramError::EXTENDED;
        ae.code = err;
        ret.append(ae);
    }
    return ret;
}

TelegramErrorList restrictionErrors(
        const RestrictionParser::RestrictionErrors &errorCodes)
{
    TelegramErrorList ret;
    foreach (const ParserErrors::RestrictionError err, errorCodes)
    {
        TelegramError ae;
        ae.type = TelegramError::RESTRICTION;
        ae.code = err;
        ret.append(ae);
    }
    return ret;
}

TelegramErrorList meteoErrors(const MeteoParser::MeteoErrors &errorCodes)
{
    TelegramErrorList ret;
    foreach (const ParserErrors::MeteoError err, errorCodes)
    {
        TelegramError ae;
        ae.type = TelegramError::METEO;
        ae.code = err;
        ret.append(ae);
    }
    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
