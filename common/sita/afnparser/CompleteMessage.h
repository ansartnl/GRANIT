#pragma once

#include <utility>

#include <QString>
#include <QMetaType>

#include "Header.h"


namespace AFN {


struct CompleteMessage {
    Header header;
    QString next_atc_address;
    Reason reason;

    CompleteMessage(const Header& _header = Header(), const QString& _next_atc_address = QString(), Reason _reason = Unknown)
        : header(_header), next_atc_address(_next_atc_address), reason(_reason)
    {}
};

QString buildCompleteMessage(const CompleteMessage& message);
std::pair<CompleteMessage, bool> parseCompleteMessage(const QString& source);


} // namespace AFN


Q_DECLARE_METATYPE(AFN::CompleteMessage)
