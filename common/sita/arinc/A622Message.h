#ifndef A622MESSAGE_H_
#define A622MESSAGE_H_

#include <utility>

#include <QString>
#include <QByteArray>


namespace A622 {


enum Error {
    ParsingError,
    ChecksumError,
    NoError
};


struct Message {
    QString identifier;
    QString data;
    QString address;

    Message(const QString& _identifier = QString(), const QString& _data = QString(), const QString& _address = QString())
        : identifier(_identifier), data(_data), address(_address)
    {}
};


std::pair<Message, Error> parseMessage(const QString& source);
QString buildMessage(const Message& message, int line_length);
QString wrapMessage(const QString& message, int line_length);


} // namespace A622


#endif // A622MESSAGE_H_
