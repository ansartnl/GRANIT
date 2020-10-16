#ifndef STRING2MIBOBJECT_H
#define STRING2MIBOBJECT_H

#include "ValueMibObject.h"

/*!
 * \class String2MibObject
 */

class String2MibObject : public ValueMibObject
{
    Q_OBJECT
public:
    String2MibObject(const QString &name_, const QString &oidString_, const QString &key_, bool readOnly);
    virtual ~String2MibObject();

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

protected:
    void init();
    void initValue(const char *val);

    netsnmp_watcher_info * createWatcher();
    void setValueCommit(netsnmp_vardata val, size_t val_len);

private:
    static int writer(int action, u_char *var_val, u_char var_val_type, size_t var_val_len
                      , u_char *statP, oid *name, size_t name_len);
    static u_char * reader(struct variable *vp, oid *name, size_t *length, int exact
                           , size_t *var_len, WriteMethod **write_method);

    void writerAction(const char *val, size_t size);
    void writerCommit();
    void writerUndo();

private:
    // read COMMON-MIB for details
    static const size_t kMaximumStringLength = 1024;

    QSharedPointer < char > mValue;
    QSharedPointer < char > mOldValue;
    u_char mMagic;
};

/*!
 * \class String2MibObjectCreator
 */

class String2MibObjectCreator : public ValueMibObjectCreator
{
public:
    String2MibObjectCreator();

    ValueMibObject * create(const QString &name, const QString &oidString, const QString &key, bool readOnly);
};

#endif // STRING2MIBOBJECT_H
