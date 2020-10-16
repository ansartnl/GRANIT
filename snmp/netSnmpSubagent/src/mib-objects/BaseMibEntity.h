#ifndef BASEMIBENTITY_H
#define BASEMIBENTITY_H

#include <QtCore/QObject>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

class BaseMibEntity : public QObject
{
    Q_OBJECT
public:
    explicit BaseMibEntity(const QString &name_, const QString &oidString_);
    virtual ~BaseMibEntity();

    void postInit();

    const QString oidString() const;
    const QString fullMibName() const;

    virtual u_char asnType() const = 0;
    virtual char type() const = 0;

signals:
    void error(const QString &text);

protected:
    const QString mibName() const;

    oid * objid() const;
    size_t size() const;

    virtual void init() = 0;

private:
    void fillMibName();

private:
    QString mOidString;
    QString mMibBase;
    QString mMibName;

    oid mObjid[MAX_OID_LEN];
    size_t mObjidSize;

    friend class TrapMibObject;
};

#endif // BASEMIBENTITY_H
