#include "ProgramStateMibObject.h"

#include "Log.h"

#include <QtCore/QVariant>

ProgramStateMibObject::ProgramStateMibObject(const QString &name_, const QString &oidString_) :
    BaseMibObject(name_, oidString_), mValue(PS_DOWN)
{
}

u_char ProgramStateMibObject::asnType() const
{
    return ASN_INTEGER;
}

char ProgramStateMibObject::type() const
{
    return 'i';
}

const QByteArray ProgramStateMibObject::value() const
{
    return QByteArray::number(mValue);
}

void ProgramStateMibObject::setValue(const QVariant &value)
{
    if (!value.canConvert < int >()) {
        qLogWarn(objectName()) << tr("Cannot convert value to ProgramState");
        return;
    }
    mValue = value.toInt();
    if (mValue == PS_DOWN)
        emit programIsDown();
}

void ProgramStateMibObject::init()
{
    tryRegisterVariable(
                netsnmp_create_handler_registration(qPrintable(mibName()), handler
                                                    , objid(), size(), HANDLER_CAN_RWRITE)
                , netsnmp_create_watcher_info(&mValue, sizeof(mValue), asnType(), WATCHER_MAX_SIZE));
}
