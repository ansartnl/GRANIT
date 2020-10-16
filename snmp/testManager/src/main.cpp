#include "ManagerWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("testManager");

    ManagerWindow w;
    w.show();

    return a.exec();
}
