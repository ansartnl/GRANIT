#ifndef ANMMESSAGE_H
#define ANMMESSAGE_H

#include "AtfMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AnmStruct
 * \brief Структура сообщения ANM
 */

struct AnmStruct : AtfStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const AnmStruct &msg)
{
    stream << (const AtfStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AnmStruct &msg)
{
    stream >> (AtfStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class AnmMessage
 * \brief Класс сообщения ANM
 */

class AnmMessage : public AtfMessage
{
public:
    AnmMessage();
    AnmMessage(const AnmStruct &s);
    virtual ~AnmMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    AnmStruct mAnm;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AnmStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AnmStruct)
#endif

#endif // ANMMESSAGE_H
