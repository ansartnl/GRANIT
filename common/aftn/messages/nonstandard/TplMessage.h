#ifndef TPLMESSAGE_H
#define TPLMESSAGE_H

#include "FplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct TplStruct
 * \brief Структура сообщения TPL
 */

struct TplStruct : FplStruct
{
    TplStruct(const FplStruct &s) :
        FplStruct(s)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const TplStruct &msg)
{
    stream << (const FplStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, TplStruct &msg)
{
    stream >> (FplStruct &)msg;
    return stream;
}

/*!
 * \class TplMessage
 * \brief Класс TPL сообщения
 */

class TplMessage : public FplMessage
{
public:
    TplMessage();
    TplMessage(const TplStruct &s);
    virtual ~TplMessage();

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TPLMESSAGE_H
