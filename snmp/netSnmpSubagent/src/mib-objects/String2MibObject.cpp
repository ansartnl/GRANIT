#include "String2MibObject.h"

#include "CallbackHandler.h"

#include "Log.h"

#include <QtCore/QVariant>

static QMap < int, String2MibObject * > stringObjects;

/*!
 * \class String2MibObject
 */

String2MibObject::String2MibObject(const QString &name_, const QString &oidString_, const QString &key_
                                 , bool readOnly) :
    ValueMibObject(name_, oidString_, key_, readOnly)
  , mValue(new char[kMaximumStringLength])
  , mOldValue(new char[kMaximumStringLength])
  , mMagic(-1)
{
    memset(mValue.data(), 0, kMaximumStringLength);
    memset(mOldValue.data(), 0, kMaximumStringLength);
}

String2MibObject::~String2MibObject()
{
    stringObjects.remove(mMagic);
}

u_char String2MibObject::asnType() const
{
    return ASN_OCTET_STR;
}

char String2MibObject::type() const
{
    return 's';
}

const QByteArray String2MibObject::value() const
{
    return QByteArray(mValue.data());
}

void String2MibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < QString >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to string");
        return;
    }
    QString str = value.toString();
    strcpy(mValue.data(), str.toUtf8().constData());
}

void String2MibObject::init()
{
    static u_char magic = 0;
    ++magic;
    mMagic = magic;

    variable var = { magic, (char)(asnType()), u_short(readOnly() ? HANDLER_CAN_RONLY : HANDLER_CAN_RWRITE)
                   , reader, 0, {} };

    int ret = MIB_REGISTERED_OK;
    if ((ret = register_mib(objectName().toUtf8().constData(), &var, sizeof(variable), 1, objid(), size()))
            != MIB_REGISTERED_OK) {
        emit error(tr("Error #%1 occured while trying to register watcher for variable '%2'")
                   .arg(ret).arg(objectName()));
        return;
    }

    registerCallback(this);

    setDefaultValue();

    stringObjects.insert(mMagic, this);
}

void String2MibObject::setValueCommit(netsnmp_vardata val, size_t val_len)
{
    Q_UNUSED(val)
    emit valueChanged(QString::fromUtf8(mValue.data(), val_len));
}

int String2MibObject::writer(int action, u_char *var_val, u_char var_val_type, size_t var_val_len
                             , u_char *statP, oid *name, size_t name_len)
{
    Q_UNUSED(statP) // Donnow what it is.

    static const size_t buff_len = 30000;
    static char buff[buff_len];

    snprint_objid(buff, buff_len, name, name_len);
    QString objid(buff);
    BaseMibObject *bmo = callbackHandler->objectByMibName(objid);
    String2MibObject *smo = qobject_cast < String2MibObject * >(bmo);
    if (!smo)
        return SNMP_ERR_WRONGTYPE;

    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_OCTET_STR) {
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > kMaximumStringLength) {
            return SNMP_ERR_WRONGVALUE;
        }
        break;
    case RESERVE2:
    case FREE:
    case ACTION:
        smo->writerAction((char *) var_val, var_val_len);
        break;
    case UNDO:
        smo->writerUndo();
        break;
    case COMMIT:
        smo->writerCommit();
        break;
    }
    return SNMP_ERR_NOERROR;
}

u_char * String2MibObject::reader(variable *vp, oid *name, size_t *length, int exact, size_t *var_len
                                  , WriteMethod **write_method)
{
    String2MibObject *stringObject = stringObjects[vp->magic];
    if (!stringObject)
        return 0;

    oid newname[MAX_OID_LEN];

    memcpy((char *) newname, (char *) vp->name, vp->namelen * sizeof(oid));
    newname[vp->namelen] = 0;
    int result = snmp_oid_compare(name, *length, newname, vp->namelen + 1);
    if ((exact && (result != 0)) || (!exact && (result >= 0)))
        return 0;
    memcpy((char *) name, (char *) newname, (vp->namelen + 1) * sizeof(oid));
    *length = vp->namelen + 1;

    *var_len = strlen(stringObject->mValue.data());

    *write_method = stringObject->writer;

    return (u_char *) stringObject->mValue.data();
}

void String2MibObject::writerAction(const char *val, size_t size)
{
    memcpy(mOldValue.data(), mValue.data(), kMaximumStringLength);
    memset(mValue.data(), 0, kMaximumStringLength);
    strncpy(mValue.data(), val, size);
}

void String2MibObject::writerCommit()
{
    memset(mOldValue.data(), 0, kMaximumStringLength);
    emit valueChanged(QString::fromUtf8(mValue.data()));
}

void String2MibObject::writerUndo()
{
    memcpy(mValue.data(), mOldValue.data(), kMaximumStringLength);
    memset(mValue.data(), 0, kMaximumStringLength);
}

netsnmp_watcher_info * String2MibObject::createWatcher()
{
    return netsnmp_create_watcher_info(mValue.data(), kMaximumStringLength, ASN_OCTET_STR, WATCHER_MAX_SIZE);
}

void String2MibObject::initValue(const char *val)
{
    strcpy(mValue.data(), val);
}

/*!
 * \class String2MibObjectCreator
 */

String2MibObjectCreator::String2MibObjectCreator()
{
}

ValueMibObject * String2MibObjectCreator::create(const QString &name, const QString &oidString
                                                , const QString &key, bool readOnly)
{
    return new String2MibObject(name, oidString, key, readOnly);
}
