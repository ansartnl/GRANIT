#ifndef SCRIPTMIBOBJECT_H
#define SCRIPTMIBOBJECT_H

#include "BaseMibObject.h"

#include "ScriptRunner.h"

#include <QtCore/QScopedPointer>

class ScriptRunner;

class ScriptMibObject : public BaseMibObject
{
    Q_OBJECT
public:
    ScriptMibObject(const QString &name_, const QString &oidString_
                    , const QString &path_, const QString &args_);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

    enum RunScript {
        RS_STOPPED = 1
        , RS_RUN = 2
        , RS_RUNNING = 3
    };

protected:
    void init();

    void nodeHandler(netsnmp_mib_handler *, netsnmp_handler_registration *
                     , netsnmp_agent_request_info *, variable_list *);

private slots:
    void onScriptFinished();
    void onScriptError(const QString &text);

private:
    QScopedPointer < ScriptRunner > mScriptRunner;
    int mValue;
};

#endif // SCRIPTMIBOBJECT_H
