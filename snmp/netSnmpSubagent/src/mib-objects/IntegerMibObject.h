#ifndef INTEGERMIBOBJECT_H
#define INTEGERMIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class IntegerMibObject
 */

class IntegerMibObject : public ValueMibObject
{
    Q_OBJECT
public:
    IntegerMibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    qint32 mValue;
};

/*!
 * \class IntegerMibObjectCreator
 */

class IntegerMibObjectCreator : public ValueMibObjectCreator
{
public:
    IntegerMibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // INTEGERMIBOBJECT_H
