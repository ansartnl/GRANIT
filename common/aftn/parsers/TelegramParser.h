#ifndef TELEGRAMPARSER_H
#define TELEGRAMPARSER_H

#include "ParseErrors.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class Telegram;
class ChTelegram;
class AftnTelegram;

struct TextWithBeginningAndEnding
{
    QString beginning;
    QStringList text;
    QString ending;

    TextWithBeginningAndEnding()
    {}
    TextWithBeginningAndEnding(const QString &b, const QStringList &t, const QString &e) :
        beginning(b), text(t), ending(e)
    {}
};

class TelegramParser : public QObject
{
    Q_OBJECT
public:
    TelegramParser(QObject *parent = 0);

    static Telegram * parse(const QString &data, ParserErrors::AftnError &error);
    Telegram * parse(const QString &data);

    static const QString message(const QString &data);

    ParserErrors::AftnError lastError() const;

    static QStringList splitText(const QString &text);
    static const TextWithBeginningAndEnding splitText2(const QString &text);

protected:
    ChTelegram * tryParseCh(const QString &data);
    AftnTelegram * parseAftn(const QString &data);

protected:
    ParserErrors::AftnError mError;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TELEGRAMPARSER_H
