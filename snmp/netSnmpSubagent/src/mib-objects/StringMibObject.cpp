#include "StringMibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

/*!
 * \class StringMibObject
 */

StringMibObject::StringMibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly), mValue(new char[kMaximumStringLength])
{
    memset(mValue.data(), 0, kMaximumStringLength);
}

u_char StringMibObject::asnType() const
{
    return ASN_OCTET_STR;
}

char StringMibObject::type() const
{
    return 's';
}

const QByteArray StringMibObject::value() const
{
    return QByteArray(mValue.data());
}

void StringMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < QString >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to string");
        return;
    }
    QString str = value.toString();
    strcpy(mValue.data(), str.toUtf8().constData());
}

void StringMibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    emit valueChanged(QString::fromUtf8(mValue.data(), val_len));
}

netsnmp_watcher_info * StringMibObject::createWatcher()
{
    return netsnmp_create_watcher_info(mValue.data(), kMaximumStringLength, asnType(), WATCHER_MAX_SIZE);
}

void StringMibObject::initValue(const char *val)
{
    strcpy(mValue.data(), val);
}

/*!
 * \class StringMibObjectCreator
 */

StringMibObjectCreator::StringMibObjectCreator()
{
}

ValueMibObject * StringMibObjectCreator::create(const QString &name, const QString &oidString
                                                , const QString &key, bool readOnly)
{
    return new StringMibObject(name, oidString, key, readOnly);
}
