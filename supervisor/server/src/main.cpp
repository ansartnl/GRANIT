#include "SupervisorService.h"

#include "InitMain.h"
#include "Log.h"
#include "init.h"

#include "version.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>

#if defined(SS_NO_GUI)
#include <QCoreApplication>
#include "qtsinglecoreapplication.h"
#else
#include <QApplication>
#include "qtsingleapplication.h"
#endif


int main(int argc, char *argv[])
{
    InitMain::setupCodec();

#if defined(SS_NO_GUI)
//    QCoreApplication a(argc, argv);
    QtSingleCoreApplication a(argc, argv);
#else
//    QApplication a(argc, argv);
    QtSingleApplication a(argc, argv);
#endif
    a.setApplicationName("SupervisorServer");
    a.setOrganizationName("ITOS");
    a.setApplicationVersion( VERSION_FULL );

    try
    {
        if(!a.isRunning())
        {
            SupervisorService service(argc, argv, QFileInfo(argv[0]).baseName());
            service.start();

            int result = a.exec();

            delete xmlSettings();

            return result;
        }
    }
    catch ( ... )
    {
    }
    return 0;
}
