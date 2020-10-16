#ifndef _FMTP_CONTROLLERS_H__
#define _FMTP_CONTROLLERS_H__

#include <QtCore/QtGlobal>

#include "ProcessController.h"
#include "ServiceController.h"
#include "common.h"


//! FMTP controller handler.
class FMTPControllerHandler
{
public:
    FMTPControllerHandler(Controller *ctrl, const QString &dbConnectionName);

    //! Current FMTP status of this controller.
    EFMTPStatus fmtpStatus() const;

    //! Set configuration name.
    void setConfigName(const QString &name);

    //! Create database clone if necessary.
    bool cloneDatabase();

protected:
    //! Retrieve list of FMTP status codes from database.
    QList<EFMTPStatus> statusCodes() const;

    QString mStatusGiver;
    EFMTPStatus mFmtpStatus;

    /** Connection name to database associated with controller */
    QString     mDbConnectionName;
    /** Local connection to database (main thread connection name) */
    QString     mLocalDbConnectionName;

    /** Controller reference */
    Controller  *mController;
};


//! FMTP concrete-class process controller.
class FMTPProcessController : public ProcessController, public FMTPControllerHandler
{
    Q_OBJECT
public:
    FMTPProcessController(const QString &dbConnectionName);

    //! Type associated with this controller.
    virtual Type type() const;

signals:
    void fmtpStatusChanged(const QString &name, EFMTPStatus statmLocalDbConnectionNameus);

protected slots:
    void onStatusChanged(Type , const QString &, Status status);

protected:
    void timerEvent(QTimerEvent *);

private:
    int mTimerId;
};


//! FMTP concrete-class service controller.
class FMTPServiceController : public ServiceController, public FMTPControllerHandler
{
    Q_OBJECT
public:
    FMTPServiceController(const QString &dbConnectionName);
    //! Type associated with this controller.
    virtual Type type() const;

signals:
    void fmtpStatusChanged(const QString &name, EFMTPStatus status);

protected slots:
    void onStatusChanged(Type , const QString &, Status status);

protected:
    void timerEvent(QTimerEvent *);

private:
    int mTimerId;
};

#endif // _FMTP_CONTROLLERS_H__
