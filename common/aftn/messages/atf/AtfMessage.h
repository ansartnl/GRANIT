#ifndef ATFMESSAGE_H
#define ATFMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDate>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AtfStruct
 * \brief Структура сообщений ОПВД
 *
 * Данная структура содержит общие поля телеграмм SPS/СПС, AIM, ANM/АНМ, SCN/СЦН, RRP/РРП,
 * SAM/САМ, SLC/СЛЦ, SLT/СЛТ, CSL.
 */

struct AtfStruct
{
    QString type;
    QString text;
    QDate dof;

    ParseVersion::Version version;
};

inline QDataStream & operator<< (QDataStream &stream, const AtfStruct &msg)
{
    stream << msg.type;
    stream << msg.text;
    stream << msg.dof;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtfStruct &msg)
{
    stream >> msg.type;
    stream >> msg.text;
    stream >> msg.dof;
    stream >> msg.version;
    return stream;
}

/*!
 * \class AtfMessage
 * \brief Класс ОПВД сообщения
 */

class AtfMessage : public AftnMessage
{
public:
    AtfMessage();
    AtfMessage(const AtfStruct &s);
    virtual ~AtfMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    virtual AtfStruct & atf();
    virtual const AtfStruct & atfConst() const;

    AtfStruct mAtf;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ATFMESSAGE_H
