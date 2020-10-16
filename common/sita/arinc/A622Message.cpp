#include "A622Message.h"

#include <QRegExp>
#include <QStringList>

#include "CRC.h"


namespace A622 {


std::pair<Message, Error> parseMessage(const QString& source) {
    static QRegExp pattern("(?:/([A-Z]{7})\\.)?(\\w{3})/(.*)([0-9A-F]{4})");

    if (pattern.exactMatch(source)) {
        Message message(pattern.cap(2), pattern.cap(3), pattern.cap(1));
        ushort crc = ~CRC::crcCCITT_FFFF(source.left(source.size() - 4).toLatin1());
        Error error = (crc == pattern.cap(4).toInt(0, 16)) ? NoError : ChecksumError;

        return std::make_pair(message, error);
    }

    return std::make_pair(Message(), ParsingError);
}

QString buildMessage(const Message& message, int line_length) {
    QString result = "/" + message.address + "." + message.identifier + "/" + wrapMessage(message.data, line_length);
    ushort crc = ~CRC::crcCCITT_FFFF(result.toLatin1());

    return result + QString("%1").arg(crc, 4, 16, QChar('0')).toUpper();
}

QString wrapMessage(const QString& message, int line_length) {
    QStringList result;
    QString text = message;
    QStringList list = text.remove("\r").split("\n");

    if (line_length > 0) {
        for (auto& line : list) {
            while (line.length() > line_length) {
                int index = line.lastIndexOf(" ", line_length);

                if (index == -1 || index == 0)
                    break;

                result.append(line.mid(0, index));
                line.remove(0, index);
            }
            result.append(line);
        }
    } else {
        result = list;
    }

    return result.join("\r\n");
}


} // namespace A622
