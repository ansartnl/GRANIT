#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QMutex>
#include <QtCore/QString>

class Config
{
private:
    Config(const QString &path);

public:
    static void init(const QString &path = QString());
    static Config * instance();

    const QString logPath() const;
    int logLevel() const;

    const QStringList clients() const;

    int clientTimerMs() const;
    int managerLoopTimerMs() const;
    const QString pingOid() const;

    const QString trapsCommunity() const;
    const QStringList trapsReceivers() const;

private:
    void checkConfig() const;

    const QString configPath(const QString &fileName) const;

private:
    static Config *mConfig;
    mutable QMutex mutex;
};

#define config Config::instance()

#endif // CONFIG_H
