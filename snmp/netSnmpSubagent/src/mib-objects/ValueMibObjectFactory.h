#ifndef VALUEMIBOBJECTFACTORY_H
#define VALUEMIBOBJECTFACTORY_H

#include "ValueMibObject.h"

#include <QtCore/QHash>

class ValueMibObjectFactory;

ValueMibObjectFactory * mibObjectFactory(QObject *parent = 0);

class ValueMibObjectFactory : public QObject
{
    Q_OBJECT
public:
    explicit ValueMibObjectFactory(QObject *parent = 0);

    ValueMibObject * create(const QString &type, const QString &name, const QString &oidString
                            , const QString &key, bool readOnly);

private:
    void installCreators();

private:
    QHash < QString, PValueMibObjectCreator > mCreators;
};

#endif // VALUEMIBOBJECTFACTORY_H
