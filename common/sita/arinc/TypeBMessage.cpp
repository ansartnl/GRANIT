#include "TypeBMessage.h"

#include <QDateTime>
#include <QRegExp>



namespace TypeB {


std::pair<Message, bool> parseMessage(const QString& source) {
    static QRegExp pattern("(\\r\\n\\0001.+\\r\\n\\.\\0037)*"
                           "\\r\\n\\0001(.+)\\r\\n\\."             //  address section
                           "(.+)"                                  //  origin section
                           "\\r\\n\\0002(.*)\\r\\n\\0003");        //  text section
    static QRegExp address_pattern("\\s|(\\r\\n)");

    Message message;

    if (pattern.exactMatch(source)) {
        auto addressSection = pattern.cap(2);
        addressSection.remove(QRegExp("^([A-Z]{2} )"));
        for (const auto& address : addressSection.split(address_pattern)) {
            if (QRegExp("[A-Z]{7}").exactMatch(address))
                message.addresses.append(address);
            else
                return std::make_pair(Message(), false);
        }

//        static QRegExp originRegExp("^([A-Z]{7})\\b");
        static QRegExp originRegExp("([A-Z]{7})\\b( +(\\d{2})(\\d{2})(\\d{2})\\b)?");
        if (originRegExp.exactMatch(pattern.cap(3))) {
            message.signature = originRegExp.cap(1);

            if (!originRegExp.cap(2).isEmpty()) {
                int day = originRegExp.cap(3).toInt();
                QDate date = QDateTime::currentDateTimeUtc().date();

                if (day > date.day())
                    date = date.addMonths(-1);

                QDate messageDate(date.year(), date.month(), day);
                QTime messageTime(originRegExp.cap(4).toInt(), originRegExp.cap(5).toInt());

                QDateTime time(messageDate, messageTime, Qt::UTC);
                if (time.isValid()) {
                    message.time = time;
                }
            }
        }
        else {
            return std::make_pair(Message(), false);
        }

        message.data = pattern.cap(4);
        return std::make_pair(message, true);
    }

    return std::make_pair(Message(), false);
}

QString buildMessage(const Message& message) {
    QString addressSection = "QU";
    for (const auto& address : message.addresses)
        addressSection += " " + address;

    QDateTime time = message.time.isNull() ? QDateTime::currentDateTimeUtc() : message.time;

    auto originSection = message.signature + " " + time.toString("ddHHmm");

    return QString("\r\n\001%1\r\n.%2\r\n\002%3\r\n\003").arg(addressSection, originSection, message.data);
}


} // namespace TypeB
