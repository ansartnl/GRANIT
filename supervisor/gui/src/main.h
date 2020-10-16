#ifndef _MAIN_H__
#define _MAIN_H__

#include "usDeclare.h"

#include <QtCore/QSettings>

//! Retrieve project xml settings.
QSettings & xmlSettings(const QString &pathConfig = QString());

QString & buildDate();

#define RestartPermission UserSystem::Rewrite
#define RunScriptPermission UserSystem::Write
#define RebootPermission UserSystem::User1

#endif  // _MAIN_H__
