#ifndef IPADDRESSMIBOBJECT_H
#define IPADDRESSMIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class IpAddressMibObject
 */

class IpAddressMibObject : public ValueMibObject
{
    Q_OBJECT
public:
    static const size_t kSize = 4;

    IpAddressMibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    char mValue[kSize];
};

/*!
 * \class IpAddressMibObjectCreator
 */

class IpAddressMibObjectCreator : public ValueMibObjectCreator
{
public:
    IpAddressMibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // IPADDRESSMIBOBJECT_H
