#include "IpAddressMibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

/*!
 * \class IpAddressMibObject
 */

IpAddressMibObject::IpAddressMibObject(const QString &name_, const QString &oidString_, const QString &key_
                                       , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly)
{
    memset(mValue, 0, kSize);
}

u_char IpAddressMibObject::asnType() const
{
    return ASN_IPADDRESS;
}

char IpAddressMibObject::type() const
{
    return 'a';
}

const QByteArray IpAddressMibObject::value() const
{
    return QByteArray(mValue);
}

void IpAddressMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < QString >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to ip address");
        return;
    }
    const QHostAddress addr(value.toString());
    const quint32 ip4 = addr.toIPv4Address();
    mValue[0] = (ip4 & 0xFF000000) / 0x1000000;
    mValue[1] = (ip4 & 0x00FF0000) / 0x10000;
    mValue[2] = (ip4 & 0x0000FF00) / 0x100;
    mValue[3] = (ip4 & 0x000000FF) / 0x1;

// Same thing in loop, don't know which is better:
//    for (int i = kSize - 1; i >= 0; --i) {
//        mValue[i] = ip4 % 256;
//        ip4 /= 256;
//    }
}

void IpAddressMibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    emit valueChanged(QString("%1.%2.%3.%4")
                      .arg(uchar(mValue[0]), 0, 10, QChar('0'))
                      .arg(uchar(mValue[1]), 0, 10, QChar('0'))
                      .arg(uchar(mValue[2]), 0, 10, QChar('0'))
                      .arg(uchar(mValue[3]), 0, 10, QChar('0'))
                      );
}

netsnmp_watcher_info * IpAddressMibObject::createWatcher()
{
    return netsnmp_create_watcher_info(mValue, kSize, asnType(), WATCHER_MAX_SIZE);
}

void IpAddressMibObject::initValue(const char *val)
{
    memcpy(mValue, val, kSize);
}

/*!
 * \class IpAddressMibObjectCreator
 */

IpAddressMibObjectCreator::IpAddressMibObjectCreator()
{
}

ValueMibObject * IpAddressMibObjectCreator::create(const QString &name, const QString &oidString
                                                   , const QString &key, bool readOnly)
{
    return new IpAddressMibObject(name, oidString, key, readOnly);
}
