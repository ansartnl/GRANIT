#ifndef FLAMESSAGE_H
#define FLAMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct FlaStruct
 * \brief Структура сообщения ФЛА
 */

struct FlaStruct
{
    QString acid;
    QString ades;
    QString arrivalAirport;

    QString other;

    QDate dof;

    ParseVersion::Version version;

    FlaStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const FlaStruct &msg)
{
    stream << msg.acid;
    stream << msg.ades;
    stream << msg.arrivalAirport;
    stream << msg.other;
    stream << msg.dof;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FlaStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.ades;
    stream >> msg.arrivalAirport;
    stream >> msg.other;
    stream >> msg.dof;
    return stream;
}

/*!
 * \class FlaMessage
 * \brief Класс FLA сообщения
 */

class FlaMessage : public AtsMessage
{
public:
    FlaMessage();
    FlaMessage(const FlaStruct &s);
    virtual ~FlaMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    FlaStruct mFla;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::FlaStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(FlaStruct)
#endif

#endif // FLAMESSAGE_H
