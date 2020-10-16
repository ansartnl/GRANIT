#include "CompleteMessage.h"


namespace AFN {


struct RegisterCompleteMessage {RegisterCompleteMessage() {qRegisterMetaType<AFN::CompleteMessage>();}};
static RegisterCompleteMessage registerCompleteMessage;

QString buildCompleteMessage(const CompleteMessage& message) {
    return buildHeader(message.header) + "/FCP" + message.next_atc_address + "," + QString::number(message.reason);
}

std::pair<CompleteMessage, bool> parseCompleteMessage(const QString& source) {
    int headerSize = source.indexOf('/');
    if (headerSize == -1)
        return std::make_pair(CompleteMessage(), false);

    auto parsed_header = parseHeader(source.left(headerSize));
    if (!parsed_header.second)
        return std::make_pair(CompleteMessage(), false);

    CompleteMessage message(parsed_header.first);

    QString body(source.mid(headerSize + 1));
    int comma_pos = body.indexOf(',');
    if (body.left(3) != "FCP" || comma_pos == -1 || comma_pos != body.size() - 2)
        return std::make_pair(CompleteMessage(), false);

    message.next_atc_address = body.left(comma_pos).mid(3);

    bool ok = false;
    int reason = body.right(1).toInt(&ok);
    if (!ok)
        return std::make_pair(CompleteMessage(), false);

    message.reason = (Reason)reason;

    return std::make_pair(message, true);
}


} // namespace AFN
