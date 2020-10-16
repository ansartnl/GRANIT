#include "DateAndTimeMibObject.h"

#include "Log.h"

#include <QtCore/QDateTime>
#include <QtCore/QVariant>

/*!
 * \class DateAndTimeMibObject
 */

DateAndTimeMibObject::DateAndTimeMibObject(const QString &name_, const QString &oidString_
                                           , const QString &key_, bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly)
{
    memset(mValue, 0, kMaxSize);
}

u_char DateAndTimeMibObject::asnType() const
{
    return ASN_OCTET_STR;
}

char DateAndTimeMibObject::type() const
{
    return 's';
}

const QByteArray DateAndTimeMibObject::value() const
{
    return QByteArray(mValue);
}

void DateAndTimeMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < QDateTime >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to date and time");
        return;
    }
    const QDateTime dateTime = value.toDateTime();
    if (!dateTime.isValid())
        return;
    mValue[0] = dateTime.date().year() / 256;
    mValue[1] = dateTime.date().year() % 256;
    mValue[2] = dateTime.date().month();
    mValue[3] = dateTime.date().day();
    mValue[4] = dateTime.time().hour();
    mValue[5] = dateTime.time().minute();
    mValue[6] = dateTime.time().second();
    mValue[7] = dateTime.time().msec() / 100;
}

void DateAndTimeMibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    QDateTime ret(QDate(uchar(mValue[0]) * 256 + uchar(mValue[1]), uchar(mValue[2]), uchar(mValue[3]))
                  , QTime(uchar(mValue[4]), uchar(mValue[5]), uchar(mValue[6]), uchar(mValue[7]) * 100)
                  , Qt::UTC);
    emit valueChanged(ret);
}

netsnmp_watcher_info * DateAndTimeMibObject::createWatcher()
{
    return netsnmp_create_watcher_info(mValue, kMaxSize, asnType(), WATCHER_MAX_SIZE);
}

void DateAndTimeMibObject::initValue(const char *val)
{
    strncpy(mValue, val, kMaxSize);
}

/*!
 * \class DateAndTimeMibObjectCreator
 */

DateAndTimeMibObjectCreator::DateAndTimeMibObjectCreator()
{
}

ValueMibObject * DateAndTimeMibObjectCreator::create(const QString &name, const QString &oidString
                                                     , const QString &key, bool readOnly)
{
    return new DateAndTimeMibObject(name, oidString, key, readOnly);
}
