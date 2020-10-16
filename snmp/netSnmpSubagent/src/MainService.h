#ifndef MAINSERVICE_H
#define MAINSERVICE_H

#if defined (Q_OS_WIN) && !defined(_MSC_VER)
#include "qtservice.h"
#else
#  ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "qtservice.h"
#  else
#include <QtService/QtServiceBase>
#  endif
#endif

#include <QtCore/QObject>

class ServiceStarter;

class MainService : public QObject, public QtService < QCoreApplication >
{
    Q_OBJECT
public:
    MainService(int argc, char** argv, const QString &name);
    virtual ~MainService();

protected:
    void createApplication(int &argc, char **argv);
    void start();
    void stop();

private:
    bool loadConfig();
    void createDefaultConfig();

    void initLog4Qt();

private:
    ServiceStarter *mServiceStater;
};

#endif // MAINSERVICE_H
