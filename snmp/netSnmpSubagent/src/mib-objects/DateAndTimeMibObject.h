#ifndef DATEANDTIMEMIBOBJECT_H
#define DATEANDTIMEMIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class DateAndTimeMibObject
 */

class DateAndTimeMibObject : public ValueMibObject
{
    Q_OBJECT
public:
    static const size_t kMaxSize = 8;

    DateAndTimeMibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    char mValue[kMaxSize];
};

/*!
 * \class DateAndTimeMibObjectCreator
 */

class DateAndTimeMibObjectCreator : public ValueMibObjectCreator
{
public:
    DateAndTimeMibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // DATEANDTIMEMIBOBJECT_H
