#ifndef PLNMESSAGE_H
#define PLNMESSAGE_H

#include "FplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct PlnStruct
 * \brief Структура сообщения PLN
 */

struct PlnStruct : FplStruct
{
    PlnStruct(const FplStruct &s) :
        FplStruct(s)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const PlnStruct &msg)
{
    stream << (const FplStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, PlnStruct &msg)
{
    stream >> (FplStruct &)msg;
    return stream;
}

/*!
 * \class PlnMessage
 * \brief Класс PLN сообщения
 */

class PlnMessage : public FplMessage
{
public:
    PlnMessage();
    PlnMessage(const PlnStruct &s);
    virtual ~PlnMessage();

    void setVersion(ParseVersion::Version v);
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PLNMESSAGE_H
