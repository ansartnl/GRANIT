#include "BaseMibEntity.h"

#include <QtCore/QRegExp>

BaseMibEntity::BaseMibEntity(const QString &name_, const QString &oidString_) :
    QObject(0), mOidString(oidString_)
{
    setObjectName(name_);

    mObjidSize = MAX_OID_LEN;
    read_objid(qPrintable(oidString()), mObjid, &mObjidSize);
}

BaseMibEntity::~BaseMibEntity()
{
}

void BaseMibEntity::postInit()
{
    fillMibName();
    init();
}

const QString BaseMibEntity::oidString() const
{
    return mOidString;
}

const QString BaseMibEntity::fullMibName() const
{
    return QString("%1::%2.0").arg(mMibBase).arg(mMibName);
}

const QString BaseMibEntity::mibName() const
{
    return mMibName;
}

oid * BaseMibEntity::objid() const
{
    return const_cast < oid * >(mObjid);
}

size_t BaseMibEntity::size() const
{
    return mObjidSize;
}

void BaseMibEntity::fillMibName()
{
    const size_t buff_len = 30000;
    char buff[buff_len];

    snprint_objid(buff, buff_len, mObjid, mObjidSize);
    QString objidString(buff);

    QRegExp rexp("(.+)::(.+)(.0)?");
    if (rexp.exactMatch(objidString)) {
        mMibBase = rexp.cap(1);
        mMibName = rexp.cap(2);
    }
}
