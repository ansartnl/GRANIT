#include "Header.h"

#include <QtCore/QStringList>
#include <QtCore/QRegExp>


namespace AFN {

QString buildHeader(const Header& header) {
    QString result = "FMH" + header.callsign + "," + header.tailnumber;

    if (!header.time.isNull()) {
        result += "," + header.identifier + "," + header.time.toString("HHmmss");
    }
    else if (!header.identifier.isEmpty()) {
        result += "," + header.identifier;
    }

    return result;
}

std::pair<Header, bool> parseHeader(const QString& source) {
    auto result = std::make_pair(Header(), false);

    QStringList parts = source.split(",");
    if (parts.count() < 2 || parts.count() > 4) {
        return result;
    }

    Header header;
    QRegExp rx;

    rx.setPattern("FMH([A-Z0-9]+)");
    if (!rx.exactMatch(parts[0])) {
        return result;
    }
    header.callsign = rx.cap(1);

    rx.setPattern("([A-Z0-9.-]{7})");
    if (!rx.exactMatch(parts[1])) {
        return result;
    }
    header.tailnumber = rx.cap(1);
    header.tailnumber.remove(QRegExp("^\\.*"));

    static QRegExp identifier("([0-9A-F]{6})");

    if (parts.count() == 3) {   // only identifier
        if (!identifier.exactMatch(parts[2])) {
            return result;
        }
        header.identifier = identifier.cap(1);
    }

    if (parts.count() == 4) {   // identifier (optional) and timestamp
        if (!parts[2].isEmpty()) {
            if (!identifier.exactMatch(parts[2])) {
                return result;
            }
            header.identifier = identifier.cap(1);
        }

        rx.setPattern("(\\d{6})");
        if (!rx.exactMatch(parts[3])) {
            return result;
        }
        header.time = QTime::fromString(rx.cap(1), "HHmmss");
        if (!header.time.isValid()) {
            return result;
        }
    }

    return std::make_pair(header, true);
}

QString getHeader(const QString &source) {
    int headerSize = source.indexOf('/');
    if (headerSize == -1)
        return QString();

    return source.left(headerSize);
}

} // namespace AFN
