#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QtCore/QMap>
#include <QtCore/QObject>

class LogController : public QObject
{
    Q_OBJECT
public:
    explicit LogController(QObject *parent = 0);

    void addLog(const QString &name, const QString &path);

    QStringList names() const;
    QStringList readLog(const QString &name, int countLines = 300) const;

protected:
    QStringList readStream(const QString &filePath, int countLines) const;
    QStringList readAllStream(const QString &filePath) const;
    QStringList packLines(QStringList &lines) const;

protected:
    QMap < QString, QString > mObjects;
};

#endif // LOGCONTROLLER_H
