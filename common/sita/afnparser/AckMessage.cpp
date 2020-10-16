#include <QtCore/QStringList>

#include "AckMessage.h"


namespace AFN {

struct RegisterAckMessage {RegisterAckMessage() {qRegisterMetaType<AFN::AckMessage>();}};
static RegisterAckMessage registerAckMessage;

QString buildAckMessage(const AckMessage& message) {
    QString result = buildHeader(message.header) + "/FAK" + QString::number(message.reason) + "," + message.airport;
    for (const auto& app : message.apps) {
        result += "/FAR" + app.name + "," + QString::number(app.reason);
        if (!app.address.isEmpty())
            result += "," + app.address;
    }
    return result;
}

std::pair<AckMessage, bool> parseAckMessage(const QString& source) {
    int headerSize = source.indexOf('/');
    if (headerSize == -1)
        return std::make_pair(AckMessage(), false);

    auto header = parseHeader(source.left(headerSize));
    if (!header.second)
        return std::make_pair(AckMessage(), false);

    AckMessage result(header.first);

    QStringList parts = source.mid(headerSize + 1).split("/");
    if (parts.isEmpty())
        return std::make_pair(AckMessage(), false);

    QRegExp regExp("FAK([0-5]),([A-Z]{4})");
    if (!regExp.exactMatch(parts[0]))
        return std::make_pair(AckMessage(), false);

    result.reason = (Reason)regExp.cap(1).toInt();
    result.airport = regExp.cap(2);

    regExp.setPattern("FAR([A-Z]{3}),([0-5]),([A-Z0-9]{7})");
    for (int i = 1; i < parts.count(); ++i) {
        if (!regExp.exactMatch(parts[i]))
            return std::make_pair(AckMessage(), false);

        AckApp app;
        app.name = regExp.cap(1);
        app.reason = (Reason)regExp.cap(2).toInt();
        app.address = regExp.cap(3);

        result.apps.push_back(app);
    }

    return std::make_pair(result, true);
}

} // namespace AFN
