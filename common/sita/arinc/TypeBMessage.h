#ifndef TYPEBMESSAGE_H_
#define TYPEBMESSAGE_H_

#include <utility>

#include <QDateTime>
#include <QStringList>

namespace TypeB {


struct Message {
    QString signature;
    QDateTime time;
    QString data;
    QStringList addresses;

    Message(const QString& _signature = QString(), const QString& _data = QString())
        : signature(_signature), data(_data)
    {}
};


std::pair<Message, bool> parseMessage(const QString& source);
QString buildMessage(const Message& message);


} // namespace TypeB


#endif // TYPEBMESSAGE_H_
