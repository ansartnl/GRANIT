#ifndef SVCQTARPTMESSAGE2_H
#define SVCQTARPTMESSAGE2_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaRptStruct
 * \brief Структура SVC/СЖЦ сообщения QTA RPT/ЩТА РПТ
 *
 * Версия со списком номеров телеграмм.
 */

struct SvcQtaRptStruct2
{
    QString aftnChannel;
    int numFrom;
    int numTill;

    ParseVersion::Version version;

    SvcQtaRptStruct2() :
        numFrom(0), numTill(0), version(ParseVersion::PV_UNKNOWN)
    {}
};

/*!
 * \class SvcQtaRptMessage
 * \brief Класс SVC/СЖЦ сообщения QTA RPT/ЩТА РПТ
 *
 * Версия со списком номеров телеграмм.
 */

class SvcQtaRptMessage2 : public AftnMessage
{
public:
    SvcQtaRptMessage2();
    SvcQtaRptMessage2(const SvcQtaRptStruct2 &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaRptStruct2 mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTARPTMESSAGE2_H
