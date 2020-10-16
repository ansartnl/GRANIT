#include "BaseMibObject.h"

#include "CallbackHandler.h"

BaseMibObject::BaseMibObject(const QString &name_, const QString &oidString_) :
    BaseMibEntity(name_, oidString_)
{
}

void BaseMibObject::registerCallback(BaseMibObject *bmo)
{
    callbackHandler->registerMibObject(bmo);
}

int BaseMibObject::handler(netsnmp_mib_handler *mibHandler, netsnmp_handler_registration *reginfo
                           , netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    static const size_t buff_len = 30000;
    static char buff[buff_len];

    for (variable_list *vl = requests->requestvb; vl; vl = vl->next_variable) {
        snprint_objid(buff, buff_len, vl->name, vl->name_length);
        QString objid(buff);
        BaseMibObject *bmo = callbackHandler->objectByMibName(objid);
        if (bmo)
            bmo->nodeHandler(mibHandler, reginfo, reqinfo, vl);
    }

    return 0;
}

bool BaseMibObject::tryRegisterVariable(netsnmp_handler_registration *reginfo, netsnmp_watcher_info *winfo)
{
    int ret = netsnmp_register_watched_scalar(reginfo, winfo);
    if (ret) {
        emit error(tr("Error #%1 occured while trying to register watcher for variable '%2'")
                   .arg(ret).arg(reginfo->handler->handler_name));
        return false;
    }
    return true;
}

void BaseMibObject::nodeHandler(netsnmp_mib_handler *mibHandler, netsnmp_handler_registration *reginfo
                                , netsnmp_agent_request_info *reqinfo, variable_list *vl)
{
    Q_UNUSED(mibHandler)
    Q_UNUSED(reginfo)
    Q_UNUSED(reqinfo)
    Q_UNUSED(vl)
}
