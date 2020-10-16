#include "ContactMessage.h"

#include <QRegExp>
#include <QStringList>


namespace AFN {


struct RegisterContactMessage {RegisterContactMessage() {qRegisterMetaType<AFN::ContactMessage>();}};
static RegisterContactMessage registerContactMessage;

QString buildContactMessage(const ContactMessage& message) {
    QString latitude = QString("%1%2%3%4")
            .arg(message.position.latitude.direction == Latitude::North ? 'N' : 'S')
            .arg(message.position.latitude.degrees, 2, 10, QChar('0'))
            .arg(message.position.latitude.minutes, 2, 10, QChar('0'))
            .arg(message.position.latitude.tenths, 1);
    
    QString longitude = QString("%1%2%3%4")
            .arg(message.position.longitude.direction == Longitude::East ? 'E' : 'W')
            .arg(message.position.longitude.degrees, 3, 10, QChar('0'))
            .arg(message.position.longitude.minutes, 2, 10, QChar('0'))
            .arg(message.position.longitude.tenths, 1);

    QString result = buildHeader(message.header) + "/FPO" + latitude + longitude + ",1";

    for (const auto& app : message.apps) {
        result += QString("/FCO%1,%2").arg(app.name).arg(app.version, 2, 10, QChar('0'));
    }

    return result;
}

std::pair<ContactMessage, bool> parseContactMessage(const QString& source) {
    int headerSize = source.indexOf('/');
    if (headerSize == -1)
        return std::make_pair(ContactMessage(), false);

    auto parsed_header = parseHeader(source.left(headerSize));
    if (!parsed_header.second)
        return std::make_pair(ContactMessage(), false);

    static QRegExp pattern1("FCO([A-Z]{3}),(\\d{2})");
    static QRegExp pattern2("FPO((N|S)(\\d{2})(\\d{2})(\\d)(E|W)(\\d{3})(\\d{2})(\\d)),(0|1)");

    ContactMessage message(parsed_header.first);

    bool hasFpo = false;
    QStringList tokens(source.mid(headerSize + 1).split('/'));
    for (const auto& token : tokens) {
        if (pattern1.exactMatch(token)) {
            message.apps.push_back(ContactApp(pattern1.cap(1), pattern1.cap(2).toInt()));
        } else if (pattern2.exactMatch(token)) {
            if (hasFpo)
                return std::make_pair(ContactMessage(), false);

            message.rawPosition = pattern2.cap(1);
            message.position.latitude = Latitude(pattern2.cap(3).toInt(), pattern2.cap(4).toInt(), pattern2.cap(5).toInt(),
                                                 (pattern2.cap(2) == "N" ? Latitude::North : Latitude::South));
            message.position.longitude = Longitude(pattern2.cap(7).toInt(), pattern2.cap(8).toInt(), pattern2.cap(9).toInt(),
                                                   (pattern2.cap(6) == "E" ? Longitude::East : Longitude::West));
            hasFpo = true;
        } else {
            return std::make_pair(ContactMessage(), false);
        }
    }

    if (!hasFpo)
        return std::make_pair(ContactMessage(), false);

    return std::make_pair(message, true);
}


} // namespace AFN
