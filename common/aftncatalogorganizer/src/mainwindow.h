#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtxmlsettings.h"
#include "catalogorganizer.h"

QSettings * xmlSettings(const QString &pathConfig = NULL);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    //! reads parameters from XML file
    void init();

    Ui::MainWindow *ui;

    QString mIncPath;
    QString mOutPath;
    CatalogOrganizer organizer;
};

#endif // MAINWINDOW_H
