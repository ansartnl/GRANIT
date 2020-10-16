#ifndef HEADER_H_
#define HEADER_H_

#include <utility>

#include <QString>
#include <QTime>


namespace AFN {


enum Reason {
    Successful = 0,
    ProtocolError,
    VersionMismatch,
    NoResources,
    NoFlightPlan,
    Unknown
};


struct Header {
    QString callsign;   // aircraft identification/callsign
    QString tailnumber; // aircraft registration number/tail number
    QString identifier; // ICAO 24-bit identifier (optional)
    QTime   time;       // timestamp (optional)

    Header(const QString& _callsign = QString(), const QString& _tailnumber = QString(),
           const QString& _identifier = QString(), const QTime& _time = QTime())
        : callsign(_callsign), tailnumber(_tailnumber), identifier(_identifier), time(_time)
    {}
};


QString buildHeader(const Header& header);
std::pair<Header, bool> parseHeader(const QString& source);

QString getHeader(const QString& source);


} // namespace AFN


#endif // HEADER_H_
