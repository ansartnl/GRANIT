#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_ClientWindow.h"

class ClientWindow : public QMainWindow, private Ui::ClientWindow
{
    Q_OBJECT
public:
    explicit ClientWindow(QWidget *parent = 0);

private:
    void writeLog(const QString &level, const QString &text);

private slots:
    void onText(const QString &text);
    void onInfo(const QString &text);
    void onError(const QString &text);
};

#endif // MAINWINDOW_H
