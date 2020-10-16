#include "Counter32MibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

/*!
 * \class Counter32MibObject
 */

Counter32MibObject::Counter32MibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly), mValue(0)
{
}

u_char Counter32MibObject::asnType() const
{
    return ASN_COUNTER;
}

char Counter32MibObject::type() const
{
    return 'c';
}

const QByteArray Counter32MibObject::value() const
{
    return QByteArray::number(mValue);
}

void Counter32MibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < quint32 >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to Counter32");
        return;
    }
    mValue = value.toUInt();
}

void Counter32MibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    // Не проверено
//    emit valueChanged(mValue);
}

netsnmp_watcher_info * Counter32MibObject::createWatcher()
{
    return netsnmp_create_watcher_info(&mValue, sizeof(mValue), asnType(), WATCHER_MAX_SIZE);
}

void Counter32MibObject::initValue(const char *val)
{
    mValue = QByteArray(val, sizeof(mValue)).toUInt();
}

/*!
 * \class Counter32MibObjectCreator
 */

Counter32MibObjectCreator::Counter32MibObjectCreator()
{
}

ValueMibObject * Counter32MibObjectCreator::create(const QString &name, const QString &oidString
                                                 , const QString &key, bool readOnly)
{
    return new Counter32MibObject(name, oidString, key, readOnly);
}
