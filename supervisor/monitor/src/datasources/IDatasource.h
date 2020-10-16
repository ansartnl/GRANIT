#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#include <QObject>

class MonitoringSocket;
class IConnection;
//! Interface for all datasources
class IDatasource : public QObject
{
    Q_OBJECT
public:
    explicit IDatasource(QObject* parent):QObject(parent){}
    virtual ~IDatasource(){}

    virtual const QString& id() const = 0;

    virtual const QObject* item(const QString& key) = 0;
    virtual const QMetaObject* metaItem(const QString& key) = 0;
    //virtual MonitoringSocket* dataConnection() const = 0;
    virtual IConnection* dataConnection() const = 0;
    virtual void setDataConnection(IConnection* connection) = 0;
    virtual void setDefaut(const QString& key, const QString& defaut) = 0;
    virtual void setDatafield(const QString& key, const QString& defaut) = 0;

    virtual void addReadableName(const QString &datakey, const QString &name)
    {
        Q_UNUSED(datakey)
        Q_UNUSED(name)
    }

public slots:
    virtual void setID(const QString& id) = 0;
    virtual void bind() = 0;    //! must send NOTIFY signals for all tracked datafields
};

#endif // IDATASOURCE_H
