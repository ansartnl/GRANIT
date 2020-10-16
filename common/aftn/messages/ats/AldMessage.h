#ifndef ALDMESSAGE_H
#define ALDMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AldStruct
 * \brief Структура сообщения ALD из ТС-2013
 */

struct AldStruct
{
    QString acid;

    int numberOfAircrafts;
    QString aircraftType;
    QString turbulence;

    QString other;

    QDate dof;

    ParseVersion::Version version;

    AldStruct() :
        numberOfAircrafts(0), version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const AldStruct &msg)
{
    stream << msg.acid;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    stream << msg.turbulence;
    stream << msg.other;
    stream << msg.dof;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AldStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    stream >> msg.turbulence;
    stream >> msg.other;
    stream >> msg.dof;
    stream >> msg.version;
    return stream;
}

/*!
 * \class AldMessage
 * \brief Класс ALD сообщения из ТС-2013
 */

class AldMessage : public AtsMessage
{
public:
    AldMessage();
    AldMessage(const AldStruct &s);
    virtual ~AldMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    AldStruct mAld;

    inline friend QDataStream & operator<< (QDataStream &stream, const AldMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mAld;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, AldMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mAld;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AldStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AldStruct)
#endif

#endif // ALDMESSAGE_H
