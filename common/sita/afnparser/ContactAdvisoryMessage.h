#pragma once

#include <QString>
#include <QList>
#include <QMetaType>

#include "Header.h"


namespace AFN {


struct ContactAdvisoryMessage {
    Header header;
    QString next_atc_center_address;
    bool pass_active_status;

    ContactAdvisoryMessage(const Header& _header = Header(), const QString& _next_atc_center_address = QString(),
                           bool _pass_active_status = false)
        : header(_header), next_atc_center_address(_next_atc_center_address), pass_active_status(_pass_active_status)
    {}
};


QString buildContactAdvisoryMessage(const ContactAdvisoryMessage& message);
std::pair<ContactAdvisoryMessage, bool> parseContactAdvisoryMessage(const QString& source);


} // namespace AFN


Q_DECLARE_METATYPE(AFN::ContactAdvisoryMessage)
