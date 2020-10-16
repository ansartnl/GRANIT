#ifndef _CWP_CONTROLLER_H__
#define _CWP_CONTROLLER_H__

#include <QtCore/QtGlobal>

#include "ProcessController.h"
#include "common.h"


//! CWP concrete-class process controller.
class CWPProcessController : public ProcessController
{
    Q_OBJECT
public:
    CWPProcessController();

    //! Type associated with this controller.
    virtual Type type() const;
    //! Current CWP client of this controller.
    inline QString client() const { return mClient; }
    //! Current CWP user of this controller.
    inline QString user() const { return mCurrentUser; }
    //! Current CWP sectors of this controller.
    inline QStringList sectors() const { return mSectors; }

    //! Overrides Controller::setConfigPath function.
    void setConfigPath(const QString &name);

signals:
    void cwpSectorsChanged(const QString &name, const QString &client, const QStringList &sectors);
    void cwpUserChanged(const QString &name, const QString &user);

protected:
    void timerEvent(QTimerEvent *);

protected slots:
    void onStatusChanged(Type, const QString &, Status status);

protected:
    int mTimerId;
    QString mClient;
    QString mCurrentUser;
    QStringList mSectors;
};

#endif // _CWP_CONTROLLER_H__
