#ifndef SVCQTAMISMESSAGE_H
#define SVCQTAMISMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaMisStruct
 * \brief Структура SVC/СЖЦ сообщения QTA MIS/ЩТА МИС
 */

struct SvcQtaMisStruct
{
    QString aftnChannel;
    int numFrom;
    int numTill;

    ParseVersion::Version version;

    SvcQtaMisStruct() :
        numFrom(0), numTill(0), version(ParseVersion::PV_UNKNOWN)
    {}
};

/*!
 * \class SvcQtaMisMessage
 * \brief Класс SVC/СЖЦ сообщения QTA MIS/ЩТА МИС
 */

class SvcQtaMisMessage : public AftnMessage
{
public:
    SvcQtaMisMessage();
    SvcQtaMisMessage(const SvcQtaMisStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaMisStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTAMISMESSAGE_H
