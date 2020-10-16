#ifndef DATEANDTIMEWITHZONEMIBOBJECT_H
#define DATEANDTIMEWITHZONEMIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class DateAndTimeWithZoneMibObject
 */

class DateAndTimeWithZoneMibObject : public ValueMibObject
{
    Q_OBJECT
public:
    static const size_t kMaxSize = 11;

    DateAndTimeWithZoneMibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly);

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
 * \class DateAndTimeWithZoneMibObjectCreator
 */

class DateAndTimeWithZoneMibObjectCreator : public ValueMibObjectCreator
{
public:
    DateAndTimeWithZoneMibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // DATEANDTIMEWITHZONEMIBOBJECT_H
