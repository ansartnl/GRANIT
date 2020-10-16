#ifndef CHTELEGRAM_H
#define CHTELEGRAM_H

#include "Telegram.h"

#include <QtCore/QVariant>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ChStruct
 * \brief Структура ЦХ
 *
 * Данная структура содержит все поля телеграммы ЦХ.
 */

struct ChStruct
{
    QString transmissionIdentificator;  // 3 letters + 3 digits
    QString additionServiceIndication;  // 0-10 characters; hhmm
    QString lr;                         // last received transmission identificator: 3 letters + 3 digits

    ParseVersion::Version version;
};

inline QDataStream & operator<< (QDataStream &stream, const ChStruct &msg)
{
    stream << msg.transmissionIdentificator;
    stream << msg.additionServiceIndication;
    stream << msg.lr;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ChStruct &msg)
{
    stream >> msg.transmissionIdentificator;
    stream >> msg.additionServiceIndication;
    stream >> msg.lr;
    stream >> msg.version;
    return stream;
}

/*!
 * \class ChTelegram
 * \brief Класс телеграммы ЦХ
 */

class ChTelegram : public Telegram
{
public:
    ChTelegram();
    ChTelegram(const ChStruct &s);
    virtual ~ChTelegram();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    const QString aftnChannel() const;
    int telegramNumber() const;

    ChTelegram * replyMessage() const;
    ChStruct reply() const;

    ChStruct mCh;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CHTELEGRAM_H
