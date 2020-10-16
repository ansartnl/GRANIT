#ifndef CALLBACKHANDLER_H
#define CALLBACKHANDLER_H

#include <QtCore/QMap>
#include <QtCore/QObject>

class BaseMibObject;

class CallbackHandler : public QObject
{
    Q_OBJECT
private:
    explicit CallbackHandler(QObject *parent = 0);

public:
    static CallbackHandler *instance(QObject *parent = 0);

    void registerMibObject(BaseMibObject *bmo);

    BaseMibObject * objectByMibName(const QString &fullMibName) const;

private slots:
    void onDestroyed();

private:
    QMap < QString, QObject * > mMibObjects;
};

#define callbackHandler CallbackHandler::instance()

#endif // CALLBACKHANDLER_H
