#ifndef MANMESSAGE_H
#define MANMESSAGE_H

#include "FplMessage.h"

#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ManStruct
 * \brief Структура сообщения MAN
 */

struct ManStruct : FplStruct
{
    QStringList reasons;
};

inline QDataStream & operator<< (QDataStream &stream, const ManStruct &msg)
{
    stream << (const FplStruct &)msg;
    stream << msg.reasons;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ManStruct &msg)
{
    stream >> (FplStruct &)msg;
    stream >> msg.reasons;
    return stream;
}

/*!
 * \class ManMessage
 * \brief Класс MAN сообщения
 */

class ManMessage : public AtsMessage
{
public:
    ManMessage();
    ManMessage(const ManStruct &s);
    virtual ~ManMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    ManStruct mMan;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // MANMESSAGE_H
