#include "main.h"

#include "MainService.h"

#include "InitMain.h"
#include "version.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

int main(int argc, char *argv[])
{
    InitMain::setupCodec();

    QString fileName = QFileInfo(argv[0]).baseName();

    QCoreApplication::setApplicationName("netSnmpSubagent");
    QCoreApplication::setOrganizationName("ITOS");
    QCoreApplication::setApplicationVersion(QString("%1").arg(VERSION_FULL));

    MainService service(argc, argv, fileName);

    return service.exec();
}
