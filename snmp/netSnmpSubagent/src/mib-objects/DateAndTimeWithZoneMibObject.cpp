#include "DateAndTimeWithZoneMibObject.h"

#include "Log.h"

#include <QtCore/QDateTime>
#include <QtCore/QVariant>

/*!
 * \class DateAndTimeWithZoneMibObject
 */

DateAndTimeWithZoneMibObject::DateAndTimeWithZoneMibObject(const QString &name_, const QString &oidString_
                                           , const QString &key_, bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly)
{
    memset(mValue, 0, kMaxSize);
}

u_char DateAndTimeWithZoneMibObject::asnType() const
{
    return ASN_OCTET_STR;
}

char DateAndTimeWithZoneMibObject::type() const
{
    return 's';
}

const QByteArray DateAndTimeWithZoneMibObject::value() const
{
    return QByteArray(mValue);
}

void DateAndTimeWithZoneMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < QDateTime >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to date and time");
        return;
    }
    const QDateTime dateTime = value.toDateTime().toLocalTime();
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

    const int hoursLocal = dateTime.time().hour();
    const int minutesLocal = dateTime.time().minute();
    const int hoursUtc = dateTime.toUTC().time().hour();
    const int minutesUtc = dateTime.toUTC().time().minute();
    if ((hoursUtc * 60 + minutesUtc) < (hoursLocal * 60 + minutesLocal)) {
        mValue[8] = '+';
    } else {
        mValue[8] = '-';
    }
    mValue[9] = qAbs(hoursUtc - hoursLocal);
    mValue[10] = qAbs(minutesUtc - minutesLocal);
}

void DateAndTimeWithZoneMibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    Q_UNUSED(val_len)
    QDateTime ret(QDate(mValue[0] * 256 + mValue[1], mValue[2], mValue[3])
                  , QTime(mValue[4], mValue[5], mValue[6], mValue[7] * 100)
                  , Qt::UTC);
    if (mValue[8] == '+')
        ret.time().addSecs((mValue[9] * 60 + mValue[10]) * 60);
    else if (mValue[8] == '-')
        ret.time().addSecs((mValue[9] * 60 + mValue[10]) * -60);
    emit valueChanged(ret);
}

netsnmp_watcher_info * DateAndTimeWithZoneMibObject::createWatcher()
{
    return netsnmp_create_watcher_info(mValue, kMaxSize, asnType(), WATCHER_MAX_SIZE);
}

void DateAndTimeWithZoneMibObject::initValue(const char *val)
{
    strncpy(mValue, val, kMaxSize);
}

/*!
 * \class DateAndTimeWithZoneMibObjectCreator
 */

DateAndTimeWithZoneMibObjectCreator::DateAndTimeWithZoneMibObjectCreator()
{
}

ValueMibObject * DateAndTimeWithZoneMibObjectCreator::create(const QString &name, const QString &oidString
                                                     , const QString &key, bool readOnly)
{
    return new DateAndTimeWithZoneMibObject(name, oidString, key, readOnly);
}
