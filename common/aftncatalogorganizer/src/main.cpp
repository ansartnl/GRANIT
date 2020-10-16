#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QtCore/QDir>
#include <QMessageBox>
#include <QFileDialog>
#include "CmdLineArguments.h"

QString findConfigFilePath(const QString &applicationDirPath, const QString &applicationName)
{
    QString configMask("%1/%2.xml");
    QString pathConfigFile = QString(configMask).arg(applicationDirPath).arg(applicationName);
    if ( !QFile::exists(pathConfigFile) )
    {
        QDir etcDir("etc:");
        if ( !etcDir.exists() )
            return QString();

        pathConfigFile = QString(configMask).arg(etcDir.absolutePath()).arg(applicationName);
        if ( !QFile::exists(pathConfigFile) )
        {
            pathConfigFile = QString(configMask).arg(etcDir.absolutePath()).arg("config");
            if ( !QFile::exists(pathConfigFile) )
                return QString();
        }
    }

    return pathConfigFile;
}

QString findConfigFilePath()
{
    // Initialize command line signleton.
    CmdLineArguments::instance(QCoreApplication::arguments());

    // Try to find configuration file in the command line arguments.
    QString pathConfigFile = CmdLineArguments::instance().config();
    if ( QFile::exists(pathConfigFile) )
        return pathConfigFile;

    // Search path to config by executable file name.
    pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                        QFileInfo(QCoreApplication::applicationFilePath()).baseName());

    // Search path to config by application name.
    if ( pathConfigFile.isEmpty() )
        pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                            QCoreApplication::applicationName());

    return pathConfigFile;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString pathConfig = findConfigFilePath();
    if ( pathConfig.isEmpty() )
    {
        QMessageBox mb;
        mb.setText("Configuration file not found!");
        mb.exec();

        QString fileName = QFileDialog::getOpenFileName(NULL,
                                                        QObject::tr("Config file..."),
                                                        QDir::currentPath(),
                                                        QObject::tr("XML Files (*.xml)"));
        if (fileName.isEmpty())
            return -1;
        xmlSettings(fileName);
    }
    else
        xmlSettings(pathConfig);

    MainWindow w;
    w.show();
    
    return a.exec();
}
