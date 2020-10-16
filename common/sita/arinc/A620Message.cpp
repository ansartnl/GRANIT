#include "A620Message.h"

#include <QRegExp>
#include <QStringList>


namespace A620 {


void incrementCurrentLine(const QString& source, int& line_number, QString& current_line) {
    ++line_number;
    current_line = source.section(QRegExp("\\r\\n"), line_number, line_number);
}

bool findPattern(QString& current_line, QRegExp& pattern) {
    auto pos = pattern.indexIn(current_line);
    if (pos != -1) {
        current_line.remove(pos, pattern.matchedLength());
        return true;
    }
    return false;
}

QString parseIdentifier(const QString& source, int& line_number, QString& current_line) {
    static QRegExp pattern("^([A-Z]{3})$");
    QString identifier;
    if (findPattern(current_line, pattern))
        identifier = pattern.cap(1);
    else
        throw 0;
    incrementCurrentLine(source, line_number, current_line);

    return identifier;
}

void parseTextElements(const QString& source, int& line_number, QString& current_line, Message::TextElements& text_elements) {
    static QRegExp pattern("([A-Z0-9]{2}) (\\S+)");
    std::list<std::pair<QString, QString> > res;
    for (const auto& str : current_line.split("/")) {
        if (pattern.exactMatch(str))
            text_elements.insert(pattern.cap(1), pattern.cap(2));
        else
            throw 0;
    }
    incrementCurrentLine(source, line_number, current_line);
}

QDateTime parseCommunicationLine(const QString& source, int& line_number, QString& current_line) {
    static QRegExp pattern("^DT \\S+ \\S+ (\\d{2})(\\d{2})(\\d{2}) \\S+$");
    QDateTime time;

    if (findPattern(current_line, pattern)) {
        int day = pattern.cap(1).toInt();
        QDate date = QDateTime::currentDateTimeUtc().date();

        if (day > date.day())
            date = date.addMonths(-1);

        QDate messageDate(date.year(), date.month(), day);
        QTime messageTime(pattern.cap(2).toInt(), pattern.cap(3).toInt());

        if (!messageDate.isValid() || !messageTime.isValid())
            throw 0;

        time = QDateTime(messageDate, messageTime, Qt::UTC);
    } else {
        return time;
    }
    incrementCurrentLine(source, line_number, current_line);

    return time;
}

std::pair<Message, bool> parseMessage(const QString& source_message) {
    int line_number = -1;
    QString source(source_message);
    QString current_line;
    Message message;

    try {
        incrementCurrentLine(source, line_number, current_line);
        message.identifier = parseIdentifier(source, line_number, current_line);

        if (message.identifier != "SVC") {
            parseTextElements(source, line_number, current_line, message.textElements);
            message.time = parseCommunicationLine(source, line_number, current_line);
        }

        QString freeText = source.section(QRegExp("\\r\\n"), line_number);
        if (!freeText.isEmpty()) {
            static QRegExp pattern("-  (.*)");
            if (pattern.exactMatch(freeText))
                message.data = pattern.cap(1);
            else
                throw 0;
        }
    } catch (...) {
        return std::make_pair(Message(), false);
    }

    return std::make_pair(message, true);
}

QString buildMessage(const Message& message) {
    // move Flight Identification and Aircraft Number to begin of Text Elements line
    QStringList keys = message.textElements.keys();

    auto key_index = keys.indexOf("AN");
    if (key_index != -1)
        keys.move(key_index, 0);

    key_index = keys.indexOf("FI");
    if (key_index != -1)
        keys.move(key_index, 0);

    QString textElementsLine;
    for (const auto& key : keys) {
        if (key == "DT")
            continue;

        if (!textElementsLine.isEmpty())
            textElementsLine += "/";
        textElementsLine += key + " " + message.textElements.value(key);
    }

    if (message.textElements.contains("DT")) {
        if (!textElementsLine.isEmpty())
            textElementsLine += "\r\n";

        textElementsLine += "DT " + message.textElements.value("DT");
    }

    return QString("%3\r\n%4\r\n-  %5").arg(message.identifier, textElementsLine, message.data);
}


} // namespace A620
