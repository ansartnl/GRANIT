#ifndef SVCMESSAGE_H
#define SVCMESSAGE_H

#include "AftnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcStruct
 * \brief Структура SVC/СЖЦ сообщения
 */

struct SvcStruct
{
    QString text;

    ParseVersion::Version version;

    SvcStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const SvcStruct &msg)
{
    stream << msg.text;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, SvcStruct &msg)
{
    stream >> msg.text;
    stream >> msg.version;
    return stream;
}

/*!
 * \class SvcMessage
 * \brief Класс SVC/СЖЦ сообщения
 */

class SvcMessage : public AftnMessage
{
public:
    SvcMessage();
    SvcMessage(const SvcStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCMESSAGE_H
