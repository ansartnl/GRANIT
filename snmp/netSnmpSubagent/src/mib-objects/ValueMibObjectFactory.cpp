#include "ValueMibObjectFactory.h"

#include "Counter32MibObject.h"
#include "Counter64MibObject.h"
#include "DateAndTimeMibObject.h"
#include "DateAndTimeWithZoneMibObject.h"
#include "IntegerMibObject.h"
#include "IpAddressMibObject.h"
#include "StringMibObject.h"
#include "String2MibObject.h"

#include "Log.h"

ValueMibObjectFactory * mibObjectFactory(QObject *parent)
{
    static ValueMibObjectFactory *mValueMibObjectFactory = new ValueMibObjectFactory(parent);
    return mValueMibObjectFactory;
}

ValueMibObjectFactory::ValueMibObjectFactory(QObject *parent) :
    QObject(parent)
{
    setObjectName("mib object factory");

    installCreators();
}

ValueMibObject * ValueMibObjectFactory::create(const QString &type, const QString &name
                                               , const QString &oidString, const QString &key, bool readOnly)
{
    PValueMibObjectCreator creator = mCreators[type.toLower()];
    if (!creator.isNull())
        return creator->create(name, oidString, key, readOnly);

    qLogWarn(objectName()) << tr("Type '%1' of value '%2' is not supported").arg(type).arg(name);
    return 0;
}

void ValueMibObjectFactory::installCreators()
{
    mCreators.insert("counter32", PValueMibObjectCreator(new Counter32MibObjectCreator));
    mCreators.insert("counter64", PValueMibObjectCreator(new Counter64MibObjectCreator));
    mCreators.insert("dateandtime", PValueMibObjectCreator(new DateAndTimeMibObjectCreator));
    mCreators.insert("dateandtimewithzone", PValueMibObjectCreator(new DateAndTimeWithZoneMibObjectCreator));
    mCreators.insert("integer", PValueMibObjectCreator(new IntegerMibObjectCreator));
    mCreators.insert("ipaddress", PValueMibObjectCreator(new IpAddressMibObjectCreator));
    mCreators.insert("string", PValueMibObjectCreator(new String2MibObjectCreator));
}
