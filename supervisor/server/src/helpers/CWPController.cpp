#include "CWPController.h"

#include "Log.h"
#include "qtxmlsettings.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>


/////////////////////////////////////////////////////////////////////////////////
// CWPProcessController implementation.

CWPProcessController::CWPProcessController()
    : mTimerId(0)
{
    connect(this, SIGNAL(statusChanged(Type,QString,Status))
            , this, SLOT(onStatusChanged(Type,QString,Status)));
}

void CWPProcessController::setConfigPath(const QString &name)
{
    Controller::setConfigPath(name);

    QtXmlSettings settings(name);
    mClient = settings.value("oldi/owner_name").toString();
}

void CWPProcessController::timerEvent(QTimerEvent *)
{
    QtXmlSettings settings(configPath());
    QString currentUser = settings.value("users/current_user").toString();
    if (currentUser != mCurrentUser)
    {
        mCurrentUser = currentUser;
        emit cwpUserChanged(name(), user());
    }
    QStringList list = QStringList() << settings.value("users/current_sector").toString();
    if (list != mSectors)
    {
        mSectors = list;
        emit cwpSectorsChanged(name(), client(), sectors());
    }
}

void CWPProcessController::onStatusChanged(Type , const QString &, Status status)
{
    if (status == Started)
    {
        if ( mTimerId )
            killTimer(mTimerId);
        mTimerId = startTimer(1000);
    } else
    {
        killTimer(mTimerId);
        mTimerId = 0;
    }
}

Type CWPProcessController::type() const
{
    return CWP;
}
