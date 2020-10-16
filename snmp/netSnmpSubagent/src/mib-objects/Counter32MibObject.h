#ifndef COUNTER32MIBOBJECT_H
#define COUNTER32MIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class Counter32MibObject
 */

class Counter32MibObject : public ValueMibObject
{
    Q_OBJECT
public:
    Counter32MibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    quint32 mValue;
};

/*!
 * \class Counter32MibObjectCreator
 */

class Counter32MibObjectCreator : public ValueMibObjectCreator
{
public:
    Counter32MibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // COUNTER32MIBOBJECT_H
