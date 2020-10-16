#ifndef STRINGMIBOBJECT_H
#define STRINGMIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class StringMibObject
 */

class StringMibObject : public ValueMibObject
{
    Q_OBJECT
public:
    StringMibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    // read COMMON-MIB for details
    static const size_t kMaximumStringLength = 1024;

    QSharedPointer < char > mValue;
};

/*!
 * \class StringMibObjectCreator
 */

class StringMibObjectCreator : public ValueMibObjectCreator
{
public:
    StringMibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // STRINGMIBOBJECT_H
