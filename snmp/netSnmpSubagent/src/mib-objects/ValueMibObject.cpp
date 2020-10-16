#include "ValueMibObject.h"

/*!
 * \class ValueMibObject
 */

ValueMibObject::ValueMibObject(const QString &name_, const QString &oidString_, const QString &key_
                               , bool readOnly_) :
    BaseMibObject(name_, oidString_), mKey(key_), mReadOnly(readOnly_)
{
}

const QString ValueMibObject::key() const
{
    return mKey;
}

bool ValueMibObject::readOnly() const
{
    return mReadOnly;
}

void ValueMibObject::setDefaultValue()
{
    tree *t = get_tree(objid(), size(), get_tree_head());
    if (t->defaultValue)
        initValue(t->defaultValue);
}

void ValueMibObject::nodeHandler(netsnmp_mib_handler *, netsnmp_handler_registration *
                                 , netsnmp_agent_request_info *info, variable_list *vl)
{
    if (info->mode == MODE_SET_COMMIT) {
        setValueCommit(vl->val, vl->val_len);
    }
}

void ValueMibObject::init()
{
    if (!tryRegisterVariable(
                netsnmp_create_handler_registration(qPrintable(mibName()), handler
                                                    , objid(), size()
                                                    , readOnly() ? HANDLER_CAN_RONLY : HANDLER_CAN_RWRITE)
                , createWatcher()))
        return;

    registerCallback(this);

    setDefaultValue();
}

/*!
 * \class ValueMibObjectCreator
 */

ValueMibObjectCreator::ValueMibObjectCreator()
{
}

ValueMibObjectCreator::~ValueMibObjectCreator()
{
}
