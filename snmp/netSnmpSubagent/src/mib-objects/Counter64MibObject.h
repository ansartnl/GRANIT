#ifndef COUNTER64MIBOBJECT_H
#define COUNTER64MIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class Counter64MibObject
 */

class Counter64MibObject : public ValueMibObject
{
    Q_OBJECT
public:
    static const size_t kSize = 8;

    Counter64MibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

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
 * \class Counter64MibObjectCreator
 */

class Counter64MibObjectCreator : public ValueMibObjectCreator
{
public:
    Counter64MibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // COUNTER64MIBOBJECT_H
