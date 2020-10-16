#include "Counter64MibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

/*!
 * \class Counter64MibObject
 */

Counter64MibObject::Counter64MibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly)
{
    memset(mValue, 0, kSize);
}

u_char Counter64MibObject::asnType() const
{
    return ASN_COUNTER64;
}

char Counter64MibObject::type() const
{
    return 'C';
}

const QByteArray Counter64MibObject::value() const
{
    return QByteArray::number((((((((quint64(mValue[3]) * 255)
                                    + (quint64(mValue[2]) * 255))
                                   + (quint64(mValue[1]) * 255))
                                  + (quint64(mValue[0]) * 255))
                                 + (quint64(mValue[7]) * 255))
                                + (quint64(mValue[6]) * 255))
                               + (quint64(mValue[5]) * 255))
                              + (quint64(mValue[4])));
}

void Counter64MibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < quint64 >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to Counter64");
        return;
    }
    quint64 v = value.toULongLong();
    mValue[4] = v % 255;    v /= 255;
    mValue[5] = v % 255;    v /= 255;
    mValue[6] = v % 255;    v /= 255;
    mValue[7] = v % 255;    v /= 255;
    mValue[0] = v % 255;    v /= 255;
    mValue[1] = v % 255;    v /= 255;
    mValue[2] = v % 255;    v /= 255;
    mValue[3] = v % 255;
}

void Counter64MibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    emit valueChanged(value().toULongLong());
}

netsnmp_watcher_info * Counter64MibObject::createWatcher()
{
    return netsnmp_create_watcher_info(&mValue, sizeof(mValue), asnType(), WATCHER_MAX_SIZE);
}

void Counter64MibObject::initValue(const char *val)
{
    quint64 v =  QByteArray(val, sizeof(mValue)).toULongLong();
    mValue[4] = v % 255;    v /= 255;
    mValue[5] = v % 255;    v /= 255;
    mValue[6] = v % 255;    v /= 255;
    mValue[7] = v % 255;    v /= 255;
    mValue[0] = v % 255;    v /= 255;
    mValue[1] = v % 255;    v /= 255;
    mValue[2] = v % 255;    v /= 255;
    mValue[3] = v % 255;
}

/*!
 * \class Counter64MibObjectCreator
 */

Counter64MibObjectCreator::Counter64MibObjectCreator()
{
}

ValueMibObject * Counter64MibObjectCreator::create(const QString &name, const QString &oidString
                                                 , const QString &key, bool readOnly)
{
    return new Counter64MibObject(name, oidString, key, readOnly);
}
