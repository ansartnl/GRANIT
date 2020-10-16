#ifndef GPLMESSAGE_H
#define GPLMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDate>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct GplStruct
 * \brief Структура сообщения GPL
 */

struct GplStruct
{
    QDate start;
    QDate finish;
};

inline QDataStream & operator<< (QDataStream &stream, const GplStruct &msg)
{
    stream << msg.start;
    stream << msg.finish;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, GplStruct &msg)
{
    stream >> msg.start;
    stream >> msg.finish;
    return stream;
}

/*!
 * \class GplMessage
 * \brief Класс GPL сообщения
 */

class GplMessage : public AftnMessage
{
public:
    GplMessage();
    GplMessage(const GplStruct &s);
    virtual ~GplMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    GplStruct mGpl;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // GPLMESSAGE_H
