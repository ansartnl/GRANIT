#ifndef RRPMESSAGE2_H
#define RRPMESSAGE2_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RrpStruct2
 * \brief Структура сообщения RRP/РРП
 */

struct RrpStruct2 : AtfFplStruct
{
    QString route;
};

inline QDataStream & operator<< (QDataStream &stream, const RrpStruct2 &msg)
{
    stream << (const AtfFplStruct &)msg;
    stream << msg.route;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RrpStruct2 &msg)
{
    stream >> (AtfFplStruct &)msg;
    stream >> msg.route;
    return stream;
}

/*!
 * \class RrpMessage2
 * \brief Класс сообщения RRP/РРП
 */

class RrpMessage2 : public AtfFplMessage
{
public:
    RrpMessage2();
    RrpMessage2(const RrpStruct2 &s);
    virtual ~RrpMessage2();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    virtual AtsStruct ats() const;

    RrpStruct2 mRrp;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RrpStruct2)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RrpStruct2)
#endif

#endif // RRPMESSAGE2_H
