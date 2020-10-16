#ifndef CONTACTMESSAGE_H_
#define CONTACTMESSAGE_H_

#include <utility>
#include <list>

#include <QString>
#include <QList>
#include <QMetaType>

#include "Header.h"


namespace AFN {


struct ContactApp {
    QString name;
    int version;

    ContactApp(const QString& _name = QString(), int _version = 0)
        : name(_name), version(_version)
    {}
};


struct Coordinates {
    int degrees;
    int minutes;
    int tenths;

    Coordinates(int _degrees = 0, int _minutes = 0, int _tenths = 0)
        : degrees(_degrees), minutes(_minutes), tenths(_tenths)
    {}
};


struct Latitude: Coordinates {
    enum Direction {
        North,
        South
    } direction;

    Latitude(int _degrees = 0, int _minutes = 0, int _tenths = 0, Direction _direction = North)
        : Coordinates(_degrees, _minutes, _tenths), direction(_direction)
    {}
};


struct Longitude: Coordinates {
    enum Direction {
        East,
        West
    } direction;

    Longitude(int _degrees = 0, int _minutes = 0, int _tenths = 0, Direction _direction = East)
        : Coordinates(_degrees, _minutes, _tenths), direction(_direction)
    {}
};


struct Position {
    Latitude  latitude;
    Longitude longitude;

    Position(const Latitude& _latitude = Latitude(), const Longitude& _longitude = Longitude())
        : latitude(_latitude), longitude(_longitude)
    {}
};


struct ContactMessage {
    typedef std::list<ContactApp> Apps;

    Header header;
    QString rawPosition;
    Position position;
    Apps apps;

    ContactMessage(const Header& _header = Header(), const Position& _position = Position())
        : header(_header), position(_position)
    {}
};

QString buildContactMessage(const ContactMessage& message);
std::pair<ContactMessage, bool> parseContactMessage(const QString& source);


} // namespace AFN


Q_DECLARE_METATYPE(AFN::ContactMessage)


#endif // CONTACTMESSAGE_H_
