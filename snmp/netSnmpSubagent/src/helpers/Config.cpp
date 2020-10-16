#include "Config.h"

#include "ConfigKeys.h"

#ifdef NITA_XML
#include "nitaxmlsettings.h"
typedef NitaXmlSettings XmlSettings;
#else
#include "qtxmlsettings.h"
typedef QtXmlSettings XmlSettings;
#endif

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

XmlSettings * xmlSettings(const QString &path = QString())
{
    static XmlSettings *s = new XmlSettings(path);
    return s;
}

Config * Config::mConfig = 0;

Config::Config(const QString &path)
{
    xmlSettings(path);
    checkConfig();
}

void Config::init(const QString &path)
{
    if (mConfig)
        return;
    mConfig = new Config(path);
}

Config * Config::instance()
{
    return mConfig;
}

const QString Config::logPath() const
{
    QMutexLocker locker(&mutex);
    xmlSettings()->beginGroup(xml::kLog);
    const QString ret = xmlSettings()->value(xml::kPath).toString();
    xmlSettings()->endGroup();
    return ret;
}

int Config::logLevel() const
{
    QMutexLocker locker(&mutex);
    xmlSettings()->beginGroup(xml::kLog);
    const int ret = xmlSettings()->value(xml::kLevel).toInt();
    xmlSettings()->endGroup();
    return ret;
}

const QStringList Config::clients() const
{
    QMutexLocker locker(&mutex);
    QStringList ret;
    xmlSettings()->beginGroup(xml::kClients);
    foreach (const QString &key, xmlSettings()->childKeys()) {
#ifdef NITA_XML
        if (key != NitaXmlSettings::kDescription)
#endif
            ret << configPath(xmlSettings()->value(key).toString());
    }
    xmlSettings()->endGroup();
    return ret;
}

int Config::clientTimerMs() const
{
    QMutexLocker locker(&mutex);
    return xmlSettings()->value(xml::kClientTimerMs).toInt();
}

int Config::managerLoopTimerMs() const
{
    QMutexLocker locker(&mutex);
    return xmlSettings()->value(xml::kManagerLoopTimerMs).toInt();
}

const QString Config::pingOid() const
{
    QMutexLocker locker(&mutex);
    return xmlSettings()->value(xml::kPingOid).toString();
}

const QString Config::trapsCommunity() const
{
    QMutexLocker locker(&mutex);
    xmlSettings()->beginGroup(xml::kTraps);
    const QString ret = xmlSettings()->value(xml::kCommunity).toString();
    xmlSettings()->endGroup();
    return ret;
}

const QStringList Config::trapsReceivers() const
{
    QMutexLocker locker(&mutex);
    xmlSettings()->beginGroup(xml::kTraps);
    const QStringList ret = xmlSettings()->value(xml::kReceivers).toString().split(',');
    xmlSettings()->endGroup();
    return ret;
}

void Config::checkConfig() const
{
    QDir binDir("bin:");

    // log
    xmlSettings()->beginGroup(xml::kLog);
    if (!xmlSettings()->contains(xml::kPath)) {
        QString path;
        if (QDir("log:").exists())
            path = QDir("log:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
        else
            path = binDir.absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
        xmlSettings()->setValue(xml::kPath, path);
    }
    if (!xmlSettings()->contains(xml::kLevel))
        xmlSettings()->setValue(xml::kLevel, 6);
    xmlSettings()->endGroup();

    // clients

    // timers
    if (!xmlSettings()->contains(xml::kClientTimerMs))
        xmlSettings()->setValue(xml::kClientTimerMs, 1000);
    if (!xmlSettings()->contains(xml::kManagerLoopTimerMs))
        xmlSettings()->setValue(xml::kManagerLoopTimerMs, 0);

    // ping oid
    if (!xmlSettings()->contains(xml::kPingOid))
        xmlSettings()->setValue(xml::kPingOid, ".1.3.6.1.4.1.252093.0");

    // traps
    xmlSettings()->beginGroup(xml::kTraps);
    if (!xmlSettings()->contains(xml::kCommunity))
        xmlSettings()->setValue(xml::kCommunity, "thedarkempire");
    if (!xmlSettings()->contains(xml::kReceivers))
        xmlSettings()->setValue(xml::kReceivers, "127.0.0.1");
    xmlSettings()->endGroup();

#ifdef NITA_XML
    // log
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kLog)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kLog)
                                , "Log writing settings");
    xmlSettings()->beginGroup(xml::kLog);
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kPath)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kPath)
                                , "Path to log file");
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kLevel)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kLevel)
                                , "Maximum log level, wanted to be written");
    if (!xmlSettings()->contains(NitaXmlSettings::range(xml::kLevel)))
        xmlSettings()->setValue(NitaXmlSettings::range(xml::kLevel)
                                , "0-6");
    xmlSettings()->endGroup();

    // clients
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kClients)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kClients)
                                , "List of conf files to be read");

    // timers
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kClientTimerMs)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kClientTimerMs)
                                , "Time interval between reading clients' monitored variables");
    if (!xmlSettings()->contains(NitaXmlSettings::unit(xml::kClientTimerMs)))
        xmlSettings()->setValue(NitaXmlSettings::unit(xml::kClientTimerMs)
                                , "ms");
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kManagerLoopTimerMs)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kManagerLoopTimerMs)
                                , "Time interval between reading snmp queries");
    if (!xmlSettings()->contains(NitaXmlSettings::range(xml::kManagerLoopTimerMs)))
        xmlSettings()->setValue(NitaXmlSettings::range(xml::kManagerLoopTimerMs)
                                , "0-1");
    if (!xmlSettings()->contains(NitaXmlSettings::unit(xml::kManagerLoopTimerMs)))
        xmlSettings()->setValue(NitaXmlSettings::unit(xml::kManagerLoopTimerMs)
                                , "ms");

    // ping oid
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kPingOid)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kPingOid)
                                , "OID of control value for checking subagent accessibility");

    // traps
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kTraps)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kTraps)
                                , "Traps settings");
    xmlSettings()->beginGroup(xml::kTraps);
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kCommunity)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kCommunity)
                                , "Community of session under which traps would be sent");
    if (!xmlSettings()->contains(NitaXmlSettings::description(xml::kReceivers)))
        xmlSettings()->setValue(NitaXmlSettings::description(xml::kReceivers)
                                , "Comma separated list of addresses of traps receivers");
    xmlSettings()->endGroup();

#endif

    xmlSettings()->sync();
}

const QString Config::configPath(const QString &fileName) const
{
    if (QFile::exists(fileName))
        return fileName;

    if (QFileInfo(fileName).isAbsolute())
        return QString();

    const QDir xmlDir = QFileInfo(xmlSettings()->fileName()).dir();
    const QString absoluteFilePath = xmlDir.absoluteFilePath(fileName);

    if (QFile::exists(absoluteFilePath))
        return absoluteFilePath;

    return QString();
}
