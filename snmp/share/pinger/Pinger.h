#ifndef PINGER_H
#define PINGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>

class QTimer;

class Pinger : public QObject
{
    Q_OBJECT
public:
    explicit Pinger(QObject *parent = 0);

    void setTimerInterval(int msecs);
    void setMaximumLosses(int count);
    void setRemoveOnTimeout(bool remove = true);

public slots:
    void addItem(const QString &key);
    void removeItem(const QString &key);
    void pingCame(const QString &key);

signals:
    void sendPing(const QString &key);
    void timeout(const QString &key);

private:
    void updateItem(const QString &key, int count);

private slots:
    void onTimer();

private:
    static const int kDefaultTimerInterval = 3000;
    static const int kDefaultMaximumLosses = 2;
    static const bool kDefaultRemoveOnTimeout = false;

    QTimer *mTimer;
    QMap < QString, int > mItems;
    int mMaximumLosses;
    bool mRemoveOnTimeout;
};

#endif // PINGER_H
