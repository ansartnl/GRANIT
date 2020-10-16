#ifndef VALUEMIBOBJECT_H
#define VALUEMIBOBJECT_H

#include "BaseMibObject.h"

/*!
 * \class ValueMibObject
 */

class ValueMibObject : public BaseMibObject
{
    Q_OBJECT
public:
    ValueMibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly_);

    const QString key() const;
    bool readOnly() const;

public slots:
    void setDefaultValue();

protected:
    void init();
    virtual void initValue(const char *val) = 0;

    void nodeHandler(netsnmp_mib_handler *, netsnmp_handler_registration *
                     , netsnmp_agent_request_info *info, variable_list *vl);

    virtual netsnmp_watcher_info * createWatcher() = 0;
    virtual void setValueCommit(netsnmp_vardata val, size_t val_len) = 0;

private:
    QString mKey;
    bool mReadOnly;
};

/*!
 * \class ValueMibObjectCreator
 */

class ValueMibObjectCreator
{
public:
    ValueMibObjectCreator();
    virtual ~ValueMibObjectCreator();

    virtual ValueMibObject * create(const QString &name, const QString &oidString, const QString &key
                                    , bool readOnly) = 0;
};

typedef QSharedPointer < ValueMibObjectCreator > PValueMibObjectCreator;

#endif // VALUEMIBOBJECT_H
