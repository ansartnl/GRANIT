#ifndef PPNMESSAGE_H
#define PPNMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct PpnStruct
 * \brief Структура сообщения PPN
 */

struct PpnStruct
{
    QString acid;

    QString other;

    QDate dof;

    ParseVersion::Version version;

    PpnStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const PpnStruct &msg)
{
    stream << msg.acid;
    stream << msg.other;
    stream << msg.dof;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, PpnStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.other;
    stream >> msg.dof;
    stream >> msg.version;
    return stream;
}

/*!
 * \class PpnMessage
 * \brief Класс PPN сообщения
 */

class PpnMessage : public AtsMessage
{
public:
    PpnMessage();
    PpnMessage(const PpnStruct &s);
    virtual ~PpnMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    PpnStruct mPpn;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::PpnStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(PpnStruct)
#endif

#endif // PPNMESSAGE_H
