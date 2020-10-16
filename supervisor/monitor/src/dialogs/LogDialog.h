#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
    class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();

    void setLogText(const QStringList &log);
    void setTitle(const QString &title);

signals:
    void print();

protected:
    virtual void changeEvent(QEvent * event);
    QStringList filter(const QStringList &log);

protected slots:
    void onPrint();
    void showLog();

private:
    Ui::LogDialog *ui;
    QString mTitle;
    QStringList mLog;
};

#endif // LOGDIALOG_H
