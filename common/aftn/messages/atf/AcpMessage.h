#ifndef ACPMESSAGE_H
#define ACPMESSAGE_H

#include "AftnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AcpStruct
 * \brief Структура сообщения ACP
 */

struct AcpStruct
{
    QString acid;
    QString adep;
    QString ades;

    ParseVersion::Version version;

    AcpStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const AcpStruct &msg)
{
    stream << msg.acid;
    stream << msg.adep;
    stream << msg.ades;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AcpStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.adep;
    stream >> msg.ades;
    stream >> msg.version;
    return stream;
}

/*!
 * \class AcpMessage
 * \brief Класс ACP сообщения
 */

class AcpMessage : public AftnMessage
{
public:
    AcpMessage();
    AcpMessage(const AcpStruct &s);
    virtual ~AcpMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    AcpStruct mAcp;

    inline friend QDataStream & operator<< (QDataStream &stream, const AcpMessage &msg)
    {
        stream << (const AftnMessage &)msg;
        stream << msg.mAcp;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, AcpMessage &msg)
    {
        stream >> (AftnMessage &)msg;
        stream >> msg.mAcp;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AcpStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AcpStruct)
#endif

#endif // ACPMESSAGE_H
