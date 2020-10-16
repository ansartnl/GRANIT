#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "ParseResult.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class MessageParser : public QObject
{
    Q_OBJECT
public:
    MessageParser(QObject *parent = 0);

    static void init(); // не обязательная функция

    void setFplStandard(int value);

    static ParseResult parse(const QString &data, int fplStandard, bool _cutFir, bool _validateRoute);
    static ParseResult parse(const QString &data, int fplStandard);
    ParseResult parse(const QString &data);

    static AftnMessage * parseMessage(const QString &data, int fplStandard);
    AftnMessage * parseMessage(const QString &data);

protected:
    AftnMessage * parseMessage_p(const QString &data);
    AftnMessage * parseNonStandardMessage(const QString &data);
    AftnMessage * parseRestrictionMessage(const QString &data);
    AftnMessage * parseMeteoMessage(const QString &data);
    AftnMessage * parseStandardMessage(const QString &data);
    AftnMessage * parseAdexpMessage(const QString &data);

protected:
    int mFplStandard;
    TelegramErrorList errors;
    bool saveOnlyFir;
    bool cutFir;
    bool validateRoute;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // MESSAGEPARSER_H
