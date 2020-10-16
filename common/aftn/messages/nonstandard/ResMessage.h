#ifndef RESMESSAGE_H
#define RESMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ResStruct
 * \brief Структура сообщения RES
 */

struct ResStruct
{
    QDateTime time;

    ResStruct() :
        time(QDateTime::currentDateTimeUtc())
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const ResStruct &msg)
{
    stream << msg.time;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ResStruct &msg)
{
    stream >> msg.time;
    return stream;
}

/*!
 * \class ResMessage
 * \brief Класс RES сообщения
 */

class ResMessage : public AftnMessage
{
public:
    ResMessage();
    ResMessage(const ResStruct &s);
    virtual ~ResMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    ResStruct mRes;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RESMESSAGE_H
