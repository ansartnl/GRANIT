#ifndef ULSMESSAGE_H
#define ULSMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct UlsStruct
 * \brief Структура сообщения ULS
 */

struct UlsStruct : public AtsStruct
{
    QString other;

    QString letter;
    QString dep;
    QString dest;

#ifdef PATCH1
    bool hasField9;
    // 9
    int numberOfAircrafts;
    QString aircraftType;
    QString turbulence;
#endif

    inline QString generateOther() const
    {
        QString ret;
        if (dof.isValid())
            ret = Transliteration::toVersion(QString("DOF/%1").arg(dof.toString("yyMMdd")), version);
        else
            ret = Transliteration::toVersion(QString("DOF/%1")
                                             .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                             , version);
        if (!letter.isEmpty())
            ret += Transliteration::toVersion(QString(" СТС/%2").arg(letter), version);
        if (!dep.isEmpty())
            ret += Transliteration::toVersion(QString(" DEP/%2").arg(dep), version);
        if (!dest.isEmpty())
            ret += Transliteration::toVersion(QString(" DEST/%2").arg(dep), version);
        return ret;
    }
};

inline QDataStream & operator<< (QDataStream &stream, const UlsStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.other;
    stream << msg.letter;
    stream << msg.dep;
    stream << msg.dest;
#ifdef PATCH1
    stream << msg.hasField9;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    stream << msg.turbulence;
#endif
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, UlsStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.other;
    stream >> msg.letter;
    stream >> msg.dep;
    stream >> msg.dest;
#ifdef PATCH1
    stream >> msg.hasField9;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    stream >> msg.turbulence;
#endif
    return stream;
}

/*!
 * \class UlsMessage
 * \brief Класс ULS сообщения
 */

class UlsMessage : public AtsMessage
{
public:
    UlsMessage();
    UlsMessage(const UlsStruct &s);
    virtual ~UlsMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    UlsStruct mUls;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::UlsStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(UlsStruct)
#endif

#endif // ULSMESSAGE_H
