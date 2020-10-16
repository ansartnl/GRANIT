#ifndef RRPMESSAGE_H
#define RRPMESSAGE_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RrpStruct
 * \brief Структура сообщения RRP/РРП
 */

struct RrpStruct : AtfFplStruct
{
    FplChange changes;
};

inline QDataStream & operator<< (QDataStream &stream, const RrpStruct &msg)
{
    stream << (const AtfFplStruct &)msg;
    stream << msg.changes;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RrpStruct &msg)
{
    stream >> (AtfFplStruct &)msg;
    stream >> msg.changes;
    return stream;
}

/*!
 * \class RrpMessage
 * \brief Класс сообщения RRP/РРП
 */

class RrpMessage : public AtfFplMessage
{
public:
    RrpMessage();
    RrpMessage(const RrpStruct &s);
    virtual ~RrpMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    virtual AtsStruct ats() const;

    RrpStruct mRrp;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RrpStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RrpStruct)
#endif

#endif // RRPMESSAGE_H
