#ifndef SVCQTARPTMESSAGE_H
#define SVCQTARPTMESSAGE_H

#include "SvcMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaRptStruct
 * \brief Структура SVC/СЖЦ сообщения QTA RPT/ЩТА РПТ
 *
 * Версия со строкой отправителя.
 */

struct SvcQtaRptStruct
{
    QDateTime dateTime;
    QString address;

    ParseVersion::Version version;

    SvcQtaRptStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

/*!
 * \class SvcQtaRptMessage
 * \brief Класс SVC/СЖЦ сообщения QTA RPT/ЩТА РПТ
 *
 * Версия со строкой отправителя.
 */

class SvcQtaRptMessage : public AftnMessage
{
public:
    SvcQtaRptMessage();
    SvcQtaRptMessage(const SvcQtaRptStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaRptStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTARPTMESSAGE_H
