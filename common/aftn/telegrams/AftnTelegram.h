#ifndef AFTNTELEGRAM_H
#define AFTNTELEGRAM_H

#include "Telegram.h"

#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AftnStruct
 * \brief Структура АФТН телеграммы
 *
 * Данная структура содержит все поля АФТН телеграммы.
 */

struct AftnStruct
{
    QString transmissionIdentificator;  // 3 letters + 3 digits
    QString additionServiceIndication;  // 0-10 digits
    QString priorityIndicator;          // 2 letters: KK FF GG DD SS
    QStringList receiverAddresses;      // 8 letters each
    // filling time: 6 digits: ddhhmm
    int fillingDate;
    int fillingHour;
    int fillingMinute;
    QString senderAddress;              // 8 letters

    // beggining of the text
    QStringList text; // pack of messages, or any message/text in text[0]

    QString confirmation;               // text
    QString correction;                 // text

    QString textBeforeMessage;
    QString textAfterMessage;

    ParseVersion::Version version;

    enum Part {
        NOT_PART
        , PART
        , LAST_PART
    } part;
    int partNumber;

    AftnStruct() :
        fillingDate(0), fillingHour(0), fillingMinute(0)
      , version(ParseVersion::PV_UNKNOWN)
      , part(NOT_PART), partNumber(-1)
    {}

    const QString fillingTime() const;
};
QT_DECLARE_ENUM_STREAMING(AftnStruct::Part)

inline QDataStream & operator<< (QDataStream &stream, const AftnStruct &msg)
{
    stream << msg.transmissionIdentificator;
    stream << msg.additionServiceIndication;
    stream << msg.priorityIndicator;
    stream << msg.receiverAddresses;
    stream << msg.fillingDate;
    stream << msg.fillingHour;
    stream << msg.fillingMinute;
    stream << msg.senderAddress;
    stream << msg.text;
    stream << msg.confirmation;
    stream << msg.correction;
    stream << msg.version;
    stream << msg.part;
    stream << msg.partNumber;
    stream << msg.textBeforeMessage;
    stream << msg.textAfterMessage;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AftnStruct &msg)
{
    stream >> msg.transmissionIdentificator;
    stream >> msg.additionServiceIndication;
    stream >> msg.priorityIndicator;
    stream >> msg.receiverAddresses;
    stream >> msg.fillingDate;
    stream >> msg.fillingHour;
    stream >> msg.fillingMinute;
    stream >> msg.senderAddress;
    stream >> msg.text;
    stream >> msg.confirmation;
    stream >> msg.correction;
    stream >> msg.version;
    stream >> msg.part;
    stream >> msg.partNumber;
    stream >> msg.textBeforeMessage;
    stream >> msg.textAfterMessage;
    return stream;
}

/*!
 * \class AftnTelegram
 * \brief Класс АФТН телеграммы
 */

class AftnTelegram : public Telegram
{
public:
    AftnTelegram();
    AftnTelegram(const AftnStruct &s);
    virtual ~AftnTelegram();

    QString data() const;
    QString header() const;
    QString body() const;
    QString footer() const;
    QString textBeforeMessage() const;
    QString textAfterMessage() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    const QString aftnChannel() const;
    int telegramNumber() const;

    AftnStruct mAftn;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AftnStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AftnStruct)
#endif

#endif // AFTNTELEGRAM_H
