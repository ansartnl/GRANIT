#ifndef METEOMESSAGE_H
#define METEOMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct MeteoStruct
 * \brief Структура метео сообщений
 *
 * Данная структура содержит общую информацию для метео сообщений.
 */

struct MeteoStruct
{
    QString wmoDataType;
    QString type;
    QDateTime arrivalTime;
    QString zone;
    QString text;

    QDateTime validFrom;
    QDateTime validTill;
    QString number;
    bool isCnl;

    MeteoStruct() :
        isCnl(false)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const MeteoStruct &msg)
{
    stream << msg.wmoDataType;
    stream << msg.type;
    stream << msg.arrivalTime;
    stream << msg.zone;
    stream << msg.text;
    stream << msg.validFrom;
    stream << msg.validTill;
    stream << msg.number;
    stream << msg.isCnl;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, MeteoStruct &msg)
{
    stream >> msg.wmoDataType;
    stream >> msg.type;
    stream >> msg.arrivalTime;
    stream >> msg.zone;
    stream >> msg.text;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    stream >> msg.number;
    stream >> msg.isCnl;
    return stream;
}

/*!
 * \class MeteoMessage
 * \brief Класс метео сообщений
 *
 * Класс метео сообщений:
 * - METAR
 * - SPECI
 * - TAF
 * - SIGMET
 * - AIRMET
 * - GAMET
 */

class MeteoMessage : public AftnMessage
{
public:
    MeteoMessage();
    MeteoMessage(const MeteoStruct &s);
    virtual ~MeteoMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version);

    MeteoStruct mMeteo;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // METEOMESSAGE_H
