#ifndef SCNMESSAGES_H
#define SCNMESSAGES_H

#include "AtfMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ScnStruct
 * \brief Структура сообщений SCN/СЦН
 */

struct ScnStruct : AtfStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const ScnStruct &msg)
{
    stream << (const AtfStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ScnStruct &msg)
{
    stream >> (AtfStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class ScnMessage
 * \brief Класс сообщения SCN/СЦН
 */

class ScnMessage : public AtfMessage
{
public:
    ScnMessage();
    ScnMessage(const ScnStruct &s);
    virtual ~ScnMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    ScnStruct mScn;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::ScnStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(ScnStruct)
#endif

#endif // SCNMESSAGES_H
