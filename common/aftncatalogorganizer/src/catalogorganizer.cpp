#include "catalogorganizer.h"

CatalogOrganizer::CatalogOrganizer(bool check)
    :mDirInc(""), mDirOut(""), bCheckNeeded(check)
{    
}

QString CatalogOrganizer::GetIncomingPath()
{
    QString path = mDirInc + GetTodaysPath();
    if(CreateIfNeeded(path))
        return path;
    return "";
}

QString CatalogOrganizer::GetOutgoingPath()
{
    QString path = mDirOut + GetTodaysPath();
    if(CreateIfNeeded(path))
        return path;
    return "";
}

void CatalogOrganizer::SetPaths(QString incoming, QString outgoing)
{
    mDirInc = incoming + (incoming.endsWith("/")?"":"/");
    mDirOut = outgoing + (outgoing.endsWith("/")?"":"/");
}

QString CatalogOrganizer::GetTodaysPath()
{
    QLocale enLocale = QLocale::c() ;
    QString month = enLocale.toString(QDateTime::currentDateTimeUtc(), "MMM-yy").toLower();
    QString day = QDateTime::currentDateTimeUtc().toString("Ddd").toLower();
    QString path = month + "/" + day + "/";
    return path;
}

bool CatalogOrganizer::CreateIfNeeded(QString path)
{
    if(bCheckNeeded)
    {
        QDir configPath(path);
        if(!configPath.exists())
        {
            QDir createPath;
            return createPath.mkpath(path);
        }
    }
    return true;
}
