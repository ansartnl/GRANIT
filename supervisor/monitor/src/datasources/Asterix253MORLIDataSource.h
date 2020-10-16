#ifndef ASTERIX253MORLIDATASOURCE_H
#define ASTERIX253MORLIDATASOURCE_H

#include <QObject>
#include <QColor>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"

#include "src/connections/asterix253/AsterixListener253.h"
#include "src/connections/asterix253/Asterix253Struct.h"

class Asterix253MORLIDataSourceItem;

class Asterix253MORLIDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    typedef QHash<Asterix253Struct::Source, Asterix253MORLIDataSourceItem*> TItemList;
public:
    explicit Asterix253MORLIDataSource(QObject *parent = 0);

    const QString& id() const;
    const QString& connectionID() const;
    SupervisorWidgetState state() const;
    const QString& name() const;

    const QObject* item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

    IConnection* dataConnection() const;

    void addReadableName(const QString &datakey, const QString &name);

signals:
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void stateChanged(QVariant state);
    void nameChanged(QVariant name);
    void eventToLog(const QString &host, const QString &service
                    , const QString &message, QColor color = Qt::black);

public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setName(const QString& name);

    void setDataConnection(IConnection* socket);

private:
    const QString readableName(int sac, int sic) const;

private slots:
    void onAsterix253Data(const QVariant& data);
    void onAsterixIdle();

private:
    QString mID;
    QString mConnectionID;
    SupervisorWidgetState mState;
    SupervisorWidgetState mMORLI1State;
    SupervisorWidgetState mMORLI2State;
    QString mName;
    asterix::AsterixListener253* mConnection;
    TItemList mItems;

    QHash < QPair < int, int >, QString > mReadableNames;
};

class Asterix253MORLIDataSourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(int sic READ sic WRITE setSic NOTIFY sicChanged)
    Q_PROPERTY(int sac READ sac WRITE setSac NOTIFY sacChanged)
public:
    Asterix253MORLIDataSourceItem(QObject* parent = 0);

    const QString& id() const;
    SupervisorWidgetState state() const;
    int sic() const;
    int sac() const;
    void setDefaut(const QString& defaut);
    QString getDefaut();

signals:    //! Properties
    void idChanged(QVariant id);
    void stateChanged(QVariant state);
    void sicChanged(QVariant sic);
    void sacChanged(QVariant sac);

public slots:
    void bind();
    void setID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setSic(int sic);
    void setSac(int sac);

private:
    QString mID;
    SupervisorWidgetState mState;

    int mSic;
    int mSac;
    QString mDefaut;
};

#endif // ASTERIX253MORLIDATASOURCE_H
