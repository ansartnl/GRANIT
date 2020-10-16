#ifndef ACKMESSAGE_H_
#define ACKMESSAGE_H_

#include <QString>
#include <QList>
#include <QMetaType>

#include <list>

#include "Header.h"


namespace AFN {


struct AckApp {
    QString name;
    Reason reason;
    QString address;

    AckApp(const QString& _name = QString(), Reason _reason = Unknown, const QString& _address = QString())
        : name(_name), reason(_reason), address(_address)
    {}
};


struct AckMessage {
    typedef std::list<AckApp> Apps;

    Header header;
    Reason reason;
    QString airport;
    Apps apps;

    AckMessage(const Header& _header = Header(), Reason _reason = Unknown, const QString& _airport = QString())
        : header(_header), reason(_reason), airport(_airport)
    {}
};


QString buildAckMessage(const AckMessage& message);
std::pair<AckMessage, bool> parseAckMessage(const QString& source);


} // namespace AFN


Q_DECLARE_METATYPE(AFN::AckMessage)


#endif // ACKMESSAGE_H_
