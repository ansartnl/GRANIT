#include "IntegerMibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

/*!
 * \class IntegerMibObject
 */

IntegerMibObject::IntegerMibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly), mValue(0)
{
}

u_char IntegerMibObject::asnType() const
{
    return ASN_INTEGER;
}

char IntegerMibObject::type() const
{
    return 'i';
}

const QByteArray IntegerMibObject::value() const
{
    return QByteArray::number(mValue);
}

void IntegerMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < int >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to integer");
        return;
    }
    mValue = value.toInt();
}

void IntegerMibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    emit valueChanged(mValue);
}

netsnmp_watcher_info * IntegerMibObject::createWatcher()
{
    return netsnmp_create_watcher_info(&mValue, sizeof(mValue), asnType(), WATCHER_MAX_SIZE);
}

void IntegerMibObject::initValue(const char *val)
{
    mValue = QByteArray(val, sizeof(mValue)).toInt();
}

/*!
 * \class IntegerMibObjectCreator
 */

IntegerMibObjectCreator::IntegerMibObjectCreator()
{
}

ValueMibObject * IntegerMibObjectCreator::create(const QString &name, const QString &oidString
                                                 , const QString &key, bool readOnly)
{
    return new IntegerMibObject(name, oidString, key, readOnly);
}
