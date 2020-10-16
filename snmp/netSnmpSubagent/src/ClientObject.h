#ifndef CLIENTOBJECT_H
#define CLIENTOBJECT_H

#include "TrapMibObject.h"

#include <QtCore/QHash>

class ValueMibObject;

class QSettings;
class QTimer;

class ClientObject : public QObject
{
    Q_OBJECT
public:
    explicit ClientObject(const QString &configFile, QObject *parent = 0);

    const QString identifier() const;

    void postInit();

public slots:
    void startListening();
    void stopListening();
    void changeValue(const QString &key, const QVariant &value);
    void notify(const QString &key, const snmp_interaction::Vars &vars);

signals:
    void getValue(const QString &key);
    void setValue(const QString &key, const QVariant &value);

private:
    void readConfig(QSettings *settings, bool readOnly);
    void addObject(ValueMibObject *obj, bool readOnly);
    void insertObject(const QString &key, BaseMibObject *obj);

private slots:
    void onTimer();
    void onValueChanged(const QVariant &value);
    void onMibObjectError(const QString &text);

private:
    QString mIdentifier;
    QHash < QString, PBaseMibObject > mMibObjects;
    QHash < QString, PTrapMibObject > mTrapObjects;
    QString mProgramStateKey;
    QTimer *mTimer;
};

typedef QSharedPointer < ClientObject > PClientObject;

#endif // CLIENTOBJECT_H
