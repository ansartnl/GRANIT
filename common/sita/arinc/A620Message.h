#ifndef A620MESSAGE_H_
#define A620MESSAGE_H_

#include <utility>

#include <QString>
#include <QMap>
#include <QDateTime>


namespace A620 {


struct Message {
    typedef QMap<QString, QString> TextElements;

    QString identifier;
    QString data;
    QDateTime time;
    TextElements textElements;

    Message(const QString& _identifier = QString(), const QString& _data = QString(), const QDateTime& _time = QDateTime())
        : identifier(_identifier), data(_data), time(_time)
    {}
};


std::pair<Message, bool> parseMessage(const QString& source_message);
QString buildMessage(const Message& message);


} // namespace A620


#endif // A620MESSAGE_H_
