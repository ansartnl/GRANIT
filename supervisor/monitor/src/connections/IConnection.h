#ifndef ICONNECTION_H
#define ICONNECTION_H

#include <QObject>

//! Interface for connections
class IConnection
{
public:
    //explicit IConnection(QObject* parent):QObject(parent){}
    IConnection(){}
    virtual ~IConnection(){}

    virtual const QString id() const = 0;
    virtual const QString host() const = 0;
    virtual quint16 port() const = 0;
};

#endif // ICONNECTION_H
