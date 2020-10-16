#include "CallbackHandler.h"

#include "BaseMibObject.h"

#include <QtCore/QCoreApplication>

CallbackHandler::CallbackHandler(QObject *parent) :
    QObject(parent)
{
}

CallbackHandler * CallbackHandler::instance(QObject *parent)
{
    static CallbackHandler *mCallbackHandler = new CallbackHandler(parent);
    return mCallbackHandler;
}

void CallbackHandler::registerMibObject(BaseMibObject *bmo)
{
    connect(bmo, SIGNAL(destroyed()), SLOT(onDestroyed()));

    mMibObjects.insert(bmo->fullMibName(), bmo);
}

BaseMibObject * CallbackHandler::objectByMibName(const QString &fullMibName) const
{
    return qobject_cast < BaseMibObject * >(mMibObjects[fullMibName]);
}

void CallbackHandler::onDestroyed()
{
    QString key = mMibObjects.key(sender());
    mMibObjects.remove(key);
}
