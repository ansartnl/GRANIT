#ifndef INIT_H
#define INIT_H

#include <QtCore/QSettings>
#include <QtCore/QMap>
class QObject;


void initLog4Qt(QObject *parent = 0);

QSettings* xmlSettings(const QString &pathFile = QString());

//! Convert string format "key1=value1;key2=value2;..." to map
QMap<QString, QString> toMap(const QString &str);
//! Pack values from map to string fromat "key1=value1;key2=value2;..."
QString toString(const QMap<QString, QString> &vals);

#endif // INIT_H
