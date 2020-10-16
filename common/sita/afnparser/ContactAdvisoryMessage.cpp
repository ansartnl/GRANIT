#include "ContactAdvisoryMessage.h"

#include <QRegExp>

namespace AFN {


struct RegisterContactAdvisoryMessage {RegisterContactAdvisoryMessage() {qRegisterMetaType<AFN::ContactAdvisoryMessage>();}};
static RegisterContactAdvisoryMessage registerContactAdvisoryMessage;


QString buildContactAdvisoryMessage(const ContactAdvisoryMessage& message) {
    return (buildHeader(message.header) + "/FCA" + message.next_atc_center_address + "," +
            QString::number(message.pass_active_status));
}

std::pair<ContactAdvisoryMessage, bool> parseContactAdvisoryMessage(const QString &source) {
    int headerSize = source.indexOf('/');
    if (headerSize == -1)
        return std::make_pair(ContactAdvisoryMessage(), false);

    auto header = parseHeader(source.left(headerSize));
    if (!header.second)
        return std::make_pair(ContactAdvisoryMessage(), false);

    ContactAdvisoryMessage result(header.first);

    QRegExp regExp("FCA([A-Z0-9]{7}),([0|1])");

    if (!regExp.exactMatch(source.mid(headerSize + 1)))
        return std::make_pair(ContactAdvisoryMessage(), false);

    result.next_atc_center_address = regExp.cap(1);
    result.pass_active_status = regExp.cap(2).toInt();
    return std::make_pair(result, true);
}


} // namespace AFN
