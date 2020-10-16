#ifndef TRAPMIBOBJECT_H
#define TRAPMIBOBJECT_H

#include "BaseMibObject.h"

#include "Notify.h"

#include <QtCore/QStringList>

typedef QList < PBaseMibObject > MibObjects;

class TrapMibObject : public BaseMibEntity
{
    Q_OBJECT
public:
    TrapMibObject(const QString &name_, const QString &oidString_, const QString &key_
                  , const QString &community_, const QStringList &receivers_);

    u_char asnType() const;
    char type() const;

    const QString key() const;

    void notify(const MibObjects &objects);

protected:
    void init();

private:
    void sendNotify(const QString &ip, const MibObjects &objects);

private:
    QString mKey;
    QString mCommunity;
    QStringList mReceivers;
};

typedef QSharedPointer < TrapMibObject > PTrapMibObject;

#endif // TRAPMIBOBJECT_H
