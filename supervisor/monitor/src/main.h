#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QSettings>
#include <QtScript>

#include "SupervisorMonitor.h"
#include "SMCLogManager.h"

QSettings *globalSettings(const QString & pathFile = QString());
QSettings & xmlSettings(const QString &pathConfig = QString());
SupervisorMonitor* supervisorMonitor();
SMCLogManager& logManager();

QScriptEngine* scriptEngine();
#endif // MAIN_H
