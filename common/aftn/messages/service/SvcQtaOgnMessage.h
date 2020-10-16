#ifndef SVCQTAOGNMESSAGE_H
#define SVCQTAOGNMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaOgnStruct
 * \brief Структура SVC/СЖЦ сообщения QTA OGN/ЩТА ОГН
 */

struct SvcQtaOgnStruct
{
    QString transmissionIdentificator;

    ParseVersion::Version version;

    SvcQtaOgnStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}

    inline const QString aftnChannel() const
    {
        return transmissionIdentificator.left(3);
    }
    inline int telegramNumber() const
    {
        return transmissionIdentificator.right(3).toInt();
    }
};

/*!
 * \class SvcQtaOgnMessage
 * \brief Класс SVC/СЖЦ сообщения QTA OGN/ЩТА ОГН
 */

class SvcQtaOgnMessage : public AftnMessage
{
public:
    SvcQtaOgnMessage();
    SvcQtaOgnMessage(const SvcQtaOgnStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaOgnStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTAOGNMESSAGE_H
