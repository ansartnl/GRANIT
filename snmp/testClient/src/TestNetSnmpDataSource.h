#ifndef TESTNETSNMPDATASOURCE_H
#define TESTNETSNMPDATASOURCE_H

#include "INetSnmpDataSource.h"

#include <QtCore/QDateTime>
#include <QtCore/QHash>
#include <QtCore/QObject>

class TestNetSnmpDataSource : public QObject, public snmp::INetSnmpDataSource
{
    Q_OBJECT
public:
    explicit TestNetSnmpDataSource(QObject *parent = 0);
    ~TestNetSnmpDataSource();

    const QString identifier() const;

    const QVariant value(const QString &key) const;
    SetResult setValue(const QString &key, const QVariant &value);

    void writeLogInfo(const QString &text);
    void writeLogError(const QString &text);

signals:
    void text(const QString &text) const;
    void info(const QString &text) const;
    void error(const QString &text) const;

private slots:
    void sendTrap();

private:
    QHash < QString, QVariant > mValues;
    QDateTime mStartTime;
};

#endif // TESTNETSNMPDATASOURCE_H
