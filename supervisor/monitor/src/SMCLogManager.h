#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QFile>

class SMCLogManager : public QObject
{
    Q_OBJECT
public:
    explicit SMCLogManager(QObject *parent = 0);
    ~SMCLogManager();

    void setUseAsterixLog(bool use);
    void initLog4Qt(QObject *parent);
    void initAsterixLogger();

private:
    QString getLogFileName() const;
    QString getAsterixLogFileName() const;
    void setupLogger(QObject *parent);

private slots:
    void updateLogs();

signals:

public slots:
    void asterixLog(const QString& sender, const QByteArray& data);

private:
    bool mUseAsterixLog;

    QFile mAsterixLog;
};

#endif // LOGMANAGER_H
