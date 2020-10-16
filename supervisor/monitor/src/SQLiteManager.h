#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QColor>
#include <QDateTime>

class SQLiteManager : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteManager(QObject *parent = 0);
    ~SQLiteManager();

    static QString databasePath(const QDate &date);

signals:
    void logEntry(QString host, QString service, QString message, QColor color, QDateTime date);

public slots:
    void writeLog(const QString &host, const QString &service, const QString &message, QColor color);
    void requestLog();

private slots:
    void switchLogDatabase();

private:
    void initNewDatabase();

private:
    QSqlDatabase db;
};

#endif // SQLITEMANAGER_H
