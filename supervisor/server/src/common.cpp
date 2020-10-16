#include "common.h"
#include <QUuid>

#include <QtCore/QVariant>

Q_DECLARE_METATYPE(Status)
Q_DECLARE_METATYPE(Type)
Q_DECLARE_METATYPE(EFMTPStatus)

static int regType = qRegisterMetaType<Type>();
static int regStatus = qRegisterMetaType<Status>();
static int regEFMTPStatus = qRegisterMetaType<EFMTPStatus>();


QString getConnectionDbName()
{
    static const QString g_connectionDbName = QUuid::createUuid().toString();
    return g_connectionDbName;
}

QString toString(Type type)
{
    QString typeString;
    if ( CWP == type )
        typeString = "CWP";
    else if ( FDP == type )
        typeString = "FDP";
    else if ( FMTP == type )
        typeString = "FMTP";
    else if ( STCA == type )
        typeString = "STCA";
    else if ( AFTNWS == type )
        typeString = "AFTNWS";
    else if ( DBSYNC == type )
        typeString = "DBSYNC";
    else if ( PROCESS == type )
        typeString = "Process";
    else if ( SERVICE == type )
        typeString = "Service";
    else
        typeString = "Unknown";
    return typeString;
}
